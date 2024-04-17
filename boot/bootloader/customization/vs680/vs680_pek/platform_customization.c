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
#include "bootloader.h"
#include "gpio_defs.h"
#include "debug.h"
#include "global.h"
#include "apbRegBase.h"
#include "SysMgr.h"
#include "rt_stage.h"
#include "misc.h"
#include "gpio.h"

#include "dev_management.h"
#if defined(PV_COMP)
#include "pmic_select.h"
#endif

unsigned int get_board_ver(void)
{
	unsigned int board_version = 0xffff;
#define BOOT_STRAP_STB_REVISION_MASK    0xE
#define BOOT_STRAP_STB_REVISION_SHIFT   0x1
	REG_READ32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_bootStrap), &board_version);//0xf7ea000c, bit 1-3
	board_version = ((board_version & BOOT_STRAP_STB_REVISION_MASK) >> BOOT_STRAP_STB_REVISION_SHIFT);
	dbg_printf(PRN_ERR, "**********board_version( 0x%2x)\n", board_version);
	return board_version;
}

extern void pin_init();

static inline void set_power_sd1(int value)
{
}

static inline void set_power_hdmi(int value)
{
}

static inline void set_power_CX20921(int value)
{
}

static inline void set_ge_dll(void)
{
}

static void platform_setup_kernel_param()
{
    //setup board_kernel_param
}

/******************************************************************
* PLATFORM process fuctions of bootloader stage.
* Function used in stage should be declared by convinence macro
******************************************************************/
static int __attribute__((used)) plat_bootloader_initchip_stage(void)
{
	//set_drive_strength();
	pin_init();
	set_ge_dll();

#if defined(PV_COMP)
	unsigned int board_version = get_board_ver();

	if((0x3 == board_version) || (0x4 == board_version)) {
		dbg_printf(PRN_INFO, "INFO: STB Revision 1.0/1.1 Detected!\n");
		set_pmic_id(M88PG86X);
	}
	else if((0x2 == board_version) || (0x7 == board_version)) {
		dbg_printf(PRN_INFO, "INFO: STB Revision 3.0/4.0 Detected!\n");
		set_enable_pmic_id(SY8824B);
	}
	else
		dbg_printf(PRN_ERR, "WARN: unsupported STB Revision for pv_comp!\n");
#endif
	//dbg_printf(PRN_RES,"\n%s %s [%s %s]\n", BOARD_TYPE, CHIP_VERSION, __DATE__, __TIME__);
#if defined(DEBUG)
	//if ((uiBoot != MV_SoC_STATE_WARMUP_1) || (uiWarmDown_2_Linux_Addr == 0xFFFFFFFF)) {
	//    dump_pinmux_cfg();
	//}
#endif
	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) plat_bootloader_initchip_poststage(void)
{
	//Powerdown FE and SATA modules for power saving purpose
	//powerdown_eth();
	//powerdown_sata();

	if (!is_resume())
		set_power_sd1(0);
	set_power_hdmi(1);
	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) plat_bootloader_loadkernel_prestage(void)
{
	//FIXME: here to setup board specific kernel params
	platform_setup_kernel_param();
	return BOOTLOADER_NULL_STAGE;
}
static int __attribute__((used)) plat_bootloader_loadkernel_poststage(void)
{
	set_power_sd1(1);
	set_power_hdmi(1);
	return BOOTLOADER_NULL_STAGE;
}

DECLARE_RT_STAGE(BOOTLOADER_INITCHIP_STAGE, plat_initchip_stage, 0, plat_bootloader_initchip_stage);
DECLARE_RT_STAGE(BOOTLOADER_INITCHIP_POSTSTAGE, plat_initchip_poststage, 0, plat_bootloader_initchip_poststage);
DECLARE_RT_STAGE(BOOTLOADER_LOADKERNEL_PRESTAGE, plat_loadkernel_prestage, 0, plat_bootloader_loadkernel_prestage);
DECLARE_RT_STAGE(BOOTLOADER_LOADKERNEL_POSTSTAGE, plat_loadkernel_poststage, 0, plat_bootloader_loadkernel_poststage);
