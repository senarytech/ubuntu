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
//! \file DRM_Crypto_RSA_Cipher.h
//! \brief declaration of RSA algorithm relative functions for Crypto library.
//!
//! Purpose:
//!
//!  Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef __DRM_CRYPTO_RSA_CIPHER_H__
#define __DRM_CRYPTO_RSA_CIPHER_H__





/* RSA public and private key.*/
/*#define RSA_MODULUS_BITS 1024*/
//#define RSA_MODULUS_BITS 2048
#define RSA_MODULUS_BITS 1024

#define RSA_MODULUS_LEN ((RSA_MODULUS_BITS + 7) / 8)


/*
#define CRYPTO_RSA_PUBKEY_LEN sizeof(RSA_PUBLIC_KEY)
#define CRYPTO_RSA_PRVKEY_LEN sizeof(RSA_PRIVATE_KEY)



typedef struct tagRSA_PUBLIC_KEY
{
  UINT32 uBits;
  UINT8 bModulus[RSA_MODULUS_LEN];
  UINT8 bExponent[RSA_MODULUS_LEN];
} RSA_PUBLIC_KEY;

typedef struct tagRSA_PRIVATE_KEY
{
  UINT32 uBits;
  UINT8 bModulus[RSA_MODULUS_LEN];
  UINT8 bPublicExponent[RSA_MODULUS_LEN];
  UINT8 bExponent[RSA_MODULUS_LEN];
  UINT8 bPrime[2][RSA_MODULUS_LEN / 2];
  UINT8 bPrimeExponent[2][RSA_MODULUS_LEN / 2];
  UINT8 bCoefficient[RSA_MODULUS_LEN / 2];
} RSA_PRIVATE_KEY;

*/



/***************************************************************
* RSA
*
*      RSA Public Key Type: R_RSA_PUBLIC_KEY
*     RSA Private Key Type: R_RSA_PRIVATE_KEY
***************************************************************/
/* RSA key lengths.*/
/* SHA 1. */
#define SHA1_DIGEST_LEN 20
#define MIN_RSA_MODULUS_BITS 508

/*#define MAX_RSA_MODULUS_BITS 1024*/

#define MAX_RSA_MODULUS_BITS 2048
#define MAX_RSA_MODULUS_LEN ((MAX_RSA_MODULUS_BITS + 7) / 8)
#define MAX_RSA_PRIME_BITS ((MAX_RSA_MODULUS_BITS + 1) / 2)
#define MAX_RSA_PRIME_LEN ((MAX_RSA_PRIME_BITS + 7) / 8)

/* RSA public and private key.*/
typedef struct {
  UINT32 bits;                           /* length in bits of modulus */
  UINT8 modulus[MAX_RSA_MODULUS_LEN];                    /* modulus */
  UINT8 exponent[MAX_RSA_MODULUS_LEN];           /* public exponent */
} R_RSA_PUBLIC_KEY;

typedef struct {
  UINT32 bits;                           /* length in bits of modulus */
  UINT8 modulus[MAX_RSA_MODULUS_LEN];                    /* modulus */
  UINT8 publicExponent[MAX_RSA_MODULUS_LEN];     /* public exponent */
  UINT8 exponent[MAX_RSA_MODULUS_LEN];          /* private exponent */
  UINT8 prime[2][MAX_RSA_PRIME_LEN];               /* prime factors */
  UINT8 primeExponent[2][MAX_RSA_PRIME_LEN];   /* exponents for CRT */
  UINT8 coefficient[MAX_RSA_PRIME_LEN];          /* CRT coefficient */
} R_RSA_PRIVATE_KEY;


typedef struct _DivXRSA {

    BN_CTX *bnContext;
    BIGNUM *n;
    BIGNUM *e;
    BIGNUM *d;

} DivXRSA;


typedef struct tagRSAPKCS1KEY
{
  R_RSA_PUBLIC_KEY pub_key;
  R_RSA_PRIVATE_KEY priv_key;
}RSA_KEY_DATA;


