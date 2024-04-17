/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright � 2019-2020 Synaptics Incorporated. All rights reserved.
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
#include "sm_gpio.h"

#include "platform_config.h"

void board_sdram_poweroff(void)
{
    int retval = S_OK;
    retval = sm_gpio_set_output(SM_GPIO_VDDM, VDDM_OFF);
    if (retval != S_OK)
        PRT_ERROR("%s: failed\n", __func__);

    retval = sm_gpio_set_output(SM_GPIO_VDDM_LPQ, VDDM_LPQ_OFF);
    if (retval != S_OK)
        PRT_ERROR("%s: failed\n", __func__);
}

void board_sdram_poweron(void)
{
    int retval = S_OK;
    retval = sm_gpio_set_output(SM_GPIO_VDDM, VDDM_ON);
    if (retval != S_OK)
        PRT_ERROR("%s: failed\n", __func__);

    retval = sm_gpio_set_output(SM_GPIO_VDDM_LPQ, VDDM_LPQ_ON);
    if (retval != S_OK)
        PRT_ERROR("%s: failed\n", __func__);
}

void board_sdram_refresh(void)
{
    int retval = S_OK;
    retval = sm_gpio_set_output(SM_GPIO_VDDM_LPQ, VDDM_LPQ_OFF);
    if (retval != S_OK)
        PRT_ERROR("%s: failed\n", __func__);
}

void board_peripheral_assert(void)
{
}

void board_peripheral_deassert(void)
{
}

void board_soc_poweron(void)
{
    int retval = S_OK;
    retval = sm_gpio_set_output(SM_GPIO_VCPU, VCPU_ON);
    if (retval != S_OK)
        PRT_ERROR("%s: failed\n", __func__);

    retval = sm_gpio_set_output(SM_GPIO_VCORE, VCORE_ON);
    if (retval != S_OK)
        PRT_ERROR("%s: failed\n", __func__);
}

void board_soc_poweroff(void)
{
    int retval = S_OK;
    retval = sm_gpio_set_output(SM_GPIO_VCPU, VCPU_OFF);
    if (retval != S_OK)
        PRT_ERROR("%s: failed\n", __func__);

    retval = sm_gpio_set_output(SM_GPIO_VCORE, VCORE_OFF);
    if (retval != S_OK)
        PRT_ERROR("%s: failed\n", __func__);
}

unsigned int board_wifibt_wakeup_monitor(int wifibt_gap)
{
	return FALSE;
}

