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
//! \file disp_api.h
//!
//! \brief  Definition of all the APIs and data structures,
//|  for the APIs exported to the applications, they are defined
//|  in amp_dispsrv.idl
//!
//! Purpose:
//!
//!
//! Version, Date and Author :
//!   V 1.00,    December 20 2012,   Jiankang Yu
//!
//!
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef __DISP_API_H__
#define __DISP_API_H__

#include "com_type.h"
#include "amp_server.h"
#include "vpp_api.h"
#include "isl/amp_buf_desc.h"

#if (BERLIN_CHIP_VERSION == BERLIN_BG4_DTV)
#define MAXLVDSPHY 2
#define SSC_CLK_DIVIDER 4
#else
#define MAXLVDSPHY 4
#define SSC_CLK_DIVIDER 1
#endif
#define MAXLVDSLANE 6
#define VPP_CAPTURE_RAW_FRAME_SUPPORT
#undef CONFIG_CAPTURE_YUV422_MODE //Enable this to output frame captures in yuv422
#define EARC_CAPABILITIES_LENGTH 256

#define HDMI_PHYCONFIG_SIZE 32

typedef enum {
    AMP_DISP_EPRIVATE_BASE  = 0x1000,   /**< The module private error value base. */
    AMP_DISP_ENODEV         = 0x1001,   /**< No device. */
    AMP_DISP_EBADCALL       = 0x1002,   /**< channel not connected to DV */
    AMP_DISP_EUNSUPPORT     = 0x1003,   /**< plane/channel not connected in configuration*/
    AMP_DISP_EUNCONFIG      = 0x1004,   /**< VPP not configured */
    AMP_DISP_ECMDQFULL      = 0x1005,   /**< Command Queue is full */
    AMP_DISP_EFRAMEQFULL    = 0x1006,   /**< Frame Queue is full */
    AMP_DISP_EBCMBUFFULL    = 0x1007,   /**< BCM Buffer is full */
    AMP_DISP_EVBIBUFFULL    = 0x1008,   /**< VBI Buffer is full */
    AMP_DISP_ENOSINKCNCTED  = 0x1009,   /**< Not connected to Sink device*/
    AMP_DISP_ENOHDCPENABLED = 0x100A,   /**< HDCP not enabled */
}AMP_DISP_ERROR;

/*Enable the following macro to create built-in frame for various pattern/format
 *Otherwise - only two built-in frame will be created - 420SP and ARGB format only */
//#define AMP_DISP_ENABLE_BUILD_ALL_BUILT_IN_FRAME

typedef enum {
    AMP_DISP_BUILD_IN_FRAME_BLACK_ARGB = AMP_DISP_BUILD_IN_FRAME_TYPE_MAX,
    AMP_DISP_BUILD_IN_FRAME_LOGO,
    AMP_DISP_BUILD_IN_FRAME_TYPE_INTERNAL_MAX,
} AMP_DISP_BUILD_IN_FRAME_TYPE_INTERNAL;

typedef enum __MODULE_POWER_MODE__ {
    MODULE_POWER_MODE_FULL = 0,
    MODULE_POWER_MODE_OPTIMIZED,
} MODULE_POWER_MODE;

typedef struct amp_disp_panel_config_t {
    UINT8     iCfgEnable;

    UINT8     iBacklightCtrlType; //PWM, SPI_chip0, SPI_chip1 ...
    UINT32    iSPIVendorIndex;
    UINT32    iPanelDimmingControlOverSampleH; //only for SPI interface Panel
    UINT32    iPanelDimmingControlOverSampleV; //only for SPI interface Panel
    UINT8     iPanelLEDArrayMode; //only for SPI interface Panel
    UINT8     iPanelLEDArrayMapping[AMP_DISP_LED_BLOCK_NUM_MAX];

    UINT8     iPwmChannel;
    UINT8     iPwmPolarity;

    UINT8     iBacklightPowerGpioType;
    UINT8     iBacklightPowerGpioIndex;
#ifdef VPP_CONFIG_LVTX
    UINT8     iPanelType;   //LVDS/VBO
    UINT8     iPanelMode;   //FORMAT - bpp
    UINT8     iPanelRes;    //SD/HD/FHD/UHD
    UINT8     iPanelLanes;  //VBO specific - Nlanes
    UINT8     iPanelSplit;  //VBO specific - NSplit
    UINT16    iPanelHres;   // User Specific HRes
    UINT16    iPanelVres;   // User Specific VRes
    UINT8     iPanelPathSel;// User Specific CPCB selection
    UINT8     iPanelPackerMode; //lvdstx packer mode
    UINT8     iPanelPackerLane0; // lane 0 setting
    UINT8     iPanelPackerLane1; // lane 1 setting
    UINT8     iPanelPackerLane2; // lane 2 setting
    UINT8     iPanelPackerLane3; // lane 3 setting

    UINT8     ipanelPolSwapEnable;
    UINT8     ipanelPolSwapPhy0;
    UINT8     ipanelPolSwapPhy1;
    UINT8     ipanelPolSwapPhy2;
    UINT8     ipanelPolSwapPhy3;
    UINT8     iEnableCustChannelRmap;
    UINT32 iLVDSLaneRemapIndex[MAXLVDSPHY][MAXLVDSLANE];
#else
    UINT8     iPanelMode;
#endif //VPP_CONFIG_LVTX
    UINT8     iPanelPowerGpioType;
    UINT8     iPanelPowerGpioIndex;
    UINT8     iPanel3DEnGpioType;
    UINT8     iPanel3DEnGpioIndex;

    UINT8     iInverseMode;
    UINT8     iVbyOnePanelMode;
    INT32     iPanel3dMode;
} AMP_DISP_PANEL_CONFIG;

