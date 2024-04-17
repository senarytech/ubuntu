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
//! \file DRM_Crypto_C2.h
//! \brief some data structures of C2 algorithm 
//!
//! Purpose:
//!
//!  Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef  __DRM_CRYPTO_MD5_H__
#define __DRM_CRYPTO_MD5_H__


typedef struct {
  UINT32 state[4];                                   /* state (ABCD) */
  UINT32 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  UINT8 buffer[64];                         /* input buffer */
} MD5_HASH_DATA;


HRESULT MV_MD5_DigestInit(PMV_CRYPTO_HASH_CTX pCtx);

HRESULT MV_MD5_DigestUpdate(PMV_CRYPTO_HASH_CTX pCtx, const unsigned char *pbData, unsigned int uSize);

HRESULT MV_MD5_DigestFinal(PMV_CRYPTO_HASH_CTX pCtx);


HRESULT MV_MD5_SetHashState(PMV_CRYPTO_HASH_CTX pCtx,
				UCHAR* pbState);

HRESULT MV_MD5_GetHashState(PMV_CRYPTO_HASH_CTX pCtx,
				UCHAR* pbState);


#endif

