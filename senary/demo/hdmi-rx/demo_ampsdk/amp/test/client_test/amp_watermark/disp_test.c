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
//! \file disp_test.c
//!
//! \brief client test code for display service.
//!
//!
//! Purpose:
//!
//!
//! Version, Date and Author :
//!   V 1.00,    December 25 2012,    Jiankang Yu
//! Note:
////////////////////////////////////////////////////////////////////////////////


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "macros.h"
#include "OSAL_api.h"
#include "amp_client.h"
#include "amp_client_support.h"
#include "isl/amp_logger.h"
#include "isl/amp_buf_desc.h"
#include "isl/amp_event_queue.h"
#include "disp_test_prv.h"
#include "vpp_api.h"

#include "disp_test_framebuffer.h"
#include "math.h"

/*Definition*/
#define MAX_CMD_LEN 1024
#define MAX_ARG_CNT 20
#define PROMPT                  "DISP_TEST:$ "
#define MAX_BUF_SIZE 3840*2160*2
AMP_SHM_HANDLE hCapBuf;
AMP_BD_HANDLE prev_hBD=0;

#define ARRAY_LENGTH(x)    (sizeof(x)/sizeof(x[0]))
#define PLANE_ID_MASK    0xFFE00000
#define PLANE_ID_OFFSET 21
#define PARAM_MASK   0x1FFFFF

/*##############################log msg help definition#########################*/

char m_hdmitx_colorfmt_table[MAX_NUM_OUTPUT_COLOR_FMTS][MAX_MSG_SIZE] = {
"   OUTPUT_COLOR_FMT_RGB888      = 0",
"   OUTPUT_COLOR_FMT_YCBCR444    = 1",
"   OUTPUT_COLOR_FMT_YCBCR422    = 2",
"   OUTPUT_COLOR_FMT_YCBCR420    = 3",
};

char m_hdmitx_bitdepth_table[MAX_NUM_OUTPUT_BIT_DEPTHS][MAX_MSG_SIZE] = {
"   OUTPUT_BIT_DEPTH_12BIT    = 0",
"   OUTPUT_BIT_DEPTH_10BIT   = 1",
"   OUTPUT_BIT_DEPTH_8BIT   = 2",
};

char m_hdmitx_aspect_ratio_table[2][MAX_MSG_SIZE] = {
"   CEA_PICT_AR_4_3   = 1",
"   CEA_PICT_AR_16_9   = 2",
};

char m_display_tgid_table[2][MAX_MSG_SIZE] = {
"   CPCB_0   = 0",
"   CPCB_1   = 1",
};

char m_display_outmode_table[AMP_DISP_OUT_TYPE_MAX][MAX_MSG_SIZE] = {
"   SINGLE_MODE_PRI  = 0",
"   SINGLE_MODE_SEC  = 1",
"   DUAL_MODE_PIP    = 2",
"   DUAL_MODE_GFX0    = 3",
"   DUAL_MODE_GFX1    = 4",
};

char m_color_fmt_table[][MAX_MSG_SIZE] = {
"   SRCFMT_YUV422   = 0, /* only valid for video plane */",
"   SRCFMT_ARGB32   = 1, /* only valid for gfx/PG planes */",
"   SRCFMT_ARGB24   = 2, /* only valid for gfx planes */",
"   SRCFMT_ARGB4444 = 3, /* only valid for gfx planes */",
"   SRCFMT_RGB565   = 4, /* only valid for gfx planes */",
"   SRCFMT_ARGB1555 = 5, /* only valid for gfx planes */",
"   SRCFMT_LUT8     = 6, /* only valid for gfx planes */",
"   SRCFMT_AYUV32   = 7, /* only valid for PG planes */",
"   SRCFMT_YUV420   = 8, /* Only for VPP 4K Bypass path */",
"   SRCFMT_YUV420SP = 9,",
"   SRCFMT_YUV444   = 10,",
"   SRCFMT_RGB444   = 11,",
"   SRCFMT_ARGB32_4K   = 12,/* Only for 4K Bypass Path */",
"   SRCFMT_YUV420SP_TILED_MODE0 = 13,",
"   SRCFMT_YUV420SP_TILED_MODE1 = 14,",
"   SRCFMT_ARGB32_PM   = 15, /* only valid for gfx/PG planes */",
"   SRCFMT_XRGB32   = 16, /* only valid for gfx/PG planes */",
"   SRCFMT_YUV420SP_V4H8 = 17"
};

