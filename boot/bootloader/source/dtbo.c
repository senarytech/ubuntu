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
#include "string.h"
#include "flash_adaptor.h"
#include "load_gpt.h"
#include "boot_mode.h"
#include "libfdt_env.h"

#include "ptinfo.h"
#include "dtbo.h"

int load_dtbo(void * buff, unsigned int * size)
{
	int ret = -1;
	int index = 0;
	int read_size = 0;
	struct gpt_ent gpte_dtbo;
	struct dt_table_header * header = NULL;

	index = get_pt_index(PT_DTBO);

	if(index < 0) {
		dbg_printf(PRN_ERR,"didn't find DTBO partition!!!\n");
		ret = -1;
		goto done;
	}
	fetch_partition_info(index, &gpte_dtbo);

	read_size = sizeof(struct dt_table_header);
	read_size = (read_size > get_page_size()) ? read_size : get_page_size();

read:
	ret = read_image(&gpte_dtbo, read_size, buff);
	if(ret != 0) {
		ret = -1;
		goto done;
	}

	header = (struct dt_table_header *)buff;

	if(cpu_to_fdt32(header->total_size) > read_size) {
		read_size = cpu_to_fdt32(header->total_size);
		goto read;
	}

	INFO("there is %d dtbo\n", cpu_to_fdt32(header->dt_entry_count));

	if(cpu_to_fdt32(header->dt_entry_count) < 1) {
		ret = -1;
		goto done;
	}

	*size = cpu_to_fdt32(header->total_size);

	ret = 0;

done:
	return ret;
}
