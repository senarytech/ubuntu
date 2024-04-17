/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2020 Synaptics Incorporated
 * Author: Shanmugam Ramachandran <sramacha@synaptics.com>
 *
 */

#ifndef __VPP_CMD_H__
#define __VPP_CMD_H__

//#define TA_FASTLOGO_UUID {0x93991705, 0xFACB, 0xabcd, {0xBF, 0xFB, 0xC8, 0xD6, 0x7F, 0xBF, 0x88, 0x91}}
#define TA_FASTLOGO_UUID {0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x03, 0xf5}}

/* enum for VPP commands */
typedef enum {
    VPP_INIT,
    VPP_AVIORESET,
    VPP_CREATE,
    VPP_RESET,
    VPP_CONFIG,
    VPP_PASSSHM,
    VPP_SETRES,
    VPP_SETREFWIN,
    VPP_SETREFWINFROMISR,
    VPP_OPENDISPWIN,
    VPP_CHANGEDISPWIN,
    VPP_CHANGEDISPWINFROMISR,
    VPP_GETDISPWINDOW,
    VPP_GETREFWINDOW,
    VPP_GETCONTENTWINDOW,
    VPP_HDMISETVIDFMT,
    VPP_HDMISETTMDSMUXSEL,
    VPP_HANDLEINT,
    VPP_DISPFRAME,
    VPP_RECYCLEFRAME,
    VPP_SETDISPMODE,
    VPP_SETDISPMODEFROMISR,
    VPP_GETFIELDID,
    VPP_CLOSEDISPWINDOW,
    VPP_GETPLANESTATUS,
    VPP_SETCPCBBGCOLOR,
    VPP_GETCPCBOUTRES,
    VPP_GETCPCBVCOFREQ,
    VPP_GETCPCBINPUTCLOCK,
    VPP_GETCPCBOUTPIXELCLOCK,
    VPP_GETCPCBOUTFRAMERATE,
    VPP_SETPLANEMUTE,
    VPP_GETRUNTIMESTATUS,

    VPP_SET3DCONVERTMODE,
    VPP_SET3DINVERTMODE,
    VPP_GET3DCONVERTMODE,
    VPP_GETVIEWID,
    VPP_SETSSTYPE,
    VPP_SETSSDEFAULTOFFSET,
    VPP_SETSSOFFSETMODE,
    VPP_SETSSOFFSETSEQIDREF,

    VPP_SETALPHAPOLARITY,
    VPP_SETSCALARCOEFFMODE,
    VPP_LOADSCLCUSTCOEFFS,
    VPP_RELOADSCLDEFCOEFFS,
    VPP_GETSCLCOEFFMODES,
    VPP_SETPICTURECONTROL,
    VPP_SETBRLEVEL,
    VPP_SETSCALARNONLINERCONTROL,
    VPP_SETDEINTVNRMODE,
    VPP_SETSCALARPHASEDDA,
    VPP_SETPLANEBORDERALPHA,

    VPP_SETVOUTSOURCE,
    VPP_SETVOUTMUTE,
    VPP_SETVOUTMUTE_EXT,

    VPP_SETGFXINPUTMODE,
    VPP_SETGFXCSC,

    VPP_HDMISETPHYPOWER,
    VPP_HDMIENABLEAUDIOFMT,
    VPP_HDMISET3DSTRUCT,
    VPP_HDMIAUDIOFIFORESET,
    VPP_HDMISETAUDIOMUTE,
    VPP_HDMISETHDMIMODE,
    VPP_HDMISETVIDEOINFO,
    VPP_HDMIENABLE,
    VPP_HDMISTOPAUXPKT,

    CEC_CFG_FEATURE_SUPPORT,
    CEC_GET_FEATURE_SUPPORT,
    CEC_ENABLE,
    CEC_SET_PHY_ADDR,
    CEC_ALLOC_LOG_ADDR,
    CEC_SET_RETRANSIMIT_COUNT,

    VPP_SETXVYCCMODE,
    VPP_ENABLEHDCP,
    VPP_GETHDCPSTATE,
    VPP_GETHDCPVERSION,
    VPP_SETHDCPVERSION,
    VPP_SETHDCPREPEATERMODE,
    VPP_SETMACROVISION,
    VPP_SETVDACPOWER,

    VPP_GETHPDSTATUS,

    VPP_HDMICMDHANDLER,
    VPP_CECHANDLER,
    CMD_OSAL_HANDLER,

    VPP_REGHDCPCALLBACK,
    VPP_UNREGHDCPCALLBACK,
    VPP_REGHDMICALLBACK,
    VPP_UNREGHDMICALLBACK,
    VPP_REGHDMICPCB0CALLBACK,
    VPP_UNREGHDMICPCB0CALLBACK,
    VPP_REGCECCALLBACK,
    VPP_UNREGCECCALLBACK,

    VPP_SUSPEND,
    VPP_RESUME,

    VPP_GETSTSCOUNTER,
    VPP_GETSTSRUNNINGCOUNTER,
    VPP_HDMI_SCDC_SETSHM,
    VPP_HDCP2XDDCSETSHM,
    VPP_HDCP2XHANDLER,
    VPP_HDCP2XNOTIFYDEVACTIVE,
    VPP_HDCP2XGETDEVSTAT,
    VPP_HDCP2XSTARTAUTH,
    VPP_HDCP2XGETCONNINFO,
    VPP_HDCP2XGETDEVID,
    VPP_HDCP2XSENDCSM,

    VPP_BYPASS_ENABLE,
    VPP_HDMIHPDTASKHANDLER,

    VPP_DUMPREGS,
#ifdef TZ_3_0
    VPP_INITIATE_VPPS,
#endif
    /*Since this ID is specific used for verimatrix*/
    /*So, please don't change its value*/
    VPP_WATERMARK = 1001,

    VPP_CAPTUREVIDFRAME,
    VPP_REGCAPTUREVIDFRAME,
    VPP_FRAMECAPTUREHANDLER,
    VPP_SETHDRENABLE,
    VPP_DESTROY,
    VPP_SETROTANGLE,
    VPP_GETROTANGLE,
    VPP_CFGINVSCAN,
    VPP_GETCURHDCPVERSION,
    VPP_GETSINKHDCPCAPABILITY,
    VPP_SETCROPWIN,
    VPP_SETDEINTMODE,
    VPP_ENCLKGATE,
    VPP_GETCLKSIGNATURE,
    VPP_SETHDMIPHY,
    VPP_SETEDRENABLE,
    VPP_GETHDR10PLAYBACKSTATUS,

    VPP_DVFRAMEDUMPENABLE,
    VPP_SETSRM,


    //Start of the fastlogo specific command where physical address of the buffer is passed
    //Same as FL_VPP_PASSSHM expect that instead of passing memref detials physical address is passed directly
    //We can introduce VPP_SHM_CMD_MAX
    VPP_PASSPHY= 2001,
    VPP_PASSPAR,
    VPP_STOP,
    VPP_ENABLEAUTOPUSH,
    VPP_HDMISETTXCTRL
}VPP_CMD_ID;

