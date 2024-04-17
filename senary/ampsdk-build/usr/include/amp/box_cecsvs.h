/*******************************************************************************
*                Copyright 2012, MARVELL SEMICONDUCTOR, LTD.                   *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL.                      *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
*                                                                              *
* MARVELL COMPRISES MARVELL TECHNOLOGY GROUP LTD. (MTGL) AND ITS SUBSIDIARIES, *
* MARVELL INTERNATIONAL LTD. (MIL), MARVELL TECHNOLOGY, INC. (MTI), MARVELL    *
* SEMICONDUCTOR, INC. (MSI), MARVELL ASIA PTE LTD. (MAPL), MARVELL JAPAN K.K.  *
* (MJKK), MARVELL ISRAEL LTD. (MSIL).                                          *
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//! \file box_cecsvs.h
//!
//! \Brief Description: This file defines data structs for Box CEC Service .
//!
//! Purpose:
//!
//!
//!     Version          Date                   Author
//!  V 0.00,      April 29. 2013,               Nishchal: Added for AMP
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef _BOX_CEC_SVS_H_
#define _BOX_CEC_SVS_H_

#include "cecsvs_type.h"

// Enum
// cec enum
typedef enum {
    BOX_CEC_SVS_ADDRESS_STS_OK = 0,
    BOX_CEC_SVS_ADDRESS_STS_NOT_OK = 1,
} BOX_CEC_SVS_ENUM_ADRESS_STATUS;

typedef struct _ui_command_ {
    INT userControlCode;        // CEC_USER_CONTROL_CODE
    INT fnOperandPresent;
    union {
        INT playMode;           // CEC_PLAY_MODE
        UINT8 uiFnMedia;
        UINT8 uiFnSelAVIn;
        UINT8 uiFnSelAudIn;
        UINT8 uiFnSelSoundCtrl; // sound ppt ctrl
        UINT8 uiFnSelBrdCstIn;  // ui broadcast type
    } fnOperand;
} BOX_CEC_SVS_UI_COMMAND;
// call back function declarations

typedef struct _BOX_CEC_SVS_DEV_INFO    // CEC device information
{
    UINT32 phyAddr;             // device’s physical address
    int logAddr;                // device’s logical address
    int portID;                 // connected HDMI input port ID
    char name[15];              // device’s name (optional)
    int vendorID;               // dev vendor ID
} BOX_CEC_SVS_DEV_INFO;

// cec port table structure
typedef struct _BOX_CEC_SVS_PORT_INFO {
    int portID;
    UINT32 phyAddr;
} BOX_CEC_SVS_PORT_INFO;

typedef int (*BOX_CEC_SVS_ADDRESS_ALLOC_STATUS_CBFUNC) (void *ContextParam,
                                                        int status);
typedef int (*BOX_CEC_SVS_RECV_FEATURE_ABORT_CBFUNC) (void *ContextParam,
                                                      int opcode, int reason);
typedef int (*BOX_CEC_SVS_STANDBY_CBFUNC) (void *ContextParam);
typedef int (*BOX_CEC_SVS_GET_STANDBY_STATUS_CBFUNC) (void *ContextParam,
                                                      int *ignoreStandbyCmd);
typedef int (*BOX_CEC_SVS_GET_OSD_STRING_CBFUNC) (void *ContextParam,
                                                  char *str);
typedef int (*BOX_CEC_SVS_SET_OSD_STRING_CBFUNC) (void *ContextParam, char *str,
                                                  int addr);
typedef int (*BOX_CEC_SVS_MENU_STATUS_REQUEST_CBFUNC) (void *ContextParam,
                                                       int status);
typedef int (*BOX_CEC_SVS_REQUEST_MENU_STATUS_CBFUNC) (void *ContextParam,
                                                       int *status);
typedef int (*BOX_CEC_SVS_UC_PRESSED_CBFUNC) (void *ContextParam,
                                              BOX_CEC_SVS_UI_COMMAND key_info);
typedef int (*BOX_CEC_SVS_LONG_UC_PRESSED_CBFUNC) (void *ContextParam,
                                                   BOX_CEC_SVS_UI_COMMAND
                                                   key_info);
typedef int (*BOX_CEC_SVS_UC_RELEASED_CBFUNC) (void *ContextParam,
                                               BOX_CEC_SVS_UI_COMMAND key_info);
typedef int (*BOX_CEC_SVS_POWERON_BDP_CBFUNC) (void *ContextParam);
typedef int (*BOX_CEC_SVS_PLAY_MEDIA_CBFUNC) (void *ContextParam);
typedef int (*BOX_CEC_SVS_GET_MENULANG_CBFUNC) (void *ContextParam,
                                                int *lanCode);
typedef int (*BOX_CEC_SVS_SET_MENULANG_CBFUNC) (void *ContextParam,
                                                int lanCode);
typedef int (*BOX_CEC_SVS_SET_CEC_VERSION_CBFUNC) (void *ContextParam,
                                                   int version);
typedef int (*BOX_CEC_SVS_GET_DEVICEDECKSTATUS_CBFUNC) (void *ContextParam,
                                                        int *status);
typedef int (*BOX_CEC_SVS_HANDLE_PLAY_MESSAGE_CBFUNC) (void *ContextParam,
                                                       int mode);
typedef int (*BOX_CEC_SVS_HANDLE_DECK_CONTROL_MESSAGE_CBFUNC) (void
                                                               *ContextParam,
                                                               int mode);
typedef int (*BOX_CEC_SVS_CURRENT_ACTIVESWITCH_CBFUNC) (void *ContextParam,
                                                        int *portID);
typedef int (*BOX_CEC_SVS_SELECT_HDMIPORT_CBFUNC) (void *ContextParam,
                                                   int portID);
typedef int (*BOX_CEC_SVS_STOP_BEING_ACTIVESRC_CBFUNC) (void *ContextParam);
typedef int (*BOX_CEC_SVS_GET_BDPVENDORID_CBFUNC) (void *ContextParam, int *ID);
typedef int (*BOX_CEC_SVS_RECV_DEVICE_VENDORID_CBFUNC) (void *ContextParam,
                                                        int vendorID, int addr);
typedef int (*BOX_CEC_SVS_RECV_VENDORCMD_CBFUNC) (void *ContextParam,
                                                  int vendorID, char *pcmdBuf,
                                                  int cmdLen);
typedef int (*BOX_CEC_SVS_RECV_VENDORCMDWITHID_CBFUNC) (void *ContextParam,
                                                        int vendorID,
                                                        char *pcmdBuf,
                                                        int cmdLen);
typedef int (*BOX_CEC_SVS_SUPP_VENDORCMDWITHID_CBFUNC) (void *ContextParam,
                                                        int vendorID,
                                                        char *pcmdBuf,
                                                        int cmdLen);
typedef int (*BOX_CEC_SVS_RECV_VENDOR_BUTTONDOWN_CBFUNC) (void *ContextParam,
                                                          int vendorID,
                                                          char rcKey);
typedef int (*BOX_CEC_SVS_RECV_VENDOR_BUTTONUP_CBFUNC) (void *ContextParam,
                                                        int vendorID);
typedef int (*BOX_CEC_SVS_SM_APPREADY_CBFUNC) (void *ContextParam,
                                               char *smResponse, UINT32 length);
typedef int (*BOX_CEC_SVS_RECV_SYSTEM_AUDIO_MODE_STATUS_CBFUNC) (void
                                                                 *ContextParam,
                                                                 int AudioMode);
typedef int (*BOX_CEC_SVS_RECV_AUDIO_STATUS_CBFUNC) (void *ContextParam,
                                                     int mutestatus,
                                                     int volume);

typedef struct _PlayerImpl {
    //!members

    AMP_DISP Disp;
    BOOL bDeadLoader;           //<! indicate the failure of 'MV_DC_Init'
    INT USBDevIdx[MAX_USB_DEV]; //<! usb device index retrieved from 'MV_SYS_VOLUME_MOUNTED' event.
    INT iCurrKey;               //<!indicate current key value
    BOX_CEC_SVS_UI_COMMAND Currkey_info;
    INT iDeckStaReq;            //<!deck status request mode
    INT iNaviState;             //<!indicate current Navi state
    INT iPreviousRes;
    INT CurrentFrameRate;
    INT CurrentVideoFormat;     //<!indicates source video resolution, used for 3D
    INT TV3DCapable;            //<!used for 3D
#if defined(FASTBOOT_PROFILE)
    BOOL bFastboot_standby;
    BOOL bFastboot;
#endif
    HANDLE hListener;
} PlayerImpl;

typedef struct _tempCECConfig {
    INT32 iDeviceType;          /* device type get from MV_VPP_EVENT_CEC_LOG_ADDR_STS msg. */
    INT32 iLogicAddr;           /* logic address get from MV_VPP_EVENT_CEC_LOG_ADDR_STS msg. */
    UINT32 uiPhyAddr;           /* Physic address read from EDID. */
    INT32 bActiveSource;
    INT32 bValid;
} tempCECConfig;

