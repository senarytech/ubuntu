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
//! \file DRM_Crypto_CSS.h
//! \brief CSS relative code for Crypto library 
//!
//! Purpose:
//!
//!  Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef __drm_crypto_css_h__
#define __drm_crypto_css_h__
#define CSS_BLOCK_SIZE 0x800
#define CSS_KEY_SIZE 5
#define CSS_DISC_KEY_STRUCT_SIZE 2048

typedef struct tagCSSKeyData
{
	UINT8 pbBusKey[CSS_KEY_SIZE];
	UINT8 pbDiscKey[CSS_KEY_SIZE];
	UINT8 pbTitleKey[CSS_KEY_SIZE];
}CSS_KEY_DATA;


////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_CSS_DecryptDiscKey
//!
//! Description: the function will decrypt the disc key of a DVD.
//! Algorithm:	Decryption of the disc key with player keys: try to decrypt the disc key
//!				from every position with every player key.
//!				if the function fails to decrypt a right disc key, the reason could be:
//!					1. the device key used is revoked.
//!					2. the Encrypted disc data is not valid
//!				
//!
//! \param hKey (IN): -- Marvell DRM KEY Handle which is used to identify the cryptography algorithm
//!						specified by the user
//! \param pbEncDiscKey (IN): -- address of the encrypted disc key data buffer
//! \param uKeySize (IN): -- : -- size of the encrypted disc key data, the size should be 2048
//!
//!
//! \return Error code
//! \retval	S_OK				Succeed
//! \retval	E_INVALIDARG 		Input argument is invalid
//! \retval E_FAIL				Failed to decrypt the right disc key
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_CSS_DecryptDiscKey( MV_CRYPTO_KEY_HANDLE hKey, UCHAR* pbEncDiscKey, UINT uKeySize, UCHAR* pbPlayerKey, UINT uPlayerKeySize);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_CSS_DecryptTitleKey
//!
//! Description: the function will decrypt the title key of a DVD.
//!				
//!
//! \param hKey (IN): -- Marvell DRM KEY Handle which is used to identify the cryptography algorithm
//!						specified by the user
//! \param pbEncTitleKey (IN): -- address of the encrypted title key data buffer
//! \param uKeySize (IN): -- : -- size of the encrypted title key data, the size should be 5
//!
//!
//! \return Error code
//! \retval	S_OK				Succeed
//! \retval	E_INVALIDARG 		Input argument is invalid
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_CSS_DecryptTitleKey( MV_CRYPTO_KEY_HANDLE hKey, UCHAR* pbEncTitleKey, UINT uKeySize);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_CSS_DecodeDVD
//!
//! Description: the function will decrypt the title key of a DVD.
//!				
//!
//! \param hKey (IN): -- Marvell DRM KEY Handle which is used to identify the cryptography algorithm
//!						specified by the user
//! \param pbData (IN/OUT): -- address of the encrypted DVD data buffer, the buffer will be
//!								filled with decrypted data after the execution the the function
//! \param uSize (IN): -- : size of the encrypted DVD VOB data in the pbData buffer
//!
//!
//! \return Error code
//! \retval	S_OK				Succeed
//! \retval	E_INVALIDARG 		Input argument is invalid
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_CSS_DecodeDVD(MV_CRYPTO_KEY_HANDLE hKey, UCHAR* pbData, UINT uSize);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_CSS_DecryptAuthKey
//!
//! Description: the function will decrypt the 
//!				
//!
//! \param hKey (IN): -- Marvell DRM KEY Handle which is used to identify the cryptography algorithm
//!						specified by the user
//! \param Auth_Key (IN): -- the Authentication key type to be decrypted.
//!							CRYPTO_CSS_AUTH_KEY1 :the key of host challenge
//!							CRYPTO_CSS_AUTH_KEY2 :the key of drive challenge
//!							CRYPTO_CSS_AUTH_BUSKEY : the Bus key
//! \param uVariant (IN): -- the authentication variant chosen by drive ,between 0 and 31.
//!
//! \param pbChallenge (IN): -- address of the challenge
//! \param pbkey (IN/OUT): -- : -- size of the buffer to carry the decryption result
//!
//! \return Error code
//! \retval	S_OK				Succeed
//! \retval	E_INVALIDARG 		Input argument is invalid
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_CSS_DecryptAuthKey( MV_CRYPTO_KEY_HANDLE hKey, MV_CRYPTO_CSS_AUTH_KEY Auth_Key,
									 UINT uVariant,UCHAR const* pbChallenge,UCHAR *pbkey);
#endif
