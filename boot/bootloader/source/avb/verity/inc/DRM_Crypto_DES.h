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
#ifndef __DRM_CYRPTO_DES_H__
#define __DRM_CYRPTO_DES_H__
typedef struct DES_ks
{
  union
  {
    UCHAR cblock[8];
    /* make sure things are correct size on machines with
    * 8 byte longs */
    UINT32 deslong[2];
  } ks[16];
} DES_key_schedule;

typedef DES_key_schedule DES_KEY_DATA;
#define ITERATIONS 16




#ifdef _USE_ASM_
	void DESMakeKey_ASM(const UINT8 *key, DES_key_schedule *schedule);
	void DESEncBlk_ASM(UINT32 *data, DES_key_schedule *ks, INT32 enc);
	void DESMakeKeyOpt_ASM(const UCHAR *key, DES_key_schedule *schedule);
	#define DESEncBlk(data, ks, enc) \
	  DESEncBlk_ASM(data, ks, enc)

	#define DESMakeKey(key, schedule) \
	  DESMakeKeyOpt_ASM(key, schedule)

#else
	void DESMakeKey_Org(const UCHAR *key, DES_key_schedule *schedule);
	void DESEncBlk_Org(UINT32 *data, DES_key_schedule *ks, INT32 enc);

	#define DESEncBlk(data, ks, enc) \
	  DESEncBlk_Org(data, ks, enc)

	#define DESMakeKey(key, schedule) \
	  DESMakeKey_Org(key, schedule)

#endif


#define ROTATE(a,n)    (((a)>>(n))+((a)<<(32-(n))))
#define PERM_OP(a,b,t,n,m) ((t)=((((a)>>(n))^(b))&(m)),\
  (b)^=(t),\
  (a)^=((t)<<(n)))

#define IP(l,r) \
  { \
  UINT32 tt; \
  PERM_OP(r,l,tt, 4,0x0f0f0f0fL); \
  PERM_OP(l,r,tt,16,0x0000ffffL); \
  PERM_OP(r,l,tt, 2,0x33333333L); \
  PERM_OP(l,r,tt, 8,0x00ff00ffL); \
  PERM_OP(r,l,tt, 1,0x55555555L); \
  }

#define FP(l,r) \
  { \
  UINT32 tt; \
  PERM_OP(l,r,tt, 1,0x55555555L); \
  PERM_OP(r,l,tt, 8,0x00ff00ffL); \
  PERM_OP(l,r,tt, 2,0x33333333L); \
  PERM_OP(r,l,tt,16,0x0000ffffL); \
  PERM_OP(l,r,tt, 4,0x0f0f0f0fL); \
  }

#define LOAD_DATA(R,S,u,t) \
  u=R^s[S  ]; \
  t=R^s[S+1]

#define HPERM_OP(a,t,n,m) ((t)=((((a)<<(16-(n)))^(a))&(m)),\
  (a)=(a)^(t)^(t>>(16-(n))))

#define c2l(c,l)	(l =((UINT32)(*((c)++)))    , \
  l|=((UINT32)(*((c)++)))<< 8L, \
  l|=((UINT32)(*((c)++)))<<16L, \
  l|=((UINT32)(*((c)++)))<<24L)

extern const UINT32 des_skb[8][64];

UCHAR *auth_DEShash( UCHAR *dst, const UCHAR *key, const UCHAR *src );
HRESULT MV_DES_SetKeyParm(const UCHAR *pbData,
						  UINT uSize,
						  MV_CRYPTO_PARM uParm,
						  PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_DES_MakeKey(PMV_CRYPTO_KEY_CTX pKeyCtx);

HRESULT MV_DES_Encrypt(const UCHAR *pbDataDec, UINT32 uDecSize,
					   UCHAR *pbDataEnc, UINT32* puEncSize,
					   PMV_CRYPTO_KEY_CTX pKeyCtx);
HRESULT MV_DES_Decrypt(const UCHAR *pbDataEnc, UINT32 uEncSize,
					   UCHAR *pbDataDec, UINT32* puDecSize,
					   PMV_CRYPTO_KEY_CTX pKeyCtx);

#endif
