
/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2017 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated (“Synaptics”). The holder of this file shall treat all
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
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS,” AND
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
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS’ TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */

#include <linux/videodev2.h>
#include "v4l2_types.h"
#include "v4l2_m2m.h"
#include "v4l2_log.h"
#include "v4l2_misc.h"
#include <errno.h>

#define DTAG "[decoder]"
#define DTAG_BUF DTAG":[buf] "

#define VDEC_MIN_W 64
#define VDEC_MIN_H 64

#define VDEC_MAX_W  1920
#define VDEC_MAX_H  1088

#define VDEC_MAX_4K_W 4096
#define VDEC_MAX_4K_H 2160

#define VDEC_STEP  16

#define DEFAULT_W   VDEC_MIN_W
#define DEFAULT_H   VDEC_MIN_H

#define DEFAULT_OUT_FMT_ID 0
#define DEFAULT_CAP_FMT_ID 3
#define INVALID_CODEC_ID    -1

#define ALIGN_4K(x)  ((x) + 4095) / 4096 * 4096

static V4L2VidFmt vdec_video_format[] = {
#ifdef V4L2_PIX_FMT_H264
    {
        .fourcc = V4L2_PIX_FMT_H264,
        .type = FMT_ES,
        .num_planes = 1,
        .desc = "H.264",
    },
#endif
#ifdef V4L2_PIX_FMT_VP8
    {
        .fourcc = V4L2_PIX_FMT_VP8,
        .type = FMT_ES,
        .num_planes = 1,
        .desc = "VP8",
    },
#endif
#ifdef V4L2_PIX_FMT_VP9
    {
        .fourcc = V4L2_PIX_FMT_VP9,
        .type = FMT_ES,
        .num_planes = 1,
        .desc = "VP9",
    },
#endif
#ifdef V4L2_PIX_FMT_NV12
    {
        .fourcc = V4L2_PIX_FMT_NV12,
        .type = FMT_FRAME,
        .num_planes = 1,
        .desc = "Y/CbCr 4:2:0",
    }
#endif
};

static V4L2FrameSize vdec_codec_framesizes[] = {
#ifdef V4L2_PIX_FMT_H264
    {
        .fourcc = V4L2_PIX_FMT_H264,
        .stepwise = {VDEC_MIN_W, VDEC_MAX_4K_W, 16,
                     VDEC_MIN_H, VDEC_MAX_4K_H, 16 },
    },
#endif
#ifdef V4L2_PIX_FMT_VP8
    {
        .fourcc = V4L2_PIX_FMT_VP8,
        .stepwise = {VDEC_MIN_W, VDEC_MAX_W, 16,
                     VDEC_MIN_H, VDEC_MAX_H, 16 },
    },
#endif
#ifdef V4L2_PIX_FMT_VP9
    {
        .fourcc = V4L2_PIX_FMT_VP9,
        .stepwise = {VDEC_MIN_W, VDEC_MAX_4K_W, 16,
                     VDEC_MIN_H, VDEC_MAX_4K_H, 16 },
    },
#endif
 };

static const unsigned int eof_padding[] =
{
    0xff010000, 0x04000081,
    0x88888801, 0x88888888,
    0x88888888, 0x88888888,
    0x88888888, 0x88888888,
};
static const unsigned int uf_padding[] =
{
    0xff010000, 0x0400008f,
    0x8888880f, 0x88888888,
    0x88888888, 0x88888888,
    0x88888888, 0x88888888,
};
static const unsigned int eos_padding[] =
{
    0xff010000, 0x04000080,
    0x88888800, 0x88888888,
    0x88888888, 0x88888888,
    0x88888888, 0x88888888,
};

typedef struct {
    UINT32 frame_size;
    signed long long int  pts;
}__attribute__ ((packed)) VP9_FHDR;

typedef struct {
    UINT32 signature;
    UINT16 version;
    UINT16 hdr_len;
    UINT32 fcc;
    UINT16 width;
    UINT16 height;
    UINT32 frame_rate;
    UINT32 time_scale;
    UINT32 num_frames;
    UINT32 reserved;
    UINT32 frame_size;
    signed long long int  pts;
}__attribute__ ((packed)) VP9_IVF;


#define DEC_NUM_FORMAT  sizeof(vdec_video_format)/sizeof(vdec_video_format[0])
#define DEC_NUM_FRAMESIZE  sizeof(vdec_codec_framesizes)/sizeof(vdec_codec_framesizes[0])
static HRESULT vdec_setHWstate(V4L2M2MContext *ctx, AMP_STATE eState);

static UINT32  v4l2FmtToCodec(UINT32 fmt)
{
    switch (fmt) {
        case V4L2_PIX_FMT_H264:
            return MEDIA_VES_AVC;
        case V4L2_PIX_FMT_VP8:
            return MEDIA_VES_VP8;
        case V4L2_PIX_FMT_VP9:
            return MEDIA_VES_VP9;
        default:
            return INVALID_CODEC_ID;
    }
}

static bool is_vpx_format(UINT32 fmt)
{
    return ((fmt == V4L2_PIX_FMT_VP8)
            || (fmt == V4L2_PIX_FMT_VP9));
}

