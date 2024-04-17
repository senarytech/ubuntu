/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright � 2013-2018 Synaptics Incorporated. All rights reserved.
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
#include "io.h"
#include "memmap.h"
#include "SysMgr.h"
#include "apbRegBase.h"
#include "debug.h"
#include "timer.h"

/*BT pair(SW1): 0.50V of 0.6V set to the threshold*/
#define EVK_SW1_THRESHOLD 0x6AA
/*Standby(SW2): 0.94V of 1.2V set to the threshold*/
#define EVK_SW2_THRESHOLD 0xC99
#define THRESHOLD EVK_SW1_THRESHOLD
/*Using ADC 12bit*/
#define THRESHOLD_MAX 0x1000

#define NO_ACTION 0x0
#define SHORT_PRESS 0x1
#define LONG_PRESS 0x2
#define TIMEOUT_PRESS 0x3

#define ADC_BASE SOC_SM_MEMMAP_SMREG_BASE
// DIAG\DV_Post_Silicon\BG7\info\PVT\3641-twffc_C676-0_ADC12b5MSAR_databook_1v3.pdf
// clock is 25Mhz according to ASIC's email RE VS680 Process and voltage in PVT sensor.mht
// Chip only has 2 Single-Ended input, no Differential Input, 2-7 are connected to analog ground
/* pg9 Table 1-2 pg10Table 1-5
	Conversion 			bit	cycle
	selres1..0 = 2’b11	12	14
	selres1..0 = 2’b10	10	12
	selres1..0 = 2’b01	8	10
	selres1..0 = 2’b00	6	8
*/
#define ADC_LATCH_DELAY	0						// make sure data is good, register bus clock (SM 25Mhz SOC 100Mhz) delay, PVT_test_sequence.docx pg2 4.	s/w should wait for data latency timer once it detects interrupt/data_rdy to sample the data
#define ADC_DELAY_US	1
#define ADC_TIMEOUT 	(2/ADC_DELAY_US)		// use 25mhz crystal oscillator Max Conversion Time 14/8 + ADC_LATCH_DELAY clock cycles for 2 us max
#define ADC_TIMEOUT_VAL	0xFFFF
#define ADC_MAX_VAL		(1<<12)

typedef enum _ADC_MODE
{
	ADC_POWER_Down = 0,
	ADC_POWER_On,
	ADC_STANDBY,	// This mode is used to avoid the bandgap start up time, while minimizing the power consumption
} ADC_MODE;

typedef enum _ADC_RES
{
	ADC_6BIT = 0,
	ADC_8BIT,
	ADC_10BIT,
	ADC_12BIT,
} ADC_RES;

typedef enum _ADC_INPUT
{
	ADC_INPUT0 = 0,	// Chip only has 2 Single-Ended input, no Differential Input, 2-7 are connected to analog ground
	ADC_INPUT1,
	ADC_INPUT_INVALID,
} ADC_INPUT;

const unsigned int adc_high[ADC_12BIT+1] =
{
	0x3f,
	0xff,
	0x3ff,
	0xfff,
};

const unsigned int adc_shift[ADC_12BIT+1] =
{
	6,
	4,
	2,
	0,
};

void adc_init(void)
{
}