char m_bit_depth_table[][MAX_MSG_SIZE] = {
"   OUTPUT_BIT_DEPTH_8BIT  = 0",
"   OUTPUT_BIT_DEPTH_10BIT = 1",
"   OUTPUT_BIT_DEPTH_12BIT = 2",
};

char m_src_order_table[][MAX_MSG_SIZE] = {
"   ORDER_ARGB  = 0, /* only valid for gfx planes */",
"   ORDER_ABGR  = 1, /* only valid for gfx planes */",
"   ORDER_RGBA  = 2, /* only valid for gfx planes */",
"   ORDER_BGRA  = 3, /* only valid for gfx planes */",
"   ORDER_AVYU  = 0, /* only valid for PG plane */",
"   ORDER_AUYV  = 1, /* only valid for PG plane */",
"   ORDER_VYUA  = 2, /* only valid for PG plane */",
"   ORDER_UYVA  = 3, /* only valid for PG plane */",
"   ORDER_UYVY  = 0, /* only valid for video planes */",
"   ORDER_VYUY  = 1, /* only valid for video planes */",
"   ORDER_YUYV  = 2, /* only valid for video planes */",
"   ORDER_YVYU  = 3, /* only valid for video planes */",
"   ORDER_YUV   = 0, /* only valid for video planes */",
"   ORDER_VYU   = 1, /* only valid for video planes */",
};
char m_frame_type_table[][MAX_MSG_SIZE] = {
"   DISP_INPUT_INTERLACED_FRAME   = 0, /*BIT 0 frame-type(0-Interlaced)*/",
"   DISP_INPUT_PROGRESSIVE_FRAME  = 1, /*BIT 0 frame-type(1-progressive)*/",
"   DISP_INPUT_CMA_FRAME          = 0, /*BIT 1 frame-type(0-CMA Frame)*/",
"   DISP_INPUT_MMU_ENABLED_FRAME  = 1, /*BIT 1 frame-type(1-MMU Frame)*/",
};
char m_pattern[][MAX_MSG_SIZE] = {
    "CHESS              = 0 (default)",
    "SPHERE_ON_CHESS    = 1"
};

char m_rampColor[][MAX_MSG_SIZE] = {
    "NONE           = 0",
    "RED            = 1 (default)",
    "GREEN          = 2",
    "GREEN+RED      = 3",
    "BLUE           = 4",
    "BLUE+RED       = 5",
    "BLUE+GREEN     = 6",
    "BLUE+GREEN+RED = 7",
};

