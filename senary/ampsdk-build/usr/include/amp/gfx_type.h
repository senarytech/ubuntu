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

/*******************************************************************************
\file gfx_type.h
\brief Basic Graphics Types Definitions.
Purpose:
  Define basic graphics types.
Version, Date and Author :
  V 1.00,    Dec 21 2012    Yifan
                              Initial Version.
Note:
*******************************************************************************/
#if !defined(_GFX_TYPE_H_)
#define _GFX_TYPE_H_

/************* Header file include ********************************************/
#include "com_type.h"

/************* Macros, type definitions****************************************/
#define PALETTE_COLOR_ENTRIES   256

typedef UINT32 MV_GFX_2D_COLOR;
typedef UINT32 MV_GFX_2D_ROP;

typedef void *(*MV_GFX_2D_DestroyCelCB)(void *);

typedef enum {
    FIRST_MV_2D_BLEND_MODE      = 0,
    MV_GFX_2D_BLEND_CLEAR       = 0,
    MV_GFX_2D_BLEND_SRC         = 1,
    MV_GFX_2D_BLEND_SRC_OVER    = 2,
    MV_GFX_2D_BLEND_DST_OVER    = 3,
    MV_GFX_2D_BLEND_SRC_IN      = 4,
    MV_GFX_2D_BLEND_DST_IN      = 5,
    MV_GFX_2D_BLEND_SRC_OUT     = 6,
    MV_GFX_2D_BLEND_DST_OUT     = 7,
    MV_GFX_2D_BLEND_SRC_ATOP    = 8,
    MV_GFX_2D_BLEND_DST_ATOP    = 9,
    MV_GFX_2D_BLEND_ADD         = 10,
    MV_GFX_2D_BLEND_XOR         = 11,
    MAX_MV_2D_BLEND_MODE        = 12
} MV_GFX_2D_BLEND_MODE;

typedef struct {
    INT32 iX;
    INT32 iY;
} MV_GFX_2D_POINT;

typedef struct tag2DGFX_RECTANGLE {
    INT32 iX; /**< x-coordinate of the upper-left corner of the rectangle. */
    INT32 iY; /**< y-coordinate of the upper-left corner of the rectangle. */
    INT32 iW; /**< width of the rectangle. */
    INT32 iH; /**< height of the rectangle. */
} MV_GFX_2D_RECT;

typedef enum {
    FIRST_MV_GFX_2D_BUFTYPE         = 0x0,
    MV_GFX_2D_BUFTYPE_CACHEDSHM     = 0x0,
    MV_GFX_2D_BUFTYPE_NONCACHEDSHM  = 0x1,
    MV_GFX_2D_BUFTYPE_DUMMY         = 0x2,
    MV_GFX_2D_BUFTYPE_NONE          = 0x3,
    MV_GFX_2D_BUFTYPE_ION           = 0x4,
    MV_GFX_2D_BUFTYPE_LINUX_MEM     = 0x5,
    MAX_MV_GFX_2D_BUFTYPE           = 0x6
} MV_GFX_2D_BUFTYPE;

