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
//! \file amp_dmx_api.h
//!
//! \brief This file define all common data structure, enum and API for AMP
//!        DMX client.
//!
//!
//!     Version     Date                    Author
//!     V 1.00,     December 17 2013,       Weizhao Jiang
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef __AMP_DMX_API_H__
#define __AMP_DMX_API_H__
///////////////////////////////////////////////////////////////////////////////
//! [Part1] Header file include
///////////////////////////////////////////////////////////////////////////////
#include "amp_types.h"
#include "./isl/amp_shm.h"

///////////////////////////////////////////////////////////////////////////////
//! [Part2] Local macros, type definitions
///////////////////////////////////////////////////////////////////////////////
/** \typedef POST_PROC_FUNC
 *  \brief AMP DMX user callback handle. it will be used to pass callback func
 *         user is trying to do encryption/decryption with asynchronous mode.
 *         The callback is used to notify the operation is done.
 *         The parameter -- pUserdata is passed in ClientEncryption()
 *         ClientDecryption() called by user. It will be passed back to user when
 *         callback is called.
 */
typedef HRESULT (*POST_PROC_FUNC)(VOID *pUserdata);

#ifdef __cplusplus
extern "C"
{
#endif

/** \fn HRESULT AMP_DMX_ClientInit(void );
 *
 *  \brief This function used to init AMP DMX Client.
 *
 *         AMP_DMX_ClientDeinit() is paired with AMP_DMX_ClientInit().
 *
 *         When init dmx client, a dmx client task will be created and run as a
 *         backgroud routine service. It will take charge of amp resource cleanup
 *         and aync timeout handling.
 *
 *         Only one copy of driver structure will be initialized.
 *         One ref_cnt in driver context is introduced to support multiple entry.
 *         The ref_cnt will be ++ if user call this fucntion again.
 *
 *
 *  \param Non
 *
 *  \retval DMX_CLIENT_OK Release succeeded
 *  \retval DMX_CLIENT_NOMEM no heap memory and fail to init.
 *
 *  \sa AMP_DMX_ClientDeinit
 *  \sa AMP_DMX_ClientEncryption
 *  \sa AMP_DMX_ClientEncryption
 */
HRESULT AMP_DMX_ClientInit(void );

/** \fn HRESULT AMP_DMX_ClientDeinit(void );
 *
 *  \brief This function used to deinit AMP DMX Client.
 *
 *         AMP_DMX_ClientInit() is paired with AMP_DMX_ClientDeinit().
 *
 *         When deinit dmx client, the ref_cnt will be --. Only when ref_cnt is
 *         0, release client task/other resource and reset context.
 *
 *  \param Non
 *
 *  \retval DMX_CLIENT_OK Release succeeded
 *  \retval DMX_CLIENT_NOMEM no heap memory and fail to init.
 *  \retval DMX_CLIENT_OSALFAIL fail to allocate OS resource task/mutex.
 *
 *  \sa AMP_DMX_ClientInit
 *  \sa AMP_DMX_ClientEncryption
 *  \sa AMP_DMX_ClientEncryption
 */
HRESULT AMP_DMX_ClientDeinit(void );

/** \fn HRESULT AMP_DMX_ClientEncryption(AMP_SHM_HANDLE hInBuff,
 *                       UINT32 uiInBuffSize,
 *                       AMP_SHM_HANDLE hOutBuff,
 *                       UINT32 uiOutBuffSize,
 *                       AMP_DMX_CRYPTO_SCHEME_TYPE SchemeType,
 *                       AMP_DRM_CRYPTO_ST   *pCryptInfo,
 *                       POST_PROC_FUNC pCallback,
 *                       VOID *pUserdata);
 *
 *  \brief This function used to implement mem2mem encryption function call.
 *
 *         This is async function call, API can return  without waiting. Encryption
 *         will call pCallback() to notify user the Encryption is done.
 *
 *         User need to specify the data source/destination with SHM handle.
 *         SchemeType is used to indicate encryption algorithm, please see
 *         AMP_DMX_CRYPTO_SCHEME_TYPE for detail.
 *         DMX driver will use pCryptInfo to get key context from DRM.
 *
 *
 *  \param hInBuff          input SHM handle
 *  \param uiInBuffSize     size of input SHM
 *  \param hOutBuff         output SHM handle
 *  \param uiOutBuffSize    size of output SHM
 *  \param SchemeType       encryption scheme type
 *  \param pCallback        Callback to notify enc is done
 *  \param pUserdata        parameter of pCallback
 *
 *  \retval DMX_CLIENT_OK Encryption works well.
 *
 *  \sa AMP_DMX_ClientInit
 *  \sa AMP_DMX_ClientDeinit
 *  \sa AMP_DMX_ClientEncryption
 */
HRESULT AMP_DMX_ClientEncryption(AMP_SHM_HANDLE hInBuff,
                        UINT32 uiInBuffSize,
                        AMP_SHM_HANDLE hOutBuff,
                        UINT32 uiOutBuffSize,
                        AMP_DMX_CRYPTO_SCHEME_TYPE SchemeType,
                        AMP_DRM_CRYPTO_ST   *pCryptInfo,
                        POST_PROC_FUNC pCallback,
                        VOID *pUserdata);

/** \fn HRESULT AMP_DMX_ClientDecryption(AMP_SHM_HANDLE hInBuff,
 *                       UINT32 uiInBuffSize,
 *                       AMP_SHM_HANDLE hOutBuff,
 *                       UINT32 uiOutBuffSize,
 *                       AMP_DMX_CRYPTO_SCHEME_TYPE SchemeType,
 *                       AMP_DRM_CRYPTO_ST   *pCryptInfo,
 *                       POST_PROC_FUNC pCallback,
 *                       VOID *pUserdata);
 *
 *  \brief This function used to implement mem2mem decryption function call.
 *
 *         This is async function call, API can return  without waiting. Decryption
 *         will call pCallback() to notify user that the decryption is done.
 *
 *         User need to specify the data source/destination with SHM handle.
 *         SchemeType is used to indicate encryption algorithm, please see
 *         AMP_DMX_CRYPTO_SCHEME_TYPE for detail.
 *         DMX driver will use pCryptInfo to get key context from DRM.
 *
 *
 *  \param hInBuff          input SHM handle
 *  \param uiInBuffSize     size of input SHM
 *  \param hOutBuff         output SHM handle
 *  \param uiOutBuffSize    size of output SHM
 *  \param SchemeType       encryption scheme type
 *  \param pCallback        Callback to notify enc is done
 *  \param pUserdata        parameter of pCallback
 *
 *  \retval DMX_CLIENT_OK Decryption works well.
 *
 *  \sa AMP_DMX_ClientInit
 *  \sa AMP_DMX_ClientDeinit
 *  \sa AMP_DMX_ClientEncryption
 */
HRESULT AMP_DMX_ClientDecryption(AMP_SHM_HANDLE hInBuff,
                        UINT32 uiInBuffSize,
                        AMP_SHM_HANDLE hOutBuff,
                        UINT32 uiOutBuffSize,
                        AMP_DMX_CRYPTO_SCHEME_TYPE SchemeType,
                        AMP_DRM_CRYPTO_ST   *pCryptInfo,
                        POST_PROC_FUNC pCallback,
                        VOID *pUserdata);


#ifdef __cplusplus
}
#endif

#endif  /*__AMP_DMX_API_H__*/

