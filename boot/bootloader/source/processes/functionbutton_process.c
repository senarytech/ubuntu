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
#ifdef CONFIG_FUNCTION_BUTTON_ENABLE
#include "system_manager.h"
#include "bootloader.h"
#include "debug.h"

#include "misc.h"
#include "boot_mode.h"
#include "rt_stage.h"
#ifndef CONFIG_SM
#include "apb_watchdog.h"
#endif

extern int is_EnterRecovery;
extern void init_function_button();
extern void check_function_button();

/******************************************************************
* FUNCTIONBUTTON process fuctions of bootloader stage.
* Function used in stage should be declared by convinence macro
******************************************************************/
static int __attribute__((used)) functionbutton_bootloader_initchip_poststage(void)
{
	init_function_button();
	if( is_coldbootup() ||
			( (!is_standby_2_active()) &&
			(!is_resume()) )) {
		/* the first time to check function button */
		check_function_button();
	}
    return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) functionbutton_bootloader_getversiontable_prestage(void)
{
	/* the second time to check function button */
	check_function_button();
    return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) functionbutton_bootloader_loadkernel_prestage(void)
{
	/* the third time to check function button */
	check_function_button();
	if(is_EnterRecovery >= 1) {
#if defined(CONFIG_FUNCTION_BUTTON_FASTBOOT)
		set_bootmode(BOOTMODE_FASTBOOT);
#else
		set_bootmode(BOOTMODE_RECOVERY);
#endif

#ifdef CONFIG_SM
		return BOOTLOADER_NORMAL_RESET_STAGE;
#else
		reset_soc();
#endif
	}
    return BOOTLOADER_NULL_STAGE;
}

DECLARE_RT_STAGE(BOOTLOADER_INITCHIP_POSTSTAGE, funcbut_initchip_poststage, 0, functionbutton_bootloader_initchip_poststage);
DECLARE_RT_STAGE(BOOTLOADER_GETVERSIONTABLE_PRESTAGE, funcbut_getversiontable_prestage, 0, functionbutton_bootloader_getversiontable_prestage);
DECLARE_RT_STAGE(BOOTLOADER_LOADKERNEL_PRESTAGE, funcbut_loadkernel_prestage, 0, functionbutton_bootloader_loadkernel_prestage);
#endif