///////////////////////////////////////////////////////////////////////////////
//! \brief  structure for PlayerConfig.
//! hold the player configuration information.
//! only used with in BDP_cec.c
///////////////////////////////////////////////////////////////////////////////

typedef struct __attribute__ ((packed)) {
    /* sizeof PlayerConfig */
    UINT32 Size;
    /* system configuration */
    struct _SysConfig {
        /* CEC setting 0 for diable, 1 for enable. */
        UINT32 ucCecEnabled;
    } SysConfig;

    /* CEC configuration: Set by CEC Message callback function before using it. */
    struct _CECConfig {
        INT32 iPowerMode;       /* 0: Stand-by, 1: Power-on */
        INT32 iDeviceType;      /* device type get from MV_VPP_EVENT_CEC_LOG_ADDR_STS msg. */
        INT32 iLogicAddr;       /* logic address get from MV_VPP_EVENT_CEC_LOG_ADDR_STS msg. */
        UINT32 uiPhyAddr;       /* Physic address read from EDID. */
        INT32 bActiveSource;
        INT32 bValid;
        INT32 noOfPort;         // 2 for buddy box
        BOX_CEC_SVS_DEV_INFO devTable[14];  // dev info table used for dev info and vendor ID
        BOX_CEC_SVS_PORT_INFO portTable[100];   // port table used for port mapping info
    } CECConfig;

    struct _SMData {
        UINT8 length;
        INT8 bActive;
    } SMData;

    struct _CallBackFunc {
        BOX_CEC_SVS_ADDRESS_ALLOC_STATUS_CBFUNC addressallocstatus_callbackfn;
        void *addressallocstatus_contextparam;
        BOX_CEC_SVS_RECV_FEATURE_ABORT_CBFUNC featureabort_callbackfn;
        void *featureabort_contextparam;
        BOX_CEC_SVS_STANDBY_CBFUNC standby_callbackfn;
        void *standby_contextparam;
        BOX_CEC_SVS_GET_STANDBY_STATUS_CBFUNC getstandbystatus_callbackfn;
        void *getstandbystatus_contextparam;
        BOX_CEC_SVS_GET_OSD_STRING_CBFUNC getosdstr_callbackfn;
        void *getosdstr_contextparam;
        BOX_CEC_SVS_SET_OSD_STRING_CBFUNC setosdstr_callbackfn;
        void *setosdstr_contextparam;
        BOX_CEC_SVS_REQUEST_MENU_STATUS_CBFUNC reqmenusts_callbackfn;
        void *reqmenusts_contextparam;
        BOX_CEC_SVS_MENU_STATUS_REQUEST_CBFUNC menustsreq_callbackfn;
        void *menustsreq_contextparam;
        BOX_CEC_SVS_UC_PRESSED_CBFUNC ucpressed_callbackfn;
        void *ucpressed_contextparam;
        BOX_CEC_SVS_LONG_UC_PRESSED_CBFUNC longucpressed_callbackfn;
        void *longucpressed_contextparam;
        BOX_CEC_SVS_UC_RELEASED_CBFUNC ucreleased_callbackfn;
        void *ucreleased_contextparam;
        BOX_CEC_SVS_POWERON_BDP_CBFUNC poweronbdp_callbackfn;
        void *poweronbdp_contextparam;
        BOX_CEC_SVS_PLAY_MEDIA_CBFUNC playmedia_callbackfn;
        void *playmedia_contextparam;
        BOX_CEC_SVS_GET_MENULANG_CBFUNC getmenulang_callbackfn;
        void *getmenulang_contextparam;
        BOX_CEC_SVS_SET_MENULANG_CBFUNC setmenulang_callbackfn;
        void *setmenulang_contextparam;
        BOX_CEC_SVS_SET_CEC_VERSION_CBFUNC setcecversion_callbackfn;
        void *setcecversion_contextparam;
        BOX_CEC_SVS_GET_DEVICEDECKSTATUS_CBFUNC getdevdecksts_callbackfn;
        void *getdevdecksts_contextparam;
        BOX_CEC_SVS_HANDLE_PLAY_MESSAGE_CBFUNC handleplaymsg_callbackfn;
        void *handleplaymsg_contextparam;
         BOX_CEC_SVS_HANDLE_DECK_CONTROL_MESSAGE_CBFUNC
            handledeckctrlmsg_callbackfn;
        void *handledeckctrlmsg_contextparam;
        BOX_CEC_SVS_CURRENT_ACTIVESWITCH_CBFUNC currentactiveswitch_callbackfn;
        void *currentactiveswitch_contextparam;
        BOX_CEC_SVS_SELECT_HDMIPORT_CBFUNC selecthdmiport_callbackfn;
        void *selecthdmiport_contextparam;
        BOX_CEC_SVS_STOP_BEING_ACTIVESRC_CBFUNC stopbeingactsrc_callbackfn;
        void *stopbeingactsrc_contextparam;
        BOX_CEC_SVS_GET_BDPVENDORID_CBFUNC getbdpvendorid_callbackfn;
        void *getbdpvendorid_contextparam;
        BOX_CEC_SVS_RECV_DEVICE_VENDORID_CBFUNC recvdevicevendorid_callbackfn;
        void *recvdevicevendorid_contextparam;
        BOX_CEC_SVS_RECV_VENDORCMD_CBFUNC recvvendorcmd_callbackfn;
        void *recvvendorcmd_contextparam;
        BOX_CEC_SVS_RECV_VENDORCMDWITHID_CBFUNC recvvendorcmdwithid_callbackfn;
        void *recvvendorcmdwithid_contextparam;
         BOX_CEC_SVS_SUPP_VENDORCMDWITHID_CBFUNC
            supportedvendorcmdwithid_callbackfn;
        void *supportedvendorcmdwithid_contextparam;
        BOX_CEC_SVS_RECV_VENDOR_BUTTONDOWN_CBFUNC recvvendordown_callbackfn;
        void *recvvendordown_contextparam;
        BOX_CEC_SVS_RECV_VENDOR_BUTTONUP_CBFUNC recvvendorup_callbackfn;
        void *recvvendorup_contextparam;
         BOX_CEC_SVS_RECV_SYSTEM_AUDIO_MODE_STATUS_CBFUNC
            systemaudiomode_callbackfn;
        void *systemaudiomode_contextparam;
        BOX_CEC_SVS_RECV_AUDIO_STATUS_CBFUNC audiostatus_callbackfn;
        void *audiostatus_contextparam;
        BOX_CEC_SVS_SM_APPREADY_CBFUNC SMAppReady_callbackfn;
        void *SMAppReady_contextparam;
    } CallBackFunc;

} BD_PlayerConfig;

