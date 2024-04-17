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

#include "vpp_module.h"
#include "vpp_cfg.h"
#include "avio_memmap.h"
#include "api_avio_dhub.h"
#include "printf.h"
#include "vpp_api.h"
#include "disp_ctrl.h"

#define     MAX_FRAME_DESC  4

#define bTST(x, b) (((x) >> (b)) & 1)

typedef enum {
    FB_PATTERN_CHECKER = 0,
    FB_PATTERN_VERT_COLORBAR,
    FB_PATTERN_CUSTOM,
    FB_PATTERN_CUSTOM_APM,
    MAX_PATTERNS,
    FB_PATTERN_HORZ_COLORBAR,
} FB_PATTERN_TYPE;

static VBUF_INFO *gDescArray[MAX_FRAME_DESC];
static INT32 gDescIndex;
static VBUF_INFO *gDescArray_gfx[MAX_FRAME_DESC];
static INT32 gDescIndex_gfx;

extern VPP_OBJ *pVppobj;
extern void * malloc_ion_noncacheable(int size);

static void GaloisInit(void)
{
    // do nothing
}

void* GaloisMalloc(unsigned int size)
{
    if (size > 0)
    {
        void *ptr = (void *)((((unsigned long)malloc_ion_noncacheable(size + 32) + 31) >> 5) << 5);
        return(ptr);
    }
    return(0);
}

void* GaloisMemSet(void *s, int c, int n)
{

	unsigned char *	char_ptr = s;
	unsigned int *	int_ptr = s;
	int		remainer;

	if((remainer = ((int)s & 3)) != 0) {
		while (remainer-- > 0) {
			*char_ptr++ = (unsigned char)c;
			n--;
		}
		int_ptr = (unsigned int *)char_ptr;
		remainer = n & 3;
	}

	n >>= 2;
	while (n-- > 0) {
		*int_ptr++ = (unsigned int)c;
	}

	if (remainer != 0) {
		unsigned int	temp = *int_ptr;
		unsigned char *	temp_p = (unsigned char *)&temp;
		while (remainer-- > 0) {
			*temp_p++ = (unsigned char)c;
		}
		*int_ptr = temp;
	}
	return(s);
}

void* GaloisMemcpy(void *s1, const void *s2, int n)
{

	unsigned int *		dst = s1;
	const unsigned int *	src = s2;
	int			remainer = n & 3;

	if (((unsigned int)s1 & 3) || ((unsigned int)s2 & 3)) {
		unsigned char *		dst_char = s1;
		const unsigned char *	src_char = s2;

		while (n-- > 0) {
			*dst_char++ = *src_char++;
		}
		return s1;
	}

	n >>= 2;
	while (n-- > 0) {
		*dst++ = *src++;
	}

	if (remainer != 0) {
		unsigned int		temp = *dst;
		unsigned char *		temp_p = (unsigned char *)&temp;
		const unsigned char *	src_c = (const unsigned char *)src;
		while (remainer-- > 0) {
			*temp_p++ = (unsigned char)*src_c++;
		}
		*dst = temp;
	}

	return(s1);
}

VOID destroy_vbuf_desc(VBUF_INFO *pVBufInfo)
{
    (void)pVBufInfo;
}