/*
  case CRYPTO_ALG_RSA_PKCS1:
    {
      pKeyCtx->pMV_Decrypt = MV_RSA_PKCS1_Decrypt;
      pKeyCtx->pMV_Encrypt = MV_RSA_PKCS1_Encrypt;
      pKeyCtx->pMV_SetKeyParm = MV_RSA_PKCS1_SetKeyParm;
      pKeyCtx->pMV_MakeKey = NULL;
      pKeyCtx->pMV_Sign = MV_RSA_PKCS1_Sign;
      pKeyCtx->pMV_VerifySig = MV_RSA_PKCS1_Verify;
      pKeyCtx->nKeySize = 0;
          break;
    }

HRESULT MV_AES128_SetKeyParm( const UCHAR *pbData,
                             UINT uSize,
                             MV_CRYPTO_PARM uParm,
                             PMV_CRYPTO_KEY_CTX pKeyCtx);



HRESULT MV_ECDSAGP_SetKeyParm(const UCHAR *pbData,
                               UINT uSize,
                               MV_CRYPTO_PARM uParm,
                               PMV_CRYPTO_KEY_CTX pKeyCtx );


   */


#if SEPERATE_RSA_PADDING
/*RSA Padding Functions*/
UINT32 emsa_pkcs1_cipher_Encode( UINT8 *M,  UINT32 mLen, UINT8 *EM,  UINT32 emLen);


UINT32 emsa_pkcs1_cipher_Verify( UINT8 *EM, UINT32 emLen, UINT8 *M,  UINT32* mLen);


UINT32 emsa_pkcs1_md5sha1_Encode( UINT8 *M,  UINT32 mLen, UINT8 *EM,  UINT32 emLen);


UINT32 emsa_pkcs1_md5sha1_Verify( UINT8 *EM, UINT32 emLen, UINT8 *M,  UINT32 mLen, BOOL *pbResult);


HRESULT emsa_pkcs1_Encode(const UINT8 *M, const INT32 mLen, UINT8 *EM, const INT32 emLen);


UINT32 emsa_oaep_Encode( UINT8 *M,  UINT32 mLen, UINT8 *EM,  UINT32 emLen);

UINT32 emsa_oaep_Verify(UINT8 *EM, UINT32 emLen, UINT8 *M,  UINT32* mLen);

UINT32 emsa_pss_Encode_ex(const UINT8 *M, const UINT32 mLen, UINT8 *EM, const UINT32 emBits);

HRESULT MV_RSAPublicBlock (UINT8 *output, UINT32 *outputLen, const UINT8 *input,
                           const UINT32 inputLen, UINT8*u_e, UINT8* u_n );

HRESULT MV_RSAPrivateBlock (UINT8 *output, UINT32 *outputLen, UINT8 *input,
                            UINT32 inputLen, UINT8*u_d, UINT8* u_n );

#endif

// DivX Related RSA public key functions

HRESULT MV_DivXDRM_RsaCreatePublicKey( rsaPublicKeyHandle * keyHandle,
                                const UINT8 * n,
                                INT32 nLength,
                                const UINT8 * e,
                                INT32 eLength );


HRESULT MV_DivXDRM_RsaDeletePublicKey( rsaPublicKeyHandle keyHandle );


HRESULT MV_DivXDRM_RsaPublicEncrypt( rsaPublicKeyHandle keyHandle,
                              const UINT8* message,
                              INT32 messageLength,
                              UINT8* encryptedData,
                              INT32* encryptedDataLength );


HRESULT MV_DivXDRM_RsaPublicDecrypt( rsaPublicKeyHandle keyHandle,
                              const UINT8 * encryptedData,
                              INT32 encryptedDataLength,
                              UINT8 * message,
                              INT32 * messageLength );



