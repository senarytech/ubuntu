/*******************************************************************************
*                Copyright 2007, MARVELL SEMICONDUCTOR, LTD.                   *
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

#ifndef _VPP_API_H_
#define _VPP_API_H_

#include "errcode_vpp.h"

typedef void (*voidfunc_t)(void *);

//#define E_SUC               ( 0x00000000 )
//#define E_ERR               ( 0x80000000 )
//#define E_VPP_BASE          ( 0x0200 << 16)   //VPP base
//#define E_VPP( code ) ( E_ERR | E_VPP_BASE | ( (code) & 0xFFFF ) )

/* error code */
#define VPP_E_NODEV         E_VPP(1)
#define VPP_E_BADPARAM      E_VPP(2)
#define VPP_E_BADCALL       E_VPP(3)
#define VPP_E_UNSUPPORT     E_VPP(4)
#define VPP_E_IOFAIL        E_VPP(5)
#define VPP_E_UNCONFIG      E_VPP(6)
#define VPP_E_CMDQFULL      E_VPP(7)
#define VPP_E_FRAMEQFULL    E_VPP(8)
#define VPP_E_BCMBUFFULL    E_VPP(9)
#define VPP_E_NOMEM         E_VPP(10)
#define VPP_EVBIBUFFULL  E_VPP(11)
#define VPP_EHARDWAREBUSY  E_VPP(12)

#define MV_VPP_OK		   0
#define MV_VPP_ENODEV	   VPP_E_NODEV
#define MV_VPP_EBADPARAM   VPP_E_BADPARAM
#define MV_VPP_EBADCALL    VPP_E_BADCALL
#define MV_VPP_EUNSUPPORT  VPP_E_UNSUPPORT
#define MV_VPP_EIOFAIL	   VPP_E_IOFAIL
#define MV_VPP_EUNCONFIG   VPP_E_UNCONFIG
#define MV_VPP_ECMDQFULL   VPP_E_CMDQFULL
#define MV_VPP_EFRAMEQFULL VPP_E_FRAMEQFULL
#define MV_VPP_EBCMBUFFULL VPP_E_BCMBUFFULL
#define MV_VPP_ENOMEM	   VPP_E_NOMEM
#define MV_VPP_EVBIBUFFULL VPP_EVBIBUFFULL
#define MV_VPP_EHARDWAREBUSY VPP_EHARDWAREBUSY
#define VPP_IOCTL_VBI_DMA_CFGQ 0xbeef0001
#define VPP_IOCTL_VBI_BCM_CFGQ 0xbeef0002
#define VPP_IOCTL_VDE_BCM_CFGQ 0xbeef0003
#define VPP_IOCTL_TIMING	   0xbeef0004
#define VPP_IOCTL_START_BCM_TRANSACTION 0xbeef0005
#define VPP_IOCTL_BCM_SCHE_CMD 0xbeef0006
#define VPP_IOCTL_INTR_MSG	   0xbeef0007
#define CEC_IOCTL_RX_MSG_BUF_MSG	 0xbeef0008
#define VPP_CC_MSG_TYPE_VPP 0x00
#define VPP_CC_MSG_TYPE_CEC 0x01
#define VPP_FRAME_IN_USE	0x10

/* definition of display mode */
typedef enum {
	DISP_NON_DEFINED  = -1,
	DISP_STILL_PIC = 0,
	DISP_FRAME	   = 1,
	DISP_PATGEN    = 2,
} ENUM_DISP_MODE;

/* definition of video component data format in frame buffer */
typedef enum {
	SRCFMT_MIN		= 0,
	SRCFMT_YUV422	= 0, /* only valid for video plane */
	SRCFMT_ARGB32	 = 1, /* only valid for gfx/PG planes */
	SRCFMT_YUV420SP = 2,
	SRCFMT_YUV420SP_TILED_MODE0 = 3,
	SRCFMT_YUV420SP_TILED_MODE1 = 4,
	SRCFMT_MAX
} ENUM_SRC_FMT;


/* definition of video component data order in frame buffer */
typedef enum {
	ORDER_ARGB		= 0,
	ORDER_ABGR		= 1,
	ORDER_RGBA		= 2,
	ORDER_BGRA		= 3,
	ORDER_ARGB32_MAX = 4,
	ORDER_AVYU		= 0,
	ORDER_AUYV		= 1,
	ORDER_VYUA		= 2,
	ORDER_UYVA		= 3,
	ORDER_UYVY		= 0,
	ORDER_VYUY		= 1,
	ORDER_YUYV		= 2,
	ORDER_YVYU		= 3,
} ENUM_SRC_ORDER;