INT32 make_frame_data(UINT32 iVideo, UINT32 *pStartAddr,
              UINT32 uiPicH, UINT32 uiLineV, UINT32 uiWidth,
              UINT32 uiHeight, UINT32 uiPicA, UINT32 uiPicB)
{
    UINT32 *Ptr;
    UINT32 VLines, VRep, HS, HRep, PixVal, VBlock, HBlock, PixelPerWord;
    HRESULT Ret = MV_VPP_OK;

    if (!pStartAddr) {
        printf("invalid input parameters\n");
        Ret = -1;
        goto make_frame_data_exit;
    }

    /**
    * make the whole frame data
    */
    Ptr = pStartAddr;

    if (iVideo)
        PixelPerWord = 2;
    else
        PixelPerWord = 1;

    VBlock = uiLineV;

    for (VLines = 0; VLines < uiHeight; VLines += VBlock) {
        if ((uiHeight - VLines) < VBlock)
            VBlock = uiHeight - VLines;

        for (VRep = 0; VRep < VBlock; VRep++) {
            HBlock = uiPicH * PixelPerWord;

            for (HS = 0; HS < uiWidth; HS += HBlock) {
                if ((VLines / uiLineV +
                        HS/(uiPicH * PixelPerWord))
                        & 0x1)
                    PixVal = uiPicB;
                else
                    PixVal = uiPicA;

                if ((uiWidth - HS) < HBlock)
                    HBlock = uiWidth - HS;

                for (HRep = 0; HRep < HBlock/PixelPerWord;
                        HRep++)
                    *Ptr++ = PixVal;
            }
        }
    }
make_frame_data_exit:
    return Ret;
}

VOID build_frames(VBUF_INFO *vbufinfo, VOID *vbuf,
    INT32 srcfmt, INT32 bit_depth, INT32 x, INT32 y, INT32 width,
    INT32 height, INT32 progressive, INT32 pattern_type, unsigned char IsPatt)
{
    UINT32 datasize;

    if(bit_depth != INPUT_BIT_DEPTH_8BIT || progressive != 1 ||
            pattern_type != FB_PATTERN_CUSTOM)
    {
        printf("Error: Invalid Params\n");
        return;
    }
    /* set other fields of logo frame descriptor */
    if (srcfmt == SRCFMT_YUV422) {
        vbufinfo->m_bytes_per_pixel = 2;
        vbufinfo->m_srcfmt = SRCFMT_YUV422;
        vbufinfo->m_order = ORDER_UYVY;
    } else {
        vbufinfo->m_bytes_per_pixel = 4;
        vbufinfo->m_srcfmt = SRCFMT_ARGB32;
        vbufinfo->m_order = ORDER_RGBA;
    }
    datasize = width * height * vbufinfo->m_bytes_per_pixel;
    vbufinfo->m_content_width = width;
    vbufinfo->m_content_height = height;
    vbufinfo->m_buf_stride = vbufinfo->m_content_width *
                    vbufinfo->m_bytes_per_pixel;
    vbufinfo->m_buf_size = (INT32)datasize;
    vbufinfo->m_flags = 1;
    vbufinfo->m_disp_offset = 0;
    vbufinfo->m_buf_stride = vbufinfo->m_content_width *
                    vbufinfo->m_bytes_per_pixel;
    vbufinfo->m_active_width = width;
    vbufinfo->m_active_height = height;
    vbufinfo->m_active_left = x;
    vbufinfo->m_active_top = y;
    vbufinfo->m_content_offset = 0;
    if (IsPatt) {
        if (srcfmt == SRCFMT_YUV422)
            make_frame_data(1, vbuf, 32, 36, width,
                height, 0x00800080, 0x00800080);
        else
            make_frame_data(0, vbuf, 32, 36, width,
                height, 0xFF000000, 0xFFFFFFFF);
    }
}
INT32 create_new_frame(INT32 width, INT32 height, INT32 progressive,
    INT32 src_fmt, INT32 bit_depth, INT32 pattern_type,
    VBUF_INFO **frame) {

    VBUF_INFO *vbufinfo;
    VOID *base = NULL;
    void*  phy_base;
    VOID *shm_handle;
    INT32 datasize = 0;
    INT32 result = 0;

    vbufinfo  = GaloisMalloc(sizeof(VBUF_INFO));
    if (vbufinfo == NULL)
        return -1;

    if (SRCFMT_YUV422 == src_fmt)
        datasize = width*height*2;
    else
        datasize = width*height*4;

    phy_base = base = shm_handle = GaloisMalloc(datasize);
    if (base == NULL) {
        result = MV_VPP_EBADCALL;
        printf("share memory get vitual address failed\n");
        goto EXIT;
    }

    build_frames(vbufinfo, base, src_fmt, bit_depth,
            0, 0, width, height,
            progressive, pattern_type, 1);
    vbufinfo->hShm  = shm_handle;
    vbufinfo->m_pbuf_start = phy_base;
    vbufinfo->m_bufferID = base;
    *frame = vbufinfo;
EXIT:
    return result;
}


