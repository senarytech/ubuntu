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
#include <memmap.h>
#include <global.h>
#include "io.h"
#include "pwm_driver.h"
#include "galois_speed.h"

#define PWM_CHANNEL     2

struct duty_volt {
	int duty;
	int volt;
};

static struct duty_volt dv_table[] =
{
	{23, 1200},
	{31, 1170},
	{37, 1140},
	{40, 1125},
	{44, 1100},
	{49, 1075},
	{54, 1050},
	{58, 1025},
	{63, 1000},
	{66, 975},
	{80, 950},
	{-1, -1},
};


int pwm(int volt)
{
	int pwm_freq;
	struct duty_volt *p;
	PWM_SETTINGS pwm_setting ;

	pwm_setting.En = 1;
	pwm_setting.PreScale = 1 ;
	pwm_setting.Polarity = 0 ;
	p = dv_table;
	while(p->duty != -1) {
		if(p->volt == volt)
			break;
		p++;
	}
	if(p->duty == -1)
		return -1;
	pwm_freq = GaloisGetFrequency(SOC_FREQ_CFG);
	pwm_setting.Duty = pwm_freq*p->duty/100;
	pwm_setting.Tcnt= pwm_freq;

	pwm_write_settings(PWM_CHANNEL, &pwm_setting) ;
	pwm_on_off(PWM_CHANNEL, &pwm_setting, 1) ;

	return 0;
}
