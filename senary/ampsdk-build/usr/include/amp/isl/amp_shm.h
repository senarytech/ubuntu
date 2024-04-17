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
//! \file amp_shm.h
//!
//! \brief This file define all common data structure, enum and API for AMP
//!        share memory management.
//!
//! Purpose:
//!
//!
//! Version, Date and Author :
//!   V 1.00,    November 12 2012,    Justin Wu
//!   V 1.01,    December 17 2012,   Weizhao Jiang
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef __AMP_SHM_H__
#define __AMP_SHM_H__

///////////////////////////////////////////////////////////////////////////////
//! [Part1] Header file include
///////////////////////////////////////////////////////////////////////////////
#include "amp_types.h"
#include "shm.h"
///////////////////////////////////////////////////////////////////////////////
//! [Part2] Declaration of external variables or functions
///////////////////////////////////////////////////////////////////////////////
/** AMP share memory classes */
#define AMP_SHM_CLASS_CONTROL       MV_OSAL_SHM_CLASS_CONTROL   /**< For control data structures */
#define AMP_SHM_CLASS_VIDEO         MV_OSAL_SHM_CLASS_VIDEO     /**< For video frame buffers */
#define AMP_SHM_CLASS_GRAPHICS      MV_OSAL_SHM_CLASS_GRAPHICS  /**< For graphics buffers */
#define AMP_SHM_CLASS_OTHERS        MV_OSAL_SHM_CLASS_OTHERS    /**< For other usage */
#define AMP_SHM_CLASS_HW_VIDEO      MV_OSAL_SHM_CLASS_HW_VIDEO  /**< For video hardware frame buffers */

/** AMP share memory attribute flags */
#define AMP_SHM_FLAG_SECURE     MV_OSAL_SHM_FLAG_SECURE         /**< Secure memory or not */
#define AMP_SHM_FLAG_CACHEABLE  MV_OSAL_SHM_FLAG_CACHEABLE      /**< Cacheable memory or not */
#define AMP_SHM_FLAG_DYNAMIC    MV_OSAL_SHM_FLAG_DYNAMIC        /**< Dynamic usage or not */

#define AMP_SHM_TYPE_MAGIC      MV_OSAL_SHM_TYPE_MAGIC          /**< The magic value for SHM type */
#define AMP_SHM_TYPE_INVALIDATE MV_OSAL_SHM_TYPE_INVALIDATE     /**< The invalidate SHM type */


#ifndef MV_UNUSED
#define MV_UNUSED(x)    ((void)(x))
#endif


/** \def AMP_SHM_TYPE_GEN(c, f)
 *
 *  \brief This macro function is used to generate AMP SHM type based on
 *         memory class and memory attribute flag.
 *
 *  \param[in] c The share memory class.
 *  \param[in] f The share memory attribute flag. It can combinate several AMP
 *               share memory flags with "|" operator.
 *
 *  \sa AMP_SHM_CLASS_CONTROL
 *  \sa AMP_SHM_CLASS_VIDEO
 *  \sa AMP_SHM_CLASS_GRAPHICS
 *  \sa AMP_SHM_CLASS_OTHERS
 *  \sa AMP_SHM_FLAG_SECURE
 *  \sa AMP_SHM_FLAG_CACHEABLE
 *  \sa AMP_SHM_FLAG_DYNAMIC
 */
#define AMP_SHM_TYPE_GEN(c, f)  \
    ((AMP_SHM_TYPE)((AMP_SHM_TYPE_MAGIC << 16) | (((c) & 0x7) << 3) | ((f) & 0x7)))

/** \def AMP_SHM_TYPE_GET_INDEX(t)
 *
 *  \brief This macro function is used to get pool index from SHM type.
 *
 *  \param[in] t The share memory type.
 *
 *  \sa AMP_SHM_TYPE_GEN
 */
#define AMP_SHM_TYPE_GET_INDEX(t)   ((t) & 0x3F)

