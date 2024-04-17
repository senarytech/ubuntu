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
//! \file amp_event_listener.h
//!
//! \brief This file declares API and common type for event callback client
//!        part.
//!
//! Purpose:
//!
//!
//!     Version    Date                     Author
//!     V 1.00,    Jan 6 2013,             Ye Liu
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef __AMP_EVENT_H__
#define __AMP_EVENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "com_type.h"
#include "isl/amp_event_queue.h"

/** \typedef AMP_EVENT_CALLBACK
    \brief This type define a prototype of event callback handle function
    \param hListener Listener handle that we're listening by
    \param pEvent Pointer to an event data
    \param pUserData Pointer to user private data provide in event register
 */
typedef HRESULT (*AMP_EVENT_CALLBACK)(HANDLE hListener,
                                      AMP_EVENT *pEvent,
                                      VOID *pUserData);

/** \func
    \brief This function used to construct a global event listener list
            and create a global instance for ampservice communication
 */
void AMP_Event_Listener_Init();

/** \func
    \brief This function used to destruct the global event listener list
            and destroy the global instance for ampservice communication
 */
void AMP_Event_Listener_Uninit();

/** \func
    \brief This function used to create an event listener with with max event
           code in
    \param uiEventCodeMax Reserved for future use.
    \param uiFlags Optional feature flags, reserve 0 so far.
 */
HANDLE AMP_Event_CreateListener(UINT uiEventCodeMax,
                            UINT uiFlags);

/** \func AMP_Event_DestroyListener(HANDLE hListener)
    \brief This function used to destroy an event listener
    \param hListener Event listener handle
 */
HRESULT AMP_Event_DestroyListener(HANDLE hListener);

/** \func AMP_Event_RegisterCallback(UINT32 ServiceID,
                                    AMP_EVENT_CODE eEventCode,
                                    UINT32 Param1, UINT32 Param2,
                                    UINT8* pPayload, UINT32 uiSize)
    \brief This function used to register event onto listener
    \param eEventCode Event code to be listened to
    \param pFunc Callback function when event occurs
    \param pUserData User private data for event callback function
 */
HRESULT AMP_Event_RegisterCallback(HANDLE hListener,
                            AMP_EVENT_CODE eEventCode,
                            AMP_EVENT_CALLBACK pFunc, VOID *pUserData);

/** \func AMP_Event_UnregisterCallback(HANDLE hEventListener,
                            AMP_EVENT_CODE eEventCode,
                            AMP_EVENT_CALLBACK pFunc)
    \brief This function used to unregister event
    \param eEventCode Event code to be cancelled
    \param pFunc Callback function when event occurs
 */
HRESULT AMP_Event_UnregisterCallback(HANDLE hEventListener,
                            AMP_EVENT_CODE eEventCode,
                            AMP_EVENT_CALLBACK pFunc);

/** \func AMP_Event_GetServiceID(HANDLE hListener)
    \brief This function used to get service id for p2p event callback
           user has to provide this id to intended modules or components,
    \param hListener Event listener handle
 */
UINT32 AMP_Event_GetServiceID(HANDLE hListener);

#ifdef __cplusplus
}
#endif

#endif /* __AMP_EVENT_H__ */
