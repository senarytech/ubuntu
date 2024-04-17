/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRM_CRYPTO_OMAC_H__
#define __DRM_CRYPTO_OMAC_H__

#ifdef __cplusplus
extern "C" {
#endif
//typedef UINT32 DRM_UINT;
//typedef void DRM_VOID;
//typedef UINT8 DRM_BYTE;
//typedef UINT32 DRM_DWORD;
//typedef INT32	DRM_INT;
#define CB_NATIVE_BYTE 1
#define SIZEOF(x) (sizeof(x)*CB_NATIVE_BYTE)
#define __CB_DECL(x) (x)
#define WORDSWAP(d) ( ( (d) >> 16 ) + ( (d) << 16 ) )
#define GET_BYTE(pb,ib)             (pb)[(ib)]
#define PUT_BYTE(pb,ib,b)           (pb)[(ib)]=(b)

#define DRMCRT_memcpy( dst, src, dwcount )            memcpy( dst, src, dwcount )
#define DRM_BYT_CopyBytes(to,tooffset,from,fromoffset,count) DRMCRT_memcpy(&((to)[(tooffset)]),&((from)[(fromoffset)]),(count))

#define  MEMCPY memcpy
#define  MEMSET memset


#define MV_AES_BLOCKLEN		16
#define MV_AES_KEYLEN		16
//#ifndef WIN32
#define DRM_API
#define IN
#define OUT
//#endif

typedef struct _OMACPARM 
{
    DRM_BYTE OMAC_LU[16];
    DRM_BYTE OMAC_LU_1[16];
} DRM_OMACInternalParm;


typedef struct __tagOMACState
{
    DRM_BYTE buf[__CB_DECL(16)];
    DRM_OMACInternalParm omacInternalParm;
} DRM_OMACState;

typedef DRM_OMACState OMAC_PLAYREADY_DATA;

HRESULT MV_OMAC_DigestInit(PMV_CRYPTO_HASH_CTX pCtx);

HRESULT MV_OMAC_SetHashParm( const UCHAR *pbData,
							  UINT uSize,
							  MV_CRYPTO_PARM uParm,
							  PMV_CRYPTO_HASH_CTX pCtx);

HRESULT MV_OMAC_DigestUpdate(PMV_CRYPTO_HASH_CTX pCtx,
							   const UCHAR* pbData,
							   UINT uSize );

HRESULT MV_OMAC_DigestFinal(PMV_CRYPTO_HASH_CTX pCtx);


#ifdef __cplusplus
}
#endif

#endif /* __DRM_CRYPTO_CBCKEY_H__ */
