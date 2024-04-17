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

#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "disp_test_framebuffer.h"
#include "disp_test_prv.h"
#include <errno.h>
#include "rpu_ext_config.h"

//Enable following macro to dump the initial part of frame buffer content for debug purpose
//#define ENABLE_FRAMEBUFFER_DUMP
//Enable the following macro to allow user defined pixel value to be filled into framebuffer
//#define ENABLE_USER_DEFINED_PIXEL_VALUE

typedef UINT32 (*GenePatternFnPtr)(UINT32, UINT32, UINT32, UINT32);
GenePatternFnPtr gGenePatternFn;
static DISP_RAMP_ATTR gRampAttr={0x80000000, 4, RAMP_COLOR_RED};
ENUM_DISP_SRC_ORDER gSrcOrder;

static HRESULT create_page_table(VOID *PageTableStartAddr, UINT32 Totalpages, VOID *FrameAddress);
#ifdef ENABLE_COLOR_BAR_PATTERN
#define MAX_COLOR_RAMP_START_COLOR  8
#define MAX_COLOR_BAR_COLORS        8
#define CUSTOM_COLOR_BAR_WHITE      0
#define CUSTOM_COLOR_BAR_BLACK      (MAX_COLOR_BAR_COLORS-1)
#define MMU_PM_BIT_POSITION         (27)
#define MMU_CFG_PM_ENABLE           (1)
#define MMU_CFG_VM_ENABLE           (1)
#define MMU_PAGE_SIZE               (4096)
#define MMU_STRIDE_FACTOR           (4)
/* This Macro give pages for the frameSize.*/
#define GET_PAGES(frameSize) ((frameSize+MMU_PAGE_SIZE-1)/MMU_PAGE_SIZE)
#define GET_PAGE_TABLE_SIZE(Pages) (MMU_PAGE_SIZE * GET_PAGES((Pages * sizeof(MMU_ENTRY))))
/*Have single variable to store alpha value for ARGB32 format
 *   So that we can have same alpha used b/w draw/redraw/rect functions
 *   alpha selected for half transperency */
#define  ARGB32_FIXED_ALPHA_VALUE 0x80

static unsigned char color_yuv_values[MAX_COLOR_BAR_COLORS][3] = {
    {235, 128, 128,}, //white      = 80ffffff
    {210,  16, 146,}, //yellow     = 80fff100
    {170, 166,  16,}, //light blue = 8000e7ff
    {145,  54,  34,}, //green      = 8000d900
    {107, 202, 221,}, //pink       = 80ff29ff
    { 82,  90, 240,}, //red        = 80ff1a00
    { 41, 240, 110,}, //dark blue  = 80000fff
    { 16, 128, 128,}, //black      = 80000000
};

static unsigned int color_rgb_values[MAX_COLOR_BAR_COLORS+2];
static unsigned int multi_color_ramp_mask_values[MAX_COLOR_RAMP_START_COLOR];
static unsigned int nColorMask = 1;

double clip(double val){
    val = ceil(val);
    if(val>255.0) val = 255.0;
    else if(val<0.0) val = 0.0;
    return val;
}



struct sl_hdr1_metadata_variables {
  int tmInputSignalBlackLevelOffset;
  int tmInputSignalWhiteLevelOffset;
  int shadowGain;
  int highlightGain;
  int midToneWidthAdjFactor;
  int tmOutputFineTuningNumVal;
  int tmOutputFineTuningX[10];
  int tmOutputFineTuningY[10];
  int saturationGainNumVal;
  int saturationGainX[6];
  int saturationGainY[6];
};

struct sl_hdr1_metadata_tables {
  int luminanceMappingNumVal;
  int luminanceMappingX[33];
  int luminanceMappingY[33];
  int colourCorrectionNumVal;
  int colourCorrectionX[33];
  int colourCorrectionY[33];
  int chromaToLumaInjectionMuA;
  int chromaToLumaInjectionMuB;
};

typedef struct sl_hdr1_metadata {
  int specVersion;
  int payloadMode;
  int hdrPicColourSpace;
  int hdrMasterDisplayColourSpace;
  int hdrMasterDisplayMaxLuminance;
  int hdrMasterDisplayMinLuminance;
  int sdrPicColourSpace;
  int sdrMasterDisplayColourSpace;
  union
  {
    struct sl_hdr1_metadata_variables variables;
    struct sl_hdr1_metadata_tables tables;
  } u;
}THDR_MD;

typedef struct __mmu_entry__ {
    UINT32 flags:12;
    UINT32 pageAddress:20;
}MMU_ENTRY;


unsigned int get_rgb_from_yuv(int Y, int U, int V) {
    double R, G, B;
    unsigned int pix_val = 0;
    R = (1.164 * (Y-16)) + (0.0 * (U-128)) + (1.793 * (V-128));
    G = (1.164 * (Y-16)) + (-0.213 * (U-128)) + (-0.533 * (V-128));
    B = (1.164 * (Y-16)) + (2.112 * (U-128)) + (0.0 * (V-128));
    R = clip(R);
    B = clip(B);
    G = clip(G);

    pix_val = (int)R << 16;
    pix_val |= (int)G << 8;
    pix_val |= (int)B << 0;

    return pix_val;
}

inline static void getRGBfromYUV_values(void) {
    int i;
    unsigned int colorMask;

    for(i=0;i<MAX_COLOR_BAR_COLORS;i++) {
        color_rgb_values[i] = get_rgb_from_yuv( color_yuv_values[i][0], color_yuv_values[i][1], color_yuv_values[i][2]);
        color_rgb_values[i] |= (ARGB32_FIXED_ALPHA_VALUE<<24);
        //printf("Color : %d = %x\n",i, color_rgb_values[i]);
    }

    colorMask = gRampAttr.rampColor;
    i = 0;
    while(colorMask) {
        multi_color_ramp_mask_values[i++] = colorMask & 0xF;
        //printf("Color mask(%x) => %d : %x\n",colorMask, i, multi_color_ramp_mask_values[i-1]);
        colorMask >>= 4;
    }
    nColorMask = i;
}

// generate ver color bar pixel
inline static UINT32 genColorBarVerticle(UINT32 width, UINT32 height, UINT32 x, UINT32 y)
{
    MV_UNUSED(height);
    int max_colors = MAX_COLOR_BAR_COLORS-1;
    UINT32 v = (x  / (width / max_colors)) ;
    return v;
}
// generate hor color bar pixel
inline static UINT32 genColorBarHorizontal(UINT32 width, UINT32 height, UINT32 x, UINT32 y)
{
    int max_colors = MAX_COLOR_BAR_COLORS-1;
    UINT32 v = (y  / (height / max_colors)) ;
    return v;
}
// generate CUSTOM color bar pixel
inline static UINT32 genColorBarCustom(UINT32 width, UINT32 height, UINT32 x, UINT32 y)
{
    int max_colors = MAX_COLOR_BAR_COLORS-1;
    UINT32 v;

    if(y<height/2){
        v = (x  / (width / max_colors)) ;
    } else {
        v = x%4 == 0 ? CUSTOM_COLOR_BAR_BLACK : CUSTOM_COLOR_BAR_WHITE;
    }

    return v;
}

inline static UINT32 genColorRamp(UINT32 width, UINT32 height, UINT32 x, UINT32 y)
{
    int i;
    //UINT32 nColors = 256/gRampAttr.rampStep;
    UINT8 *pARGB_bytes = (UINT8*)&color_rgb_values[MAX_COLOR_BAR_COLORS];
    UINT32 nBars = height/nColorMask;
    UINT32 colorMask = multi_color_ramp_mask_values[y/nBars];

    //Generate a vertical color ramp
    color_rgb_values[MAX_COLOR_BAR_COLORS] = gRampAttr.rampStartColor;

    pARGB_bytes[3] = ARGB32_FIXED_ALPHA_VALUE;
    //pARGB_bytes[2-gRampAttr.rampColor] += x/gRampAttr.rampStep;
    for(i=0;i<RAMP_COLOR_MAX;i++) {
        if(colorMask & (1<<i)) {
            pARGB_bytes[2-i] += x/gRampAttr.rampStep;
        }
    }

    return MAX_COLOR_BAR_COLORS;
}
#endif //ENABLE_COLOR_BAR_PATTERN

UINT32 gTileGridTbl[][3] = {
    [DISP_SRCFMT_YUV420SP_TILED_MODE0] = {4, 4},
    [DISP_SRCFMT_YUV420SP_TILED_MODE1] = {4, 6},
    [DISP_SRCFMT_YUV420SP_TILED_V4H8] = {4, 8},
};

static int read_md_dm(dm_metadata_t *p_md, FILE* fp)
{
    int size, i;
    int ext_len;

    size = (int) fread(p_md, 1, sizeof(p_md->base), fp);

    if(size==0) return 0;
    for( i = 0; i < p_md->base.num_ext_blocks; i++ ) {
        size += (int) fread(&p_md->ext[i], 1, 5, fp);
        ext_len =  (int) ((p_md->ext[i].ext_block_length_byte3<<24) +
                (p_md->ext[i].ext_block_length_byte2<<16) +
                (p_md->ext[i].ext_block_length_byte1<< 8) +
                p_md->ext[i].ext_block_length_byte0);
        size += (int) fread(&(p_md->ext[i].l), 1, (size_t)ext_len, fp);
    }
    return size;
}

void vpp_read_md_file(void *virt_addr, unsigned int size, const char *filename, int rdFlag)
{
    FILE *fp;

    fp = fopen(filename,"rb");
    if(fp == NULL) {
        printf("File open error (%s) and erro no is %s \n",filename, strerror(errno) );
        return;
    }
    if(rdFlag) {
        printf("Reading mdfile (%s) of size (%u) into location (%p)\n", filename, size, virt_addr);
        read_md_dm(virt_addr, fp);
    } else {
        //printf("Writing file (%s) of size (%x) from location (%x)\n", filename, size, virt_addr);
        //fwrite(virt_addr, size, 1, fp);
    }

    fclose(fp);
}

