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

#include "OSAL_api.h"

extern void * malloc_ion_noncacheable(int size);
extern void * malloc_ion_cacheable(int size);

void GaloisInit(void)
{
    // do nothing
}

void * GaloisMalloc(unsigned int size)
{
    if (size > 0)
    {
        void *ptr = (void *)((((unsigned long)malloc_ion_noncacheable(size + 32) + 31) >> 5) << 5);
        return(ptr);
    }
    return(0);
}

void * GMalloc(unsigned int size)
{
    if (size > 0)
    {
        void *ptr = (void *)((((unsigned long)malloc_ion_noncacheable(size) + 32) >> 5) << 5);
        return(ptr);
    }
    return(0);
}

void * GaloisMalloc_Cacheable(unsigned int size)
{
    if (size > 0)
    {
        void *ptr = (void *)((((unsigned long)malloc_ion_cacheable(size + 32) + 31) >> 5) << 5);
        return(ptr);
    }
    return(0);
}

UINT  VPP_ALLOC(unsigned int  eMemType, unsigned int  uiSize, unsigned int  uiAlign, VPP_MEM_HANDLE  *phm)
{
#ifdef VPP_ENABLE_SECURE_MEM_ALLOCATION
    *phm = (uintptr_t) get_secure_membuff();
#else
    *phm =(uintptr_t) GMalloc(uiSize);
#endif
    return (0);
}

void * VPP_TZ_ALLOC(unsigned int  uiSize)
{
    void *pPtr;

#ifdef VPP_ENABLE_TZ_MALLOC_MEM_ALLOCATION
    pPtr = malloc_ion_cacheable(uiSize);
#else
    pPtr = GMalloc(uiSize);
#endif
    return pPtr;
}

