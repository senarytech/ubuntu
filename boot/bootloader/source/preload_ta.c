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
#include "string.h"
#include "debug.h"
#include "mmgr.h"

#include "image_chunk.h"
#include "load_gpt.h"
#include "load_gih.h"
#include "flash_adaptor.h"
#include "boot_mode.h"
#include "image_info.h"

#include "tz_comm.h"
#include "tee_comm.h"
#include "tee_client_api.h"

#include "bootloader_header.h"
#include "preload_ta.h"

#define MAX_PAGESIZE    8192

static inline unsigned get_aligned(unsigned address, unsigned page_size) {
	return (address + page_size - 1) / page_size * page_size;
}

static unsigned char * buf = NULL;
static struct image_header * ta_header = NULL;
static int load_ta(void)
{
#ifndef CONFIG_AB_BOOTFLOW
	int boot_mode = get_bootmode();
#else
	int boot_mode = get_abmode();
#endif
	int index = 0, to_read = 0;
	struct gpt_ent gpte_bl;
	long long ret = 0;
	struct image_info * img_info;
	unsigned int ta_offset = 0, ta_size = 0;

#ifndef CONFIG_AB_BOOTFLOW
	if(boot_mode == BOOTMODE_NORMAL) {
		index = find_partition(BL_NORMAL_NAME);
	} else {
		index = find_partition(BL_RECOVERY_NAME);
	}
#else
	if(boot_mode == BOOTSEL_B) {
		index = find_partition(BL_B_NAME);
	} else {
		index = find_partition(BL_A_NAME);
	}
#endif


	if(index == PARTITION_NOT_EXIST) {
		dbg_printf(PRN_ERR,"didn't find bootloader partition!!!\n");
		return -1;
	}

	fetch_partition_info(index, &gpte_bl);

	// read prepend image info to find the offset of preload_ta
	to_read = get_page_size();
	to_read = (to_read > MAX_PAGESIZE) ? MAX_PAGESIZE : to_read;
	buf = malloc_ion_cacheable(get_aligned(to_read, 512));
	ret = read_image(&gpte_bl, to_read, buf);
	if(ret < 0){
		dbg_printf(PRN_ERR, "%s, failed to read image header for bl.\n", __func__);
		return -1;
	}

	img_info = (struct image_info *)buf;
	if(img_info->magic != IMAGE_INFO_MAGIC) {
		dbg_printf(PRN_ERR, "ERROR: incorrect magic in image info  0x%08x\n", img_info->magic);
		return -1;
	}

	/* find out the start offset of preload tas */
	ta_offset = img_info->image_offset+ img_info->image_size;
	ta_offset = get_aligned(ta_offset, 512);

	ret = read_image_from_offset(&gpte_bl, ta_offset, to_read, buf);
	if(ret < 0){
		dbg_printf(PRN_ERR, "%s, failed to read image header for preload_ta.\n", __func__);
		return -1;
	}

	ta_header = (struct image_header *)buf;
	ta_size = ta_header->chunk[ta_header->chunk_num - 1].offset + ta_header->chunk[ta_header->chunk_num - 1].size;

	free_ion_cacheable(buf);
	buf = malloc_ion_cacheable(get_aligned(ta_size, 512));
	ret = read_image_from_offset(&gpte_bl, ta_offset, ta_size, buf);
	if(ret < 0){
		dbg_printf(PRN_ERR, "%s, failed to read image for preloadta.\n", __func__);
		return -1;
	}

	return 0;
}

static int reg_ta_count = 0;
static int reg_ta_list[MAX_PRELOAD_TA_COUNT];

int is_ta_registered(unsigned int chunk_id) {
    int i;
    for(i=0; i < reg_ta_count; i++) {
        if(reg_ta_list[i] == chunk_id) {
            return 1;
        }
    }

    return 0;
}

int reg_preload_tas(void)
{
	int i = 0;
	struct chunk_param * chunk;
	TEEC_Result result = TEEC_SUCCESS;

	if(load_ta())
		return -1;

	for(i = 0; i < ta_header->chunk_num; i++) {
		chunk = &(ta_header->chunk[i]);
		dbg_printf(PRN_INFO, "####register ta %c%c%c%c\n", (char)chunk->id, \
		(char)(chunk->id >> 8), (char)(chunk->id >> 16), (char)(chunk->id >> 24));
		result = register_ta((buf + chunk->offset), chunk->size);
		if (result != TEEC_SUCCESS) {
			dbg_printf(PRN_ERR, "register TA failed, ret = 0x%08x\n", result);
			return -2;
		} else if (reg_ta_count >= MAX_PRELOAD_TA_COUNT) {
			dbg_printf(PRN_ERR, "Too many prelaod tas: %d, please enlarge MAX_PRELOAD_TA_COUNT\n",
				 reg_ta_count);
			return -3;
		} else {
			reg_ta_list[reg_ta_count++] = chunk->id;
		}
	}
	free_ion_cacheable(buf);

	return 0;
}
