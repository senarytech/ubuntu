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

#include "mmgr.h"
#include "tz_boot_cmd.h"
#include "tz_nw_boot.h"
#ifdef CONFIG_GPT
#include "load_gpt.h"
#endif
#ifdef CONFIG_VT
#include "load_vt.h"
#endif
#include "flash_ts.h"
#include "timer.h"
#include "misc.h"
#include "boot_mode.h"

extern void __reset_cpu(uintptr_t cpu_id, uintptr_t address,unsigned int params);

void (*TheKernel)(int zero, int arch, unsigned int params);

#ifndef CONFIG_AB_BOOTFLOW
const char * get_ptname_bymode(int bootmode)
{
	if(bootmode == BOOTMODE_NORMAL)
		return KERNEL_NAME;
	if(bootmode == BOOTMODE_RECOVERY)
		return RECOVERY_NAME;
	return KERNEL_NAME;
}
#else
const char * get_ptname_bymode(int bootmode)
{
	void *pt_name;;
	pt_name = (get_abmode() == BOOTSEL_A) ?  KERNEL_A_NAME : KERNEL_B_NAME;

	return pt_name;
}
#endif //CONFIG_AB_BOOTFLOW

//this size must larger than GPT_PRI_READ_SIZE(0x6800) as the gpt buff address will be 64byte align for DMP transfer
#define PT_BUFF_SIZE (0x6840)
int init_pt(void)
{
	int ret = 0;
	unsigned char * pt_buf = NULL;
	pt_buf = (unsigned char *)malloc(PT_BUFF_SIZE);
	if(get_partition_info(pt_buf) != 0) {
		ret = -1;
	}
	free(pt_buf);
	return ret;
}

void set_kernel_entry(unsigned int addr)
{
	TheKernel = (void (*)(int, int,unsigned int))((uintptr_t)addr);
}

void start_android_kernel(void)
{
	// TODO: Set appropriate arch parameter.
	unsigned arch = 0;
	unsigned params = get_dtb_addr();

	if (get_bootmode() == BOOTMODE_NORMAL) {
		dbg_printf(PRN_RES,"Boot normal GTV image\n");
#ifdef CONFIG_TRUSTZONE
	//fix me: needs to remove when TZ is ok
	tz_nw_enter_boot_stage(TZ_BOOT_STAGE_LINUX, TZ_BOOT_MODE_NORMAL);
#endif
	} else {
		dbg_printf(PRN_RES,"Boot recovery image\n");
#ifdef CONFIG_TRUSTZONE
		tz_nw_enter_boot_stage(TZ_BOOT_STAGE_LINUX, TZ_BOOT_MODE_RECOVERY);
#endif
		//clear fts bootloader flag
		set_bootmode(BOOTMODE_NORMAL);
	}
	//clear fts boot options
	set_bootoptions(0x0);
	stop_timer();
	dbg_printf(PRN_NOTICE,"Start kernel at 0x%08x\n", get_kernel_addr());

	__reset_cpu((uintptr_t)TheKernel , 1330, params) ;
	TheKernel (0, arch, params);
}

extern unsigned long get_resume_linux_addr(void);
// if here, Image-3 is copied into DDR
int Image_3_Jump(void)
{
	dbg_printf(PRN_NOTICE,"[%6d] ", get_time_ms());
	// any state reach here should start/resume kernel
	if((is_resume()) && (get_resume_linux_addr() != 0xFFFFFFFF)
					&& (get_resume_linux_addr() != 0x0)){
		// resume
		stop_timer();
#ifdef CONFIG_TRUSTZONE
		tz_nw_enter_boot_stage(TZ_BOOT_STAGE_LINUX, TZ_BOOT_MODE_NORMAL);
#endif
		dbg_printf(PRN_RES,"Resume System at address: 0x%08x\n", get_resume_linux_addr()); //uiWarmDown_2_Linux_Addr
		/* we don't care the parameters when resume */
		__reset_cpu(get_resume_linux_addr(), 1330, 0); //uiWarmDown_2_Linux_Addr
	}else{
		start_android_kernel();
	}

	return 0;
}