static HRESULT formatesdata_h264(AMP_BD_HANDLE hBD)
{
    HRESULT ret = V4L2_SUCCESS;
    AMP_BDTAG_MEMINFO *pMem;
    /* the support for the legacy vmeta ES padding is removed */

    ret = AMP_BDTAG_GetWithIndex(hBD, 0, (void*)&pMem);
    ASSERT_RET_SHOW(ret, "error!! get mem tag failed");

    if (!pMem->uSize)
        pMem->uFlag = AMP_MEMINFO_FLAG_EOS_MASK;

    if (pMem->uSize != 0)
        AMP_SHM_CleanCache(pMem->uMemHandle, pMem->uMemOffset, pMem->uSize);

    return V4L2_SUCCESS;
}

static HRESULT formatesdata_vpx(AMP_BD_HANDLE hBD, V4L2M2MContext *ctx)
{
    HRESULT ret = V4L2_SUCCESS;
    UINT32 priv_len;
    V4L2DecoderCtx *dctx = &ctx->codec.ctx.dec;
    void *buf;

    AMP_BDTAG_MEMINFO *pMem;
    ret = BD_GetMemInfo(hBD, &pMem);
    ASSERT_RET_SHOW(ret, "error!! get meminfo failed\n");

    ret = AMP_SHM_GetVirtualAddress(pMem->uMemHandle, 0, (void **)&buf);
    ASSERT_RET_SHOW(ret, "error!! get mem tag failed");


    if(pMem->uSize + sizeof(VP9_IVF)> 0x400000) {
        V4L2LOGE("error!! failed format vpx data %d\n", pMem->uSize);
        return ERROR_EINVAL;
    }
    if(pMem->uSize == 0) {
        //payload=0, vpx no need  eos padding
        V4L2LOGD("send eos bd\n");
        return V4L2_SUCCESS;
    }

    if(!dctx->ivf_header_done)
    { //the first IVF header shall not be neceesary. decoder will remove it later
        priv_len = sizeof(VP9_IVF);
        VP9_IVF *IVFheader = (VP9_IVF *)dctx->ivf_header_data;
        IVFheader->signature  = 0x46494b44;
        IVFheader->version = 0x0;
        IVFheader->hdr_len  = 0x20;
        if (ctx->fourcc == V4L2_PIX_FMT_VP8) {
            IVFheader->fcc   = 0x30385056; // hard code value: VP80, little endian
        }
        if (ctx->fourcc == V4L2_PIX_FMT_VP9) {
            IVFheader->fcc   = 0x30395056; // hard code value: VP90, little endian
        }
        IVFheader->frame_rate = 0;
        IVFheader->time_scale = 0;
        IVFheader->width = 176;
        IVFheader->height = 144;
        IVFheader->num_frames = 1000;
        IVFheader->reserved = 0x0;

        IVFheader->frame_size = pMem->uSize;
        IVFheader->pts = 0;
        memmove((void *)((uintptr_t)buf + priv_len), buf, pMem->uSize);
        memcpy(buf, IVFheader, priv_len);
        dctx->ivf_header_done = TRUE;
    }
    else
    {
        priv_len = sizeof(VP9_FHDR);
        VP9_FHDR *VP9header = (VP9_FHDR *)dctx->ivf_header_data;
        VP9header->frame_size = pMem->uSize;
        VP9header->pts = 0;
        memmove((void *)((uintptr_t)buf + priv_len), buf, pMem->uSize);
        memcpy(buf, VP9header, priv_len);
    }

    pMem->uSize += priv_len;
    AMP_SHM_CleanCache(pMem->uMemHandle, 0, pMem->uSize);
    return V4L2_SUCCESS;
}


static HRESULT formatesdata(AMP_BD_HANDLE hBD, V4L2M2MContext *pCtx)
{
    UINT32 fourcc = pCtx->fourcc;

    if(fourcc == V4L2_PIX_FMT_H264) {
        formatesdata_h264(hBD);
    } else if (is_vpx_format(fourcc)) {
        formatesdata_vpx(hBD, pCtx);
    } else {
        V4L2LOGE("error!! unsupported codec\n");
        return ERROR_EINVAL;
    }
    return V4L2_SUCCESS;
}

static HRESULT vdec_callbck(CORBA_Object hCompObj, AMP_PORT_IO ePortIo,
                             UINT32 uiPortIdx, struct AMP_BD_ST *hBD,
                             AMP_IN void *pUserData)
{
    V4L2M2MContext *fh = (V4L2M2MContext*)pUserData;
    V4L2Stream *strm;

    strm = (ePortIo == AMP_PORT_INPUT)? &fh->output: &fh->capture;
    V4L2LOGD("%s Return BD %p, %d\n", (ePortIo == 0)?
                "AMP_PORT_INPUT":"AMP_PORT_OUTPUT", hBD, AMP_BD_GET_BDID(hBD));

    v4l2_buffer_EnqueBD(strm, hBD);
    if((ePortIo == AMP_PORT_OUTPUT) && is_validDispBD(hBD)){
        dump_PortBD(&fh->capture, hBD);
    }

    return V4L2_SUCCESS;
}