/** \def AMP_SHM_TYPE_GET_MAGIC(t)
 *
 *  \brief This macro function is used to get magic code from SHM type.
 *
 *  \param[in] t The share memory type.
 *
 *  \sa AMP_SHM_TYPE_GEN
 */
#define AMP_SHM_TYPE_GET_MAGIC(t)   ((t) >> 16)

/** \def AMP_SHM_TYPE_IS_VALID(t)
 *
 *  \brief This macro function is used to check the SHM type is valid or not.
 *
 *  \param[in] t The share memory type.
 *
 *  \sa AMP_SHM_TYPE_GEN
 *  \sa AMP_SHM_TYPE_GET_MAGIC
 */
#define AMP_SHM_TYPE_IS_VALID(t)    \
    (AMP_SHM_TYPE_GET_MAGIC(t) == AMP_SHM_TYPE_MAGIC)


///////////////////////////////////////////////////////////////////////////////
//! [Part3] Local macros, type definitions
///////////////////////////////////////////////////////////////////////////////

/**< For buffer descriptor usage. */
#define AMP_SHM_BUFDESC        MV_OSAL_SHM_BUFDESC

/**< For ion system pool usage. */
#define AMP_SHM_SYSTEM         MV_OSAL_SHM_BUFDESC

/**< For uncacheable usage and from MV SHM.
     The space usually will be used by hardware device like TSP, DRMDMX
     Figo. The memory will allocated from hardware init, and be kept till
     deinit. */
#define AMP_SHM_UNCACHEABLE    MV_OSAL_SHM_UNCACHEABLE

/**< Similar with AMP_SHM_UNCACHEABLE but it is cacheable */
#define AMP_SHM_STATIC         MV_OSAL_SHM_STATIC

/**< This memory will be dynamically allocated for component input/out
     buffer, like ES buffer etc. */
#define AMP_SHM_DYNAMIC        MV_OSAL_SHM_DYNAMIC

/**< Similar with AMP_SHM_DYNAMIC but the required size is bigger than
     others. */
#define AMP_SHM_VIDEO_FB       MV_OSAL_SHM_VIDEO_FB

/**< Similar with AMP_SHM_VIDEO_FB but it is hw access only. */
#define AMP_SHM_HW_ONLY        MV_OSAL_SHM_HW_ONLY

/**< Similar with AMP_SHM_VIDEO_FB but it is for graphics. */
#define AMP_SHM_GRAPHICS       MV_OSAL_SHM_GRAPHICS


/**< Similar with AMP_SHM_UNCACHEABLE but it is secure memory space. */
#define AMP_SHM_SECURE_UNCACHEABLE    MV_OSAL_SHM_SECURE_UNCACHEABLE

/**< Similar with AMP_SHM_STATIC but it is secure memory space. */
#define AMP_SHM_SECURE_STATIC         MV_OSAL_SHM_SECURE_STATIC

/**< Similar with AMP_SHM_DYNAMIC but it is secure memory space. */
#define AMP_SHM_SECURE_DYNAMIC        MV_OSAL_SHM_SECURE_DYNAMIC

/**< Similar with AMP_SHM_VIDEO_FB but it is secure memory space. */
#define AMP_SHM_SECURE_VIDEO_FB       MV_OSAL_SHM_SECURE_VIDEO_FB

/**< Similar with AMP_SHM_HW_ONLY but it is secure memory space. */
#define AMP_SHM_SECURE_HW_ONLY        MV_OSAL_SHM_SECURE_HW_ONLY

/**< Similar with AMP_SHM_GRAPHICS but it is secure memory space. */
#define AMP_SHM_SECURE_GRAPHICS       MV_OSAL_SHM_SECURE_GRAPHICS


/**< Invalidate AMP share memory pool. */
#define AMP_SHM_INVALIDATE            MV_OSAL_SHM_INVALIDATE

