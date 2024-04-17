/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2019-2020 Synaptics Incorporated. All rights reserved.
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

#include "sm_type.h"
#include "sm_printf.h"
#include "sm_func.h"
#include "sm_fxl6408.h"

#include "platform_config.h"

void board_sdram_poweroff(void)
{
	/*Pull up SDRAM_PWR_CTL*/
	if(sm_fxl16408_gpios_write(I2C_MASTER_ID, I2C_SLAVE_ADDR, (1 << SOC_PWR_EN),
			(PWR_OFF << SOC_PWR_EN)) != 0)
		PRT_ERROR("%s: SDRAM_PWR_CTL enable failed\n", __func__);

	/*Power down SDRAM*/
	if(sm_fxl16408_gpios_write(I2C_MASTER_ID, I2C_SLAVE_ADDR, \
			((1 << SDRAM_1P8_PWR_EN) | \
			(1 << SDRAM_1P1_PWR_EN) | \
			(1 << SDRAM_0P6_PWR_EN) | \
			(1 << SDRAM_VDDQ_PWR_EN)), \
			((PWR_OFF << SDRAM_1P8_PWR_EN) | \
			(PWR_OFF << SDRAM_1P1_PWR_EN) | \
			(PWR_OFF << SDRAM_0P6_PWR_EN) | \
			(PWR_OFF << SDRAM_VDDQ_PWR_EN))) != 0)
		PRT_ERROR("%s: SDRAM power offfailed\n", __func__);

	/*Pull down SDRAM_PWR_CTL*/
	if(sm_fxl16408_gpios_write(I2C_MASTER_ID, I2C_SLAVE_ADDR, (1 << SDRAM_PWR_CTL),
			(PWR_OFF << SDRAM_PWR_CTL)) != 0)
		PRT_ERROR("%s: SDRAM_PWR_CTL disable failed\n", __func__);
}

void board_sdram_poweron(void)
{
	/*Pull up SDRAM_PWR_CTL*/
	if(sm_fxl16408_gpios_write(I2C_MASTER_ID, I2C_SLAVE_ADDR, (1 << SDRAM_PWR_CTL),
			(PWR_ON << SDRAM_PWR_CTL)) != 0)
		PRT_ERROR("%s: SDRAM_PWR_CTL enable failed\n", __func__);

	/*Power on SDRAM*/
	if(sm_fxl16408_gpios_write(I2C_MASTER_ID, I2C_SLAVE_ADDR, \
			((1 << SDRAM_1P8_PWR_EN) | \
			(1 << SDRAM_1P1_PWR_EN) | \
			(1 << SDRAM_0P6_PWR_EN) | \
			(1 << SDRAM_VDDQ_PWR_EN)), \
			((PWR_ON << SDRAM_1P8_PWR_EN) | \
			(PWR_ON << SDRAM_1P1_PWR_EN) | \
			(PWR_ON << SDRAM_0P6_PWR_EN) | \
			(PWR_ON << SDRAM_VDDQ_PWR_EN))) != 0)
		PRT_ERROR("%s: SDRAM power on failed\n", __func__);

	/*Pull down SDRAM_PWR_CTL*/
	if(sm_fxl16408_gpios_write(I2C_MASTER_ID, I2C_SLAVE_ADDR, (1 << SDRAM_PWR_CTL),
			(PWR_OFF << SDRAM_PWR_CTL)) != 0)
		PRT_ERROR("%s: SDRAM_PWR_CTL disable failed\n", __func__);
}

void board_sdram_refresh(void)
{
}

void board_peripheral_assert(void)
{
}

void board_peripheral_deassert(void)
{
}

void board_soc_poweron(void)
{
	/*Power on SOC*/
	if(sm_fxl16408_gpios_write(I2C_MASTER_ID, I2C_SLAVE_ADDR, (1 << SOC_PWR_EN),
		(PWR_ON << SOC_PWR_EN)) != 0)
		PRT_ERROR("%s: SOC power on failed\n", __func__);

	/*Power on LED*/
	if(sm_fxl16408_gpios_write(I2C_MASTER_ID, I2C_SLAVE_ADDR, (1 << PWR_LED_EN),
		(LED_ON << PWR_LED_EN)) != 0)
		PRT_ERROR("%s: LED turn on failed\n", __func__);
}

void board_soc_poweroff(void)
{
	/*Power off SOC*/
	if(sm_fxl16408_gpios_write(I2C_MASTER_ID, I2C_SLAVE_ADDR, (1 << SOC_PWR_EN),
		(PWR_OFF << SOC_PWR_EN)) != 0)
		PRT_ERROR("%s: SOC power off failed\n", __func__);

	/*Power off LED*/
	if(sm_fxl16408_gpios_write(I2C_MASTER_ID, I2C_SLAVE_ADDR, (1 << PWR_LED_EN),
		(LED_OFF << PWR_LED_EN)) != 0)
		PRT_ERROR("%s: LED turn off failed\n", __func__);
}

unsigned int board_wifibt_wakeup_monitor(int wifibt_gap)
{
	return FALSE;
}

