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

#ifndef __PLATFORM_CONFIG_H__
#define __PLATFORM_CONFIG_H__

#include "chip_config.h"

/* if there are platform specific config, please undef the original
   definition in cihp_config.h. then define the new one */

// U12, FLX6408, ADDR = 0. I2C 7-bit address: 0x43
// TW3, GPIO0_0, VCORE_ON#, active low.
// TW3, GPIO0_1, VCPU_ON#, active low.
// TW3, GPIO0_2, VDDM_ON#, active low.
// TW3, GPIO0_3, VDDM_LPQ_OFF, active high.
// TW3, GPIO0_4, STAND-BY_EN, active high.

/* Power pins definitions*/
#define I2C_MASTER_ID 	1
#define I2C_SLAVE_ADDR 	0x43

/* I2C expander GPIO */
#define GPIO_VCORE 	0
#define VCORE_ON 	0
#define VCORE_OFF 	1

#define GPIO_VCPU 	1
#define VCPU_ON 	0
#define VCPU_OFF 	1

#define GPIO_VDDM	2
#define VDDM_ON	0
#define VDDM_OFF	1

#define GPIO_VDDM_LPQ	3
#define VDDM_LPQ_ON	0
#define VDDM_LPQ_OFF	1

#define GPIO_PERI 	4
#define PERI_ON 	0
#define PERI_OFF 	1

#define SM_GPIO_PORT_DDR_CTRL GPIO_VDDM

/* WiFi GPIO definitions*/
#define SM_GPIO_PORT_WIFIBT 6
#define SM_GPIO_PORT_GEPHY 7

#define WIFIBT_GAP_MIN	64
#define WIFIBT_GAP_MAX	128
#define ETH_GAP_MAX	512

/* ADC button press time threshold*/
#define ADC_BTN_SW1_SHORT_PRESS_ACTION_TIME	500
#define ADC_BTN_SW1_LONG_PRESS_ACTION_TIME	5*1000
#define ADC_BTN_SW2_SHORT_PRESS_ACTION_TIME	100
#define ADC_BTN_SW2_LONG_PRESS_ACTION_TIME	5*1000
#define ADC_BTN_SW3_SHORT_PRESS_ACTION_TIME	100
#define ADC_BTN_SW3_LONG_PRESS_ACTION_TIME	5*1000
#define ADC_BTN_SW4_SHORT_PRESS_ACTION_TIME	100
#define ADC_BTN_SW4_LONG_PRESS_ACTION_TIME	5*1000
#define LONG_PRESS_TIMEOUT			10*1000

/* ADC button IR Key */
#define ADC_BTN_SW1_SHORT_PRESS_KEY	MV_IR_KEY_BEGIN_PAIRING
#define ADC_BTN_SW1_LONG_PRESS_KEY 	MV_IR_KEY_NULL
#define ADC_BTN_SW2_SHORT_PRESS_KEY	MV_IR_KEY_POWER
#define ADC_BTN_SW2_LONG_PRESS_KEY 	MV_IR_KEY_RECOVERY
#define ADC_BTN_SW3_SHORT_PRESS_KEY	MV_IR_KEY_VOL_MINUS
#define ADC_BTN_SW3_LONG_PRESS_KEY 	MV_IR_KEY_NULL
#define ADC_BTN_SW4_SHORT_PRESS_KEY	MV_IR_KEY_VOL_PLUS
#define ADC_BTN_SW4_LONG_PRESS_KEY 	MV_IR_KEY_NULL

#endif /* __PLATFORM_CONFIG_H__ */
