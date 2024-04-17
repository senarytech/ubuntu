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
#include "bootloader_customize.h"
#include "debug.h"

#define REG_ADAC_POWER_REG      0xF7D30020

#define REG_LVDS0_POWER_REG     0xF7F70098
#define REG_LVDS1_POWER_REG     0xF7F700A0

#define REG_VDACA_POWER_REG     0xF7F7005C
#define REG_VDACB_POWER_REG     0xF7F70064
#define REG_VDACC_POWER_REG     0xF7F7006C
#define REG_VDACD_POWER_REG     0xF7F70074

/**
 * Disable (Power Down) the Audio DAC/VIDEO DAC/LVDS
 * for RDK DMP, because there is no these
 * interface and cause EMI noise.
 */

static void disable_unused_modules(void)
{
	long volatile reg_val;


	//disable ADAC
	dbg_printf(PRN_RES,"disable ADAC\n");

	reg_val = readl(REG_ADAC_POWER_REG);
	reg_val &= ~(0x1);
	writel(reg_val, REG_ADAC_POWER_REG);

	//disable LVDS
	dbg_printf(PRN_RES,"disable LVDS\n");

	//LVDS0
	reg_val = readl(REG_LVDS0_POWER_REG);
	reg_val |= (0x3F);
	reg_val &= ~(0x7 << 26);
	writel(reg_val, REG_LVDS0_POWER_REG);

	//LVDS1
	reg_val = readl(REG_LVDS1_POWER_REG);
	reg_val |= (0x3F);
	reg_val &= ~(0x7 << 26);
	writel(reg_val, REG_LVDS1_POWER_REG);

	//disable VDAC
	dbg_printf(PRN_RES,"disable VDAC\n");

	//VDAC_A
	reg_val = readl(REG_VDACA_POWER_REG);
	reg_val &= ~(0x1 << 8);
	writel(reg_val, REG_VDACA_POWER_REG);

	//VDAC_B
	reg_val = readl(REG_VDACB_POWER_REG);
	reg_val &= ~(0x1 << 8);
	writel(reg_val, REG_VDACB_POWER_REG);

	//VDAC_C
	reg_val = readl(REG_VDACC_POWER_REG);
	reg_val &= ~(0x1 << 8);
	writel(reg_val, REG_VDACC_POWER_REG);

	//VDAC_D
	reg_val = readl(REG_VDACD_POWER_REG);
	reg_val &= ~(0x1 << 8);
	writel(reg_val, REG_VDACD_POWER_REG);
}

void power_on_sdio_slot0(void)
{
	u32 val;
	/* Check whether it is B0 or above board revision */
	val = *(volatile unsigned int *)0xF7EA01D0;

	if (val >= 0xB0)  {
	    GPIO_PortSetInOut(1, 0);
	    GPIO_PortWrite(1, 1);
	} else {
	    GPIO_PortSetInOut(6, 0);
	    GPIO_PortWrite(6, 1);
	}

}

void power_on_sdio_slot1(void)
{
	GPIO_PortSetInOut(7, 0);
	GPIO_PortWrite(7, 1);
}

void test_tune_syspll(void)
{
	// here is an example to tune syspll
	//
	// you can use this code to test tune syspll.
	// after you have tune a suitable set parameters,
	// please
	// 1. feedback to us, we will build it in sys_init.
	// 2. disable this code.
	writel(0x00FB3415, 0xF7EA0020);
	writel(0x00080000, 0xF7EA001C);
	writel(0x01C8012A, 0xF7EA0018);
	writel(0x0, 0xF7EA0024);
	writel(0x0, 0xF7EA0178);
	writel(0x40, 0xF7EA0178);
	writel(0x0, 0xF7EA0178);
}

void bootloader_customize_initialize_postprocess(unsigned int boot_state)
{
	// test_tune_syspll();	// enable the syspll tune code

	// power down unused modules
	disable_unused_modules();

	power_on_sdio_slot1();

	if ((boot_state != MV_SoC_STATE_WARMDOWN_1) &&
		(boot_state != MV_SoC_STATE_WARMDOWN_2) &&
		!((boot_state == MV_SoC_STATE_WARMUP_1) && (uiWarmDown_2_Linux_Addr != 0xFFFFFFFF)))
		power_on_sdio_slot0();
}

void bootloader_customize_load_image_preprocess(unsigned int boot_state)
{
}

void bootloader_customize_load_image_postprocess(unsigned int boot_state)
{
}

void bootloader_customize_response_to_sm_preprocess(unsigned int boot_state)
{
}

void bootloader_customize_start_kernel_preprocess(unsigned int boot_state)
{
}
