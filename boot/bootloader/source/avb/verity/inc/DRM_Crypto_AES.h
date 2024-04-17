/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright � 2013-2018 Synaptics Incorporated. All rights reserved.
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
//! \file DRM_Crypto_AES.h
//! \brief some data structures of AES algorithm 
//!
//! Purpose:
//!
//!  Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef __drm_crypto_aes_h__
#define __drm_crypto_aes_h__


typedef struct tagAESKeyData
{
  UCHAR	  pbIV[CRYPTO_AES128_BLK_LEN];
  UINT32  plRKeyEnc[4*(CRYPTO_AES128_RN + 1)];
  UINT32  plRKeyDec[4*(CRYPTO_AES128_RN + 1)];
  UINT32 ulKeyParm;
} AES_KEY_DATA;


typedef struct tagAESKeyCOMData
{
  UCHAR	  pbIV[CRYPTO_AES256_BLK_LEN];
  UINT32   plRKeyEnc[4*(CRYPTO_AES256_RN + 1)];
  UINT32   plRKeyDec[4*(CRYPTO_AES256_RN + 1)];
  UINT32   ulKeyParm;
  UINT32   uRN;
} AES_KEY_COM_DATA;

HRESULT MV_AES128_Decrypt(const UCHAR *pbDataEnc, 
                             UINT32 uEncSize,
                             UCHAR *pbDataDec, 
                             UINT32* puDecSize,
                             PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_AES128_Encrypt(const UCHAR *pbDataDec, 
                          UINT32 uDecSize,
                          UCHAR *pbDataEnc, 
                          UINT32* puEncSize, 
                          PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_AES128_MakeKey( PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_AES128_SetKeyParm( const UCHAR *pbData, 
                             UINT uSize, 
                             MV_CRYPTO_PARM uParm, 
                             PMV_CRYPTO_KEY_CTX pKeyCtx);
HRESULT MV_AES128_GetKeyParm( PMV_CRYPTO_KEY_CTX pKeyCtx,
			      MV_CRYPTO_PARM uParm,
                              UCHAR* pbKeyData,
			      UINT32* uSize);



HRESULT MV_AES_Decrypt(const UCHAR *pbDataEnc, 
                             UINT32 uEncSize,
                             UCHAR *pbDataDec, 
                             UINT32* puDecSize,
                             PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_AES_Encrypt(const UCHAR *pbDataDec, 
                          UINT32 uDecSize,
                          UCHAR *pbDataEnc, 
                          UINT32* puEncSize, 
                          PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_AES_MakeKey( PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_AES_SetKeyParm( const UCHAR *pbData, 
                             UINT uSize, 
                             MV_CRYPTO_PARM uParm, 
                             PMV_CRYPTO_KEY_CTX pKeyCtx);
HRESULT MV_AES_GetKeyParm( PMV_CRYPTO_KEY_CTX pKeyCtx,
			      MV_CRYPTO_PARM uParm,
                              UCHAR* pbKeyData,
			      UINT32* uSize);


#endif
