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
//! \file amp_video_api.h
//!
//! \brief video related functions.
//!
//!
//! Purpose:
//!
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef _AMP_VIDEO_API_H_
#define _AMP_VIDEO_API_H_

#include "com_type.h"

#ifdef __cplusplus
extern "C" {
#endif


/** \typedef DISP_CROP_DESC
    \brief This structure define display crop info.
 */
typedef struct disp_crop_info {
    UINT16  disp_width;        /**< Width (in pixels) of the display window. */
    UINT16  disp_height;       /**< Height (in pixels) of the display window. */
    INT16   disp_left;         /**< Horizontal offset (in pixels) of the display
                                    window relative to the coded picture. */
    INT16   disp_top;          /**< Vertical offset (in pixels) of the display
                                    window relative to the coded picture. */
    UINT32  ps_num_vectors;    /**< Number of available pan-scan vectors. */
    UINT16  ps_width[4];       /**< Width (in pixels) of the pan-scan window. */
    UINT16  ps_height[4];      /**< Height (in pixels) of the pan-scan window. */
    INT32   ps_left[4];        /**< Horizontal offset (in 1/16 pixels) of the pan-scan
                                    window relative to the diplay window. */
    INT32   ps_top[4];         /**< Vertical offset (in 1/16 pixels) of the pan-scan
                                    window relative to the diplay window. */
    UINT32  sar_width;         /**< SAR width. */
    UINT32  sar_height;        /**< SAR height.*/
} DISP_CROP_DESC;

typedef enum {
    AMP_VDEC_BUF_3D_FMT_CBI = 0,   /**< checherboard interleaving */
    AMP_VDEC_BUF_3D_FMT_CI,        /**< column interleaving */
    AMP_VDEC_BUF_3D_FMT_RI,        /**< row interleaving */
    AMP_VDEC_BUF_3D_FMT_SBS,       /**< side by side */
    AMP_VDEC_BUF_3D_FMT_TAB,       /**< top and bottom */
    AMP_VDEC_BUF_3D_FMT_FS,        /**< frame sequential */
    AMP_VDEC_BUF_3D_FMT_FP,        /**< frame packing (for HDMI input frame) */
    AMP_VDEC_BUF_3D_FMT_MAX
} AMP_VDEC_BUF_3D_FORMAT;

typedef struct {
    AMP_VDEC_BUF_3D_FORMAT convert_mode_3d;
    UINT32 arrangement_repetition_period;
    UINT32 cur_poc;
    UINT32 arrangement_id;
    UINT32 arrangement_type;
    UINT32 quincunx_sampling_flag;
    UINT32 content_interpretation_type;
    UINT32 valid;
} FP_INFO;


/**
 * Transfer Characteristic.
 * These values match the ones defined by ITU-T H.265 | ISO/IEC 23008-2 Table E.4.
 */
typedef enum {
    AMP_HDR_TRANS_RESERVED0    = 0,  ///< reserved
    AMP_HDR_TRANS_BT709        = 1,  ///< BT.709 / BT.1361
    AMP_HDR_TRANS_UNSPECIFIED  = 2,  ///< unspecified / SDR
    AMP_HDR_TRANS_RESERVED     = 3,  ///< reserved
    AMP_HDR_TRANS_GAMMA22      = 4,  ///< BT.470 System M / Gamma 22
    AMP_HDR_TRANS_GAMMA28      = 5,  ///< BT.470 System B, G / Gamma 28
    AMP_HDR_TRANS_SMPTE170M    = 6,  ///< BT.601-6 525 / 625 / BT.1358 525 or 625 / BT.1700
    AMP_HDR_TRANS_SMPTE240M    = 7,  ///< SMPTE 240M
    AMP_HDR_TRANS_LINEAR       = 8,  ///< Linear
    AMP_HDR_TRANS_LOG          = 9,  ///< Logarithmic (100:1 range)
    AMP_HDR_TRANS_LOG_SQRT     = 10, ///< Logarithmic (100 * Sqrt(10) : 1 range)
    AMP_HDR_TRANS_IEC61966_2_4 = 11, ///< IEC 61966-2-4
    AMP_HDR_TRANS_BT1361_ECG   = 12, ///< BT.1361 ECG: extended colour gamut system
    AMP_HDR_TRANS_IEC61966_2_1 = 13, ///< IEC 61966-2-1 sRGB / sYCC
    AMP_HDR_TRANS_BT2020_10    = 14, ///< BT.2020 10-bit non-constant
    AMP_HDR_TRANS_BT2020_12    = 15, ///< BT.2020 12-bit constant
    AMP_HDR_TRANS_SMPTE2084    = 16, ///< PQ / SMPTE ST 2084 for 10, 12, 14, 16 bit system
    AMP_HDR_TRANS_SMPTEST2084  = AMP_HDR_TRANS_SMPTE2084,
    AMP_HDR_TRANS_SMPTE428     = 17, ///< SMPTE ST 428-1
    AMP_HDR_TRANS_SMPTEST428_1 = AMP_HDR_TRANS_SMPTE428,
    AMP_HDR_TRANS_ARIB_STD_B67 = 18, ///< HLG / ARIB STD-B67
    AMP_HDR_TRANS_MAX
} AMP_HDR_TRANSFER;


/**
  * Colour primaries.
  * These values match the ones defined by ITU-T H.265 | ISO/IEC 23008-2 Table E.3.
  */
typedef enum {
    AMP_HDR_PRI_RESERVED0   = 0,  ///< reserved
    AMP_HDR_PRI_BT709       = 1,  ///< BT.709 / BT.1361 / IEC 61966-2-4 / SMPTE RP177 Annex B
    AMP_HDR_PRI_UNSPECIFIED = 2,  ///< unspecified
    AMP_HDR_PRI_RESERVED    = 3,  ///< reserved
    AMP_HDR_PRI_BT470M      = 4,  ///< BT.470 M / FCC Title 47 Code of Federal Regulations 73.682 (a)(20)
    AMP_HDR_PRI_BT470BG     = 5,  ///< BT.470 BG / BT.601-6 625 / BT.1358 625 / BT.1700 625 PAL & SECAM
    AMP_HDR_PRI_SMPTE170M   = 6,  ///< SMPTE 170M / BT.601-6 525 / BT.1358 525 / BT1700 NTSC
    AMP_HDR_PRI_SMPTE240M   = 7,  ///< SMPTE 240M, functionally identical to above
    AMP_HDR_PRI_FILM        = 8,  ///< film (colour filters using Illuminant C)
    AMP_HDR_PRI_BT2020      = 9,  ///< BT.2020 / BT.2100
    AMP_HDR_PRI_SMPTE428    = 10, ///< SMPTE ST 428-1 (CIE 1931 XYZ)
    AMP_HDR_PRI_SMPTEST428_1 = AMP_HDR_PRI_SMPTE428,
    AMP_HDR_PRI_SMPTE431    = 11, ///< SMPTE ST 431-2 (2011) / DCI P3
    AMP_HDR_PRI_SMPTE432    = 12, ///< SMPTE ST 432-1 (2010) / P3 D65 / Display P3
    AMP_HDR_PRI_JEDEC_P22   = 22, ///< JEDEC P22 / EBU 3213
    AMP_HDR_PRI_MAX
} AMP_HDR_PRI;

/**
  * Matrix coefficients.
  * These values match the ones defined by ITU-T H.265 | ISO/IEC 23008-2 Table E.5
  */
typedef enum {
    AMP_HDR_MATR_RGB         = 0,  ///< typically used for GBR, also IEC 61966-2-1 (sRGB)
    AMP_HDR_MATR_BT709       = 1,  ///< BT.709 / BT.1361 / IEC 61966-2-4 xvYCC709 / SMPTE RP177 Annex B
    AMP_HDR_MATR_UNSPECIFIED = 2,  ///< unspecified
    AMP_HDR_MATR_RESERVED    = 3,  ///< reserved
    AMP_HDR_MATR_FCC         = 4,  ///< FCC Title 47 Code of Federal Regulations 73.682 (a)(20)
    AMP_HDR_MATR_BT470BG     = 5,  ///< BT.470 / BT.601-6 625 / BT.1358 625 / BT.1700 625 PAL & SECAM / IEC 61966-2-4 xvYCC601
    AMP_HDR_MATR_SMPTE170M   = 6,  ///< SMPTE 170M / BT.601-6 525 / BT.1358 525 / BT.1700 NTSC
    AMP_HDR_MATR_SMPTE240M   = 7,  ///< SMPTE 240M, functionally identical to above
    AMP_HDR_MATR_YCGCO       = 8,  ///< Used by Dirac / VC-2 and H.264 FRext, see ITU-T SG16
    AMP_HDR_MATR_YCOCG       = AMP_HDR_MATR_YCGCO,
    AMP_HDR_MATR_BT2020_NCL  = 9,  ///< BT.2020 non-constant luminance system
    AMP_HDR_MATR_BT2020_CL   = 10, ///< BT.2020 constant luminance system
    AMP_HDR_MATR_SMPTE2085   = 11, ///< SMPTE 2085, Y'D'zD'x
    AMP_HDR_MATR_HROMA_DERIVED_NCL = 12, ///< Chromaticity-derived non-constant luminance system
    AMP_HDR_MATR_CHROMA_DERIVED_CL = 13, ///< Chromaticity-derived constant luminance system
    AMP_HDR_MATR_ICTCP       = 14, ///< BT.2100-0, ICtCp
    AMP_HDR_MATR_MAX
}AMP_HDR_MATRIX;

/**
  * Color range.
  */
typedef enum {
    AMP_HDR_RANGE_UNSPECIFIED,
    AMP_HDR_RANGE_LIMITED,
    AMP_HDR_RANGE_FULL,
    AMP_HDR_RANGE_OTHER,
    AMP_HDR_RANGE_MAX
} AMP_HDR_RANGE;



typedef enum {
    AMP_HDR_TYPE_INVALID,
    AMP_HDR_TYPE_HDR10, // IN
    AMP_HDR_TYPE_HLG,   // IN, only have valid TransferCharacteristics
    AMP_HDR_TYPE_HEVC,  // OUT
    AMP_HDR_TYPE_NONE,  // OUT, only have valid ColorPrimary, ColorRange,
                        // TransferCharacteristics, MatrixCoefficients for compatibility
    AMP_HDR_TYPE_MAX,
} AMP_HDR_TYPE;

typedef struct {
    AMP_HDR_TYPE      type;
    AMP_HDR_TRANSFER  TransferCharacteristics;     /* original type, valid for all types */
    float             PrimaryChromaticityR[2];     /* (x, y) Range 0-1, valid for HEVC, HDR10 types */
    float             PrimaryChromaticityG[2];     /* (x, y) Range 0-1, valid for HEVC, HDR10 types */
    float             PrimaryChromaticityB[2];     /* (x, y) Range 0-1, valid for HEVC, HDR10 types */
    float             WhitePointChromaticity[2];   /* (x, y) Range 0-1, valid for HEVC, HDR10 types */
    float             LuminanceMax;                /* Range 0-999.9999, valid for HEVC, HDR10 types */
    float             LuminanceMin;                /* Range 0-999.9999, valid for HEVC, HDR10 types */
    AMP_HDR_MATRIX    MatrixCoefficients;          /* valid for HEVC, HDR10, AVC types */
    UINT32            MaxCLL;                      /* valid for HDR10 type */
    UINT32            MaxFALL;                     /* valid for HDR10 type */
    AMP_HDR_TRANSFER  AlternativeTC;               /* valid for HEVC, if have alternative TC in SEI */
    UINT8             bIsThdr;                     /* valid for HEVC, if have THDR meta data */
    AMP_HDR_PRI       ColorPrimaries;              /* valid for VP9-Web HDR/HLG/HEVC/AVC */
    AMP_HDR_RANGE     ColorRange;                  /* valid for VP9-Web HDR/HLG/HEVC/AVC */
} AMP_HDR_INFO;



typedef enum {
    VENC_BASE_PROFILE       = 66,       //* Only I,P, CAVLC, no transform8x8, level<=3.0
    VENC_MAIN_PROFILE       = 77,       //* Only I,P and B, CAVLC/CABAC, no transform, level>=3.0
    VENC_EXTENDED_PROFILE   = 88,       //* direct8x8=1, CAVLC, no transform8x8
    VENC_HIGH_PROFILE       = 100,      //* I,P and B, SI, SP, CAVLC/CABAC, transform
} VENC_PROFILE;

typedef enum {
    VENC_1_0_LEVEL = 10,                //* MaxMBs:99   (qcif),     MaxBR=64kbps
    VENC_1_1_LEVEL,                     //* MaxMBs:396  (cif),      MaxBR=192kbps
    VENC_1_2_LEVEL,                     //* MaxMBs:396  (cif),      MaxBR=384kbps
    VENC_1_3_LEVEL,                     //* MaxMBs:396  (cif),      MaxBR=768kbps
    VENC_2_0_LEVEL = 20,                //* MaxMBs:396  (cif),      MaxBR=2000kbps
    VENC_2_1_LEVEL,                     //* MaxMBs:792  (scif),     MaxBR=4000kbps
    VENC_2_2_LEVEL,                     //* MaxMBs:1620 (D1,y576),  MaxBR=4000kbps
    VENC_3_0_LEVEL = 30,                //* MaxMBs:1620 (D1,y576),  MaxBR=10000kbps
    VENC_3_1_LEVEL,                     //* MaxMBs:3600 (720p),     MaxBR=14000kbps
    VENC_3_2_LEVEL,                     //* MaxMBs:5120 (720p+),    MaxBR=20000kbps
    VENC_4_0_LEVEL = 40,                //* MaxMBs:8192 (HD),       MaxBR=20000kbps
    VENC_4_1_LEVEL,                     //* MaxMBs:8192 (HD),       MaxBR=50000kbps
    VENC_4_2_LEVEL,                     //* MaxMBs:8704 (HD),       MaxBR=50000kbps
    VENC_5_0_LEVEL = 50,                //* MaxMBs:22080 (2kx2k),   MaxBR=135000kbps
    VENC_5_1_LEVEL,                     //* MaxMBs:36864 (4kx2k),   MaxBR=240000kbps
} VENC_LEVEL;

#define AMP_DOLBY_VISION_DVS_LINK_MAGIC        0xDEAD

#define AMP_DOLBY_VISION_BL  (1<<0)
#define AMP_DOLBY_VISION_EL  (1<<1)
#define AMP_DOLBY_VISION_RPU (1<<2)


#define AMP_DOLBY_VISION_MODE_GEN(c)  ((AMP_DOLBY_VISION_DVS_LINK_MAGIC << 16) | ((c) & 0xff))
#define AMP_DOLBY_VISION_GET_MAGIC(t)   ((t) >> 16)
#define AMP_DOLBY_VISION_IS_LINK_IN_DVS(t)    \
        (AMP_DOLBY_VISION_GET_MAGIC(t) == AMP_DOLBY_VISION_DVS_LINK_MAGIC)

#ifdef __cplusplus
}
#endif

#endif //_AMP_VIDEO_API_H_
