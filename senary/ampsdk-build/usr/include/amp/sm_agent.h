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


#ifndef _SM_AGENT_H_
#define _SM_AGENT_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "sm_common.h"

#ifdef __cplusplus
#undef VOID
#define VOID void
#endif

//#define SM_USE_IOCTL

#ifndef S_OK
#define S_OK		0
#endif
#ifndef S_FALSE
#define S_FALSE		1
#endif

HRESULT MV_SM_Agent_Init(void);
HRESULT MV_SM_Agent_Close(void);
HRESULT MV_SM_Agent_Write_Msg(INT32 iModuleID, PVOID pMsgBody, INT32 iLen);
HRESULT MV_SM_Agent_Read_Msg_Ext(INT32 iModuleID, MV_SM_Message * pMsg, INT32* piLen, BOOL bBlock);
HRESULT MV_SM_Agent_Read_Msg(INT32 iModuleID, MV_SM_Message * pMsg, INT32* piLen);
HRESULT MV_SM_Agent_IO_CTRL(INT32 iModuleID,INT32 cmd);
#ifdef SM_USE_IOCTL
HRESULT MV_SM_Agent_ReadWrite_Msg(MV_SM_Message *pMsgSend, MV_SM_Message *pMsgRecv)
#endif

#ifdef __cplusplus
}
#endif

#endif
