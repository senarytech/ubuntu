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
//! \file amp_cbuf_helper.h
//!
//! \brief Header file of amp cbuf helper function.
//!
//! Purpose: These APIs will facilitate Users for managing a circle buffer.
//!          Those APIs can take buffers with any length and get buffers in BDs
//!          with user-defined alignment.
//!          User don't have to take care of circle buffer wrap.
//!
//!     Version    Date                     Author
//!     V 1.00,    Apr 9 2013,             Lei Wang
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef __AMP_CBUF_HELPER__
#define __AMP_CBUF_HELPER__

#ifdef __cplusplus
extern "C"
{
#endif

/* \enum
 * \brief return values of AMP CBUF API.
 */
enum {
    AMPCBUF_SUCCESS,
    AMPCBUF_WRONGPARAM,
    AMPCBUF_NOMEM,
    AMPCBUF_ERR,
    AMPCBUF_LACKDATA,
    AMPCBUF_LACKBD,
    AMPCBUF_LACKSPACE,
    AMPCBUF_WRONGPOS,
};

/** \fn UINT32 AMP_CBUFCreate(
 *             UINT32 uPoolSize,
 *             AMP_SHM_HANDLE hShm,
 *             UINT32 uBDNum,
 *             UINT32 uAlign,
 *             BOOL bThreadSafe,
 *             HANDLE *phPool,
 *             BOOL   bNoWrap,
 *             UINT32 uPaddingSize
 *             AMP_SHM_TYPE uType
 *             BOOL bCopy);
 *
 *  \brief Create a shm pool as cbuf.
 *
 *  \param[in] uPoolSize The cbuf size, should be multiple of uAlign.
 *
 *  \param[in] hShm The user-allocated shm handle.
 *                  if NULL, shm will be allocated by this API.
 *
 *  \param[in] uBDNum The number of BDs that user can get.
 *
 *  \param[in] uAlign The alignment that output buffers should meet.
 *
 *  \param[in] bThreadSafe true for multi-threads access.
 *                         false for single-thread access.
 *
 *  \param[out] phPool The pointer to the handle of this pool.
 *
 *  \param[in] bNoWrap  true: no wrap when write one packet reach to the end of buffer
 *                         false: wrap when write one packet reach to the end of buffer
 *
 *  \param[in] uPaddingSize  the padding size for created buffer to support none wrap
 *
 *  \param[in] uType  the share memory type you want to allocate.
 *
 *  \param[in] bCopy  true: copy the data to cbuf share memory in AMP_CBUFWriteData.
 *                         false: do not copy the data in AMP_CBUFWriteData.
 *
 *  \retval AMPCBUF_SUCCESS succeeded.
 *  \retval AMPCBUF_WRONGPARAM wrong parameter.
 *  \retval AMPCBUF_NOMEM run out of linux memory.
 */
UINT32 AMP_CBUFCreate(
       UINT32 uPoolSize,
       AMP_SHM_HANDLE hShm,
       UINT32 uBDNum,
       UINT32 uAlign,
       BOOL bThreadSafe,
       HANDLE *phPool,
       BOOL   bNoWrap,
       UINT32 uPaddingSize,
       AMP_SHM_TYPE uType,
       BOOL bCopy);

/** \fn UINT32 AMP_CBUFGetWrPtr(
 *             HANDLE hPool,
 *             UINT8 **ppAddr,
 *             UINT32 *pOffset,
 *             UINT32 *pFreeSpace,
 *             UINT32 *pDataSize);
 *
 *  \brief Get virtual address of the write pointer.
 *
 *  \param[in] hPool The handle of this pool.
 *
 *  \param[out] ppAddr The pointer to the address pointer.
 *
 *  \param[out] pOffset The offset to the pool.
 *
 *  \param[out] pFreeSpace The pointer to the free space.
 *
 *  \param[out] pDataSize The pointer to the writted data size.
 *
 *  \retval AMPCBUF_SUCCESS succeeded.
 *  \retval AMPCBUF_WRONGPARAM wrong parameter.
 */
UINT32 AMP_CBUFGetWrPtr(
       HANDLE hPool,
       UINT8 **ppAddr,
       UINT32 *pOffset,
       UINT32 *pFreeSpace,
       UINT32 *pDataSize);

/** \fn UINT32 AMP_CBUFGetPoolSize(
 *             HANDLE hPool,
 *             UINT32 *pPoolSize);
 *
 *  \brief Get virtual address of the write pointer.
 *
 *  \param[in] hPool The handle of this pool.
 *
 *  \param[out] pPoolSize The pointer to the pool size.
 *
 *  \retval AMPCBUF_SUCCESS succeeded.
 *  \retval AMPCBUF_WRONGPARAM wrong parameter.
 */
UINT32 AMP_CBUFGetPoolSize(
       HANDLE hPool,
       UINT32 *pPoolSize);

/** \fn UINT32 AMP_CBUFGetShm(
 *             HANDLE hPool,
 *             AMP_SHM_HANDLE *phShm);
 *
 *  \brief Get shm handle.
 *
 *  \param[in] hPool The handle of this pool.
 *
 *  \param[out] phShm The pointer to the shm handle.
 *
 *  \retval AMPCBUF_SUCCESS succeeded.
 *  \retval AMPCBUF_WRONGPARAM wrong parameter.
 */
UINT32 AMP_CBUFGetShm(
       HANDLE hPool,
       AMP_SHM_HANDLE *phShm);

/** \fn UINT32 AMP_CBUFWriteData(
 *             HANDLE hPool,
 *             UINT8 *pAddr,
 *             UINT32 uSize,
 *             UINT32 uFlag,
 *             UINT32 *pOffset);
 *
 *  \brief Write data into cbuf.
 *
 *  \param[in] hPool The handle of this pool.
 *
 *  \param[in] pAddr The pointer to the data buffer.
 *                   If NULL, Only wr ptr will be update based on size.
 *
 *  \param[in] uSize The size of the data buffer.
 *
 *  \param[in] uFlag The flag for event notify.
 *                   For AMP_MEMINFO_FLAG_EOS_MASK,
 *                   all data will be returned by AMP_CBUFRequest
 *                   without considering alignment.
 *
 *  \param[out] pOffset The write offset
 *
 *  \retval AMPCBUF_SUCCESS succeeded.
 *  \retval AMPCBUF_WRONGPARAM wrong parameter.
 *  \retval AMPCBUF_LACKSPACE lack of space.
 *  \retval AMPCBUF_ERR unknown error.
 */
UINT32 AMP_CBUFWriteData(
       HANDLE hPool,
       UINT8 *pAddr,
       UINT32 uSize,
       UINT32 uFlag,
       UINT32 *pOffset);

/** \fn UINT32 AMP_CBUFInsertTag(
 *             HANDLE hPool,
 *             UINT8 *pData,
 *             UINT32 uOffset);
 *
 *  \brief Insert a BD tag into the next BD will be returned by
 *         AMP_CBUFRequest.
 *         User should call after AMP_CBUFWriteData and
 *         uOffset should be covered by pushed data.
 *
 *  \param[in] hPool The handle of this pool.
 *  \param[in] pData The pointer to the BD tag.
 *  \param[in] uOffset The offset to the CBuf.
 *
 *  \retval AMPCBUF_SUCCESS succeeded.
 *  \retval AMPCBUF_WRONGPARAM wrong parameter.
 *  \retval AMPCBUF_LACKBD lack of BD.
 *  \retval AMPCBUF_WRONGPOS wrong tag position.
 */
UINT32 AMP_CBUFInsertTag(
       HANDLE hPool,
       UINT8 *pData,
       UINT32 uOffset);

/** \fn UINT32 AMP_CBUFRequest(
 *             HANDLE hPool,
 *             AMP_BD_HANDLE *phBD,
 *             UINT32 *pOffset,
 *             UINT32 *pSize);
 *
 *  \brief Request BD.
 *         A working BD will be saved for accepting tagging.
 *         So at maximum, uBDNum - 1 BDs will be outputed by this API.
 *
 *  \param[in] hPool The handle of this pool.
 *
 *  \param[in] phBD The pointer to the BD handle.
 *
 *  \param[out] pOffset The offset to the pool.
 *
 *  \param[out] pSize The pointer to the memory size in the BD.
 *
 *  \retval AMPCBUF_SUCCESS succeeded.
 *  \retval AMPCBUF_WRONGPARAM wrong parameter.
 *  \retval AMPCBUF_LACKBD lack of BD.
 *  \retval AMPCBUF_LACKDATA lack of data.
 */
UINT32 AMP_CBUFRequest(
       HANDLE hPool,
       AMP_BD_HANDLE *phBD,
       UINT32 *pOffset,
       UINT32 *pSize);

/** \fn UINT32 AMP_CBUFReset(
 *             HANDLE hPool);
 *
 *  \brief Reset CBuf context.
 *         Note: Still need to AMP_CBUFRelease BD even reset.
 *
 *  \param[in] hPool The handle of this pool.
 *
 *  \retval AMPCBUF_SUCCESS succeeded.
 *  \retval AMPCBUF_WRONGPARAM wrong parameter.
 */
UINT32 AMP_CBUFReset(
       HANDLE hPool);

/** \fn UINT32 AMP_CBUFRelease(
 *             HANDLE hPool,
 *             AMP_BD_HANDLE hBD);
 *
 *  \brief Release BD as well as the memory inside.
 *
 *  \param[in] hPool The handle of this pool.
 *  \param[in] hBD The BD handle.
 *
 *  \retval AMPCBUF_SUCCESS succeeded.
 *  \retval AMPCBUF_WRONGPARAM wrong parameter.
 */
UINT32 AMP_CBUFRelease(
       HANDLE hPool,
       AMP_BD_HANDLE hBD);

/** \fn UINT32 AMP_CBUFDestroy(
 *             HANDLE hPool);
 *
 *  \brief Destroy the shm pool.
 *
 *  \param[in] hPool The handle of this pool.
 *
 *  \retval AMPCBUF_SUCCESS succeeded.
 *  \retval AMPCBUF_WRONGPARAM wrong parameter.
 */
UINT32 AMP_CBUFDestroy(
       HANDLE hPool);

#ifdef __cplusplus
}
#endif

#endif
