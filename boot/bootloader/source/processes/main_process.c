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
#include "io.h"
#include "string.h"
#include "debug.h"

#include "wol.h"

#include "boot_mode.h"

#include "apb_watchdog.h"

#include "misc.h"
#include "flash_ts.h"
#include "flash_adaptor.h"

#include "bootloader.h"
#include "rt_stage.h"
#include "bootloader_header.h"
#include "preload_ta.h"
#include "image_chunk.h"
#include "vb_state.h"

#ifdef CONFIG_FASTLOGO
#include "mv_display_api.h"
#endif //CONFIG_FASTLOGO

#include "boot_devinfo.h"

extern unsigned long get_resume_linux_addr(void);
extern int init_pt(void);
extern void Setup_KeyMaster(void);

/******************************************************************
* Main process fuctions of bootloader stage.
* Function used in stage should be declared by convinence macro
******************************************************************/
static int __attribute__((used)) main_bootloader_initchip_prestage(void)
{
#if defined(CONFIG_VERIFYBOOT)
	extern void parse_reboot_reason();
	parse_reboot_reason();
#endif
	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) main_bootloader_initchip_stage(void)
{
	dbg_printf(PRN_RES,"INITCHIP_STAGE: uiBoot = %d\n", get_bootstate());
	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) main_bootloader_initstorage_prestage(void)
{
	/* if it's resume, then we just start kernel after do some initializations */
	if(is_resume()){
		//NOTICE: Add SOC customization here before suspend resume process.

		if((get_resume_linux_addr() != 0xFFFFFFFF) && (get_resume_linux_addr() != 0x0)){
			return BOOTLOADER_STARTKERNEL_PRESTAGE;
		}
		else{
			//if error occurs, we will do immediate reset
			dbg_printf(PRN_ERR, "!!!!!! Wrong resume system address !\n");
			reset_soc();
		}
	}
#ifdef CONFIG_TRUSTZONE
	get_mem_region_list_from_tz();
#endif
	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) main_bootloader_initstorage_stage(void)
{
	init_flash(get_block_size(), get_page_size(), get_addr_cycle());
	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) main_bootloader_getversiontable_stage(void)
{
	if(init_pt()) {
		dbg_printf(PRN_ERR, "get GPT error!\n");
		reset_soc();
	}
	//FIXME: add dumping GPT for debugging
	//dump_gpt_table();
#if defined(CONFIG_TRUSTZONE_RPMB_FEATURE) && defined(EMMC_BOOT)
	extern int handle_rpmb_key();
	dbg_printf(PRN_INFO, "start to handle rpmb key !\n");
	if(handle_rpmb_key()) {
		dbg_printf(PRN_ERR, "Handle RPMB Key failed !\n");
		reset_soc();
	}
#endif
	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) main_bootloader_initfts_stage(void)
{
	int dbg_level;

	init_bootmode();

	dbg_level = get_debuglevel_from_flash();
	if((dbg_level >= PRN_ERR) && (dbg_level <= PRN_DBG)) {
		dbg_printf(PRN_RES, "fts set dbg_level to %d.\n", dbg_level);
		set_dbg_level(dbg_level);
	}

	//Get Bootmode from FTS for unification, instead of SM DTCM flag
	check_bootloader_bootmode();
	check_bootoptions();
#ifdef CONFIG_AB_BOOTFLOW
	init_abmode();
	check_abmode();
#endif

#if defined(CONFIG_PRELOADTA)
	if(reg_preload_tas()) {
		dbg_printf(PRN_ERR, "Register preload ta failed !\n");
		reset_soc();
	}
#endif

	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) main_bootloader_initfts_poststage(void)
{
	//we should get reset msg before sm is reload if there is sm
	//otherwise, the dtcm will be clear
	get_reset_msg();

	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) main_bootloader_loadkernel_prestage(void)
{
#ifdef CONFIG_VERIFYBOOT
	if(VB_REBOOT == vb_state_enterstep(VB_STATE_INIT)) {
		return -1;
	}
#endif

	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) main_bootloader_loadkernel_stage(void)
{
	if( is_coldbootup() || is_lowpowerstandby_2_active() ||
		((!is_standby_2_active()) && (!is_resume())) ) {
			dbg_printf(PRN_RES, "LOADKERNEL_STAGE: get_bootstate() = 0x%x !\n", get_bootstate());

			/* note: in the case of STATE_WARMUP_0/1/2, we expect RAM is OK. In the case of
			* Beetle DDR_OFF, it's not. So make sure Image_Load_And_Start is called for the Beetle case.
			* note: MV_SoC_STATE_BOOT_RECOVERY_KERNEL takes this branch.
			*/
			int ret = Image_Load_And_Verify();
			if(BOOTLOADER_NORMAL_RESET_STAGE == ret) {
				//the reset here is defined in TZ boot flow and it only can be done by SM(or BCM)
				//becasue we need keep some parameters to finish the TZ boot flow.
				#ifdef CONFIG_SM
					return BOOTLOADER_NORMAL_RESET_STAGE;
				#else
					reset_soc();
				#endif
			} else if(BOOTLOADER_NORMAL_POWERDOWN_STAGE == ret) {

				//The powerdown soc cores here is defined for verify boot RED flow by SM
				//For those scenarios without SM, the action will be reset instead of poweroff.
				return BOOTLOADER_NORMAL_POWERDOWN_STAGE;
			} else if(ret) {
				//if error occurs, we will do immediate reset
				dbg_printf(PRN_ERR,"tbdzz---- Img_Ld_And_Start error! Spinning now!\n");
				reset_soc();
			}

#if defined(CONFIG_PRELOADTA)
			Setup_KeyMaster();
#endif
	}
	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) main_bootloader_startkernel_prestage(void)
{
	/*
	* setup linux boot parameter before kernel start
	* if it's resume, don't setup parameters again
	*/
	dbg_printf(PRN_RES, "STARTKERNEL_PRESTAGE: get_bootstate() = 0x%x !\n", get_bootstate());
#ifdef CONFIG_DTB
	if (!is_resume())
	setup_kernel_params();
#else
	dbg_printf(PRN_NOTICE, "Warning: Refine DTB paramter step has been bypassed !!\n");
#endif
	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) main_bootloader_startkernel_stage(void)
{
	dbg_printf(PRN_RES, "STARTKERNEL_STAGE: get_bootstate() = 0x%x !\n",get_bootstate());
	Image_3_Jump();
	return BOOTLOADER_NULL_STAGE;
}

