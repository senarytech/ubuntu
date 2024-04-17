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
#include "chip_voltage_info.h"
#include "pmic_select.h"

static struct leakage_table table_vcpu_h [] =
{
	{	600,  960000},
	{	860,  920000},
	{	1120, 910000},
	{	1380, 900000},
	{	1640, 890000},
	{	1900, 880000},
	{	2160, 870000},
	{	2420, 860000},
	{	2680, 850000},
	{	3000, 840000},
	{	0xffff, 840000},
};

static struct leakage_table table_vcpu_l [] =
{
	{	600,  860000},
	{	860,  820000},
	{	1120, 810000},
	{	1380, 800000},
	{	1640, 790000},
	{	1900, 780000},
	{	2160, 770000},
	{	2420, 760000},
	{	2680, 750000},
	{	3000, 740000},
	{	0xffff, 740000},
};


static struct freq_leakage_table l_table_vcpu[] =
{
	//vl is at front and vh follows vl
	{1800, table_vcpu_l},
	{2100, table_vcpu_h},
	{0, NULL},
};

static struct leakage_table table_vcore_l [] =
{
	{	600,  860000},
	{	860,  840000},
	{	1120, 830000},
	{	1380, 820000},
	{	1640, 810000},
	{	1900, 800000},
	{	2160, 790000},
	{	2420, 780000},
	{	2680, 770000},
	{	3000, 760000},
	{	0xffff, 760000},
};

static struct leakage_table table_vcore_h [] =
{
	{	600,  910000},
	{	860,  890000},
	{	1120, 870000},
	{	1380, 860000},
	{	1640, 850000},
	{	1900, 840000},
	{	2160, 830000},
	{	2420, 820000},
	{	2680, 810000},
	{	3000, 800000},
	{	0xffff, 800000},
};

static struct freq_leakage_table l_table_vcore[] =
{
	{900, table_vcore_l},
	{1100, table_vcore_h},
	{0, NULL},
};

static struct leakage_table table_vcpu_h_step12500uv [] =
{
	{	600,  987500},
	{	860,  950000},
	{	1120, 925000},
	{	1380, 912500},
	{	1640, 912500},
	{	1900, 900000},
	{	2160, 887500},
	{	2420, 875000},
	{	2680, 862500},
	{	3000, 862500},
	{	0xffff, 862500},
};

static struct leakage_table table_vcpu_l_step12500uv [] =
{
	{	600,  887500},
	{	860,  850000},
	{	1120, 825000},
	{	1380, 812500},
	{	1640, 812500},
	{	1900, 800000},
	{	2160, 787500},
	{	2420, 775000},
	{	2680, 762500},
	{	3000, 762500},
	{	0xffff, 762500},
};

static struct leakage_table table_vcore_l_step12500uv [] =
{
	{	600,  887500},
	{	860,  862500},
	{	1120, 850000},
	{	1380, 837500},
	{	1640, 825000},
	{	1900, 812500},
	{	2160, 812500},
	{	2420, 800000},
	{	2680, 787500},
	{	3000, 775000},
	{	0xffff, 775000},
};

static struct leakage_table table_vcore_h_step12500uv [] =
{
	{	600,  912500},
	{	860,  900000},
	{	1120, 875000},
	{	1380, 862500},
	{	1640, 850000},
	{	1900, 850000},
	{	2160, 837500},
	{	2420, 825000},
	{	2680, 812500},
	{	3000, 800000},
	{	0xffff, 800000},
};

static struct freq_leakage_table l_table_vcpu_step12500uv[] =
{
	//vl is at front and vh follows vl
	{1800, table_vcpu_l_step12500uv},
	{2100, table_vcpu_h_step12500uv},
	{0, NULL},
};

static struct freq_leakage_table l_table_vcore_step12500uv[] =
{
	{900, table_vcore_l_step12500uv},
	{1100, table_vcore_h_step12500uv},
	{0, NULL},
};

struct freq_leakage_table * vs680_get_vcpu_leakage_table()
{
	unsigned int pmic_id = get_pmic_id(PMIC_SOC_VCPU);

	if(SY8827N == pmic_id)
		return l_table_vcpu_step12500uv;
	else
		return l_table_vcpu;
}

struct freq_leakage_table * vs680_get_vcore_leakage_table()
{
	unsigned int pmic_id = get_pmic_id(PMIC_SOC_VCORE);

	if(SY8827N == pmic_id)
		return l_table_vcore_step12500uv;
	else
		return l_table_vcore;
}

unsigned int vs680_get_default_cpu_voltage()
{
	unsigned int volt;
	volt = DEFAULT_CPU_VOLTAGE;
	return volt;
}

unsigned int vs680_get_default_core_voltage()
{
	unsigned int volt;
	volt = DEFAULT_CORE_VOLTAGE;
	return volt;
}

extern unsigned int get_leakage(void);
unsigned int get_leakage_info()
{
	unsigned int leakage;

	leakage = get_leakage();

	//FIXME: Remove this when leakage is programmed
	if (leakage == 0) {
		leakage = 1380;
	}

	return leakage;
}

int is_SPI_controller()
{
	return 0;
}
void pv_pinmux_setting()
{
	return;
}