// API function Declarations
//****************************************************************/
//Function:BOX_CEC_SVS_Init
//
//Description: This function is used to initializing the CEC Service
//
//Input Params
//    factory: Handle returned when Display service is initialized after system startup.
//
//    This function should be the First CEC Service function that should be called
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_Init(AMP_FACTORY factory);

//****************************************************************/
//Function:BOX_CEC_SVS_Term
//
//Description: This function is used to terminate the CEC Service
//
//Input Params
//    void: No input params required
//
//    This function should be the Last CEC Service function that should be called
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_Term();

//****************************************************************/
//Function:BOX_CEC_SVS_API_ENABLE
//
//Description: This function will enable/disable the BDP CEC stack.
//             CEC must be enabled to send or recieve CEC messages
//
//Input Params
//    bEnable: TRUE (Enable)/FALSE(Disable)
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_ENABLE(BOOL bEnable);

//****************************************************************/
//Function:BOX_CEC_SVS_API_SetPowerMode
//
//Description: This function is used to set the CEC Power Mode
//
//Input Params
//    flag_on: 1: power-on mode :: 0:stand-by mode
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_SetPowerMode(BOOL flag_on);

//****************************************************************/
//Function:BOX_CEC_SVS_API_GetDeviceLogicalAddressByPort
//
//Description: This function is used to Gets the Logical Address of the PortID
//
//Input Params
//  portID: The id of target port which is previously mapped through BOX_CEC_SVS_API_MapPortID
//
//Output Params
//  logAddr: The pointer of target logical address storage, if target logical address
//              of the port with specified PortID not found, -1 assigned to the this storage
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/