/** \def AMP_SHM_HANDLE_INVALIDATE
 *
 *  \brief Invalidate AMP share memory handle.
 *
 *  \deprecated AMP_SHM_HANDLE_INVALID indicate the shm handle is invalid.
 *              This version of the library still contains a backward-compatible,
 *              but this definition will be removed by the end of 2018.
 */
#define AMP_SHM_HANDLE_INVALIDATE     MV_OSAL_SHM_HANDLE_INVALIDATE


/**< Invalid AMP share memory handle. */
#define AMP_SHM_HANDLE_INVALID        MV_OSAL_SHM_HANDLE_INVALID

typedef enum __amp_shm_buf_type__ {
    /* generic physical scatter buffer */
    AMP_SHM_SCATTER_GENERIC,
    /* UV entry also page aligned, must set total size and y size together */
    AMP_SHM_SCATTER_UV_ALIGN
}AMP_SHM_BUF_TYPE;


/* extra parameter for allocated buffer */
typedef struct __amp_shm_config__ {
    AMP_SHM_BUF_TYPE    eBufType;
    UINT32              uiSizeY;
} AMP_SHM_CONFIG;



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

#define AMP_SHM_GetStatus               MV_OSAL_SHM_GetStatus
#define AMP_SHM_CleanCache              MV_OSAL_SHM_CleanCache
#define AMP_SHM_InvalidateCache         MV_OSAL_SHM_InvalidateCache
#define AMP_SHM_GetVirtualAddress       MV_OSAL_SHM_GetVirtualAddress
#define AMP_SHM_GetPhysicalAddress      MV_OSAL_SHM_GetPhysicalAddress
#define AMP_SHM_GetPageTableAddress     MV_OSAL_SHM_GetPageTableAddress
#define AMP_SHM_GetMemID                MV_OSAL_SHM_GetMemID
#define AMP_SHM_Share                   MV_OSAL_SHM_Share
#define AMP_SHM_Dump                    MV_OSAL_SHM_Dump
#define AMP_SHM_DbgCtrl                 MV_OSAL_SHM_DbgCtrl
#define AMP_SHM_GetSize                 MV_OSAL_SHM_GetSize
#define AMP_SHM_IsScatter               MV_OSAL_SHM_IsScatter
#define AMP_SHM_IsDynamicSecure         MV_OSAL_SHM_IsDynamicSecure
#define AMP_SHM_Is_IOMMU_Enabled        MV_OSAL_SHM_Is_IOMMU_Enabled


/** \fn HRESULT AMP_SHM_Allocate(
 *       AMP_IN      AMP_SHM_TYPE    eMemType,
 *       AMP_IN      UINT32          uiSize,
 *       AMP_IN      UINT32          uiAlign,
 *       AMP_OUT     AMP_SHM_HANDLE  *phShm);
 *
 *  \brief This function is used to allocate a chunk of shared memory
 *         of a given type supported by AMP.
 *
 *         It always allocates physically contiguous 'shared memory',
 *         which can be used for inter-process communication or by the
 *         AMP drivers (as opposed to 'system memory' which is normal
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
 *         through \c eMemType == \c AMP_SHM_BUFDESC, or when \c
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
 *         AMP_SHM_GetVirtualAddress(). This is the only way for the
 *         caller to read/write the allocated memory.
 *
 *         Note:1. If the memory will later be read by a hardware, any cached
 *                 data must be flushed by calling AMP_SHM_CleanCache() after
 *                 writing to the memory.
 *
 *              2. The physical address of a shared memory handle can be
 *                 retrieved by calling AMP_SHM_GetPhysicalAddress().
 *
 *              3. A handle from AMP_SHM_Allocate() can be freed with
 *                 AMP_SHM_Release().
 *
 *              4. Calling this function will automatically initialize the AMP
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
 *  \sa AMP_SHM_TYPE
 *  \sa AMP_SHM_CleanCache
 *  \sa AMP_SHM_GetVirtualAddress
 *  \sa AMP_SHM_GetPhysicalAddress
 *  \sa AMP_SHM_Release
 */