typedef enum {
    FIRST_MV_GFX_2D_PIX        = 0x00,
    MV_GFX_2D_PIX_A8R8G8B8     = 0x00,
    MV_GFX_2D_PIX_X8R8G8B8     = 0x01,
    MV_GFX_2D_PIX_A4R4G4B4     = 0x02,
    MV_GFX_2D_PIX_X4R4G4B4     = 0x03,
    MV_GFX_2D_PIX_A1R5G5B5     = 0x04,
    MV_GFX_2D_PIX_X1R5G5B5     = 0x05,
    MV_GFX_2D_PIX_R5G6B5       = 0x06,
    MV_GFX_2D_PIX_I8XRGB       = 0x07,
    MV_GFX_2D_PIX_I8XYUV       = 0x08,
    MV_GFX_2D_PIX_MONO         = 0x09,
    MV_GFX_2D_PIX_UYVY         = 0x0a,
    MV_GFX_2D_PIX_YUY2         = 0x0b,
    MV_GFX_2D_PIX_A8           = 0x0c,
    MV_GFX_2D_PIX_AUYV         = 0xD, /** A is the first byte */
    MV_GFX_2D_PIX_AVYU         = 0xE, /** A is the first byte */
    MV_GFX_2D_PIX_VYUA         = 0xF, /** V (Cr) is the first byte */
    MV_GFX_2D_PIX_UYVA         = 0x10, /** U (Cb) is the first byte */
    MV_GFX_2D_PIX_VYUY         = 0x11, /** V (Cr) is the first byte */
    MV_GFX_2D_PIX_YUYV         = 0x12, /** Y is the first byte*/
    MV_GFX_2D_PIX_YVYU         = MV_GFX_2D_PIX_YUY2,
    MV_GFX_2D_PIX_ABGR         = 0x13, /** R is the first byte */
    MV_GFX_2D_PIX_A8B8G8R8     = 0x13,
    MV_GFX_2D_PIX_B8G8R8A8     = 0x14,
    MV_GFX_2D_PIX_R8G8B8A8     = 0x15,
    MV_GFX_2D_PIX_X8B8G8R8     = 0x16,
    MV_GFX_2D_PIX_B8G8R8X8     = 0x17,
    MV_GFX_2D_PIX_R8G8B8X8     = 0x18,
    MV_GFX_2D_PIX_A8B8G8R8_PM  = 0x19,
    MV_GFX_2D_PIX_A8R8G8B8_PM  = 0x1A,
    MAX_MV_GFX_2D_PIX,
} MV_GFX_2D_PIXEL_FORMAT;

typedef enum {
    FIRST_GFX_2D_CHANNEL                = 0,
    MV_GFX_2D_CHANNEL_PRIMARY_VIDEO     = 0,
    MV_GFX_2D_CHANNEL_SECONDARY_VIDEO   = 1,
    MV_GFX_2D_CHANNEL_GRAPHICS          = 2,
    MV_GFX_2D_CHANNEL_GFX               = 2,
    MV_GFX_2D_CHANNEL_CURSOR            = 3,
    MV_GFX_2D_CHANNEL_MOSD              = 4,
    MV_GFX_2D_CHANNEL_SUBTITLE          = 5,
    MV_GFX_2D_CHANNEL_BACKGROUND        = 6,
    MV_GFX_2D_CHANNEL_AUX               = 7,
#if (BERLIN_CHIP_VERSION >= BERLIN_BG2_DTV)
    MV_GFX_2D_CHANNEL_BYPASS            = 8,
    MV_GFX_2D_CHANNEL_SCL               = 9,
#endif
    MV_GFX_2D_CHANNEL_INVALID              ,
    MV_GFX_2D_CHANNEL_NUM               = MV_GFX_2D_CHANNEL_INVALID,
    LAST_GFX_2D_CHANNEL                 = MV_GFX_2D_CHANNEL_INVALID
} MV_GFX_2D_CHANNEL;

/** Alpha mode. */
typedef enum {
    FIRST_MV_GFX_2D_PIXEL_ALPA_MODE     = 0,
    MV_GFX_2D_PIXEL_ALPHA_STRAIGHT      = 0,
    MV_GFX_2D_PIXEL_ALPHA_INVERSED      = 1,
    MAX_MV_GFX_2D_PIXEL_ALPHA_MODE      = 2
} MV_GFX_2D_PIXEL_ALPHA_MODE;

typedef enum {
    FIRST_MV_GFX_2D_GLOBAL_ALPHA_MODE   = 0,
    MV_GFX_2D_GLOBAL_ALPHA_OFF          = 0,
    MV_GFX_2D_GLOBAL_ALPHA_ON           = 1,
    MV_GFX_2D_GLOBAL_ALPHA_SCALE        = 2,
    MAX_MV_GFX_2D_GLOBAL_ALPHA_MODE     = 3
} MV_GFX_2D_GLOBAL_ALPHA_MODE;

