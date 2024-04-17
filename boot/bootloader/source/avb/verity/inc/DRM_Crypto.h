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
//! \file DRM_Crypto.h
//! \brief Header file declare the crypto global functions
//!
//! Purpose:
//!
//!  Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef __drm_crypto_h__
#define __drm_crypto_h__

#include "DRM_Crypto_API.h"

void MV_CRYPTO_Hex2Bin(const char *pszHex, UCHAR* pbyBin, INT nMaxBinLen);

#define H2KEY(_hKey) ((MV_CRYPTO_KEY_CTX*)(_hKey))
#define KEY2H(_pKeyCtx) ((MV_CRYPTO_KEY_HANDLE)(_pKeyCtx))
#define H2HASH(_hHash) ((MV_CRYPTO_HASH_CTX*)(_hHash))
#define HASH2H(_pHashCtx) ((MV_CRYPTO_HASH_HANDLE)(_pHashCtx))

#define CRYPTO_MAX_KEY_LEN  128
#define	SHA1_BLOCK_BYTES_LEN	64
#define   SHA256_BLOCK_BYTES_LEN 64
#define	SHA1_DIGEST_BYTES_LEN	20
#define   SHA256_DIGEST_BYTES_LEN 32
#define SHA224_DIGEST_BYTES_LEN 28
#define	AESH_BLOCK_BYTES_LEN	16
#define	AESH_DIGEST_BYTES_LEN	16
#define MAX_DIGEST_BYTES      64
#define C2H_DIGEST_BYTES_LEN  8
#define MD5_DIGEST_BYTES_LEN 16

typedef struct tagCryptoKeyContext  MV_CRYPTO_KEY_CTX;
typedef MV_CRYPTO_KEY_CTX*          PMV_CRYPTO_KEY_CTX;

typedef struct tagCryptoHashContext  MV_CRYPTO_HASH_CTX;
typedef MV_CRYPTO_HASH_CTX*          PMV_CRYPTO_HASH_CTX;

#include "DRM_Crypto_BigNum.h"
#include "DRM_Crypto_AES.h"
#include "DRM_Crypto_DES.h"
#include "DRM_Crypto_ECDSA.h"
#include "DRM_Crypto_SHA1.h"
#include "DRM_Crypto_AESH.h"
#include "DRM_Crypto_RSA_Cipher.h"
#include "DRM_Crypto_CSS.h"
#include "DRM_Crypto_RC4.h"
#include "DRM_Crypto_CBCMAC.h"
#include "DRM_Crypto_OMAC.h"
#include "DRM_Crypto_HMAC.h"
#include "DRM_Crypto_SHA256.h"
#include "DRM_Crypto_C2.h"
#include "DRM_Crypto_C2H.h"
#include "DRM_Crypto_MD5.h"

struct tagCryptoKeyContext
{
	UINT32            uMagicID;//magic..
	MV_CRYPTO_ALG_ID   uKeyType;

	UINT32            nKeySize;
	UINT8             pbKeyData[CRYPTO_MAX_KEY_LEN];
	UINT32				uBlockMode;
	union
	{
		AES_KEY_DATA aesKeyData;
		AES_KEY_COM_DATA aesKeyComData;
		DES_KEY_DATA desKeyData;
		ECDSA_KEY_DATA   ecDSA;
		RSA_KEY_DATA rsaKeyData;
		CSS_KEY_DATA cssKeyData;
		RC4_KEY		rc4KeyData;
		C2_KEY_DATA c2KeyData;
	};

	HRESULT (*pMV_Decrypt)(const UCHAR *pbDataEnc,
		UINT32 uEncSize,
		UCHAR *pbDataDec,
		UINT32* puDecSize,
		PMV_CRYPTO_KEY_CTX pKeyCtx );

	HRESULT (*pMV_Encrypt)(const UCHAR *pbDataDec,
		UINT32 uDecSize,
		UCHAR *pbDataEnc,
		UINT32 *puEncSize,
		PMV_CRYPTO_KEY_CTX pKeyCtx);

	HRESULT (*pMV_Sign)( const UCHAR* pbIn,
		UINT32 uInSize,
		UCHAR* pbOut,
		UINT32* puOutSize,
		const PMV_CRYPTO_KEY_CTX pKeyCtx);

	HRESULT (*pMV_VerifySig)(const UCHAR* pbHashData,
		UINT32 uHashSize,
		const UCHAR* pbSignData,
		UINT32       uSize,
		BOOL *pbResult,
		const PMV_CRYPTO_KEY_CTX pKeyCtx);