#define AMP_SHM_Allocate(eMemType, uiSize, uiAlign, phShm)       \
                    AMP_SHM_Allocate_ext(eMemType, uiSize, uiAlign,phShm, __FUNCTION__, __LINE__)
#define AMP_SHM_Ref(phShm, addr)       \
                     MV_OSAL_SHM_Ref(phShm, addr, __FUNCTION__, __LINE__)
#define AMP_SHM_Unref(phShm)       \
                     MV_OSAL_SHM_Unref(phShm, __FUNCTION__, __LINE__)
#define AMP_SHM_Release(phShm)       \
                    AMP_SHM_Release_ext(phShm, __FUNCTION__, __LINE__)
#define AMP_SHM_Discard(phShm)       \
                    MV_OSAL_SHM_Discard(phShm, __FUNCTION__, __LINE__)


HRESULT AMP_SHM_Allocate_ext(
        AMP_IN      AMP_SHM_TYPE    eMemType,
        AMP_IN      UINT32          uiSize,
        AMP_IN      UINT32          uiAlign,
        AMP_OUT     AMP_SHM_HANDLE  *phShm,
        AMP_IN      const char      *sName,
        AMP_IN      UINT32          uiLine);

/** \fn HRESULT AMP_SHM_V1_0_Allocator(
 *            AMP_IN AMP_MEDIA_SESSION_ID uiSessionId,
 *            AMP_IN AMP_CONTENT_TYPE     eContentType,
 *            AMP_IN AMP_SHM_TYPE         eMemType,
 *            AMP_IN AMP_SHM_CLIENTS      stWriters,
 *            AMP_IN AMP_SHM_CLIENTS      stReaders,
 *            AMP_IN AMP_SHM_CONFIG       stShmConfig,
 *            AMP_IN UINT32               uiSize,
 *            AMP_OUT AMP_SHM_HANDLE      *phShm);
 *
 *  \brief This function can be used to allocate a physical contiguous buffer
 *         or scatter buffer of given type.
 *
 *  \param[in]  uiSessionId The pipeline session id.
 *
 *  \param[in]  eContentType The data content type.
 *
 *  \param[in]  eMemType The memory type to be allocated.
 *
 *  \param[in]  uiWriters The clients that write memory.
 *
 *  \param[in]  uiReaders The clients that read memory.
 *
 *  \param[in]  stShmConfig Extra parameter for allocated buffer.
 *                  eType:
 *                      AMP_SHM_SCATTER_GENERIC: to allocate generic scatter buffer;
 *                      AMP_SHM_SCATTER_UV_ALIGN:to allocate a scatter buffer whose uv page start address is 4K aligned.
 *                      uiSizeY: the Y size to be allocated, this value is must if eBufTyep=AMP_SHM_SCATTER_UV_ALIGN.
 *
 *  \param[in]  uiSize The memory size to be allocated
 *
 *  \param[out] phShm Handle to shared memory.
 *
 *  \retval SUCCESS allocation succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM eMemType is not a recognized memory type,
 *          or phShm is a NULL pointer.
 *  \retval MODULE_MEM|ERR_NOSHM    the allocation failed
 *
 *  \sa AMP_SHM_TYPE
 *  \sa AMP_SHM_CleanCache
 *  \sa AMP_SHM_GetVirtualAddress
 *  \sa AMP_SHM_GetPhysicalAddress
 *  \sa AMP_SHM_Release
 */
#define AMP_SHM_V1_0_Allocator(uiSessionId, eContentType, eMemType, uiWriters,\
                               uiReaders, stShmConfig, uiSize, phShm) \
        AMP_SHM_V1_0_Allocator_ext(uiSessionId, eContentType, eMemType, \
                                   uiWriters, uiReaders, stShmConfig, uiSize, \
                                   phShm, __FUNCTION__, __LINE__)

