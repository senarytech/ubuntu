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
#include "bootloader.h"
#include "debug.h"
#include "apb_watchdog.h"
#include "date.h"
#include "mmgr.h"
#include "rt_stage.h"
#include "timer.h"

#define PRN_DEBUG_LEVEL PRN_DBG

void do_init_mmgr(void)
{
	extern unsigned int __heap_start;
	extern unsigned int __heap_end;
	unsigned long mstart = (unsigned long)&__heap_start;
	unsigned long mend = (unsigned long)&__heap_end;
	unsigned int size = mend - mstart;

	INFO("head start = %lx, end = %lx\n", &__heap_start, &__heap_end);
	INFO("mstart = %lx - %lx, size = %x\n", mstart, mend, size);
	init_mmgr(mstart, size);
}

BOOL validate_stage(unsigned int stage)
{
    if((stage <= BOOTLOADER_NULL_STAGE) ||(stage >= BOOTLOADER_MAX_STAGE))
        return FALSE;

    if((stage & (stage - 1)) != 0)
        return FALSE;

    return TRUE;
}

extern bootloader_task_t * get_chip_tasks(int *num);
extern bootloader_task_t * get_platform_tasks(int *num);
extern void setup_boot_args(unsigned long param1, unsigned long param2, unsigned long param3);
extern void early_chip_init(void);

void StartBootLoader(unsigned long param1, unsigned long param2, unsigned long param3)
{
	unsigned int index = 0;
	unsigned int curr_stage = 0, next_stage = 0;
	rt_stage_desc_t *stage = NULL;
	unsigned int rt_stage_nums = get_rt_bl_descs_nums();

	early_chip_init();
	dbg_printf(PRN_INFO, "[%6d] start_bootloader. %s\n", get_time_ms(), compile_date);
	do_init_mmgr();
	setup_boot_args(param1, param2, param3);

	for(curr_stage = 1; curr_stage < BOOTLOADER_MAX_STAGE; (curr_stage <<= 1)) {

NEXT_STAGE:
		//some process will lead to the specific stage
		if(next_stage != BOOTLOADER_NULL_STAGE) {
			if(validate_stage(next_stage))
				curr_stage = next_stage;
			else {
				dbg_printf(PRN_ERR,"!!!!!!wrong stage 0x%08x\n", next_stage);
				dbg_printf(PRN_ERR,"!!!!!!we will go to next stage\n");
				curr_stage <<= 1;
			}
			if(curr_stage >= BOOTLOADER_MAX_STAGE)
				break;
		}

		dbg_printf(PRN_DEBUG_LEVEL, "\n--------current boot stage 0x%x--------\n", curr_stage);
		for (index = 0; index < rt_stage_nums; index ++) {
			stage = get_rt_stage_desc(index);
			if (stage->protect != 0) {
				dbg_printf(PRN_ERR, "the name of task [%d] overflowed\n", index);
				goto ERROR;
			}

			if (stage->boot_stage == curr_stage) {
				dbg_printf(PRN_DEBUG_LEVEL, "########task [%s] begin to run\n", stage->name);
				next_stage = stage->entry();
				if (BOOTLOADER_NULL_STAGE != next_stage)
					goto NEXT_STAGE;
			}
		}
}

ERROR:
    dbg_printf(PRN_ERR,"!!!!!!should never come here\n");

    reset_soc();
}
