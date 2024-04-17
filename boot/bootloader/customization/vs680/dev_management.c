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

#include "global.h"
#include "apbRegBase.h"

#include "dev_management.h"

#if 0
void powerdown_eth(void)
{
	// disable clock for mac, bit 4
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkEnable, MSK32Gbl_clkEnable_ethCoreClkEn, 0x0);

	// power down PHY
	reg_rmw((SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_FEPHY_CTRL), MSK32smSysCtl_FEPHY_CTRL_ext_pwrdn_a, 0x1);

	//Power down Sram_eth
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE +  RA_Gbl_SRAM_PWR_CTRL_ETH, 0x7, 0x6);
}

void powerdown_sata(void)
{
	// clock enable, bit 7 of clkEnable
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkEnable, MSK32Gbl_clkEnable_sata0CoreClkEn, (0x1 << 7));

	//SATA_HOST_VENDOR_SPEC_ADDR, reference from Diag
	REG_WRITE32((MEMMAP_SATA_REG_BASE + 0xa0), 0x0);

	//SATA_HOST_VENDOR_SPEC_DATA, bit 6 for SATA 0 power down; bit 14 for SATA 1
	reg_rmw((MEMMAP_SATA_REG_BASE + 0xa4), (0x1 << 6), (0x1 << 6));

	// clock disable
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkEnable, MSK32Gbl_clkEnable_sata0CoreClkEn, 0x0);

	// Power down Sram_sata
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE +  RA_Gbl_SRAM_PWR_CTRL_SATA, 0x7, 0x6);

}

void powerdown_pcie(void)
{
	reg_rmw(MEMMAP_PCIE_REG_BASE + RA_PCIE_PHY_CTRL,
				MSK32PCIE_PHY_CTRL_PIPE_ENABLE, 0x00);
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkEnable,
				MSK32Gbl_clkEnable_pcie0SysClkEn, 0x00);
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pcieTestClk,
				MSK32clkD4_ctrl_ClkEn, 0x00);
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE +  RA_Gbl_SRAM_PWR_CTRL_PCIE,
				MSK32Gbl_SRAM_PWR_CTRL_PCIE_value, 0x06);
}

void powerdown_nand(void)
{
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_nfcEccClk,
				MSK32clkD2_ctrl_ClkEn, 0x00);
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkEnable,
				MSK32Gbl_clkEnable_nfcSysClkEn, 0x00);
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE +  RA_Gbl_SRAM_PWR_CTRL_NFC,
				MSK32Gbl_SRAM_PWR_CTRL_NFC_value, 0x06);
}

void powerdown_usim(void)
{
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_usim0Clk,
				MSK32clkD4_ctrl_ClkEn, 0x00);
}

void powerdown_ge(void)
{
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gethRgmiiClk,
				MSK32clkD8_ctrl_ClkEn, 0x00);
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gethRgmiiSysClk,
				MSK32clkD4_ctrl_ClkEn, 0x00);
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE +  RA_Gbl_SRAM_PWR_CTRL_GE,
				MSK32Gbl_SRAM_PWR_CTRL_GE_value, 0x06);
}

void powerdown_sdio0(void)
{
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sd0Clk,
				MSK32clkD4_ctrl_ClkEn, 0x00);
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE +  RA_Gbl_SRAM_PWR_CTRL_SDIO0,
				MSK32Gbl_SRAM_PWR_CTRL_SDIO0_value, 0x06);
}

void powerdown_sdio1(void)
{
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sd1Clk,
				MSK32clkD4_ctrl_ClkEn, 0x00);
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE +  RA_Gbl_SRAM_PWR_CTRL_SDIO1,
				MSK32Gbl_SRAM_PWR_CTRL_SDIO1_value, 0x06);
}

void powerdown_usb3(void)
{
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_usb3CoreClk,
				MSK32clkD12_ctrl_ClkEn, 0x00);
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_usb3TestClk,
				MSK32clkD12_ctrl_ClkEn, 0x00);
}

void powerdown_rgmii(void)
{
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gethRgmiiClk,
				MSK32clkD8_ctrl_ClkEn, 0x00);
	reg_rmw(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gethRgmiiSysClk,
				MSK32clkD4_ctrl_ClkEn, 0x00);
}
#endif
