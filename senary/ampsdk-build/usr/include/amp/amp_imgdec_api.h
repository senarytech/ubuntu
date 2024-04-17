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
\file   imgdec_api.h
\brief Image decoder APIs Definitions.
Purpose:
    Define image decoder APIs and related data structure.
Version, Date   and Author :
    V   1.00,        Aug 20 2013        Lili
                                                            Initial Version.
Note:
*******************************************************************************/
#if !defined(_IMGDEC_API_H_)
#define _IMGDEC_API_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include "amp_types.h"
#include "amp_malloc.h"
#include "amp_client_support.h"

typedef enum {
    FIRST_MV_IMGDEC_SRCTYPE        = 0,
    MV_IMGDEC_SRCTYPE_FILE         = 0,
    MV_IMGDEC_SRCTYPE_SHM          = 1,
    MV_IMGDEC_SRCTYPE_MEM          = 2,
    LAST_MV_IMGDEC_SRCTYPE         = 3
} MV_IMGDEC_SRCTYPE;

typedef enum
{
    MV_IMGDEC_OUTPUT_UYVY = 0,
    MV_IMGDEC_OUTPUT_VYUY,
    MV_IMGDEC_OUTPUT_YUYV,
    MV_IMGDEC_OUTPUT_YVYU,
    MV_IMGDEC_OUTPUT_SPUV,
    MV_IMGDEC_OUTPUT_SPVU,
    MV_IMGDEC_OUTPUT_YUV,
    MV_IMGDEC_OUTPUT_YVU,
    MV_IMGDEC_INPUT_UNSUPPORTED
} MV_IMGDEC_OUTPUT_FORMAT;


/* Data source object for decompression */
typedef struct{
    MV_IMGDEC_SRCTYPE eType; /**< type of the source image */
    FILE  *pSrcFile; /**< if the source image is a file, keep the file handle */
    UINT8 *pSrcBuf; /**< if the source image is in memory, keep the address */
    UINT32 uiSHMKey; /**< if the source image is in shm, keep the offset */
    UINT32 uiSize; /**< size of the source image in bytes */
    UINT32 uiOffset; /**< current readed input offset */
} MV_IMGDEC_SRCINFO;

typedef struct {
    /* Basic description of image. */
    UINT32 uiWidth;   /**< nominal image width  */
    UINT32 uiHeight;  /**< nominal image height */

    /* Decompression processing parameters */
    UINT8 uiHScaleRatio; /**< Horizontal down-scaling ratio
                            0 or 1: no scaling, 2: 1/2, 4: 1/4, 8: 1/8*/
    UINT8 uiVScaleRatio; /**< Vertical down-scaling ratio
                            0 or 1: no scaling, 2: 1/2, 4: 1/4, 8: 1/8*/
    UINT32 uiROIX;       /**< ROI (region of interest) start location in
                            horizontal direction */
    UINT32 uiROIY;       /**< ROI start location in vertical direction */
    UINT32 uiROIW;       /**< ROI width  */
    UINT32 uiROIH;       /**< ROI height */
    UINT32 uiOutputMode; /**< output mode */

    /* Description of actual output image will be returned to application. */
    UINT32 uiOutputW;  /**< scaled image width */
    UINT32 uiOutputH;  /**< scaled image height */
    UINT32 uiAlignedOutputW;  /**< aligned image width */
    UINT32 uiAlignedOutputH;  /**< aligned image height */
    UINT32 uiOutputSize;  /**< Size (in bytes) of the output buffer */
    UINT32 uiStride; /**< Stride (in bytes) of the output buffer */

    BOOLEAN     bIsProgressive;    // 0: baseline jpeg, 1: progressive jpeg
    UINT32      uiImgNum;       // number of images.
    UINT32      uiImgSize[4];
    UINT32      uiImgOffset[4];
} MV_IMGDEC_CONFIG;

/* Initialization of image decoder objects. */
HRESULT MV_IMGDEC_CreateDecoder(MV_IMGDEC_CONFIG *pInfo, HANDLE *pIMGDEC);

/* destroy image decoder objects. */
HRESULT MV_IMGDEC_DestroyDecoder(HANDLE hIMGDEC);

/* Set source image*/
/* Caller is responsible for opening the file before and closing after. */
HRESULT MV_IMGDEC_SetSrc (HANDLE hIMGDEC, FILE * infile);
HRESULT MV_IMGDEC_SetSrcBuffer (HANDLE hIMGDEC,
                                UINT8 *pSrcBuffer,
                                UINT32 uiSize);
HRESULT MV_IMGDEC_SetSrcSHM (HANDLE hIMGDEC,
                             UINT32 uiSHMKey,
                             UINT32 uiSize);

/* Set destination buffer if user wants to use shm allocated by himeself*/
HRESULT MV_IMGDEC_SetDes (HANDLE hIMGDEC,
                          UINT32 uiSHMKey);

HRESULT MV_IMGDEC_SetROI (HANDLE hIMGDEC,
                          UINT32 uiStartX,
                          UINT32 uiStartY,
                          UINT32 uiWidth,
                          UINT32 uiHeight);

HRESULT MV_IMGDEC_SetConfig (HANDLE hIMGDEC,
                             MV_IMGDEC_CONFIG *pInfo);


/* Decoder startup: read start of JPEG datastream to see what's there */
HRESULT MV_IMGDEC_ReadHeader (HANDLE hIMGDEC, MV_IMGDEC_CONFIG *pInfo);

/* Main entry points for decompression */
HRESULT MV_IMGDEC_StartDecode(HANDLE hIMGDEC);

UINT32 MV_IMGDEC_Read (HANDLE hIMGDEC,
                       UINT8 *pOutputBuf,
                       UINT32 uiBufSize);

HRESULT MV_IMGDEC_finish (HANDLE hIMGDEC);
HRESULT MV_IMGDEC_abort (HANDLE hIMGDEC);

#if defined(__cplusplus)
}
#endif  // __cplusplus

#endif // define _IMGDEC_API_H_
