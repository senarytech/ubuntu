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
#include "bootloader_header.h"
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
#include "libfdt.h"
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

static void select_i2c_port(void)
{
	//select i2c port on Linux kernel's behalf
	writel(0x20, 0xF7EA0040); //for vcpu pmic i2c port select
	writel(0x01, 0xF7FE2050); //for vcore pmic i2c port select
}

static inline void set_power_sd1(int value)
{
}

static inline void set_power_hdmi(int value)
{
    SM_GPIO_PortWrite(SM_GPIO_PORT_HDMI_TX_5V, value);
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

#ifdef PV_COMP
static void update_pmic(void *fdt)
{
	int offset = 0;
	unsigned int vcpu_pmic_id  = get_pmic_id(PMIC_SOC_VCPU);
	unsigned int vcore_pmic_id = get_pmic_id(PMIC_SOC_VCORE);
	unsigned int default_pmic_vcpu  = get_default_pmic_id(PMIC_SOC_VCPU);
	unsigned int default_pmic_vcore = get_default_pmic_id(PMIC_SOC_VCORE);

	NOTICE("Platform VCPU default PMIC : %s, VCORE default PMIC: %s\n", \
		get_pmic_name(default_pmic_vcpu), get_pmic_name(default_pmic_vcore));
	NOTICE("Platform VCPU actual  PMIC : %s, VCORE actual  PMIC: %s\n", \
		get_pmic_name(vcpu_pmic_id), get_pmic_name(vcore_pmic_id));

	// Update VCPU PMIC if changed
	if (vcpu_pmic_id != default_pmic_vcpu) {
		INFO("Select PMIC chip %s for Kernel !\n", get_pmic_name(vcpu_pmic_id));

		/* disable MP886X */
		offset = fdt_node_offset_by_compatible(fdt, -1, "mps,mp8867");
		while(offset != -FDT_ERR_NOTFOUND) {
			fdt_del_node(fdt, offset);
			offset = fdt_node_offset_by_compatible(fdt, offset, "mps,mp8867");
		}

		if(SY8827N == vcpu_pmic_id) {
			/* enable SY8827N */
			offset = fdt_node_offset_by_compatible(fdt, -1, "silergy,sy8827n");
			while(offset != -FDT_ERR_NOTFOUND) {
				uint32_t phandle = fdt_get_phandle(fdt, offset);
				fdt_setprop_string(fdt, offset, "status", "okay");

				if((phandle != (uint32_t)(-1)) && phandle) {
					const char *name = fdt_getprop(fdt, offset, "regulator-name", NULL);
					if(name && !strcmp(name, "vcpu")) {
						int cpu0_offset = fdt_path_offset(fdt, "/cpus/cpu@0");
						fdt_setprop_u32(fdt, cpu0_offset, "cpu0-supply", phandle);
					}
				}

				offset = fdt_node_offset_by_compatible(fdt, offset, "silergy,sy8827n");
			}
		}
		else
			NOTICE("FIXME: Add Vcore PMIC update code for get_pmic_name(vcore_pmic_id)\n");
	}

	// Update VCORE PMIC if changed
	if (vcore_pmic_id != default_pmic_vcore) {
		INFO("Select PMIC chip %s for Kernel !\n", get_pmic_name(vcore_pmic_id));
		/* VS680 SW doesn't support VCORE DVFS feature currently, so needn't to
		 * change VCORE PMIC regulator information in DTB.
		 * If VS680 SW supports VCORE DVFS feature, need to add PMIC update code in here.
		 */
	}
}
#endif
/******************************************************************
* PLATFORM process fuctions of bootloader stage.
* Function used in stage should be declared by convinence macro
******************************************************************/
static int __attribute__((used)) plat_bootloader_initchip_stage(void)
{
	//set_drive_strength();
	pin_init();
	set_ge_dll();

	select_i2c_port();

#if defined(PV_COMP)
	//Default VCPU and VCORE PMIC aligned to DTS Setting
	set_default_pmic_id(MP886X, MP886X);

	unsigned int board_version = get_board_ver();
	if(0x0 == board_version) {
		NOTICE("INFO: STB Revision 1.0/1.01 Board Detected!\n");
		set_pmic_id(MP886X, MP886X);
	}
	else if(0x4 == board_version) {
		NOTICE("INFO: STB Revision 2.0(3'b100) Board Detected!\n");
		set_pmic_id(SY8827N, SY8827N);
	}
	else if(0x5 == board_version) {
		NOTICE("INFO: STB Revision 2.0(3'b101) Board Detected!\n");
		set_pmic_id(MP886X, MP886X);
	}
	else
		ERR("WARN: unsupported STB Revision for pv_comp!\n");
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

static int __attribute__((used)) plat_bootloader_startkernel_prestage(void)
{
#ifdef PV_COMP
	update_pmic((void *)(uintptr_t)get_dtb_addr());
#endif
	return BOOTLOADER_NULL_STAGE;
}
DECLARE_RT_STAGE(BOOTLOADER_INITCHIP_STAGE, plat_initchip_stage, 0, plat_bootloader_initchip_stage);
DECLARE_RT_STAGE(BOOTLOADER_INITCHIP_POSTSTAGE, plat_initchip_poststage, 0, plat_bootloader_initchip_poststage);
DECLARE_RT_STAGE(BOOTLOADER_LOADKERNEL_PRESTAGE, plat_loadkernel_prestage, 0, plat_bootloader_loadkernel_prestage);
DECLARE_RT_STAGE(BOOTLOADER_LOADKERNEL_POSTSTAGE, plat_loadkernel_poststage, 0, plat_bootloader_loadkernel_poststage);
DECLARE_RT_STAGE(BOOTLOADER_STARTKERNEL_PRESTAGE, plat_startkernel_prestage, 0, plat_bootloader_startkernel_prestage);
