/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2018 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner
 * unless Synaptics has otherwise provided express, written
 * permission.
 *
 * Use of the materials may require a license of intellectual property
 * from a third party or from Synaptics. This file conveys no express
 * or implied licenses to any intellectual property rights belonging
 * to Synaptics.
 *
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
 * SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 * INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
 * BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
 * COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */
#include "apb_perf_base.h"
#include "debug.h"
#include "spi_driver.h"
#include "util.h"
#include "galois_speed.h"

// polling mode vs. interrupt mode
#define POLLING_MODE

// number of SPI masters in system
#if defined(BERLIN)
#define SPI_MASTER_NUM	2
#else
#define SPI_MASTER_NUM	3
#endif

// SSI system clock
// SPI output clock can be calculated as: SPI_SYS_CLOCK/SPI_REG_BAUD
#if defined(BERLIN)
	#if defined(FPGA)
		#define SPI_SYS_CLOCK	1500 // 1.5MHz
		#define SPI_SM_CLOCK	9000 // 9MHz
	#else
		#define SPI_SYS_CLOCK   75000 // 75MHz, it uses config clock
		#define SPI_SM_CLOCK    25000 // 25MHz
	#endif
#else
	#if defined(SOC_ES1)
		#define SPI_SYS_CLOCK   100000 // 100 MHz
	#else
		#define SPI_SYS_CLOCK	5000 // 5MHz
	#endif
#endif

// SPI master TX/RX FIFO size
#define SPI_TX_FIFO_SIZE	64
#define SPI_RX_FIFO_SIZE	64

// register definitions of SPI master
#define SPI_REG_CTRL0	0x00
#define SPI_REG_CTRL1	0x04
#define SPI_REG_SSIENR	0x08
#define SPI_REG_MWCR	0x0C
#define SPI_REG_SER	0x10
#define SPI_REG_BAUD	0x14
#define SPI_REG_TXFTLR	0x18
#define SPI_REG_RXFTLR	0x1C
#define SPI_REG_TXFLR	0x20
#define SPI_REG_RXFLR	0x24
#define SPI_REG_SR	0x28
#define SPI_REG_IMR	0x2C
#define SPI_REG_ISR	0x30
#define SPI_REG_RISR	0x34
#define SPI_REG_TXOICR	0x38
#define SPI_REG_RXOICR	0x3C
#define SPI_REG_RXUICR	0x40
#define SPI_REG_MSTICR	0x44
#define SPI_REG_ICR	0x48
#define SPI_REG_DMACR	0x4C
#define SPI_REG_DMATDLR	0x50
#define SPI_REG_DMARDLR	0x54
#define SPI_REG_IDR	0x58
#define SPI_REG_DR	0x60

// interrupt masks
#define IMR_MASK_RXF	0x10
#define IMR_MASK_RXO	0x08
#define IMR_MASK_RXU	0x04
#define IMR_MASK_TXO	0x02
#define IMR_MASK_TXE	0x01

// interrupt status
#define ISR_STAT_RXF	0x10
#define ISR_STAT_RXO	0x08
#define ISR_STAT_RXU	0x04
#define ISR_STAT_TXO	0x02
#define ISR_STAT_TXE	0x01

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)>(b))?(b):(a))
#endif

// Macros of read/write SPI master registers
#define SPI_RegRead(a, pv)	REG_READ32(base_addr+(a), pv) /*(*(volatile int *)(base_addr+(a)))*/
#define SPI_RegWrite(a, v)	REG_WRITE32(base_addr+(a), v) /**(volatile int *)(base_addr+(a)) = (v)*/

// SPI master's transfer buffer, shared by TX and RX.
// spi_master_buffer[][0] is the number of valid TX/RX frames in buffer
int spi_master_buffer[SPI_MASTER_NUM][256+5];
// number of receive frames
int spi_master_rx_number[SPI_MASTER_NUM];
// transmit done flag
volatile int spi_master_tx_done[SPI_MASTER_NUM];
// receive done flag
volatile int spi_master_rx_done[SPI_MASTER_NUM];