static void vdec_queue_res_change_event(void *fh)
{
    static const struct v4l2_event ev_res_ch = {
        .type = V4L2_EVENT_SOURCE_CHANGE,
        .u.src_change.changes = V4L2_EVENT_SRC_CH_RESOLUTION,
    };

    v4l2_event_queue(fh, &ev_res_ch);
    V4L2LOGD("queued one res_change event\n");
}

static void vdec_update_picInfo(V4L2M2MContext *fh, AMP_VDEC_RES_CHANGE_DESC *pDesc)
{

    VdecPicInfo* pInfo = &fh->codec.ctx.dec.picinfo;
    UINT32 stride = 0;

    pInfo->align_w = align(pDesc->uiBufferWidth, 128);
    pInfo->align_h = align(pDesc->uiBufferHeight, 128);
    pInfo->pic_w = pDesc->uiFrameWidth;
    pInfo->pic_h = pDesc->uiFrameHeight;
    /**
     * Setting back the buf_num to total buffer number (pDesc->uiBufferNum)
     * from the "accumDisBufNum" API which includes the minimum buffer required
     * plus extra buffer needed since display rendering requires more buffers
     */
    pInfo->buf_num = pDesc->uiBufferNum;
    pInfo->buf_size = pDesc->uiBufferSize;
    pInfo->crop_l = pDesc->uiCropLeft;
    pInfo->crop_t = pDesc->uiCropTop;
    pInfo->crop_w = pDesc->uiCropWidth;
    pInfo->crop_h = pDesc->uiCropHeight;
    pInfo->sar_w = pDesc->uiSarWidth;
    pInfo->sar_h = pDesc->uiSarHeight;
    pInfo->bitdepth = pDesc->uiBitDepth;
    //calc y size and uv size
    stride = align(pDesc->uiBufferWidth * pInfo->bitdepth / 8, 128);
    pInfo->buf_size_y = align(stride * pInfo->align_h, 4096);
    pInfo->buf_size_uv = pInfo->buf_size_y >> 1;

    V4L2LOGH("ResChange align-%dx%d(pic-%dx%d), buf-%d,alloc:(%d+%d)*%d crop(%d,%d,%d,%d),sar(%d,%d)\n",
             pInfo->align_w, pInfo->align_h,
             pInfo->pic_w, pInfo->pic_h,
             pInfo->buf_size, pInfo->buf_size_y,
             pInfo->buf_size_uv, pInfo->buf_num,
             pInfo->crop_l, pInfo->crop_t,
             pInfo->crop_w, pInfo->crop_h,
             pInfo->sar_w, pInfo->sar_h);

    //Update V4L2 fmt
    struct v4l2_pix_format_mplane *pix_mp = &fh->capture.format.fmt.pix_mp;
    pix_mp->width = pInfo->align_w;
    pix_mp->height = pInfo->align_h;
    pix_mp->plane_fmt[0].bytesperline = stride;
    pix_mp->plane_fmt[0].sizeimage = pInfo->buf_size_y + pInfo->buf_size_uv;
    //pix_mp->plane_fmt[1].bytesperline = stride;
    //pix_mp->plane_fmt[1].sizeimage = pInfo->buf_size_uv;

}

static HRESULT  vdec_event_handler(HANDLE hListener, AMP_EVENT *pEvent,
                                        VOID *pUserData)
{
    UINT32 *payload = AMP_EVENT_PAYLOAD_PTR(pEvent);
    V4L2M2MContext *fh = (V4L2M2MContext*)pUserData;

    switch (AMP_EVENT_GETCODE(*pEvent)) {
        case AMP_EVENT_API_VDEC_CALLBACK:
            switch (AMP_EVENT_GETPAR1(*pEvent)) {
                case AMP_VDEC_EVENT_RES_CHANGE:
                    V4L2LOGD("res change got \n");
                    vdec_update_picInfo(fh, (AMP_VDEC_RES_CHANGE_DESC *)payload);
                    vdec_queue_res_change_event(fh);
                    v4l2_setState(fh, STATE_HEADER);
                break;
                case AMP_VDEC_EVENT_STREAM_DONE:
                    V4L2LOGH("streamdone\n");
                    fh->capture.last_buff_sent = 1;
                break;
            }
        break;

        default:
        break;
    }

    return V4L2_SUCCESS;
}

static HRESULT vdec_register_notify(V4L2M2MContext *fh)
{
    HRESULT ret = SUCCESS;
    AMP_COMPONENT vdec = fh->comp;

    fh->hListener = AMP_Event_CreateListener(16, 0);
    ret = AMP_Event_RegisterCallback(fh->hListener,
                    AMP_EVENT_API_VDEC_CALLBACK,
                    vdec_event_handler, fh);
    ASSERT_RET_SHOW(ret, "register callback failed");

    AMP_RPC(ret, AMP_VDEC_RegisterNotify, fh->comp,
           AMP_Event_GetServiceID(fh->hListener), AMP_EVENT_API_VDEC_CALLBACK);
    ASSERT_RET_SHOW(ret, "register Notify failed");

    return ret;
}