typedef enum {
    FIRST_MV_GFX_2D_BLEND_FACTOR_MODE           = 0,
    MV_GFX_2D_BLEND_ZERO                        = 0,
    MV_GFX_2D_BLEND_ONE                         = 1,
    MV_GFX_2D_BLEND_STRAIGHT                    = 2,
    MV_GFX_2D_BLEND_INVERSED                    = 3,
    MV_GFX_2D_BLEND_COLOR                       = 4,
    MV_GFX_2D_BLEND_COLOR_INVERSED              = 5,
    MV_GFX_2D_BLEND_SRC_ALPHA_SATURATED         = 6,
    MV_GFX_2D_BLEND_STRAIGHT_NO_CROSS           = 7,
    MV_GFX_2D_BLEND_INVERSED_NO_CROSS           = 8,
    MV_GFX_2D_BLEND_COLOR_NO_CROSS              = 9,
    MV_GFX_2D_BLEND_COLOR_INVERSED_NO_CROSS     = 10,
    MV_GFX_2D_BLEND_SRC_ALPHA_SATURATED_CROSS   = 11,
    MAX_MV_GFX_2D_BLEND_FACTOR_MODE             = 12
} MV_GFX_2D_BLEND_FACTOR_MODE;

/** Multiply mode. */
typedef enum {
    FIRST_MV_GFX_2D_PIXEL_COLOR_MULTIPLY_MODE   = 0,
    MV_GFX_2D_COLOR_MULTIPLY_DISABLE            = 0,
    MV_GFX_2D_COLOR_MULTIPLY_ENABLE             = 1,
    MAX_MV_GFX_2D_PIXEL_COLOR_MULTIPLY_MODE     = 9
} MV_GFX_2D_PIXEL_COLOR_MULTIPLY_MODE;

typedef enum {
    FIRST_MV_GFX_2D_GLOBAL_COLOR_MULTIPLY_MODE  = 0,
    MV_GFX_2D_GLOBAL_COLOR_MULTIPLY_DISABLE     = 0,
    MV_GFX_2D_GLOBAL_COLOR_MULTIPLY_ALPHA       = 1,
    MV_GFX_2D_GLOBAL_COLOR_MULTIPLY_COLOR       = 2,
    MAX_MV_GFX_2D_GLOBAL_COLOR_MULTIPLY_MODE    = 9
} MV_GFX_2D_GLOBAL_COLOR_MULTIPLY_MODE;

/*
typedef enum {
    FIRST_MV_GFX_2D_PIXEL_COLOR_MODE  = 0,
    MV_GFX_2D_COLOR_STRAIGHT          = 0,
    MV_GFX_2D_COLOR_MULTIPLY          = 1,
    MAX_MV_GFX_2D_PIXEL_COLOR_MODE    = 9
} MV_GFX_2D_PIXEL_COLOR_MODE;
*/

typedef enum {
    FIRST_MV_GFX_2D_ROTATION        = 0,
    MV_GFX_2D_ROTATION_0_DEGREE     = 0,
    MV_GFX_2D_ROTATION_90_DEGREE    = 1,
    MV_GFX_2D_ROTATION_180_DEGREE   = 2,
    MV_GFX_2D_ROTATION_270_DEGREE   = 3,
    MV_GFX_2D_ROTATION_FLIP_X       = 4,
    MV_GFX_2D_ROTATION_FLIP_Y       = 5,
    MAX_MV_GFX_2D_ROTATION          = 9
} MV_GFX_2D_ROTATION;

