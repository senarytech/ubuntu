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
#ifndef __RT_STAGE_H
#define __RT_STAGE_H

/* Prototype for process entry function */
typedef int (*stage_func)(void);

typedef struct rt_stage_desc {
	char name[32];
	int protect; /* avoid buffer overflow of name. use 4 bytes directly to keep 4 bytes alignment */
	/* the return value is the next stage we will go to. otherwise, return 0 */
	int boot_stage;
	stage_func entry; /* task entry */
} rt_stage_desc_t;

#define DECLARE_RT_STAGE(_bootloader_stage, _name, _protect, _task_entry) \
	static const rt_stage_desc_t __rt_stage_desc_ ## _name \
		__attribute__((__section__("rt_bl_stage_descs"))) __attribute__((__used__)) = { \
			.name = #_name, \
			.protect = (_protect), \
			.boot_stage = (_bootloader_stage), \
			.entry = (_task_entry) }

rt_stage_desc_t * get_rt_stage_desc(int index);
unsigned int get_rt_bl_descs_nums(void);
#endif