char m_hdmitx_resolution_table[MAX_NUM_RESS + 1][MAX_MSG_SIZE] = {
"   FIRST_RES     = 0   RES_NTSC_M    = 0",
"   RES_NTSC_J    = 1",
"   RES_PAL_M     = 2",
"   RES_PAL_BGH   = 3",
"   RES_525I60    = 4",
"   RES_525I5994  = 5",
"   RES_625I50    = 6",
"   RES_525P60    = 7",
"   RES_525P5994  = 8",
"   RES_625P50    = 9",
"   RES_720P30    = 10",
"   RES_720P2997  = 11",
"   RES_720P25    = 12",
"   RES_720P60    = 13",
"   RES_720P5994  = 14",
"   RES_720P50    = 15",
"   RES_1080I60   = 16",
"   RES_1080I5994 = 17",
"   RES_1080I50   = 18",
"   RES_1080P30   = 19",
"   RES_1080P2997 = 20",
"   RES_1080P25   = 21",
"   RES_1080P24   = 22",
"   RES_1080P2398 = 23",
"   RES_1080P60   = 24",
"   RES_1080P5994 = 25",
"   RES_1080P50   = 26",
"   RES_LVDS_1080P48   = 27",
"   RES_LVDS_1080P50   = 28",
"   RES_LVDS_1080P60   = 29",
"   RES_LVDS_2160P12   = 30",
"   RES_VGA_480P60 = 31",
"   RES_VGA_480P5994 = 32",
"   FIRST_RES_3D = 33   RES_720P50_3D = 33",
"   RES_720P60_3D = 34",
"   RES_720P5994_3D = 35",
"   RES_1080P24_3D = 36",
"   RES_1080P2398_3D = 37",
"   RES_1080P30_3D = 38",
"   RES_1080P2997_3D = 39",
"   RES_1080P25_3D = 40",
"   RES_1080I60_FP = 41",
"   RES_1080I5994_FP = 42",
"   RES_1080I50_FP = 43",
"   RES_LVDS_1920X540P60_3D = 44",
"   RES_LVDS_1920X540P30_3D = 45",
"   RES_LVDS_1920X540P24_3D = 46",
"   RES_LVDS_720P100_3D = 47",
"   RES_LVDS_720P120_3D = 48",
"   RES_LVDS_1080P48_3D = 49",
"   RES_LVDS_1080P50_3D = 50",
"   RES_LVDS_1080P60_3D = 51",
"   RES_LVDS_1920X540P100_3D = 52",
"   RES_LVDS_1920X540P120_3D = 53",
"   RES_LVDS_960X1080P100_3D = 54",
"   RES_LVDS_960X1080P120_3D = 55   MAX_NUM_RES_3D = 55",
#if (BERLIN_CHIP_VERSION >= BERLIN_BG2_DTV)
"   RES_MIN_4Kx2K      = 56   RES_4Kx2K2398      = 56",
"   RES_4Kx2K24        = 57",
"   RES_4Kx2K24_SMPTE  = 58",
"   RES_4Kx2K25        = 59",
"   RES_4Kx2K2997      = 60",
"   RES_4Kx2K30        = 61",
"   RES_4Kx2K50        = 62",
"   RES_4Kx2K5994      = 63",
"   RES_4Kx2K60        = 64",
"   RES_4Kx2K30_HDMI   = 65",
"   RES_4Kx1K120       = 66   RES_MAX_4Kx2K      = 66",
"   RES_720P_4Kx1K120_3D = 67",
"   RES_720P100        = 68",
"   RES_720P11988      = 69",
"   RES_720P120        = 70",
"   HFR_RES_MIN        = 71 RES_1080P100       = 71",
"   RES_1080P11988     = 72",
"   RES_1080P120       = 73",
"   RES_1080P8991      = 74",
"   RES_1080P90        = 75 HFR_RES_MAX        = 75",
"   RES_4Kx2K2398_SMPTE = 76",
"   RES_4Kx2K25_SMPTE   = 77",
"   RES_4Kx2K2997_SMPTE = 78",
"   RES_4Kx2K30_SMPTE   = 79",
"   RES_4Kx2K50_SMPTE   = 80",
"   RES_4Kx2K5994_SMPTE = 81",
"   RES_4Kx2K60_SMPTE   = 82",
"   RES_4Kx2K50_420     = 83",
"   RES_4Kx2K5994_420   = 84",
"   RES_4Kx2K60_420     = 85",
"   RES_4Kx2K2398_3D    = 86",
"   RES_4Kx2K24_3D      = 87",
"   RES_4Kx2K25_3D      = 88",
"   RES_4Kx2K2997_3D    = 89",
"   RES_4Kx2K30_3D      = 90",
"   RES_LVDS_1088P60    = 91",
"   RES_LVDS_1366X768P60   = 92",
"   RES_LVDS_1366X768P5994 = 93",
"   RES_LVDS_1366X768P50   = 94",
"   RES_LVDS_1366X768P48   = 95",
"   RES_LVDS_1366X768P4796 = 96",
#endif
"   RES_DSI_540x960P60 = 97",
"   RES_DSI_1920x1200P60 = 98",
"   RES_DSI_800x1280P60 = 99",
"   RES_DSI_1920x1080P5994 = 100",
"   RES_DSI_WNC_800x1280P60 = 101",
"   RES_DSI_CUSTOM = 102",
"   RES_DSI_UBQ_800x1280P60 = 103",
"   RES_CVT_RB_3840_1600P60    = 104",
"   RES_CVT_RB_1920_1600P60    = 105",
"   RES_CVT_RB_3440_1440P60    = 106",
"   RES_CVT_RB_1720_1440P60    = 107",
"   RES_CVT_RB_1920_2160P60    = 108",
"   RES_CVT_RB_1920x1200P60    = 109",
"   RES_CUSTOM_2880x1440P60    = 110",
"   RES_800x600P60             = 111",
"   RES_1280x768P60            = 112",
"   RES_1360x768P60            = 113",
"   RES_1024x768P60            = 114",
"   RES_1280x800P60            = 115",
"   RES_1280x1024P60           = 116",
"   RES_1440X900P60            = 117",
"   RES_1400x1050P60           = 118",
"   RES_1680x1050P60           = 119",
"   RES_1600x1200P60           = 120",
"   RES_1366X768P60            = 121",
"   RES_CVT_RB_2560X1440P60    = 122",
"   RES_RESET                  = 123",
};