#ifdef CONFIG_FASTLOGO
static int __attribute__((used)) main_bootloader_normalreset_stage(void)
{
	MV_DISP_DestroyDisplay();
	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) main_bootloader_normalpowerdown_stage(void)
{
	MV_DISP_DestroyDisplay();
	return BOOTLOADER_NULL_STAGE;
}

DECLARE_RT_STAGE(BOOTLOADER_NORMAL_RESET_STAGE, main_normalreset_stage, 0, main_bootloader_normalreset_stage);
DECLARE_RT_STAGE(BOOTLOADER_NORMAL_POWERDOWN_STAGE, main_normalpowerdown_stage, 0, main_bootloader_normalpowerdown_stage);
#endif //CONFIG_FASTLOGO

DECLARE_RT_STAGE(BOOTLOADER_INITCHIP_PRESTAGE, main_initchip_prestage, 0, main_bootloader_initchip_prestage);
DECLARE_RT_STAGE(BOOTLOADER_INITCHIP_STAGE, main_initchip_stage, 0, main_bootloader_initchip_stage);
DECLARE_RT_STAGE(BOOTLOADER_INITSTORAGE_PRESTAGE, main_initstorage_prestage, 0, main_bootloader_initstorage_prestage);
DECLARE_RT_STAGE(BOOTLOADER_INITSTORAGE_STAGE, main_initstorage_stage, 0, main_bootloader_initstorage_stage);
DECLARE_RT_STAGE(BOOTLOADER_GETVERSIONTABLE_STAGE, main_getversiontable_stage, 0, main_bootloader_getversiontable_stage);
DECLARE_RT_STAGE(BOOTLOADER_INITFTS_STAGE, main_initfts_stage, 0, main_bootloader_initfts_stage);
DECLARE_RT_STAGE(BOOTLOADER_INITFTS_POSTSTAGE, main_initfts_poststage, 0, main_bootloader_initfts_poststage);
DECLARE_RT_STAGE(BOOTLOADER_LOADKERNEL_PRESTAGE, main_loadkernel_prestage, 0, main_bootloader_loadkernel_prestage);
DECLARE_RT_STAGE(BOOTLOADER_LOADKERNEL_STAGE, main_loadkernel_stage, 0, main_bootloader_loadkernel_stage);
DECLARE_RT_STAGE(BOOTLOADER_STARTKERNEL_PRESTAGE, main_startkernel_prestage, 0, main_bootloader_startkernel_prestage);
DECLARE_RT_STAGE(BOOTLOADER_STARTKERNEL_STAGE, main_startkernel_stage, 0, main_bootloader_startkernel_stage);
