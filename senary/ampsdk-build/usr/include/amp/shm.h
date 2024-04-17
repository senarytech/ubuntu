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
//! \file shm.h
//!
//! \brief This file define all common data structure, enum and API for MV
//!        share memory management, it is a lower level of amp_shm.
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef _MV_OSAL_SHM_H
#define _MV_OSAL_SHM_H

///////////////////////////////////////////////////////////////////////////////
//! [Part1] Header file include
///////////////////////////////////////////////////////////////////////////////
#include "OSAL_api.h"
#include "shm_type.h"
#include "shm_helper.h"
#include "ion_debug.h"


///////////////////////////////////////////////////////////////////////////////
//! [Part4] Declaration of local/global variables
///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif
///////////////////////////////////////////////////////////////////////////////
//! [Part5] Declarations of local static functions
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//! [Part6] Global functions
///////////////////////////////////////////////////////////////////////////////

/** \fn HRESULT MV_OSAL_SHM_Allocate(
 *       MV_OSAL_IN      MV_OSAL_SHM_TYPE    eMemType,
 *       MV_OSAL_IN      UINT32              uiSize,
 *       MV_OSAL_IN      UINT32              uiAlign,
 *       MV_OSAL_OUT     MV_OSAL_SHM_HANDLE  *phShm);
 *
 *  \brief This function is used to allocate a chunk of shared memory
 *         of a given type.
 *
 *         It always allocates physically contiguous 'shared memory',
 *         which can be used for inter-process communication or by the
 *         MV drivers (as opposed to 'system memory' which is normal
 *         Linux memory). It can allocate cached or uncached memory,
 *         or allocate for a a specific purpose: currently 'buffer
 *         descriptor' and 'video frame' are defined.
 *
 *         The video buffer pool is intended to hold video frames,
 *         which are typically large allocations, and it's separated
 *         in order to reduce memory fragmentation.
 *
 *         In the normal case, a SHM memory block is allocated which
 *         is large enough to hold the requested memory block, any
 *         padding required due to the alignment, and an internal
 *         header structure. In this case \c uiSize specifies the size
 *         of the memory block to be available to the caller.
 *
 *         As a special case, when allocating a buffer descriptor
 *         through \c eMemType == \c MV_OSAL_SHM_BUFDESC, or when \c
 *         uiAlign == 0, the space for the internal header is taken
 *         from the requested memory block size, so in this case the
 *         \c uiSize parameter specifies the total size of the
 *         allocation.
 *
 *         The size of the internal header depends on the internal
 *         implementation and can not be determined by the caller.
 *
 *         The memory type to allocate is controlled by the \c eMemType
 *         parameter.
 *
 *         The output parameter \c phShm is a shared memory handle. In
 *         case this function fails, the output parameter is not
 *         touched. The virtual address of the allocated shared
 *         memory can be retrieved by calling
 *         MV_OSAL_SHM_GetVirtualAddress(). This is the only way for the
 *         caller to read/write the allocated memory.
 *
 *         Cache invalidation has been done after allocation, since we regard hw
 *         will access it
 *
 *         Note:1. If the memory will later be read by a hardware, any cached
 *                 data must be flushed by calling MV_OSAL_SHM_CleanCache() after
 *                 writing to the memory.
 *
 *              2. The physical address of a shared memory handle can be
 *                 retrieved by calling MV_OSAL_SHM_GetPhysicalAddress().
 *
 *              3. A handle from MV_OSAL_SHM_Allocate() can be freed with
 *                 MV_OSAL_SHM_Release().
 *
 *              4. Calling this function will automatically initialize the MV
 *                 SHM subsystem, if it is not already initialized.
*
 *  \param[in]  eMemType The memory type to be allocated.
 *
 *  \param[in]  uiSize The memory size to be allocated
 *
 *  \param[in]  uiAlign The required alignment for the allocated
 *                      memory. 0 means no alignment, and additionally
 *                      uses space from \c uiSize to store the
 *                      internal header. Any integer alignment is
 *                      supported.
 *
 *  \param[out] phShm Handle to shared memory.
 *
 *  \retval SUCCESS allocation succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM eMemType is not a recognized memory type,
 *          or phShm is a NULL pointer.
 *  \retval MODULE_MEM|ERR_NOSHM    the allocation failed
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 *  \sa MV_OSAL_SHM_Release
 */