typedef struct BE_HDMITX_PHY_SETTING_T {
    UINT8 AUX_CTRL_1;
    UINT8 SEL_DIVA;
    UINT8 SEL_DIVX;
    UINT8 SICP;
    UINT8 SIKVCO;
    UINT8 SVTX;
    UINT8 CP;
    UINT16 DAMP;
    UINT16 EAMP;
    UINT16 IDRV;
    UINT16 RTERM_CTRL;
}BE_HDMITX_PHY_SETTING;

typedef enum tagHDMITX_FREQ_RANGE
{
    FREQ_UPTO_38 = 0,
    FREQ_39_TO_74P5,
    FREQ_75_TO_148,
    FREQ_149_TO_222,
    FREQ_223_TO_297,
    FREQ_298_TO_594,
    MAX_FREQ_RANGE
}HDMITX_FREQ_RANGE;

typedef VOID (*DISP_HDMI_EVT_NOTIFIER)(VOID *pUserData, INT32 event);

typedef struct disp_hdmi_event_notifier_params_t {
    DISP_HDMI_EVT_NOTIFIER hdmiEventNotifier;
    VOID *pUserData;
}DISP_HDMI_EVENT_NOTIFIER_PARAMS;

typedef struct HDR_CONFIG_T {
    UINT8 Enable;
    UINT8 AutoModeType;
    UINT32 SignalDelay[5];
    UINT32 ModuleSelect;
}HDR_CONFIG;

//Color temperature Coeff/Intc to convert temperature to R,G,B gain
#define R_A_COEFF               0.00000000000000000
#define R_B_COEFF               0.00000000000000000
#define R_Y_INTC                1.00000000000000000
#define G_A_COEFF              -0.00000000962353339
#define G_B_COEFF               0.00015304547600000
#define G_Y_INTC                0.39078277800000000
#define B_A_COEFF              -0.00000001893590410
#define B_B_COEFF               0.00030241221100000
#define B_Y_INTC               -0.19865089500000000

typedef struct DISP_COLORTEMP_COFF_T {
    double aCoeff;
    double bCoeff;
    double yIntercept;
}DISP_COLORTEMP_COFF;

typedef struct DISP_COLORTEMP_T {
    DISP_COLORTEMP_COFF R, G, B;
}DISP_COLORTEMP;

typedef struct _DISP_EARC_CONFIG {
    BOOL      isUpdated;
    UINT8     bootupEarcMode;
    UINT8     extrAudio;
    UINT8     latency;
    UINT8     earcCapabilities[EARC_CAPABILITIES_LENGTH];
}DISP_EARC_CONFIG;

typedef struct DISP_RES_CONFIG_T {
    AMP_DISP_OUT_RES    resID;
    VPP_MIPI_INIT_PARAMS         initparams;
    RESINFO_TIMING_DESCRIPTION   infoparams;
    UINT16    cmdbufsize;
    void *hndmipicmdbuf;
}DISP_RES_CONFIG;

typedef struct amp_disp_config_t {

    AMP_DISP_OUT_RES  OutResPri;/**< Output resolution for HDMI and Component */
    AMP_DISP_OUT_RES  OutResSec;/**< Output resolution for CVBS and S-video */

    UINT32    uiDispOutId;
    INT32     iHdmiColorFmt;
    UINT32    uiHdmiBitDepth;
    INT32     iHdmiPixelRept;

    INT32     iVdacEnable;
    INT32     iLvdsEnable;
    INT32     iHDMIEnable;
    UINT32    iHDMITmdsMuxSel;
    INT32     iHdcpVersionSel;

    UINT32    uiHDMITmdsDAMP;
    UINT32    uiHDMITmdsEAMP;
    UINT32    uiHDMITmdsIDRV;
    UINT32    uiHDMITmdsTXDRVX2;

    INT32     iHdmiSignalSwap;

    BOOL      bSupportedBuildInFrame[AMP_DISP_BUILD_IN_FRAME_TYPE_INTERNAL_MAX];
    BOOL      bCustomPhy;

    AMP_DISP_PANEL_CONFIG iPanelCfg;
    BE_HDMITX_PHY_SETTING HdmiPhySetting[MAX_FREQ_RANGE];
    UINT8 HdmiPhySettingRaw[HDMI_PHYCONFIG_SIZE];
    UINT8 HdmiPhySettingRawSize;
    HDMI_CONFIG HdmiConfig;
    HDR_CONFIG HdrConfig;
    DISP_COLORTEMP ColorTemp;

    DISP_EARC_CONFIG EarcConfig;

    UINT8     noofResID;
    DISP_RES_CONFIG *pResConfig;
    void *hndshmMipi;
} AMP_DISP_CONFIG;

typedef VOID (*DISP_VSCALE_UPDATE_NOTIFIER)(VOID *pData, UINT32 uiHeight);

