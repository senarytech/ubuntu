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
#ifndef __VB_STATE_H_
#define __VB_STATE_H_

typedef enum{
	DEV_LOCK = 0x10,
	DEV_UNLOCK,
}VB_DEVICE_STATE;

typedef enum{
	VB_FINISH = 0x1,
	VB_CONTINUE,
	VB_REBOOT,
}VB_RET;

typedef enum{
	VB_STATE_INIT = 0x100,
	//VB_STATE_INIT_LOCK,
	VB_STATE_OEMK_FAIL,
	VB_STATE_OEMK_SUCC,
	VB_STATE_USERK_FAIL,
	VB_STATE_USERK_SUCC,
}VB_STATE;

typedef enum {
	YELLOW = 0,
	ORANGE,
	RED,
	GREEN,
	STOP_DISPLAY = 0xFF,
}BOOT_VERIFY_STATE;

VB_RET vb_state_enterstep(VB_STATE state);
int is_vb_finish(void);
int is_dev_lock(void);
int get_verify_state(void);
void vb_setup_linux_parameter(char * kernel_param);

#endif