void vpp_read_write_file(void *virt_addr, unsigned int size, const char *filename, int rdFlag) {

    FILE *fp;

    fp = fopen(filename,"rb");
    if(fp == NULL) {
        printf("File open error (%s) and erro no is %s \n",filename, strerror(errno) );
        return;
    }
    if(!size) {
        if(!rdFlag) {
            printf("Invalid size specified : %x\n",size);
            fclose(fp);
            return;
        }
        fseek(fp, 0L, SEEK_END);
        size = ftell(fp);
        rewind(fp);
    }

    if(rdFlag) {
        printf("Reading file (%s) of size (%u) into location (%p)\n", filename, size, virt_addr);
        size_t rsize = fread(virt_addr, size, 1, fp);
        if (rsize != size) {
            printf("read data %u size, but want %u\n", rsize, size);
        }
    } else {
        printf("Writing file (%s) of size (%u) from location (%p)\n", filename, size, virt_addr);
        fwrite(virt_addr, size, 1, fp);
    }

    fclose(fp);
}

// move this function to shared
static UINT32 get_frame_bitsperpixel(ENUM_DISP_SRC_FMT iSrcFmt, ENUM_DISP_INPUT_BIT_DEPTH bit_depth)
{
    int bits_per_pixel = 0;
    int bitdepth = 0;

    if (bit_depth == DISP_INPUT_BIT_DEPTH_8BIT) {
        bitdepth = 8;
    } else {
        bitdepth = 10;
    }

    switch (iSrcFmt) {
    case DISP_SRCFMT_LUT8:
        bits_per_pixel = bitdepth;
        break;
    case DISP_SRCFMT_YUV422:
    case DISP_SRCFMT_ARGB1555:
    case DISP_SRCFMT_RGB565:
        bits_per_pixel = bitdepth * 2;
        break;
    case DISP_SRCFMT_YUV420:
    case DISP_SRCFMT_YUV420SP:
    case DISP_SRCFMT_YUV420SP_TILED_MODE0:
    case DISP_SRCFMT_YUV420SP_TILED_MODE1:
    case DISP_SRCFMT_YUV420SP_TILED_V4H8:
        bits_per_pixel = bitdepth / 2 * 3;
        break;
    case DISP_SRCFMT_ARGB32:
    case DISP_SRCFMT_ARGB32_PM:
    case DISP_SRCFMT_XRGB32:
    case DISP_SRCFMT_ARGB4444:
    case DISP_SRCFMT_AYUV32:
    case DISP_SRCFMT_ARGB32_4K:
        bits_per_pixel = bitdepth * 4;
        break;
    case DISP_SRCFMT_ARGB24:
    case DISP_SRCFMT_YUV444:
    case DISP_SRCFMT_RGB444:
        bits_per_pixel = bitdepth * 3;
        break;
    default:
        bits_per_pixel = 16;
        break;
    };

    CMDLINE_PRINT("Bitsperpixel: %d\n", bits_per_pixel);
    return bits_per_pixel;
}

/*Fix me*/
static UINT32 get_frame_stride(ENUM_DISP_SRC_FMT iSrcFmt,
            ENUM_DISP_INPUT_BIT_DEPTH bit_depth, UINT32 width)
{
    int bits_per_pixel = 0;
    int buf_stride = 0;
    float tail_frac=1.0;

    switch (iSrcFmt) {
        case DISP_SRCFMT_YUV420SP:
            bit_depth = bit_depth == 0 ? 8 : 10;
            buf_stride = width * bit_depth / 8;
            CMDLINE_PRINT("YUV420SP buf_stride:%d\n", buf_stride);
            break;
        case DISP_SRCFMT_YUV420SP_TILED_MODE0:
        case DISP_SRCFMT_YUV420SP_TILED_MODE1:
        case DISP_SRCFMT_YUV420SP_TILED_V4H8:
            if (bit_depth == DISP_INPUT_BIT_DEPTH_10BIT &&
                (iSrcFmt == DISP_SRCFMT_YUV420SP_TILED_MODE0 ||
                       iSrcFmt == DISP_SRCFMT_YUV420SP_TILED_MODE1))
                tail_frac = 16.0/15.0;

            bit_depth = bit_depth == 0 ? 8 : 10;
            //buf_stride = width * gTileGridTbl[iSrcFmt][1] * bit_depth / 8;
            buf_stride = tail_frac*width * bit_depth / 8 * 4;
            CMDLINE_PRINT("YUV420SP mode %d buf_stride:%d\n", \
                          iSrcFmt, buf_stride);
            break;
        default:
            bits_per_pixel = get_frame_bitsperpixel(iSrcFmt, bit_depth);
            buf_stride = width*bits_per_pixel/8;
    }

    CMDLINE_PRINT("BufStride: %d\n", buf_stride);
    return buf_stride;
}

UINT32 get_frame_size(ENUM_DISP_SRC_FMT iSrcFmt,
        ENUM_DISP_INPUT_BIT_DEPTH bit_depth, UINT32 width, UINT32 height)
{
    int size = 0;
    int bits_per_pixel = 0;
    float tail_frac=1.0;

    if (bit_depth == DISP_INPUT_BIT_DEPTH_10BIT &&
        (iSrcFmt == DISP_SRCFMT_YUV420SP_TILED_MODE0 ||
               iSrcFmt == DISP_SRCFMT_YUV420SP_TILED_MODE1))
        tail_frac = 16.0/15.0;

    bits_per_pixel = get_frame_bitsperpixel(iSrcFmt, bit_depth);
    size = tail_frac*width*height*bits_per_pixel/8;

    return size;
}

/*Fixme: Since AMP don't have standard frameinfo export, we need sync it here*/
static UINT32 get_src_fmt_definition_during_vpp(ENUM_DISP_SRC_FMT iSrcFmt)
{
    unsigned int src_fmt = SRCFMT_YUV422; // default to 422

    switch (iSrcFmt) {
        case DISP_SRCFMT_YUV422:
            src_fmt = SRCFMT_YUV422;
            break;
        case DISP_SRCFMT_ARGB32:
            src_fmt = SRCFMT_ARGB32;
            break;
        case DISP_SRCFMT_ARGB32_PM:
            src_fmt = SRCFMT_ARGB32_PM;
            break;
        case DISP_SRCFMT_XRGB32:
            src_fmt = SRCFMT_XRGB32;
            break;
        case DISP_SRCFMT_ARGB24:
            src_fmt = SRCFMT_ARGB24;
            break;
        case DISP_SRCFMT_ARGB4444:
            src_fmt = SRCFMT_ARGB4444;
            break;
        case DISP_SRCFMT_RGB565:
            src_fmt = SRCFMT_RGB565;
            break;
        case DISP_SRCFMT_ARGB1555:
            src_fmt = SRCFMT_ARGB1555;
            break;
        case DISP_SRCFMT_LUT8:
            src_fmt = SRCFMT_LUT8;
            break;
        case DISP_SRCFMT_AYUV32:
            src_fmt = SRCFMT_AYUV32;
            break;
        case DISP_SRCFMT_YUV420:
            src_fmt = SRCFMT_YUV420;
            break;
        case DISP_SRCFMT_YUV420SP:
            src_fmt = SRCFMT_YUV420SP;
            break;
        case DISP_SRCFMT_YUV444:
            src_fmt = SRCFMT_YUV444;
            break;
        case DISP_SRCFMT_RGB444:
            src_fmt = SRCFMT_RGB444;
            break;
        case DISP_SRCFMT_ARGB32_4K:
            src_fmt = SRCFMT_ARGB32_4K;
            break;
        case DISP_SRCFMT_YUV420SP_TILED_MODE0:
            src_fmt = 13;
            break;
        case DISP_SRCFMT_YUV420SP_TILED_MODE1:
            src_fmt = 14;
            break;
        case DISP_SRCFMT_YUV420SP_TILED_V4H8:
            src_fmt = 17;
            break;
        default:
            break;
    };

    return src_fmt;
}