// SPI master's base-address vector
#if defined(BERLIN)
// We actually have only two SPI, but the driver always assume that
// SPI_BUS 0 = APB_SSI_INST0
// SPI_BUS 1 = APB_SSI_INST1
// SPI_BUS 2 = APB_SSI_INST2
const unsigned int SPI_MASTER_BASEADDR[SPI_MASTER_NUM] = {APB_SSI_INST1_BASE, APB_SSI_INST2_BASE};
#else
const unsigned int SPI_MASTER_BASEADDR[SPI_MASTER_NUM] = {APB_SSI_INST0_BASE, APB_SSI_INST1_BASE, APB_SSI_INST2_BASE};
#endif


/*************************************************
 * set transfer mode.
 * mode = 0: full-duplex transfer mode
 *        1: transmit only
 *        2: receive only
 *        3: half-duplex tranfer mode
 *
 *************************************************/
static void set_transfer_mode(int master_id, int mode)
{
	unsigned int base_addr;
	int value;

	if (mode<0 || mode>3) return;

	base_addr = SPI_MASTER_BASEADDR[master_id];

	SPI_RegRead(SPI_REG_CTRL0, &value);
	value &= 0xfcff;
	value |= (mode<<8);
	SPI_RegWrite(SPI_REG_CTRL0, value);

	return;
}

/*************************************************
 * set SSI protocol .
 * protocol = 0: Motorola SPI
 *            1: TI SSP
 *            2: National Semiconductor Microwire
 *
 *************************************************/
static void set_ssi_protocol(int master_id, int protocol)
{
	unsigned int base_addr;
	int value;

	if (protocol<0 || protocol>2) return;

	base_addr = SPI_MASTER_BASEADDR[master_id];

	SPI_RegRead(SPI_REG_CTRL0, &value);
	value &= 0xffcf;
	value |= (protocol<<4);
	SPI_RegWrite(SPI_REG_CTRL0, value);

	return;
}
/*************************************************
 * set SPI master TX FIFO underflow threshold .
 * thrld = 0-255
 *
 *************************************************/
static void set_txfifo_threshold(int master_id, int thrld)
{
	unsigned int base_addr;

	if (thrld<0 || thrld>255) return;

	base_addr = SPI_MASTER_BASEADDR[master_id];

	SPI_RegWrite(SPI_REG_TXFTLR, thrld);

	return;
}

/*************************************************
 * set SPI master RX FIFO overflow threshold .
 * thrld = 1-256
 *
 *************************************************/
static void set_rxfifo_threshold(int master_id, int thrld)
{
	unsigned int base_addr;

	if (thrld<1 || thrld>256) return;

	base_addr = SPI_MASTER_BASEADDR[master_id];

	SPI_RegWrite(SPI_REG_RXFTLR, thrld-1);

	return;
}

/*****************************************************
 * select slave device
 * flag: 1 - select
 *       0 - de-select
 *****************************************************/
static void select_slave_device(int master_id, int dev_id, int flag)
{
	unsigned int base_addr;

	base_addr = SPI_MASTER_BASEADDR[master_id];

	if (flag)
		SPI_RegWrite(SPI_REG_SER, 1<<dev_id);
	else
		SPI_RegWrite(SPI_REG_SER, 0);

	return;
}

/*****************************************************
 * judge if a master is idle, it should be called
 * before you init the master.
 *****************************************************/
int diag_spi_master_is_idle(int master_id)
{
	unsigned int base_addr;
	int i;

	if (master_id<0 || master_id>SPI_MASTER_NUM-1)
		return (SSI_EBADPARAM);

	base_addr = SPI_MASTER_BASEADDR[master_id];

	// disable SPI
	SPI_RegRead(SPI_REG_SSIENR, &i);
	if (i)
		return (SSI_EBUSY);
	return (SSI_OK);
}

/*******************************************************************
 * FUNCTION: Initialize SSI master.
 *           More than one SSI masters could exist in system.
 * PARAMETERS: master_id - ID of SSI master to initialize
 * RETURN: SSI_OK - succeed
 *         SSI_EBADPARAM - invalid parameters
 *******************************************************************/
int diag_spi_master_init(int master_id)
{
	unsigned int base_addr;

	if (master_id<0 || master_id>SPI_MASTER_NUM-1) return (SSI_EBADPARAM);

	base_addr = SPI_MASTER_BASEADDR[master_id];

	// disable SPI
	SPI_RegWrite(SPI_REG_SSIENR, 0);

	// disable all interrupts
	SPI_RegWrite(SPI_REG_IMR, 0);

	// configure SSI as a SPI master device
	set_ssi_protocol(master_id, 0); // SPI protocol
	set_transfer_mode(master_id, 0); // duplex transfer mode

	// global variables for SPI master
	spi_master_rx_number[master_id] = 0;
	spi_master_tx_done[master_id] = 0;
	spi_master_rx_done[master_id] = 0;

	return (SSI_OK);
}

