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
//! \file amp_client_support.h
//!
//! \brief  This file provides the AMP client-side support API.
//!
//! Purpose:
//!
//!
//! Version, Date and Author :
//!   V 1.00,    ,                    Pontus Lidman
//!   V 1.10,    December 12 2012,    Justin Wu
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef __AMP_CLIENT_SUPPORT_H__
#define __AMP_CLIENT_SUPPORT_H__

#include <stdio.h>
#include "amp_types.h"
#include "amp_malloc.h"
#include "isl/amp_logger.h"
#include "isl/amp_event_queue.h"
#include "isl/amp_shm.h"
#include "isl/amp_debug.h"


/** \def AMP_RPC(ret, func, param...)
 *
 *  \brief This function is used to call a RPC function.
 *
 *         Since current RPC framework is CORBA based implementation, it has a
 *         CORBA specified parameter at the last place of parameter list. To
 *         offload the caller effort, we provide this macro function to process
 *         the CORBA return code. It will do some error handling for each of
 *         RPC function call.
 *
 *         For the error happenned in RPC communication layer, it will print
 *         some information to help identify issue. The message looks like:
 *         ${function_name}: RPC error: ${corba_error_string}.
 *
 *         The ${corba_error_string} could be:
 *         "IDL:omg.org/CORBA/BAD_PARAM:1.0": Corba intialize parameter format
 *                                            is error.
 *
 *         "IDL:omg.org/CORBA/NO_MEMORY:1.0": No enough heap memory when do RPC.
 *
 *         "IDL:omg.org/CORBA/IMP_LIMIT:1.0": Don't implemented since some
 *                                            limitation from system.
 *
 *         "IDL:omg.org/CORBA/COMM_FAILURE:1.0": Communication channel buffer
 *                                               not enough, bind socket
 *                                               fail or other socket and
 *                                               communication issue.
 *
 *         "IDL:omg.org/CORBA/INV_OBJREF:1.0": Invalidate Corba object handle.
 *
 *         "IDL:omg.org/CORBA/NO_IMPLEMENT:1.0": Corba feature not implemented.
 *
 *         "IDL:omg.org/CORBA/BAD_OPERATION:1.0": RPC function don't exist in
 *                                                server side.
 *
 *         "IDL:omg.org/CORBA/INV_IDENT:1.0": Invalide identifier for host name.
 *
 *         As AMP RPC function call rule, all AMP RPC API should have a return
 *         code. This function will check all RPC function call's return value
 *         and print a log message when error happenned. The message looks
 *         like: ${function_name}: RPC function error: ${error_code}.
 *
 *         Please note this macro function should be used only for CORBA RPC
 *         call.
 *
 *         This function can be used only after the AMP_Initialize() call
 *         returned success.
 *
 *  \param[out] ret The variable used to storage the RPC function return code.
 *  \param[in] func The RPC function name.
 *  \param[in] param The RPC function parameter list. Please ref related RPC
 *                   function declaration.
 *
 *  \retval No return code since the RPC function's reture code has been
 *          returned through parameter ret.
 *
 *  \sa AMP_ReleaseObject
 */