HRESULT set_frameinfo_data(
                AMP_BGTAG_FRAME_INFO *pFrameInfo, ENUM_DISP_SRC_FMT iSrcFmt,
                UINT32 width, UINT32 height, ENUM_DISP_DEINT_TYPE progressive,
                ENUM_DISP_INPUT_BIT_DEPTH bit_depth, ENUM_DISP_SRC_ORDER src_order)
{
    UINT32 width_adj;
    double bit_frac = 1.0;
    /**
    * number of bytes per pixel
    */
    pFrameInfo->uiBpp = 2; //get_frame_bytesperpixel(iSrcFmt, bit_depth);
    pFrameInfo->uiSrcFmt = get_src_fmt_definition_during_vpp(iSrcFmt);
    pFrameInfo->uiOrder = src_order;
    pFrameInfo->uiContentW = width;
    pFrameInfo->uiContentH = height;
    pFrameInfo->iActiveOff = 0;

    width_adj = width+7;
    width_adj /= 8;
    width_adj *= 8;

    pFrameInfo->uiBufStride = get_frame_stride(iSrcFmt,bit_depth,\
                                            width_adj);

    pFrameInfo->iActiveOff += 0;

    pFrameInfo->uiIsFs = progressive;
    pFrameInfo->uiFrNum = 60000;
    pFrameInfo->uiFrDen = 1001;
    pFrameInfo->uiActiveW = width;
    pFrameInfo->uiActiveH = height;
    pFrameInfo->iActiveX = 0;
    pFrameInfo->iActiveY = 0;
    pFrameInfo->uiPar = 0x10001;


#if (BERLIN_CHIP_VERSION >= BERLIN_BG4_CD && BERLIN_CHIP_VERSION != BERLIN_BG4_DTV)
    bit_frac = 1.0;
    if (bit_depth == DISP_INPUT_BIT_DEPTH_8BIT) {
        bit_frac = 1.0;
    } else if (bit_depth == DISP_INPUT_BIT_DEPTH_10BIT) {
        bit_frac = 1.25;
    }

    if ((DISP_SRCFMT_YUV422 == iSrcFmt) || (DISP_SRCFMT_YUV444 == iSrcFmt)) {
         pFrameInfo->uiBitsPerPixel = 8 * bit_frac;
    }

    if(DISP_SRCFMT_RGB565  == iSrcFmt){
         pFrameInfo->uiBitsPerPixel = 16 * bit_frac;
    } else if (DISP_SRCFMT_ARGB32 == iSrcFmt || DISP_SRCFMT_ARGB32_PM == iSrcFmt || DISP_SRCFMT_XRGB32 == iSrcFmt){
         pFrameInfo->uiBitsPerPixel = 32 * bit_frac;
    } else if (DISP_SRCFMT_YUV420SP == iSrcFmt ||
    DISP_SRCFMT_YUV420SP_TILED_MODE0 == iSrcFmt ||
    DISP_SRCFMT_YUV420SP_TILED_MODE1 == iSrcFmt ||
    DISP_SRCFMT_YUV420SP_TILED_V4H8 == iSrcFmt){
         pFrameInfo->uiBitsPerPixel = 8 * bit_frac;
    }
#else
    if (bit_depth == DISP_INPUT_BIT_DEPTH_8BIT) {
        pFrameInfo->uiBitsPerPixel = 8;
    } else if (bit_depth == DISP_INPUT_BIT_DEPTH_10BIT) {
        pFrameInfo->uiBitsPerPixel = 10;
    } else {
        CMDLINE_PRINT("set_frameinfo_data unknown bitdepth:%d\n", bit_depth);
    }
#endif
    CMDLINE_PRINT("pFrameInfo->uiBitsPerPixel %d\n", pFrameInfo->uiBitsPerPixel);

    if (iSrcFmt == DISP_SRCFMT_YUV420SP) {
        //Start Address of UV data in 420SP format
        pFrameInfo->uiPbufStartUV = ((UINT32)pFrameInfo->pBufStart) +
            pFrameInfo->uiBufStride * pFrameInfo->uiContentH;
        //stride of UV data in 420SP format
        pFrameInfo->uiBufStrideUV = pFrameInfo->uiBufStride;
    } else if (iSrcFmt == DISP_SRCFMT_YUV420SP_TILED_MODE0 ||
               iSrcFmt == DISP_SRCFMT_YUV420SP_TILED_MODE1 ||
               iSrcFmt == DISP_SRCFMT_YUV420SP_TILED_V4H8) {
        float tail_frac=1.0;
        if(bit_depth == DISP_INPUT_BIT_DEPTH_10BIT)
            tail_frac = 16.0/15.0;
        pFrameInfo->uiPbufStartUV = ((UINT32)pFrameInfo->pBufStart) +
        #if 0
            pFrameInfo->uiBufStride * pFrameInfo->uiContentH / gTileGridTbl[iSrcFmt][1];
        #else
            width*height*bit_frac*tail_frac;
        #endif
        pFrameInfo->uiBufStrideUV = pFrameInfo->uiBufStride;
    } else {
        pFrameInfo->uiPbufStartUV = 0;
        pFrameInfo->uiBufStrideUV = 0;
    }
    CMDLINE_PRINT("Y: %p UV: %x YStride: %d UVStride: %d\n, uiBitsPerPixel :%d",
                  pFrameInfo->pBufStart, pFrameInfo->uiPbufStartUV,
                  pFrameInfo->uiBufStride, pFrameInfo->uiBufStrideUV, pFrameInfo->uiBitsPerPixel);

    pFrameInfo->uiClutKey = -1;
    pFrameInfo->uiClutStart = 0;
    pFrameInfo->uiClutLength = 255;

    return SUCCESS;
}

#define SHIFTBIN(x, y, n)  ((x << n) | y)
// color table
// actually, Y -> [0, 1) or [0, 0xff]
// U/V -> [-0.5, 0.5) or [0, 0xff]
// so U/V is zero, means its value is 0x80 in binary format
// white, black
#define WHITE8          (SHIFTBIN(0xff, 0x80, 8))
#define BLACK8          (SHIFTBIN(0x00, 0x80, 8))
UINT32 YUV8Tbl[] = {WHITE8, BLACK8};
UINT32 YUV8bit2Tbl[] = {
    SHIFTBIN(BLACK8, BLACK8, 16),
    SHIFTBIN(BLACK8, WHITE8, 16),
    SHIFTBIN(WHITE8, BLACK8, 16),
    SHIFTBIN(WHITE8, WHITE8, 16)
};
#define WHITE10       (SHIFTBIN(0x3ffl, 0x200l, 10))
#define BLACK10       (SHIFTBIN(0x000l, 0x200l, 10))
UINT32 YUV10Tbl[] = {WHITE10, BLACK10};
UINT64 YUV10bit2Tbl[] = {
    SHIFTBIN((UINT64)BLACK10, BLACK10, 20),
    SHIFTBIN((UINT64)BLACK10, WHITE10, 20),
    SHIFTBIN((UINT64)WHITE10, BLACK10, 20),
    SHIFTBIN((UINT64)WHITE10, WHITE10, 20)
};
// #define WHITE4208       (SHIFTBIN(0xff, 0x08, 4))
// #define BLACK4208       (SHIFTBIN(0x00, 0x08, 4))
    // SHIFTBIN(BLACK4208, BLACK4208, 12),
    // SHIFTBIN(BLACK4208, WHITE4208, 12),
    // SHIFTBIN(WHITE4208, BLACK4208, 12),
    // SHIFTBIN(WHITE4208, WHITE4208, 12),
UINT32 YUV4208bit2Tbl[] = {
    0x008000,
    0x008ff0,
    0xff8000,
    0xff8ff0,
};

UINT32 YUV444_8bit_Tbl[] = {
    /* Color order 0xYUV */
    0xff8080, /* White */
    0x008080, /* Black */
};

UINT32 YUV444_10bit_Tbl[] = {
    /* Color order 0xYUV */
    0x3FC80200, /* White */
    0x00080200, /* Black */
};

UINT32 VYU444_8bit_Tbl[] = {
    /* Color order 0xVYU */
    0x80ff80, /* White */
    0x800080, /* Black */
};

UINT32 VYU444_10bit_Tbl[] = {
    /* Color order 0xVYU */
    0x200FF200, /* White */
    0x20000200, /* Black */
};


#define GET_YU8(v)  SHIFTBIN(color_yuv_values[v][0], color_yuv_values[v][1], 8)
#define GET_YV8(v)  SHIFTBIN(color_yuv_values[v][0], color_yuv_values[v][2], 8)
#define GET_YUV8(v) SHIFTBIN(GET_YU8(v), GET_YV8(v), 16)
#define GET_YU10(v) SHIFTBIN((color_yuv_values[v][0]<<2), (color_yuv_values[v][1]<<2), 10)
#define GET_YV10(v) SHIFTBIN((color_yuv_values[v][0]<<2), (color_yuv_values[v][2]<<2), 10)
#define GET_YUV10(v) SHIFTBIN((GET_YU10(v)*0x1l), (GET_YV10(v)*0x1l, 20)

    //
// generate ray tracing sphere pixel
// cx, cy is center of sphere
inline static UINT32 genRay(UINT32 width, UINT32 height, UINT32 x, UINT32 y,
                            UINT32 cx, UINT32 cy, UINT32 radius)
{
    // tricky on var type, NOT change
    UINT32 gridsize = height / 16;
    INT32 iRadius = (INT32)radius;
    UINT32 radius2 = radius * radius;
    double N = 3;

    INT32 dx = x - cx;
    INT32 dy = y - cy;

    if (abs(dx) < iRadius && abs(dy) < iRadius) {
        UINT32 r2 = dx * dx + dy * dy;
        if (r2 < radius2) {
            double s = sqrt(r2) / radius;
            double k = sqrt(1. - s * s) * sqrt(1. - (s / N) * (s / N)) + s * s / N;
            k *= N;
            dx = dx / k + cx;
            dy = dy / k + cy;
            x = (UINT32)dx;
            y = (UINT32)dy;
        }
    }

    UINT32 v = (x / gridsize + y / gridsize) % 2;
    return v;
}

typedef struct RayCentreCfg {
    BOOL autocfg;
    UINT32 cx;
    UINT32 cy;
    UINT32 radius;
} RayCentreCfg;

RayCentreCfg gRayCfg = {
    .autocfg = true,
    .cx = 1920 / 2,
    .cy = 1080 / 2,
    .radius = 1080 / 4,
};

#if 0
static void setRayAuto()
{
    gRayCfg.autocfg = true;
}
static void setRayCentreCfg(UINT32 cx, UINT32 cy, UINT32 radius)
{
    gRayCfg.autocfg = false;
    gRayCfg.cx = cx;
    gRayCfg.cy = cy;
    gRayCfg.radius = radius;

}
#endif
inline static UINT32 genRayCentre(UINT32 width, UINT32 height, UINT32 x, UINT32 y)
{
    if (gRayCfg.autocfg == true) {
        // place sphere in circle
        return genRay(width, height, x, y, width / 2, height / 2, height / 4);
    } else {
        return genRay(width, height, x, y, gRayCfg.cx, gRayCfg.cy, gRayCfg.radius);
    }
}

// generate chess pixel
inline static UINT32 genChess(UINT32 width, UINT32 height, UINT32 x, UINT32 y)
{
    UINT32 grid = height / 16;;
    UINT32 v = (x  / grid + y / grid + 1) % 2;
    return v;
}

typedef UINT32 (*GenePatternFnPtr)(UINT32, UINT32, UINT32, UINT32);
GenePatternFnPtr genePatternTbl[] = {
   [PATTERN_CHESS] = genChess,
   [PATTERN_SPHERE_ON_CHESS] = genRayCentre,
#ifdef ENABLE_COLOR_BAR_PATTERN
    [PATTERN_COLOR_BAR_VER] = genColorBarVerticle,
    [PATTERN_COLOR_BAR_HOR] = genColorBarHorizontal,
    [PATTERN_COLOR_BAR_CUSTOM] = genColorBarCustom,
    [PATTERN_COLOR_RAMP] = genColorRamp,
#endif //ENABLE_COLOR_BAR_PATTERN
};
GenePatternFnPtr gGenePatternFn = genChess;

