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
//! \file amp_event_service.h
//!
//! \brief This file defines API and common type for event service
//!
//! Purpose:
//!
//!
//!     Version    Date                     Author
//!     V 1.00,    Jan 6 2013,             Ye Liu
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef __AMP_EVENT_SERVICE_H__
#define __AMP_EVENT_SERVICE_H__

#include "com_type.h"
#include "isl/amp_event_queue.h"

/** \func AMP_InitializeRemoteEventService();
 *  \brief This function used to initialize all remote event serive before any
 *         function below is called.
 */
HRESULT AMP_InitializeRemoteEventService();

/** \func AMP_DeinitialzeRemoteEventService();
 *  \brief This function used to deinitialize event remote event serive.
 */
HRESULT AMP_DeinitialzeRemoteEventService();

/** \func AMP_RemoteEventCallback(AMP_EVENT_CODE eEventCode,
 *                                UINT32 Param1, UINT32 Param2,
 *                                UINT8* pPayload, UINT32 uiSize);
 *  \brief This function used to broadcast event to all listeners which
 *         registers this event.
 */
HRESULT AMP_RemoteEventCallback(AMP_EVENT_CODE eEventCode,
                                 UINT32 Param1, UINT32 Param2);

/** \def REMOTE_EVENT_SERVERID_DEFAULT
 *  \brief service id is used to unidirection event callback to specified
 *         client entity, default value is intent to broadcast event.
 */
#define REMOTE_EVENT_SERVERID_DEFAULT   0x80000000

/** \func AMP_RemoteEventCallbackEx(UINT32 ServiceID,
 *                                  AMP_EVENT_CODE eEventCode,
 *                                  UINT32 Param1, UINT32 Param2,
 *                                  UINT8* pPayload, UINT32 uiSize);
 *  \brief This function used to send event to listener with service ID
 */
HRESULT AMP_RemoteEventCallbackEx(UINT32 ServiceID, AMP_EVENT_CODE eEventCode,
                                 UINT32 Param1, UINT32 Param2,
                                 UINT8* pPayload, UINT32 uiSize);

/** \func AMP_RemoteEventCallbackPro(UINT32 ServiceID, AMP_EVENT *pEvent)
 *  \brief This function used to send event to listener with service ID, use
 *         compose and event before calling this function.
 */
HRESULT AMP_RemoteEventCallbackPro(UINT32 ServiceID, AMP_EVENT *pEvent);

HRESULT ISL_Listener_Create(UINT uiSrvIdN, UINT uiSrvIdC, HANDLE *phListener);
HRESULT ISL_Listener_Destory(HANDLE hListener);
HRESULT ISL_Listener_AddEvent(HANDLE hListener, AMP_EVENT_CODE eEventCode,
    UINT Param1, UINT Param2);
HRESULT ISL_Listener_RemoveEvent(HANDLE hListener,
    AMP_EVENT_CODE eEventCode);
HRESULT ISL_Listener_Stop(HANDLE hListener);

#endif /* __AMP_EVENT_SERVICE_H__ */