#if SHM_DEBUG_ENHANCEMENT
HRESULT MV_OSAL_SHM_Allocate(
              MV_OSAL_SHM_TYPE    eMemType,
              UINT32              uiSize,
              UINT32              uiAlign,
              MV_OSAL_SHM_HANDLE  *phShm,
              UINT32              uiPid,
              const char*         sName,
              UINT32              uiLine);
#else
HRESULT MV_OSAL_SHM_Allocate(
              MV_OSAL_SHM_TYPE    eMemType,
              UINT32              uiSize,
              UINT32              uiAlign,
              MV_OSAL_SHM_HANDLE  *phShm);
#endif

#if SHM_DEBUG_ENHANCEMENT
HRESULT MV_OSAL_SHM_V1_0_Allocator(
              MV_OSAL_MEDIA_SESSION_ID uiSessionId,
              MV_OSAL_SHM_CONTENT_TYPE eContentType,
              MV_OSAL_SHM_TYPE         eMemType,
              MV_OSAL_SHM_CLIENTS      stWriters,
              MV_OSAL_SHM_CLIENTS      stReaders,
              MV_OSAL_SHM_CONFIG       stShmConfig,
              UINT32                   uiSize,
              MV_OSAL_SHM_HANDLE       *phShm,
              UINT32                   uiPid,
              const char               *sName,
              UINT32                   uiLine);
#else
HRESULT MV_OSAL_SHM_V1_0_Allocator(
              MV_OSAL_MEDIA_SESSION_ID uiSessionId,
              MV_OSAL_SHM_CONTENT_TYPE eContentType,
              MV_OSAL_SHM_TYPE         eMemType,
              MV_OSAL_SHM_CLIENTS      stWriters,
              MV_OSAL_SHM_CLIENTS      stReaders,
              MV_OSAL_SHM_CONFIG       stShmConfig,
              UINT32                   uiSize,
              MV_OSAL_SHM_HANDLE       *phShm);
#endif

/** \fn HRESULT MV_OSAL_SHM_Release(
 *       MV_OSAL_IN      MV_OSAL_SHM_HANDLE  hShm);
 *
 *  \brief This function used to free shm with Shm Handle.
 *
 *         MV_OSAL_SHM_Release() is paired with MV_OSAL_SHM_Allocate().
 *
 *         The parameter \c hShm will be strictly checked before the share
 *         memory is freed. ERR_NOSHM will be returned to indicate that
 *         the shm doesn't exist any more, it may be freed before.
 *
 *         A ref count is introduced to maintian shm alloc/free. The count will
 *         be decreased if API i called. Only when the count is 0, the shm will
 *         be freed.
 *
 *         Cache invalidation has been done after allocation, since we regard hw
 *         will access it
 *
 *  \param hShm the share memory handle to be freed.
 *
 *  \retval SUCCESS Release succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hShm is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    the allocation failed
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Alloc
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
HRESULT MV_OSAL_SHM_Release(
              MV_OSAL_SHM_HANDLE  hShm,
              const char  *sName,
              UINT32       uiLine);
/** \fn HRESULT MV_OSAL_SHM_Share(
 *      MV_OSAL_IN      MV_OSAL_SHM_HANDLE  hShm,
 *      MV_OSAL_OUT     int                 *fd);
 *
 *  \brief This function is used to share one SHM handle with fd
 *
 *  \param hShm     the share memory handle to be shared
 *  \param fd       File descriptor that can be opened with open/mmap etc
 *                  call close(fd) if fd won't be used anymore
 *
 *  \retval SUCCESS construct succeeded.
 *  \retval MODULE_MEM|ERR_ERRPARAM hShm or fd is an invalid one.
 *          *fd will be -1 if hShm is not valid.
 */
HRESULT MV_OSAL_SHM_Share(
              MV_OSAL_SHM_HANDLE  hShm,
              int                 *fd);