char m_builtin_frames[][MAX_MSG_SIZE] = {
"BUILD_IN_FRAME_BLACK               = 0",
"BUILD_IN_FRAME_BLUE                = 1",
"BUILD_IN_FRAME_BLUE_WHITE_SQUARE   = 2",
"BUILD_IN_FRAME_TEST_PATTERN        = 3",
"AMP_DISP_BUILD_IN_FRAME_BLACK_ARGB = 4",
"AMP_DISP_BUILD_IN_FRAME_LOGO       = 5",
};

char m_disp_id[AMP_DISP_OUT_MAX][MAX_MSG_SIZE] = {
    "AMP_DISP_OUT_HDMI   = 0",
    "AMP_DISP_OUT_HD     = 1",
    "AMP_DISP_OUT_SD     = 2",
    "AMP_DISP_OUT_TTL24  = 3",
    "AMP_DISP_OUT_TTL30  = 4",
    "AMP_DISP_OUT_LVDS   = 5",
    "AMP_DISP_OUT_DSI    = 6",
};

AMP_DISP_WIN m_PerFrameSrcWin[AMP_DISP_PLANE_MAX], m_PerFrameDstWin[AMP_DISP_PLANE_MAX];

void dispWait()
{
    UINT32 cnt;
    for (cnt = 0; cnt < 10; cnt++) {
        MV_OSAL_Task_Sleep(100);
    }
}

/*##############################log msg help definition#########################*/

/*Global varible*/
static AMP_LIB  amp_lib;
static AMP_FACTORY hFactory;
AMP_DISP gDisp;
MV_OSAL_HANDLE_TASK_t gThreadHandle;
extern DISP_AUTO_HANDLER autocase[];
extern unsigned int uiNumAutoCaseCmd;
static CHAR *client_argv[] =
    {"client", "iiop:1.0//127.0.0.1:999/AMP::FACTORY/factory"};

static int cmd_handler_push_frame(int argc, char *argv[]);

