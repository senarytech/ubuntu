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

#ifndef __DRM_MISC_AGENT_API_H__
#define __DRM_MISC_AGENT_API_H__

#include "OSAL_api.h"
#include "amp_client.h"
#include "amp_client_support.h"

#ifdef __cplusplus
extern "C" {
#endif

//HDCP Key Loading status
#define NOT_LOADED        0
#define LOAD_SUCCESS      1
#define FAILED_TO_LOAD   -1

////////////////////////////////////////////////////////////////////////////////
//! \brief Get HDCP Key loading status
//!
//! \param pLoadStatus (OUT):   Pointer to the expected hdcp load status
//!
//! \return Error code
//! \retval S_OK                Succeed
//! \retval E_INVALIDARG        Input argument is invalid
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_MISC_Get_HDCP_Loading_Status(INT32* pLoadStatus);

////////////////////////////////////////////////////////////////////////////////
//! \brief Calcluate SHA1 of raw private data for secure store
//!
//! \param sstoreFile(IN):   secure store file name
//! \param shaBuf(OUT):      buffer for sha1 hash
//!
//! \return Error code
//! \retval S_OK                Succeed
//! \retval E_INVALIDARG        Input argument is invalid
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_MISC_Utils_Calc_SStore_SHA1(CHAR* sstoreFile, UCHAR* sha1Buf);
#ifdef __cplusplus
}
#endif

#endif
