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
//! \file amp_config_passer.c
//!
//! \brief This file implement the config passer support functions.
//!
//! Purpose: Provide TinyXML based wrap funtions for AMP config file passing.
//!
//!
//!     Version    Date                     Author
//!     V 1.00,    May 30 2013,             Justin Wu.
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef __AMP_CONFIG_PASSER_H__
#define __AMP_CONFIG_PASSER_H__

#include "amp_types.h"
#include "tinyxml.h"


extern HRESULT AMP_ConfigPasserInitialize(
            AMP_IN      TiXmlDocument   *pDoc);

extern HRESULT AMP_GetElementByPath(
            AMP_IN      TiXmlDocument   *pDoc,
            AMP_IN      const char      **sElePath,
            AMP_IN      int             iListNum,
            AMP_OUT     TiXmlElement    **pEleHandle);

extern HRESULT AMP_GetElementAttribString(
            AMP_IN      TiXmlElement    *pEleHandle,
            AMP_IN      const char      *sName,
            AMP_OUT     const char      **sValue);

extern HRESULT AMP_GetElementAttribInteger(
            AMP_IN      TiXmlElement    *pEleHandle,
            AMP_IN      const char      *sName,
            AMP_OUT     int             *iValue);

extern HRESULT AMP_GetElementAttribUInteger(
            AMP_IN      TiXmlElement    *pEleHandle,
            AMP_IN      const char      *sName,
            AMP_OUT     UINT32          *uiValue);

extern HRESULT AMP_GetElementAttribHex(
            AMP_IN      TiXmlElement *pEleHandle,
            AMP_IN      const char      *sName,
            AMP_OUT     UINT32          *uiValue);

#endif /* __AMP_CONFIG_PASSER_H__ */