static HRESULT vdec_unregister_notify(V4L2M2MContext *fh)
{
    HRESULT ret = SUCCESS;
    AMP_RPC(ret, AMP_VDEC_UnregisterNotify, fh->comp,
            AMP_Event_GetServiceID(fh->hListener), AMP_EVENT_API_VDEC_CALLBACK);
    ASSERT_RET_SHOW(ret, "unregister notify failed");

    ret = AMP_Event_UnregisterCallback(fh->hListener,
                AMP_EVENT_API_VDEC_CALLBACK, vdec_event_handler);
    ASSERT_RET_SHOW(ret, "unregister callback failed");

    ret = AMP_Event_DestroyListener(fh->hListener);
    ASSERT_RET_SHOW(ret, "destroy listener  failed");

    return ret;
}

static HRESULT vdec_push_bd(void *fh, AMP_PORT_IO port, AMP_BD_HANDLE hBD)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2M2MContext *ctx = (V4L2M2MContext *)fh;
    AMP_COMPONENT comp = ctx->comp;

    V4L2LOGD("\t\t%s Push BD %p \n", (port == 0)?
                    "AMP_PORT_INPUT":"AMP_PORT_OUTPUT", hBD);
    if(port == AMP_PORT_INPUT) {
        formatesdata(hBD, ctx);
     //   dump_PortBD(&ctx->output, hBD);
    }
    AMP_RPC(ret, AMP_VDEC_PushBD, comp, port, 0, hBD);
    ASSERT_RET_SHOW(ret, "vdec push port faild");

    return ret;
}

static HRESULT vdec_setHWstate(V4L2M2MContext *ctx, AMP_STATE eState)
{
    HRESULT ret = V4L2_SUCCESS;

    V4L2LOGH("set HWstate %s \n",
        (eState == AMP_EXECUTING)? "AMP_EXECUTING":"AMP_IDLE");
    ctx->codec.hwState = eState;

    if(eState == AMP_EXECUTING || eState == AMP_IDLE) {
        AMP_RPC(ret, AMP_VDEC_SetState, ctx->comp, eState);
        ASSERT_RET_SHOW(ret, "vdec set state faild");
    }
    return ret;
}

static HRESULT vdec_set_ctx_default_params(V4L2M2MContext *fh)
{
    struct v4l2_pix_format_mplane *pix_fmt_mp = NULL;
    struct v4l2_plane_pix_format *plane = NULL;
    V4L2Stream *strm;

    //src: output
    strm = &fh->output;
    strm->width = DEFAULT_W;
    strm->height = DEFAULT_H;

    pix_fmt_mp = &strm->format.fmt.pix_mp;
    pix_fmt_mp->colorspace = V4L2_COLORSPACE_REC709;
    //pix_fmt_mp->ycbcr_enc = V4L2_YCBCR_ENC_DEFAULT;  // We need update build_sysroot for android
    //pix_fmt_mp->quantization = V4L2_QUANTIZATION_DEFAULT;
    //pix_fmt_mp->xfer_func = V4L2_XFER_FUNC_DEFAULT;
    pix_fmt_mp->field = V4L2_FIELD_NONE;
    pix_fmt_mp->pixelformat = V4L2_PIX_FMT_H264;

    pix_fmt_mp->width = DEFAULT_W;
    pix_fmt_mp->height = DEFAULT_H;
    //output planes
    pix_fmt_mp->num_planes = 1; //1 for es format
    plane = &pix_fmt_mp->plane_fmt[0];
    plane->sizeimage = 1024*1024;
    plane->bytesperline = 0;

    //dst :capture
    strm = &fh->capture;
    pix_fmt_mp = &strm->format.fmt.pix_mp;
    pix_fmt_mp->colorspace = V4L2_COLORSPACE_REC709;
   // pix_fmt_mp->ycbcr_enc = V4L2_YCBCR_ENC_DEFAULT;
   // pix_fmt_mp->quantization = V4L2_QUANTIZATION_DEFAULT;
   // pix_fmt_mp->xfer_func = V4L2_XFER_FUNC_DEFAULT;
    pix_fmt_mp->field = V4L2_FIELD_NONE;
    pix_fmt_mp->width = DEFAULT_W;
    pix_fmt_mp->height = DEFAULT_H;
    pix_fmt_mp->pixelformat = V4L2_PIX_FMT_NV12;
    //output planes
    pix_fmt_mp->num_planes = 1; //1 for nv12 format
    //y plane
    plane = &pix_fmt_mp->plane_fmt[0];
    plane->sizeimage = ALIGN_4K(DEFAULT_W * DEFAULT_H) * 1.5;
    plane->bytesperline = DEFAULT_W;
    //uv plane
    //plane = &pix_fmt_mp->plane_fmt[1];
    //plane->sizeimage = ALIGN_4K(DEFAULT_W * DEFAULT_H/2);
    //plane->bytesperline = DEFAULT_W;

    return SUCCESS;
}

static V4L2VidFmt *vdec_find_format(struct v4l2_format *f)
{
  V4L2VidFmt *fmt;
  unsigned int i;

  for (i = 0; i < DEC_NUM_FORMAT; i++) {
    fmt = &vdec_video_format[i];
    if (fmt->fourcc == f->fmt.pix_mp.pixelformat)
      return fmt;
  }
  return NULL;
}

