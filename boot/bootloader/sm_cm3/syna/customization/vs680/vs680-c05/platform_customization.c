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
#include "sm_gpio.h"

#include "platform_config.h"

void board_sdram_poweroff(void)
{
	if(sm_fxl16408_gpios_write(I2C_MASTER_ID, I2C_SLAVE_ADDR, \
			((1 << GPIO_VDDM) | \
			(1 << GPIO_VDDM_LPQ)), \
			((VDDM_OFF << GPIO_VDDM) | \
			(VDDM_LPQ_OFF << GPIO_VDDM_LPQ))) != 0)
		PRT_ERROR("%s: failed\n", __func__);
}

void board_sdram_poweron(void)
{
	if(sm_fxl16408_gpios_write(I2C_MASTER_ID, I2C_SLAVE_ADDR, \
			((1 << GPIO_VDDM) | \
			(1 << GPIO_VDDM_LPQ)), \
			((VDDM_ON << GPIO_VDDM) | \
			(VDDM_LPQ_ON << GPIO_VDDM_LPQ))) != 0)
		PRT_ERROR("%s: failed\n", __func__);
}

void board_sdram_refresh(void)
{
	if(sm_fxl16408_gpios_write(I2C_MASTER_ID, I2C_SLAVE_ADDR, \
			(1 << GPIO_VDDM_LPQ), \
			(VDDM_LPQ_OFF << GPIO_VDDM_LPQ)) != 0)
		PRT_ERROR("%s: failed\n", __func__);
}

void board_peripheral_assert(void)
{
	if(sm_fxl16408_gpios_write(I2C_MASTER_ID, I2C_SLAVE_ADDR, (1 << GPIO_PERI), (PERI_OFF << GPIO_PERI)) != 0)
		PRT_ERROR("%s: failed\n", __func__);
}

void board_peripheral_deassert(void)
{
	if(sm_fxl16408_gpios_write(I2C_MASTER_ID, I2C_SLAVE_ADDR, (1 << GPIO_PERI), (PERI_ON << GPIO_PERI)) != 0)
		PRT_ERROR("%s: failed\n", __func__);
}

void board_soc_poweron(void)
{
	if(sm_fxl16408_gpios_write(I2C_MASTER_ID, I2C_SLAVE_ADDR, ((1 << GPIO_VCPU) | (1 << GPIO_VCORE)),
		((VCPU_ON << GPIO_VCPU) | (VCORE_ON << GPIO_VCORE))) != 0)
		PRT_ERROR("%s: failed\n", __func__);
}

void board_soc_poweroff(void)
{
	if(sm_fxl16408_gpios_write(I2C_MASTER_ID, I2C_SLAVE_ADDR, ((1 << GPIO_VCPU) | (1 << GPIO_VCORE)),
		((VCPU_OFF << GPIO_VCPU) | (VCORE_OFF << GPIO_VCORE))) != 0)
		PRT_ERROR("%s: failed\n", __func__);
}

unsigned int board_wifibt_wakeup_monitor(int wifibt_gap)
{
	unsigned int is_wakeup = FALSE;

	if (sm_gpio_read(SM_GPIO_PORT_WIFIBT) == FALLING_EDGE && wifibt_gap >= WIFIBT_GAP_MAX) {
		is_wakeup = TRUE;
	}

	return is_wakeup;
}