/* enum for VPP callbacks */
typedef enum {
    VPP_CB_GETVCOFREQ,
    VPP_CB_SETVIDEOFREQ,
    VPP_CB_GETOUTPUTFREQ,
    VPP_CB_CLEANUPBCM,
    VPP_CB_ENABLEINT,
    VPP_CB_MALLOCSHM,
    VPP_CB_FREESHM,
    VPP_CB_NOTIFYCAPAUXFRAM,

    VPP_CB_CREATE_HDMITASK,
    VPP_CB_CREATE_CECTASK,
    VPP_CB_DELETE_HDMITASK,//FIXME: club together in single callback??
    VPP_CB_DELETE_CECTASK,

    VPP_CB_GPIO_PORTSETINOUT,
    VPP_CB_GPIO_PORTWRITE,
    VPP_CB_GPIO_PORTREAD,

    CB_TIMER_FIRST,// = 0x500,
    CB_TIMER_CREATE = CB_TIMER_FIRST,
    CB_TIMER_DESTROY,
    CB_TIMER_RESET,
    CB_TIMER_START,
    CB_TIMER_STOP,
    CB_TIMER_RESUME,
    CB_TIMER_PAUSE,
    CB_TIMER_RESET_PERIOD,
    CB_TIMER_END = CB_TIMER_RESET_PERIOD,

    VPP_CB_HDCP,
    VPP_CB_HDMI,
    VPP_CB_CEC,
    VPP_CB_RDCEC_MSG,
    VPP_CB_HDMICPCB0,
    VPP_CB_HDMI_SCDC_WRITE,
    VPP_CB_HDMI_SCDC_READ,
    VPP_CB_CREATE_HDCP2XTASK,
    VPP_CB_DELETE_HDCP2XTASK,
    VPP_CB_HDCP2X_DDC_WRITE,
    VPP_CB_HDCP2X_DDC_READ,
    VPP_CB_CREATE_HDMI_HPDTASK,
    VPP_CB_DELETE_HDMI_HPDTASK,

    VPP_CB_DIAG_SETCLKFREQ,
    VPP_CB_DIAG_CHNGOTHRCLK,

    VPP_CB_CREATE_FRAMECAPTURETASK,
    VPP_CB_DELETE_FRAMECAPTURETASK,
    VPP_CB_NOTIFYCAPVIDFRAM,
}VPP_CB_ID;