static HRESULT internal_vdec_open(V4L2M2MContext *fh)
{
    HRESULT ret;
    AMP_COMPONENT_CONFIG config;
    AMP_COMPONENT vdec = fh->comp;

    UINT32 video_codec = v4l2FmtToCodec(fh->fourcc);
    if (INVALID_CODEC_ID == video_codec)
        return ERROR_EINVAL;

    UINT32 mode = AMP_NON_TUNNEL;
    UINT32 flag = 0;

    //frame in
    flag |= AMP_VDEC_MODE_FRAME_IN;
    //spuv output
    flag |= 3 << 10;//420SP

    AmpMemClear(&config, sizeof(AMP_COMPONENT_CONFIG));
    config._d = AMP_COMPONENT_VDEC;
    config._u.pVDEC.mode = mode;
    config._u.pVDEC.uiType = video_codec;
    config._u.pVDEC.uiFlag = flag;
    AMP_RPC(ret, AMP_VDEC_Open, vdec, &config);
    ASSERT_RET_SHOW(ret, "open vdec failed");

    fh->codec.hwState = AMP_IDLE;

    return ret;

}

static int vdec_try_decode(V4L2M2MContext *fh)
{

    //check codec stauts
    if(fh->codec.state < STATE_WORKING) {
        V4L2LOGH("state wrong(%d)\n", fh->codec.state);
        goto try_fail;
    }
    switch(fh->codec.state)
    {
        case STATE_WORKING:
        case STATE_DRAIN:
        case STATE_HEADER:
            if(fh->codec.hwState == AMP_IDLE) {
                vdec_setHWstate(fh, AMP_EXECUTING);
            }
            v4l2_stream_enqueue_hw((void *)fh, (PushBDFuc)vdec_push_bd);
            break;
        default:
            V4L2LOGH("state wrong(%d)\n", fh->codec.state);
            break;
    }

 try_fail:
    return 0;
}

static int vdec_try_stop_decode(V4L2M2MContext *fh)
{
    V4L2_API_ENTRY();
    if((fh->capture.streamon == FALSE)
        && (fh->output.streamon == FALSE)) {
        vdec_setHWstate(fh, AMP_IDLE);
    }
    return 0;
}
//VIDIOC_TRY_DECODER_CMD can be used to try a command without actually executing it.
static int vidioc_try_decoder_cmd(void *fh, void *p)
{
    struct v4l2_decoder_cmd *cmd = (struct v4l2_decoder_cmd *)p;

    switch (cmd->cmd) {
        case V4L2_DEC_CMD_STOP:
        case V4L2_DEC_CMD_START:
            if (cmd->flags != 0) {
                V4L2LOGE("cmd->flags=%u", cmd->flags);
                return ERROR_EINVAL;
            }
        break;
        default:
        return ERROR_EINVAL;
    }

    return 0;
}

static int vidioc_decoder_cmd(void *fh, void *p)
{
    M2MCTX(fh)
    int ret;
    struct v4l2_decoder_cmd *cmd = (struct v4l2_decoder_cmd *)p;
    V4L2Stream *output, *capture;

    ret = vidioc_try_decoder_cmd(fh, p);
    if (ret)
        return ret;

    V4L2LOGH("decoder: %s\n", (cmd->cmd==0)? "start":"stop");
    capture = &ctx->capture;
    output = &ctx->output;

    switch (cmd->cmd) {
    case V4L2_DEC_CMD_STOP:
        if (!output->streamon) {
            V4L2LOGE("output stream is off. No need to flush.");
            return 0;
        }
        if (!capture->streamon) {
            V4L2LOGE("capture stream is off. No need to flush.");
            return 0;
        }
        v4l2_setState(fh, STATE_DRAIN);
        v4l2_buffer_EnqueFlushBuf((void *)output);
        break;

    case V4L2_DEC_CMD_START:
        v4l2_setState(fh, STATE_WORKING);
        break;

    default:
        return ERROR_EINVAL;

    }
    vdec_try_decode(fh);

    return 0;
}

static int vidioc_vdec_streamon(void *fh, void *p)
{
    enum v4l2_buf_type type = *(unsigned int *)p;

    V4L2LOGH("%s on\n", buf2str(type));
    V4L2Stream *strm = v4l2_get_stream(fh, type);
    v4l2_stream_on(strm);

    v4l2_setState(fh, STATE_WORKING);

    vdec_try_decode(fh);

    return 0;
}

static int vidioc_vdec_streamoff(void *fh, void *p)
{
    enum v4l2_buf_type type = *(unsigned int *)p;
    V4L2Stream *strm = v4l2_get_stream(fh, type);

    V4L2LOGH("%s off\n", buf2str(type));
    if(V4L2_TYPE_IS_OUTPUT(type))
        v4l2_setState(fh, STATE_SEEK);
    else
        v4l2_setState(fh, STATE_STOP);

    v4l2_stream_off(strm);

    vdec_try_stop_decode((V4L2M2MContext*)fh);

    return 0;
}


