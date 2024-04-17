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

///////////////////////////////////////////////////////////////////////////////
//! \file		std_string.c
//! \brief 
//! \author		Junfeng Geng
//! \version	0.1
//! \date		March 2008
///////////////////////////////////////////////////////////////////////////////

#ifndef _STD_STRING_H_
#define _STD_STRING_H_

#include "ErrorCode.h"

//! \typedef std_string
typedef struct _std_string
{
	INT iRefCnt;
	BOOL bMask;
	char* str;
} std_string;

///////////////////////////////////////////////////////////////////////////////
//! \brief		Constructs a string initialized with pszContent
//!
//!	\param[in]	pszContent	A null-terminated string used to initialize content of string
//!
//!	\return		The pointer to the string
///////////////////////////////////////////////////////////////////////////////

std_string* std_string_ctor(CHAR* pszContent);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Destructs a string
//!
//! \param[in]	self	The pointer to the string.
///////////////////////////////////////////////////////////////////////////////

VOID std_string_dtor(std_string* self);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Erases the content of string
//!
//! \param[in]	self	The pointer to the string
//!
///////////////////////////////////////////////////////////////////////////////

HRESULT std_string_clear(std_string* self);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Returns the number of characters in a string.
//!
//! \param[in]	self	The pointer to the string
//! \param[out] puiSize	The pointer to the length
//!
//!	\return		S_OK if successful.
//!	\return		E_POINTER if self or puiSize is NULL.
///////////////////////////////////////////////////////////////////////////////

HRESULT std_string_length(std_string* self, UINT* puiLength);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Concat two strings. Sources strings are not modified.
//!
//! \param[in]	self	The pointer to the string
//! \param[in]	pOther	The pointer to the other string
//! \param[out] ppResult The pointer to the result
//!
//!	\return		S_OK if successful.
//!	\return		E_POINTER if self pOther or ppResult is NULL.
///////////////////////////////////////////////////////////////////////////////

HRESULT std_string_concat(std_string* self, std_string* pOther, std_string** ppResult);

#endif //<! #ifndef _std_string_H_
