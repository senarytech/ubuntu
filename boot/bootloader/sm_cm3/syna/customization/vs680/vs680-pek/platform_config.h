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
#define I2C_SLAVE_ADDR 	0x70

/* I2C expander GPIO */
#define SOC_PWR_EN 		1
#define SDRAM_1P8_PWR_EN 	2
#define SDRAM_1P1_PWR_EN 	3
#define SDRAM_0P6_PWR_EN 	4
#define SDRAM_VDDQ_PWR_EN 	5
#define SDRAM_PWR_CTL 		6
#define PWR_LED_EN		7

#define PWR_ON 		1
#define PWR_OFF 		0

#define LED_ON 		0
#define LED_OFF 		1

//#define SM_GPIO_PORT_DDR_CTRL SDRAM_PWR_CTL

/* WiFi GPIO definitions*/
#define SM_GPIO_PORT_WIFIBT 6
#define SM_GPIO_PORT_GEPHY 7

#endif /* __PLATFORM_CONFIG_H__ */
