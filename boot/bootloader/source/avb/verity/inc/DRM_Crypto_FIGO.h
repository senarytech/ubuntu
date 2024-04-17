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
#ifndef __drm_crypto_figo_h__
#define __drm_crypto_figo_h__
#include "DRM_Crypto_API.h"
enum
{
    //! Input FIGO DTCM Block Number
    DRMFIGO_BLKIN_NUM = 6,

    //! Input FIGO DTCM Block size in Bytes
    DRMFIGO_BLKIN_SIZE = 192 * 4,

    //! Output FIGO DTCM Block Number
    DRMFIGO_BLKOUT_NUM = 2,
    
    //! Output FIGO DTCM Block Size
    DRMFIGO_BLKOUT_SIZE = 192 * 4 + 64,

    //! FIGO DRM Filter processing size for each time
    DRMFIGO_PROC_SIZE = 192 * 4,

    //! Input Data Stream FIFO preload size
    DRMFIGO_PRELOAD_SIZE  = 192 * 4 * 2,
    
    //! FIGO DRM Filter initial stream counter
    DRMFIGO_INIT_STRMCNT = 0, 

    //! Input control FIFO path
    DRMFIGO_CTRLFIFO_DEPTH = 8,

    DRMFIGO_KSFIFO_DEPTH = 8,
	DRMFIGO_EVFIFO_SIZE   = (32 * 2 * 4 * 64),
};


enum
{
    //AES 128 
    DRM_CRYPTO_SPHAL_AES128 = 0,

    //AES 192
    DRM_CRYPTO_SPHAL_AES192 = 1,

    //AES 256
    DRM_CRYPTO_SPHAL_AES256 = 2,

    //AES Wrapper
    DRM_CRYPTO_SPHAL_AESWrap = 3,

    //AES Unwrap
    DRM_CRYPTO_SPHAL_AESUnwrap = 4,
};


#endif