static void adc_enable(ADC_MODE mode)
{
	T32smSysCtl_SM_CTRL reg;
	BFM_HOST_Bus_Read32((ADC_BASE+RA_smSysCtl_SM_CTRL), &reg.u32);
	reg.uSM_CTRL_ADC_CONT=0;
	reg.uSM_CTRL_ADC_RESET=0;
	reg.uSM_CTRL_ADC_START=0;					// During the power up time of the supply voltages, ensure that enadc=1’b0 and soc=1’b0
	reg.uSM_CTRL_ADC_SELIN=ADC_INPUT_INVALID;	// ADC_9001532902.pdf Synopsys Star: 9001532902 ADC input network is in sampling mode when the IP is put in power down/standby mode Workaround: Before putting the ADC in power down mode, Select an unused input channel 2-7 should be connected to analog ground
	if(mode==ADC_POWER_Down)
		reg.uSM_CTRL_ADC_SELBG=0;
	else
		reg.uSM_CTRL_ADC_SELBG=1;				// pg23 3.1.3 standby mode avoid the bandgap start up time, while minimizing the power consumption
	reg.uSM_CTRL_ADC_SEL_VREF=0;				// Use external voltage reference. Not applicable for Dolphin. Program 0 to reduce leakage during ADC shut-down
	reg.uSM_CTRL_ADC_EN=0;						// During the power up time of the supply voltages, ensure that enadc=1’b0 and soc=1’b0
	reg.uSM_CTRL_ADC_ENCTR=0;					// pg24 3.1.4 number of digital input signals (enctr2..0) that enable internal analog programmable modes, which are used for test purposes. In normal operation these signals should be tied low
	BFM_HOST_Bus_Write32((ADC_BASE+RA_smSysCtl_SM_CTRL), reg.u32);
	if(mode==ADC_POWER_On)
	{
		T32smSysCtl_SM_ADC_CTRL reg1;
		T32smSysCtl_SM_TEST_DATA0 reg2;
		T32smSysCtl_SM_TEST_DATA1 reg3;
		T32smSysCtl_SM_ADC_STATUS reg4;
		BFM_HOST_Bus_Read32((ADC_BASE+RA_smSysCtl_SM_ADC_CTRL), &reg1.u32);
		reg1.uSM_ADC_CTRL_ADC_DAT_LT=ADC_LATCH_DELAY;
		BFM_HOST_Bus_Write32((ADC_BASE+RA_smSysCtl_SM_ADC_CTRL), reg1.u32);
		BFM_HOST_Bus_Read32((ADC_BASE+RA_smSysCtl_SM_TEST_DATA0), &reg2.u32);
		reg2.uSM_TEST_DATA0_ADC_DATA_HIGH=adc_high[ADC_12BIT];
		BFM_HOST_Bus_Write32((ADC_BASE+RA_smSysCtl_SM_TEST_DATA0), reg2.u32);
		BFM_HOST_Bus_Read32((ADC_BASE+RA_smSysCtl_SM_TEST_DATA1), &reg3.u32);
		reg3.uSM_TEST_DATA1_ADC_DATA_LOW=0;
		BFM_HOST_Bus_Write32((ADC_BASE+RA_smSysCtl_SM_TEST_DATA1), reg3.u32);
		reg.uSM_CTRL_ADC_SELBG=1;				// pg10 Table 1-3 vref = vrefint (selref=1’b1 selbg=1’b1)
		reg.uSM_CTRL_ADC_SEL_VREF=1;			// Use internal voltage reference. Normal operation, Dolphin has no external voltage reference
		reg.uSM_CTRL_ADC_EN=1;					// Set enadc = 1’b1,
		BFM_HOST_Bus_Write32((ADC_BASE+RA_smSysCtl_SM_CTRL), reg.u32);
		udelay(1);							// 3641_AuxADC_ProductTrainnig_0v3.pdf pg11 Waiting tpup ( 17*clk_cycles) 17/25=0.68us
		reg.uSM_CTRL_ADC_RESET=1;
		BFM_HOST_Bus_Write32((ADC_BASE+RA_smSysCtl_SM_CTRL), reg.u32);
		BFM_HOST_Bus_Read32((ADC_BASE+RA_smSysCtl_SM_ADC_STATUS), &reg4.u32);
		reg4.uSM_ADC_STATUS_DATA_RDY=0; 		// clear for new conversion pull or interrupt
		reg4.uSM_ADC_STATUS_INT_EN=0;
		BFM_HOST_Bus_Write32((ADC_BASE+RA_smSysCtl_SM_ADC_STATUS), reg4.u32);
		udelay(1);							// Set the reset signal to high during at least 10ns
		reg.uSM_CTRL_ADC_RESET=0;
		BFM_HOST_Bus_Write32((ADC_BASE+RA_smSysCtl_SM_CTRL), reg.u32);
		reg.uSM_CTRL_ADC_CONT=0;				// Start a dummy conversion cycle by setting soc = 1’b1
		reg.uSM_CTRL_ADC_START=1;
		reg.uSM_CTRL_ADC_SELRES=ADC_6BIT;
		reg.uSM_CTRL_ADC_SELDIFF=0;				// Chip only has 2 Single-Ended input, no Differential Input
		reg.uSM_CTRL_ADC_SELIN=ADC_INPUT0;
		BFM_HOST_Bus_Write32((ADC_BASE+RA_smSysCtl_SM_CTRL), reg.u32);
//		reg.uSM_CTRL_ADC_START=0;				// single conversion
//		BFM_HOST_Bus_Write32((ADC_BASE+RA_smSysCtl_SM_CTRL), reg.u32);
		udelay(ADC_TIMEOUT*ADC_DELAY_US);		// 3641_AuxADC_ProductTrainnig_0v3.pdf pg11 After this dummy conversion cycle has ended, the ADC is ready to receive analog inputs
		BFM_HOST_Bus_Read32((ADC_BASE+RA_smSysCtl_SM_ADC_STATUS), &reg4.u32);
		if(reg4.uSM_ADC_STATUS_DATA_RDY)
		{
			T32smSysCtl_SM_TEST reg5;
			reg4.uSM_ADC_STATUS_DATA_RDY=0;		// clear for new conversion pull or interrupt
			BFM_HOST_Bus_Write32((ADC_BASE+RA_smSysCtl_SM_ADC_STATUS), reg4.u32);
			BFM_HOST_Bus_Read32((ADC_BASE+RA_smSysCtl_SM_TEST), &reg5.u32);
			if(reg5.uSM_TEST_ADC_TEST_FAIL)
				dbg_printf(PRN_ERR, "ADC data is not in the valid window\n");
		}
		else
			dbg_printf(PRN_ERR, "ADC dummy conversion is not finished\n");
	}
}

