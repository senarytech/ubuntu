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
//! \file DRM_Memory.h
//! \brief declaration of debug malloc.
//!
//! Purpose:
//!
//!  Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef __DRM_MEMORY_H__
#define __DRM_MEMORY_H__
#ifdef linux
        #include "stddef.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include "shm_api.h"
#include "isl/amp_shm.h"
void* _DebugMalloc(size_t nSize, const char * lpszFileName, int nLine);
void  _DebugFree(void* pMem);
void  _DebugDumpMemoryLeak();

typedef UINT32  MV_DRM_CC_SHM_Key_t;
typedef UINT32  *pMV_DRM_CC_SHM_Key_t;
typedef UINT32  MV_DRM_CC_SHM_HANDLE_t;
typedef UINT32  *pMV_DRM_CC_SHM_HANDLE_t;
#define MV_DRM_CC_SHM_KEY_DEFAULT               (MV_DRM_CC_SHM_Key_t)(0xFFFFFFFF)
void*  MV_DRM_CC_SHM_Get( unsigned int * pKey );
int  MV_DRM_CC_SHM_Put( int pKey );

void* MV_DRM_CC_SHM_Malloc(MV_DRM_CC_SHM_Key_t* phShm, unsigned int uShmSize );
void* MV_DRM_UC_SHM_Malloc(AMP_SHM_HANDLE* phShm, unsigned int uShmSize);
void* MV_DRM_CC_SHM_Secure_Malloc(AMP_SHM_HANDLE * phShm, unsigned int uShmSize );
HRESULT MV_DRM_CC_SHM_Free(MV_DRM_CC_SHM_Key_t hShm);

void* MV_DRM_CC_SHM_V2P(MV_DRM_CC_SHM_Key_t * phShm, unsigned char* pVirAddr);

#define _DRM_NONCACHE_USE_AMP_DIRECTLY_
unsigned char* MV_DRM_CC_NONCACHE_SHM_Malloc(AMP_SHM_HANDLE * phShm, unsigned int uShmSize);

HRESULT MV_DRM_CC_NONCACHE_SHM_Free(AMP_SHM_HANDLE hShm);

unsigned char* MV_DRM_CC_NONCACHE_SHM_V2P(AMP_SHM_HANDLE * phShm, unsigned char* pVirAddr);

UINT8* MV_DRM_VMemAlloc(UINT32 *hFigoDrv, MV_DRM_CC_SHM_Key_t* uHandle, UINT32 uVSize);

void MV_DRM_VMemFree(UINT32 *hFigoDrv, UINT8* pMem, UINT32 uHandle);

UINT32 MV_DRM_MEM_V2P(UINT32 hFigoDrv, void *vptr);

#ifdef __cplusplus
}  /*extern "C"*/
#endif

#endif
