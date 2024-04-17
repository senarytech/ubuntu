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
#include "io.h"
#include "memmap.h"
#include "SysMgr.h"
#include "apbRegBase.h"
#include "debug.h"

#define ADC_TIMEOUT_COUNT	100

void adc_init(void)
{
	T32smSysCtl_SM_CTRL reg;
	reg.u32 = read32((void *)(uintptr_t)SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CTRL);
	reg.uSM_CTRL_ADC_CKSEL = 0;//clk/2
	reg.uSM_CTRL_ADC_PU = 1;//power up
	reg.uSM_CTRL_ADC_RESET = 1;//reset ADC after power up

	REG_WRITE32(SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CTRL, reg.u32);
	mdelay(1);

	/*  */
	reg.u32=read32((void *)(uintptr_t)SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CTRL);
	reg.uSM_CTRL_ADC_RESET = 0;//de_reset ADC after power up
	REG_WRITE32(SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CTRL,reg.u32);

}

static void adc_enable(UINT32 en)
{
	T32smSysCtl_SM_CTRL reg;
	//T32smSysCtl_SM_ADC_STATUS status;
	//enable/disable
	reg.u32 = read32((void *)(uintptr_t)SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CTRL);

	reg.uSM_CTRL_ADC_RESET = 0;
	reg.uSM_CTRL_ADC_PU = en;
	reg.uSM_CTRL_ADC_BUF_EN = 0;
#if (BERLIN_CHIP_VERSION < BERLIN_BG2CT_A0)
	reg.uSM_CTRL_ADC_BG_RDY = 1;
#endif
	reg.uSM_CTRL_ADC_CONT = 0;   // 0: continue mode?

	REG_WRITE32((SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CTRL), reg.u32);
}


static void adc_select(UINT32 sel,UINT32 clk)
{

	T32smSysCtl_SM_CTRL reg;
	//enable/disable
	reg.u32 = read32((void *)(uintptr_t)SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CTRL);

	reg.uSM_CTRL_ADC_SEL = sel;      // 0~15
	reg.uSM_CTRL_ADC_CKSEL = clk;    // 2/3/4/8
	REG_WRITE32((SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CTRL), reg.u32);
}

static void adc_start(UINT32 start)
{
	T32smSysCtl_SM_CTRL reg;

	//start/stop
	reg.u32 = read32((void *)(uintptr_t)SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CTRL);

	reg.uSM_CTRL_ADC_START = start;      // 0/1

	REG_WRITE32((SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CTRL), reg.u32);
}

static INT32 adc_getdata(unsigned int ch)
{
	UINT32 status;
	INT32 cnt = ADC_TIMEOUT_COUNT;
	INT32 result = -1;

	/* FIXME: the while maybe casue block issue */
	// wait for channel data ready
	while (--cnt) {
		status = read32((void *)(uintptr_t)SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_ADC_STATUS);
		if (status & (1 << ch))
			break;
	}

	if (cnt > 0) {
		result = read32((void *)(uintptr_t)SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_ADC_DATA);
		// write 0 to clear
		status &= 0xFFFF0000;
		REG_WRITE32((SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_ADC_STATUS), status);
	}

	return result;
}


INT32 adc_read(int ch, int sample)
{
	UINT32 clk = 0;// clock divider = 2
	//The time to get final test result:
	//sample = 4: 25 us
	//sample = 1: 12 us
	UINT32 adc_sum = 0;
	UINT32 i;
	INT32 data;

	ch &= 0xF;

	adc_enable(1);
	adc_select(ch, clk);

	// save sampled data to memory
	for (i = 0; i < sample; i++)
	{
		adc_start(1);
		data = adc_getdata(ch);
		if (data < 0) {
			adc_start(0);
			dbg_printf(PRN_ERR,"%s - failed, need another attempt.\n", __func__);
			return -1;
		}
		adc_sum += data;
	}

	adc_start(0);
	data = adc_sum / sample;

	return data;
}


#define HIGH_THRESHOLD 900
#define LOW_THRESHOLD 300

#define NO_ACTION 0x0
#define SHORT_PRESS 0x1
#define LONG_PRESS 0x2
#define TIMEOUT_PRESS 0x3

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
		if(data > 0 && data < LOW_THRESHOLD) { //key is pressed
			count++;
			if(count > 150) { // long press
				ret = LONG_PRESS;
				mdelay(10);
				break;
			}
		}
		if(data > HIGH_THRESHOLD) {
			if(count > 0) { //short press
				ret = SHORT_PRESS;
				break;
			}
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
		if(data > 0 && data < LOW_THRESHOLD) { //key is pressed
			dbg_printf(PRN_DBG,"function key is pressed\n");
			return 1;
		}
		try_count--;
	}

	return 0;
}

#if 0
void test_adc(void)
{
	int32_t ret = 0;

	adc_init();
	while(1) {
		dbg_printf(PRN_NOTICE, "begin detect\n");
		ret = adc_key_detect(0);
		if(ret == SHORT_PRESS) {
			dbg_printf(PRN_NOTICE,"short press\n");
		}
		if(ret == LONG_PRESS) {
			dbg_printf(PRN_NOTICE,"long press\n");
		}
		dbg_printf(PRN_NOTICE, "end detect\n");
		mdelay(2000);
	}
}
#endif
