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

#include "debug.h"
#include "load_gpt.h"
#include "boot_mode.h"
#include "ptinfo.h"

typedef struct {
	int ipt;
	char name[16];
}pt_mapping;

static pt_mapping pt_mapping_table[] = {
#ifndef CONFIG_AB_BOOTFLOW
	{PT_KEY, KEY1_NAME},
	{PT_KEY + 1, KEY2_NAME},
	{PT_FASTLOGO, LOGO_NAME},
	{PT_FASTLOGO + 1, LOGO_NAME},
	{PT_BOOT, KERNEL_NAME},
	{PT_BOOT + 1, RECOVERY_NAME},
	{PT_DTBO, DTBO_NAME},
	{PT_DTBO + 1, DTBO_NAME},
#else
	{PT_KEY, KEY_A_NAME},
	{PT_KEY + 1, KEY_B_NAME},
	{PT_FASTLOGO, LOGO_A_NAME},
	{PT_FASTLOGO + 1, LOGO_B_NAME},
	{PT_BOOT, KERNEL_A_NAME},
	{PT_BOOT + 1, KERNEL_B_NAME},
	{PT_DTBO, DTBO_A_NAME},
	{PT_DTBO + 1, DTBO_B_NAME},
#endif
};

void * get_pt_name(int ipt)
{
	int add = 0;
	if((ipt < PT_INDEX_MIN) || (ipt > PT_INDEX_MAX))
		return NULL;

#ifdef CONFIG_AB_BOOTFLOW
	add = (get_abmode() == BOOTSEL_A) ? ipt : (ipt + 1);
#else
	add = (get_bootmode() == BOOTMODE_NORMAL) ? ipt : (ipt + 1); //only normal and recovery mode
#endif

	if(add == pt_mapping_table[add].ipt) {
		return pt_mapping_table[add].name;
	}

	return NULL;
}

int get_pt_index(int ipt)
{
	int ret = -1;
	void * ptname;

	ptname = get_pt_name(ipt);
	if(NULL == ptname) {
		goto done;
	}

	ret = find_partition(ptname);
done:
	return ret;
}

char * get_pt_suffix()
{
#ifdef CONFIG_AB_BOOTFLOW
	if(get_abmode() == BOOTSEL_A)
		return "_a";
	else
		return "_b";
#else
	return "";
#endif
}

char * get_boot_prefix()
{
#ifndef CONFIG_AB_BOOTFLOW
	if(get_bootmode() != BOOTMODE_NORMAL)
		return "recovery";
#endif
	return "boot";
}

char * get_dtbo_prefix()
{
#ifdef CONFIG_ENABLE_DTBO
#ifdef CONFIG_NR_BOOTFLOW
	if(get_bootmode() != BOOTMODE_NORMAL)
		return NULL;
#endif
	return "dtbo";
#else

	return NULL;
#endif
}

char * get_vendor_boot_prefix()
{
	return "vendor_boot";
}

char * get_init_boot_prefix()
{
	return "init_boot";
}