#define AMP_RPC(ret, func, param...)                                        \
    do {                                                                    \
        CORBA_Environment _ev;                                              \
        AmpMemSet(&_ev, 0, sizeof(CORBA_Environment));                      \
        ret = AMP_RPC_ParamCheck(#func, (HRESULT (*)())func, param);        \
        if (ret == SUCCESS) {                                               \
            ret = func(param, &_ev);                                        \
            if (_ev._major != CORBA_NO_EXCEPTION) {                         \
                AMPLOGE("%s: RPC internal library error: %s\n",             \
                        #func, CORBA_exception_id(&_ev));                   \
                CORBA_exception_free(&_ev);                                 \
                ret = HRESULT_GEN(FW, ERR_RCPERROR);                        \
            }                                                               \
            if (ret != SUCCESS) {                                           \
                AMPLOGE("%s: AMP function call return an error: 0x%08x\n",  \
                        #func, ret);                                        \
            } else {                                                        \
                AMP_RPC_Cleanup(#func, (HRESULT (*)())func, param);         \
            }                                                               \
            AMP_ASSERT(ret != HRESULT_GEN(FW, ERR_RCPERROR));               \
        }                                                                   \
    } while (0)

/** \def AMP_ReleaseObject(obj)
 *
 *  \brief This function is used to release a RPC object handle.
 *
 *         The RPC object handle could be allocated and returned by AMP factory
 *         (by AMP_FACTORY_CreateXxxx()), or returned by AMP_NameToInstance().
 *
 *         If the RPC object handle is useless it should be released by call
 *         this function, else it will cause heap memory leak.
 *
 *         Inside this function, it calls CORBA_Object_release to release the
 *         Corba object. It will exit current process if meet any issues
 *         (actually it will never get a Corba error since CORBA_Object_release
 *         just try to call free() to free some memory that allocated from
 *         heap). Please avoid call this function two or more times for the
 *         sample object handle, else it could meet issue.
 *
 *         This function can be used even before call AMP_Initialize(). It's
 *         a standalone function.
 *
 *  \param[in] obj The RPC object handle.
 *
 *  \retval No return value.
 *
 *  \sa AMP_FACTORY_CreateComponent
 *  \sa AMP_FACTORY_CreateDisplayService
 *  \sa AMP_FACTORY_CreateSoundService
 *  \sa AMP_FACTORY_CreateDrmService
 *  \sa AMP_FACTORY_CreateLibrary
 *  \sa AMP_FACTORY_CreatePipeline
 *  \sa AMP_NameToInstance
 */
#define AMP_ReleaseObject(obj)                                            \
    do {                                                                    \
        CORBA_Environment _ev;                                              \
        AmpMemSet(&_ev, 0, sizeof(CORBA_Environment));                      \
        if (isFactoryObject(obj)) {                                         \
            AMPLOGH("Release AMP factory object!");               \
        }                                                            \
        CORBA_Object_release(obj, &_ev);                                \
        if (_ev._major != CORBA_NO_EXCEPTION) {                         \
            AMPLOGE("CORBA_Object_release: %s", CORBA_exception_id(&_ev));\
            CORBA_exception_free(&_ev);                                 \
            exit(EXIT_FAILURE);                                         \
        }                                                               \
    } while (0)


/** \typedef HRESULT (*AMP_CONNECT_CALLBACK)(
 *          AMP_IN      AMP_COMPONENT           hCompObj,
 *          AMP_IN      AMP_PORT_IO             ePortIo,
 *          AMP_IN      UINT32                  uiPortIdx,
 *          AMP_IN      struct AMP_BD_ST        *hBufDesc,
 *          AMP_IN      void                    *pUserData)
 *
 *  \brief This type defined the connection callback for application.
 *
 *         Any of AMP component could be connect with another AMP component or
 *         application. The connection is used to push media data buffer
 *         (buffer descriptor, BD) to consummer (applicatoin or downstream
 *         component), and it also be used to release back buffer when comsumer
 *         used out the media data.
 *
 *         Since we don't support Corba based server to client RPC callback, we
 *         provided another approach to support this kind feature. Then the
 *         application could get notification from server to get the buffer
 *         push and release event callback.
 *
 *         AMP_CONNECT_CALLBACK should provided by application when call
 *         AMP_ConnectApp(). When application connects to AMP component's input
 *         port, the callback will be called when the input buffer has been
 *         consumed by component. When applicatoin connects to AMP component's
 *         output port, the callback will be called when the compnent pushed
 *         out a data buffer.
 *
 *         The server to client callback is based on AMP event listener, please
 *         ref related documents for detail.
 *
 *         Please note this callback will be executed in a event listener's
 *         provided thread which running in client process. This thread is a
 *         standalone thread from other callbacks since we always create a new
 *         event listener for each of AMP_CONNECT_CALLBACK.
 *
 *         Since the connection callback is used to push or release buffer, it
 *         will be called only when the connection has been built successfully.
 *
 *  \param[in] hCompObj Indicate the component object handle.
 *  \param[in] ePortIo Indicate the callback is from input port or output port.
 *  \param[in] uiPortIdx Indicate the port index.
 *  \param[in] hBufDesc Indicate the BD pushed or released from server.
 *  \param[in] pUserData The optional data structure pointer provided by
 *             application when do connection to make application easy find
 *             it's context.
 *
 *  \retval The callback return value don't cared by system even it could
 *          return error code based on execut result.
 *
 *  \sa Event listener
 *  \sa Buffer descriptor
 *  \sa AMP_ConnectApp
 */
typedef HRESULT (*AMP_CONNECT_CALLBACK)(
            AMP_IN      AMP_COMPONENT           hCompObj,
            AMP_IN      AMP_PORT_IO             ePortIo,
            AMP_IN      UINT32                  uiPortIdx,
            AMP_IN      struct AMP_BD_ST        *hBufDesc,
            AMP_IN      void                    *pUserData);


#ifdef __cplusplus
extern "C"
{
#endif

/** \fn HRESULT AMP_Initialize(
 *          AMP_IN      int                     uiArgc,
 *          AMP_IN      char                    **pArgv,
 *          AMP_OUT     AMP_FACTORY             *hFactory);
 *
 *  \brief This function used to initialize AMP environment.
 *
 *         Application process should call this function to initialize AMP
 *         enironment before any of other AMP function calls. This function
 *         could also be called several times, but only the first call will
 *         initialize the environment.
 *
 *         This function will initialize the AMP debug logger module, component
 *         connection management structure (used by AMP_ConnectApp), connect to
 *         AMP server and get the factory handle.
 *
 *         Actually uiArgc and pArgv will not be used since current
 *         implementation has fixed the AMP server setting (Corba server IP,
 *         listening port and root object) inside this function. We keep those
 *         two arguments just to make it is compatiable with old programs.
 *
 *         This function will record the factory's handle to an internal static
 *         variable. And this variable also works as a flag that indicate the
 *         initialize is done or not. Please note it's no any protection for
 *         this flag, so please avoid call this function in different thread in
 *         a process.
 *
 *         After this function return SUCCESS, application can call AMP_RPC()
 *         to execute other AMP provided APIs.
 *
 *  \param[in] uiArgc The optional command line parameter counter. Don't used
 *                    now.
 *  \param[in] pArgv The optional command line parameter list. Don't used now.
 *  \param[out] hFactory The pointer to get current AMP factory handle. It will
 *                       not return factory handle if it's equal to NULL.
 *
 *  \retval SUCCESS Initialize successed.
 *  \retval HRESULT_GEN(FW, ERR_NOMEM) No enough heap memory for connection
 *                                     management data structure.
 *  \retval HRESULT_GEN(FW, ERR_RCPERROR) Fail to connect to AMP server.
 *
 *  \sa AMP_FACTORY
 *  \sa AMP_ConnectApp
 *  \sa AMP_RPC
 *  \sa AMP_Deinitialize
 */
extern HRESULT AMP_Initialize(
            AMP_IN      int                     uiArgc,
            AMP_IN      char                    **pArgv,
            AMP_OUT     AMP_FACTORY             *hFactory);

/** \fn HRESULT AMP_Deinitialize();
 *
 *  \brief This function used to de-initialize AMP environment.
 *
 *         Application process should call this function to de-initialize AMP
 *         enironment if it don't need call any of AMP function calls.
 *         This function could also be called several times, but only the
 *         first call will de-initialize the environment.
 *
 *         This function will release the component connection management
 *         structure (used by AMP_ConnectApp), and free the factory handle.
 *
 *         This function will also clear the factory's handle which storaged in
 *         an internal static variable. This variable also works as a
 *         flag that indicate the initialize is done or not.
 *         Please note it's no any protection for this flag, so please avoid
 *         call this function in different thread in a process.
 *
 *         After this function return SUCCESS, application shall not can call
 *         AMP_RPC() to execute other AMP provided APIs.
 *
 *  \retval SUCCESS Initialize successed.
 *
 *  \sa AMP_FACTORY
 *  \sa AMP_Initialize
 */
extern HRESULT AMP_Deinitialize(void);

/** \fn HRESULT AMP_GetFactory(
 *          AMP_OUT     AMP_FACTORY         *hFactory);
 *
 *  \brief This function is used to get current process's AMP factory handle.
 *
 *         The AMP factory's handle has been storegged in an internal
 *         data structure when the client process first successed to call
 *         AMP_Initialize(). Else it will return an error code.
 *
 *         Application shall not use the hFactory after the application called
 *         AMP_Deinitailize() since it's already be freed.
 *
 *  \param[out] hFactory The pointer to get current AMP factory handle. It will
 *                       not return factory handle if it's equal to NULL.
 *
 *  \retval SUCCESS The AMP intialize is successed.
 *  \retval HRESULT_GEN(FW, ERR_NOSWRSC) Can't get the AMP factory handle since
 *                                       haven't call AMP_Initialize() or call
 *                                       it failed.
 *
 *  \sa AMP_Initialize
 */
extern HRESULT AMP_GetFactory(
            AMP_OUT     AMP_FACTORY         *hFactory);

/** \fn HRESULT AMP_RegisterServerExitCb(
 *          AMP_IN      void (*callback)());
 *
 *  \brief This function is used to set AMP server on exit callback.
 *
 *         The AMP framework is moniting the AMP server since it succesed the
 *         first RPC call. By default, the framework will call a default
 *         callback that embedded in framework, which one will call exit(0) to
 *         make the client exit. But application can also replace this default
 *         one by call this function. After the function returned SUCCESS, the
 *         new callback will applied.
 *
 *         Since all RPC function call will return error and all objects handles
 *         became invalidate after the AMP server exited, the client registered
 *         callback should make itself exit or restart before it return from
 *         the callback.
 *
 *         Application shall not call this function after the application called
 *         AMP_Deinitailize().
 *
 *  \param[in] callback The function pointer which used replace the default
 *                      server exit calback functin. The callback shouldn't
 *                      return but exit or restart the client process.
 *
 *  \retval SUCCESS The AMP intialize is successed.
 *  \retval HRESULT_GEN(FW, ERR_RCPERROR) Haven't call AMP_Initialize()
 *                                        successfully.
 *
 *  \sa AMP_Initialize
 */
extern HRESULT AMP_RegisterServerExitCb(
            AMP_IN      void (*callback)(void *));

/** \fn HRESULT AMP_ConnectApp(
 *          AMP_IN      AMP_COMPONENT           hCompObj,
 *          AMP_IN      AMP_PORT_IO             ePortIo,
 *          AMP_IN      UINT32                  uiPortIdx,
 *          AMP_IN      AMP_CONNECT_CALLBACK    pCallback,
 *          AMP_IN      void                    *pUserData);
 *
 *  \brief This function is used to connect component to application.
 *
 *         AMP component could connect with application. The connection is used
 *         to transmit buffer between component and application.
 *
 *         If application connect with component's input port, the application
 *         should call AMP_COMPONENT_PushBD() to push buffer to AMP component's
 *         input port. After the buffer is consumed by component, it will
 *         generate event callback (pCallback) to application.
 *
 *         If application connect with component's output port, the component
 *         will generate event callback to application for push a new buffer.
 *         The application should call AMP_COMPONENT_PushBD() to output port to
 *         release back the buffer after it consumed the media data.
 *
 *         Inside AMP_ConnectApp(), it will call AMP_COMPONENT_QueryInfo() to
 *         query the component's information, then create a new event listener
 *         for the event callback. At last call AMP_COMPONENT_SetupPort() to
 *         setup the connection in component.
 *
 *         This function should be called only after the component has been
 *         openned successful.
 *
 *         Currently it support up to 256 application connections for each of
 *         application process.
 *
 *  \param[in] hCompObj The component object handle.
 *  \param[in] ePortIo Connect to component's input or output port.
 *  \param[in] uiPortIdx Component port index to be connected.
 *  \param[in] pCallback Application provided connection callback.
 *  \param[in] pUserData The optional data structure pointer provided by
 *             application when do connection to make application easy find
 *             it's context.
 *
 *  \retval SUCCESS Connect component with application successed.
 *  \retval HRESULT_GEN(EVENTLI, ERR_NOSWRSC) Exceed connection limitation.
 *  \retval HRESULT_GEN(FW, ERR_ERRPARAM) ePortIo is a wrong value.
 *  \retval HRESULT_GEN(FW, ERR_RCPERROR) RPC error since communication failed
 *                                        or data structure parameter error.
 *  \retval Other error returned from:
 *          . AMP_COMPONENT_QueryInfo();
 *          . AMP_Event_RegisterCallback();
 *          . AMP_COMPONENT_SetupPort().
 *
 *  \sa AMP_COMPONENT_QueryInfo
 *  \sa AMP_Event_RegisterCallback
 *  \sa AMP_COMPONENT_SetupPort
 *  \sa AMP_COMPONENT_PushBD
 *  \sa AMP_DisconnectApp
 */
extern HRESULT AMP_ConnectApp(
            AMP_IN      AMP_COMPONENT           hCompObj,
            AMP_IN      AMP_PORT_IO             ePortIo,
            AMP_IN      UINT32                  uiPortIdx,
            AMP_IN      AMP_CONNECT_CALLBACK    pCallback,
            AMP_IN      void                    *pUserData);

/** \fn HRESULT AMP_DisconnectApp(
 *          AMP_IN      AMP_COMPONENT           hCompObj,
 *          AMP_IN      AMP_PORT_IO             ePortIo,
 *          AMP_IN      UINT32                  uiPortIdx,
 *          AMP_IN      AMP_CONNECT_CALLBACK    pCallback);
 *
 *  \brief This function is used to disconnect component from application.
 *
 *         This function will destroy the event listener that used for event
 *         callback. Then call AMP_COMPONENT_ClearPort() to clear the connection
 *         in component for specifyed port.
 *
 *         This function should be called only when the correspond component's
 *         port connection was built.
 *
 *  \param[in] hCompObj Component object handle.
 *  \param[in] ePortIo Input port or output port.
 *  \param[in] uiPortIdx Port index.
 *  \param[in] pCallback Application provided connection callback.
 *
 *  \retval SUCCESS Disconnect component from application successed.
 *  \retval HRESULT_GEN(FW, ERR_ERRPARAM) ePortIo is a wrong value.
 *  \retval HRESULT_GEN(FW, ERR_RCPERROR) RPC error since communication failed
 *                                        or data structure parameter error.
 *  \retval Other error returned from:
 *          . AMP_Event_DestroyListener();
 *          . AMP_COMPONENT_ClearPort().
 *
 *  \sa AMP_Event_DestroyListener
 *  \sa AMP_COMPONENT_ClearPort
 *  \sa AMP_ConnectApp
 */
extern HRESULT AMP_DisconnectApp(
            AMP_IN      AMP_COMPONENT           hCompObj,
            AMP_IN      AMP_PORT_IO             ePortIo,
            AMP_IN      UINT32                  uiPortIdx,
            AMP_IN      AMP_CONNECT_CALLBACK    pCallback);

/** \fn HRESULT AMP_ConnectComp(
 *          AMP_IN      AMP_COMPONENT           hUpCompObj,
 *          AMP_IN      UINT32                  uiUpPortIdx,
 *          AMP_IN      AMP_COMPONENT           hDownCompObj,
 *          AMP_IN      UINT32                  uiDownPortIdx);
 *
 *  \brief This function is used to connect two components.
 *
 *         Application connect two component together, connect upstream
 *         component's output port with downstream component's input port.
 *
 *         After connection build up, the upstream component can push media
 *         data buffer to downstream component. The downstream component will
 *         release back buffer after the media data buffer has been consumed.
 *
 *         This function will call AMP_COMPONENT_QueryInfo() and
 *         AMP_COMPONENT_QueryPort() to get the component's information. Then
 *         call AMP_COMPONENT_SetupPort() for both of components to setup the
 *         connection.
 *
 *         This function can only be called after both of components has been
 *         opened successful.
 *
 *         Don't like connect with application, it's no limitation for the
 *         connection number between two components.
 *
 *  \param[in] hUpCompObj Upstream component handle.
 *  \param[in] uiUpPortIdx Upstream component output port index.
 *  \param[in] hDownCompObj Downstream component handle.
 *  \param[in] uiDownPortIdx Downstream component input port index.
 *
 *  \retval SUCCESS Connect two components successed.
 *  \retval Other error returned from:
 *          . AMP_COMPONENT_QueryInfo();
 *          . AMP_COMPONENT_QueryPort().
 *
 *  \sa AMP_COMPONENT_QueryInfo
 *  \sa AMP_COMPONENT_QueryPort
 *  \sa AMP_COMPONENT_SetupPort
 *  \sa AMP_DisconnectComp
 */
extern HRESULT AMP_ConnectComp(
            AMP_IN      AMP_COMPONENT           hUpCompObj,
            AMP_IN      UINT32                  uiUpPortIdx,
            AMP_IN      AMP_COMPONENT           hDownCompObj,
            AMP_IN      UINT32                  uiDownPortIdx);

/** \fn HRESULT AMP_DisconnectComp(
 *          AMP_IN      AMP_COMPONENT           hUpCompObj,
 *          AMP_IN      UINT32                  uiUpPortIdx,
 *          AMP_IN      AMP_COMPONENT           hDownCompObj,
 *          AMP_IN      UINT32                  uiDownPortIdx);
 *
 *  \brief This function is used to disconnect two components.
 *
 *         This function will call AMP_COMPONENT_ClearPort() to clear both
 *         connected components port.
 *
 *         This function should be called only when the correspond components
 *         port connection was built.
 *
 *  \param[in] hUpCompObj Upstream component handle.
 *  \param[in] uiUpPortIdx Upstream component output port index.
 *  \param[in] hDownCompObj Downstream component handle.
 *  \param[in] uiDownPortIdx Downstream component input port index.
 *
 *  \retval SUCCESS Disconnect two components successed.
 *  \retval Other error returned from:
 *          . AMP_COMPONENT_ClearPort();
 *
 *  \sa AMP_COMPONENT_ClearPort
 *  \sa AMP_ConnectComp
 */
extern HRESULT AMP_DisconnectComp(
            AMP_IN      AMP_COMPONENT           hUpCompObj,
            AMP_IN      UINT32                  uiUpPortIdx,
            AMP_IN      AMP_COMPONENT           hDownCompObj,
            AMP_IN      UINT32                  uiDownPortIdx);

/** \fn HRESULT AMP_ObjectToName(
 *          AMP_IN      void                    *hObject,
 *          AMP_OUT     char                    **pName);
 *
 *  \brief This function is used to get the object name from object handle.
 *
 *         The object (components and services) handle is created and used in
 *         an AMP clien process. Its an process orented handle. To make another
 *         process get the same object, we need translet object handle to an
 *         object name. Then we can send this name to another client process
 *         and call AMP_NameToObject() to get the object handle in another
 *         client process.
 *
 *         This feature is very useful if a component is shared between several
 *         applicatoins that running in different processes.
 *
 *         This function will call Corba provided API
 *         CORBA_ORB_object_to_readable_string() to get the object name. The
 *         name string memory will allocated from heap in this function.
 *         Application should free the name string memory if it don't need it
 *         anymore.
 *
 *  \param[in] hObject The object handle. It could be component or service
 *                     handle which returned from AMP_FACTORY_CreateXxx().
 *  \param[out] pName Return the object name string start address. It can't
 *                    be NULL pointer.
 *
 *  \retval SUCCESS Get object name successed.
 *  \retval HRESULT_GEN(FW, ERR_NOMEM) No enough heap memory for the name.
 *
 *  \sa AMP_NameToObject
 *  \sa AMP_FACTORY
 */
extern HRESULT AMP_ObjectToName(
            AMP_IN      void                    *hObject,
            AMP_OUT     char                    **pName);

/** \fn HRESULT AMP_NameToObject(
            AMP_IN      char                    *pName,
            AMP_OUT     void                    **hObject);
 *
 *  \brief This function is used to get the object from object name.
 *
 *         The object (components and services) handle is created and used in
 *         an AMP clien process. Its an process orented handle. To make another
 *         process get the same object, we need translet object handle to an
 *         object name by call AMP_ObjectToName(). Then we can send this name
 *         to another client process and call this function to get the
 *         object handle in another client process.
 *
 *         This feature is very useful if a component is shared between several
 *         applicatoins that running in different processes.
 *
 *         This function will call Corba provided API
 *         CORBA_ORB_string_to_object() to get the object handle. The
 *         object handle memory will allocated in this function.
 *         Application should free the object handle by call
 *         AMP_ReleaseObject() if it don't need it anymore.
 *
 *  \param[in] pName The object name. It could come from AMP_ObjectToName().
 *  \param[out] hObject Return the object handle start address.
 *
 *  \retval SUCCESS Get object handle successed.
 *  \retval HRESULT_GEN(FW, ERR_ERRPARAM) The name can't be converted to handle.
 *                                        It can't be NULL pointer.
 *
 *  \sa AMP_ObjectToName
 *  \sa AMP_FACTORY
 */
extern HRESULT AMP_NameToObject(
            AMP_IN      char                    *pName,
            AMP_OUT     void                    **hObject);


/** \fn HRESULT AMP_API_RegisterWatchdogCb(
            AMP_IN      void (*callback)(AMP_EVENT_CODE, void *),
            AMP_IN      void *pUserdata);
 *
 *  \brief This function is used to register heatbeat of ampservice
 *
 *         ampservice will broadcast heartbeat every second. This API can be
 *         used to monitor the status of ampservice.
 *
 *  \param[in] function pointer that will be called when heatbeat is received
 *  \param[in] a pointer that will be passed to callback
 *
 *  \retval SUCCESS Get object handle successed.
 */
extern HRESULT AMP_API_RegisterWatchdogCb(
            AMP_IN      void (*callback)(AMP_EVENT_CODE, void *),
            AMP_IN      void                    *pUserdata);

/* Internal functions, don't exported to customers. */
extern int isFactoryObject(CORBA_Object obj);
extern HRESULT AMP_RPC_ParamCheck(const char *name, HRESULT (*func)(void), void *hObject,
                                  ...);
/** \deprecated since 12 Feb 2018, this API is no longer supported.
 * It will be removed by the end of 2018;
 * please use AMP_RPC_ParamCheck instead, need rebuild all binrary which depend on AMP.
 */
extern HRESULT AMP_RPC_ParemCheck(const char *name, HRESULT (*func)(void), void *hObject,
                                  ...);
extern HRESULT AMP_RPC_Cleanup(const char *name, HRESULT (*func)(void), void *hObject,
                               ...);

extern void AMP_Obj_Mutex_Lock();
extern void AMP_Obj_Mutex_Unlock();
#ifdef __cplusplus
}
#endif

/* The old API taht shalln't be used anymore. Keeping here to make it pass
   compiling */
#define AMP_FACTORY_Release(obj)        AMP_ReleaseObject(obj)

#endif /* __AMP_CLIENT_SUPPORT_H__ */

