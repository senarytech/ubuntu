
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
//! \file    DRM_CRYPTOENGINE_Def.h
//! \brief  Definition of  CRYPTOENGINE DRM COMMON
//! \author  Changsong Zhu
//! \date    June. 2010
///////////////////////////////////////////////////////////////////////////////


#ifndef __HDCP2_PM_H__
#define __HDCP2_PM_H__

#include "DRM_HDCP2x_Def.h"
#include "amp_client.h"
#include "amp_client_support.h"
#include "amp_drm_obj.h"
#include "DRM_Memory.h"
#ifdef __cplusplus
extern "C" {
#endif


#define MAX_SESSION_NUM 10


typedef unsigned int HDCP2_Auth_Ctx_Handle;


typedef enum
{
    IN_ONLY,
    OUT_ONLY,
    IN_OUT,
    CREATE_AUTH,
    DESTROY_AUTH,
    PREPARE_CONTENTKEY,
    KS_XOR_LC
}PARA_RPC_TYPE;

typedef struct
{

    PARA_RPC_TYPE MsgRPCType;
    HDCP2_Auth_Ctx_Handle AuthHandle;
    UINT32 InLen;
    UINT32 OutLen;
    UINT32 RtnStatus;
    UCHAR InBuf[1024];
    UCHAR OutBuf[1024];

}HDCP2_SHM_PARAM_FRAME;

#define MV_HDCP2X_PM_MSG_Dispatcher_RPC 0x0A90

int HDCP2x_Auth_RPC(PARA_RPC_TYPE rpc_type, HDCP2_Auth_Ctx_Handle * auth_handle, UCHAR * inbuf,
                     UINT32 inlen, UCHAR * outbuf, UINT32 outlen);

HRESULT MV_HDCP2_Crypto_Test_Agent();

#ifdef  __cplusplus
}
#endif

#endif