/** \fn HRESULT MV_OSAL_SHM_Import(
 *      MV_OSAL_IN      int                 gid,
 *      MV_OSAL_IN      MV_OSAL_SHM_TYPE    eMemType,
 *      MV_OSAL_IN      UINT32              uiAlign,
 *      MV_OSAL_OUT     MV_OSAL_SHM_HANDLE  *phShm);
 *
 *  \brief This function is used to import one SHM handle with ion gid.
 *
 *         Some internal structure will be created to maintain this share
 *         memory. The SHM handle will be return for user to call MV SHM API.
 *
 *         Note: This API only will be called to make compatible for
 *               ion user space API.
 *               There is no cache operation in this API
 *
 *  \param gid      The ion SHM gid.
 *  \param eMemType The memory type for this memory.
 *  \param uiAlign  The alignment of the shm area.
 *  \param phShm    Returned the Shm Handle.
 *
 *  \retval SUCCESS construct succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM iShareFd is an invalid one, or mmap fail.
 *  \retval MODULE_MEM|ERR_NOSHM    Import failed
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Discard
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
#if SHM_DEBUG_ENHANCEMENT
HRESULT MV_OSAL_SHM_Import(
              int                 gid,
              MV_OSAL_SHM_TYPE    eMemType,
              UINT32              uiAlign,
              MV_OSAL_SHM_HANDLE  *phShm,
              UINT32              uiPid,
              const char*         sName,
              UINT32              uiLine);
#else
HRESULT MV_OSAL_SHM_Import(
              int                 gid,
              MV_OSAL_SHM_TYPE    eMemType,
              UINT32              uiAlign,
              MV_OSAL_SHM_HANDLE  *phShm);
#endif

/** \fn HRESULT MV_OSAL_SHM_V1_0_Importer(
 *      MV_OSAL_IN      MEDIA_SESSION_ID    uiSessionId,
 *      MV_OSAL_IN      CONTENT_TYPE        eContentType,
 *      MV_OSAL_IN      SHM_CLIENTS         stWriters,
 *      MV_OSAL_IN      SHM_CLIENTS         stReaders;
 *      MV_OSAL_IN      MV_OSAL_SHM_CONFIG  stShmConfig,
 *      MV_OSAL_IN      int                 gid,
 *      MV_OSAL_IN      MV_OSAL_SHM_TYPE    eMemType,
 *      MV_OSAL_OUT     MV_OSAL_SHM_HANDLE  *phShm);
 *
 *  \brief This function is used to import one SHM handle with ion gid.
 *
 *         Some internal structure will be created to maintain this share
 *         memory. The SHM handle will be return for user to call MV SHM API.
 *
 *         Note: This API only will be called to make compatible for
 *               ion user space API.
 *               There is no cache operation in this API
 *
 *  \param uiSessionId The Pipeline session id.
 *  \param eContentType The data content type.
 *  \param eMemType The memory type to be allocated.
 *  \param uiWriters The clients that write memory.
 *  \param uiReaders The clients that read memory.
 *  \param stShmConfig Extra parameter for allocated buffer.
 *             eType:
 *                 MV_OSAL_SHM_MMU_SCATTER_GENERIC: to allocate generic scatter buffer;
 *                 MV_OSAL_SHM_MMU_SCATTER_UV_ALIGN:to allocate a scatter buffer whose uv page start address is 4K aligned.
 *                 uiSizeY: the Y size to be allocated, this value is must if eBufTyep=AMP_SHM_SCATTER_UV_ALIGN.
 *  \param gid      The ion SHM gid.
 *  \param eMemType The memory type for this memory.
 *  \param uiAlign  The alignment of the shm area.
 *  \param phShm    Returned the Shm Handle.
 *
 *  \retval SUCCESS construct succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM iShareFd is an invalid one, or mmap fail.
 *  \retval MODULE_MEM|ERR_NOSHM    Import failed
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Discard
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
#if SHM_DEBUG_ENHANCEMENT
HRESULT MV_OSAL_SHM_V1_0_Importer(
              MV_OSAL_MEDIA_SESSION_ID uiSessionId,
              MV_OSAL_SHM_CONTENT_TYPE eContentType,
              MV_OSAL_SHM_CLIENTS      stWriters,
              MV_OSAL_SHM_CLIENTS      stReaders,
              MV_OSAL_SHM_CONFIG       stShmConfig,
              int                      gid,
              MV_OSAL_SHM_TYPE         eMemType,
              MV_OSAL_SHM_HANDLE       *phShm,
              UINT32                   uiPid,
              const char*              sName,
              UINT32                   uiLine);
#else
HRESULT MV_OSAL_SHM_V1_0_Importer(
              MV_OSAL_MEDIA_SESSION_ID uiSessionId,
              MV_OSAL_SHM_CONTENT_TYPE eContentType,
              MV_OSAL_SHM_CLIENTS      stWriters,
              MV_OSAL_SHM_CLIENTS      stReaders,
              MV_OSAL_SHM_CONFIG       stShmConfig,
              int                      gid,
              MV_OSAL_SHM_TYPE         eMemType,
              MV_OSAL_SHM_HANDLE       *phShm);
#endif

/** \fn HRESULT MV_OSAL_SHM_Discard(
 *       MV_OSAL_IN      MV_OSAL_SHM_HANDLE  hShm);
 *
 *  \brief This function used to discard the shm that imported before.
 *
 *         MV_OSAL_SHM_Discard() is paired with MV_OSAL_SHM_Import().
 *
 *         The shm data area won't be touched but free the shm header.
 *
 *         The parameter \c hShm will be strictly checked before the share
 *         memory is freed. ERR_NOSHM will be returned to indicate that
 *         the shm doesn't exist any more, it may be freed before.
 *
 *  \param hShm the memory handle to be discard.
 *
 *  \retval SUCCESS Succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hShm is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    Failed
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Import
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
HRESULT MV_OSAL_SHM_Discard(
               MV_OSAL_SHM_HANDLE  hShm,
               const char  *sName,
               UINT32       uiLine);

/** \fn HRESULT MV_OSAL_SHM_Ref(
 *       MV_OSAL_IN      MV_OSAL_SHM_HANDLE  hShm,
 *       MV_OSAL_OUT     VOID                **ppVirtAddr);
 *
 *  \brief This function used to ref a shm.
 *
 *         The parameter \c hShm will be strictly checked before the share
 *         memory is freed. ERR_NOSHM will be returned to indicate that
 *         the shm doesn't exist any more, it may be frees before.
 *
 *  \param hShm the memory handle to be ref.
 *  \param ppVirtAddr Return the virtual address of this process.
 *
 *  \retval SUCCESS Ref succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hShm is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    Ref failed
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Construct
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
HRESULT MV_OSAL_SHM_Ref(MV_OSAL_SHM_HANDLE hShm,
                        void **pAddress,
                        const char  *sName,
                        UINT32       uiLine);

/** \fn HRESULT MV_OSAL_SHM_Unref(
 *       MV_OSAL_IN      MV_OSAL_SHM_HANDLE  hShm);
 *
 *  \brief This function used to un-refer a shm.
 *
 *         MV_OSAL_SHM_Unref() is paired with MV_OSAL_SHM_Ref().
 *
 *         The parameter \c hShm will be strictly checked before the share
 *         memory is freed. ERR_NOSHM will be returned to indicate that
 *         the shm doesn't exist any more, it may be freed before.
 *
 *  \param hShm the memory handle to be unref.
 *
 *  \retval SUCCESS unref succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hShm is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    unref failed
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Construct
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
HRESULT MV_OSAL_SHM_Unref(MV_OSAL_SHM_HANDLE hShm,
                        const char  *sName,
                        UINT32       uiLine);

/** \fn MV_OSAL_SHM_CleanCache(
 *       MV_OSAL_IN      MV_OSAL_SHM_HANDLE  hShm,
 *       MV_OSAL_IN      UINT32              uiOffset,
 *       MV_OSAL_IN      UINT32              uiSize);
 *
 *  \brief This function used to clean share memory cache.
 *
 *         The parameter \c hShm will be strictly checked before the share
 *         memory is freed. ERR_NOSHM will be returned to indicate that
 *         the shm doesn't exist any more, it may be freed before.
 *
 *         The parameters \c uiSize and \c uiOffset will be checked to avoid
 *         the dangerous case -- out of the shm range.
 *
 *  \param hShm the memory handle.
 *  \param uiOffset The offset of shm start.
 *  \param uiSize The share memory size.
 *
 *  \retval SUCCESS CleanCache succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hShm is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    CleanCache failed
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Construct
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
HRESULT MV_OSAL_SHM_CleanCache(
               MV_OSAL_SHM_HANDLE  hShm,
               UINT32              uiOffset,
               UINT32              uiSize);

/** \fn MV_OSAL_SHM_InvalidateCache(
 *       MV_OSAL_IN      MV_OSAL_SHM_HANDLE  hShm,
 *       MV_OSAL_IN      UINT32              uiOffset,
 *       MV_OSAL_IN      UINT32              uiSize);
 *
 *  \brief This function used to invalidate share memory cache.
 *
 *         The parameter \c hShm will be strictly checked before the share
 *         memory is freed. ERR_NOSHM will be returned to indicate that
 *         the shm doesn't exist any more, it may be freed before.
 *
 *         The parameters \c uiSize and \c uiOffset will be checked to avoid
 *         the dangerous case -- out of the shm range.
 *
 *  \param hShm the memory handle.
 *  \param uiOffset The offset of shm start.
 *  \param uiSize The share memory size.
 *
 *  \retval SUCCESS InvalidateCache succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hShm is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    InvalidateCache failed
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Construct
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
HRESULT MV_OSAL_SHM_InvalidateCache(
               MV_OSAL_SHM_HANDLE  hShm,
               UINT32              uiOffset,
               UINT32              uiSize);


void mv_shm_trace(unsigned int key, SHM_TRACE_ACTION action);

/** \fn MV_OSAL_SHM_GetVirtualAddress(
 *       MV_OSAL_IN      MV_OSAL_SHM_HANDLE  hShm,
 *       MV_OSAL_IN      UINT32              uiOffset,
 *       MV_OSAL_OUT     void                **pVirtAddr);
 *
 *  \brief This function used to get share memory cacheable virtual address.
 *
 *         The parameter \c hShm will be strictly checked before the share
 *         memory is freed. ERR_NOSHM will be returned to indicate that
 *         the shm doesn't exist any more, it may be freed before.
 *
 *         The parameters \c uiSize will be checked to avoid
 *         the dangerous case -- out of the shm range.
 *
 *  \param hShm the memory handle.
 *  \param uiOffset The offset of shm start.
 *  \param pVirtAddr The share memory cacheable virtual address.
 *
 *  \retval SUCCESS GetVirtualAddress succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hShm is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    GetVirtualAddress failed
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Construct
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */

