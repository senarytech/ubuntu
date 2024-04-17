/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2017 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated (“Synaptics”). The holder of this file shall treat all
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
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS,” AND
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
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS’ TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */

////////////////////////////////////////////////////////////////////////////////
//! \file DRM_FIRSTBOOT_CLIENT_API.h
//! \brief Header file declare the FIRSTBOOT CLIENT APIs
//!
//! Purpose:
//!
//!  Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef __DRM_FIRSTBOOT_AGENT_API_H__
#define __DRM_FIRSTBOOT_AGENT_API_H__

#include "OSAL_api.h"
#include "amp_client.h"
#include "amp_client_support.h"
#include "amp_client_rpc.h"
#include "DRM_Memory.h"

typedef struct __Key_Injection_Image{
    unsigned char *pucOemRootRsa;
    unsigned int  uOemRootRsaSize;
    unsigned char *pucSysInit;
    unsigned int  uSysInitSize;
    unsigned char *pucOemCustk;
    unsigned int  uOemCustkSize;
    unsigned char *pucOemExtRsa;
    unsigned int  uOemExtRsaSize;
#ifdef CONFIG_AMP_GENX_ENABLE
    unsigned char *pucGenXBootloader;
    unsigned int  uGenXBootloaderSize;
    unsigned char *pucGenXOemBL;
    unsigned int  uGenXOemBLSize;
    unsigned char *pucGenXKey;
    unsigned int  uGenXKeySize;
    unsigned char *pucGenXFastboot;
    unsigned int  uGenXFastbootSize;
    unsigned char *pucGenXTZK;
    unsigned int  uGenXTZKSize;
    unsigned char *pucGenXSMFW;
    unsigned int  uGenXSMFWSize;
    unsigned char *pucGenXFastlogo;
    unsigned int  uGenXFastlogoSize;
    unsigned char *pucGenXBoot;
    unsigned int  uGenXBootSize;

#endif
} Key_Injection_Image;

//! SHA-1 Releated definition

#define      SHA1_DIGEST_BYTES_LEN     20
#define      SHA1_BLOCK_BYTES_LEN      64

typedef struct {
    UINT32  state[5];
    UINT32  count[2];
    UINT8   buffer[SHA1_BLOCK_BYTES_LEN];
} SHA1CTX;

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

#define blk0(i) \
(block->l[i] = (rol(block->l[i],24)&(UINT32)0xFF00FF00)|(rol(block->l[i],8)&(UINT32)0x00FF00FF))
#define blk(i) \
(block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15]^block->l[(i+2)&15]^block->l[i&15],1))


typedef union {
UINT8   c[SHA1_BLOCK_BYTES_LEN];
UINT32  l[SHA1_DIGEST_BYTES_LEN];
} BYTE64QUAD16;


/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);


int kms_request_kms_status();

int kms_retrieve_initimg(unsigned long long device_sid,
                         unsigned long unique_id,
                         unsigned char * mainboard_id,
                         unsigned int mainboard_id_len,
                         unsigned char **data, unsigned int * datalen);

HRESULT mv_drm_firstboot_delete_old_appkeys();

HRESULT mv_drm_firstboot_get_appkeys_status(UINT8 bAppKey);

HRESULT mv_drm_firstboot_get_serialid(UINT8* pbSerialID, UINT32 uSize);

HRESULT mv_drm_firstboot_check_initimage(UINT8* pbPackage, UINT32 uPackSize);

HRESULT mv_drm_firstboot_check_initimage_hash(UINT8* pbPackage, UINT32 uPackSize,
                                               UINT8* pbHash, UINT32 uHashSize);

HRESULT mv_drm_firstboot_parse_initimage(UINT8* pbPackage, UINT32 uPackSize);

HRESULT mv_drm_firstboot_init_finish();

HRESULT mv_drm_firstboot_get_uniqueid(unsigned long * unique_id);

HRESULT mv_drm_firstboot_prepare_key_inject(void);

HRESULT mv_drm_firstboot_writeback_key_inject(void);

HRESULT mv_drm_firstboot_check_keyinjection(void);

HRESULT mv_drm_firstboot_load_sysinit(void);

HRESULT mv_drm_firstboot_check_appkeys(UINT8* pbPackage, UINT32 uPackSize);

HRESULT mv_drm_load_secure_boot_keys(UINT8* pbPackage, UINT32 uPackSize);

HRESULT mv_drm_load_otp_layout(UINT8* pbPackage, UINT32 uPackSize);

HRESULT mv_drm_firstboot_write_appkeys(UINT8* pbPackage, UINT32 uPackSize);

HRESULT mv_drm_cipher_otp_burner(void);

HRESULT mv_drm_firstboot_update_spi_mp_area(void);

#ifdef CONFIG_AMP_GENX_ENABLE
HRESULT mv_drm_firstboot_load_genx_bootloader(void);
HRESULT mv_drm_firstboot_load_genx_bl(void);
HRESULT mv_drm_firstboot_load_genx_key(void);
HRESULT mv_drm_firstboot_load_genx_fastboot(void);
HRESULT mv_drm_firstboot_load_genx_tzk(void);
HRESULT mv_drm_firstboot_load_genx_sm_fw(void);
HRESULT mv_drm_firstboot_load_genx_fastlogo(void);
HRESULT mv_drm_firstboot_load_genx_boot(void);
#endif


#ifdef CONFIG_AMP_IP_DRM_NAGRA_EFPK
HRESULT mv_drm_firstboot_write_efpk(UINT8 *efpk);
#endif
#endif