	HRESULT (*pMV_MakeKey)(PMV_CRYPTO_KEY_CTX pKeyCtx);

	HRESULT (*pMV_SetKeyParm)( const UCHAR *pbData,
		UINT uSize,
		MV_CRYPTO_PARM uParm,
		PMV_CRYPTO_KEY_CTX pKeyCtx );

	HRESULT (*pMV_GetKeyParm)( PMV_CRYPTO_KEY_CTX pKeyCtx,
		MV_CRYPTO_PARM uParm,
		UCHAR *pbKeyData,
		UINT32 *uSize);
	
	HRESULT (*pMV_ReleaseKey)(PMV_CRYPTO_KEY_CTX pKeyCtx);

} ;


typedef struct tagCryptoHashContext
{
	UINT32            uMagicID;//magic..
	MV_CRYPTO_ALG_ID   uAlgID;

	MV_CRYPTO_KEY_HANDLE   hKey;
	UINT32                 uDigestLen;
	UINT8                  pbDigest[MAX_DIGEST_BYTES];
	union
	{
		SHA1_HASH_DATA sha1Data;
		SHA256_HASH_DATA sha256Data;
		MD5_HASH_DATA md5Data;
		AESH_HASH_DATA aeshData;
		CBCMAC_WMDRM_DATA cbcmacData;
		OMAC_PLAYREADY_DATA omacData;
		HMAC_WMDRM_DATA hmacData;
		C2H_HASH_DATA c2hData;
	};

	HRESULT (*pMV_HashInit)(PMV_CRYPTO_HASH_CTX pHashCtx);

	HRESULT (*pMV_HashUpdate)( PMV_CRYPTO_HASH_CTX pHashCtx,
		const UCHAR* pbData,
		UINT uSize);

	HRESULT (*pMV_HashFinal)(PMV_CRYPTO_HASH_CTX pHashCtx);

	HRESULT (*pMV_SetHashParm)( const UCHAR *pbData,
		UINT uSize,
		MV_CRYPTO_PARM uParm,
		PMV_CRYPTO_HASH_CTX pKeyCtx);

	HRESULT (*pMV_GetHashParm)( const UCHAR *pbData,
		UINT uSize,
		MV_CRYPTO_PARM uParm,
		PMV_CRYPTO_HASH_CTX pKeyCtx);
	HRESULT (*pMV_SetHashState)(PMV_CRYPTO_HASH_CTX pHashCtx,
		UINT8 * pbState);
	HRESULT (*pMV_GetHashState)(PMV_CRYPTO_HASH_CTX pHashCtx,
		UINT8 * pbState);
} CryptoHashContext;


//data structure for secure key store
typedef union tagDataOfBlockType
{
	struct tagDOBMsg
	{
		UCHAR  uMsgType; //! == 0x00
		UCHAR  uReserved[3];
	} dobMsgType;

	struct tagDOBCert
	{
		UCHAR  uCertType; //! == 0x01
		UCHAR  uReserved[3];
	} dobCertType;

	struct tagDOBKey
	{
		UCHAR  uKeyType; //! == 0x02
		UCHAR  uReserved[3];
	} dobKeyType;

	UINT32 uValue;
} DOB_TYPE;

typedef struct tagCryptoDataOfBlock
{
	MV_CRYPTO_KEY_ID uProctectKeyID;
	DOB_TYPE  DOBType;
	INT32 hmac;                     //! \todo add keyed-hash support.
	UINT32 uDataSize;                //to store the size of the plaintext.
	INT bPWDProtected;
	UCHAR pbDoBData[1];             //to store the encyrpted data.
}MV_CRYPTO_DOB;


#define IS_VALID_KEY_CTX(_pKeyCtx) \
	(((_pKeyCtx) != NULL) && (((_pKeyCtx)->uMagicID) == CRYPTO_KEY_CTX_MAGIC_ID))

#define IS_VALID_HASH_CTX(_pHashCtx) \
	(((_pHashCtx) != NULL) && (((_pHashCtx)->uMagicID) == CRYPTO_HASH_CTX_MAGIC_ID))



void XOR(UCHAR* pbDst, const UCHAR* pbSrc, UINT32 uSize);
void Hex2Bin(char *pszHex, UCHAR* pbyBin, INT nMaxBinLen) ;
HRESULT MV_CRYPTO_CBCMAC_Invert(MV_CRYPTO_HASH_HANDLE hHash,
								UCHAR *pbData, UINT32* puSize );
#endif





