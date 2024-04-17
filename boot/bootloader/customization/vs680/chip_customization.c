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
#include "flash_self_adapt.h"

#include "global.h"
#include "debug.h"
#include "sys_mgr.h"

#include "system_manager.h"

#include "bootloader.h"

#include "galois_speed.h"

//#include "mem_map.h"
#include "apbRegBase.h"
#include "rt_stage.h"
#include "soc.h"
#include "avio_memmap.h"
#include "avioGbl.h"
#include "aio.h"
#include "string.h"

char chip_kernel_param[256];

extern int g_block_size;
extern int g_page_size;
extern int g_addr_cycle;

#ifdef PV_COMP
extern void pv_comp(int cpu_pll, int vmeta_pll);
extern void init_cpuPll(void);
#endif
extern void init_uart_base(UNSG32 uart_base);

char * get_chip_kernel_param(void)
{
        return chip_kernel_param;
}

static inline void set_sm_por_ctrl()
{
	writel(0x07, 0xF7FE2058); //Set POR reset bypass
}

void early_chip_init(void)
{
#ifdef CONFIG_FPGA
	init_dbg_port(SM_APB_UART0_BASE);
#else
	init_uart_base(SM_APB_UART0_BASE);
#endif
	set_sm_por_ctrl();
}
static void axi_cache_or(void)
{
	unsigned int addr = MEMMAP_SOC_REG_BASE + RA_SOC_cacheOR;

	writel(0xffff, addr + RA_cacheOR_CacheGFX3D0);
	writel(0xffff, addr + RA_cacheOR_CacheGFX3D1);
	writel(0xffff, addr + RA_cacheOR_CacheDECODER);
	writel(0xffff, addr + RA_cacheOR_CacheNPU);
	writel(0xffff, addr + RA_cacheOR_CacheACPU);
	writel(0xffff, addr + RA_cacheOR_CacheSOC2DDR);
	writel(0xffff, addr + RA_cacheOR_CacheENCODER);
	writel(0xffff, addr + RA_cacheOR_CacheOVP);
}

static void set_drive_strength(void)
{
	unsigned int offset, val, drv;

	for (offset = RA_Gbl_SDIO_CDnCntl; offset <= RA_Gbl_I2S3_DICntl; offset += 4)
	{
		switch (offset){
			case RA_Gbl_SDIO_CDnCntl ... RA_Gbl_SDIO_WPCntl:
			case RA_Gbl_TW0_SCLCntl ... RA_Gbl_TW0_SDACntl:
				drv = 0x7;
				break;
			case RA_Gbl_TX_EDDC_SCLCntl ... RA_Gbl_TX_EDDC_SDACntl:
				drv = 0x8;
				break;
			case RA_Gbl_RGMII_RXCTLCntl+4 ... RA_Gbl_I2S1_DO0Cntl - 4:
				continue;
			default:
				drv = 0x3;
				break;
		}

		val = readl(MEMMAP_CHIP_CTRL_REG_BASE + offset);
		val |= drv;
		writel(val, MEMMAP_CHIP_CTRL_REG_BASE + offset);
	}

	for (offset = RA_smSysCtl_SM_TW3_SCLCntl; offset <= RA_smSysCtl_SM_TW3_SDACntl; offset += 4 )
	{
		switch (offset){
			case RA_smSysCtl_SM_TW3_SCLCntl ... RA_smSysCtl_SM_TW3_SDACntl:
				drv = 0x7;
				break;
			default:
				continue;
		}

		val = readl(SOC_SM_SYS_CTRL_REG_BASE + offset);
		val |= drv;
		writel(val, SOC_SM_SYS_CTRL_REG_BASE + offset);
	}
}

