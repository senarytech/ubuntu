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
#include "com_type.h"
#include "memmap.h"
#include "global.h"
#include "apb_perf_base.h"
#include "apb_gpio.h"
#include "util.h"
#include "gpio.h"
#include "debug.h"

#define ONEWIRE_GPIO_PORT_NUM   16
#define DELAY_LONG      26
#define DELAY_SHORT     5

#define GPIO_DDR_REG    (APB_GPIO_INST2_BASE + APB_GPIO_SWPORTA_DDR)
#define GPIO_DR_REG     (APB_GPIO_INST2_BASE + APB_GPIO_SWPORTA_DR)

#define PG877_PREAMBLE  0x152
#define PG877_DATA_BIT  18
#define PG877_DEFAULT_DATA_FIELD 0x24
#define PG877_PWM_MODE 0x5050

int g_dr_high, g_dr_low;

static inline void delay1ns()
{

        __asm__ __volatile__ (
                "MOV r0, r0"	"\n\t"
				:
				:
				:"r0"
       ) ;

}
extern unsigned int spi_one_wire_write_frame(unsigned int data,
				unsigned int master_id, unsigned int slave_id);
//writing a 16 bit frame out, with start and stop bits, total 18 bits on wire
//For 88PG847, the 16 bits are:
//bit[15,12]: chip select
//bit[11,8]: reg addr
//bit[7, 0]: data field
static void one_wire_write_frame(unsigned int value, unsigned int bit)
{
	unsigned int mask=1<<bit;
	int i=0;
	int j=0;

    mask >>=1;
	while(mask) {
		if(value&mask) {
            // "1" pulse
            //high
			REG_WRITE32(GPIO_DR_REG, g_dr_high);
			for(i=0; i<DELAY_LONG; i++)
				delay1ns();

			//low
			REG_WRITE32(GPIO_DR_REG, g_dr_low);
			for(i=0; i<DELAY_SHORT; i++)
				delay1ns();

		} else {
            // "0" pulse
            //high
			REG_WRITE32(GPIO_DR_REG, g_dr_high);
			for(i=0; i<DELAY_SHORT; i++)
				delay1ns();

			//low
			REG_WRITE32(GPIO_DR_REG, g_dr_low);
			for(i=0; i<DELAY_LONG; i++)
				delay1ns();

		}
		mask >>=1;
	}
	for(i=0; i<10; i++)//at least 10us delay between two writes
		for(j=0; j<DELAY_LONG+DELAY_SHORT; j++)
			delay1ns();
}

static void one_wire_init(void)
{
	unsigned int idx=ONEWIRE_GPIO_PORT_NUM%8;
	unsigned int g_ddr, g_dr;

	// Set as input
	REG_READ32(GPIO_DDR_REG, &g_ddr);
	g_ddr &= ~(1<<idx);
	REG_WRITE32(GPIO_DDR_REG, g_ddr);

	delay1ns();
	one_wire_write_frame(0xAAAA, 2); // dummy write to put code in cache

	// Set as output
	REG_READ32(GPIO_DDR_REG, &g_ddr);
	g_ddr |= (1<<idx);
	REG_WRITE32(GPIO_DDR_REG, g_ddr);

	//drive low to create idle state
	REG_READ32(GPIO_DR_REG, &g_dr);
	g_dr_low = g_dr & (~(1<<idx));
	g_dr_high = g_dr | (1<<idx);
	REG_WRITE32(GPIO_DR_REG, g_dr_low);
}

static void one_wire_stop(void)
{
	int i, j;
	for(i=0; i<5; i++)//5us low after stop bit
		for(j=0; j<DELAY_LONG+DELAY_SHORT; j++)
			delay1ns();
	//drive the SDI to high when it is not programming the output voltage
	REG_WRITE32(GPIO_DR_REG, g_dr_high);
	for(i=0; i<5; i++)//keep 5us after SDI high
		for(j=0; j<DELAY_LONG+DELAY_SHORT; j++)
			delay1ns();
}

int GpioOnewirePWMMode(void)
{
	//program Reg 0 from 70 to 50 (hex)
	//PG878 FB pin volutage ddip issue
	unsigned int data;
	data = PG877_PWM_MODE;
	data |= 1<<16;//start pulse
	data = data<<1 | 1;//stop pulse
	one_wire_init();
	one_wire_write_frame(data, PG877_DATA_BIT);
	one_wire_stop();
	return 0;
}
/* use SPI to imitate OneWire protocol.
 * It will be more stable than GPIO way.
 * And this function will set PG878 to work on PWM Mode
*/

