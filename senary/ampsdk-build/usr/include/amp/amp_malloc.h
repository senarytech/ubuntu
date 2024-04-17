/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (C) 2013-2017 Synaptics Incorporated. All rights reserved.
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
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS", AND
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

////////////////////////////////////////////////////////////////////////////////
//! \file amp_malloc.h
//!
//! \brief sound service related functions.
//!
//!
//! Purpose:
//!
//!
//! Version     Date            Author
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef __AMP_MALLOC_H__
#define __AMP_MALLOC_H__

#ifdef ANDROID
#include <cutils/memory.h>
#endif

/** \def AmpMalloc
    \brief This macro is used to allocate system memory
    \prarm size memory size to be allocated
 */
#define AmpMalloc       malloc

/** \def AmpCalloc
    \brief This macro is used to allocate a block of system memory with preset
           value 0
    \prarm unsigned n, number of memory block
    \prarm unsigned size, memory size for each block
 */
#define AmpCalloc       calloc

/** \def AmpZalloc
    \brief This macro is used to allocate a block of system memory with preset
           value 0
 */
#define AmpZalloc       zalloc

/** \def AmpFree
    \brief This macro is used to free a block of system memory
    \prarm p, pointer to memory to be freed
 */
#define AmpFree(p)                  \
    do {                            \
        if ((p) != NULL) {          \
            free((void *)(p));      \
            (p) = NULL;             \
        }                           \
    } while (0)

/** \def AmpMemmove
    \brief This macro is used to move a block of system memory
    \prarm void *dest, pointer to memory to move to
    \param const void *src, pointer to memory to move from
    \param size_t n, memory size to move in bytes
 */
#define AmpMemmove      memmove

/** \def AmpMemcpy
    \brief This macro is used to copy a block of system memory
    \prarm void *dest, pointer to memory to copy to
    \param const void *src, pointer to memory to copy from
    \param size_t n, memory size to copy in bytes
 */
#define AmpMemcpy       memcpy


/** \def AmpMemcpyDsb
    \brief This macro is an extension of AmpMemcpy, it gurantees
           the memory copy is done before next instruction
           is executed. The typical usage is to issue DMA from
           non-cacheable DDR memory. It will ensure data is correct
           when DMA starts. This API is not necessary for cacheable
           DDR memory because flush operation will ensure the data
           correctness.
    \prarm void *dest, pointer to memory to copy to
    \param const void *src, pointer to memory to copy from
    \param size_t n, memory size to copy in bytes
 */
#define AmpMemcpyDsb(pDst,pSrc,uSz)                    \
    do {                                               \
            memcpy((void*)(pDst), (void*)(pSrc), uSz); \
            asm("DSB");                                \
    } while (0)


/** \def AmpMemClear
    \brief This macro is used to clear a block of system memory
     \prarm void *buf, pointer to memory to clear
     \param size_t n, memory size to clear in bytes
 */
#define AmpMemClear(buf, n)     memset((buf), 0, (n))

/** \def AmpMemSet
    \brief This macro is used to set a block of system memory with specified
           value (c)
     \prarm void *buf, pointer to memory to set
     \prarm int c, value in char to set memory with
     \param size_t n, memory size to set in bytes
 */
#define AmpMemSet(buf, c, n)    memset((buf), (c), (n))

#ifdef ANDROID
#define AmpMemSet32(buf, c, n)    android_memset32((buf), (c), (n))
#define AmpMemSet16(buf, c, n)    android_memset16((buf), (c), (n))
#else
#define AmpMemSet32(buf, c, n)    memset((buf), (c), (n))
#define AmpMemSet16(buf, c, n)    memset((buf), (c), (n))
#endif

#endif /* __AMP_MALLOC_H__ */