HRESULT AMP_SHM_V1_0_Allocator_ext(
        AMP_IN     AMP_MEDIA_SESSION_ID uiSessionId,
        AMP_IN     AMP_CONTENT_TYPE     eContentType,
        AMP_IN     AMP_SHM_TYPE         eMemType,
        AMP_IN     AMP_SHM_CLIENTS      stWriters,
        AMP_IN     AMP_SHM_CLIENTS      stReaders,
        AMP_IN     AMP_SHM_CONFIG       stShmConfig,
        AMP_IN     UINT32               uiSize,
        AMP_OUT    AMP_SHM_HANDLE       *phShm,
        AMP_IN     const char           *sName,
        AMP_IN     UINT32               uiLine);


/** \fn HRESULT AMP_SHM_Release(
 *       AMP_IN      AMP_SHM_HANDLE  hShm);
 *
 *  \brief This function used to free shm with Shm Handle.
 *
 *         AMP_SHM_Release() is paired with AMP_SHM_Allocate().
 *
 *         The parameter \c hShm will be strictly checked before the share
 *         memory is freed. ERR_NOSHM will be returned to indicate that
 *         the shm doesn't exist any more, it may be freed before.
 *
 *         A ref count is introduced to maintian shm alloc/free. The count will
 *         be decreased if API i called. Only when the count is 0, the shm will
 *         be freed.
 *
 *  \param hShm the share memory handle to be freed.
 *
 *  \retval SUCCESS Release succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hShm is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    the allocation failed
 *
 *  \sa AMP_SHM_TYPE
 *  \sa AMP_SHM_Alloc
 *  \sa AMP_SHM_CleanCache
 *  \sa AMP_SHM_GetVirtualAddress
 *  \sa AMP_SHM_GetPhysicalAddress
 */
HRESULT AMP_SHM_Release_ext(
        AMP_IN      AMP_SHM_HANDLE  hShm,
        AMP_IN   const char      *sName,
        AMP_IN   UINT32          uiLine);




/** \fn HRESULT AMP_SHM_Import(
 *   AMP_IN      int             gid,
 *   AMP_IN      AMP_SHM_TYPE    eMemType,
 *   AMP_IN      UINT32          uiAlign,
 *   AMP_OUT     AMP_SHM_HANDLE  *phShm);
 *
 *  \brief This function is used to import one SHM handle with ion gid.
 *
 *      Some internal structure will be created to maintain this share
 *      memory. The SHM handle will be return for user to call AMP SHM API.
 *
 *      Note: This API only will be called to make compatible for
 *            ion user space API.
 *
 *  \param gid   The ion SHM gid.
 *  \param eMemType The memory type for this memory.
 *  \param uiAlign  The alignment of the shm area.
 *  \param phShm     Returned the Shm Handle.
 *
 *      Some internal structure will be created to maintain this share
 *      memory. The SHM handle will be return for user to call AMP SHM API.
 *
 *      Note: This API only will be called to make compatible for
 *            ion user space API.
 *
 *  \param gid   The ion SHM gid.
 *  \param eMemType The memory type for this memory.
 *  \param uiAlign  The alignment of the shm area.
 *  \param phShm     Returned the Shm Handle.
 *
 *  \retval SUCCESS construct succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM iShareFd is an invalid one, or mmap fail.
 *  \retval MODULE_MEM|ERR_NOSHM     Import failed
 *
 *  \sa AMP_SHM_TYPE
 *  \sa AMP_SHM_Discard
 *  \sa AMP_SHM_CleanCache
 *  \sa AMP_SHM_GetVirtualAddress
 *  \sa AMP_SHM_GetPhysicalAddress
 */
#define AMP_SHM_Import(gid, eMemType, uiAlign, phShm)       \
                        AMP_SHM_Import_ext(gid, eMemType, uiAlign, phShm, __FUNCTION__, __LINE__)

