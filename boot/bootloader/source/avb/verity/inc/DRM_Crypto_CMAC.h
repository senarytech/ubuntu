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
////////////////////////////////////////////////////////////////////////////////
//! \file DRM_Crypto_CMAC.h
//! \brief some data structures and API of CMAC algorithm
//!
//! Purpose:
//!
//!  Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef __drm_crypto_cmac_h__
#define __drm_crypto_cmac_h__

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_CMAC
//!
//! Description: MV_CRYPTO_CMAC function will calculate the CMAC of a given data buffer.
//!
//! \param hKey (IN): -- Marvell DRM Handle which is used to identify the cryptography algorithm
//!						specified by the user
//! \param pbData (IN): -- address of data that is going to be verified
//! \param uSize (IN): -- size of the data pointed by pbData
//! \param pCMAC (IN/OUT): -- address of the buffer to contain the calculated CMAC.it should be no 
//!                         less than 16bytes(128bit).
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_CMAC(MV_CRYPTO_KEY_HANDLE hKey,
                       const UCHAR *pbData,
                       UINT uSize,
                       UCHAR *pCMAC);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_VerifyCMAC
//!
//! Description: MV_CRYPTO_VerifyCMAC function will verify the CMAC of a given data buffer.
//!
//! \param hKey (IN): -- Marvell DRM Handle which is used to identify the cryptography algorithm
//!						specified by the user
//! \param pbData (IN): -- address of data that is going to be verified
//! \param uSize (IN): -- size of the data pointed by pbData
//! \param pCMAC (IN): -- address of the CMAC data that is going to be verified, 
//!                         it should contain 16bytes(128bit) CMAC value.
//! \param pbResult (OUT): -- result of the verification TRUE if it successes, FALSE if it fails
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_VerifyCMAC(MV_CRYPTO_KEY_HANDLE hKey,
                             const UCHAR *pbData,
                             UINT uSize,
                             UCHAR *pCMAC,
                             BOOL *pbResult);
#endif