/*******************************************************************
 * FUNCTION: disable SSI master.
 *           More than one SSI masters could exist in system.
 * PARAMETERS: master_id - ID of SSI master to initialize
 * RETURN: SSI_OK - succeed
 *         SSI_EBADPARAM - invalid parameters
 *******************************************************************/
int diag_spi_master_disable(int master_id)
{
	unsigned int base_addr;

	if (master_id<0 || master_id>SPI_MASTER_NUM-1) return (SSI_EBADPARAM);

	base_addr = SPI_MASTER_BASEADDR[master_id];

	// disable SPI
	SPI_RegWrite(SPI_REG_SSIENR, 0);

	return (SSI_OK);
}

/*******************************************************************
 * FUNCTION: enable SSI master.
 *           More than one SSI masters could exist in system.
 * PARAMETERS: master_id - ID of SSI master to initialize
 * RETURN: SSI_OK - succeed
 *         SSI_EBADPARAM - invalid parameters
 *******************************************************************/
int diag_spi_master_enable(int master_id)
{
	unsigned int base_addr;

	if (master_id<0 || master_id>SPI_MASTER_NUM-1) return (SSI_EBADPARAM);

	base_addr = SPI_MASTER_BASEADDR[master_id];

	// enable SPI
	SPI_RegWrite(SPI_REG_SSIENR, 1);

	return (SSI_OK);
}

/*******************************************************************
 * FUNCTION: Set SPI master clock mode.
 * PARAMETERS: master_id - ID of SSI master to config
 *             mode - 0: polarity 0 & phase 0
 *                    1: polarity 0 & phase 1
 *                    2: polarity 1 & phase 0
 *                    3: polarity 1 & phase 1
 * RETURN: >=0 - master clock mode
 *         SSI_EBADPARAM - invalid parameters
 *         SSI_EBUSY - master is busy
 *******************************************************************/
int diag_spi_master_set_clock_mode(int master_id, int mode)
{
	unsigned int base_addr;
	int value;

	if (master_id<0 || master_id>SPI_MASTER_NUM-1) return (SSI_EBADPARAM);

	if (mode<0 || mode>3) return(SSI_EBADPARAM);

	base_addr = SPI_MASTER_BASEADDR[master_id];

	// check status first
	SPI_RegRead(SPI_REG_SR, &value);
	if (value&0x01) return (SSI_EBUSY);

	// config SPI master clock mode
	SPI_RegRead(SPI_REG_CTRL0, &value);
	value &= 0xff3f;
	value |= (mode<<6);
	SPI_RegWrite(SPI_REG_CTRL0, value);

	//return (mode);
	return (SSI_OK);
}

extern UINT32 GaloisGetFrequency(int FreqID);

/*******************************************************************
 * FUNCTION: Set SPI master speed.
 * PARAMETERS: master_id - ID of SSI master to config
 *             speed - master speed in unit of KHz
 * RETURN: >0 - master speed
 *         SSI_EBADPARAM - invalid parameters
 *         SSI_EBUSY - master is busy
 *******************************************************************/
int diag_spi_master_set_speed(int master_id, int speed)
{
	unsigned int base_addr;
	int value;

	if (master_id<0 || master_id>SPI_MASTER_NUM-1) return (SSI_EBADPARAM);

	if (speed<=0) return(SSI_EBADPARAM);

	base_addr = SPI_MASTER_BASEADDR[master_id];

	// check status first
	SPI_RegRead(SPI_REG_SR, &value);
	if (value&0x01) return (SSI_EBUSY);
#if defined(BERLIN)
	{
		unsigned base_clk;
		if (base_addr == APB_SSI_INST1_BASE)
			//base_clk = SPI_SYS_CLOCK;
			//base_clk = get_cfgclk() * 1000;
			base_clk = GaloisGetFrequency(SOC_FREQ_CFG) * 1000;
		else if (base_addr == APB_SSI_INST2_BASE)
			base_clk = SPI_SM_CLOCK;
		// config SPI master speed
		if (speed>base_clk) value = 1;
		else value = (base_clk+speed-1)/speed;

		SPI_RegWrite(SPI_REG_BAUD, value);

		dbg_printf(PRN_INFO, "SPI speed set to %fkHz\n", (float)base_clk/(value&0xfffe));
		//return (base_clk/value);
		return (SSI_OK);
	}
#else
	// config SPI master speed
	if (speed>SPI_SYS_CLOCK) value = 1;
	else value = (SPI_SYS_CLOCK+speed-1)/speed;

	SPI_RegWrite(SPI_REG_BAUD, value);

	return (SPI_SYS_CLOCK/value);
#endif
}

