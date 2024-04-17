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
#include "com_type.h"
#include "io.h"
#include "bl_boot_arg.h"
#include "chip_id.h"
#include "debug.h"
#include "libfdt_syna.h"

extern void set_flash_parameters(int blocksize, int pagesize, int addrcycle);

static unsigned long resume_addr = 0xFFFFFFFF;
static unsigned int bgs = 0x0;
static system_info_t system_info = {0};
static unsigned int soc_tsen_id = 105;

unsigned int get_leakage(void) {
	return system_info.leakage_current;
}

unsigned long long get_chip_id(void) {
	return (((unsigned long long)system_info.system_serial_low << 32) | system_info.system_serial_high);
}

unsigned long get_resume_linux_addr(void){
	return resume_addr;
}

unsigned int get_chip_revision(void) {
	return system_info.chip_ver;
}

unsigned int get_fephy_bgs(void) {
	return bgs;
}

unsigned int get_soc_tsen_id(void) {
	return soc_tsen_id;
}

void setup_boot_args(unsigned long param1, unsigned long param2, unsigned long param3)
{
	get_chip_type();
	if(param1 == BL_ARG_MAGIC) {
		struct bl_boot_arg * args = (struct bl_boot_arg *)param2;

		INFO("CHIP_ID (byte[0:7]) = %08x%08x\n", args->chip_id[0], args->chip_id[1]);
		INFO("Leakage ID = %d\n", args->leakage);
		INFO("SOCTSEN ID = %d\n", args->soc_tsen_id);
		INFO("CPUTSEN ID = %d\n", args->cpu_tsen_id);
		INFO("FE.BGS     = %d\n", args->param[0]);
		INFO("Chip_ver   = %d\n", args->chip_revision);

		set_flash_parameters(args->flash_param[0], args->flash_param[1], args->flash_param[2]);

		bgs = args->param[0];
		system_info.leakage_current    = args->leakage;
		system_info.system_serial_low  = args->chip_id[0];
		system_info.system_serial_high = args->chip_id[1];
		system_info.chip_ver           = get_chip_ver(args->chip_revision);
		soc_tsen_id = args->soc_tsen_id;

	}

	//Only for Suspend resume state, we will use param3
	resume_addr = param3;
	INFO("resume_addr = 0x%x\n", resume_addr);
}