INT32 adc_read(int ch, int sample)
{
	T32smSysCtl_SM_CTRL reg;
	T32smSysCtl_SM_ADC_STATUS reg1;
	T32smSysCtl_SM_ADC_DATA reg2;
	T32smSysCtl_SM_TEST reg3;
	T32smSysCtl_SM_TEST_DATA0 reg4;
	unsigned int sum,i,j;
	unsigned short max, min, value;
	sample = 4;
	ADC_RES res = ADC_12BIT;

	if (ch>=ADC_INPUT_INVALID)
	{
		dbg_printf(PRN_ERR, "Valid input 0 - %d\n",ADC_INPUT_INVALID-1);
		return -1;
	}

	sum=0;
	max=0;
	min=ADC_MAX_VAL-1;
	BFM_HOST_Bus_Read32((ADC_BASE+RA_smSysCtl_SM_CTRL), &reg.u32);
	if(reg.uSM_CTRL_ADC_EN==0)
		adc_enable(ADC_POWER_On);

	BFM_HOST_Bus_Read32((ADC_BASE+RA_smSysCtl_SM_ADC_STATUS), &reg1.u32);
	reg1.uSM_ADC_STATUS_DATA_RDY=0;
	BFM_HOST_Bus_Write32((ADC_BASE+RA_smSysCtl_SM_ADC_STATUS), reg1.u32);
	BFM_HOST_Bus_Read32((ADC_BASE+RA_smSysCtl_SM_TEST_DATA0), &reg4.u32);
	reg4.uSM_TEST_DATA0_ADC_DATA_HIGH=adc_high[ADC_12BIT];
	BFM_HOST_Bus_Write32((ADC_BASE+RA_smSysCtl_SM_TEST_DATA0), reg4.u32);
	BFM_HOST_Bus_Read32((ADC_BASE+RA_smSysCtl_SM_CTRL), &reg.u32);
	reg.uSM_CTRL_ADC_SELRES=res;
	reg.uSM_CTRL_ADC_SELIN=ch;
	BFM_HOST_Bus_Write32((ADC_BASE+RA_smSysCtl_SM_CTRL), reg.u32);
	if(sample>1)	// For continuous sampling mode we don’t use ADC_START register bit. We use ADC_CONT bit.
	{
		reg.uSM_CTRL_ADC_CONT=1;
		reg.uSM_CTRL_ADC_START=0;
	}
	else
	{
		reg.uSM_CTRL_ADC_CONT=0;
		reg.uSM_CTRL_ADC_START=1;
	}
	BFM_HOST_Bus_Write32((ADC_BASE+RA_smSysCtl_SM_CTRL), reg.u32);
	for(i=0;i<sample;i++)
	{
		for(j=0;j<ADC_TIMEOUT;j++)
		{
			udelay(ADC_DELAY_US);

				BFM_HOST_Bus_Read32((ADC_BASE+RA_smSysCtl_SM_ADC_STATUS), &reg1.u32);
				if(reg1.uSM_ADC_STATUS_DATA_RDY)
					break;
		}
		if(j>=ADC_TIMEOUT)
		{
			dbg_printf(PRN_ERR, "ADC read timeout\n");
			break;
		}
		reg1.uSM_ADC_STATUS_DATA_RDY=0;
		BFM_HOST_Bus_Write32((ADC_BASE+RA_smSysCtl_SM_ADC_STATUS), reg1.u32);
		BFM_HOST_Bus_Read32((ADC_BASE+RA_smSysCtl_SM_TEST), &reg3.u32);
		if(reg3.uSM_TEST_ADC_TEST_FAIL)
		{
			dbg_printf(PRN_ERR, "ADC data is not in the valid window\n");
			break;
		}
		BFM_HOST_Bus_Read32((ADC_BASE+RA_smSysCtl_SM_ADC_DATA), &reg2.u32);
		value=reg2.uSM_ADC_DATA_ADC_DATA>>adc_shift[res];
		if(reg2.uSM_ADC_DATA_ADC_DATA!=(value<<adc_shift[res]))
			dbg_printf(PRN_ERR, "ADC data 0x%03x lower %d bits is not 0\n",reg2.uSM_ADC_DATA_ADC_DATA,adc_shift[res]);
		sum+=value;
		if(max<value)
			max=value;
		if(min>value)
			min=value;

	}
	reg.uSM_CTRL_ADC_CONT=0;					//
//	reg.uSM_CTRL_ADC_START=0;					// Start ADC digitalization, clear automatically
	reg.uSM_CTRL_ADC_SELIN=ADC_INPUT_INVALID;	// ADC_9001532902.pdf Synopsys Star: 9001532902 ADC input network is in sampling mode when the IP is put in power down/standby mode Workaround: Before putting the ADC in power down mode, Select an unused input channel 2-7 should be connected to analog ground
	BFM_HOST_Bus_Write32((ADC_BASE+RA_smSysCtl_SM_CTRL), reg.u32);

	if(i)
	{
		sum/=i;
		dbg_printf(PRN_RES, " ADC read %d times avg 0x%03x max 0x%03x min 0x%03x\n",i,sum,max,min);
	}
	return sum;
}

