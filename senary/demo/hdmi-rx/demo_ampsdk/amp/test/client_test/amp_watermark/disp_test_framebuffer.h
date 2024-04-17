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

#ifndef _DISP_TEST_FRAMEBUFFER_H_
#define _DISP_TEST_FRAMEBUFFER_H_
#include "OSAL_api.h"
#include "amp_client_support.h"
#include "isl/amp_logger.h"
#include "isl/amp_buf_desc.h"
#include "isl/amp_event_queue.h"

#define ENABLE_COLOR_BAR_PATTERN
#define ENABLE_COLOR_RAMP_PATTERN

#define WRAP_FGETS(s, size, stream) {                   \
    if (fgets(s, size, stream) == NULL) {               \
        printf("lineno=%d: fail to fgets\n", __LINE__); \
        Ret = -1;                                       \
        goto fail;                                      \
    }                                                   \
}

typedef enum {
    DISP_SRCFMT_MIN      = 0,
    DISP_SRCFMT_YUV422   = 0, /* only valid for video plane */
    DISP_SRCFMT_ARGB32   = 1, /* only valid for gfx/PG planes */
    DISP_SRCFMT_ARGB24   = 2, /* only valid for gfx planes */
    DISP_SRCFMT_ARGB4444 = 3, /* only valid for gfx planes */
    DISP_SRCFMT_RGB565   = 4, /* only valid for gfx planes */
    DISP_SRCFMT_ARGB1555 = 5, /* only valid for gfx planes */
    DISP_SRCFMT_LUT8     = 6, /* only valid for gfx planes */
    DISP_SRCFMT_AYUV32   = 7, /* only valid for PG planes */
    DISP_SRCFMT_YUV420   = 8, /* Only for VPP 4K Bypass path */
    DISP_SRCFMT_YUV420SP = 9,
    DISP_SRCFMT_YUV444   = 10,
    DISP_SRCFMT_RGB444   = 11,
    DISP_SRCFMT_ARGB32_4K   = 12,/* Only for 4K Bypass Path */
    DISP_SRCFMT_YUV420SP_TILED_MODE0 = 13,
    DISP_SRCFMT_YUV420SP_TILED_MODE1 = 14,
    DISP_SRCFMT_ARGB32_PM   =15,
    DISP_SRCFMT_XRGB32   =16,
	DISP_SRCFMT_YUV420SP_TILED_V4H8 = 17,
    DISP_SRCFMT_MAX
}ENUM_DISP_SRC_FMT;

/* definition of video component data order in frame buffer */
typedef enum {
    DISP_ORDER_ARGB      = 0, /* only valid for gfx planes */
    DISP_ORDER_ABGR      = 1, /* only valid for gfx planes */
    DISP_ORDER_RGBA      = 2, /* only valid for gfx planes */
    DISP_ORDER_BGRA      = 3, /* only valid for gfx planes */
    DISP_ORDER_AVYU      = 0, /* only valid for PG plane */
    DISP_ORDER_AUYV      = 1, /* only valid for PG plane */
    DISP_ORDER_VYUA      = 2, /* only valid for PG plane */
    DISP_ORDER_UYVA      = 3, /* only valid for PG plane */
    DISP_ORDER_UYVY      = 0, /* only valid for video planes */
    DISP_ORDER_VYUY      = 1, /* only valid for video planes */
    DISP_ORDER_YUYV      = 2, /* only valid for video planes */
    DISP_ORDER_YVYU      = 3, /* only valid for video planes */
    DISP_ORDER_YUV       = 0, /* only valid for video planes */
    DISP_ORDER_VYU       = 1, /* only valid for video planes */
}ENUM_DISP_SRC_ORDER;

// reverse vs VPP definition
typedef enum {
    DISP_INPUT_BIT_DEPTH_INVALID = -1,
    DISP_FIRST_INPUT_BIT_DEPTH   = 0,
    DISP_INPUT_BIT_DEPTH_8BIT    = 0,
    DISP_INPUT_BIT_DEPTH_10BIT   = 1,
    DISP_INPUT_BIT_DEPTH_12BIT   = 2,
    DISP_MAX_NUM_INPUT_BIT_DEPTHS
} ENUM_DISP_INPUT_BIT_DEPTH;

typedef enum {
    DISP_INPUT_INTERLACED_FRAME = 0,
    DISP_INPUT_PROGRESSIVE_FRAME = 1
}ENUM_DISP_DEINT_TYPE;

typedef enum {
    PATTERN_MIN = 0,
    PATTERN_DEFAULT = 0,
    PATTERN_CHESS = 0,
    PATTERN_SPHERE_ON_CHESS = 1,
#ifdef ENABLE_COLOR_BAR_PATTERN
    PATTERN_MAX_NON_ARGB = 2,
    PATTERN_COLOR_BAR_VER = 2,
    PATTERN_COLOR_BAR_HOR = 3,
    PATTERN_COLOR_BAR_CUSTOM = 4,
    PATTERN_COLOR_RAMP = 5,
#endif //ENABLE_COLOR_BAR_PATTERN
    PATTERN_MAX,
} ENUM_PATTERN_TYPE;