VOID *MV_VPP_GetFrame(INT32 x,
            INT32 y, INT32 w, INT32 h, INT32 stride)
{
    int Ret = 0;
    VBUF_INFO *vbufinfo = gDescArray[gDescIndex];

    if(vbufinfo == NULL) {
#ifdef ENABLE_GFX
        Ret = create_new_frame(w, h, 1, SRCFMT_ARGB32,
                    INPUT_BIT_DEPTH_8BIT, FB_PATTERN_CUSTOM,
                    &(gDescArray[gDescIndex]));
#else
        Ret = create_new_frame(w, h, 1, SRCFMT_YUV422,
                    INPUT_BIT_DEPTH_8BIT, FB_PATTERN_CUSTOM,
                    &(gDescArray[gDescIndex]));
#endif
        if (Ret != 0) {
            printf("create_global_desc_array fails\n");
            return NULL;
        }
    }

    vbufinfo = gDescArray[gDescIndex];
    gDescIndex++;
    if (gDescIndex == MAX_FRAME_DESC)
        gDescIndex = 0;

    vbufinfo->m_active_left  = x;
    vbufinfo->m_active_top   = y;
    vbufinfo->m_buf_stride = stride;
    vbufinfo->m_active_width = w;
    vbufinfo->m_active_height = h;

    return vbufinfo;
}

VOID *MV_VPP_GetFrame_gfx(INT32 x,
            INT32 y, INT32 w, INT32 h, INT32 stride)
{
    UINT32 datasize;
    VBUF_INFO *vbufinfo = gDescArray_gfx[gDescIndex_gfx];

    if(vbufinfo == NULL) {
        vbufinfo  = GaloisMalloc(sizeof(VBUF_INFO));
        if (vbufinfo == NULL)
            return NULL;
        vbufinfo->m_bytes_per_pixel = 4;
        vbufinfo->m_srcfmt = SRCFMT_ARGB32;
        vbufinfo->m_order = ORDER_RGBA;

        datasize = w * h * vbufinfo->m_bytes_per_pixel;
        vbufinfo->m_content_width = w;
        vbufinfo->m_content_height = h;
        vbufinfo->m_buf_stride = vbufinfo->m_content_width *
                        vbufinfo->m_bytes_per_pixel;
        vbufinfo->m_buf_size = (INT32)datasize;
        vbufinfo->m_flags = 1;
        vbufinfo->m_disp_offset = 0;
        vbufinfo->m_buf_stride = vbufinfo->m_content_width *
                        vbufinfo->m_bytes_per_pixel;
        vbufinfo->m_active_width = w;
        vbufinfo->m_active_height = h;
        vbufinfo->m_active_left = x;
        vbufinfo->m_active_top = y;
        vbufinfo->m_content_offset = 0;
        gDescArray_gfx[gDescIndex_gfx] = vbufinfo;
    }

    vbufinfo = gDescArray_gfx[gDescIndex_gfx];
    gDescIndex_gfx++;
    if (gDescIndex_gfx == MAX_FRAME_DESC)
        gDescIndex_gfx = 0;

    vbufinfo->m_active_left  = x;
    vbufinfo->m_active_top   = y;
    vbufinfo->m_buf_stride = stride;
    vbufinfo->m_active_width = w;
    vbufinfo->m_active_height = h;

    return vbufinfo;
}

INT32 create_global_desc_array(VOID)
{
    GaloisMemSet(gDescArray, 0, MAX_FRAME_DESC * sizeof(VBUF_INFO *));
    return 0;
}

VOID destroy_global_desc_array(VOID)
{
    GaloisMemSet(gDescArray, 0, MAX_FRAME_DESC * sizeof(VBUF_INFO *));
}

