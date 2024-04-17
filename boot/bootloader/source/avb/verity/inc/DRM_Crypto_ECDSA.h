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
//! \file DRM_Crypto_ECDSA.h
//! \brief declaration of ECDSA algorithm relative functions for Crypto library. 
//!
//! Purpose:
//!
//!  Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef __DRM_CRYPTO_ECDSAGP_H_
#define __DRM_CRYPTO_ECDSAGP_H_

typedef struct tagEllipticCurvePoint
{
  BIGNUM X;
  BIGNUM Y;
  BIGNUM Z;
  int Z_is_one; /* enable optimized point arithmetics for special case */
}EC_POINT;

typedef struct tagEllipticCurveParmeters
{
  BIGNUM        a, b;
  BIGNUM        field; /* Field specification.
                * For curves over GF(p), this is the modulus;
                * for curves over GF(2^m), this is the 
                * irreducible polynomial defining the field.
                */
  BIGNUM        order, cofactor;
  BN_CTX        bnCtx;
  BN_MONT_CTX   montCtx;
  BIGNUM        one;
  EC_POINT      generator;
  UINT32        a_is_minus3;
}EC_GROUP;

typedef struct tagEllipticCurveDigest
{
  EC_GROUP    group;
  EC_POINT    pub_key;
  BIGNUM	  priv_key;
  UINT32      enc_flag;
}ECDSA_KEY_DATA;

HRESULT EC_BIGNUM_Set( EC_GROUP* pECGroup,
					  BIGNUM* pbnRes,
					  const MV_BN_DATA* pBnData,
					  UINT32 bValidate);
HRESULT EC_POINT_set(EC_GROUP* pECGroup,
					 EC_POINT* pECPoint,
					 const MV_BN_DATA* pBnDataX,
					 const MV_BN_DATA* pBnDataY);
EC_POINT *EC_POINT_new(EC_GROUP *group);
void EC_POINT_free(EC_POINT *point);

int EC_POINT_mul(EC_GROUP *group, EC_POINT *r, const BIGNUM *g_scalar,
				 const EC_POINT *point, const BIGNUM *p_scalar, BN_CTX *ctx);

HRESULT MV_ECDSAGP_SetKeyParm(const UCHAR *pbData, 
                               UINT uSize, 
                               MV_CRYPTO_PARM uParm, 
                               PMV_CRYPTO_KEY_CTX pKeyCtx );