/* definition of CPCB output video format */
typedef enum {
	OUTPUT_PROGRESSIVE	 = 0,
	OUTPUT_INTERLACED	  = 1,
	OUTPUT_AUTO_SELECT	   = 2,
} ENUM_OUTPUT_MODE;

/* definition of all the output color supported by VPP*/
typedef enum {
	OUTPUT_COLOR_FMT_INVALID	 = -1,
	FIRST_OUTPUT_COLOR_FMT	   = 0,
	OUTPUT_COLOR_FMT_RGB888		 = 0,
	OUTPUT_COLOR_FMT_YCBCR444	 = 1,
	OUTPUT_COLOR_FMT_YCBCR422	 = 2,
	OUTPUT_COLOR_FMT_YCBCR420	 = 3,
	MAX_NUM_OUTPUT_COLOR_FMTS
} ENUM_OUTPUT_COLOR_FMT;

/* definition of all the bit depths (output) supported by VPP*/
typedef enum {
	OUTPUT_BIT_DEPTH_INVALID = -1,
	FIRST_OUTPUT_BIT_DEPTH	 = 0,
	OUTPUT_BIT_DEPTH_12BIT	   = 0,
	OUTPUT_BIT_DEPTH_10BIT	 = 1,
	OUTPUT_BIT_DEPTH_8BIT	  = 2,
	MAX_NUM_OUTPUT_BIT_DEPTHS
} ENUM_OUTPUT_BIT_DEPTH;

/* definition of all the input depths (input) supported by VPP*/
typedef enum {
	INPUT_BIT_DEPTH_INVALID = -1,
	FIRST_INPUT_BIT_DEPTH	= 0,
	INPUT_BIT_DEPTH_8BIT	= 0,
	INPUT_BIT_DEPTH_10BIT	= 1,
	MAX_NUM_INPUT_BIT_DEPTHS
} ENUM_INPUT_BIT_DEPTH;

/* definition of aspect ratio */
typedef enum {
	ASPECT_RATIO_INVALID = -1,
	FIRST_ASPECT_RATIO	 = 0,
	ASPECT_RATIO_NONE	 = 0,
	ASPECT_RATIO_4BY3	 = 1,
	ASPECT_RATIO_16BY9	 = 2,
	MAX_ASPECT_RATIO
} ENUM_ASPECT_RATIO;

/* definition of scan information */
typedef enum {
	SCAN_DATA_INVALID = -1,
	FIRST_SCAN_DATA   = 0,
	SCAN_DATA_NONE	  = 0,
	OVER_SCAN_DATA	  = 1,
	UNDER_SCAN_DATA   = 2,
	MAX_SCAN_DATA
} ENUM_SCAN_DATA;

