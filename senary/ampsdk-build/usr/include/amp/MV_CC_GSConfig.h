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

#ifndef MV_CC_GSConfig_H
#define MV_CC_GSConfig_H

#include "com_type.h"
#include "OSAL_config.h"
#include "OSAL_type.h"

#ifdef __cplusplus
extern "C"
{
#endif	// #ifdef __cplusplus


////////////////////////////////////////////////////////////////////////////////
//! \brief Global Service ID (GS)Definition :
//!
////////////////////////////////////////////////////////////////////////////////
#define MV_CC_ServiceID_None				(0x00000000)
#define MV_CC_ServiceID_StaticStart			(0x00000001)
#define MV_CC_GSConfigList_EndFlagID		(0x7FFFFFFF)
#define MV_CC_ServiceID_DynamicStart		(0x80000000)
#define MV_CC_ServiceID_DynamicApply		(0x80000000)

#ifdef __cplusplus
}		// extern "C"
#endif	// #ifdef __cplusplus

#endif
