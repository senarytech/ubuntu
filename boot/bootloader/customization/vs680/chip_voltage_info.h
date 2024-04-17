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
#ifndef _CHIP_VOLTAGE_INFO_H_
#define _CHIP_VOLTAGE_INFO_H_

#include <io.h>
#include "debug.h"

#include "voltage_info.h"
#include "platform_voltage_info.h"

#define CORE_VOLT_VALUE

#ifndef DEFAULT_CPU_VOLTAGE
#define DEFAULT_CPU_VOLTAGE    (1000)
#endif

#ifndef DEFAULT_CORE_VOLTAGE
#define DEFAULT_CORE_VOLTAGE    (1050)
#endif

#define VOUT_CPU_ID				(1)

#define VOUT_CORE_ID			(2)

#ifndef get_vcpu_leakage_table
#define get_vcpu_leakage_table vs680_get_vcpu_leakage_table
struct freq_leakage_table * vs680_get_vcpu_leakage_table();
#endif

#ifndef get_vcore_leakage_table
#define get_vcore_leakage_table vs680_get_vcore_leakage_table
struct freq_leakage_table * vs680_get_vcore_leakage_table();
#endif

#ifndef get_default_cpu_voltage
#define get_default_cpu_voltage vs680_get_default_cpu_voltage
unsigned int vs680_get_default_cpu_voltage();
#endif

#ifndef get_default_core_voltage
#define get_default_core_voltage vs680_get_default_core_voltage
unsigned int vs680_get_default_core_voltage();
#endif

unsigned int get_leakage_info();
int is_SPI_controller();
void pv_pinmux_setting();

#endif

