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
//! \file amp_monitor.h
//!
//! \brief This file defines API and common type for health monitor
//!
//!     Version    Date                     Author
//!     V 1.00,    June 5 2014,             Ye Liu
//!
///////////////////////////////////////////////////////////////////////////////

#ifndef __AMP_MONITOR_H__
#define __AMP_MONITOR_H__
#include "amp_types.h"

#define AMP_TIMER_ID_VPP_ISR        (0x3001)


#define AMP_TIMER_DEFAULT_TIMEOUT   (1000)
#define AMP_HEALTH_HEARTBEAT_PERIOD (1)

typedef void *AMP_Monitor_Handle;

HRESULT AMP_Monitor_Initialize();
HRESULT AMP_Monitor_CreateTimer(IN CHAR *cName, IN UINT32 uiTimeout,
        IN void *user1, IN void *user2, OUT AMP_Monitor_Handle *phandle);
HRESULT AMP_Monitor_DestroyTimer(IN AMP_Monitor_Handle handle);
HRESULT AMP_Monitor_StartTimer(IN AMP_Monitor_Handle handle);
HRESULT AMP_Monitor_StopTimer(IN AMP_Monitor_Handle handle);
HRESULT AMP_Monitor_ReportStatus(IN AMP_Monitor_Handle handle);

UINT32 AMP_Monitor_GetTimeoutCounter(IN AMP_Monitor_Handle handle);
UINT32 AMP_Monitor_GetHeartbeatCounter();
HRESULT AMP_Monitor_GetHandleById(IN UINT32 TimerId, OUT AMP_Monitor_Handle *phandle);

#endif /* __AMP_MONITOR_H__ */