/*******************************************************************
 * FUNCTION: Set SPI master data frame width.
 * PARAMETERS: master_id - ID of SSI master to config
 *             width - data frame width in unit of bit
 * RETURN: >0 - master data frame width
 *         SSI_EBADPARAM - invalid parameters
 *         SSI_EBUSY - master is busy
 * NOTE: valid frame width is 4~16, while most SPI device work
 *       in 8-bit frame mode.
 *******************************************************************/
int diag_spi_master_set_framewidth(int master_id, int width)
{
	unsigned int base_addr;
	int value;

	if (master_id<0 || master_id>SPI_MASTER_NUM-1) return (SSI_EBADPARAM);

	if (width<4 || width>16) return(SSI_EBADPARAM);

	base_addr = SPI_MASTER_BASEADDR[master_id];

	// check status first
	SPI_RegRead(SPI_REG_SR, &value);
	if (value&0x01) return (SSI_EBUSY);

	// config SPI master data frame width
	SPI_RegRead(SPI_REG_CTRL0, &value);
	value &= 0xfff0;
	value |= (width-1);
	SPI_RegWrite(SPI_REG_CTRL0, value);

	//return (width);
	return (SSI_OK);
}

/*************************************************************************
 * FUNCTION: Select a SPI slave device to operate.
 * PARAMETERS: master_id - ID of SSI master
 *             dev_id - ID of SPI slave device
 *             flag - 1: select
 *                    0: de-select
 * RETURN: >=0 - ID of slave device selected
 *         SSI_EBADPARAM - invalid parameters
 * NOTE: this is a fake function, only used to validate the device id
 *       we keep this function is to be compatable with the middleware
 ************************************************************************/
int diag_spi_master_device_select(int master_id, int dev_id, int flag)
{

	if (master_id<0 || master_id>SPI_MASTER_NUM-1) return (SSI_EBADPARAM);

	if (dev_id<0 || dev_id>16) return(SSI_EBADPARAM);

	//return (dev_id);
	return (SSI_OK);
}

/*******************************************************************
 * FUNCTION: half-duplex read/write frames.
 * PARAMETERS: master_id - ID of SSI master
 *             pwbuf - pointer to transmit data buffer
 *             wnum - number of frames to transmit
 *             prbuf - pointer to receive data buffer
 *             rnum - number of frames to receive
 *			   eeprom - flag when both read and write buffers have data
 *                           if 1, SPI is set to eeprom mode, first tx then rx
 *                           if 0, SPI tx and rx happens at the same time
 * RETURN: SSI_OK - transfer succeed
 *         SSI_EBADPARAM - invalid parameters
 *         SSI_EBUSY - master is busy
 *         SSI_EBUSRW - master read/write fail
 * NOTE: the maximum value of wnum is 256+4,
 *       while the maximum value of rnum is 256
 *******************************************************************/