int BOX_CEC_SVS_API_GetDeviceLogicalAddressByPort(int portID, int *logAddr);

//****************************************************************/
//Function:BOX_CEC_SVS_API_MapPortID
//
//Description: This function will regsiter portID correcposnding to connected device phys addr
//           (note): Always register portInfo[0] as BDP itself address with 0 port number
//
//Input Params
//   *PortInfo: portinfo table containing information of port and addr
//     num     : Number of ports
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_MapPortID(BOX_CEC_SVS_PORT_INFO * PortInfo, int num);

//****************************************************************/
//Function:BOX_CEC_SVS_API_FEATURE_ENABLE
//
//Description: This function configures the optional features that needs to be
//              enabled/disabled for CEC.
//
//Input Params
//    flag        : If flag is set to 1 Features in featureID are enabled,
//                   if flag is zero Features in featureID are disabled.
//    featureID: Config setting to indicate features to be- enabled/disabled
//
//
//  This method should be call before cec enable.
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_FEATURE_ENABLE(int flag, int featureID);

//****************************************************************/
//Function:BOX_CEC_SVS_API_SendOTP
//
//Description: This function will start One Touch Play
//
//Input Params
//    void: No input params required
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_SendOTP();

//****************************************************************/
//Function:BOX_CEC_SVS_API_SendOTP_TextViewOn
//
//Description: This function will start One Touch Play  <text view on> and  <active source>
//
//Input Params
//    void: No input params required
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_SendOTP_TextViewOn();

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterAddressAllocStatusCallback
//
//Description: Register a addressallocstatus(status) callback function
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterAddressAllocStatusCallback
    (BOX_CEC_SVS_ADDRESS_ALLOC_STATUS_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_SendFeatureAbort
//
//Description: This function will send <feature abort>
//
//Input Params
//    pDev: CEC Device Info
//    opcode : CEC Feature Message OPCODE.
//    reason  : Feature abort reason
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_SendFeatureAbort(BOX_CEC_SVS_DEV_INFO * pDev, int opcode,
                                     int reason);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterReceiveFeatureAbortCallback
//
//Description: Register a featureabort() callback function
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterReceiveFeatureAbortCallback
    (BOX_CEC_SVS_RECV_FEATURE_ABORT_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_SendStandby
//
//Description: This function will send <Standby>
//
//Input Params
//    pDev: CEC Device Info
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_SendStandby(BOX_CEC_SVS_DEV_INFO * pDev);

//****************************************************************/
//Function:BOX_CEC_SVS_API_SetBDPAsInActiveSource
//
//Description: This function will set BDP as inactive source
//
//Input Params
//    void: No input params required
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_SetBDPAsInActiveSource();

//****************************************************************/
//Function:BOX_CEC_SVS_API_SetBDPAsActiveSource
//
//Description: This function will set BDP as active source
//
//Input Params
//    void: No input params required
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_SetBDPAsActiveSource();

//****************************************************************/
//Function:BOX_CEC_SVS_API_GivePowerStatus
//
//Description: This function will send <Give power status>
//
//Input Params
//    pDev: CEC Device Info
//
//
//S_OK if sucessful or error code otherwise.

//
//****************************************************************/
int BOX_CEC_SVS_API_GivePowerStatus(BOX_CEC_SVS_DEV_INFO * pDev);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterStandbyCallback
//
//
//Description: Register a standby() callback function
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterStandbyCallback(BOX_CEC_SVS_STANDBY_CBFUNC cbFunc,
                                            void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterGetStandbyStatusCallback
//
//Description: This function is used to register a callback to get the STANDBY Status.
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterGetStandbyStatusCallback
    (BOX_CEC_SVS_GET_STANDBY_STATUS_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_SendMenuStatus
//
//Description: This function is used to
//
//Description: This function is used to send a <Menu Status>['Activated/Desctivated"] message
//
//Input Params
//    pDev: CEC Device Info
//    status - menu status
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_SendMenuStatus(BOX_CEC_SVS_DEV_INFO * pdev, int status);

//****************************************************************/
//Function:BOX_CEC_SVS_API_SendRCPressed
//
//Description: This function is used to send a <User Control Pressed> message
//
//Input Params
//  pDev - CEC Device Info
//   code - key code
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_SendRCPressed(BOX_CEC_SVS_DEV_INFO * pdev, int code);

//****************************************************************/
//Function:BOX_CEC_SVS_API_SendRCReleased
//
//Description: This function is used to send a <User Control Released> message
//
//Input Params
//  pDev - CEC Device Info
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_SendRCReleased(BOX_CEC_SVS_DEV_INFO * pdev);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterRequestMenuStatusCallback
//
//Description: Register a ReuestMenustatus callback function
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterRequestMenuStatusCallback
    (BOX_CEC_SVS_REQUEST_MENU_STATUS_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterMenuStatusRequestCallback
//
//Description: Register a menustatusrequest callback function
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterMenuStatusRequestCallback
    (BOX_CEC_SVS_MENU_STATUS_REQUEST_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterUCPressedCallback
//
//Description:  Register a UCPressed callback function
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterUCPressedCallback(BOX_CEC_SVS_UC_PRESSED_CBFUNC
                                              cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterUCReleasedCallback
//
//Description: Register a UCReleased callback function
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterUCReleasedCallback(BOX_CEC_SVS_UC_RELEASED_CBFUNC
                                               cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterLongUCPressedCallback
//
//Description: Register a Long UCPressed callback function
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int
BOX_CEC_SVS_API_RegisterLongUCPressedCallback(BOX_CEC_SVS_LONG_UC_PRESSED_CBFUNC
                                              cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterPowerOnBDPCallback
//
//Description: Register a poweronBDP() callback function
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterPowerOnBDPCallback(BOX_CEC_SVS_POWERON_BDP_CBFUNC
                                               cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterPlayMediaCallback
//
//Description:   Register a playMedia() callback function
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterPlayMediaCallback(BOX_CEC_SVS_PLAY_MEDIA_CBFUNC
                                              cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_QueryTVCECVersion
//
//Description: To be used to send a <Get CEC  Version> message to query TV's CEC Version
//
//Input Params
//  pDev - CEC Device Info
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_QueryTVCECVersion(BOX_CEC_SVS_DEV_INFO * pDev);

//****************************************************************/
//Function:BOX_CEC_SVS_API_QueryTVMenuLanguage
//
//Description: To be used to Send a <Get Menu Language> message to
//             query a TV's menu language setting
//
//Input Params
//  pDev - CEC Device Info
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_QueryTVMenuLanguage(BOX_CEC_SVS_DEV_INFO * pDev);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterGetMenuLangCallback
//
//Description: Register a GetBDPMenuLang() callback function which will$
//  be called to get current BDP menu language
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterGetMenuLangCallback(BOX_CEC_SVS_GET_MENULANG_CBFUNC
                                                cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterSetMenuLangCallback
//
//Description: Register a SetBDPMenuLang(int langCode) callback function which$
//  will be called when receiving a
//                 <Set Menu Language> message as a response
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterSetMenuLangCallback(BOX_CEC_SVS_SET_MENULANG_CBFUNC
                                                cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterSetCecVersionCallback
//
//Description: Register a SetCecVersion(int version) callback function$
//  which will be called when receiving a
//                 <Set Cec Version> message as a response
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int
BOX_CEC_SVS_API_RegisterSetCecVersionCallback(BOX_CEC_SVS_SET_CEC_VERSION_CBFUNC
                                              cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterGetDeviceDeckStatusCallback
//
//Description: Registe a GetDeviceDeckStatus(CEC_DEV_INFO *pDev, int *status)$
//  callback function which will be called when receiving a
//                 <Give Deck Status> message from a device
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int
 BOX_CEC_SVS_API_RegisterGetDeviceDeckStatusCallback
    (BOX_CEC_SVS_GET_DEVICEDECKSTATUS_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterHandlePlayMessageCallback
//
//Description: Registe a handlePlayMessage(int mode) callback function which will$
//  be called when receiving a <Play> message from a device
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int
 BOX_CEC_SVS_API_RegisterHandlePlayMessageCallback
    (BOX_CEC_SVS_HANDLE_PLAY_MESSAGE_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterHandleDeckControlMessageCallback
//
//Description: Registe a handleDeckControlMessage(int mode) callback function$
//  which will be called when receiving a
//                <Deck Control> message from a device
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int
 BOX_CEC_SVS_API_RegisterHandleDeckControlMessageCallback
    (BOX_CEC_SVS_HANDLE_DECK_CONTROL_MESSAGE_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RequestActiveSource
//
//Description: This function is used to Broadcast <Request Active source> message
//
//Input Params
//        None
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RequestActiveSource();

//****************************************************************/
//Function:BOX_CEC_SVS_API_SendRoutingChange
//
//Description: This function is used to Send <Routing Change> meesage
//
//Input Params
// oldportID - old port ID
// newportID - new portID
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_SendRoutingChange(int oldportID, int newportID);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterCurrentActiveSwitchCallback
//
//Description: Registe a currentActiveSwitch(int *portID) callback function
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterCurrentActiveSwitchCallback
    (BOX_CEC_SVS_CURRENT_ACTIVESWITCH_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterSelectHDMIPortCallback
//
//Description: Registe a selectHDMIPort() callback function
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterSelectHDMIPortCallback
    (BOX_CEC_SVS_SELECT_HDMIPORT_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterStopBeingActiveSourceCallback
//
//Description: Register a stopbeingactivesource callback function
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterStopBeingActiveSourceCallback
    (BOX_CEC_SVS_STOP_BEING_ACTIVESRC_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_GiveOSDName
//
//Description: Sends a < Give OSD Name> message to a device.
//
//Input Params
//  pDev - CEC Device Info
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_GiveOSDName(BOX_CEC_SVS_DEV_INFO * pDev);

//****************************************************************/
//Function:BOX_CEC_SVS_API_SendOSDName
//
//Description: Sends a < Set OSD NAME> message to a device.
//
//Input Params
//  pDev - CEC Device Info
//
//S_OK if sucessful or error code otherwise.

//
//****************************************************************/
int BOX_CEC_SVS_API_SendOSDName(BOX_CEC_SVS_DEV_INFO * pDev, char *str);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterGetOSDStringCallback
//
//Description: Register a GetOSDString() callback function which will be$
//  called when receiving <Get OSD String> message to get current OSD string.
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int
BOX_CEC_SVS_API_RegisterGetOSDStringCallback(BOX_CEC_SVS_GET_OSD_STRING_CBFUNC
                                             cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterSetOSDStringCallback
//
//Description: Register a SetOSDString() callback function which will be called$
//  when receiving <Set OSD String> message to store OSD Name of given device.
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int
BOX_CEC_SVS_API_RegisterSetOSDStringCallback(BOX_CEC_SVS_SET_OSD_STRING_CBFUNC
                                             cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_GiveDeviceVendorID
//
//Description: Sends a < Give Device Vendor ID> message to a device.
//
//Input Params
//  pDev - CEC Device Info
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_GiveDeviceVendorID(BOX_CEC_SVS_DEV_INFO * pDev);

//****************************************************************/
//Function:BOX_CEC_SVS_API_SendDeviceVendorID
//
//Description: Sends a < Vendor Command> message to a device.
//
//Input Params
//             vendorID - device vendorID
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_SendDeviceVendorID(int vendorID);

//****************************************************************/
//Function:BOX_CEC_SVS_API_SendDeviceVendorCommand
//
//Description: Sends a < Vendor Command> message to a device.
//
//Input Params
//      pDev - device info
//      pcmdBuf - command buffer
//      cmdLen - command length
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_SendDeviceVendorCommand(BOX_CEC_SVS_DEV_INFO * pDev,
                                            char *pcmdBuf, int cmdLen);

//****************************************************************/
//Function:BOX_CEC_SVS_API_SendDeviceVendorCommandID
//
//Description: Sends a < Vendor Command ID> message to a device.
//
//Input Params
//      pDev - device info
//      pcmdBuf - command buffer
//      cmdLen - command length
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_SendDeviceVendorCommandID(BOX_CEC_SVS_DEV_INFO * pDev,
                                              int vendorID, char *pcmdBuf,
                                              int cmdLen);

//****************************************************************/
//Function:BOX_CEC_SVS_API_SendDeviceVendorRemoteButtonUp
//
//Description: Sends a < Vendor Remote Button Up> message.
//
//Input Params
//        pDev - CEC device info
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_SendDeviceVendorRemoteButtonUp(BOX_CEC_SVS_DEV_INFO * pDev);

//****************************************************************/
//Function:BOX_CEC_SVS_API_SendDeviceVendorRemoteButtonDown
//
//Description: Sends a < Vendor Remote Button Down> message.
//
//Input Params
//  pDev - CEC device info
//  rcKeyey - rc key
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_SendDeviceVendorRemoteButtonDown(BOX_CEC_SVS_DEV_INFO *
                                                     pDev, char rcKey);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterGetBDPVendorIDCallback
//
//Description: Register a callback function GetBDPVendorID(int *ID )$
// which will be called to get BDP's vendor ID when receives
// <Give Vendor ID> . As a result, a <Device Vendor ID> message$
// will be broadcasted to other devices.
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterGetBDPVendorIDCallback
    (BOX_CEC_SVS_GET_BDPVENDORID_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterReceiveDeviceVendorIDCallback
//
//Description: Register a callback function RecvDeviceVendorID(int ID, addr ) which$
//   will be called to store device vendor ID when receives
//   <Device Vendor ID> . As a result, a <Give Device Vendor ID> message will be send other devices.
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterReceiveDeviceVendorIDCallback
    (BOX_CEC_SVS_RECV_DEVICE_VENDORID_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterReceiveVendorCommandCallback
//
//Description: Register a ReceiveVendorCommand(int vendorID, char *pcmdBuf, int cmdLen)$
//   callback function which will be called when receiving
//   <Vendor Command> message or a <Vendor Command With ID> from a device with supported vendor ID.
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterReceiveVendorCommandCallback
    (BOX_CEC_SVS_RECV_VENDORCMD_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterReceiveVendorCommandWithIDCallback
//
//Description: Register a ReceiveVendorCommandWithID(int vendorID, char *pcmdBuf, int cmdLen)$
//    callback function which will be called when receiving
//    <Vendor Command With ID> from a device with supported vendor ID.
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterReceiveVendorCommandWithIDCallback
    (BOX_CEC_SVS_RECV_VENDORCMDWITHID_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterSupportVendorCommandWithIDCallback
//
//Description: Register a SupportedVendorCmd(vendorID, cmdbuf, len) callback$
//  function which will be called when we recv <vendor cmd with ID>
//  to check the supported status.
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterSupportVendorCommandWithIDCallback
    (BOX_CEC_SVS_SUPP_VENDORCMDWITHID_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterReceiveVendorRemoteButtonUpCallback
//
//Description: Register a ReceiveVendorRemoteButtonUp(int vendorID, char *pkeyBuf, int keyLen)$
//  callback function which will be called when receiving a
//  <Vendor Remote Button Up> message from a device with supported vendor ID.
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterReceiveVendorRemoteButtonUpCallback
    (BOX_CEC_SVS_RECV_VENDOR_BUTTONUP_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterReceiveVendorRemoteButtonDownCallback
//
//Description: Register a ReceiveVendorRemoteButtonDown
// (int vendorID, char *pkeyBuf, int keyLen) callback function which$
// will be called when receiving a <Vendor Remote Button Down> message$
// from a device with supported vendor
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterReceiveVendorRemoteButtonDownCallback
    (BOX_CEC_SVS_RECV_VENDOR_BUTTONDOWN_CBFUNC cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_GetSMAppReadyResponse
//
//Description:
//
//Input Params:
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_GetSMAppReadyResponse(char *arglist);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterSMAppReadyCallback
//
//Description:
//
//Input Params:
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterSMAppReadyCallback(BOX_CEC_SVS_SM_APPREADY_CBFUNC
                                               cbFunc, void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_GiveSystemAudioModeStatus
//
//Description: Sends a < Give System Audio Mode Status> message to a device.
//
//Input Params
//      pDev - CEC device info
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_GiveSystemAudioModeStatus(BOX_CEC_SVS_DEV_INFO * pDev);

//****************************************************************/
//Function:BOX_CEC_SVS_API_GiveAudioStatus
//
//Description: Sends a < Give Audio Status> message to a device.
//
//Input Params
//      pDev - CEC device info
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
//****************************************************************/
int BOX_CEC_SVS_API_GiveAudioStatus(BOX_CEC_SVS_DEV_INFO * pDev);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterReceiveSystemAudioModeStatusCallback
//
//Description: Register a recvsystemaudiomodestatus(status) which$
//will be called when we recv <System Audio Mode Status>
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
//****************************************************************/
int BOX_CEC_SVS_API_RegisterReceiveSystemAudioModeStatusCallback
    (BOX_CEC_SVS_RECV_SYSTEM_AUDIO_MODE_STATUS_CBFUNC cbFunc,
     void *contextParam);

//****************************************************************/
//Function:BOX_CEC_SVS_API_RegisterReceiveAudioStatusCallback
//
//Description: Register a recvsystemaudiostatus(status) which$
//will be called when we recv <Audio Status>
//
//Input Params
//    cbFunc: Callback Function ptr to be registered.
//    contextParam : A parameter passed unchanged to the callbackfunc
//
//
//S_OK if sucessful or error code otherwise.
//
//****************************************************************/
int BOX_CEC_SVS_API_RegisterReceiveAudioStatusCallback
    (BOX_CEC_SVS_RECV_AUDIO_STATUS_CBFUNC cbFunc, void *contextParam);

#endif