typedef VOID (*DISP_PLANE_NOTIFIER)(VOID *pData);

typedef struct disp_vscale_update_callback_t {
    DISP_VSCALE_UPDATE_NOTIFIER ScaleUpdateNotifier;
    AMP_DISP_PLANE_ID uiPlaneId;
    VOID *pUserPara;
}DISP_VSCALE_UPDATE_CALLBACK;

typedef struct disp_plane_callback_t {
    DISP_PLANE_NOTIFIER PreNotifier;
    DISP_PLANE_NOTIFIER PostNotifier;
    VOID *pUserPara;
}DISP_PLANE_CALLBACK;

typedef enum {
   HDCP_USER_SEL_VERSION_1_4 = 0 , /*HDCP version 1.4*/
   HDCP_USER_SEL_VERSION_2_2 = 1 , /*HDCP Version 2.2.*/
   HDCP_USER_SEL_DEFAULT = 2       /*Best Hdcp Version Selected by Driver*/
}DISP_HDCP_VERSION;


#define FACTORY_DATA_PATH        "/factory_data"
#define MODEL_INDEX_FILE         "model_index.dat"

/**
*  Display service generic API
*/
typedef HRESULT (*CMDFUNC)(CHAR *pCmd);

void DISP_LoadPanelCfg(AMP_DISP_PANEL_CONFIG* pPanelCfg);