/*enable master clocks default to 24.576Mhz*/
static void audio_mclk_config(void)
{
	/* copy below definiation from aio.h
		* Audio Port Control register
		###
		%unsigned 1  clk_Enable                0x0
							 ###
							 * Clock Enable
							 ###
							 : DISABLE                   0x0
							 : ENABLE                    0x1
		%unsigned 2  src_sel                   0x0
							 ###
							 * Source Clock Selection
							 * 0: APLL-0 is used as clock source
							 * 1: APLL-1 is used as clock source
							 * 2: External MCLK is used as clock source
							 ###
							 : AVPLL_A3                  0x0
							 : AVPLL_A4                  0x1
							 : MCLK_EXT                  0x2
		%unsigned 1  clkSwitch                 0x0
							 ###
							 * Clock Source Selection
							 * 0: use source clock
							 * 1: use divided clock (decided by clkD3Switch and clkSel)
							 ###
							 : SRC_CLK                   0x0
							 : DIV_CLK                   0x1
		%unsigned 1  clkD3Switch               0x0
							 ###
							 * Divide Factor
							 * 0 : divide factor is decided by clkSwitch and clkSel
							 * 1: use divide by 3 clock when clkSwitch = 1
							 ###
							 : DIV_NOR                   0x0
							 : DIV_3                     0x1
		%unsigned 3  clkSel                    0x0
							 ###
							 * Use the following divide factor if clkD3Switch=0
							 ###
							 : d2                        0x1
							 : d4                        0x2
							 : d6                        0x3
							 : d8                        0x4
							 : d12                       0x5
		%unsigned 1  sw_sync_rst               0x1
							 ###
							 * Soft Reset to Audio Modules
							 ###
							 : ASSERT                    0x0
							 : DEASSERT                  0x1
		%%        23         # Stuffing bits...
	*/
	unsigned int avio_gbl_base_addr = MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_GBL_BASE;
	unsigned int avio_i2s_base_addr = MEMMAP_AVIO_REG_BASE + AVIO_MEMMAP_AVIO_I2S_REG_BASE;

	/*the value of 0x189 is to divide apll - 196.608MHz by 8 and enable clock*/
	writel(0x189, avio_i2s_base_addr + RA_AIO_MCLKPRI);
	writel(0x189, avio_i2s_base_addr + RA_AIO_MCLKSEC);
	writel(0x189, avio_i2s_base_addr + RA_AIO_MCLKHD);
#ifdef CHIP_VER_Z1
	writel(0x189, avio_i2s_base_addr + RA_AIO_MCLKHDARCTX);
	writel(0x189, avio_i2s_base_addr + RA_AIO_MCLKSPF1);
#endif
	writel(0x189, avio_i2s_base_addr + RA_AIO_MCLKSPF);
	writel(0x189, avio_i2s_base_addr + RA_AIO_MCLKPDM);
	writel(0x189, avio_i2s_base_addr + RA_AIO_MCLKMIC1);
	writel(0x189, avio_i2s_base_addr + RA_AIO_MCLKMIC2);

	/*zero AVPLL_CTRL0 to have Normal operation, by default is power down status*/
	writel(0x0, avio_gbl_base_addr + RA_avioGbl_AVPLL_CTRL0);
	/*Clock controls for APLL CLK after APLL*/
	writel(0x269, avio_gbl_base_addr + RA_avioGbl_APLL_WRAP0);
	writel(0x269, avio_gbl_base_addr + RA_avioGbl_APLL_WRAP1);
	/*AVPLLA_CLK_EN -- Enable channel output*/
	writel(0x4D, avio_gbl_base_addr + RA_avioGbl_AVPLLA_CLK_EN);
	/*enable I2S1_MCLK_OEN& PDM_CLK_OEN to output mclk and PDM clock*/
	writel(0x2400, avio_gbl_base_addr + RA_avioGbl_CTRL1);
}

/******************************************************************
* INIT process fuctions of bootloader stage.
* Function used in stage should be declared by convinence macro
******************************************************************/
static int __attribute__((used)) init_bootloader_initchip_poststage(void)
{
#if defined(PV_COMP)
	pv_comp(CPUPLL, 1100);

	if(CPUPLL > 800)
		init_cpuPll();
#endif

	axi_cache_or();
	set_drive_strength();
	audio_mclk_config();
	return BOOTLOADER_NULL_STAGE;
}

DECLARE_RT_STAGE(BOOTLOADER_INITCHIP_POSTSTAGE, init_initchip_poststage, 0, init_bootloader_initchip_poststage);
