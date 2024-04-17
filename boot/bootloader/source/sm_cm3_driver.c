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
#include "debug.h"
#include "memmap.h"
#include "system_manager.h"
#include "io.h"
#include "image_info.h"
#include "bootloader_header.h"
#include "load_gpt.h"
#include "bcm_verify.h"
#include "load_gpt.h"
#include "load_gih.h"
#include "image_chunk.h"
#include "boot_mode.h"

#define MAX_PAGESIZE    8192

INT32 MV_SM_StartUp( VOID )
{
	UINT32 reg = 0;

	REG_READ32(SOC_SM_SYS_CTRL_REG_BASE + 0, &reg);
	dbg_printf(PRN_RES, "SM ID: 0x%08x, ctrl reg: 0x%08x+0x%x\n", reg, SOC_SM_SYS_CTRL_REG_BASE, RA_smSysCtl_SM_CPU_CTRL);

	REG_READ32((SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CPU_CTRL), &reg);
	reg |= (1 << LSb32smSysCtl_SM_CPU_CTRL_CPU_RST_GO);
	REG_WRITE32( SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CPU_CTRL, reg );
	return S_OK;
}

INT32 MV_SM_Suspend( VOID )
{
	UINT32 reg = 0;
	REG_READ32(SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CPU_CTRL, &reg);
	reg &= ~(1 << LSb32smSysCtl_SM_CPU_CTRL_CPU_RST_GO);

	REG_WRITE32( SOC_SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_CPU_CTRL, reg );
	return S_OK;
}

#ifndef CONFIG_GENX_ENABLE
INT32 MV_SM_Download_Code( UINT32 base, UCHAR * pucData_Buffer, UINT32 uiData_Size )
{
	UCHAR *pITCM, *pCode;
	UINT32 i;

	pITCM = (UCHAR *)(uintptr_t)SOC_ITCM(base);
	pCode = (UCHAR *)pucData_Buffer;

	for( i = 0 ; i < uiData_Size ; i ++ )
	{
		*pITCM ++ = *pCode ++;
	}

	return S_OK;
}

INT32 MV_SM_Dev_Init(UCHAR * pucData_Buffer,UINT32 uiData_Size)
{
	INT32 res;

	dbg_printf(PRN_RES, "%s: itcmram %d\n", __func__, uiData_Size);

	MV_SM_Suspend();
	MV_SM_Download_Code(SM_ITCM_BASE, pucData_Buffer, uiData_Size );
	MV_SM_StartUp();
	res = S_OK;

	return res;
}
#endif

/*Layout of firmware.subimg in GenX Secure Boot*/
/*
**+-----------------------------------------------------------+
**|<Subimg info header>            |   struct image_info      |
**|--------------------------------+--------------------------|
**
**+-----------------------------------------------------------+
**|<Image header >                 |   struct image_header    |
**+-----------------------------------------------------------+
**
**+-----------------------------------------------------------+
**|<GenX Format TSP image>         |   len(16 byte align)     |
**+-----------------------------------------------------------+
**
**+-----------------------------------------------------------+
**|<GenX Format GPU image>         |   len(16 byte align)     |
**+-----------------------------------------------------------+
**
**+-----------------------------------------------------------+
**|<GenX Format DSP image>         |   len(16 byte align)     |
**+-----------------------------------------------------------+
**
**+-----------------------------------------------------------+
**|<GenX Format SM image>          |   len(16 byte align)     |
**+-----------------------------------------------------------+
*/

#ifdef CONFIG_GENX_ENABLE
int GenX_MV_SM_Load_Code(const void * module_name)
{
	int ret = -1;
	unsigned char * payload_buff = malloc_ion_cacheable(0x100000);
	unsigned char * temp_buff    = malloc_ion_cacheable(MAX_PAGESIZE);
	struct image_header *img_hdr = (struct image_header *)(temp_buff + PREPEND_IMAGE_INFO_SIZE);
	unsigned int img_size = 0;
	unsigned int img_offset = 0;
	unsigned int img_read_size = 0;
	unsigned int img_read_offset = 0;
	unsigned int aligned_offset;

	struct gpt_ent gpte;
	ret = find_partition(module_name);
	if(ret < 0) {
		ERR("%s: fail to get %s gpt entry\n", __FUNCTION__, module_name);
		return ret;
	}
	else
		fetch_partition_info(ret, &gpte);

	/* Read one page data to get firmware image header */
	ret = read_image_from_offset(&gpte, 0x0, MAX_PAGESIZE, temp_buff);
	if(ret < 0){
		ERR("%s: failed to read image header.\n", __FUNCTION__);
		return -1;
	}

	/* Find out sm firmware chunk to get sm firmware image size and offset */
	ret = find_chunk(IMAGE_CHUNK_ID_SM_FW, img_hdr);
	if(ret >= 0) {
		img_offset = img_hdr->chunk[ret].offset;
		img_size = img_hdr->chunk[ret].size;
	} else {
		ERR("%s: doesn't find SM firmware image.\n", __FUNCTION__);
		return -1;
	}

	/* Add prepend subimage header size*/
	img_offset += PREPEND_IMAGE_INFO_SIZE;

	/* Align start offset and size to 512 bytes */
	img_read_offset = (img_offset / SDIO_BLK_SIZE) * SDIO_BLK_SIZE;
	img_read_size = (img_size + 2 * SDIO_BLK_SIZE - 1) / SDIO_BLK_SIZE * SDIO_BLK_SIZE;
	aligned_offset = img_offset - img_read_offset;

	/* Read Image to Non-Secure Buffer from flash*/
	ret = read_image_from_offset(&gpte, img_read_offset, img_read_size, payload_buff);
	if (ret < 0){
		ERR("%s: fail to read SM firmware image ret %d\n", __FUNCTION__, ret);
		return -1;
	}

	/* Verify Image, Skip aligned data */
	ret = VerifyImage((void *)(payload_buff + aligned_offset), img_size, payload_buff, IMAGE_TYPE_SM);

	return ret;
}

INT32 Genx_MV_SM_Dev_Init(void)
{
	INT32 ret;

	dbg_printf(PRN_RES, "Genx_MV_SM_Dev_Init");
#ifdef CONFIG_AB_BOOTFLOW
	const char *FW_NAME = get_abmode() ? FIRMWARE_B_NAME : FIRMWARE_A_NAME;
#else
	const char *FW_NAME = FIRMWARE_NAME;
#endif

	MV_SM_Suspend();
	ret = GenX_MV_SM_Load_Code(FW_NAME);
	if (ret < 0){
		FATAL("ERROR: Failed to load SM firmware image %d\n", ret);
		return -1;
	}
	MV_SM_StartUp();

	return ret;
}

#endif