//args for CMD_OSAL_HANDLER
enum {
    HANDLE_OSAL_TIMER,
};

/* enum for VPP shm ID */
typedef enum {
    DISPLAY_FRAME                   = 0,
    WRITE_REG_GROUP                 = 1,    //API control
    LOAD_LVDS_GAMMALUT              = 2,
    READ_LVDS_GAMMALUT              = 3,
    LOAD_LVDS_ICSC_CUSTCOEFFS       = 4,
    SET_LVDS_MAPPING                = 5,
    PREPARE_SCL_CUSTCOEFFS          = 6,
    SET_GFX_LUT                     = 7,
    SET_SHIFT3D_PARAM               = 8,
    GET_SHIFT3D_PARAM               = 9,
    SET_STILL_PICTURE               = 10,

    SET_HDMI_AUDIOCFG_PARAM         = 11,
    SET_HDMI_AUDIOVUCCFG_PARAM      = 12,
    SET_HDMI_AUDIO_INFO             = 13,
    GET_HDMI_SINKCAPS               = 14,
    READ_HDMI_SINKCAPS              = 15,
    READ_HDMI_RAWEDID               = 16,
    SEND_HDMI_AUXPKT                = 17,

    SET_ZORDER                      = 18,
    GET_ZORDER                      = 19,

    SET_WSSDATA                     = 20,
    SET_CGMSDATA                    = 21,

    CEC_SEND_MSG                    = 22,
    CEC_READ_EVT_INFO               = 23,
    VPP_LVTXSETLANEMAPP             = 24,
    VPP_GET_RES_DESCRIPTION         = 25,

    READ_REG_GROUP                  = 26,
    GET_HDCP_KSV                    = 27,
    READ_HDMI_HDCP_AKSV             = 28,
    GET_HDMI_AUXPKT                 = 29,
    GET_PLANE_VIDEO_INFO            = 30,
    GET_HDMITX_REG_DUMP             = 31,
    VPP_SETCLKSIGNATURE             = 32,

    VPP_DVINITFRAMEDUMP            = 33,
    VPP_DVGETFRAMEDUMPINFO         = 34,

}VPP_SHM_ID;

#ifdef TZ_3_0
#define INSTID_MASK 0x00FF0000
#define CMDID_MASK 0x0000FFFF
#define INSTID_POS 16
#define CREATE_CMD_ID(cmdId,instID)  (((instID<<INSTID_POS)&INSTID_MASK)|(cmdId&CMDID_MASK))
#define GETINST_ID(cmdID) (cmdID & INSTID_MASK)>>16
#define GETCMD_ID(cmdID)  (cmdID & CMDID_MASK)
#endif

#endif /* __VPP_CMD_H__ */