#ifdef __cplusplus
extern "C"
{
#endif
int DISP_LVDS_ParseSetting(CMDFUNC runCmd);
int DISP_LVDS_ParsePowerOn(CMDFUNC runCmd);
int DISP_LVDS_ParsePowerOff(CMDFUNC runCmd);

extern HRESULT DISP_Exit(void);
extern HRESULT DISP_Init(void);
extern HRESULT DISP_Config(AMP_DISP_CONFIG *pDispCfg);

HRESULT DISP_Push_BuildInFrame(AMP_DISP_PLANE_ID uiPlaneId,
                               AMP_DISP_BUILD_IN_FRAME_TYPE FrameType);

HRESULT DISP_Make_BuildInFrame(UINT32 uiWidth, UINT32 uiHeight,
    UINT32 uiShmOffset, AMP_DISP_BUILD_IN_FRAME_TYPE FrameType);

HRESULT DISP_Show_Bootlogo(void);

HRESULT DISP_Suspend(void);
HRESULT DISP_Resume(void);
HRESULT DISP_Freeze(void);
HRESULT DISP_Unfreeze(void);

HRESULT DISP_IsPlaneValid(AMP_DISP_PLANE_ID uiPlaneId, UINT32 *pValid);
HRESULT DISP_SetPlaneMute(AMP_DISP_PLANE_ID uiPlaneId, UINT32 uiMute);
HRESULT DISP_GetPlaneMute(AMP_DISP_PLANE_ID uiPlaneId, UINT32 *pMute);

HRESULT DISP_MuteActiveVideo(AMP_DISP_PLANE_ID uiPlaneId, UINT32 uiMute);

HRESULT DISP_SetVoutMute(UINT32 uiOutPath, UINT32 uiMute);

HRESULT DISP_SetVoutSource(UINT32 uiOutPath, UINT32 uiCPCBId, UINT32 uiInterlaced);
HRESULT DISP_SetPortPower(INT32 iPort, INT32 iEnable);

HRESULT DISP_SetPlaneZOrder(AMP_DISP_TG_ID uiTgId, AMP_DISP_ZORDER *pZorder);
HRESULT DISP_GetPlaneZOrder(AMP_DISP_TG_ID uiTgId, AMP_DISP_ZORDER *pZorder);

HRESULT DISP_GetScale(AMP_DISP_PLANE_ID uiPlaneId, AMP_DISP_WIN *pSrcWin,
                      AMP_DISP_WIN *pDstWin);

HRESULT DISP_SetScale(AMP_DISP_PLANE_ID uiPlaneId, AMP_DISP_WIN *pSrcWin,
                      AMP_DISP_WIN *pDstWin);

HRESULT DISP_LoadCustScalerFilter(UINT8 coeffID, UINT8 hv,
                      UINT8 sclUnit, UINT16* pCoeffs);
HRESULT DISP_LoadSystemScalerFilter(UINT8 coeffID, UINT8 hv,
                      UINT8 sclUnit);

HRESULT DISP_SetScalerNonLinearControl(AMP_DISP_PLANE_ID uiPlaneId,
    UINT32 uiEnable, UINT32 uiCenterFrac, UINT32 uiCenterRatio);

HRESULT DISP_SetScale_RatioCtrl(AMP_DISP_PLANE_ID uiPlaneId,
                                AMP_DISP_WIN_RATIO *pSrcWinRatio,
                                AMP_DISP_WIN *pDstWin);

HRESULT DISP_SetScalerSubPixel(UINT32 uiScalerId, UINT32 uiHv, UINT32 uiSubPixel);

HRESULT DISP_SetPlaneBgColor(AMP_DISP_PLANE_ID PlaneId, INT32 iColor);

HRESULT DISP_GetPlaneBgColor(AMP_DISP_PLANE_ID PlaneId, INT32 *pColor);

HRESULT DISP_WriteRegGroup(AMP_DISP_VPP_REG_GROUP *pRegGroup);

HRESULT DISP_ReadRegGroup(AMP_DISP_VPP_REG_GROUP *pRegGroup);

HRESULT DISP_Get_FPS(double* pFps);

HRESULT DISP_Push_EarlyFrame (AMP_DISP_PLANE_ID uiPlaneId, AMP_BD_HANDLE hBD);

HRESULT DISP_Get_LastISRTime ( unsigned int *msTime );

// Add handle when push / pop frame to identify BD from which handle
// Actually we have three cases now:
// - external directly call
//      Use DISP's self handle
// - VOUT call it
//      Use VOUT's instance
// - DISP internal buildin frame
//      Use NULL as it doesn't need pop back
HRESULT DISP_VID_PopFrame(AMP_DISP_PLANE_ID uiPlaneId, VOID *handle, AMP_BD_HANDLE *pBD);
HRESULT DISP_VID_PushFrame(AMP_DISP_PLANE_ID uiPlaneId, VOID *handle, AMP_BD_ST *pBD);

HRESULT DISP_AttachPlane(AMP_DISP_PLANE_ID uiPlaneId,
                         DISP_PLANE_CALLBACK *pPlaneCallback,
                         AMP_DISP_PLANE_MODE uiPlaneMode);

HRESULT DISP_WatermarkCTL(UINT32 WatermarkPlaneID, UINT32 subcmd, UINT32 pAddrHandle);


HRESULT DISP_DetachPlane(AMP_DISP_PLANE_ID uiPlaneId, DISP_PLANE_CALLBACK *pPlaneCallback);

HRESULT DISP_OUT_SetFlipMode(UINT32 uiMode);

HRESULT DISP_OUT_SetSource(AMP_DISP_OUT_ID OutPath, AMP_DISP_TG_ID TgId, AMP_DISP_OUT_MODE OutMode);
/**
* Display service output APIs
*/
HRESULT DISP_OUT_SetMute(AMP_DISP_OUT_ID OutPath, UINT32 uiMute);

HRESULT DISP_OUT_SetMute_Ext(AMP_DISP_OUT_ID OutPath, BOOL uiMute, BOOL bVideo, BOOL bAudio);

HRESULT DISP_OUT_SetResolution(AMP_DISP_TG_ID uiTgId,
    AMP_DISP_OUT_RES uiResId, UINT32 uiBitDepth);

HRESULT DISP_OUT_GetResolution(AMP_DISP_TG_ID uiTgId, AMP_DISP_OUT_RES *pResId);
HRESULT DISP_OUT_GetResolutionDescription(AMP_DISP_OUT_RES ResId,
                                    AMP_DISP_OUT_RESOLUTION_DESCRIPTION *pResDesc);

HRESULT DISP_OUT_SetVidFmt(INT32 iHdmiColorFmt,
                           INT32 iHdmiBitDepth,
                           INT32 iHdmiPixelRept);
HRESULT DISP_OUT_HDMI_GetVidFmt(AMP_DISP_OUT_CLR_FMT  *pHdmiColorFmt,
                                AMP_DISP_OUT_BIT_DEP *pHdmiBitDepth,
                                INT32 *pHdmiPixelRept);

HRESULT DISP_OUT_GetClkGateSignature(UINT32 *puiSignatureCount, AMP_DISP_CLK_GATE_SIGNATURE *pSignature,
                                     UINT32 uiOption);
HRESULT DISP_OUT_SetClkGateSignature(AMP_DISP_CLK_GATE_SIGNATURE *pSignatureValue,
                                     AMP_DISP_CLK_GATE_SIGNATURE *pSignatureMask, UINT32 uiOption);


HRESULT DISP_OUT_SetTVType(AMP_DISP_TG_ID uiTgId, UINT32 uiTVType);
HRESULT DISP_OUT_GetTVType(AMP_DISP_TG_ID uiTgId, UINT32 *pTVType);

HRESULT DISP_OUT_HDMI_SetVideoInfo(INT32 iAspRatio, INT32 iScanInfo);
HRESULT DISP_OUT_HDMI_GetSinkCaps(AMP_DISP_HDMI_SINK_CAPS *pCaps);

HRESULT DISP_OUT_HDMI_GetRawEdid(AMP_DISP_HDMI_RAW_EDID *pEdid);
HRESULT DISP_OUT_HDMI_SetMode(UINT32 uiMode);

HRESULT DISP_OUT_HDMI_SetSRM(AMP_DISP_HDCP_SRM_DATA *pSrmData, AMP_DISP_HDCP_SRM_INFO *pSrmInfo);

HRESULT DISP_OUT_HDMI_GetSRMVersion(UINT16 *pHdcp22SrmVersion, UINT16 *pHdcp14SrmVersion);

HRESULT DISP_OUT_HDMI_DumpRegisters(AMP_DISP_HDMI_REG_DUMP *hdmiRegDump);
HRESULT DISP_OUT_HDMI_SendAuxPkt(AMP_DISP_HDMI_AUX_PKT_ID uiPktType, AMP_DISP_HDMI_PKT *pAuxPkt,
                                 BOOL RepeatPkt);

HRESULT DISP_OUT_HDMI_StopAuxPkt(AMP_DISP_HDMI_AUX_PKT_ID uiPktType);

HRESULT DISP_OUT_HDMI_GetAuxPkt(AMP_DISP_HDMI_AUX_PKT_ID uiPktType, AMP_DISP_HDMI_PKT_RAW *pAuxPkt);

HRESULT DISP_OUT_HDMI_EnablexvYCC(UINT32 uiEnable);

HRESULT DISP_OUT_HDMI_SetxvYCCMode(INT32 iMode);

HRESULT DISP_OUT_HDMI_EnableHDR(UINT32 uiEnable);

HRESULT DISP_OUT_HDMI_EnableEDR(UINT32 uiEnable);

HRESULT DISP_OUT_HDMI_RegisterEventNotifier (DISP_HDMI_EVENT_NOTIFIER_PARAMS *pHdmiEventNotParams, INT32 *pHandle);

HRESULT DISP_OUT_HDMI_UnregisterEventNotifier (INT32 handle);

HRESULT DISP_OUT_HDMI_SetVidInfo(INT32 iAspRatio, INT32 iScanInfo);

HRESULT DISP_OUT_HDMI_SetVidFmt(INT32 iHdmiColorFmt,
                                UINT32 uiHdmiBitDepth,
                                INT32 iHdmiPixelRept);
/**
 * Display service output LVDS APIs
 */
/*******************************************************************************
\fn HRESULT DISP_OUT_LVDS_SetPower(UINT8 uiPowerOnOff);
\brief Power on/off LVDS PHY
\param uiPowerOnOff,  1: power on, 0: power off
\param uiPanelType,   1: Generic, 0: CMO
\return HRESULT_VALUE   0 for success, non 0 for failure.
*******************************************************************************/
HRESULT DISP_OUT_LVDS_SetPower(UINT8 uiPowerOnOff, UINT8 uiPanelType);

/*******************************************************************************
\fn HRESULT DISP_OUT_LVDS_Config(void);
\brief Parse the LVDS configuration file to set LVDS user setting.
\return HRESULT_VALUE   0 for success, non 0 for failure.
*******************************************************************************/
HRESULT DISP_OUT_LVDS_Config (void);

/*******************************************************************************
\fn HRESULT DISP_OUT_LVDS_SetDefaultMapping(UINT8 uiIndex);
\brief Set LVDS default channel mapping table
\param uiIndex - Lvds mapping table index in LVDS channel table array
\return HRESULT_VALUE    0 for success, non 0 for failure.
*******************************************************************************/
HRESULT DISP_OUT_LVDS_SetDefaultMapping(UINT8 uiIndex);

/*******************************************************************************
\fn HRESULT DISP_OUT_LVDS_SetMapping(UINT32 uiMapTab);
\brief Set LVDS channel mapping with user defined data table
\param pMapTab - pointer to Lvds mapping table in LVDS channel table array
\return HRESULT_VALUE   0 for success, non 0 for failure.
*******************************************************************************/
HRESULT DISP_OUT_LVDS_SetMapping(UINT8 *pMapTab);

/*******************************************************************************
\fn HRESULT DISP_OUT_LVDS_LoadIcscCustCoeffs(UINT8 uiDeGammaGamma, UINT32 *pCoeffsTab);
\brief Set LVDS's ICSC coeffs mode
\param uiDeGammaGamma - 1: LVDS_DEGAMMA, 2: LVDS_GAMMA
\param pCoeffsTab - refer VppVopCscCoeff's description
\return HRESULT_VALUE   0 for success, non 0 for failure.
*******************************************************************************/
HRESULT DISP_OUT_LVDS_LoadIcscCustCoeffs(UINT8 uiDeGammaGamma, AMP_DISP_LVDS_ICSC_COEFFS* pCoeffsTab);

/*******************************************************************************
\fn HRESULT DISP_OUT_LVDS_SetIcsc(UINT8 uiDeGammaGamma, UINT8 uiIcscMode);
\brief Set LVDS's ICSC coeffs mode
\param uiDeGammaGamma - 1: LVDS_DEGAMMA, 2: LVDS_GAMMA
\param uiIcscMode - 0: bypass mode,  others: refer VppVopCscCoeff's description
\return HRESULT_VALUE   0 for success, non 0 for failure.
*******************************************************************************/
HRESULT DISP_OUT_LVDS_SetIcsc(UINT8 uiDeGammaGamma, UINT8 uiIcscMode);

/*******************************************************************************
\fn HRESULT DISP_OUT_LVDS_GetIcsc(UINT8 uiDeGammaGamma, UINT8* pIcscMode);
\brief Get LVDS's ICSC coeffs mode
\param uiDeGammaGamma - 1: LVDS_DEGAMMA, 2: LVDS_GAMMA
\param pIcscMode - pointer of Icsc mode
\return HRESULT_VALUE   0 for success, non 0 for failure.
*******************************************************************************/
HRESULT DISP_OUT_LVDS_GetIcsc(UINT8 uiDeGammaGamma, UINT8* pIcscMode);

/*******************************************************************************
\fn HRESULT DISP_OUT_LVDS_LoadGammaLUT(UINT8 uiDeGammaGamma,
                                   UINT8 uiOptBGR,
                                   UINT16 *pLUT);
\brief Load LVDS Gamma LUT with user defined arrary
\param uiDeGammaGamma - 1: LVDS_DEGAMMA, 2: LVDS_GAMMA
\param uiOptBGR - bit0, 0: loading into B_LUT
                        1: unloading into B_LUT
                  bit1, 0: loading into G_LUT
                        1: unloading into G_LUT
                  bit2, 0: loading into R_LUT
                        1: unloading into R_LUT
\param pLUT - pointer to LUT table
\return HRESULT_VALUE   0 for success, non 0 for failure.
*******************************************************************************/
HRESULT DISP_OUT_LVDS_LoadGammaLUT(UINT8 uiDeGammaGamma,
                                   UINT8 uiOptBGR,
                                   UINT16 *pLUT);

/*******************************************************************************
\fn HRESULT DISP_OUT_LVDS_WriteGammaLUT(UINT8 uiDeGammaGamma,
                                    UINT8 uiGammaBitDepth);
\brief Write gamma data in the memory into LVDS_GAMMA_BCMBUF
\param uiDeGammaGamma - 1: LVDS_DEGAMMA, 2: LVDS_GAMMA
\param uiGammaBitDepth - only have 3 options: 8, 10 ,12
\return HRESULT_VALUE   0 for success, non 0 for failure.
*******************************************************************************/
HRESULT DISP_OUT_LVDS_WriteGammaLUT(UINT8 uiDeGammaGamma,UINT8 uiGammaBitDepth);
HRESULT DISP_OUT_LVDS_ReadGammaLUT(UINT8 uiDeGammaGamma, UINT8 optBGR, UINT16 *pLut);
HRESULT DISP_OUT_LVDS_BypassDeGammaGamma(UINT8 uiDeGammaGamma, UINT8 uiBypass);
HRESULT DISP_OUT_LVDS_SetWBGain(UINT8 uiOptBGR, UINT32 uiGain);
HRESULT DISP_OUT_LVDS_GetWBGain(UINT8 uiOptBGR, UINT32* pGain);
HRESULT DISP_OUT_LVDS_SetWBOffset(UINT8 uiOptBGR, INT32 iOffset);
HRESULT DISP_OUT_LVDS_GetWBOffset(UINT8 uiOptBGR, INT32* pOffset);
HRESULT DISP_OUT_LVDS_SetSSC(AMP_DISP_LVDS_SSC_CONFIG *pLvdsSscCfg);

/*******************************************************************************
\fn HRESULT DISP_OUT_LVDS_OpenPanel (void);
\brief Do the whole panel power on sequence
\param N/A
\return HRESULT_VALUE   0 for success, non 0 for failure.
*******************************************************************************/
HRESULT DISP_OUT_LVDS_OpenPanel (void);

/*******************************************************************************
\fn HRESULT DISP_OUT_LVDS_ClosePanel (void);
\brief Do the whole panel power off sequence
\param N/A
\return HRESULT_VALUE   0 for success, non 0 for failure.
*******************************************************************************/
HRESULT DISP_OUT_LVDS_ClosePanel (void);

/*******************************************************************************
\fn HRESULT DISP_OUT_LVDS_SetBacklight(UINT8 uiPwmBL)
\brief Configure Backlight
\param uiPwmBL - backlight value, used to calculate PWM brightness
\return HRESULT_VALUE   0 for success, non 0 for failure.
*******************************************************************************/
HRESULT DISP_OUT_LVDS_SetBacklight(UINT8 uiPwmBL);

/*******************************************************************************
\fn HRESULT DISP_OUT_LVDS_SetPwm(UINT8 uiPwmCh,
                             UINT8 uiPwmBL,
                             UINT16 uiTermCount)
\brief Configure PWM
\param uiPwmCh -  PWM channel
\param uiPwmBL - backlight value, used to calculate PWM brightness
\param uiTermCount - terminal count, used to calculate PWM brightness
\return HRESULT_VALUE   0 for success, non 0 for failure.
*******************************************************************************/
HRESULT DISP_OUT_LVDS_SetPwm(UINT8 uiEnable,
                             UINT8 uiPwmCh,
                             UINT8 uiPwmBL,
                             UINT32 uiTermCount);

/*******************************************************************************
\fn HRESULT DISP_OUT_LVDS_SetLed(UINT8 uiPort, UINT8 uiOnOff);
\brief Set LED on/off
\param uiPort - GPIO IO number which connect to panel to control the panel power
\param uiOnOff - 0: off,  1: on
\return HRESULT_VALUE   0 for success, non 0 for failure.
*******************************************************************************/
HRESULT DISP_OUT_LVDS_SetLed(UINT8 uiPort, UINT8 uiOnOff);

/*******************************************************************************
\fn HRESULT DISP_OUT_LVDS_SetGpio(UINT8 uiPort,
                                    UINT8 uiOnOff);
\brief Set GPIO on/off
\param uiMode - gpio control mode, 0: SM GPIO, 1, Non SM GPIO
\param uiPort - GPIO IO number which connect to panel to control the panel power
\param uiOnOff - 0: off,  1: on
\return HRESULT_VALUE   0 for success, non 0 for failure.
*******************************************************************************/
HRESULT DISP_OUT_LVDS_SetGpio(UINT8 uiMode,
                                    UINT8 uiPort,
                                    UINT8 uiOnOff);

HRESULT DISP_OUT_LVDS_InitMemcCmd(UINT8 uiFrc);
HRESULT DISP_OUT_LVDS_SetMemcMode(UINT8 uiFrc, UINT8 uiCtrlMode, UINT8 uiMemcMode);
HRESULT DISP_OUT_LVDS_GetMemcMode(UINT8 uiFrc, UINT8 uiCtrlMode, UINT8 *uiMemcMode);
HRESULT DISP_OUT_LVDS_Set3dMode(UINT8 uiFrc, UINT8 uiCtrlMode, UINT8 ui3dMode);

HRESULT DISP_OUT_LVDS_GetSCLCoeffModes(INT32 iUnitNum, INT32* pHSCLCoeffMode,
                                           INT32* pVSCLCoeffMode);

HRESULT DISP_LVDS_GetCurrentModelId(const CHAR *pModelIdFile, UINT32 *pModelId);
HRESULT DISP_OUT_VBO_SetLaneMapping(UINT8 *pLaneMap);
HRESULT DISP_OUT_VBO_SetScramlerEn(BOOL enable, BOOL bypassMode);
HRESULT DISP_OUT_VBO_SetEncoderEn(BOOL enable, BOOL bypassMode);
HRESULT DISP_OUT_VBO_SetEmphasis( BOOL enable, UINT8 tx_swing, UINT8 emph_swing);
HRESULT DISP_OUT_VBO_GetRecieverStatus(UINT8 *pLockN, UINT8 *pPwrdN);
HRESULT DISP_OUT_VBO_SetScramblerEn(BOOL enable, BOOL bypassMode);

/**
* Display service output HDCP APIs
*/
HRESULT DISP_OUT_HDCP_GetState(INT32 *pState);
HRESULT DISP_OUT_HDCP_GetKsv(AMP_DISP_HDCP_KSV_DATA *pKsvData);
HRESULT DISP_OUT_HDCP_SetRepeaterMode(UINT32 uiMode);
HRESULT DISP_OUT_HDCP_LoadKeys(UINT8 *pHdcpKeys);
HRESULT DISP_OUT_HDCP_Enable(UINT8 uiEnable);
HRESULT DISP_OUT_HDCP_GetAKsv(AMP_DISP_HDCP_AKSV_DATA *pAKsvData);
HRESULT DISP_OUT_HDCP_GetVersion(AMP_DISP_HDMI_HDCP_VER *pHDCPVersion);
HRESULT DISP_OUT_HDMI_GetSinkHdcpCapability(AMP_DISP_HDMI_HDCP_VER *pHDCPVersion);

/**
* HDMI HPD status
*/
HRESULT DISP_OUT_HPD_GetStatus(UINT32 *pHpdStatus);
HRESULT DISP_OUT_HDR10_GetPlaybackStatus(BOOL *pIsHdrPlaying);

/**
* Display service output CEC APIs
*/
HRESULT DISP_OUT_CEC_Enable(INT32 iEnable);
HRESULT DISP_OUT_CEC_SetPhysicalAddr(INT32 iPhyAddr);
HRESULT DISP_OUT_CEC_AllocLogicalAddr (BOOL Alloc, UINT8 uiDevType, INT32 iIniPollAddr);
HRESULT DISP_OUT_CEC_SendMessage (AMP_DISP_CEC_MSG *pCecMsg);
HRESULT DISP_OUT_CEC_GetSupportedFeature(INT32 *pFeature);
HRESULT DISP_OUT_CEC_SetSupportedFeature(INT32 iFeature);

/**
* Display service output Analog APIs
*/
HRESULT DISP_OUT_ANALOG_SetMacrovision(UINT32 uiOutPath, UINT32 uiMode);
HRESULT DISP_OUT_ANALOG_SetWSSData(UINT32 uiOutPath, AMP_DISP_WSS_DATA *pWssData);
HRESULT DISP_OUT_ANALOG_SetCGMSData(UINT32 uiOutPath, AMP_DISP_CGMS_DATA *pCgmsData);
HRESULT DISP_OUT_ANALOG_SetCCData(UINT16 *pData, UINT32 uiLength);

/**
* Display service output 3D APIs
*/
HRESULT DISP_OUT_3D_SetSSOffsetMode(AMP_DISP_PLANE_ID uiPlaneId, UINT32 uiOffsetmode);
HRESULT DISP_OUT_3D_SetSSOffsetSeqIDRef(AMP_DISP_PLANE_ID uiPlaneId, UINT32 uiSeqId);
HRESULT DISP_OUT_3D_SetSSDefaultOffset(AMP_DISP_PLANE_ID uiPlaneId, UINT32 uiOffset);
HRESULT DISP_OUT_3D_SetSSType(UINT32 uiChannelMask, UINT32 uiPresenttypes);
HRESULT DISP_OUT_3D_SetHDMIStructure(AMP_DISP_3D_Structure StructureID, UINT32 uiExtData);
HRESULT DISP_OUT_3D_SetConvertMode(UINT32 uiMode);
HRESULT DISP_OUT_3D_SetInvertMode(BOOL Mode);

/**
* Display GFX APIs
*/
HRESULT DISP_GFX_SetCsc(AMP_DISP_PLANE_ID uiPlaneId, UINT32 uiCscMode);

/**
* Display service PQ APIs
*/
HRESULT DISP_PQ_SetProperty(UINT32 uiParaID, UINT32 uiParaVal);
HRESULT DISP_PQ_GetProperties(UINT32 uiParaID, UINT32 uiCount, UINT32 *pParaVal);
HRESULT DISP_PQ_LoadProperty(UINT32 uiParaID, UINT32 uiParaVal);
HRESULT DISP_PQ_CommitProperties(void);


/**
*Dolby related API's
*/
#ifdef CONFIG_AMP_DOLBY_DUMP
HRESULT DISP_OUT_DV_EnableDolbyDump(UINT8 uiEnable);
HRESULT DISP_OUT_DV_InitFrameDump(UINT32 Path, UINT32 Enable, AMP_LIB_DV_FRAME_DUMP_CONFIG *pFrameDumpConfig);
HRESULT DISP_OUT_DV_GetFrameDumpStatus(AMP_LIB_DV_FRAME_DUMP_STATUS *pFrameDumpStatus);
#endif //CONFIG_AMP_DOLBY_DUMP
HRESULT DISP_OUT_DV_LoadVSDB(AMP_DISP_DV_VSDB *vsdb);
HRESULT DISP_OUT_DV_SetPriorityMode(UINT32 mode);
HRESULT DISP_OUT_DV_EnableLowLatency(UINT32 enable);
HRESULT DISP_OUT_DV_SetDovi2HdrMap(UINT32 mode);
HRESULT DISP_OUT_DV_SetHdrFeature(UINT32 feature);
HRESULT DISP_OUT_DV_SetGfxInfo(UINT32 type, UINT32 gfx_min, UINT32 gfx_max);
HRESULT DISP_OUT_SetHdmiOutputMode(UINT32 mode);
HRESULT DISP_OUT_SetColorTransformGain(float Rgain, float Ggain, float Bgain);
HRESULT DISP_OUT_SetColorTemperature(UINT32 temp);
/**
* EE
*/
HRESULT DISP_OUT_SetEEMode(UINT32 mode, AMP_DISP_VPP_CMU_EE_CTRL_PARAM *pCmuEeCtrlParam);

HRESULT DISP_OUT_SetScalingPreference(UINT32 uiScalingPrefer);
/**
* Gamma
*/
HRESULT DISP_OUT_SetGammaMode(VPP_VOP_TYPE vopID, ENUM_GAMMA_MODE GmMode, AMP_BD_HANDLE pBD);
/**
* Rotation
*/
HRESULT DISP_OUT_GetRotationAngle(UINT32 uiPlaneId, AMP_DISP_ROTATION_MODE *puiRotationAngle);
HRESULT DISP_OUT_SetRotationAngle(UINT32 uiPlaneId, AMP_DISP_ROTATION_MODE uiRotationAngle);

/**
* GFX API
*/
HRESULT DISP_GFX_WaitVSync(AMP_DISP_PLANE_ID uiPlaneId);
HRESULT DISP_GFX_WaitBDFree(AMP_DISP_PLANE_ID uiPlaneId, AMP_BD_ST *phBD);
HRESULT DISP_GFX_PopBD(AMP_DISP_PLANE_ID uiPlaneId, AMP_BD_ST *phBD);
HRESULT DISP_GFX_PushBD(AMP_DISP_PLANE_ID uiPlaneId, AMP_BD_ST *hBD);

/**
* DDD
*/
HRESULT DISP_DDD_SetFormat(AMP_DISP_DDD_Format ddd_format, BOOL enableFlag);
HRESULT DISP_DDD_SetParameters(INT32 strength, INT32 focal_point);
HRESULT DISP_DDD_GetParameters(AMP_DISP_DDD_Format *pDdd_format,
                               INT32 *pStrength, INT32 *pFocal_point);

/**
* Frame Capture
*/
HRESULT DISP_GetCaptureInfo(AMP_DISP_PLANE_ID uiPlaneId, AMP_DISP_PLANE_CAP_INFO *pVidInfo);
HRESULT DISP_CaptureVideoFrame(AMP_DISP_CAP_FRM_SETTING *pCapFrmSetting,
                                       AMP_DISP_CAP_FRM_INFO *pVidCapInfo);
HRESULT DISP_CaptureAUXFrame(UINT32 paddrhandle);

/**
* Window
*/
HRESULT DISP_ConfigWinAttr(AMP_DISP_PLANE_ID uiPlaneId, AMP_DISP_VPP_WIN_ATTR *winAttr);

HRESULT DISP_SaveLastIns(VOID *uiIns, AMP_DISP_PLANE_ID uiPlaneId);


HRESULT DISP_RegisterVscaleUpdateNotifier(INT32 *pHandle, DISP_VSCALE_UPDATE_CALLBACK *pCallback);
HRESULT DISP_UnregisterVscaleUpdateNotifier(INT32 iHandle);
HRESULT DISP_SetThdrVitmParams(INT32 icmdId, INT32 iValue);

HRESULT DISP_SetDcmMode(INT32 iInMode, INT32 iOutMode);

BOOL is_last_frame(AMP_BD_HANDLE pAMPBD);

HRESULT DISP_OUT_Debug(UINT32 uiCmd, UINT32 uiCmdParam, AMP_BD_ST *pInBD, AMP_BD_ST *pOutBD);
#ifdef __cplusplus
}
#endif

#endif

