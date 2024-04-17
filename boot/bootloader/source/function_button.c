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
#include "SysMgr.h"
#include "system_manager.h"
#include "apbRegBase.h"
#include "debug.h"
#include "gpio.h"

#include "misc.h"
#ifdef CONFIG_FUNCTION_BUTTON_ADC
#include "adc_driver.h"
#endif

#ifdef CONFIG_FUNCTION_BUTTON_ENABLE
/* function button check for skipper box*/
#ifndef CONFIG_FUNCTION_BUTTON_SKIPPER
/* adc button or gpio button */
#ifdef CONFIG_FUNCTION_BUTTON_ADC
#define HIGH_LEV	800
#define LOW_LEV		220
#define ADC_SAMPLE_TIME	4
#ifdef CONFIG_FUNCTION_BUTTON_HIGH_LEV_VALID
#define BUTTON_PRESSED(val)	((val) > HIGH_LEV)
#else
#define BUTTON_PRESSED(val)	((val) < LOW_LEV)
#endif /* CONFIG_FUNCTION_BUTTON_HIGH_LEV_VALID */

#define ADC_TRY_COUNT 2

#else /*ifndef CONFIG_FUNCTION_BUTTON_ADC*/
#ifdef CONFIG_FUNCTION_BUTTON_HIGH_LEV_VALID
#define BUTTON_PRESSED(val)	((val) == 1)
#else
#define BUTTON_PRESSED(val)	((val) == 0)
#endif /* CONFIG_FUNCTION_BUTTON_HIGH_LEV_VALID */
#endif /* CONFIG_FUNCTION_BUTTON_ADC */

/* for function button into recovery mode */
int is_EnterRecovery = 0;

void init_function_button()
{
	/* only excute when system is from cold boot */
	if(is_poweron())
	{
#ifdef CONFIG_FUNCTION_BUTTON_ADC
		adc_init();
#else /* ifndef CONFIG_FUNCTION_BUTTON_ADC */
		unsigned int GpioPortNum = CONFIG_FUNCTION_BUTTON_SM_GPIO_PORT;
		SM_GPIO_PortSetInOut(GpioPortNum, 1);
#endif /* CONFIG_FUNCTION_BUTTON_ADC */
	}
	is_EnterRecovery = 0;
}

void check_function_button()
{
#ifdef CONFIG_FUNCTION_BUTTON_ADC
	if(is_poweron())
	{
		if (is_key_pressed(ADC_TRY_COUNT))
			is_EnterRecovery++;
	}
#else /* ifndef CONFIG_FUNCTION_BUTTON_ADC */
	if(is_poweron())
	{
		int press = 0;
		unsigned int GpioPortNum = CONFIG_FUNCTION_BUTTON_SM_GPIO_PORT;
		SM_GPIO_PortRead(GpioPortNum, &press);
		if( press )
			is_EnterRecovery++;
	}
#endif /* CONFIG_FUNCTION_BUTTON_ADC */
}
#endif /* CONFIG_FUNCTION_BUTTON_SKIPPER */
#endif /* CONFIG_FUNCTION_BUTTON_ENABLE */