HRESULT MV_RSA_PKCS1_Decrypt(const UCHAR *pbDataEnc,
                             UINT32 uEncSize,
                             UCHAR *pbDataDec,
                             UINT32* puDecSize,
                             PMV_CRYPTO_KEY_CTX pKeyCtx);


HRESULT MV_RSA_PKCS1_Encrypt(const UCHAR *pbDataDec,
                          UINT32 uDecSize,
                          UCHAR *pbDataEnc,
                          UINT32* puEncSize,
                          PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_RSA_PKCS1_Sign( const UCHAR* pbHashData,
                    UINT32 uHashSize,
                    UCHAR* pbSignData,
                    UINT32* puSignSize,
                    const PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_RSA_PKCS1_Verify(const UCHAR* pbHashData,
                          UINT32       uHashSize,
                          const UCHAR* pbSignData,
                          UINT32       uSignSize,
              BOOL *pbResult,
                          const PMV_CRYPTO_KEY_CTX pKeyCtx);




HRESULT MV_RSA_PKCS1_SetKeyParm(const UCHAR *pbData,
                           UINT uSize,
                           MV_CRYPTO_PARM uParm,
                           PMV_CRYPTO_KEY_CTX pKeyCtx );



HRESULT MV_RSA_OAEP_SetKeyParm(const UCHAR *pbData,
                           UINT uSize,
                           MV_CRYPTO_PARM uParm,
                           PMV_CRYPTO_KEY_CTX pKeyCtx );

//for ROM API
  //  DecryptWithRSAPKCS1
  //    Decrypt with RSA-PKCS1.5
  HRESULT MV_DecWithRSAPKCS1(const R_RSA_PRIVATE_KEY *pPrivateKey,
              const UINT8 *pbCiphertext, const UINT32 uCptLen,
              UINT8 *pbPlaintext, UINT32 *puPltLen);

  HRESULT MV_RSA_OAEP_Decrypt(const UCHAR *pbDataEnc,
    UINT32 uEncSize,
    UCHAR *pbDataDec,
    UINT32* puDecSize,
    PMV_CRYPTO_KEY_CTX pKeyCtx);

  HRESULT MV_RSA_OAEP_SHA256_Decrypt(const UCHAR *pbDataEnc,
    UINT32 uEncSize,
    UCHAR *pbDataDec,
    UINT32* puDecSize,
    PMV_CRYPTO_KEY_CTX pKeyCtx);


  HRESULT MV_RSA_OAEP_Encrypt(const UCHAR *pbDataDec,
    UINT32 uDecSize,
    UCHAR *pbDataEnc,
    UINT32* puEncSize,
    PMV_CRYPTO_KEY_CTX pKeyCtx);

  HRESULT MV_RSA_OAEP_Sign( const UCHAR* pbHashData,
    UINT32 uHashSize,
    UCHAR* pbSignData,
    UINT32* puSignSize,
    const PMV_CRYPTO_KEY_CTX pKeyCtx);

  HRESULT MV_RSA_OAEP_Verify(const UCHAR* pbHashData,
    UINT32       uHashSize,
    const UCHAR* pbSignData,
    UINT32       uSignSize,
    BOOL *pbResult,
    const PMV_CRYPTO_KEY_CTX pKeyCtx);


  HRESULT MV_RSA_PSS_Sign( const UCHAR* pbHashData,
             UINT32 uHashSize,
             UCHAR* pbSignData,
             UINT32* puSignSize,
             const PMV_CRYPTO_KEY_CTX pKeyCtx);


  HRESULT MV_RSA_PSS_Verify(const UCHAR* pbHashData,
               UINT32       uHashSize,
               const UCHAR* pbSignData,
               UINT32       uSignSize,
               BOOL *pbResult,
               const PMV_CRYPTO_KEY_CTX pKeyCtx);



  HRESULT MV_RSA_PSS_SetKeyParm(const UCHAR *pbData,
                           UINT uSize,
                           MV_CRYPTO_PARM uParm,
                           PMV_CRYPTO_KEY_CTX pKeyCtx );

#endif