typedef enum {
    RAMP_COLOR_MIN=0,
    RAMP_COLOR_DEFAULT=1,   //Since this is bit position; select red by default
    RAMP_COLOR_RED  =0,
    RAMP_COLOR_BLUE =1,
    RAMP_COLOR_GREEN=2,
    RAMP_COLOR_MAX,
} ENUM_RAMP_COLOR;

typedef enum {
    DOLBY_FRAME_TYPE_LEGACY_VID = 0,
    DOLBY_FRAME_TYPE_BL         = 1,
    DOLBY_FRAME_TYPE_16bBL_MD   = 3, //CVM Test Mode
    DOLBY_FRAME_TYPE_BL_MD      = 5,
    DOLBY_FRAME_TYPE_BL_EL_MD   = 7,
    DOLBY_FRAME_TYPE_BL_P5EL_MD = 9,
    DOLBY_FRAME_TYPE_LEGACY_GFX = 10,
    DOLBY_FRAME_TYPE_MAX,
} ENUM_DOLBY_FRAME_TYPE;

typedef struct gRampAttr {
    UINT32 rampStartColor;
    UINT32 rampStep;
    UINT32 rampColor;           // used for ARGB format - ramp R/G/B only
}DISP_RAMP_ATTR;

typedef struct VITM_PARAMS_t
{
    INT32 iYuvInRang;
    INT32 iYuvOutRange;
    INT32 iGaussianFilter;
    INT32 iYLutFilter;
    INT32 iPqEn;
    INT32 iPqLmax;
    INT32 iInCscMode;
    INT32 iOutCscMode;
    INT32 iDebanding;
    INT32 iDenoising;
}VITM_PARAMS;

typedef struct gFrameAttr {
    unsigned int w;
    unsigned int h;
    unsigned int sub_w;
    unsigned int sub_h;
    ENUM_DISP_SRC_FMT color_fmt;
    ENUM_DISP_INPUT_BIT_DEPTH bit_depth;
    ENUM_DISP_SRC_ORDER src_order;
    ENUM_DISP_DEINT_TYPE intelace_mode;
    ENUM_PATTERN_TYPE pattern;
    const char *raw_frame_path;
    char *raw_frame_subpath;
    char *comp_md_path;
    char *dm_md_path;
    unsigned int pm_alpha;
    DISP_RAMP_ATTR rampAttr;
    unsigned int frame_type;
    unsigned int uiThdrPresentMode;
    int m_iDisplayOETF;
    int m_iPeakLuminance;
    int m_iProcessMode;
    VITM_PARAMS vitm_params;
    int hdrEn;
    int hlgEn;
    int isSec;
    int isMtr;
}DISP_TEST_FRAME;

HRESULT create_buildin_frame(AMP_BGTAG_FRAME_INFO *pFrameInfo, ENUM_DISP_SRC_FMT iSrcFmt,
                             UINT32 width, UINT32 height, ENUM_DISP_DEINT_TYPE progressive,
                             ENUM_DISP_INPUT_BIT_DEPTH bit_depth, ENUM_DISP_SRC_ORDER src_order,
                             UINT32 pattern, const char *filename,
                             DISP_RAMP_ATTR *pRampAttr, int frame_type);
HRESULT update_mtrr_info ( AMP_BGTAG_FRAME_INFO *pFrameInfo, DISP_TEST_FRAME frame_info);
HRESULT create_buildin_metadata_frame(AMP_BDTAG_MEMINFO *pmemInfo, char *comp_mdfile,  char *dm_mdfile);
HRESULT create_buildin_tHDR_metadata_frame(AMP_BDTAG_MEMINFO *pmemInfo,
                                           char *pDecoderParams, char *pDynamicMdFile);
HRESULT set_frameinfo_data(
                AMP_BGTAG_FRAME_INFO *pFrameInfo, ENUM_DISP_SRC_FMT iSrcFmt,
                UINT32 width, UINT32 height, ENUM_DISP_DEINT_TYPE progressive,
                ENUM_DISP_INPUT_BIT_DEPTH bit_depth, ENUM_DISP_SRC_ORDER src_order);
UINT32 get_frame_size(ENUM_DISP_SRC_FMT iSrcFmt,
        ENUM_DISP_INPUT_BIT_DEPTH bit_depth, UINT32 width, UINT32 height);
HRESULT create_buildin_frame_secure(AMP_BGTAG_FRAME_INFO *pFrameInfo, ENUM_DISP_SRC_FMT iSrcFmt,
                             UINT32 width, UINT32 height, ENUM_DISP_DEINT_TYPE progressive,
                             ENUM_DISP_INPUT_BIT_DEPTH bit_depth, ENUM_DISP_SRC_ORDER src_order,
                             UINT32 pattern, const char *filename,
                             DISP_RAMP_ATTR *pRampAttr, int frame_type);
#endif