void setPatternFn(ENUM_PATTERN_TYPE e, ENUM_DISP_SRC_FMT iSrcFmt, ENUM_DISP_SRC_ORDER src_order)
{
#ifdef ENABLE_COLOR_BAR_PATTERN
    if (((DISP_SRCFMT_ARGB32==iSrcFmt||DISP_SRCFMT_RGB565==iSrcFmt ||DISP_SRCFMT_XRGB32==iSrcFmt) && e >= PATTERN_MAX) || ((DISP_SRCFMT_ARGB32!=iSrcFmt && DISP_SRCFMT_RGB565!=iSrcFmt && DISP_SRCFMT_XRGB32!=iSrcFmt) && e >= PATTERN_MAX_NON_ARGB))
#else //ENABLE_COLOR_BAR_PATTERN
    if (e >= PATTERN_MAX)
#endif //ENABLE_COLOR_BAR_PATTERN
    {
        CMDLINE_PRINT("setting pattern error, using default\n");
        e = PATTERN_DEFAULT;
    }
    gGenePatternFn = genePatternTbl[e];
    gSrcOrder = src_order;
}


typedef void (*FillFnPtr)(UINT8*, UINT32, UINT32);
inline static void fill8bit(UINT8 *p, UINT32 v0, UINT32 v1)
{
    *(UINT32 *)p = YUV8bit2Tbl[v0 + v1 * 2];
}
inline static void fill10bit(UINT8 *p, UINT32 v0, UINT32 v1)
{
    UINT64 v = YUV10bit2Tbl[v0 + v1 * 2];
    UINT32 i;
    for (i = 0; i < 5; i++) {
        p[i] = (v >> i * 8) & 0xff;
    }
}

UINT32 ARGB8bitTbl[] = {0xff000000, 0xffffffff};
UINT32 ARGB8bitTbl2[] = {0x000000ff, 0xffffffff};

int g_RgbPm=0,g_userAlpha=0, g_RgbX=0, g_src_order;
void preMultiply(UINT8 *rgb, double alpha) {
    int i;

    for(i=0;i<4;i++) {
        *rgb = *rgb *(alpha/(double)255);
        rgb++;
    }
}

inline static void fillARGB(UINT8 *p, UINT32 v)
{
    UINT32 argb_val =(g_src_order==0 || g_src_order==1)? ARGB8bitTbl2[v]: ARGB8bitTbl[v];
#ifdef ENABLE_COLOR_BAR_PATTERN
    if(gGenePatternFn == genColorBarHorizontal || gGenePatternFn == genColorBarVerticle || gGenePatternFn ==  genColorBarCustom || gGenePatternFn == genColorRamp) {
        argb_val = color_rgb_values[v];
    }
#endif //ENABLE_COLOR_BAR_PATTERN

    if(g_RgbPm) {
        preMultiply((UINT8 *)&argb_val, g_userAlpha);
    }
    if(g_RgbX) {
        //Clear Alpha value
        if((g_src_order==0 || g_src_order==1)) {
            argb_val &= ~ARGB8bitTbl2[0];
        } else {
            argb_val &= ~ARGB8bitTbl[0];
        }
    }
    AmpMemcpy(p, &argb_val, 4);
}

#define ENABLE_BIT_DROPPING_LOGIC
inline static void fillRGB565(UINT8 *p, UINT32 v)
{
    UINT32 ARGB;
    UINT8 *pRGB,t;
#ifdef ENABLE_COLOR_BAR_PATTERN
    if(gGenePatternFn == genColorBarHorizontal || gGenePatternFn == genColorBarVerticle || gGenePatternFn ==  genColorBarCustom || gGenePatternFn == genColorRamp) {
        pRGB = (UINT8*)&color_rgb_values[v];
    }
    else
#endif //ENABLE_COLOR_BAR_PATTERN
    pRGB = (UINT8*)&ARGB8bitTbl[v];

    if(!(gSrcOrder&0x01)) {
        //Swap required for Src order : 0(ARGB), 2(RGBA);  No Swap for : 1(ABGR) & 3(BGRA)
        AmpMemcpy(&ARGB, pRGB, 4);
        pRGB = (UINT8 *)&ARGB;
        //swap R & B
        t = pRGB[0]; pRGB[0] = pRGB[2]; pRGB[2]=t;
    }
#ifdef ENABLE_BIT_DROPPING_LOGIC
    #if 0
        //Drop MSB
        *(UINT16*)p = (pRGB[0] & 0x1F) | ((pRGB[1] & 0x3F)<<5) | ((pRGB[2] & 0x1F)<<(6+5)) ;
    #else
        //Drop LSB
        *(UINT16*)p = ((pRGB[0]>>3) & 0x1F) | (((pRGB[1]>>2) & 0x3F)<<5) | (((pRGB[2]>>3) & 0x1F)<<(6+5)) ;
    #endif
#else
    {
    UINT8 R5,G6,B5;
    //MAP RGB888 to RGB565
    R5 = ( pRGB[0] * 249 + 1014 ) >> 11;
    G6 = ( pRGB[1] * 253 +  505 ) >> 10;
    B5 = ( pRGB[2] * 249 + 1014 ) >> 11;

    *(UINT16*)p = (R5) | (G6<<5) | (B5<<(6+5));
    }
#endif
}