static int cmd_handler_push_frame(int argc, char *argv[])
{
    HRESULT Ret = SUCCESS;
    unsigned int i;
    int PlaneID = -1;
    DISP_TEST_FRAME frame_info;
    int fileOperation = 0;
    int gfxOperation = 0;
    int max_argc = 8;
    int dolbypb = 0, thdrEn = 0, hdrEn = 0, hlgEn = 0, secEn = 0;
    int pushPrevFrame = 0;
    const int fake_argc = 100;
    int actual_argc = -1;
    int iter=0, ms_delay1=0, ms_delay2=0;
    int pushPrevFrame1 = 0;
    FILE *fpDecoderParams = NULL;
    char achLine[200] = "\0";
    int Value = 0, thdr_vitm = 0, thdr_vitm_en = 0;


    fileOperation = 1;
    max_argc++;

	frame_info.frame_type = gfxOperation? DOLBY_FRAME_TYPE_LEGACY_GFX: DOLBY_FRAME_TYPE_LEGACY_VID;
		frame_info.uiThdrPresentMode = 0;
		frame_info.hdrEn = 0;
		frame_info.hlgEn = 0;
    // argv[0]   1  2    3   4 5 6 7 8  9
   // pushframef 0 1920 1080 0 0 0 1 4  /root/raw.dat

        frame_info.dm_md_path = NULL;
        frame_info.comp_md_path = NULL;
        PlaneID = 0;
        frame_info.w = 1920;
        frame_info.h = 1080;
        frame_info.color_fmt = 0;
        frame_info.bit_depth = 0;
        frame_info.src_order = 0; //default 0
        /*
         * This argument(argv[7]) contains bit field as below
         * bit 0: frame-type (0-interlaced , 1-progressive)
         * bit 1: mtr/mmu (0-cma frame, 1-mmu enabled frame)
         */
        frame_info.intelace_mode = 1 & 0x1;
        frame_info.isMtr = (1 >> 1) & 0x1? MMU_ENABLED:0;
        CMDLINE_PRINT("frame_info.intelace_mode %d MTR=%d \n", frame_info.intelace_mode , frame_info.isMtr);
        frame_info.pattern = 4;
        frame_info.rampAttr.rampStartColor = 0x80000000;
        frame_info.rampAttr.rampColor = RAMP_COLOR_DEFAULT;
        frame_info.rampAttr.rampStep = 4;
        frame_info.raw_frame_path = "/root/raw.dat";


    frame_info.isSec = 0;


    if (PlaneID >= AMP_DISP_PLANE_FIRST && PlaneID <= AMP_DISP_PLANE_MAX) {
        if(dolbypb)
            CMDLINE_PRINT("PlaneID: %d, w: %u, h:%u,"
                    "Bitdepth: %d \n", PlaneID, frame_info.w, frame_info.h, frame_info.bit_depth);
        else if(argc != fake_argc)
            CMDLINE_PRINT("PlaneID: %d, w: %u, h:%u,"
                    "Color fmt: %d, Bitdepth: %d, SRC Order: %d, Interlace mode: %d\n",
                    PlaneID, frame_info.w, frame_info.h, frame_info.color_fmt,
                    frame_info.bit_depth, frame_info.src_order, frame_info.intelace_mode);
        if(argc != fake_argc)
            CMDLINE_PRINT("pattern: %d\n", frame_info.pattern);
        if(pushPrevFrame) {
            disptest_push_frame_previous(gDisp, PlaneID);
        } else if(pushPrevFrame1) {
            disptest_push_frame_previous1(gDisp, PlaneID, iter, ms_delay1, ms_delay2);
        } else {
            disptest_push_buildinframe(gDisp, PlaneID, frame_info);
        }
    } else {
        CMDLINE_PRINT("PlaneID: %d unknown\n", PlaneID);
    }

    dispWait();
    if (fpDecoderParams) {
        fclose(fpDecoderParams);
    }

    return Ret;
fail:
    if (fpDecoderParams) {
        fclose(fpDecoderParams);
    }
    return Ret;
}


int disp_main(int argc, char **argv)
{
    unsigned int Ret;

    MV_OSAL_Init();
    AMP_Initialize(2, client_argv, &hFactory);

    AMP_LOG_Initialize(AMP_LOG_FATAL, AMP_LOG_CONSOLE);
    AMP_LOG_Control(AMP_LOG_CMD_SETLEVEL, MODULE_GENERIC,
                    ((1 << AMP_LOG_USER1) - 1), NULL);

    CMDLINE_PRINT("%s,%s: Start!\n", __FILE__, __FUNCTION__);

    /* Create disp service */
    AMP_RPC(Ret, AMP_FACTORY_CreateDisplayService, hFactory, &gDisp);
    if( Ret == SUCCESS ) {
        CMDLINE_PRINT("Create disp service OK\n");
    } else {
        CMDLINE_PRINT("Create disp service Failed\n");
    }

    AMP_RPC(Ret, AMP_FACTORY_CreateLibrary, hFactory, &amp_lib);
    if( Ret == SUCCESS ) {
        CMDLINE_PRINT("Create AMP_Factory Library OK\n");
    } else {
        CMDLINE_PRINT("Create AMP_Factory Library Failed\n");
    }

	cmd_handler_push_frame(argc, argv);
	
	

    CMDLINE_PRINT("%s,%s: End!\n", __FILE__, __FUNCTION__);

    AMP_Deinitialize();

    MV_OSAL_Exit();

    return 0;
}

