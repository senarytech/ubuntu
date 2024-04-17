/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_CRYPTO_CBCMAC_H__
#define __DRM_CRYPTO_CBCMAC_H__

#ifdef __cplusplus
extern "C" {
#endif
typedef UINT32 DRM_UINT;
typedef void DRM_VOID;
typedef UINT8 DRM_BYTE;
typedef UINT32 DRM_DWORD;
typedef INT32	DRM_INT;
#define CB_NATIVE_BYTE 1
#define SIZEOF(x) (sizeof(x)*CB_NATIVE_BYTE)
#define __CB_DECL(x) (x)
#define WORDSWAP(d) ( ( (d) >> 16 ) + ( (d) << 16 ) )
#define GET_BYTE(pb,ib)             (pb)[(ib)]
#define PUT_BYTE(pb,ib,b)           (pb)[(ib)]=(b)
//#ifndef WIN32
#define DRM_API
#define IN
#define OUT
//#endif

typedef struct _CBCKey 
{
    DRM_UINT a1, b1, c1, d1, e1, f1, a2, b2, c2, d2, e2, f2;
} DRM_CBCKey;

/*********************************************************************
**
**  Function:  DRM_CBC_Mac
**
**  Synopsis:  Create a 64-bit MAC
**
**  Arguments:  
**     [pbData] -- Byte pointer to DWORD blocks that are to be MAC'd
**     [cBlocks] -- Length of pbData in DWORD's
**     [rgdwKey2] -- 2 DWORD array to hold the 64-bit result
**     [pCBCkey] -- Key structure filled by the caller.
**
**  Returns:  None
** Notes: dwNumBlocks must be in DWORDS and it should be multiple of 
**        DWORD. Suppose if length is 8 bytes, dwNumBlocks should be 2
*********************************************************************/
DRM_VOID DRM_API DRM_CBC_Mac(
    IN        DRM_BYTE  *pbData,
    IN        DRM_DWORD  cBlocks,
    OUT       DRM_DWORD  rgdwKeys[2],
    IN  const DRM_CBCKey *pCBCkey );

/*********************************************************************
**
**  Function:  DRM_CBC_InverseMac
**
**  Synopsis:  Inverse MAC function.  It decrypts the last two bloacks of pdwData
**              ( replaces 64-bit ciphertext pdwData[dwNumBlocks-1] and pdwData[dwNumBlocks-2]
**              with plaintext ).
**
**  Arguments:  
**     [pbData] -- Byte pointer to DWORD blocks that are to be MAC'd( inverse MAC )
**     [cBlocks] -- Length of pbData in DWORD's
**     [key] -- Key structure filled by caller
**     [ikey] -- Inverse key structure filled by caller.
**
**  Returns:  
**
** Notes: dwNumBlocks must be in DWORDS and it should be multiple of 
**        DWORD. Suppose if length is 8 bytes, dwNumBlocks should be 2
**
*********************************************************************/
DRM_VOID DRM_API DRM_CBC_InverseMac(
    IN OUT   DRM_BYTE  *pbData,
    IN       DRM_DWORD  cBlocks,
    IN const DRM_CBCKey *key,
    IN const DRM_CBCKey *ikey );

/*********************************************************************
**
**  Function:  DRM_MAC_inv32
**
**  Synopsis:  Returns the inverse of n ( inverse in terms of what the CBC Mac inverse code wants ).
**
**  Arguments:  
**     [n] -- Value of n to compute inverse of.
**
**  Returns:  Inverse of n
**
*********************************************************************/
DRM_UINT DRM_API DRM_MAC_inv32( DRM_UINT n );

typedef struct __tagCBCState
{
    DRM_UINT sum,t;
    DRM_BYTE buf[__CB_DECL(8)];
    DRM_DWORD dwBufLen;
	DRM_CBCKey cbcKey;
	DRM_CBCKey invcbcKey;
} DRM_CBCState;

typedef DRM_CBCState CBCMAC_WMDRM_DATA;

DRM_VOID DRM_API DRM_CBC64Init( DRM_CBCKey *cbckey, DRM_CBCState *cbcstate, DRM_BYTE *pKey );

#define DRMV2_MAC_LENGTH 8

DRM_VOID DRM_API DRM_CBC64Update( DRM_CBCKey *key, DRM_CBCState *cbcstate,DRM_DWORD dwLen, DRM_BYTE *pData );
DRM_UINT DRM_API DRM_CBC64Finalize( DRM_CBCKey *key, DRM_CBCState *cbcstate, DRM_UINT *pKey2 );
DRM_UINT DRM_API DRM_CBC64Invert( 
    DRM_CBCKey  *key, 
    DRM_CBCKey  *ikey, 
    DRM_UINT     MacA1, 
    DRM_UINT     MacA2,
    DRM_UINT     MacB1, 
    DRM_UINT     MacB2, 
    DRM_UINT    *pInvKey2 );

DRM_VOID DRM_API DRM_CBC64InitState( DRM_CBCState *cbcstate );

HRESULT MV_CBCMAC_DigestInit(PMV_CRYPTO_HASH_CTX pCtx);

HRESULT MV_CBCMAC_SetHashParm( const UCHAR *pbData,
							  UINT uSize,
							  MV_CRYPTO_PARM uParm,
							  PMV_CRYPTO_HASH_CTX pCtx);

HRESULT MV_CBCMAC_GetHashParm( const UCHAR *pbData,
							  UINT uSize,
							  MV_CRYPTO_PARM uParm,
							  PMV_CRYPTO_HASH_CTX pCtx);
HRESULT MV_CBCMAC_DigestUpdate(PMV_CRYPTO_HASH_CTX pCtx,
							   const UCHAR* pbData,
							   UINT uSize );

HRESULT MV_CBCMAC_DigestFinal(PMV_CRYPTO_HASH_CTX pCtx);
HRESULT MV_CBCMAC_Invert(PMV_CRYPTO_HASH_CTX pCtx, UINT8 * pbOutData, UINT32 * puSize);

#ifdef __cplusplus
}
#endif

#endif /* __DRM_CRYPTO_CBCKEY_H__ */
