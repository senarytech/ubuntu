/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2022 Synaptics Incorporated. All rights reserved.
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
#ifdef CONFIG_TRUSTZONE_RPMB_FEATURE

#include "com_type.h"
#include "string.h"
#include "debug.h"

#ifdef CONFIG_DCACHE
#include "cache.h"
#endif
#include "tz_nw_boot.h"
#include "emmc.h"
#include "bootloader_header.h"
#include "rpmb.h"

static uint32_t rpmb_avb_offset = 0;
#define ANDROID_RPMB_AVB_BLKS 8

int handle_rpmb_key(void)
{
	int ret = 0x0;
	int rpmbSector = 0x0;
	int rpmb_fastcall_generic_param_size = sizeof(struct rpmb_fastcall_generic_param);

	struct rpmb_fastcall_generic_param *pGenericCallParam = malloc_ion_cacheable(rpmb_fastcall_generic_param_size);

	memset((void *)pGenericCallParam, 0x00, rpmb_fastcall_generic_param_size);

	dbg_printf(PRN_INFO, "INFO: start to get emmc Capacity !\n");
	rpmbSector = do_emmc_rpmbCapacity();
	if(!rpmbSector)
		return RPMB_CAPACITY_ERROR;
	dbg_printf(PRN_INFO, "INFO: got capacity success, 0x%x !\n", rpmbSector);
	pGenericCallParam->sub_cmd_id = RPMB_FAST_CALL_SUB_ID;
	pGenericCallParam->rpmb_param.rpmb_sub_cmd = RPMB_CONFIG_CMD;
	// rpmbblk num = rpmbSector * 128K / 256 =rpmbSector * 512
#ifdef CONFIG_ANDROID_BOOTFLOW
	pGenericCallParam->rpmb_param.rpmb_size = rpmbSector * 512 - ANDROID_RPMB_AVB_BLKS;
	rpmb_avb_offset = pGenericCallParam->rpmb_param.rpmb_size;
	dbg_printf(PRN_INFO, "android: reserve rpmb block size = %d!\n", ANDROID_RPMB_AVB_BLKS);
#else
	pGenericCallParam->rpmb_param.rpmb_size = rpmbSector * 512;
	dbg_printf(PRN_INFO, "linux: reserve rpmb block size = 0!\n");
#endif
	pGenericCallParam->param_len = sizeof(struct rpmb_fastcall_param);
#ifdef CONFIG_DCACHE
	flush_dcache_range((void*)pGenericCallParam, (void*)pGenericCallParam + rpmb_fastcall_generic_param_size);
#endif
	ret = tz_nw_rpmb_ops((void *)pGenericCallParam, rpmb_fastcall_generic_param_size);
	if(ret)
	{
		dbg_printf(PRN_ERR, "INFO: Fastcall to Config RPMB fail !\n");
		return RPMB_ENABLE_ERROR;
	}

#ifdef CONFIG_DCACHE
	invalidate_dcache_range((void*)pGenericCallParam, (void*)pGenericCallParam + rpmb_fastcall_generic_param_size);
#endif

	dbg_printf(PRN_INFO, "INFO: Fastcall to Enable RPMB !\n");
	pGenericCallParam->rpmb_param.rpmb_sub_cmd = RPMB_ENABLE_CMD;
#ifdef CONFIG_DCACHE
	flush_dcache_range((void*)pGenericCallParam, (void*)pGenericCallParam + rpmb_fastcall_generic_param_size);
#endif
	ret = tz_nw_rpmb_ops((void *)pGenericCallParam, rpmb_fastcall_generic_param_size);
	if(ret)
	{
		dbg_printf(PRN_ERR, "INFO: Fastcall to Enable RPMB fail !\n");
		return RPMB_ENABLE_ERROR;
	}

	dbg_printf(PRN_INFO, "INFO: Fastcall to Enable RPMB success !\n");
	free_ion_cacheable(pGenericCallParam);

	return 0x0;
}

uint32_t get_rpmb_avb_offset(void) {
	return rpmb_avb_offset;
}

#endif