int32_t adc_key_detect(int32_t time_out)
{
	uint32_t ret = NO_ACTION;
	int32_t ms = 0, count = 0;
	int32_t data = 0;

	if(time_out == 0)
		ms = 0x7fffffff;
	else
		ms = time_out;

	while(ms > 0) {
		data = adc_read(0, 4);
		if(data > THRESHOLD && data < THRESHOLD_MAX) { //key is pressed
			count++;
			if(count > 150) { // long press
				ret = LONG_PRESS;
				mdelay(10);
				break;
			}
		}
		if(ms <= 10 && count > 0) {
			ret = SHORT_PRESS;
			break;
		}
		mdelay(10);
		ms -= 10;
	}

	if(!(ms > 0))
		return TIMEOUT_PRESS;

	return ret;
}

int32_t is_key_pressed(int32_t try_count)
{
	int32_t data = -1;
	while (try_count > 0) {
		data = adc_read(0, 4);
#ifdef CONFIG_FUNCTION_BUTTON_HIGH_LEV_VALID
		if(data > THRESHOLD && data < THRESHOLD_MAX) { //key is pressed
#else
		if(data < THRESHOLD) { //key is pressed
#endif
			dbg_printf(PRN_DBG,"function key is pressed\n");
			return 1;
		}
		try_count--;
	}

	return 0;
}