HRESULT MV_OSAL_SHM_GetVirtualAddress(
               MV_OSAL_SHM_HANDLE  hShm,
               UINT32              uiOffset,
              void                **pVirtAddr);

/** \fn MV_OSAL_SHM_GetPhysicalAddress(
 *       MV_OSAL_IN      MV_OSAL_SHM_HANDLE  hShm,
 *       MV_OSAL_IN      UINT32              uiOffset,
 *       MV_OSAL_OUT     void                **pPhyAddr);
 *
 *  \brief This function used to get share memory physical from shm offset.
 *
 *         The parameter \c hShm will be strictly checked before the share
 *         memory is freed. ERR_NOSHM will be returned to indicate that
 *         the shm doesn't exist any more, it may be freed before.
 *
 *         The parameters \c uiOffset will be checked to avoid
 *         the dangerous case -- out of the shm range.
 *
 *  \param hShm the memory handle.
 *  \param uiOffset The offset of shm start.
 *  \param pPhyAddr Return the share memory physical address.
 *
 *  \retval SUCCESS GetPhysicalAddress succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hShm is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    GetPhysicalAddress failed
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Construct
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
HRESULT MV_OSAL_SHM_GetPhysicalAddress(
               MV_OSAL_SHM_HANDLE  hShm,
               UINT32              uiOffset,
              void                **pPhyAddr);


/** \fn MV_OSAL_SHM_GetPageTableAddress(
 *       MV_OSAL_IN      MV_OSAL_SHM_HANDLE  hShm,
 *       MV_OSAL_IN      UINT32              uiOffset,
 *       MV_OSAL_OUT     void                **pPhyAddr);
 *
 *  \brief This function used to get share memory physical address of page table,
 *         used for IOMMU feature of certain synaptics hw IP, such as decoder/display .etc.
 *
 *         The parameter \c hShm will be strictly checked before the share
 *         memory is freed. ERR_NOSHM will be returned to indicate that
 *         the shm doesn't exist any more, it may be freed before.
 *
 *         The parameters \c uiOffset will be checked to avoid
 *         the dangerous case -- out of the shm range.
 *
 *  \param hShm the memory handle.
 *  \param uiOffset The offset of shm start.
 *  \param pPhyAddr Return the share memory physical address of page table.
 *
 *  \retval SUCCESS GetPageTableAddress succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hShm is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    GetPageTableAddress failed
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Construct
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
HRESULT MV_OSAL_SHM_GetPageTableAddress(
               MV_OSAL_SHM_HANDLE  hShm,
               UINT32              uiOffset,
              void                **pPhyAddr);


/** \fn MV_OSAL_SHM_GetMemID(
 *       MV_OSAL_IN      MV_OSAL_SHM_HANDLE  hShm,
 *       MV_OSAL_OUT     UINT32              *mem_id);
 *
 *  \brief This function is used to get mem id of the shm handle.
 *         mem id is the abstraction of buffer registered in TEE.
 *         only iommu enabled can have this mem id right now
 *
 *         The parameter \c hShm will be strictly checked before the share
 *         memory is freed. ERR_NOSHM will be returned to indicate that
 *         the shm doesn't exist any more, it may be freed before.
 *
 *         the dangerous case -- out of the shm range.
 *
 *  \param hShm the memory handle.
 *  \param mem_id Return mem id registed in TEE.
 *
 *  \retval SUCCESS Get Mem ID succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hShm is an invalid one.
 *  \retval MODULE_MEM|ERR_SWSTATEWRONG IOMMU not enabled
 *
 *  \sa MV_OSAL_SHM_GetPageTableAddress
 */