HRESULT MV_ECDSAGP_SetKeyParm_P256(const UCHAR *pbData,
                              UINT uSize,
                              MV_CRYPTO_PARM uParm,
                              PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_ECDSAGP_SetKeyParm_P224(const UCHAR *pbData,
                              UINT uSize,
                              MV_CRYPTO_PARM uParm,
                              PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_ECDSAGP_SetKeyParm_Common(const UCHAR *pbData,
                              UINT uSize,
			      UINT uKeyByteCnt,	
                              MV_CRYPTO_PARM uParm,
                              PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_ECDSAGP_Sign( const UCHAR* pbHashData, 
                    UINT32 uHashSize,
                    UCHAR* pbSignData, 
                    UINT32* puSignSize,
                    const PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_ECDSAGP_Sign_OPENCRYPTO( const UCHAR* pbHashData,
                        UINT32 uHashSize,
                        UCHAR* pbSignData,
                        UINT32* pbSignSize,
                        const PMV_CRYPTO_KEY_CTX pKeyCtx,
			UINT32 uKeyIndex);
                    
HRESULT MV_ECDSAGP_Sign_PLAYREADY_P256( const UCHAR* pbHashData,
                        UINT32 uHashSize,
                        UCHAR* pbSignData,
                        UINT32* pbSignSize,
                        const PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_ECDSAGP_Sign_PLAYREADY_P256_OPENCRYPTO( const UCHAR* pbHashData,
			UINT32 uHashSize,
			UCHAR* pbSignData,
			UINT32* pbSignSize,
			const PMV_CRYPTO_KEY_CTX pKeyCtx,
			UINT32 uKeyTableOffset);

HRESULT MV_ECDSAGP_Sign_P224( const UCHAR* pbHashData,
                        UINT32 uHashSize,
                        UCHAR* pbSignData,
                        UINT32* pbSignSize,
                        const PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_ECDSAGP_Sign_P224_OPENCRYPTO( const UCHAR* pbHashData,
			UINT32 uHashSize,
			UCHAR* pbSignData,
			UINT32* pbSignSize,
			const PMV_CRYPTO_KEY_CTX pKeyCtx,
			UINT32 uKeyTableOffset);

HRESULT MV_ECDSAGP_Sign_Common( const UCHAR* pbHashData, 
                    UINT32 uHashSize,
                    UCHAR* pbSignData, 
                    UINT32* puSignSize,
		    UINT uKeyByteCnt,
		    UINT uRefHashSize,
                    const PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_ECDSAGP_Sign_Common_OPENCRYPTO( const UCHAR* pbHashData,
                        UINT32 uHashSize,
                        UCHAR* pbSignData,
                        UINT32* pbSignSize,
			UINT32 uKeyTop,
			UINT32 uRefHashSize,
                        const PMV_CRYPTO_KEY_CTX pKeyCtx,
			UINT32 uKeyIndex);
 
HRESULT MV_ECDSAGP_Verify(const UCHAR* pbHashData, 
                          UINT32       uHashSize,
                          const UCHAR* pbSignData, 
                          UINT32       uSignSize,
						  BOOL *pbResult,
                          const PMV_CRYPTO_KEY_CTX pKeyCtx);
                          
HRESULT MV_ECDSAGP_Verify_PLAYREADY_P256(const UCHAR* pbHashData,
                          UINT32       uHashSize,
                          const UCHAR* pbSignData,
                          UINT32       uSignSize,
						  BOOL *pbResult,
                          const PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_ECDSAGP_Verify_P224(const UCHAR* pbHashData,
                          UINT32       uHashSize,
                          const UCHAR* pbSignData,
                          UINT32       uSignSize,
						  BOOL *pbResult,
                          const PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_ECDSAGP_Verify_Common(const UCHAR* pbHashData,
                          UINT32       uHashSize,
                          const UCHAR* pbSignData,
                          UINT32       uSignSize,
			  UINT32        uKeyByteCnt,
			  BOOL *pbResult,
                          const PMV_CRYPTO_KEY_CTX pKeyCtx);


HRESULT MV_ECDSAGP_Release(PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_EC_Decrypt_WMDRM(UINT8 * pbDataEnc, UINT8 * pbDataDec, MV_CRYPTO_KEY_HANDLE pKeyCtx);
HRESULT MV_EC_Encrypt_WMDRM(UINT8 * pbDataDec, UINT8 * pbDataEnc, MV_CRYPTO_KEY_HANDLE pKeyCtx);

HRESULT MV_EC_Encrypt_PLAYREADY_P256(UINT8 * pbDataDec, UINT8 * pbDataEnc, const PMV_CRYPTO_KEY_CTX pKeyCtx);
HRESULT MV_EC_Decrypt_PLAYREADY_P256(UINT8 * pbDataEnc, UINT8 * pbDataDec, const PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_EC_Encrypt_P224(UINT8 * pbDataDec, UINT8 * pbDataEnc, const PMV_CRYPTO_KEY_CTX pKeyCtx);
HRESULT MV_EC_Decrypt_P224(UINT8 * pbDataEnc, UINT8 * pbDataDec, const PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_EC_Encrypt_Common(UINT8 * pbDataDec, UINT8 * pbDataEnc, UINT32 uKeyByteCnt, const PMV_CRYPTO_KEY_CTX pKeyCtx);
HRESULT MV_EC_Decrypt_Common(UINT8 * pbDataEnc, UINT8 * pbDataDec, UINT32 uKeyByteCnt, const PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_EC_Decrypt_PLAYREADY_P256_OPENCRYPTO(UINT8 * pbDataEnc, UINT8 * pbDataDec, const PMV_CRYPTO_KEY_CTX pKeyCtx, UINT32 uKeyTableOffset, UINT32 uTargetTableOffset, UINT32 uKeyMode);

HRESULT MV_EC_GenKeyPair_PLAYREADY_P256(UINT8 * pPrivKey, UINT8 * pPubKey, const PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_EC_Decrypt_PLAYREADY(UINT8 * pbDataEnc, UINT8 * pbDataDec, const PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_DTCPIP_EllipticCurve_DH( UINT8* pbInPutValue,UINT32 nInputSize, 
                                                               UINT8* pbRandomData, UINT32 nRandomSize, 
                                                               UINT8* pbOutputValue, UINT32* pOutputSize, 
                                                               UINT8* pbSharedData, UINT32* pSharedSize,   const PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_DTCPIP_EC_Generate_Xv( UINT8* pbXv,UINT32* uXvSize, UINT8* pbXk, UINT32 uXkSize,   const PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_DTCPIP_EC_Generate_AuthKey( UINT8* pbXv,UINT32 uXvSize, UINT8* pbKauth, UINT32* uKauthSize, UINT8* pbXk, UINT32 uXkSize,   const PMV_CRYPTO_KEY_CTX pKeyCtx);
HRESULT MV_EC_Base_Point_Mul(UCHAR* pbOutPointData,
                        UINT32* pbOutSize,
			UCHAR* pbScalar,
			UINT32 uScalarSize,
                        const PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_EC_Base_Point_Mul_OpenCrypto(UCHAR* pbOutPointData,
                        UINT32* pbOutSize,
			UCHAR* pbScalar,
			UINT32 uScalarSize,
                        const PMV_CRYPTO_KEY_CTX pKeyCtx, UINT32 uKeyTableOffset);

HRESULT MV_EC_Point_Mul(UCHAR* pbInPointData,
                        UINT32 uDataSize,
                        UCHAR* pbOutPointData,
                        UINT32* pbOutSize,
			UCHAR* pbScalar,
			UINT32 uScalarSize,
                        const PMV_CRYPTO_KEY_CTX pKeyCtx);
HRESULT MV_EC_Point_Mul_OpenCrypto(UCHAR* pbInPointData,
                        UINT32 uDataSize,
                        UCHAR* pbOutPointData,
                        UINT32* pbOutSize,
			UCHAR* pbScalar,
			UINT32 uScalarSize,
                        const PMV_CRYPTO_KEY_CTX pKeyCtx, UINT32 uKeyTableOffset);
HRESULT MV_CRYPTO_GenHMACKey_P256(const PMV_CRYPTO_KEY_CTX pKeyCtx, UINT8 * pbData);

#endif