UINT32 maskTbl[] = {
    (0x1 << 0) - 1,
    (0x1 << 1) - 1,
    (0x1 << 2) - 1,
    (0x1 << 3) - 1,
    (0x1 << 4) - 1,
    (0x1 << 5) - 1,
    (0x1 << 6) - 1,
    (0x1 << 7) - 1,
    (0x1 << 8) - 1,
};
/*
UINT32 maskTbl[] = {
    (0x1 << 8) - 1,
    (0x1 << 7) - 1,
    (0x1 << 6) - 1,
    (0x1 << 5) - 1,
    (0x1 << 4) - 1,
    (0x1 << 3) - 1,
    (0x1 << 2) - 1,
    (0x1 << 1) - 1,
    (0x1 << 0) - 1,
};
*/
// fill data pointer P, start OFFSET, fill LEN size data from V, (start low-bit)
inline static void fillData(UINT8 *p, UINT32 offset, UINT32 len, UINT64 v)
{
    UINT32 byteoffset = offset / 8;
    UINT32 bitoffset = offset % 8;
    p += byteoffset;
    UINT64 vv;
    if (bitoffset != 0) {
        // extract original bitoffset data;
        vv = (*p) & maskTbl[bitoffset];
        // merge original data and filling data
        vv = vv | (v << bitoffset);
    } else {
        vv = v;
    }
    if (bitoffset + len > 64) {
        CMDLINE_PRINT("fillData overflow\n");
    }
    UINT32 i;
    for (i = 0; i < (bitoffset + len) / 8; i++) {
        p[i] = (vv >> i * 8) & 0xff;
    }
    bitoffset = (bitoffset + len) % 8;
    if (bitoffset != 0) {
        p[i] = (vv >> i * 8) & maskTbl[bitoffset];
    }
}
inline static void fill4208bit(UINT8 *p, UINT32 v0, UINT32 v1)
{
    // UINT32 v = YUV4208bit2Tbl[v0 + v1 * 2];
    // UINT32 i;
    // for (i = 0; i < 3; i++) {
    //     p[i] = (v >> i * 8) & 0xff;
    // }
    fillData(p, 0, 8, v0 * 0xff);
    fillData(p, 8, 4, 0x80 >> 4);
    fillData(p, 12, 8, v1 * 0xff);
    fillData(p, 20, 4, 0x80);
}
inline static void dispBuf(UINT8 *p, UINT32 len)
{
    UINT32 i;
    for (i = 0; i < len; i++) {
        printf("%02x ", p[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        } else if ((i + 1) % 4 == 0) {
            printf("\t");
        }
    }
    if (i % 16 != 0) {
        printf("\n");
    }
}
inline static void fillDataTest()
{
    printf("fillData test begin\n");
    UINT8 buf[16];
    memset(buf, 0, 16);
    fillData(buf, 0, 10, -1);
    dispBuf(buf, 16);

    memset(buf, 0, 16);
    fillData(buf, 0, 15, -1);
    dispBuf(buf, 16);

    memset(buf, 0, 16);
    fillData(buf, 0, 20, -1);
    dispBuf(buf, 16);

    memset(buf, 0, 16);
    fillData(buf, 5, 10, -1);
    dispBuf(buf, 16);

    memset(buf, 0, 16);
    fillData(buf, 10, 10, -1);
    dispBuf(buf, 16);

    memset(buf, 0, 16);
    fillData(buf, 15, 10, -1);
    dispBuf(buf, 16);
    printf("fillData test end\n");
}

#if 0
static int saveBuf(const char *filename, void *p, size_t size)
{
    FILE* fp = fopen(filename, "wb");
    UINT32 ret = fwrite(p, size, 1, fp);
    if (ret != 1) {
        ret = ferror(fp);
        CMDLINE_PRINT("write data error: %d\n", ret);
    } else {
        ret = 0;
    }
    fclose(fp);
    return ret;
}
#endif

static HRESULT generate_yuv422_framedata_new(INT32 iBitDepth, volatile UINT32 *pStartAddr,
                                             UINT32 uiWidth, UINT32 uiHeight)
{
    UINT8 *p = (UINT8 *)pStartAddr;
    UINT32 j, i;
    UINT32 v0, v1;
    UINT32 twoPixelStride = 2;
    FillFnPtr fillFn = fill8bit;
    if (iBitDepth == DISP_INPUT_BIT_DEPTH_8BIT) {
        twoPixelStride = 4;
        fillFn = fill8bit;
        CMDLINE_PRINT("4 fill8bit\n");
    } else if (iBitDepth == DISP_INPUT_BIT_DEPTH_10BIT) {
        twoPixelStride = 5;
        fillFn = fill10bit;
        CMDLINE_PRINT("5 fill10bit\n");
    } else {
        CMDLINE_PRINT("unkown iBitDepth %d, twoPixelStride default to %d\n",
                      iBitDepth, twoPixelStride);
    }

    for (j = 0; j < uiHeight; j++) {
        // 2 -> 1 sample in horizontal
        for (i = 0; i < uiWidth; i += 2, p += twoPixelStride) {
            // siwtch genChess or genRayCentre function to get different frame
            // using 10bit address, but setting 8bit data, 10bit display will OK.
            // 10bit is buggy
            // p = p0 + uiWidth * twoPixelStride / 2 * j + i * 4 / 2;
            v0 = gGenePatternFn(uiWidth, uiHeight, i, j);
            v1 = gGenePatternFn(uiWidth, uiHeight, i + 1, j);
            fillFn(p, v0, v1);
        }
    }

    CMDLINE_PRINT("generate frame ok!\n");
    return SUCCESS;
}

static HRESULT generate_yuv444_framedata(INT32 iBitDepth, volatile UINT8 *pStartAddr,
                                    UINT32 uiWidth, UINT32 uiHeight, ENUM_DISP_SRC_ORDER src_order)
{
    UINT8 *p = (UINT8 *)pStartAddr;
    UINT32 j, i;
    UINT32 *data = YUV444_8bit_Tbl;
    UINT32 v, cur =0, bitdepth = 8;
    UINT32 bitmask = 0xff;
    if (iBitDepth == DISP_INPUT_BIT_DEPTH_8BIT) {
        bitdepth = 8;
        bitmask = 0xff;
        data = YUV444_8bit_Tbl;
        if (src_order == DISP_ORDER_VYU) {
            data = VYU444_8bit_Tbl;
        }
    }  else if (iBitDepth == DISP_INPUT_BIT_DEPTH_10BIT) {
        bitdepth = 10;
        bitmask = 0x3ff;
        data = YUV444_10bit_Tbl;
        if (src_order == DISP_ORDER_VYU) {
            data = VYU444_10bit_Tbl;
        }
    }
    for (j = 0; j < uiHeight; j++) {
        for(i = 0; i < uiWidth; i++) {
            v = gGenePatternFn(uiWidth, uiHeight, i, j);
            for (int k = 2; k >= 0; k--) {
                fillData(p, cur, bitdepth, (data[v] >> (k * bitdepth)) & bitmask);
                cur += bitdepth;
            }
        }
    }
    CMDLINE_PRINT(" %d generate frame ok!\n", __LINE__);
    return SUCCESS;
}

static HRESULT generate_argb_framedata(volatile UINT32 *pStartAddr,
                                       UINT32 uiWidth, UINT32 uiHeight)
{
    UINT8 *p = (UINT8 *)pStartAddr;
    UINT32 j, i;
    UINT32 v;
    for (j = 0; j < uiHeight; j++) {
        for(i = 0; i < uiWidth; i++, p += 4) {
            v = gGenePatternFn(uiWidth, uiHeight, i, j);
            fillARGB(p, v);
        }
    }
    return SUCCESS;
}

static HRESULT generate_rgb565_framedata(volatile UINT32 *pStartAddr,
                                       UINT32 uiWidth, UINT32 uiHeight)
{
    UINT8 *p = (UINT8 *)pStartAddr;
    UINT32 j, i;
    UINT32 v;
    UINT32 userVal=0x0, userValFlag=0x0;
#ifdef ENABLE_USER_DEFINED_PIXEL_VALUE
    printf("Enter the hex flag and pixel value for user defined pixel : ");
    scanf("%x %x",&userValFlag, &userVal);
    printf("User defined flag:%d, PixelVal : %x\n", userValFlag, userVal);
#endif
    for (j = 0; j < uiHeight; j++) {
        for(i = 0; i < uiWidth; i++, p += 2) {
            v = gGenePatternFn(uiWidth, uiHeight, i, j);
            if(!userValFlag)
                fillRGB565(p, v);
            else
                *(UINT16*)p = userVal;
        }
    }
    return SUCCESS;
}


static HRESULT generate_yuv420sp_framedata(volatile UINT32 *pStartAddr,
                                           UINT32 uiWidth, UINT32 uiHeight)
{
    UINT8 *pY = (UINT8 *)pStartAddr;
    UINT8 *pUV = (UINT8 *)pStartAddr + uiWidth * 1 * uiHeight;
    UINT32 j, i;
    UINT32 v;
    for (j = 0; j < uiHeight; j++) {
        for(i = 0; i < uiWidth; i++, pY++) {
            v = gGenePatternFn(uiWidth, uiHeight, i, j);
            *pY = (v * 0xff) & 0xff;
        }
    }
    AmpMemSet(pUV, 0x80, uiWidth * uiHeight / 2 / 2 * 2);
    return SUCCESS;
}

static HRESULT generate_yuv420sp_10bit_framedata(void *p, UINT32 uiWidth, UINT32 uiHeight)
{
    UINT8 *pY = p;
    UINT8 *pUV = pY + uiWidth * uiHeight * 10 / 8;
    UINT32 j, i;
    UINT64 offset = 0, bitdepth = 10, cur;
    UINT32 v;
    for (j = 0; j < uiHeight; j++) {
        for (i = 0; i < uiWidth; i++, offset += bitdepth) {
            v = gGenePatternFn(uiWidth, uiHeight, i, j);
            cur = offset;
            fillData(pY, cur, bitdepth, v * 0x3ff);
        }
    }
    for (j = 0; j < uiHeight; j += 2) {
        for (i = 0; i < uiWidth; i+= 2, offset += bitdepth) {
            fillData(pUV, offset, bitdepth, 0x200);
        }
    }
    return SUCCESS;
}

static HRESULT generate_yuv420sp_tile_framedata(volatile UINT32 *pStartAddr,
                                                UINT32 uiWidth, UINT32 uiHeight,
                                                UINT32 uiTileW, UINT32 uiTileH)
{
    UINT8 *pY = (UINT8 *)pStartAddr;
    UINT8 *pUV = (UINT8 *)pStartAddr + uiWidth * 1 * uiHeight;
    UINT32 j, i, tj, ti;
    UINT32 v;
    for (j = 0; j < uiHeight; j += uiTileH) {
        for (i = 0; i < uiWidth; i += uiTileW) {
            for (tj = 0; tj < uiTileH; tj++) {
                for (ti = 0; ti < uiTileW; ti++, pY++) {
                    v = gGenePatternFn(uiWidth, uiHeight, i + ti, j + tj);
                    *pY = (v * 0xff) & 0xff;
                }
            }
        }
    }
    AmpMemSet(pUV, 0x80, uiWidth * uiHeight / 2 / 2 * 2);
    return SUCCESS;
}

static HRESULT generate_yuv420sp_tile_10bit_framedata(void *p,
                                                      UINT32 uiWidth, UINT32 uiHeight,
                                                      UINT32 uiTileW, UINT32 uiTileH)
{
    UINT64 bitdepth = 10;
    UINT8 *pY = (UINT8 *)p;
    UINT8 *pUV = (UINT8 *)p + uiWidth * uiHeight * bitdepth / 8;
    UINT32 j, i, tj, ti;
    UINT32 v;
    UINT64 offset = 0;
    for (j = 0; j < uiHeight; j += uiTileH) {
        for (i = 0; i < uiWidth; i += uiTileW) {
            for (tj = 0; tj < uiTileH; tj++) {
                for (ti = 0; ti < uiTileW; ti++, offset += bitdepth) {
                    v = gGenePatternFn(uiWidth, uiHeight, i + ti, j + tj);
                    fillData(pY, offset, bitdepth, v * 0x3ff);
                }
            }
        }
    }
    offset = 0;
    for (j = 0; j < uiHeight; j += uiTileH) {
        for (i = 0; i < uiWidth; i += uiTileW) {
            for (tj = 0; tj < uiTileH; tj += 2) {
                for (ti = 0; ti < uiTileW; ti += 2, offset += bitdepth) {
                    // v = gGenePatternFn(uiWidth, uiHeight, i + ti, j + tj);
                    fillData(pUV, offset, bitdepth, 0x200);
                }
            }
        }
    }
    return SUCCESS;
}

static HRESULT generate_yuv420_8bit_framedata(volatile UINT32 *pStartAddr,
                                              UINT32 uiWidth, UINT32 uiHeight)
{
    UINT8 *p = (UINT8 *)pStartAddr;
    UINT32 j, i;
    UINT32 v0, v1;
    for (j = 0; j < uiHeight; j++) {
        for (i = 0; i < uiWidth; i += 2, p += 3) {
            v0 = gGenePatternFn(uiWidth, uiHeight, i, j);
            v1 = gGenePatternFn(uiWidth, uiHeight, i + 1, j);
            fill4208bit(p, v0, v1);
        }
    }
    CMDLINE_PRINT("WARNING: YUV420 need more test\n");
    return SUCCESS;
}
static HRESULT generate_yuv420_10bit_framedata(void *pStartAddr,
                                               UINT32 uiWidth, UINT32 uiHeight)
{
    UINT8 *p = (UINT8 *)pStartAddr;
    UINT32 j, i;
    UINT32 v0, v1;
    UINT64 offset = 0, cur;
    UINT64 bitdepth = 10;
    UINT64 bits_per_pixel = bitdepth * 3 / 2;
    for (j = 0; j < uiHeight; j++) {
        for (i = 0; i < uiWidth; i += 2, offset += 2 * bits_per_pixel) {
            v0 = gGenePatternFn(uiWidth, uiHeight, i, j);
            v1 = gGenePatternFn(uiWidth, uiHeight, i + 1, j);
            cur = offset;
            fillData(p, cur, bitdepth, v0 * 0x3ff);
            cur += bitdepth;
            fillData(p, cur, bitdepth / 2, 0x200 >> (bitdepth / 2));
            cur += bitdepth / 2;
            fillData(p, cur, bitdepth, v1 * 0x3ff);
            cur += bitdepth;
            fillData(p, cur, bitdepth / 2, 0x200 & maskTbl[bitdepth / 2]);
        }
    }
    return SUCCESS;
}
#if 0
static HRESULT generate_yuv422_framedata(INT32 iBitDepth, volatile UINT32 *pStartAddr,
                               UINT32 uiPicH,   UINT32 uiLineV, UINT32 uiWidth,
                               UINT32 uiHeight, UINT32 uiPicA,  UINT32 uiPicB)
{
    volatile UINT32 *Ptr;
    UINT32 VLines, VRep,  HS, HRep, PixVal, VBlock, HBlock;
    UINT32 PixelPerWord = 0;
    int bits_per_pixel = 0;
    HRESULT Ret = SUCCESS;

    bits_per_pixel = get_frame_bitsperpixel(DISP_SRCFMT_YUV422, iBitDepth);

    PixelPerWord = bits_per_pixel/8;

    if (!pStartAddr) {
        Ret = ERR_ERRPARAM;
        goto generate_yuv422_framedata_exit;
    }
    CMDLINE_PRINT("iBitDepth %d, picH %d uiLineV %d uiWidth %d uiHeight %d uiPicA %x uiPicB %x\n",
                  iBitDepth, uiPicH, uiLineV, uiWidth, uiHeight, uiPicA, uiPicB);

    /**
    * make the whole frame data
    */
    Ptr = pStartAddr;

    VBlock = (double)uiLineV;

    for(VLines = 0; VLines < uiHeight; VLines += VBlock) {
        if ((uiHeight - VLines) < VBlock) {
            VBlock = uiHeight - VLines;
        }

        for (VRep=0; VRep < VBlock; VRep++) {
            HBlock = uiPicH * PixelPerWord;

            for (HS = 0; HS < uiWidth; HS += HBlock) {
                if ((VLines / uiLineV + HS/(uiPicH * PixelPerWord)) & 0x1) {
                    PixVal = uiPicB;
                }
                else {
                    PixVal = uiPicA;
                }

                if ((uiWidth - HS) < HBlock) {
                    HBlock = uiWidth - HS;
                }
                for (HRep = 0; HRep < HBlock/PixelPerWord; HRep++) {
                    *Ptr++ = PixVal;
                }
            }
        }
    }

generate_yuv422_framedata_exit:
  return Ret;
}
#endif



HRESULT create_buildin_tHDR_metadata_frame(AMP_BDTAG_MEMINFO *pmemInfo, char *pDecoderParams, char *pDynamicMdFile)
{
    UINT32 MdSz;
    VOID *mdBase;
    AMP_SHM_HANDLE hShm1 = 0;
    HRESULT Ret = SUCCESS;
    void *buf= NULL;
    THDR_MD *pstMdBuf = NULL;
#define SIZE_OF_BUFFER  200

    FILE *SM_File = NULL, *DM_File = NULL;
    char achLine[SIZE_OF_BUFFER] = "\0";
    int Value = 0;
    int i = 0;

    MdSz = sizeof(THDR_MD);
    Ret = AMP_SHM_Allocate(AMP_SHM_DYNAMIC, MdSz, 1024, &hShm1);
    if (Ret != SUCCESS) {
        CMDLINE_PRINT("AMP_SHM_Allocate Fails Size:%d\n", MdSz);
        Ret = ERR_NOMEM;
        goto fail;
    }
    Ret = AMP_SHM_Ref(hShm1,&buf);
    if (Ret != SUCCESS) {
        CMDLINE_PRINT("AMP_SHM_Ref Fails \n");
        goto fail;

    }
    Ret = AMP_SHM_GetVirtualAddress(hShm1, 0, &mdBase);
    if ((Ret != SUCCESS) || !mdBase) {
        CMDLINE_PRINT("AMP_SHM_GetVirtualAddress Fails,Ret:%d, Base:%p\n", Ret, mdBase);
        Ret = ERR_NOMEM;
        goto fail;
    }
    pstMdBuf = (THDR_MD *)mdBase;

    SM_File = fopen(pDecoderParams,"rb+");
    DM_File = fopen(pDynamicMdFile,"rb+");

    if(SM_File == NULL|| DM_File == NULL)
    {
        printf("Unable to open file\n");
        Ret = -1;
        goto fail;
    }

    WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
    sscanf(achLine, "%d", &Value);
    pstMdBuf->specVersion = Value;

    WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
    sscanf(achLine, "%d", &Value);
    pstMdBuf->payloadMode = Value;

    WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
    sscanf(achLine, "%d", &Value);
    pstMdBuf->hdrPicColourSpace = Value;

    WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
    sscanf(achLine, "%d", &Value);
    pstMdBuf->hdrMasterDisplayColourSpace = Value;


    WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
    sscanf(achLine, "%d", &Value);
    pstMdBuf->hdrMasterDisplayMaxLuminance = Value;

    WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
    sscanf(achLine, "%d", &Value);
    pstMdBuf->hdrMasterDisplayMinLuminance = Value;

    WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
    sscanf(achLine, "%d", &Value);
    pstMdBuf->sdrPicColourSpace = Value;

    WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
    sscanf(achLine, "%d", &Value);
    pstMdBuf->sdrMasterDisplayColourSpace = Value;

    if(0 == pstMdBuf->payloadMode)
    {
         WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
         sscanf(achLine, "%d", &Value);
         pstMdBuf->u.variables.tmInputSignalBlackLevelOffset = Value;

         WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
         sscanf(achLine, "%d", &Value);
         pstMdBuf->u.variables.tmInputSignalWhiteLevelOffset = Value;

         WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
         sscanf(achLine, "%d", &Value);
         pstMdBuf->u.variables.shadowGain = Value;

         WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
         sscanf(achLine, "%d", &Value);
         pstMdBuf->u.variables.highlightGain = Value;

         WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
         sscanf(achLine, "%d", &Value);
         pstMdBuf->u.variables.midToneWidthAdjFactor = Value;

         WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
         sscanf(achLine, "%d", &Value);
         pstMdBuf->u.variables.tmOutputFineTuningNumVal = Value;

         if(pstMdBuf->u.variables.tmOutputFineTuningNumVal)
         {
             for (i = 0; i < pstMdBuf->u.variables.tmOutputFineTuningNumVal; i++)
             {
                 WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
                 sscanf(achLine, "%d", &Value);
                 pstMdBuf->u.variables.tmOutputFineTuningX[i] = Value;

                 WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
                 sscanf(achLine, "%d", &Value);
                 pstMdBuf->u.variables.tmOutputFineTuningY[i] = Value;
             }
         }

         WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
         sscanf(achLine, "%d", &Value);
         pstMdBuf->u.variables.saturationGainNumVal = Value;

         if(pstMdBuf->u.variables.saturationGainNumVal)
         {
             for(i = 0; i < pstMdBuf->u.variables.saturationGainNumVal; i++)
             {
                 WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
                 sscanf(achLine, "%d", &Value);
                 pstMdBuf->u.variables.saturationGainX[i] = Value;

                 WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
                 sscanf(achLine, "%d", &Value);
                 pstMdBuf->u.variables.saturationGainY[i] = Value;
             }
         }
    }
    else
    {
        WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
        sscanf(achLine, "%d", &Value);
        pstMdBuf->u.tables.luminanceMappingNumVal = Value;

        if(pstMdBuf->u.tables.luminanceMappingNumVal)
        {
            for(i = 0; i < pstMdBuf->u.tables.luminanceMappingNumVal; i++)
            {
                WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
                sscanf(achLine, "%d", &Value);
                pstMdBuf->u.tables.luminanceMappingX[i] = Value;

                WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
                sscanf(achLine, "%d", &Value);
                pstMdBuf->u.tables.luminanceMappingY[i] = Value;
            }
        }

        WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
        sscanf(achLine, "%d", &Value);
        pstMdBuf->u.tables.colourCorrectionNumVal = Value;

        if(pstMdBuf->u.tables.colourCorrectionNumVal)
        {
            for(i = 0; i < pstMdBuf->u.tables.colourCorrectionNumVal; i++)
            {
                WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
                sscanf(achLine, "%d", &Value);
                pstMdBuf->u.tables.colourCorrectionX[i] = Value;

                WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
                sscanf(achLine, "%d", &Value);
                pstMdBuf->u.tables.colourCorrectionY[i] = Value;
            }
        }

        WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
        sscanf(achLine, "%d", &Value);
        pstMdBuf->u.tables.chromaToLumaInjectionMuA = Value;

        WRAP_FGETS(achLine, SIZE_OF_BUFFER, DM_File);
        sscanf(achLine, "%d", &Value);
        pstMdBuf->u.tables.chromaToLumaInjectionMuB = Value;
    }

  /**
     * flush data in cache
     */
    Ret = AMP_SHM_CleanCache(hShm1, 0, MdSz);
    if (Ret != SUCCESS) {
        CMDLINE_PRINT("AMP_SHM_CleanCache Fails,Ret:%d\n", Ret);
        Ret = ERR_NOMEM;
        goto fail;
    }


    pmemInfo->Header.eType = AMP_BDTAG_ASSOCIATE_MEM_INFO;
    pmemInfo->Header.uLength = sizeof(AMP_BDTAG_MEMINFO);
    pmemInfo->uMemHandle = (UINT32)hShm1;
    pmemInfo->uMemOffset = 0;
    pmemInfo->uSize = MdSz;
    fclose(SM_File);
    fclose(DM_File);
    return Ret;

fail:
    if (SM_File) {
        fclose(SM_File);
    }
    if (DM_File) {
        fclose(DM_File);
    }
    return Ret;
}



HRESULT create_buildin_metadata_frame(AMP_BDTAG_MEMINFO *pmemInfo, char *comp_mdfile,  char *dm_mdfile)
{
    UINT32 MdSz;
    VOID *mdBase;
    AMP_SHM_HANDLE hShm1 = 0;
    HRESULT Ret = SUCCESS;
    void *buf= NULL;

    MdSz = sizeof(rpu_ext_config_fixpt_main_t) + sizeof(dm_metadata_t);
    Ret = AMP_SHM_Allocate(AMP_SHM_DYNAMIC, MdSz, 1024, &hShm1);
    if (Ret != SUCCESS) {
        CMDLINE_PRINT("AMP_SHM_Allocate Fails Size:%d\n", MdSz);
        Ret = ERR_NOMEM;
        goto create_buildin_mdframe_exit;
    }
    Ret = AMP_SHM_Ref(hShm1,&buf);
    if (Ret != SUCCESS) {
        CMDLINE_PRINT("AMP_SHM_Ref Fails \n");
        goto create_buildin_mdframe_exit;
    }
    Ret = AMP_SHM_GetVirtualAddress(hShm1, 0, &mdBase);
    if ((Ret != SUCCESS) || !mdBase) {
        CMDLINE_PRINT("AMP_SHM_GetVirtualAddress Fails,Ret:%d, Base:%p\n", Ret, mdBase);
        Ret = ERR_NOMEM;
        goto create_buildin_mdframe_exit;
    }

    vpp_read_write_file(mdBase, 1800, comp_mdfile, 1); //Read composer metadata, 1800 bytes
    vpp_read_md_file((char *)mdBase + sizeof(rpu_ext_config_fixpt_main_t), 0, dm_mdfile, 1); //read dm metadata

    /**
     * flush data in cache
     */
    Ret = AMP_SHM_CleanCache(hShm1, 0, MdSz);
    if (Ret != SUCCESS) {
        CMDLINE_PRINT("AMP_SHM_CleanCache Fails,Ret:%d\n", Ret);
        Ret = ERR_NOMEM;
        goto create_buildin_mdframe_exit;
    }
    pmemInfo->Header.eType = AMP_BDTAG_ASSOCIATE_MEM_INFO;
    pmemInfo->Header.uLength = sizeof(AMP_BDTAG_MEMINFO);
    pmemInfo->uMemHandle = (UINT32)hShm1;
    pmemInfo->uMemOffset = 0;
    pmemInfo->uSize = MdSz;

create_buildin_mdframe_exit:
    return Ret;
}


HRESULT create_buildin_frame(AMP_BGTAG_FRAME_INFO *pFrameInfo, ENUM_DISP_SRC_FMT iSrcFmt,
                             UINT32 width, UINT32 height, ENUM_DISP_DEINT_TYPE progressive,
                             ENUM_DISP_INPUT_BIT_DEPTH bit_depth, ENUM_DISP_SRC_ORDER src_order,
                             UINT32 pattern, const char *filename,
                             DISP_RAMP_ATTR *pRampAttr, int frame_type)
{
    VOID *Base;
    AMP_SHM_HANDLE hShm = 0;
    HRESULT Ret = SUCCESS;
    INT32 Size, rdSize;
    UINT32 *pTileGrid = NULL;
    UINT32 width_adj;

    CMDLINE_PRINT("Build Frame start\n");
    CMDLINE_PRINT("Frame Info: src_fmt: %d, width: %d,"
        "height: %d, progressive: %d, bitdepth: %d, src_order: %d\n",
        iSrcFmt, width, height, progressive, bit_depth, src_order);
    //MSB byte is pm alpha
    g_userAlpha = ((pattern>>8) & 0xFF);
    //LSB 8 bit for pattern
    pattern = (pattern & 0xFF);
    AmpMemClear(pFrameInfo, sizeof(AMP_BGTAG_FRAME_INFO));

    pFrameInfo->Header.eType   = AMP_BGTAG_FRAME_INFO_META;
    pFrameInfo->Header.uLength = sizeof(AMP_BGTAG_FRAME_INFO);

    width_adj = width+7;
    width_adj /= 8;
    width_adj *= 8;

    Size = get_frame_size(iSrcFmt, bit_depth, width_adj, height?height:1);

    if(frame_type)
        Size = Size * 2; //16-bit YUV420SP

    rdSize = Size;
    Ret = AMP_SHM_Allocate(AMP_SHM_DYNAMIC, Size, 1024, &hShm);
    if (Ret != SUCCESS) {
        CMDLINE_PRINT("AMP_SHM_Allocate Fails Size:%d\n", Size);
        Ret = ERR_NOMEM;
        goto create_buildin_frame_exit;
    }

    /**
    * get virtual address
    */
    Ret = AMP_SHM_GetVirtualAddress(hShm, 0, &Base);

    if ((Ret != SUCCESS) || !Base) {
        CMDLINE_PRINT("AMP_SHM_GetVirtualAddress Fails,Ret:%d, Base:%p\n", Ret, Base);
        Ret = ERR_NOMEM;
        goto create_buildin_frame_exit;
    }

        // Ret = generate_yuv422_framedata(bit_depth, (volatile UINT32 *)Base, 32, 36,
            // width, height, 0x40604060, 0xb0a0b0a0);
    gRampAttr = *pRampAttr;
#ifdef ENABLE_COLOR_BAR_PATTERN
    getRGBfromYUV_values();
#endif
    AmpMemClear(Base, Size);
    gGenePatternFn = genChess;
    setPatternFn(pattern, iSrcFmt, src_order);
    switch (iSrcFmt) {
        case DISP_SRCFMT_YUV422:
        //make YUV422 frame data
        CMDLINE_PRINT("generate yuv422 frame\n");
        Ret = generate_yuv422_framedata_new(bit_depth, (volatile UINT32 *)Base, width_adj, height);
        // Ret = generate_yuv422_framedata(bit_depth, (volatile UINT32 *)Base, 32, 32,
            // width, height, 0x40604060, 0xb0a0b0a0);
        break;
        case DISP_SRCFMT_YUV420:
        if (bit_depth == DISP_INPUT_BIT_DEPTH_8BIT) {
            CMDLINE_PRINT("generate yuv420 8bit frame\n");
            Ret = generate_yuv420_8bit_framedata((volatile UINT32 *)Base, width_adj, height);
        } else if (bit_depth == DISP_INPUT_BIT_DEPTH_10BIT) {
            CMDLINE_PRINT("generate yuv420 10bit frame\n");
            Ret = generate_yuv420_10bit_framedata(Base, width_adj, height);
        }
        break;
        case DISP_SRCFMT_YUV444:
            CMDLINE_PRINT("generate yuv444 frame\n");
            Ret = generate_yuv444_framedata(bit_depth, (volatile UINT8 *)Base, width_adj, \
                                                            height, src_order);
        break;
        case DISP_SRCFMT_YUV420SP:
        if (bit_depth == DISP_INPUT_BIT_DEPTH_8BIT) {
            CMDLINE_PRINT("generate YUV420SP 8bit frame\n");
            Ret = generate_yuv420sp_framedata((volatile UINT32 *)Base, width_adj, height);
        } else if (bit_depth == DISP_INPUT_BIT_DEPTH_10BIT) {
            CMDLINE_PRINT("generate YUV420SP 10bit frame\n");
            Ret = generate_yuv420sp_10bit_framedata(Base, width_adj, height);
        }
        break;
        case DISP_SRCFMT_YUV420SP_TILED_MODE0:
        case DISP_SRCFMT_YUV420SP_TILED_MODE1:
        case DISP_SRCFMT_YUV420SP_TILED_V4H8:
        pTileGrid = gTileGridTbl[iSrcFmt];
        if (bit_depth == DISP_INPUT_BIT_DEPTH_8BIT) {
            CMDLINE_PRINT("generate YUV420SP mode %d %d %d 8bit\n", iSrcFmt,
                          pTileGrid[0], pTileGrid[1]);
            Ret = generate_yuv420sp_tile_framedata(Base, width_adj, height,
                                                   pTileGrid[0], pTileGrid[1]);
        } else if (bit_depth == DISP_INPUT_BIT_DEPTH_10BIT) {
            CMDLINE_PRINT("generate YUV420SP mode %d %d %d 10bit\n", iSrcFmt,
                          pTileGrid[0], pTileGrid[1]);
            Ret = generate_yuv420sp_tile_10bit_framedata(Base, width_adj, height,
                                                         pTileGrid[0], pTileGrid[1]);
        }
        break;
        case DISP_SRCFMT_ARGB32_PM:
            g_src_order=src_order;
            CMDLINE_PRINT("generate argb32_pm frame\n");
            g_RgbPm=1;
            CMDLINE_PRINT("ARGB32_PM: the user entered alpha is %x pattern=%d\n", g_userAlpha, pattern);
            Ret = generate_argb_framedata((volatile UINT32 *)Base, width_adj, height);
            g_RgbPm=0;
        break;
        case DISP_SRCFMT_XRGB32:
            CMDLINE_PRINT("generate xrgb32 frame\n");
            g_RgbX=1;
            CMDLINE_PRINT("XRGB32: the user entered alpha is %x pattern=%d\n", g_userAlpha, pattern);
            g_src_order=src_order;
            Ret = generate_argb_framedata((volatile UINT32 *)Base, width_adj, height);
            g_RgbX=0;
        break;
        case DISP_SRCFMT_ARGB32:
        case DISP_SRCFMT_ARGB4444:
        case DISP_SRCFMT_ARGB32_4K:
        CMDLINE_PRINT("generate argb32 frame\n");
        g_src_order=src_order;
        Ret = generate_argb_framedata((volatile UINT32 *)Base, width_adj, height);
        break;
        case DISP_SRCFMT_RGB565:
        CMDLINE_PRINT("generate rgb565 frame\n");
        Ret = generate_rgb565_framedata((volatile UINT32 *)Base, width_adj, height);
        break;

        default:
        CMDLINE_PRINT("unkown frame type %d\n", iSrcFmt);
    }
    if (Ret != SUCCESS) {
        CMDLINE_PRINT("make_frame_data fails");
        goto create_buildin_frame_exit;
    }
    if(filename) {
        vpp_read_write_file(Base, rdSize, filename, 1);
    }
#ifdef ENABLE_FRAMEBUFFER_DUMP
    {
        int i;
        CMDLINE_PRINT("Content of frame buffer at : %x\n",Base);
        for(i=0;i<96;i++) {
            CMDLINE_PRINT("%08X ",*(((UINT32*)Base)+i));
            if((i+1)%8 == 0)
                CMDLINE_PRINT("\n");
        }
    }
#endif //ENABLE_FRAMEBUFFER_DUMP
    /**
    * flush data in cache
    */
    Ret = AMP_SHM_CleanCache(hShm, 0, Size);
    if (Ret != SUCCESS) {
        CMDLINE_PRINT("AMP_SHM_CleanCache Fails,Ret:%d\n", Ret);
        Ret = ERR_NOMEM;
        goto create_buildin_frame_exit;
    }

    /**
    * get cache physical address for DMA
    */
    Ret = AMP_SHM_GetPhysicalAddress(hShm, 0, &Base);

    if ((Ret != SUCCESS) || !Base) {
        CMDLINE_PRINT("AMP_SHM_GetPhysicalAddress Fails,Ret:%d, Base:%p\n", Ret, Base);
        Ret = ERR_NOMEM;
        goto create_buildin_frame_exit;
    }

    pFrameInfo->uStatus = AMP_FRAME_ST_DECODED;
    pFrameInfo->pBufStart = Base;
    pFrameInfo->hShm = hShm;
    CMDLINE_PRINT("Share memory handle: 0x%x\n", hShm);

    fillDataTest();
    set_frameinfo_data(pFrameInfo, iSrcFmt, width, height, progressive, bit_depth, src_order);

    CMDLINE_PRINT("Build Frame Done\n");
create_buildin_frame_exit:
    return Ret;
}


HRESULT update_mtrr_info(AMP_BGTAG_FRAME_INFO *pFrameInfo, DISP_TEST_FRAME frame_info)
{
    VOID *PageAdress,*PageAddressUV;
    AMP_SHM_HANDLE hShm;
    HRESULT Ret = SUCCESS;
    UINT32 FrameSize,Pages,uvPages,yFrameSize,PageTabelSize;

    if (NULL == pFrameInfo) {
        Ret = ERR_NOMEM;
        CMDLINE_PRINT("%d Invalid pFrameInfo \n",__LINE__ );
        goto create_buildin_frame_exit;
    }

    FrameSize = get_frame_size(frame_info.color_fmt, frame_info.bit_depth, frame_info.w,
                                                                           frame_info.h);
    Pages = GET_PAGES(FrameSize);
    PageTabelSize = GET_PAGE_TABLE_SIZE(Pages);
    if (!VPP_IS_SINGLE_CLIENT_SRC_FMT(pFrameInfo->uiSrcFmt)) {
        /* if the total page size is 1 , we need to allocate additional/separate page for UV*/
        PageTabelSize += (PageTabelSize == MMU_PAGE_SIZE)* MMU_PAGE_SIZE;
    }
    Ret = AMP_SHM_Allocate(AMP_SHM_DYNAMIC, PageTabelSize, MMU_PAGE_SIZE, &hShm);
    if (Ret != SUCCESS) {
        CMDLINE_PRINT("%d AMP_SHM_Allocate Fails FrameSize:%d\n",__LINE__, FrameSize);
        Ret = ERR_NOMEM;
        goto create_buildin_frame_exit;
    }

    CMDLINE_PRINT("%d AMP_SHM_Allocate FrameSize:%d pages=%d \n",__LINE__,FrameSize,  Pages);

    /*
     * get virtual address of page table
     */
    Ret = AMP_SHM_GetVirtualAddress(hShm, 0, &PageAdress);

    if ((Ret != SUCCESS) || !PageAdress) {
        CMDLINE_PRINT("%d Fails,Ret:%d, PageAdress:%p\n",__LINE__ , Ret, PageAdress);
        Ret = ERR_NOMEM;
        goto create_buildin_frame_exit;
    }

    if (VPP_IS_SINGLE_CLIENT_SRC_FMT(pFrameInfo->uiSrcFmt)) {
        create_page_table(PageAdress, Pages, pFrameInfo->pBufStart);
    } else {
        yFrameSize = (UINT32)pFrameInfo->uiPbufStartUV - (UINT32)pFrameInfo->pBufStart;
        Pages = GET_PAGES(yFrameSize);

        create_page_table(PageAdress, Pages, pFrameInfo->pBufStart);
        /* PageAddress should be page aligned*/
        PageAddressUV = (VOID*)((UINT32)PageAdress + GET_PAGE_TABLE_SIZE(Pages));
        uvPages = GET_PAGES(FrameSize - yFrameSize);
        create_page_table(PageAddressUV, uvPages, (VOID*)pFrameInfo->uiPbufStartUV);
        /*updating Y Page offset here,Y physical pageAddress will be added after getting it*/
        pFrameInfo->uiPbufStartUV = (UINT32)PageAddressUV - (UINT32)PageAdress;
    }
    /*
     * flush data in cache
     */
    Ret = AMP_SHM_CleanCache(hShm, 0, Pages * 4);
    if (Ret != SUCCESS) {
        CMDLINE_PRINT("%d AMP_SHM_CleanCache Fails,Ret:%d\n",__LINE__,  Ret);
        Ret = ERR_NOMEM;
        goto create_buildin_frame_exit;
    }

    /*
     * get cache physical address for DMA
     */
    Ret = AMP_SHM_GetPhysicalAddress(hShm, 0, &PageAdress);

    if ((Ret != SUCCESS) || !PageAdress) {
        CMDLINE_PRINT(" %d Fails,Ret:%d, PageAdress:%p\n", Ret,__LINE__,  PageAdress);
        Ret = ERR_NOMEM;
        goto create_buildin_frame_exit;
    }

    pFrameInfo->pBufStart = PageAdress;
    pFrameInfo->hShm = hShm;
    pFrameInfo->uiBufStride *= MMU_STRIDE_FACTOR;
    AmpMemClear(pFrameInfo->uiLumaMtrrCfg, sizeof(pFrameInfo->uiLumaMtrrCfg));
    pFrameInfo->uiLumaMtrrCfg[2] |= MMU_CFG_PM_ENABLE << MMU_PM_BIT_POSITION;
    pFrameInfo->uiLumaMtrrCfg[3] |= MMU_CFG_VM_ENABLE;
    if (!VPP_IS_SINGLE_CLIENT_SRC_FMT(pFrameInfo->uiSrcFmt)) {
        pFrameInfo->uiPbufStartUV += (UINT32)PageAdress;
        pFrameInfo->uiBufStrideUV = pFrameInfo->uiBufStride;
        AmpMemClear(pFrameInfo->uiChromaMtrrCfg, sizeof(pFrameInfo->uiChromaMtrrCfg));
        pFrameInfo->uiChromaMtrrCfg[2] |= MMU_CFG_PM_ENABLE << MMU_PM_BIT_POSITION;
        pFrameInfo->uiChromaMtrrCfg[3] |= MMU_CFG_VM_ENABLE;
    }
create_buildin_frame_exit:
    return Ret;
}

/* This function creates a dummy frame in secure memory. */
HRESULT create_buildin_frame_secure(AMP_BGTAG_FRAME_INFO *pFrameInfo, ENUM_DISP_SRC_FMT iSrcFmt,
                             UINT32 width, UINT32 height, ENUM_DISP_DEINT_TYPE progressive,
                             ENUM_DISP_INPUT_BIT_DEPTH bit_depth, ENUM_DISP_SRC_ORDER src_order,
                             UINT32 pattern, const char *filename,
                             DISP_RAMP_ATTR *pRampAttr, int frame_type)
{
    VOID *Base;
    AMP_SHM_HANDLE hShm = 0;
    HRESULT Ret = SUCCESS;
    INT32 Size;
    UINT32 width_adj;

    CMDLINE_PRINT("Build Frame start\n");
    CMDLINE_PRINT("Frame Info: src_fmt: %d, width: %d,"
        "height: %d, progressive: %d, bitdepth: %d, src_order: %d\n",
        iSrcFmt, width, height, progressive, bit_depth, src_order);
    //MSB byte is pm alpha
    g_userAlpha = ((pattern>>8) & 0xFF);
    //LSB 8 bit for pattern
    pattern = (pattern & 0xFF);
    AmpMemClear(pFrameInfo, sizeof(AMP_BGTAG_FRAME_INFO));

    pFrameInfo->Header.eType   = AMP_BGTAG_FRAME_INFO_META;
    pFrameInfo->Header.uLength = sizeof(AMP_BGTAG_FRAME_INFO);

    width_adj = width+7;
    width_adj /= 8;
    width_adj *= 8;

    Size = get_frame_size(iSrcFmt, bit_depth, width_adj, height?height:1);

    if(frame_type)
        Size = Size * 2; //16-bit YUV420SP

    Ret = AMP_SHM_Allocate(AMP_SHM_SECURE_DYNAMIC, Size, 1024, &hShm);
    if (Ret != SUCCESS) {
        CMDLINE_PRINT("AMP_SHM_Allocate Fails Size:%d\n", Size);
        Ret = ERR_NOMEM;
        goto create_buildin_frame_exit;
    }

    /**
    * get virtual address
    */

    gRampAttr = *pRampAttr;
#ifdef ENABLE_COLOR_BAR_PATTERN
    getRGBfromYUV_values();
#endif
    gGenePatternFn = genChess;
    setPatternFn(pattern, iSrcFmt, src_order);

    /**
    * get cache physical address for DMA
    */
    Ret = AMP_SHM_GetPhysicalAddress(hShm, 0, &Base);

    if ((Ret != SUCCESS) || !Base) {
        CMDLINE_PRINT("AMP_SHM_GetPhysicalAddress Fails,Ret:%d, Base:%p\n", Ret, Base);
        Ret = ERR_NOMEM;
        goto create_buildin_frame_exit;
    }

    pFrameInfo->uStatus = AMP_FRAME_ST_DECODED;
    pFrameInfo->pBufStart = Base;
    pFrameInfo->hShm = hShm;
    CMDLINE_PRINT("Share memory handle: 0x%x\n", hShm);

    fillDataTest();
    set_frameinfo_data(pFrameInfo, iSrcFmt, width, height, progressive, bit_depth, src_order);

    CMDLINE_PRINT("Build Frame Done\n");
create_buildin_frame_exit:
    return Ret;
}

/* This function creates pagetable for the frame buffer. */
static HRESULT create_page_table(VOID *PageTableStartAddr, UINT32 Totalpages, VOID *FrameAddress)
{
    UINT32 FrameCurrAddr;
    MMU_ENTRY *PagetableCurEntry;

    FrameCurrAddr = (UINT32)FrameAddress >> 12;
    PagetableCurEntry = (MMU_ENTRY*)PageTableStartAddr;
    while (Totalpages--) {
        /*4k page alligned frame address are updated in pagetables and enabling page entry */
        PagetableCurEntry->pageAddress = FrameCurrAddr++;
        PagetableCurEntry->flags = 0x001;
        PagetableCurEntry++;
    }
     return SUCCESS;
}