/* definition of VPP TG timing formats */
typedef enum {
    RES_INVALID   = -1,
    FIRST_RES     = 0,
    RES_NTSC_M    = 0,
    RES_NTSC_J    = 1,
    RES_PAL_M     = 2,
    RES_PAL_BGH   = 3,
    RES_525I60    = 4,
    RES_525I5994  = 5,
    RES_625I50    = 6,
    RES_525P60    = 7,
    RES_525P5994  = 8,
    RES_625P50    = 9,
    RES_720P30    = 10,
    RES_720P2997  = 11,
    RES_720P25    = 12,
    RES_720P60    = 13,
    RES_720P5994  = 14,
    RES_720P50    = 15,
    RES_1080I60   = 16,
    RES_1080I5994 = 17,
    RES_1080I50   = 18,
    RES_1080P30   = 19,
    RES_1080P2997 = 20,
    RES_1080P25   = 21,
    RES_1080P24   = 22,
    RES_1080P2398 = 23,
    RES_1080P60   = 24,
    RES_1080P5994 = 25,
    RES_1080P50   = 26,
    RES_LVDS_1080P48   = 27,
    RES_LVDS_1080P50   = 28,
    RES_LVDS_1080P60   = 29,
    RES_LVDS_2160P12   = 30,
    RES_VGA_480P60 = 31,
    RES_VGA_480P5994 = 32,
    FIRST_RES_3D = 33,
    RES_720P50_3D = 33,
    RES_720P60_3D = 34,
    RES_720P5994_3D = 35,
    RES_1080P24_3D = 36,
    RES_1080P2398_3D = 37,
    RES_1080P30_3D = 38,
    RES_1080P2997_3D = 39,
    RES_1080P25_3D = 40,
    RES_1080I60_FP = 41,
    RES_1080I5994_FP = 42,
    RES_1080I50_FP = 43,
    RES_LVDS_1920X540P60_3D = 44,
    RES_LVDS_1920X540P30_3D = 45,
    RES_LVDS_1920X540P24_3D = 46,
    RES_LVDS_720P100_3D = 47,
    RES_LVDS_720P120_3D = 48,
    RES_LVDS_1080P48_3D = 49,
    RES_LVDS_1080P50_3D = 50,
    RES_LVDS_1080P60_3D = 51,
    RES_LVDS_1920X540P100_3D = 52,
    RES_LVDS_1920X540P120_3D = 53,
    RES_LVDS_960X1080P100_3D = 54,
    RES_LVDS_960X1080P120_3D = 55,
    MAX_NUM_RES_3D = 55,
    RES_MIN_4Kx2K      = 56,
    RES_4Kx2K2398      = 56,
    RES_4Kx2K24        = 57,
    RES_4Kx2K24_SMPTE  = 58,
    RES_4Kx2K25        = 59,
    RES_4Kx2K2997      = 60,
    RES_4Kx2K30        = 61,
    RES_4Kx2K50        = 62,
    RES_4Kx2K5994      = 63,
    RES_4Kx2K60        = 64,
    RES_4Kx2K30_HDMI   = 65,
    RES_4Kx1K120       = 66,
    RES_MAX_4Kx2K      = 66,
    RES_720P_4Kx1K120_3D = 67,
    RES_720P100        = 68,
    RES_720P11988      = 69,
    RES_720P120        = 70,
    HFR_RES_MIN        = 71,
    RES_1080P100       = 71,
    RES_1080P11988     = 72,
    RES_1080P120       = 73,
    RES_1080P8991      = 74,
    RES_1080P90        = 75,
    HFR_RES_MAX        = 75,
    RES_4Kx2K2398_SMPTE = 76,
    RES_4Kx2K25_SMPTE   = 77,
    RES_4Kx2K2997_SMPTE = 78,
    RES_4Kx2K30_SMPTE   = 79,
    RES_4Kx2K50_SMPTE   = 80,
    RES_4Kx2K5994_SMPTE = 81,
    RES_4Kx2K60_SMPTE   = 82,
    RES_4Kx2K50_420     = 83,
    RES_4Kx2K5994_420   = 84,
    RES_4Kx2K60_420     = 85,
    RES_4Kx2K2398_3D    = 86,
    RES_4Kx2K24_3D      = 87,
    RES_4Kx2K25_3D      = 88,
    RES_4Kx2K2997_3D    = 89,
    RES_4Kx2K30_3D      = 90,
    RES_LVDS_1088P60    = 91, //Non Standard Resolution
    RES_LVDS_1366X768P60 = 92,
    RES_LVDS_1366X768P5994 = 93,
    RES_LVDS_1366X768P50 = 94,
    RES_LVDS_1366X768P48 = 95,
    RES_LVDS_1366X768P4796 = 96,
    RES_MIN_DSI          = 97,
    RES_DSI_540x960P60   = 97,
    RES_DSI_1920x1200P60 = 98,
    RES_DSI_800x1280P60  = 99,
    RES_DSI_1920x1080P5994 = 100,
    RES_DSI_WNC_800x1280P60 = 101,
    RES_DSI_CUSTOM       = 102,
    RES_DSI_UBQ_800x1280P60 = 103,
    RES_MAX_DSI            = 103,
    RES_CVT_RB_MIN             = 104,
    RES_CVT_RB_3840_1600P60    = 104,
    RES_CVT_RB_1920_1600P60    = 105,
    RES_CVT_RB_3440_1440P60    = 106,
    RES_CVT_RB_1720_1440P60    = 107,
    RES_CVT_RB_1920_2160P60    = 108,
    RES_CVT_RB_MAX             = 108,
    RES_RESET,
    MAX_NUM_RESS
} ENUM_CPCB_TG_RES;

/* definition of VPP status */
typedef enum {
	STATUS_INACTIVE			= 0,
	STATUS_INACTIVE_PENDING = 1,
	STATUS_ACTIVE			= 2,
	STATUS_ACTIVE_PENDING	= 3,
	STATUS_DISP_VIDEO		= 4,
} ENUM_PLANE_STATUS;

/* definition of VPP CPCB video outputs(for Berlin) */
typedef enum {
	CPCB_INVALID  = -1,
	FIRST_CPCB	  = 0,
	CPCB_1		  = 0,
	MAX_NUM_CPCBS
} ENUM_CPCB_ID;
/* definition of alpha polarity */
typedef enum {
	ALPHA_POL_NORMAL = 0,
	ALPHA_POL_INVERSE = 1,
} ENUM_ALPHA_POL;

#endif

