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
#define _PWM_DRIVER_C_

#include "pwm_driver.h"
#include "memmap.h"
#include "io.h"

#define PWM_RegRead(a, pv)	REG_READ32(base_addr+(a), pv) /*(*(volatile int *)(base_addr+(a)))*/
#define PWM_RegWrite(a, v)	REG_WRITE32(base_addr+(a), v) /**(volatile int *)(base_addr+(a)) = (v)*/

static const int pwm_reg_offset[4] = {0, 0x10, 0x20, 0x30};

int pwm_on_off(int index, PWM_SETTINGS * pwm_settings, int on_off) {
	int base_addr = MEMMAP_PWM_REG_BASE + pwm_reg_offset[index];

	if(index < 0 || index > 3) {
		return -1;
	}

	on_off = on_off ? 1 : 0;
	PWM_RegWrite(0, on_off);

	//dbg_printf(PRN_RES,"<pwm_on_off> %d -- %d\n", index, on_off);

	pwm_settings->En = on_off;
	return 0;
}

int pwm_read_settings(int index, PWM_SETTINGS * pwm_settings) {
	int base_addr = MEMMAP_PWM_REG_BASE + pwm_reg_offset[index];
	UINT32 t[4];

	if(index < 0 || index > 3) {
		return -1;
	}

	PWM_RegRead(4, &t[1]);
	PWM_RegRead(8, &t[2]);
	PWM_RegRead(12, &t[3]);

	pwm_settings->PreScale = t[1] & 7;
	pwm_settings->Polarity = (t[1] >> 3) & 1;
	pwm_settings->Duty = t[2] & 0xffff;
	pwm_settings->Tcnt = t[3] & 0xffff;

	return 0;
} 

int pwm_write_settings(int index, PWM_SETTINGS * pwm_settings) {
	int base_addr = MEMMAP_PWM_REG_BASE + pwm_reg_offset[index];
	UINT32 t[4];

	if(index < 0 || index > 3) {
		return -1;
	}

	t[1] = (pwm_settings->PreScale & 7) | ((pwm_settings->Polarity & 1) << 3);
	t[2] = pwm_settings->Duty & 0xffff;
	t[3] = pwm_settings->Tcnt & 0xffff;

	PWM_RegWrite(4, t[1]);
	PWM_RegWrite(8, t[2]);
	PWM_RegWrite(12, t[3]);

	return 0;
}

