/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMHMAC_H__
#define __DRMHMAC_H__


#ifdef __cplusplus
extern "C" {
#endif
#define SHA_DIGEST_LEN 20	/*5 32-bit numbers which is 20 BYTES */
#define SHA_BLOCK_SIZE 64	/*Sha operates on 64 BYTE blocks */
#define MAX_SHA_DIGEST_LEN 32

#define SHA256_DIGEST_LEN 32
#define SHA256_BLOCK_SIZE 64

#define HMAC_KEY_LEN 128
typedef struct __tagHMAC_CONTEXT
{
	MV_CRYPTO_ALG_ID algID;
	UINT8   pbKeyData[HMAC_KEY_LEN];
	UINT32  uKeyLen;
	MV_CRYPTO_HASH_HANDLE hashContext;
	UINT8    shaDigest[__CB_DECL(MAX_SHA_DIGEST_LEN)];
	UINT8    rgbBuffer[__CB_DECL(SHA_BLOCK_SIZE)]; /* This is 64 bytes.  */
} HMAC_CONTEXT;

typedef HMAC_CONTEXT HMAC_WMDRM_DATA;
/*********************************************************************
**
**  Function:  HMACInit
**
**  Synopsis:  HMAC is a keyed-hash message authentication code.  It creates a
**             unique hash of data based on the data and an arbitrary key.
**
**  Arguments:  
**     [pcontextHMAC] -- Context structure to contain a running HMAC calculation.
**     [pbKey] -- Pointer a array of bytes to as the HMAC key.
**     [cbKey] -- Length of pbKey in bytes.
**
**  Notes:  This HMAC code uses SHA-1 as the Secure Hash
**
*********************************************************************/
HRESULT MV_HMAC_Init(
    PMV_CRYPTO_HASH_CTX pHashCtx);

HRESULT MV_HMAC_SHA256_Init(
    PMV_CRYPTO_HASH_CTX pHashCtx);


/*********************************************************************
**
**  Function:  DRM_HMAC_Update
**
**  Synopsis:  
**
**  Arguments:  
**     [pcontextHMAC] -- Valid HMAC_CONTEXT create with DRM_HMAC_Init
**     [pbData] -- Data to add the the secure hash
**     [cbData] -- Length of pbData in bytes
**
*********************************************************************/
HRESULT MV_HMAC_Update(
    PMV_CRYPTO_HASH_CTX pHashCtx,
    const UINT8     *pbData,
    UINT32      cbData);

/*********************************************************************
**
**  Function:  DRM_HMAC_Finalize
**
**  Synopsis:  Closes the MHAC context and returns the computed secure hash.
**
**  Arguments:  
**     [pcontextHMAC] -- Valid HMAC_CONTEXT created with DRM_HMAC_Init
**     [pbKeyedHash] -- Pointer to byte array that will recieve the secure hash
**     [cbKeyedHash] -- Length of pbKeyedHash in bytes
**
**  Notes:  cbKeyedHash must be less than or equal to SHA_DIGEST_LEN and greater than 0.
*********************************************************************/
HRESULT MV_HMAC_Finalize(
    PMV_CRYPTO_HASH_CTX pHashCtx);

HRESULT MV_HMAC_SetHashParm( const UCHAR *pbData,
							 UINT uSize,
							 MV_CRYPTO_PARM uParm,
							 PMV_CRYPTO_HASH_CTX pCtx);


#ifdef __cplusplus
}
#endif

#endif /* __DRMHMAC_H__ */