static int vidioc_vdec_g_selection(void *fh, void *p)
{
    M2MCTX(fh)
    struct v4l2_selection *s = (struct v4l2_selection *)p;
    V4L2DecoderCtx *dctx = &ctx->codec.ctx.dec;

    if (s->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
    return ERROR_EINVAL;

    switch (s->target) {
        case V4L2_SEL_TGT_COMPOSE_DEFAULT:
            s->r.left = 0;
            s->r.top = 0;
            s->r.width = dctx->picinfo.pic_w;
            s->r.height = dctx->picinfo.pic_h;
        break;
        case V4L2_SEL_TGT_COMPOSE_BOUNDS:
            s->r.left = 0;
            s->r.top = 0;
            s->r.width = dctx->picinfo.align_w;
            s->r.height = dctx->picinfo.align_h;
        break;
        case V4L2_SEL_TGT_COMPOSE:
            s->r.left = dctx->picinfo.crop_l;
            s->r.top = dctx->picinfo.crop_t;
            s->r.width = dctx->picinfo.crop_w;
            s->r.height = dctx->picinfo.crop_h;
        break;
        default:
        return ERROR_EINVAL;
    }

    if (ctx->codec.state < STATE_HEADER) {
        /* set to default value if header info not ready yet*/
        s->r.left = 0;
        s->r.top = 0;
        s->r.width = DEFAULT_W;
        s->r.height = DEFAULT_H;
        return 0;
    }

    V4L2LOGH("g_selection %d, %d, %d, %d\n", s->r.left, s->r.top,
             s->r.width, s->r.height);

    return 0;
}

static int vidioc_vdec_s_selection(void *fh, void *p)
{
    M2MCTX(fh)
    struct v4l2_selection *s = (struct v4l2_selection *)p;
    V4L2DecoderCtx *dctx = &ctx->codec.ctx.dec;

    if (s->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
        return ERROR_EINVAL;

    switch (s->target) {
    case V4L2_SEL_TGT_COMPOSE:
        s->r.left = 0;
        s->r.top = 0;
        s->r.width = dctx->picinfo.pic_w;
        s->r.height = dctx->picinfo.pic_h;
        break;
    default:
        return ERROR_EINVAL;
    }

    return 0;
}


static int vidioc_vdec_enum_framesizes(void *fh, void *p)
{
    struct v4l2_frmsizeenum *fsize = (struct v4l2_frmsizeenum *)p;
    int i = 0;

    if (fsize->index != 0)
        return ERROR_EINVAL;

    for (i = 0; i < DEC_NUM_FRAMESIZE; ++i) {
        if (fsize->pixel_format != vdec_codec_framesizes[i].fourcc)
            continue;

        fsize->type = V4L2_FRMSIZE_TYPE_STEPWISE;
        fsize->stepwise = vdec_codec_framesizes[i].stepwise;

        return 0;
    }
    return ERROR_EINVAL;
}

static int __vidioc_vdec_enum_fmt(struct v4l2_fmtdesc *f, bool bOutput)
{
    V4L2VidFmt *fmt;
    int i;
    int j = 0;

    for (i = 0; i < DEC_NUM_FORMAT; i++) {
        if (bOutput && vdec_video_format[i].type != FMT_ES)
            continue;
        if (!bOutput && vdec_video_format[i].type != FMT_FRAME)
            continue;
        if (j == f->index)
            break;

        ++j;
    }

    if (i == DEC_NUM_FORMAT)
        return ERROR_EINVAL;

    fmt = &vdec_video_format[i];

    //update v4l2_fmtdesc
    f->pixelformat = fmt->fourcc;
    f->flags = (fmt->type == FMT_ES) ? V4L2_FMT_FLAG_COMPRESSED : 0;
    if (fmt->desc) {
        int rsz = strlen(fmt->desc);
        char *buff = (char*)f->description;
        const unsigned sz = sizeof(f->description);
        if (rsz > sz)
            strncpy(buff, fmt->desc, sz);
        else
            strcpy(buff, fmt->desc);

        buff[sz - 1] = '\0';
    }
    return 0;
}

static int vidioc_vdec_enum_fmt(void *fh, void *p)
{
    struct v4l2_fmtdesc *f = (struct v4l2_fmtdesc *)p;
    if(V4L2_TYPE_IS_OUTPUT(f->type))
        return __vidioc_vdec_enum_fmt(f, true);
    return __vidioc_vdec_enum_fmt(f, false);
}

static int vidioc_vdec_s_fmt(void *fh, void *p)
{
    int ret = 0;
    struct v4l2_format *fmt = (struct v4l2_format *)p;
    struct v4l2_pix_format_mplane *dst_pix_mp, *src_pix_mp;

    if(!V4L2_TYPE_IS_MULTIPLANAR(fmt->type)) {
        V4L2LOGE("Unsupported v4l2 format \n");
        return ERROR_EINVAL;
    }

    V4L2Stream *strm = v4l2_get_stream(fh, fmt->type);

   dst_pix_mp = &strm->format.fmt.pix_mp;
   src_pix_mp = &fmt->fmt.pix_mp;

    show_v4l2fmt(fmt);
    if(fmt->type == V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE) {
        strm->format.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
        dst_pix_mp->pixelformat = src_pix_mp->pixelformat;
        dst_pix_mp->num_planes = src_pix_mp->num_planes;
        dst_pix_mp->plane_fmt[0].sizeimage = src_pix_mp->plane_fmt[0].sizeimage;
    } else if(fmt->type == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE) {
        strm->format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        dst_pix_mp->pixelformat = src_pix_mp->pixelformat;
    }
    ret = v4l2_stream_set_format(strm, fmt);
    show_v4l2fmt(&strm->format);
    return ret;
}

static int vidioc_vdec_g_fmt(void *fh, void *p)
{
    int ret = 0;
    struct v4l2_format *fmt = (struct v4l2_format *)p;
    V4L2Stream *strm = v4l2_get_stream(fh, fmt->type);

    ret = v4l2_stream_get_format(strm, fmt);
    if ((fmt->type == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE)
        && (fmt->fmt.pix_mp.width == DEFAULT_W)) {
        errno = EINVAL;
        ret = ERROR_EINVAL;
    }
    show_v4l2fmt(fmt);
    return ret;
}

//only found it in in image processer for chome gpu v4l2 code.
static int vidioc_vdec_try_fmt(void *fh, void *p)
{
    int i=0;

    struct v4l2_format *f = (struct v4l2_format *)p;
    struct v4l2_pix_format_mplane *pix_fmt_mp = &f->fmt.pix_mp;
    int idx = V4L2_TYPE_IS_OUTPUT(f->type) ? DEFAULT_OUT_FMT_ID : DEFAULT_CAP_FMT_ID;

    V4L2VidFmt *fmt = vdec_find_format(f);
    if(!fmt) {
        pix_fmt_mp->pixelformat= vdec_video_format[idx].fourcc;
        fmt = &vdec_video_format[idx];
    }

    if(V4L2_TYPE_IS_OUTPUT(f->type)) {
        pix_fmt_mp->num_planes = 1;
        pix_fmt_mp->plane_fmt[0].bytesperline = 0;
    } else {
        pix_fmt_mp->num_planes = fmt->num_planes;
        pix_fmt_mp->width = DEFAULT_W;
        pix_fmt_mp->height = DEFAULT_H;  //TODO: chekc this data if valid
        pix_fmt_mp->plane_fmt[0].bytesperline = pix_fmt_mp->width;
        pix_fmt_mp->plane_fmt[0].sizeimage = pix_fmt_mp->width * pix_fmt_mp->height;
        if(fmt->num_planes == 2) {
            pix_fmt_mp->plane_fmt[1].bytesperline = pix_fmt_mp->width;
            pix_fmt_mp->plane_fmt[1].sizeimage = pix_fmt_mp->width * pix_fmt_mp->height /2;
        }
        for(i=0; i<pix_fmt_mp->num_planes; i++){
            memset(&pix_fmt_mp->plane_fmt[i].reserved, 0, sizeof(pix_fmt_mp->plane_fmt[0].reserved));
        }
    }

    //pix_fmt_mp->flags = 0;
    pix_fmt_mp->field = V4L2_FIELD_NONE;
    memset(&pix_fmt_mp->reserved, 0 , sizeof(pix_fmt_mp->reserved));

    return 0;
}

static int vidioc_vdec_qbuf(void *fh, void *p)
{
    struct v4l2_buffer *buf = (struct v4l2_buffer *)p;

    V4L2M2MContext *ctx = (V4L2M2MContext *)fh;
    //do not feed buffer when decoder is stopped such as while changing resolution dynamically
    if(ctx->codec.state >= STATE_STOP)
        return 0;

    v4l2_buffer_QBuff(fh, buf);
    vdec_try_decode(fh);

    return 0;
}

static int vidioc_vdec_dqbuf(void *fh, void *p)
{
    struct v4l2_buffer *buf = (struct v4l2_buffer *)p;

    return v4l2_buffer_DQBuff(fh, buf);
}

static int vidioc_vdec_querybuf(void *fh, void *p)
{
    struct v4l2_buffer *buf = (struct v4l2_buffer *)p;

    return v4l2_buffer_QueryBuff(fh, buf);
}

static int vidioc_vdec_reqbufs(void *fh, void *p)
{
    struct v4l2_requestbuffers * req;
    req = (struct v4l2_requestbuffers *)p;

    V4L2Stream *strm = v4l2_get_stream(fh, req->type);

    return v4l2_stream_ReqBuff(strm, req);
}

static int vidioc_vdec_expbuf(void *fh, void *eb)
{
    return v4l2_buffer_expbuf(fh, eb);
}

static int vidioc_vdec_subscribe_event(void *fh, void *p)
{
    struct v4l2_event_subscription* sub;
    sub = (struct v4l2_event_subscription*)p;

    V4L2LOGD("event type %s\n", event2str(sub->type));
    switch(sub->type) {
        case V4L2_EVENT_EOS:
            return v4l2_event_subscribe(fh, sub, 2, NULL);
        case V4L2_EVENT_SOURCE_CHANGE:
            return v4l2_src_change_event_subscribe(fh, sub);
        default :
            return v4l2_event_subscribe(fh, sub, 0, NULL);
        //TODO: are we going to support control sub event?
    }
}
static int vidioc_vdec_unsubscribe_event(void *fh, void *p)
{
    struct v4l2_event_subscription* sub;
    sub = (struct v4l2_event_subscription*)p;

    return v4l2_event_unsubscribe(fh, sub);
}

static int vidioc_vdec_dqevent(void *fh, void *p)
{
    M2MCTX(fh)
    struct v4l2_event* e = (struct v4l2_event *)p;

    return v4l2_event_dequeue(fh, e, ctx->nonblocking);
}

static int vidioc_vdec_g_ctrl(void *fh, void *p)
{
    int ret = 0;
    M2MCTX(fh)
    struct v4l2_control *c = (struct v4l2_control *)p;
    V4L2DecoderCtx *dctx = &ctx->codec.ctx.dec;
    switch(c->id){
        case V4L2_CID_MIN_BUFFERS_FOR_CAPTURE:
            c->value = dctx->picinfo.buf_num;
        break;
        default:
            ret = ERROR_EINVAL;
        break;

    }

    V4L2LOGD("getctrl id %d val %d", c->id, c->value);
    return ret;
}

HRESULT v4l2_decoder_init(V4L2M2MContext *fh)
{
    HRESULT ret = SUCCESS;

    V4L2_API_ENTRY();

    //create vdec comp
    AMP_COMPONENT vdec;
    AMP_RPC(ret, AMP_FACTORY_CreateComponent,
                fh->pV4l2Ctx->amp_factory,
                AMP_COMPONENT_VDEC, 0,
                &vdec);
    ASSERT_RET_SHOW(ret, "create vdec failed");

    //fillin m2m ctx
    fh->comp = vdec;
    V4L2LOGH(DTAG"new instance %p\n", vdec);

    internal_vdec_open(fh);

    //register buffer return callback
    ret = AMP_ConnectApp(vdec, AMP_PORT_INPUT, 0, vdec_callbck, fh);
    ASSERT_RET_SHOW(ret, "connect input failed");

    ret = AMP_ConnectApp(vdec, AMP_PORT_OUTPUT, 0, vdec_callbck, fh);
    ASSERT_RET_SHOW(ret, "connect output failed");

    //register event notify
    vdec_register_notify(fh);

    //default pix_mp value
    vdec_set_ctx_default_params(fh);
    //flush BD
    createFlushBDBuffer((void *)fh);

    //! only need to override vdec own function
    //! buffer
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_QBUF, vidioc_vdec_qbuf);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_DQBUF, vidioc_vdec_dqbuf);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_QUERYBUF, vidioc_vdec_querybuf);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_REQBUFS, vidioc_vdec_reqbufs);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_EXPBUF, vidioc_vdec_expbuf);
    //! fomat
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_ENUM_FMT, vidioc_vdec_enum_fmt);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_TRY_FMT, vidioc_vdec_try_fmt);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_G_FMT, vidioc_vdec_g_fmt);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_S_FMT, vidioc_vdec_s_fmt);
    //! event
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_SUBSCRIBE_EVENT, vidioc_vdec_subscribe_event);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_UNSUBSCRIBE_EVENT, vidioc_vdec_unsubscribe_event);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_DQEVENT, vidioc_vdec_dqevent);
    //control
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_G_CTRL, vidioc_vdec_g_ctrl);
    //OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_S_CTRL, vidioc_vdec_s_ctrl);
    //selection(crop)
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_G_SELECTION, vidioc_vdec_g_selection);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_S_SELECTION, vidioc_vdec_s_selection);
    //frame
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_ENUM_FRAMESIZES, vidioc_vdec_enum_framesizes);
    //stream
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_STREAMON, vidioc_vdec_streamon);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_STREAMOFF, vidioc_vdec_streamoff);
     //decoder
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_DECODER_CMD, vidioc_decoder_cmd);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_TRY_DECODER_CMD, vidioc_try_decoder_cmd);

    V4L2_API_EXIT(ret);
    return ret;

}

HRESULT v4l2_decoder_deinit(V4L2M2MContext *fh)
{
    HRESULT ret = SUCCESS;

    V4L2_API_ENTRY();
    AMP_COMPONENT vdec = fh->comp;

    vdec_unregister_notify(fh);

    deleteFlushBDBuffer(fh->output.flush_buf->hBD);

    ret = AMP_DisconnectApp(vdec, AMP_PORT_INPUT, 0, vdec_callbck);
    ASSERT_RET_SHOW(ret, "disconnect input failed");

    ret = AMP_DisconnectApp(vdec, AMP_PORT_OUTPUT, 0, vdec_callbck);
    ASSERT_RET_SHOW(ret, "disconnect output failed");

    AMP_RPC(ret, AMP_VDEC_Close, vdec);
    ASSERT_RET_SHOW(ret, "close failed");

    AMP_RPC(ret, AMP_VDEC_Destroy, vdec);
    ASSERT_RET_SHOW(ret, "distroy vdec failed");

    V4L2LOGH(DTAG"destroy instance %p\n", vdec);
    V4L2_API_EXIT(ret);
    return ret;
}


