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
//! \file DRM_Crypto_SHA256.h
//! \brief declaration of SHA256 algorithm relative functions for Crypto library. 
//!
//! Purpose:
//!
//!  Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef  __DRM_CRYPTO_SHA256_H__
#define __DRM_CRYPTO_SHA256_H__

typedef struct tagSHA256HashData
{

  UINT32  h0,h1,h2,h3,h4,h5,h6,h7;
  UINT32  nblocks;
  UINT8    buf[SHA256_BLOCK_BYTES_LEN];
  UINT32  count;
}SHA256_HASH_DATA;


HRESULT MV_SHA256_DigestInit(PMV_CRYPTO_HASH_CTX pCtx);
HRESULT MV_SHA256_DigestUpdate(PMV_CRYPTO_HASH_CTX pCtx,
                            const UCHAR* pbData,
                            UINT uSize );
HRESULT MV_SHA256_DigestFinal(PMV_CRYPTO_HASH_CTX pCtx);
HRESULT MV_SHA256_SetHashState(PMV_CRYPTO_HASH_CTX pCtx,
				UCHAR* pbState);

HRESULT MV_SHA256_GetHashState(PMV_CRYPTO_HASH_CTX pCtx,
				UCHAR* pbState);

#endif