typedef enum {
    FIRST_MV_GFX_2D_RASTER_SETTING_FLAG               = 0x1,
    /** Alpha blend mode. */
    MV_GFX_2D_RASTER_SETTING_ENABLE_ALPHA_BLEND       = 0x1,
    MV_GFX_2D_RASTER_SETTING_SRC_PIX_ALPHA_MODE       = 0x2,
    MV_GFX_2D_RASTER_SETTING_DST_PIX_ALPHA_MODE       = 0x4,
    MV_GFX_2D_RASTER_SETTING_SRC_GLB_ALPHA_MODE       = 0x8,
    MV_GFX_2D_RASTER_SETTING_DST_GLB_ALPHA_MODE       = 0x10,
    MV_GFX_2D_RASTER_SETTING_SRC_FACTOR_MODE          = 0x20,
    MV_GFX_2D_RASTER_SETTING_DST_FACTOR_MODE          = 0x40,
    MV_GFX_2D_RASTER_SETTING_ALPHA_BLEND_MASK         = 0x7f,

    /** Global color. */
    MV_GFX_2D_RASTER_SETTING_SRC_GLOBAL_COLOR         = 0x80,
    MV_GFX_2D_RASTER_SETTING_DST_GLOBAL_COLOR         = 0x100,

    /** Premultiply. */
    MV_GFX_2D_RASTER_SETTING_SRC_PREMULT_SRC_ALPHA    = 0x200,
    MV_GFX_2D_RASTER_SETTING_DST_PREMULT_DST_ALPHA    = 0X400,
    MV_GFX_2D_RASTER_SETTING_DST_DEMULT_DST_ALPHA     = 0x800,
    MV_GFX_2D_RASTER_SETTING_SRC_PREMULT_GLB_ALPHA    = 0x1000,
    MV_GFX_2D_RASTER_SETTING_PREMULT_MASK             = 0x1e00,

    /** Transparency. */
    MV_GFX_2D_RASTER_SETTING_SRC_TRANSPARENCY         = 0x2000,
    MV_GFX_2D_RASTER_SETTING_DST_TRANSPARENCY         = 0x4000,
    MV_GFX_2D_RASTER_SETTING_PAT_TRANSPARENCY         = 0x8000,
    MV_GFX_2D_RASTER_SETTING_TRANSPARENCY_MASK        = 0xe000,

    /** Color key. */
    MV_GFX_2D_RASTER_SETTING_SRC_COLOR_KEY_RANGE      = 0x10000,
    MV_GFX_2D_RASTER_SETTING_DST_COLOR_KEY_RANGE      = 0x20000,
    MV_GFX_2D_RASTER_SETTING_COLOR_KEY_PROTECT        = 0x40000,

    /** Rotation. */
    MV_GFX_2D_RASTER_SETTING_SRC_ROTATION             = 0x100000,
    MV_GFX_2D_RASTER_SETTING_DST_ROTATION             = 0x200000,
    MAX_MV_GFX_2D_RASTER_SETTING_FLAG                 = 0xffffffff
} MV_GFX_2D_RASTER_SETTING_FLAGS;

typedef enum {
    FIRST_MV_GFX_2D_TRANSPARENCY    = 0,
    MV_GFX_2D_OPAQUE                = 0,
    MV_GFX_2D_KEYED                 = 1,
    MV_GFX_2D_MASKED                = 2,
    MAX_MV_GFX_2D_TRANSPARENCY      = 3
} MV_GFX_2D_TRANSPARENCY;

typedef struct tagGFX_ColorKeyRange {
    UINT32 uiLow;
    UINT32 uiHigh;
} MV_GFX_2D_ColorKeyRange;

