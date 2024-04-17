/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (C) 2013-2017 Synaptics Incorporated. All rights reserved.
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
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS", AND
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
//! \file DRM_RSAEngine_Agent_API.h
//! \brief API of rsa engine Agent.
//!
//! Purpose:
//!
//!  Note:
//!
////////////////////////////////////////////////////////////////////////////////

#ifndef __DRM_RSAENGINE_AGENT_API_H__
#define __DRM_RSAENGINE_AGENT_API_H__

#include "OSAL_api.h"
#include "DRM_Memory.h"


////////////////////////////////////////////////////////////////////////////////
//! \brief Generate key function.
//!
//! \param e(in): --     key buffer e.
//! \param elen(in): --    Length of e.
//! \param dn(OUT): -- key buffer in bn format, min size is (bits/8) * 2 byte.
//! \param pdnlen(OUT): -- Length of bn.
//! \param bits (IN): --  Bits of modulus of RSA, MUST be 1024 or 2048 bits.
//!
//! \return Error code
//! \retval S_OK        Succeed
//! \retval E_INVALIDARG    Input argument is invalid
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_RsaEngine_gen_key(UINT8 *e, UINT32 elen, UINT8 *dn, UINT32 *pdnlen, UINT32 bits);


////////////////////////////////////////////////////////////////////////////////
//! \brief Encrypt function.
//!
//! \param flen (IN): --    Length of from
//! \param from(IN): --     Pointer to the input data
//! \param to(OUT): --      Pointer to the output data, to buffer should not smaller than modulus.
//! \param ptlen(OUT): --   Length of to
//! \param dn(in): --       key buffer in bn format.
//! \param dnlen(in): --    Length of bn.
//! \param ebuf(in): --     key buffer e.
//! \param elen(in): --    Length of e.
//! \param padding(IN): -- Padding type using OpenSSL definition.Support:
//!                        RSA_PKCS1_PADDING    1
//!                        RSA_PKCS1_OAEP_PADDING   4
//!
//! \return Error code
//! \retval S_OK        Succeed
//! \retval E_INVALIDARG    Input argument is invalid
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_RsaEngine_public_encrypt(UINT32 flen, const unsigned char *from,
                                    UINT32 *ptlen, unsigned char *to,
                                    UINT8 *dn, UINT32 dnlen,
                                    unsigned char *ebuf, UINT32 elen,
                                    UINT32 padding);

////////////////////////////////////////////////////////////////////////////////
//! \brief Decrypt function.
//!
//! \param flen (IN): --  Length of from
//! \param from(IN): -- Pointer to the input data
//! \param to(OUT): -- Pointer to the output data, to buffer should not smaller than modulus.
//! \param ptlen(OUT): -- Length of to
//! \param dn(in): --       key buffer in bn format.
//! \param dnlen(in): --    Length of bn.
//! \param padding(IN): -- Padding type using OpenSSL definition.Support:
//!                        RSA_PKCS1_PADDING    1
//!                        RSA_PKCS1_OAEP_PADDING   4
//!
//! \return Error code
//! \retval S_OK        Succeed
//! \retval E_INVALIDARG    Input argument is invalid
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_RsaEngine_private_decrypt(UINT32 flen, const unsigned char *from,
                                    UINT32 *ptlen, unsigned char *to,
                                    UINT8 *dn, UINT32 dnlen,
                                    unsigned char *ebuf, UINT32 elen,UINT32 padding);

////////////////////////////////////////////////////////////////////////////////
//! \brief Sign function.
//!
//! \param flen (IN): --  Length of from
//! \param from(IN): -- Pointer to the input data
//! \param to(OUT): -- Pointer to the output data, to buffer should not smaller than modulus.
//! \param ptlen(OUT): -- Length of to
//! \param dn(in): --       key buffer in bn format.
//! \param dnlen(in): --    Length of bn.
//! \param padding(IN): -- Padding type using OpenSSL definition.Support:
//!                        RSA_PKCS1_PADDING    1
//!                        RSA_NO_PADDING       3
//!
//! \return Error code
//! \retval S_OK        Succeed
//! \retval E_INVALIDARG    Input argument is invalid
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_RsaEngine_private_encrypt(UINT32 flen, const unsigned char *from,
                                    UINT32 *ptlen, unsigned char *to,
                                    UINT8 *dn, UINT32 dnlen,
                                    unsigned char *ebuf, UINT32 elen,UINT32 padding);

////////////////////////////////////////////////////////////////////////////////
//! \brief Verify function.
//!
//! \param flen (IN): --  Length of from
//! \param from(IN): -- Pointer to the input data
//! \param to(OUT): -- Pointer to the output data, to buffer should not smaller than modulus.
//! \param ptlen(OUT): -- Length of to
//! \param dn(in): --       key buffer in bn format.
//! \param dnlen(in): --    Length of bn.
//! \param ebuf(in): --     key buffer e.
//! \param elen(in): --    Length of e.
//! \param padding(IN): -- Padding type using OpenSSL definition.Support:
//!                        RSA_PKCS1_PADDING    1
//!                        RSA_NO_PADDING       3
//!
//! \return Error code
//! \retval S_OK        Succeed
//! \retval E_INVALIDARG    Input argument is invalid
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_RsaEngine_public_decrypt(UINT32 flen, const unsigned char *from,
                                    UINT32 *ptlen, unsigned char *to,
                                    UINT8 *dn, UINT32 dnlen,
                                    unsigned char *ebuf, UINT32 elen,
                                    UINT32 padding);


////////////////////////////////////////////////////////////////////////////////
//! \brief import key function.
//!
//! \param dnin(in): --    clear key buffer dn.
//! \param dnout(OUT): -- cipher key buffer dn.
//! \param pdnlen(OUT): -- Length of dn, MUST be 256 or 512 byte.
//!
//! \return Error code
//! \retval S_OK        Succeed
//! \retval E_INVALIDARG    Input argument is invalid
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_RsaEngine_import_private_key(UINT8 *dnin, UINT8 *dnout, UINT32 dnlen);


////////////////////////////////////////////////////////////////////////////////
//! \brief RSA sign and verify function.
//!
//! \param InputData(in):  -- plain text Msg to be signed.
//! \param Inlen(in):      -- Length of input message.
//!
//! \return Error code
//! \retval S_OK           Succeed
//! \retval E_OUTOFMEMORY  Out of memory
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_RSA_PKCS_PSS_Sign_Verify_Message(UINT8* inputData, UINT32 len);


#endif