HRESULT MV_OSAL_SHM_GetMemID(
               MV_OSAL_SHM_HANDLE  hShm,
               UINT32              *mem_id);


/** \def MV_OSAL_SHM_Initialize
 *
 *  \brief This function used to initialize SHM module.
 *
 *  \param isServer         is server or not.
 *  \param bEnableDebug     deprecated.
 *
 *  \retval SUCCESS Initialize succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM resource initialize failed.
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Construct
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
HRESULT MV_OSAL_SHM_Initialize(int isServer, int bEnableDebug);


/** \def MV_OSAL_SHM_Deinitialize
 *
 *  \brief This function used to destory SHM module.
 *
 *  \param isServer     is server or not.
 *
 *  \retval SUCCESS Deinitialize  succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM resource destory failed.
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Construct
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
HRESULT MV_OSAL_SHM_Deinitialize(int isServer);


/** \def MV_OSAL_SHM_Initialize_Server
 *
 *  \brief This function used to initialize SHM module of ampservice.
 *
 *  \param pSHM_Drv_Cfg     config file.
 *
 *  \retval SUCCESS Initialize succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM resource initialize failed.
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Construct
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
HRESULT MV_OSAL_SHM_Initialize_Server(MV_OSAL_SHM_DRV_CFG *pSHM_Drv_Cfg);


/** \def MV_OSAL_SHM_Dump
 *
 *  \brief This function used to dump the status of one specified shm handle.
 *
 *  \param hShm     the shm handle for dump.
 *
 *  \retval SUCCESS GetSize succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hShm is an invalid one.
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Construct
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
HRESULT MV_OSAL_SHM_Dump(MV_OSAL_SHM_HANDLE hShm);



/** \def MV_OSAL_SHM_DbgCtrl
 *
 *  \brief This function used to intpu different cmd for debug purpose.
 *
 *  \param SHM_DBG_CTRL_CMD     the control cmd.
 *  \param uiParam1             the first param.
 *  \param uiParam2             the second param.
 *  \param sPath                the path of dumping status.
 *
 *  \retval SUCCESS GetSize succeeded
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Construct
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
HRESULT MV_OSAL_SHM_DbgCtrl(SHM_DBG_CTRL_CMD cmd, void* uiParam1, UINT32 uiParam2, char *sPath);



/** \def MV_OSAL_SHM_GetSize
 *
 *  \brief This function used to get share memory size from shm handle.
 *
 *         The parameter \c hShm will be strictly checked before the share
 *         memory is freed. ERR_NOSHM will be returned to indicate that
 *         the shm doesn't exist any more, it may be freed before.
 *
 *  \param hShm the memory handle.
 *  \param pSize Return the share memory size.
 *
 *  \retval SUCCESS GetSize succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hShm is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    GetSize failed
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Construct
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
HRESULT MV_OSAL_SHM_GetSize(MV_OSAL_SHM_HANDLE          hShm, UINT32 *pSize);

/** \fn HRESULT MV_OSAL_SHM_GetStatus(
 *       MV_OSAL_IN      MV_OSAL_SHM_HANDLE  hShm,
 *       MV_OSAL_OUT     BOOL                *pValid,
 *       MV_OSAL_OUT     MV_OSAL_SHM_TYPE    *pType,
 *       MV_OSAL_OUT     UINT32              *pRefCnt);
 *
 *  \brief This function used to get the status by SHM handle. It also will
 *         debug fucntion for user.
 *
 *         The parameter \c hShm will be strictly checked before the share
 *         memory is freed. ERR_NOSHM will be returned to indicate that
 *         the shm doesn't exist any more, it may be freed before.
 *
 *  \param hShm the memory handle.
 *  \param pValid Return TRUE if SHM handle is valid one, otherwise FALSE.
 *  \param pType Return the SHM type.
 *  \param pRefCnt Return the ref count of SHM.
 *
 *  \retval SUCCESS GetStatus succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hShm is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    GetStatus failed
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Construct
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
HRESULT MV_OSAL_SHM_GetStatus(
               MV_OSAL_SHM_HANDLE  hShm,
               BOOL                *pValid,
               MV_OSAL_SHM_TYPE    *pType,
               UINT32              *pRefCnt);

/** \def MV_OSAL_SHM_IsScatter
 *
 *  \brief This function is used to get scatter attribute from shm handle.
 *
 *         The parameter \c hShm will be strictly checked before the share
 *         memory is freed. ERR_NOSHM will be returned to indicate that
 *         the shm doesn't exist any more, it may be freed before.
 *
 *  \param hShm the memory handle.
 *  \param pIsScatter Return the share memory scatter attribute.
 *
 *  \retval SUCCESS Get Scatter attribute succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hShm is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    Get Scatter attribute failed
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Construct
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
HRESULT MV_OSAL_SHM_IsScatter(MV_OSAL_SHM_HANDLE hShm, BOOL *pIsScatter);

/** \def MV_OSAL_SHM_IsDynamicSecure
 *
 *  \brief This function is used to get dynamic secure attribute from shm
 *         handle.
 *
 *         The parameter \c hShm will be strictly checked before the share
 *         memory is freed. ERR_NOSHM will be returned to indicate that
 *         the shm doesn't exist any more, it may be freed before.
 *
 *  \param hShm the memory handle.
 *  \param pIsDynamicSecure Return the share memory dynamic secure attribute.
 *
 *  \retval SUCCESS Get DynamicSecure attribute succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hShm is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    Get DynamicSecure attribute failed
 *
 *  \sa MV_OSAL_SHM_TYPE
 *  \sa MV_OSAL_SHM_Construct
 *  \sa MV_OSAL_SHM_CleanCache
 *  \sa MV_OSAL_SHM_GetVirtualAddress
 *  \sa MV_OSAL_SHM_GetPhysicalAddress
 */
HRESULT MV_OSAL_SHM_IsDynamicSecure(MV_OSAL_SHM_HANDLE hShm,
                                    BOOL *pIsDynamicSecure);

/** \def MV_OSAL_SHM_Is_IOMMU_Enabled
 *
 *  \brief This function is used to get the IOMMU enabled status
 *
 *  \retval TRUE IOMMU is enabled
 *  \retval FALSE IOMMU is disabled
 *
 */
HRESULT MV_OSAL_SHM_Is_IOMMU_Enabled();

#ifdef __cplusplus
}
#endif


#endif  /* __MV_OSAL_SHM_H__ */