typedef struct tagGFX_RasterSettings {
    /** Flags */
    MV_GFX_2D_RASTER_SETTING_FLAGS          eFlags;

    /** Alpha blending. */
    BOOLEAN                                 bEnableAlphaBlending;
    MV_GFX_2D_PIXEL_ALPHA_MODE              eSrcPixAlphaMode;
    MV_GFX_2D_PIXEL_ALPHA_MODE              eDstPixAlphaMode;
    MV_GFX_2D_GLOBAL_ALPHA_MODE             eSrcGlbAlphaMode;
    MV_GFX_2D_GLOBAL_ALPHA_MODE             eDstGlbAlphaMode;
    MV_GFX_2D_BLEND_FACTOR_MODE             eSrcFactorMode;
    MV_GFX_2D_BLEND_FACTOR_MODE             eDstFactorMode;
    UINT32                                  uiSrcGlobalColor;
    UINT32                                  uiDstGlobalColor;

    /** Premultiply/Demultiply. */
    MV_GFX_2D_PIXEL_COLOR_MULTIPLY_MODE     eSrcPremultSrcAlpha;
    MV_GFX_2D_PIXEL_COLOR_MULTIPLY_MODE     eDstPremultDstAlpha;
    MV_GFX_2D_PIXEL_COLOR_MULTIPLY_MODE     eDstDemultDstAlpha;
    MV_GFX_2D_GLOBAL_COLOR_MULTIPLY_MODE    eSrcPremultGlbAlpha;

    /** Transparency mode. */
    MV_GFX_2D_TRANSPARENCY                  eSrcTransparency;
    MV_GFX_2D_TRANSPARENCY                  eDstTransparency;
    MV_GFX_2D_TRANSPARENCY                  ePatTransparency;

    /** Color key. */
    MV_GFX_2D_ColorKeyRange                 srcClrKeyRange;
    MV_GFX_2D_ColorKeyRange                 dstClrKeyRange;
    UINT32                                  uiClorKeyProtect;

    /** For blit only. */
    MV_GFX_2D_ROTATION                      eSrcRotation;
    MV_GFX_2D_ROTATION                      eDstRotation;

    /** Reserved. */
    UINT32                                  uiReserved[8];
} MV_GFX_2D_RasterSettings;

typedef struct tagGFX_CelInfo {
    MV_GFX_2D_BUFTYPE eType; /**< Point out the type of this surface buffer. */
    UINT32 uiShmKey;            /**< Shm offset. */
    UINT32 uiPhysicalAddr;      /**< Physical address. */
    void*  uiVirtualAddr;       /**< Virtual address. */
    UINT32 uiWidth;
    UINT32 uiHeight;
    UINT32 uiSize;
    UINT32 uiStride;
    unsigned long uiBindBDId;
    MV_GFX_2D_PIXEL_FORMAT ePixelFormat;
    BOOL bUseBindBD;
} MV_GFX_2D_CelInfo;

typedef struct tagGFX_CelCallBack {
    MV_GFX_2D_DestroyCelCB callBack;
    void *pCBData;
    BOOLEAN bDeepCopy;
    UINT32 uiCBDataSize;
} MV_GFX_2D_CelCallBack;

typedef enum {
    FIRST_MV_GFX_PM_CLOCK_FLAG          = 0x1,
    MV_GFX_PM_3D_CORE_CLOCK_SOURCE      = 0x1,
    MV_GFX_PM_3D_CORE_CLOCK_DIV         = 0x2,
    MV_GFX_PM_3D_SYS_CLOCK_SOURCE       = 0x4,
    MV_GFX_PM_3D_SYS_CLOCK_DIV          = 0x8,
    MV_GFX_PM_3D_SH_CLOCK_SOURCE        = 0x10,
    MV_GFX_PM_3D_SH_CLOCK_DIV           = 0x20,
    MV_GFX_PM_CLOCK_MASK                = 0x3F,
    MAX_MV_GFX_PM_CLOCK_FLAG            = 0x40,
} MV_GFX_PM_CLOCK_FLAGS;

typedef enum {
    FIRST_MV_GFX_3D_CORE_CLOCK_SOURCE   = 0x0,
    MV_GFX_3D_CORE_CLOCK_AVPLLB4        = 0x0,
    MV_GFX_3D_CORE_CLOCK_AVPLLB5        = 0x1,
    MV_GFX_3D_CORE_CLOCK_AVPLLB6        = 0x2,
    MV_GFX_3D_CORE_CLOCK_AVPLLB7        = 0x3,
    MV_GFX_3D_CORE_CLOCK_SYSPLL         = 0x4,
    MAX_MV_GFX_3D_CORE_CLOCK_SOURCE     = 0x5,
} MV_GFX_3D_CoreClockSource;