int diag_spi_master_writeread_frames(int master_id, int dev_id, int *pwbuf, int wnum, int *prbuf, int rnum, int eeprom)
{
	unsigned int base_addr;
	int value;
#ifndef POLLING_MODE // interrupt mode
	int irqs;
#endif
	int i, j;

	if (master_id<0 || master_id>SPI_MASTER_NUM-1) return (SSI_EBADPARAM);
	if (wnum>260 || rnum>256) return (SSI_EBADPARAM);

	base_addr = SPI_MASTER_BASEADDR[master_id];

	// check status first
	SPI_RegRead(SPI_REG_SR, &value);
	if (value&0x01) return (SSI_EBUSY);



	// setup transfer
	if ((pwbuf && wnum>0) && (prbuf && rnum>0)){ // duplex write, read transfer
		// set transfer mode to half-duplex
		if(eeprom)
			set_transfer_mode(master_id, 3); //eeprom mode
		else
			set_transfer_mode(master_id, 0); //full-duplex mode

		// set number of read frames
		SPI_RegWrite(SPI_REG_CTRL1, rnum-1);
		spi_master_rx_number[master_id] = rnum;

		// set TX fifo threshold
		if (wnum>SPI_TX_FIFO_SIZE)
			set_txfifo_threshold(master_id, SPI_TX_FIFO_SIZE/2);
		else
			set_txfifo_threshold(master_id, 0);

		// set RX fifo threshold
		if (rnum>SPI_RX_FIFO_SIZE)
			set_rxfifo_threshold(master_id, SPI_RX_FIFO_SIZE/2);
		else
			set_rxfifo_threshold(master_id, rnum);

		// enable SPI
		SPI_RegWrite(SPI_REG_SSIENR, 1);

		// write TX FIFO
		for (i=0; i<MIN(wnum,SPI_TX_FIFO_SIZE); i++)
			SPI_RegWrite(SPI_REG_DR, pwbuf[i]);

		// write the remaining TX frames into master TX/RX buffer
		if (wnum>i){
			spi_master_buffer[master_id][0] = wnum - i;
			j = wnum-i;
			do{ spi_master_buffer[master_id][j--] = pwbuf[i++];
			} while (i<wnum);
		} else spi_master_buffer[master_id][0] = 0;

		// initiate tx_done and rx_done flags
		spi_master_tx_done[master_id] = 0;
		spi_master_rx_done[master_id] = 0;

#ifndef POLLING_MODE // interrupt mode
		// if number of write frames is larger than TX fifo size,
		// unmask TX empty interrupt
		irqs = 0;
		if (wnum>SPI_TX_FIFO_SIZE)
			irqs |= IMR_MASK_TXE;
		// if number of read frames is larger than RX fifo size,
		// unmask RX full interrupt
		if (rnum>SPI_RX_FIFO_SIZE)
			irqs |= IMR_MASK_RXF;
		SPI_RegWrite(SPI_REG_IMR, irqs);
#endif

		// select slave device to start transfer
		select_slave_device(master_id, dev_id, 1);

#ifndef POLLING_MODE // interrupt mode
		// wait until RX finish
		if (irqs & IMR_MASK_RXF) { /* RX interrupt wait required */

			while (!spi_master_rx_done[master_id]);

		} else { /* no RX interrupt wait required */
		    do{
			    SPI_RegRead(SPI_REG_SR, &i);
		    } while (!(i&0x08)); /* RX fifo is empty */

			do {
				SPI_RegRead(SPI_REG_SR, &i);
			} while (i&0x01); /* RX transaction is in process */

			/* read data from RX fifo */
            SPI_RegRead(SPI_REG_RXFLR, &i);
            for (j=0; j<i; j++) {
		        SPI_RegRead(SPI_REG_DR, &(spi_master_buffer[master_id][j+1]));
			}
			spi_master_buffer[master_id][0] = i;
		}
#else // polling mode

        i = 0;
        while (i<rnum) {
		    do{
			    SPI_RegRead(SPI_REG_SR, &j);
		    } while (!(j&0x08));

		    SPI_RegRead(SPI_REG_SR, &j);
		    if( j&0x10 ) break;

            SPI_RegRead(SPI_REG_RXFLR, &j);
            while (j--){
		        SPI_RegRead(SPI_REG_DR, &(spi_master_buffer[master_id][i+1]));
                i ++;
            }
	    }
	    spi_master_buffer[master_id][0] = i;

#endif
		// copy out the received frames in buffer
		if (spi_master_buffer[master_id][0]!=rnum){
			// SPI I/O error!!!
			// de-select slave device
			select_slave_device(master_id, dev_id, 0);
			// disable SPI
			SPI_RegWrite(SPI_REG_SSIENR, 0);

			return (SSI_EBUSRW);
		}
		for (i=0; i<rnum; i++)
			prbuf[i] = spi_master_buffer[master_id][i+1];

	} else if (pwbuf && wnum>0){ // write-only transfer
		// set transfer mode to transmit-only
		set_transfer_mode(master_id, 1);

		// set TX fifo threshold
		if (wnum>SPI_TX_FIFO_SIZE)
			set_txfifo_threshold(master_id, SPI_TX_FIFO_SIZE/2);
		else
			set_txfifo_threshold(master_id, 0);

		// enable SPI
		SPI_RegWrite(SPI_REG_SSIENR, 1);

		// write TX FIFO
		for (i=0; i<MIN(wnum,SPI_TX_FIFO_SIZE); i++)
			SPI_RegWrite(SPI_REG_DR, pwbuf[i]);

		// write the remaining TX frames into master TX/RX buffer
		if (wnum>i){
			spi_master_buffer[master_id][0] = wnum - i;
			j = wnum-i;
			do{ spi_master_buffer[master_id][j--] = pwbuf[i++];
			} while (i<wnum);
		} else spi_master_buffer[master_id][0] = 0;

		// initiate tx_done flags
		spi_master_tx_done[master_id] = 0;

#ifndef POLLING_MODE // interrupt mode
		// if number of write frames is larger than TX fifo size,
		// unmask TX empty interrupt
		irqs = 0;
		if (wnum>SPI_TX_FIFO_SIZE)
			irqs |= IMR_MASK_TXE;
		SPI_RegWrite(SPI_REG_IMR, irqs);
#endif

		// select slave device to start transfer
		select_slave_device(master_id, dev_id, 1);

#ifndef POLLING_MODE // interrupt mode
		// wait until TX finish
		if (irqs & IMR_MASK_TXE) { /* TX interrupt wait required */

			while (!spi_master_tx_done[master_id]);

		} else { /* no TX interrupt wait required */

			/* wait until TX fifo is empty */
			do {
				SPI_RegRead(SPI_REG_SR, &j);
			} while (!(j&0x04)); /* TX fifo is not empty */

		}

		// wait until the shift register is flushed
		do {
			SPI_RegRead(SPI_REG_SR, &i);
		} while (i&0x01);

#else // polling mode
	    i = spi_master_buffer[master_id][0];
	    while (i>0) {
		    do {
			    SPI_RegRead(SPI_REG_SR, &j);
		    } while (!(j&0x02)); /* TX fifo is full */

		    SPI_RegRead(SPI_REG_SR, &j);
		    if( j&0x04 ) break; /* TX fifo is empty */

		    SPI_RegRead(SPI_REG_TXFLR, &j);
		    j = MIN(SPI_TX_FIFO_SIZE-j, i);
		    while (j-->0) {
			SPI_RegWrite(SPI_REG_DR, spi_master_buffer[master_id][i--]);
		    }
	    }
	    spi_master_buffer[master_id][0] = i;

	    do {
		    SPI_RegRead(SPI_REG_SR, &j);
	    } while (!(j&0x04)); /* TX fifo is not empty */

	    do {
		    SPI_RegRead(SPI_REG_SR, &j);
	    } while (j&0x01); /* SSI is transfering */

		if (spi_master_buffer[master_id][0]!=0){
			// SPI I/O error!!!
			// de-select slave device
			select_slave_device(master_id, dev_id, 0);
			// disable SPI
			SPI_RegWrite(SPI_REG_SSIENR, 0);

			return (SSI_EBUSRW);
		}
#endif

	} else if (prbuf && rnum>0){ // read-only transfer
		// set transfer mode to receive-only
		set_transfer_mode(master_id, 2);

		// set number of read frames
		SPI_RegWrite(SPI_REG_CTRL1, rnum-1);
		spi_master_rx_number[master_id] = rnum;

		// set RX fifo threshold
		if (rnum>SPI_RX_FIFO_SIZE)
			set_rxfifo_threshold(master_id, SPI_RX_FIFO_SIZE/2);
		else
			set_rxfifo_threshold(master_id, rnum);

		// enable transfer
		SPI_RegWrite(SPI_REG_SSIENR, 1);

		// reset RX buffer
		spi_master_buffer[master_id][0] = 0;

		// initiate rx_done flags
		spi_master_rx_done[master_id] = 0;

		// write TX FIFO to start transfer
		SPI_RegWrite(SPI_REG_DR, 0);

#ifndef POLLING_MODE // interrupt mode
		// if number of read frames is larger than RX fifo size,
		// unmask RX full interrupt
		irqs = 0;
		if (rnum>SPI_RX_FIFO_SIZE)
			irqs |= IMR_MASK_RXF;
		SPI_RegWrite(SPI_REG_IMR, irqs);
#endif

		// select slave device to start transfer
		select_slave_device(master_id, dev_id, 1);

#ifndef POLLING_MODE // interrupt mode
		// wait until RX finish
		if (irqs & IMR_MASK_RXF) { /* RX interrupt wait required */

			while (!spi_master_rx_done[master_id]);

		} else { /* no RX interrupt wait required */
		    do{
			    SPI_RegRead(SPI_REG_SR, &i);
		    } while (!(i&0x08)); /* RX fifo is empty */

			do {
				SPI_RegRead(SPI_REG_SR, &i);
			} while (i&0x01); /* RX transaction is in process */

			/* read data from RX fifo */
            SPI_RegRead(SPI_REG_RXFLR, &i);
            for (j=0; j<i; j++) {
		        SPI_RegRead(SPI_REG_DR, &(spi_master_buffer[master_id][j+1]));
			}
			spi_master_buffer[master_id][0] = i;
		}
#else // polling mode
	    for (i=0; i<rnum; i++) {
		    do {
			    SPI_RegRead(SPI_REG_SR, &j);
		    } while (!(j&0x08));

		    SPI_RegRead(SPI_REG_SR, &j);
		    if( j&0x10 ) break;

			SPI_RegRead(SPI_REG_DR, &(spi_master_buffer[master_id][i+1]));
	    }
	    spi_master_buffer[master_id][0] = i;
#endif

		// copy out the received frames in buffer
		if (spi_master_buffer[master_id][0]!=rnum){
			// SPI I/O error!!!
			// de-select slave device
			select_slave_device(master_id, dev_id, 0);
			// disable SPI
			SPI_RegWrite(SPI_REG_SSIENR, 0);

			return (SSI_EBUSRW);
		}
		for (i=0; i<rnum; i++)
			prbuf[i] = spi_master_buffer[master_id][i+1];
	}

	// de-select slave device
	select_slave_device(master_id, dev_id, 0);
	// disable SPI
	SPI_RegWrite(SPI_REG_SSIENR, 0);

	return (SSI_OK);
}