HRESULT AMP_SHM_Import_ext(
        AMP_IN   int             gid,
        AMP_IN   AMP_SHM_TYPE    eMemType,
        AMP_IN   UINT32          uiAlign,
        AMP_OUT  AMP_SHM_HANDLE  *phShm,
        AMP_IN   const char      *sName,
        AMP_IN   UINT32          uiLine);


/** \fn HRESULT AMP_SHM_V1_0_Importer(
 *   AMP_IN AMP_MEDIA_SESSION_ID uiSessionId,
 *   AMP_IN AMP_CONTENT_TYPE     eContentType,
 *   AMP_IN AMP_SHM_CLIENTS      stWriters,
 *   AMP_IN AMP_SHM_CLIENTS      stReaders,
 *   AMP_IN AMP_SHM_CONFIG       stShmConfig,
 *   AMP_IN      int             gid,
 *   AMP_IN      AMP_SHM_TYPE    eMemType,
 *   AMP_OUT     AMP_SHM_HANDLE  *phShm);
 *
 *  \brief This function is used to import one SHM handle with ion gid.
 *
 *      Some internal structure will be created to maintain this share
 *      memory. The SHM handle will be return for user to call AMP SHM API.
 *
 *      Note: This API only will be called to make compatible for
 *            ion user space API.
 *
 *  \param uiSessionId The Pipeline session id.
 *  \param eContentType The data content type.
 *  \param eMemType The memory type to be allocated.
 *  \param uiWriters The clients that write memory.
 *  \param uiReaders The clients that read memory.
 *  \param stShmConfig Extra parameter for allocated buffer.
 *             eType:
 *                 AMP_SHM_SCATTER_GENERIC: to allocate generic scatter buffer;
 *                 AMP_SHM_SCATTER_UV_ALIGN:to allocate a scatter buffer whose uv page start address is 4K aligned.
 *                 uiSizeY: the Y size to be allocated, this value is must if eBufTyep=AMP_SHM_SCATTER_UV_ALIGN.
 *  \param gid The ion SHM gid.
 *  \param eMemType The memory type for this memory.
 *  \param phShm     Returned the Shm Handle.
 *
 *  \retval SUCCESS construct succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM iShareFd is an invalid one, or mmap fail.
 *  \retval MODULE_MEM|ERR_NOSHM     Import failed
 *
 *  \sa AMP_SHM_TYPE
 *  \sa AMP_SHM_Discard
 *  \sa AMP_SHM_CleanCache
 *  \sa AMP_SHM_GetVirtualAddress
 *  \sa AMP_SHM_GetPhysicalAddress
 */
#define AMP_SHM_V1_0_Importer(uiSessionId, eContentType, uiWriters, uiReaders,\
                              stShmConfig, gid, eMemType, phShm) \
        AMP_SHM_V1_0_Importer_ext(uiSessionId, eContentType, uiWriters, \
                                  uiReaders, stShmConfig, gid, eMemType, \
                                  phShm, __FUNCTION__, __LINE__)

HRESULT AMP_SHM_V1_0_Importer_ext(
        AMP_IN   AMP_MEDIA_SESSION_ID uiSessionId,
        AMP_IN   AMP_CONTENT_TYPE     eContentType,
        AMP_IN   AMP_SHM_CLIENTS      stWriters,
        AMP_IN   AMP_SHM_CLIENTS      stReaders,
        AMP_IN   AMP_SHM_CONFIG       stShmConfig,
        AMP_IN   int                  gid,
        AMP_IN   AMP_SHM_TYPE         eMemType,
        AMP_OUT  AMP_SHM_HANDLE       *phShm,
        AMP_IN   const char           *sName,
        AMP_IN   UINT32               uiLine);


#ifdef __cplusplus
}
#endif


#endif  /* __AMP_SHM_H__ */