int SPIOnewirePWMMode(void)
{
	//program Reg 0 from 70 to 50 (hex)
	//PG878 FB pin volutage ddip issue
	unsigned int data;
	data = PG877_PWM_MODE;
	spi_one_wire_write_frame(data, 1, 1);
	return 0;
}

static int percent_table[][2] = {
	{0x8, -100},
	{0x9, -75},
	{0xA, -50},
	{0xB, -25},
	{0xC, 25},
	{0xD, 50},
	{0xE, 75},
	{0xF, 100},
	{-1, -1},
};

static int voltage_table[][2] = {
	{0x8, 800},
	{0x9, 1000},
	{0xA, 1200},
	{0xB, 1500},
	{0xC, 1800},
	{0xD, 2500},
	{0xE, 3000},
	{0xF, 3300},
	{-1, -1},
};

int GpioOneWireVoltCtrl(int vout, int default_vout)
{
	int i = 0, j = 0;
	unsigned int data;
	unsigned int voltage_table_index = 0,percent_table_index = 0;
	int mini_trim = 0xffff;

	for(i = 0; voltage_table[i][0] > 0; i++){
		for(j = 0; percent_table[j][0] > 0; j++){
			if (voltage_table[i][1] + (voltage_table[i][1] * percent_table[j][1])/1000 >= vout)
			if(mini_trim > voltage_table[i][1] + (voltage_table[i][1] * percent_table[j][1])/1000 - vout){
				mini_trim = voltage_table[i][1] + (voltage_table[i][1] * percent_table[j][1])/1000 - vout;
				voltage_table_index = i;
				percent_table_index = j;
			}
		}
	}
	if (default_vout > vout && mini_trim > default_vout - vout){
		data = PG877_DEFAULT_DATA_FIELD;
		dbg_printf(PRN_RES,"set Vout to %dmV, default setting by hardware. \n",default_vout);
	}else{
		data = (voltage_table[voltage_table_index][0] << 4) | percent_table[percent_table_index][0];
		dbg_printf(PRN_RES,"vout_percentage %x, set Vout to %dmV\n", data, voltage_table[voltage_table_index][1] +
					(voltage_table[voltage_table_index][1] * percent_table[percent_table_index][1])/1000);
	}
	//add preamble
	data |= PG877_PREAMBLE << 8;
	//add stop bit
	data = data<<1 | 1;
	one_wire_init();
	dbg_printf(PRN_RES,"GpioOneWireVoltCtrl, data = 0x%x\n", data);
	one_wire_write_frame(data, PG877_DATA_BIT);
	one_wire_stop();
	return 0;
}
/* use SPI to imitate OneWire protocol.
 * It will be more stable than GPIO way.
*/
int SPIOneWireVoltCtrl(int vout, int default_vout)
{
	//static unsigned int initialized = 0;
	unsigned int data;
	int i ,j;
	unsigned int voltage_table_index = 0,percent_table_index = 0;
	int mini_trim = 0xffff;

	for(i = 0; voltage_table[i][0] > 0; i++){
		for(j = 0; percent_table[j][0] > 0; j++){
			if (voltage_table[i][1] + (voltage_table[i][1] *
				percent_table[j][1])/1000 >= vout)
				if(mini_trim > voltage_table[i][1] +
			(voltage_table[i][1] * percent_table[j][1])/1000 - vout)
				{
					mini_trim = voltage_table[i][1] +
			(voltage_table[i][1] * percent_table[j][1])/1000 - vout;
				voltage_table_index = i;
				percent_table_index = j;
			}
		}
	}
	if (default_vout > vout && mini_trim > default_vout - vout){
		data = PG877_DEFAULT_DATA_FIELD;
		dbg_printf(PRN_RES,"set Vout to %dmV, default setting by hardware. \n",
				default_vout);
	}else{
		data = (voltage_table[voltage_table_index][0] << 4) |
			percent_table[percent_table_index][0];
		dbg_printf(PRN_RES,"vout_percentage %x, set Vout to %dmV\n", data,
				voltage_table[voltage_table_index][1] +
				(voltage_table[voltage_table_index][1] *
				 percent_table[percent_table_index][1])/1000);
	}
	data |= 0x52 << 8;
	spi_one_wire_write_frame(data, 1, 1);
	return 0;
}