#define SPI_MAX_BUF_SIZE 128

//static int rbuf[SPI_MAX_BUF_SIZE];
static int wbuf[SPI_MAX_BUF_SIZE];
static unsigned int bit_to_frame[16] =
{
/*
	0x8888,
	0x888E,
	0x88E8,
	0x88EE,
	0x8E88,
	0x8E8E,
	0x8EE8,
	0x8EEE,
	0xE888,
	0xE88E,
	0xE8E8,
	0xE8EE,
	0xEE88,
	0xEE8E,
	0xEEE8,
	0xEEEE,
*/
//reverse with inverter on board
	0x7777,
	0x7771,
	0x7717,
	0x7711,
	0x7177,
	0x7171,
	0x7117,
	0x7111,
	0x1777,
	0x1771,
	0x1717,
	0x1711,
	0x1177,
	0x1171,
	0x1117,
	0x1111

};

static void spi_one_wire_construct_frame(unsigned int data) 	//7 frames 14bytes
{
	unsigned int i = 0, temp;
	//reversed data with inverter on SDI pin
	wbuf[0] = 0xFFFF;
	wbuf[1] = 0xFFFF;

	if((data & 0x8000) != 0)//start bit + bit 0
		wbuf[2] = 0xFF11;
	else
		wbuf[2] = 0xFF17;

	data = (data << 1) | 1;//stop bit

	for(i = 0; i < 4; i++)
	{
		temp = (data & 0xF000) >> 12;
		wbuf[3 + i] = bit_to_frame[temp];
		data <<= 4;

	}

	wbuf[7] = 0;//dummy write drive to high on SDI pin.(after inverter on board)

}

unsigned int spi_one_wire_write_frame(unsigned int data, unsigned int master_id, unsigned int slave_id)
{
	unsigned int iResult = 0;

	iResult = diag_spi_master_set_clock_mode(master_id, 3);
	iResult |= diag_spi_master_set_framewidth(master_id, 16);
	iResult |= diag_spi_master_init(master_id);
	iResult |= diag_spi_master_set_speed(master_id, 4500);//around 4.17MHz

	if(iResult)
	{
		dbg_printf(PRN_ERR,"SPI Master %d init fail !\n",master_id);
		return 1;
	}

	spi_one_wire_construct_frame(data);

	iResult = diag_spi_master_writeread_frames(master_id, slave_id, wbuf, 8, 0, 0, 0);

	if (iResult)
	{
		dbg_printf(PRN_RES," SPI write Failed\n");
		return 1;
	}
	return 0;
}