typedef enum {
    FIRST_MV_GFX_3D_CORE_CLOCK_DIV      = 0x0,
    MV_GFX_3D_CORE_CLOCK_DIV3           = 0x0,
    MV_GFX_3D_CORE_CLOCK_DIV4           = 0x1,
    MV_GFX_3D_CORE_CLOCK_DIV6           = 0x2,
    MV_GFX_3D_CORE_CLOCK_DIV8           = 0x3,
    MV_GFX_3D_CORE_CLOCK_DIV12          = 0x4,
    MAX_MV_GFX_3D_CORE_CLOCK_DIV        = 0x5,
} MV_GFX_3D_CoreClockDiv;

typedef enum {
    FIRST_MV_GFX_3D_SYS_CLOCK_SOURCE    = 0x0,
    MV_GFX_3D_SYS_CLOCK_AVPLLB4         = 0x0,
    MV_GFX_3D_SYS_CLOCK_AVPLLB5         = 0x1,
    MV_GFX_3D_SYS_CLOCK_AVPLLB6         = 0x2,
    MV_GFX_3D_SYS_CLOCK_AVPLLB7         = 0x3,
    MV_GFX_3D_SYS_CLOCK_SYSPLL          = 0x4,
    MAX_MV_GFX_3D_SYS_CLOCK_SOURCE      = 0x5,
} MV_GFX_3D_SysClockSource;

typedef enum {
    FIRST_MV_GFX_3D_SYS_CLOCK_DIV       = 0x0,
    MV_GFX_3D_SYS_CLOCK_DIV2            = 0x0,
    MV_GFX_3D_SYS_CLOCK_DIV3            = 0x1,
    MV_GFX_3D_SYS_CLOCK_DIV4            = 0x2,
    MV_GFX_3D_SYS_CLOCK_DIV6            = 0x3,
    MV_GFX_3D_SYS_CLOCK_DIV8            = 0x4,
    MV_GFX_3D_SYS_CLOCK_DIV12           = 0x5,
    MAX_MV_GFX_3D_SYS_CLOCK_DIV         = 0x6,
} MV_GFX_3D_SysClockDiv;

typedef enum {
    FIRST_MV_GFX_3D_SH_CLOCK_SOURCE     = 0x0,
    MV_GFX_3D_SH_CLOCK_AVPLLB4          = 0x0,
    MV_GFX_3D_SH_CLOCK_AVPLLB5          = 0x1,
    MV_GFX_3D_SH_CLOCK_AVPLLB6          = 0x2,
    MV_GFX_3D_SH_CLOCK_AVPLLB7          = 0x3,
    MV_GFX_3D_SH_CLOCK_SYSPLL           = 0x4,
    MAX_MV_GFX_3D_SH_CLOCK_SOURCE       = 0x5,
} MV_GFX_3D_ShClockSource;

typedef enum {
    FIRST_MV_GFX_3D_SH_CLOCK_DIV      = 0x0,
    MV_GFX_3D_SH_CLOCK_DIV3           = 0x0,
    MV_GFX_3D_SH_CLOCK_DIV4           = 0x1,
    MV_GFX_3D_SH_CLOCK_DIV6           = 0x2,
    MV_GFX_3D_SH_CLOCK_DIV8           = 0x3,
    MV_GFX_3D_SH_CLOCK_DIV12          = 0x4,
    MAX_MV_GFX_3D_SH_CLOCK_DIV        = 0x5,
} MV_GFX_3D_ShClockDiv;

typedef struct tagGFX_PM_ClockSettings {
    MV_GFX_PM_CLOCK_FLAGS       eFlags;
    MV_GFX_3D_CoreClockSource   e3DCoreSource;
    MV_GFX_3D_CoreClockDiv      e3DCoreDiv;
    MV_GFX_3D_SysClockSource    e3DSysSource;
    MV_GFX_3D_SysClockDiv       e3DSysDiv;
    MV_GFX_3D_ShClockSource     e3DShSource;
    MV_GFX_3D_ShClockDiv        e3DShDiv;
} MV_GFX_PM_ClockSettings;
#endif


