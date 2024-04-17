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
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

#include "platform_config.h"

#include "sm_type.h"
#include "SysMgr.h"
#include "sm_common.h"

#include "sm_printf.h"
#include "sm_gpio.h"
#include "sm_state.h"
#include "sm_timer.h"

#include "sm_rt_module.h"
#include "sm_power.h"
#include "sm_func.h"

#ifndef WIFIBT_GAP_MIN
#define WIFIBT_GAP_MIN	64
#define WIFIBT_GAP_MAX	256
#define ETH_GAP_MAX	512
#endif

#define WIFIBT_STACK_SIZE  ((uint16_t) 128)

volatile BOOL is_wakeup = FALSE;

int wifibt_falling_time = 0;
int wifibt_gap = 0;

static TimerHandle_t wifibt_timer = NULL;
static SemaphoreHandle_t wifibt_sem = NULL;

static void sm_wifibt_timer_isr(TimerHandle_t xtimer)
{

	wifibt_gap = mv_sm_timer_gettimems() - wifibt_falling_time;

	is_wakeup = board_wifibt_wakeup_monitor(wifibt_gap);

	if (is_wakeup) {
		if(xTimerStop(wifibt_timer, 0) == pdFAIL)
			PRT_ERROR("wifibt timer can't be stopped\n");
		wifibt_falling_time = 0;
		wifibt_gap = WIFIBT_GAP_MAX - 10;
	}
}

static void __attribute__((used)) sm_wifibt_isr(void)
{
	sm_gpio_clearint(SM_GPIO_PORT_WIFIBT);

	if (wifibt_falling_time == 0)
		wifibt_falling_time = mv_sm_timer_gettimems_formISR();

	if ( wifibt_sem != NULL ) {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(wifibt_sem, &xHigherPriorityTaskWoken);
	}
}

DECLARE_GPIO_ISR(SM_GPIO_PORT_WIFIBT, EDGE_SENSITIVE, FALLING_EDGE, sm_wifibt_isr);

static int __attribute__((used)) power_eth_init(void)
{
	/* Clean up interrupt & set WiFiBT wake pin direction */
	sm_gpio_int_disable(SM_GPIO_PORT_WIFIBT);
	sm_gpio_clearint(SM_GPIO_PORT_WIFIBT);
	sm_gpio_set_input(SM_GPIO_PORT_WIFIBT);

	/* Create notify semaphore */
	wifibt_sem = xSemaphoreCreateBinary();
	if (wifibt_sem == NULL)
		PRT_ERROR("wifibt semaphore create failed\n");

	/* Crate Wifibt timer */
	if (wifibt_timer == NULL) {
		// Create timer during system init
		wifibt_timer = xTimerCreate("wifibt",
				pdMS_TO_TICKS(5), // 5ms
				pdTRUE,
				0,
				sm_wifibt_timer_isr);
	}
	if (wifibt_timer == NULL)
		PRT_ERROR("wifibt timer request failed\n");

	wifibt_falling_time = 0;

	sm_gpio_int_enable(SM_GPIO_PORT_WIFIBT);

	return S_OK;
}

DECLARE_RT_INIT(power_eth, INIT_DEV_P_25, power_eth_init);

static void wifibt_wake_task(void *para)
{
	while (1) {
		xSemaphoreTake(wifibt_sem, portMAX_DELAY);

		if (mv_sm_get_state() != MV_SM_STATE_ACTIVE) {
			if (wifibt_timer != NULL) {
				if ( xTimerIsTimerActive(wifibt_timer) == pdFALSE ) {
					if(xTimerStart(wifibt_timer, pdMS_TO_TICKS(1)) == pdFAIL)
						PRT_ERROR("wifibt timer can't be started\n");
				}
			}
		}
	}
}

static void __attribute__((used)) create_wifibt_wake_task(void)
{
	/* Create timer trigger task */
	xTaskCreate(wifibt_wake_task, "wifibt_wake", WIFIBT_STACK_SIZE, NULL,
		TASK_PRIORITY_2, NULL);
}

DECLARE_RT_MODULE(
	power_eth,
	MV_SM_ID_ETH,
	create_wifibt_wake_task,
	NULL,
	NULL
);

#ifdef GETHENABLE
volatile BOOL geth_wakeup = FALSE;

void sm_geth_isr(void)
{
	if(mv_sm_get_state() != MV_SM_STATE_ACTIVE){
		geth_wakeup = TRUE;
	}

	// Clear GPIO interrupt
	sm_gpio_clearint(SM_GPIO_PORT_GEPHY);
	PRT_INFO("@@@GEPHY GPIO INT!\n");
}

DECLARE_GPIO_ISR(SM_GPIO_PORT_GEPHY, EDGE_SENSITIVE, FALLING_EDGE, sm_geth_isr);
#endif

void eth_wifibt_wakeup_handler(void)
{
	if(is_wakeup) {
		switch (wifibt_gap) {
			case WIFIBT_GAP_MIN ... WIFIBT_GAP_MAX:
				mv_sm_power_setwakeupsource(MV_SM_WAKEUP_SOURCE_WIFI_BT);
				break;
			case WIFIBT_GAP_MAX + 1 ... ETH_GAP_MAX:
				mv_sm_power_setwakeupsource(MV_SM_WAKEUP_SOURCE_WOL);
				break;
			default:
				mv_sm_power_setwakeupsource(MV_SM_WAKEUP_SOURCE_INVALID);
				break;
		}
		mv_sm_power_enterflow_bysmstate();
		is_wakeup = FALSE;
	}

#ifdef GETHENABLE
	//FIXME: Keep original wifibt_gap for Ethernet wakeup temporarily until hw solution are fixed.
	if(geth_wakeup) {
		mv_sm_power_setwakeupsource(MV_SM_WAKEUP_SOURCE_WOL);
		mv_sm_power_enterflow_bysmstate();
		geth_wakeup = FALSE;
	}
#endif
}
