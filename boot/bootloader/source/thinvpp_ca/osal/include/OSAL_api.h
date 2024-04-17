/*******************************************************************************
 *                Copyright 2012, MARVELL SEMICONDUCTOR, LTD.                   *
 * THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL.                      *
 * NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
 * OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
 * DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
 * THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
 * IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
 *                                                                              *
 * MARVELL COMPRISES MARVELL TECHNOLOGY GROUP LTD. (MTGL) AND ITS SUBSIDIARIES, *
 * MARVELL INTERNATIONAL LTD. (MIL), MARVELL TECHNOLOGY, INC. (MTI), MARVELL    *
 * SEMICONDUCTOR, INC. (MSI), MARVELL ASIA PTE LTD. (MAPL), MARVELL JAPAN K.K.  *
 * (MJKK), MARVELL ISRAEL LTD. (MSIL).                                          *
 *******************************************************************************/
#ifndef _OSAL_API_H_
#define _OSAL_API_H_

#include "com_type.h"
#include "string.h"

#define GaloisMemSet            memset
#define GaloisFree(x)            (void)0
#define GaloisMemcpy            memcpy
#define GaloisMemClear(buf, n)	memset((buf), 0, (n))
#define GaloisMemComp
#define MV_ASSERT(n)
#define ASSERT(x)
#define AMP_SHM_STATIC  8
#define devmem_phy_to_virt(a) (a)

typedef UINT32    VPP_MEM_HANDLE;
typedef signed int HRESULT;
typedef void *MV_OSAL_HANDLE;
typedef MV_OSAL_HANDLE  *pMV_OSAL_HANDLE_MUTEX_t;
typedef MV_OSAL_HANDLE  MV_OSAL_HANDLE_MUTEX_t;
typedef UINT32                  AMP_SHM_HANDLE;
//typedef void *  VDEC_VID_BUF_DESC;
typedef char    CHAR;

void GaloisInit(void);
void * GaloisMalloc(unsigned int size);
void * GaloisMalloc_Cacheable(unsigned int size);
UINT  VPP_ALLOC(unsigned int  eMemType, unsigned int  uiSize, unsigned int  uiAlign, VPP_MEM_HANDLE  *phm);
void * VPP_TZ_ALLOC(unsigned int  uiSize);

#define MV_OSAL_Mutex_Create(...)      SUCCESS
#define AMP_SHM_GetVirtualAddress(...) SUCCESS
#define AMP_SHM_Release(...)
#define TEEC_RegisterSharedMemory(...)
#define TEEC_ReleaseSharedMemory(...)

#define TEEC_FinalizeContext(...)

#endif /* _OSAL_API_H_ */
