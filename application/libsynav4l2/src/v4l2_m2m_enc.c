
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

#define VENC_MIN_W 144
#define VENC_MIN_H 96

#define VENC_MAX_W  1920
#define VENC_MAX_H  1088
#define VENC_MAX_4K_W 4096
#define VENC_MAX_4K_H 2160

#define VENC_STEP  16

#define DEFAULT_W   VENC_MIN_W
#define DEFAULT_H   VENC_MIN_H

#define DEFAULT_OUT_FMT_ID 3
#define DEFAULT_CAP_FMT_ID 0
#define INVALID_CODEC_ID    -1

#define ES_BUFFER_MAX_SIZE    (512 * 1024)

static V4L2VidFmt venc_video_format[] = {
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
    #ifdef V4L2_PIX_FMT_NV12
    {
        .fourcc = V4L2_PIX_FMT_NV12,
        .type = FMT_FRAME,
        .num_planes = 1,
        .desc = "Y/CbCr 4:2:0",
    },
    #endif
    #ifdef V4L2_PIX_FMT_UYVY
    {
        .fourcc = V4L2_PIX_FMT_UYVY,
        .type = FMT_FRAME,
        .num_planes = 1,
        .desc = "Y/CbCr 4:2:2",
    },
    #endif
    #ifdef V4L2_PIX_FMT_YUV420
    {
        .fourcc = V4L2_PIX_FMT_YUV420,
        .type = FMT_FRAME,
        .num_planes = 1,
        .desc = "YCbCr 4:2:0",
    },
    #endif
};
#define VENC_NUM_FORMAT  sizeof(venc_video_format)/sizeof(venc_video_format[0])

static V4L2FrameSize venc_codec_framesizes[] = {
#ifdef V4L2_PIX_FMT_H264
    {
        .fourcc = V4L2_PIX_FMT_H264,
        .stepwise = {VENC_MIN_W, VENC_MAX_W, 16,
                     VENC_MIN_H, VENC_MAX_H, 16 },
        .desc = "H264",
    },
#endif
#ifdef V4L2_PIX_FMT_VP8
    {
        .fourcc = V4L2_PIX_FMT_VP8,
        .stepwise = {VENC_MIN_W, VENC_MAX_W, 16,
                     VENC_MIN_H, VENC_MAX_H, 16 },
        .desc = "VP8",
    },
#endif
 };
#define ENC_NUM_FRAMESIZE  sizeof(venc_codec_framesizes)/sizeof(venc_codec_framesizes[0])


static HRESULT venc_setHWstate(V4L2M2MContext *ctx, AMP_STATE eState)
{
    HRESULT ret = V4L2_SUCCESS;

    V4L2LOGD("VENC Comp state change[%s-->%s]\n",
            ctx->codec.hwState==AMP_EXECUTING?"EXECUTING":(ctx->codec.hwState==AMP_IDLE?"IDLE":"UNKOWN"),
            eState==AMP_EXECUTING?"EXECUTING":(eState==AMP_IDLE?"IDLE":"UNKOWN"));
    ctx->codec.hwState = eState;

    if(eState == AMP_EXECUTING || eState == AMP_IDLE) {
        AMP_RPC(ret, AMP_VENC_SetState, ctx->comp, eState);
        ASSERT_RET_SHOW(ret, "venc set state faild");
    }
    V4L2LOGD("VENC Comp state:%s done\n", eState==AMP_EXECUTING?"EXECUTING": \
                        (eState==AMP_IDLE?"IDLE":"UNKOWN"));
    return ret;
}

static HRESULT venc_callback(CORBA_Object hCompObj, AMP_PORT_IO ePortIo,
                             UINT32 uiPortIdx, struct AMP_BD_ST *hBD,
                             AMP_IN void *pUserData)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2M2MContext *fh = (V4L2M2MContext*)pUserData;
    AMP_BDTAG_MEMINFO *pMemInfo = NULL;
    V4L2Stream *strm;

    strm = (ePortIo == AMP_PORT_INPUT)? &fh->output: &fh->capture;

    V4L2_DATAFLOW_TRACE("AMP[%s] return [BD:%p] to [%s]\n",
                (ePortIo == AMP_PORT_INPUT)?"input_port":"output_port",
                hBD,
                STREAM_NAME(strm->strm_type));
    DUMP_BD_MEMINFO(hBD);

    if(ePortIo == AMP_PORT_OUTPUT) {
        /*for output port, invalidate the shm cache...*/
        AMP_BDTAG_GetWithIndex(hBD, 0, (void **)&pMemInfo);
        ASSERT_RET_SHOW(ret, "fail to get tag");
        if(pMemInfo->uSize > 0) {
            ret = AMP_SHM_InvalidateCache(pMemInfo->uMemHandle,
                                    pMemInfo->uMemOffset,
                                    pMemInfo->uSize);
            ASSERT_RET_SHOW(ret, "fail to invalidate cache");
            if(strm->last_buff_sent == 0)
                dump_PortBD(strm, hBD);
        }
        else {
            if(pMemInfo->uFlag & AMP_MEMINFO_FLAG_EOS_MASK) {
                strm->last_buff_sent = 1;
                ret = venc_setHWstate(fh, AMP_IDLE);
                ASSERT_RET_SHOW(ret, "venc comp set idle failed");
                V4L2_DATAFLOW_TRACE("[%s] AMP[%s] fluah done\n",
                        STREAM_NAME(strm->strm_type),
                        (ePortIo == AMP_PORT_INPUT)?"input_port":"output_port");
            }
         }
    }

    v4l2_buffer_EnqueBD(strm, hBD);

    return V4L2_SUCCESS;
}

static HRESULT  venc_event_handler(HANDLE hListener, AMP_EVENT *pEvent,
                                        VOID *pUserData)
{
    UINT32 *payload = AMP_EVENT_PAYLOAD_PTR(pEvent);
    V4L2M2MContext *fh = (V4L2M2MContext*)pUserData;

    switch (AMP_EVENT_GETCODE(*pEvent)) {
        case AMP_EVENT_VENC_HEADER_DATA_UPDATE:
        {
            UINT32 hdsize=AMP_EVENT_GETPAR1(*pEvent);
            UINT8 *ptr = AMP_EVENT_PAYLOAD_PTR(pEvent);

            V4L2LOGD("AMP_EVENT_VENC_HEADER_DATA_UPDATE size %d\n", hdsize);
            if(hdsize > 8) {
                V4L2LOGD("payload:0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x\n",
                        ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]);
            }
            break;
        } break;
        default:
            break;

    }

    return V4L2_SUCCESS;
}

static HRESULT venc_register_notify(V4L2M2MContext *fh)
{
    HRESULT ret = SUCCESS;
    AMP_COMPONENT venc = fh->comp;

    fh->hListener = AMP_Event_CreateListener(16, 0);
    ret = AMP_Event_RegisterCallback(fh->hListener,
                    AMP_EVENT_VENC_HEADER_DATA_UPDATE,
                    venc_event_handler, fh);
    ASSERT_RET_SHOW(ret, "register callback failed");

    AMP_RPC(ret, AMP_VENC_RegisterNotify, fh->comp,
           AMP_Event_GetServiceID(fh->hListener), AMP_EVENT_VENC_HEADER_DATA_UPDATE);
    ASSERT_RET_SHOW(ret, "register Notify failed");

    return ret;
}

static HRESULT venc_unregister_notify(V4L2M2MContext *fh)
{
    HRESULT ret = SUCCESS;
    AMP_RPC(ret, AMP_VENC_UnregisterNotify, fh->comp,
            AMP_Event_GetServiceID(fh->hListener), AMP_EVENT_VENC_HEADER_DATA_UPDATE);
    ASSERT_RET_SHOW(ret, "unregister notify failed");

    ret = AMP_Event_UnregisterCallback(fh->hListener,
                AMP_EVENT_VENC_HEADER_DATA_UPDATE, venc_event_handler);
    ASSERT_RET_SHOW(ret, "unregister callback failed");

    ret = AMP_Event_DestroyListener(fh->hListener);
    ASSERT_RET_SHOW(ret, "destroy listener  failed");

    return ret;
}

static HRESULT venc_push_bd(void *fh, AMP_PORT_IO port, AMP_BD_HANDLE hBD)
{
    HRESULT ret = V4L2_SUCCESS;
    AMP_BDTAG_MEMINFO *pMemInfo = NULL;
    AMP_BDTAG_STREAM_INFO *pStreamInfo = NULL;
    V4L2M2MContext *ctx = (V4L2M2MContext *)fh;
    AMP_COMPONENT comp = ctx->comp;
    V4L2Stream *strm;

    strm = (port == AMP_PORT_INPUT)? &ctx->output: &ctx->capture;

    /**
     * for venc output, need to reset BD meminfo tag and streaminfo tag:
     */
    if(port == AMP_PORT_OUTPUT) {
        ret = AMP_BDTAG_GetWithIndex(hBD, 0, (void **)&pMemInfo);
        ASSERT_RET_SHOW(ret, "fail to get tag");
        if(pMemInfo) {
            pMemInfo->uSize = ES_BUFFER_MAX_SIZE;
            pMemInfo->uMemOffset = 0;
            pMemInfo->uFlag = 0;
        }
        #if 0
        AMP_BDTAG_GetWithIndex(hBD, 1, (void **)&pStreamInfo);
        pStreamInfo->uType = 0;
        pStreamInfo->uiPtsHigh = 0;
        pStreamInfo->uiPtsLow = 0;
        pStreamInfo->uUnitFlag = 0;
        pStreamInfo->isHeader = 0;
        #endif
    }
    else {
        /*for input port, clean the shm cache...*/
        AMP_BDTAG_GetWithIndex(hBD, 0, (void **)&pMemInfo);
        ASSERT_RET_SHOW(ret, "fail to get tag");
        if(pMemInfo->uSize > 0) {
            ret = AMP_SHM_CleanCache(pMemInfo->uMemHandle,
                                    pMemInfo->uMemOffset,
                                    pMemInfo->uSize);
            ASSERT_RET_SHOW(ret, "fail to clean cache");
        }
        dump_PortBD(strm, hBD);
    }

    V4L2_DATAFLOW_TRACE("[%s] push [BD:%p] to AMP[%s]\n",
                STREAM_NAME(strm->strm_type),
                hBD,
                (port == AMP_PORT_INPUT)?"input_port":"output_port");
    AMP_RPC(ret, AMP_VENC_PushBD, comp, port, 0, hBD);
    ASSERT_RET_SHOW(ret, "venc push BD faild");

    V4L2LOGD("push BD to venc comp %s port successfully\n",
                    (port == AMP_PORT_OUTPUT)?"output":"input");

    return ret;
}

static HRESULT venc_set_ctx_default_params(V4L2M2MContext *fh)
{
    struct v4l2_pix_format_mplane *pix_fmt_mp = NULL;
    struct v4l2_plane_pix_format *plane = NULL;
    V4L2Stream *strm;

    //src: output
    strm = &fh->output;
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
    pix_fmt_mp->num_planes = 1; //2 for nv12 format
    //yuv plane
    plane = &pix_fmt_mp->plane_fmt[0];
    plane->sizeimage = DEFAULT_W * DEFAULT_H*3/2;
    plane->bytesperline = DEFAULT_W;
    //dst :capture
    strm = &fh->capture;
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
    //capture planes
    pix_fmt_mp->num_planes = 1; //1 for es format
    plane = &pix_fmt_mp->plane_fmt[0];
    plane->sizeimage = DEFAULT_W * DEFAULT_H;
    plane->bytesperline = 0;
    return SUCCESS;
}

static V4L2VidFmt *venc_find_format(struct v4l2_format *f)
{
  V4L2VidFmt *fmt;
  unsigned int i;

  for (i = 0; i < VENC_NUM_FORMAT; i++) {
    fmt = &venc_video_format[i];
    if (fmt->fourcc == f->fmt.pix_mp.pixelformat)
      return fmt;
  }
  return NULL;
}

static HRESULT internal_venc_open(V4L2M2MContext *fh)
{
    HRESULT ret = V4L2_SUCCESS;
    AMP_COMPONENT_CONFIG amp_config;
    UINT32 width = 0, height = 0, color_fmt = 0;
    struct v4l2_format *format = NULL;
    UINT32 enc_codec = 0;
    V4L2EncoderCtx *enc_ctx = &fh->codec.ctx.enc;

    AmpMemClear(&amp_config, sizeof(AMP_COMPONENT_CONFIG));
    amp_config._d = AMP_COMPONENT_VENC;
    amp_config._u.pVENC.mode = AMP_NON_TUNNEL;

    /* Input config */
    format = &fh->output.format;
    if(V4L2_TYPE_IS_MULTIPLANAR(format->type)) {
        struct v4l2_pix_format_mplane *pix_fmt_mp = &format->fmt.pix_mp;
        width = pix_fmt_mp->width;
        height = pix_fmt_mp->height;
        color_fmt = pix_fmt_mp->pixelformat;
    }
    else {
        struct v4l2_pix_format *pix_fmt = &format->fmt.pix;
        width = pix_fmt->width;
        height = pix_fmt->height;
        color_fmt = pix_fmt->pixelformat;
    }
    if(color_fmt == V4L2_PIX_FMT_NV12) {
        color_fmt = AMP_VENC_YUV420_SP;
    }
    else if(color_fmt == V4L2_PIX_FMT_UYVY) {
        color_fmt = AMP_VENC_YUV422_UYVY;
    }
    else if(color_fmt == V4L2_PIX_FMT_YUV420){
        color_fmt = AMP_VENC_YUV420_PLANE;
    }
    else {
        color_fmt = AMP_VENC_YUV420_SP;
    }

    amp_config._u.pVENC.uiWidth = width;
    amp_config._u.pVENC.uiHeight = height;
    amp_config._u.pVENC.uiColorFmt = color_fmt;

    /* Output config */
    struct v4l2_pix_format_mplane *pix_mp = &fh->capture.format.fmt.pix_mp;
    if(pix_mp->pixelformat == V4L2_PIX_FMT_H264) {
        amp_config._u.pVENC.uiCodingFmt = MEDIA_VES_AVC;
    }
    else if(pix_mp->pixelformat == V4L2_PIX_FMT_VP8) {
        amp_config._u.pVENC.uiCodingFmt = MEDIA_VES_VP8;
    }
    else {
        amp_config._u.pVENC.uiCodingFmt = MEDIA_VES_AVC;
    }
    amp_config._u.pVENC.uiEncMode = AMP_VENC_MODE_EXPRESS;
    amp_config._u.pVENC.uiInitQp = enc_ctx->max_qp; //[0 ~ 100]
    amp_config._u.pVENC.uiGopSize = 30;
    amp_config._u.pVENC.uiGopType = AMP_RCGOP_IP;
    if(enc_ctx->profile == V4L2_MPEG_VIDEO_H264_PROFILE_BASELINE) {
        amp_config._u.pVENC.uiH264Profile = AMP_VENC_PROFILE_BASE;
    }
    else if(enc_ctx->profile == V4L2_MPEG_VIDEO_H264_PROFILE_MAIN) {
        amp_config._u.pVENC.uiH264Profile = AMP_VENC_PROFILE_MAIN;
    }
    else if(enc_ctx->profile == V4L2_MPEG_VIDEO_H264_PROFILE_EXTENDED) {
        amp_config._u.pVENC.uiH264Profile = AMP_VENC_PROFILE_EXTENDED;
    }
    else if(enc_ctx->profile == V4L2_MPEG_VIDEO_H264_PROFILE_HIGH) {
        amp_config._u.pVENC.uiH264Profile = AMP_VENC_PROFILE_HIGH;
    }
    else {
        amp_config._u.pVENC.uiH264Profile = AMP_VENC_PROFILE_HIGH;
    }
    amp_config._u.pVENC.uiH264Level = enc_ctx->level; // VENC_LEVEL_4_0
    amp_config._u.pVENC.uiFrameRateNum = enc_ctx->fs_denominator;
    amp_config._u.pVENC.uiFrameRateDen = enc_ctx->fs_numerator;
    amp_config._u.pVENC.uiRateCtrlSel = 9;
    amp_config._u.pVENC.uiVBVSize = 0;
    amp_config._u.pVENC.uiHRDBitRate = 0;
    amp_config._u.pVENC.uiAvcOn = 1;  //just output global sps/pps
    amp_config._u.pVENC.uiHRDBitRate = enc_ctx->bitrate;

    AMP_RPC(ret, AMP_VENC_Open, fh->comp, &amp_config);
    ASSERT_RET_SHOW(ret, "venc comp open failed");

    ret = venc_setHWstate(fh, AMP_IDLE);
    ASSERT_RET_SHOW(ret, "venc comp set idle failed");

    //register buffer return callback
    ret = AMP_ConnectApp(fh->comp, AMP_PORT_INPUT, 0, venc_callback, fh);
    ASSERT_RET_SHOW(ret, "connect input failed");

    ret = AMP_ConnectApp(fh->comp, AMP_PORT_OUTPUT, 0, venc_callback, fh);
    ASSERT_RET_SHOW(ret, "connect output failed");

    //register event notify
    venc_register_notify(fh);

    ASSERT_RET_SHOW(ret, "open venc failed");

    return V4L2_SUCCESS;
error:
    return ret;

}

static int venc_try_encode(V4L2M2MContext *fh)
{
    //check codec stauts
    if(fh->codec.state < STATE_WORKING)
        return 0;

    switch(fh->codec.state)
    {
        case STATE_WORKING:
        case STATE_HEADER:
            if((!fh->capture.streamon) ||(!fh->output.streamon))
                return 0;
            if(fh->codec.hwState == AMP_IDLE) {
                V4L2LOGD("set venc comp executing...\n");
                venc_setHWstate(fh, AMP_EXECUTING);
            }
        case STATE_DRAIN:
            v4l2_stream_enqueue_hw((void *)fh, (PushBDFuc)venc_push_bd);
            break;
        default:
            break;
    }
    return 0;
}

static int venc_try_start_encoder(void *fh)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2M2MContext* ctx = (V4L2M2MContext*)fh;
    V4L2Stream *output = &ctx->output;
    V4L2Stream *capture = &ctx->capture;

    if (!output->streamon) {
        V4L2LOGE("output stream is off. Not ready to start.\n");
        return 0;
    }
    if (!capture->streamon) {
        V4L2LOGE("capture stream is off. Not ready to start.\n");
        return 0;
    }

    if(ctx->codec.ctx.enc.comp_opened == 0) {
        internal_venc_open(ctx);
        ctx->codec.ctx.enc.comp_opened = 1;
    }

    v4l2_setState(fh, STATE_WORKING);
    venc_try_encode(ctx);

    return ret;
}

static int venc_try_stop_encoder(void *fh)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2M2MContext* ctx = (V4L2M2MContext*)fh;

    V4L2Stream *output = &ctx->output;
    V4L2Stream *capture = &ctx->capture;

    if (output->streamon && capture->streamon) {
        V4L2LOGD("output&capture streams are on, need to flush.\n");
        v4l2_setState(fh, STATE_DRAIN);
        v4l2_buffer_EnqueFlushBuf((void *)output);
        /*call venc_try_encode() to handle flush buffer*/
        venc_try_encode(ctx);
    }
    else if((!output->streamon) && (!capture->streamon)) {
        V4L2LOGD("output&capture streams are off, need to stop hw enc\n");
        venc_setHWstate(ctx, AMP_IDLE);
    }

    return ret;
}


static int vidioc_encoder_cmd(void *fh, void *p)
{
    int ret;
    struct v4l2_encoder_cmd *cmd = (struct v4l2_encoder_cmd *)p;

    V4L2LOGH("encoder cmd=%u", cmd->cmd);

    switch (cmd->cmd) {
    case V4L2_ENC_CMD_STOP:
        venc_try_stop_encoder(fh);
        break;

    case V4L2_ENC_CMD_START:
        V4L2LOGD("V4L2_ENC_CMD_START() recieved\n");
        venc_try_start_encoder(fh);
        break;

    default:
        return ERROR_EINVAL;

    }

    return 0;
}

static int vidioc_venc_streamon(void *fh, void *p)
{
    enum v4l2_buf_type type = *(int *)p;
    V4L2Stream *strm = v4l2_get_stream(fh, type);
    v4l2_stream_on(strm);
    v4l2_setState(fh, STATE_WORKING);
    venc_try_start_encoder((V4L2M2MContext *)fh);

    V4L2LOGH("[%s] streamon\n", STREAM_NAME(strm->strm_type));
    return 0;
}

static int vidioc_venc_streamoff(void *fh, void *p)
{
    M2MCTX(fh)
    enum v4l2_buf_type type = *(int *)p;
    V4L2Stream *strm = v4l2_get_stream(fh, type);

    if(V4L2_TYPE_IS_OUTPUT(type))
        v4l2_setState(fh, STATE_STOP);
    else
        v4l2_setState(fh, STATE_RESET);

    v4l2_stream_off(strm);

    venc_try_stop_encoder((V4L2M2MContext *)fh);

    V4L2LOGH("[%s] streamoff\n", STREAM_NAME(strm->strm_type));
    return 0;
}

static int vidioc_try_encoder_cmd(void *fh, void *p)
{
    struct v4l2_encoder_cmd *cmd = (struct v4l2_encoder_cmd *)p;

    return 0;
}

static int vidioc_enum_framesizes(void *fh, struct v4l2_frmsizeenum *fsize)
{
    return 0;
}

static int vidioc_venc_querryctrl(void *fh, void *p)
{
    struct v4l2_queryctrl *querry_ctrl = (struct v4l2_queryctrl *)p;

    if(querry_ctrl->id < V4L2_CID_BASE ||
        querry_ctrl->id == (V4L2_CID_MPEG_BASE + 388))
    {
        return ERROR_EINVAL;
    }

    return V4L2_SUCCESS;
}

static int vidioc_venc_SetExtCtrls(void *fh, void *p)
{
    HRESULT ret = V4L2_SUCCESS;
    struct v4l2_ext_controls *ext_controls = (struct v4l2_ext_controls *)p;
    struct v4l2_ext_control *ext_control = NULL;
    V4L2M2MContext *ctx = (V4L2M2MContext *)fh;
    V4L2EncoderCtx *enc_ctx = &ctx->codec.ctx.enc;
    UINT32 i = 0;

    for(i=0; i<ext_controls->count; i++) {
        ext_control = &ext_controls->controls[i];
        switch(ext_control->id) {
            #ifdef V4L2_CID_MPEG_VIDEO_FORCE_KEY_FRAME
            case V4L2_CID_MPEG_VIDEO_FORCE_KEY_FRAME:
                enc_ctx->force_keyframe = ext_control->value;
                V4L2LOGD("force_keyframe=%d\n", enc_ctx->force_keyframe);
                break;
            #endif
            case V4L2_CID_MPEG_VIDEO_BITRATE:
                enc_ctx->bitrate = ext_control->value;
                V4L2LOGD("bitrate=%d\n", enc_ctx->bitrate);
                break;
            case V4L2_CID_MPEG_VIDEO_FRAME_RC_ENABLE:
                enc_ctx->enable_rc_frame = ext_control->value;
                //! TODO...
                break;
            #ifdef V4L2_CID_MPEG_VIDEO_H264_SPS_PPS_BEFORE_IDR
            case V4L2_CID_MPEG_VIDEO_H264_SPS_PPS_BEFORE_IDR:
                enc_ctx->inject_sps_pps = ext_control->value;
                V4L2LOGD("inject_sps_pps=%d\n", enc_ctx->inject_sps_pps);
                //! not support
                break;
            #endif
            case V4L2_CID_MPEG_VIDEO_B_FRAMES:
                enc_ctx->has_b_frame = ext_control->value;
                V4L2LOGD("has_b_frame=%d\n", enc_ctx->has_b_frame);
                break;
            case V4L2_CID_MPEG_VIDEO_H264_MAX_QP:
                enc_ctx->max_qp = ext_control->value;
                V4L2LOGD("max_qp=%d\n", enc_ctx->max_qp);
                break;
            case V4L2_CID_MPEG_VIDEO_H264_PROFILE:
                enc_ctx->profile = ext_control->value;
                V4L2LOGD("profile=%d\n", enc_ctx->profile);
                break;
            case V4L2_CID_MPEG_VIDEO_H264_LEVEL:
                enc_ctx->level = ext_control->value;
                V4L2LOGD("level=%d\n", enc_ctx->level);
                break;
            case V4L2_CID_MPEG_VIDEO_HEADER_MODE:
                enc_ctx->header_mode = ext_control->value;
                V4L2LOGD("header_mode=%d\n", enc_ctx->header_mode);
                break;
            default:
                V4L2LOGE("ext_control->id(0x%x) not support\n", ext_control->id);
        }
    }

    return ret;
}

static int vidioc_venc_enum_framesizes(void *fh, void *p)
{
    struct v4l2_frmsizeenum *fsize = (struct v4l2_frmsizeenum *)p;
    int i = 0;

    V4L2LOGD("[vidioc] [index=%d][pixel_format=0x%x]\n",
                    fsize->index, fsize->pixel_format);
    if (fsize->index != 0)
        return ERROR_EINVAL;

    for (i = 0; i < ENC_NUM_FRAMESIZE; ++i) {
        if (fsize->pixel_format != venc_codec_framesizes[i].fourcc)
            continue;

        fsize->type = V4L2_FRMSIZE_TYPE_STEPWISE;
        memcpy(&fsize->stepwise,
                    &venc_codec_framesizes[i].stepwise,
                    sizeof(struct v4l2_frmsize_stepwise));
        V4L2LOGD("[vidioc] [type:%d][%s]{stepwise:[min_width:%d][max_width:%d]"
                            "[step_width:%d][min_height:%d][max_height:%d][step_height:%d]}\n",
                            fsize->type, venc_codec_framesizes[i].desc,
                            fsize->stepwise.min_width, fsize->stepwise.max_width,
                            fsize->stepwise.step_width, fsize->stepwise.min_height,
                            fsize->stepwise.max_height, fsize->stepwise.step_height);
        return V4L2_SUCCESS;
    }
    return ERROR_EINVAL;
}


static int vidioc_venc_set_param(void *fh, void *p)
{
    struct v4l2_streamparm *param = (struct v4l2_streamparm *)p;
    V4L2M2MContext *ctx = (V4L2M2MContext *)fh;
    V4L2EncoderCtx *enc_ctx = &ctx->codec.ctx.enc;

    if(V4L2_TYPE_IS_OUTPUT(param->type)) {
        enc_ctx->fs_denominator = param->parm.output.timeperframe.denominator;
        enc_ctx->fs_numerator = param->parm.output.timeperframe.numerator;

        V4L2LOGD("[vidioc] [fs_denominator=0x%x][fs_numerator=0x%x]\n",
                            enc_ctx->fs_denominator, enc_ctx->fs_numerator);
    }

    return V4L2_SUCCESS;
}


static int _vidioc_venc_enum_fmt(struct v4l2_fmtdesc *f, bool bOutput)
{
    V4L2VidFmt* fmt;
    int i, j=0;

    V4L2LOGD("[vidioc] [index=%d]\n", f->index);
    const unsigned sz = sizeof(f->description);
    for (i=0; i<VENC_NUM_FORMAT; i++) {
        if(bOutput && venc_video_format[i].type != FMT_FRAME)
            continue;
        if(!bOutput && venc_video_format[i].type != FMT_ES)
            continue;
        if(j == f->index) {
            break;
        }
        ++j;
    }
    if(i == VENC_NUM_FORMAT)
        return ERROR_EINVAL;

    fmt = &venc_video_format[i];

    //update v4l2_fmtdesc
    f->pixelformat = fmt->fourcc;
    f->flags = (fmt->type == FMT_ES)? V4L2_FMT_FLAG_COMPRESSED : 0;
    if(fmt->desc){
        int rsz = strlen(fmt->desc);
        char *buff = (char*)f->description;
        if (rsz > sz - 1)
            rsz = sz - 1;
        strncpy(buff, fmt->desc, rsz);
        buff[rsz] = '\0';
    }
    return 0;
}

static int vidioc_venc_enum_fmt(void *fh, void *p)
{
    struct v4l2_fmtdesc *f = (struct v4l2_fmtdesc *)p;
    if(V4L2_TYPE_IS_OUTPUT(f->type))
        return _vidioc_venc_enum_fmt(f, true);
    return _vidioc_venc_enum_fmt(f, false);
}

static int vidioc_venc_s_fmt(void *fh, void *p)
{
    int ret = 0;
    struct v4l2_format *fmt = (struct v4l2_format *)p;
    V4L2VidFmt *default_fmt;
    UINT32 i = 0;
    UINT32 num_planes = 0;

    default_fmt = venc_find_format(fmt);
    if(!default_fmt) {
        return ERROR_EINVAL;
    }
    /*update num_planes/sizeimage/bytesperline */
    struct v4l2_pix_format_mplane *pix_fmt_mp = &fmt->fmt.pix_mp;
    if(V4L2_TYPE_IS_OUTPUT(fmt->type)) {
        pix_fmt_mp->num_planes = 1;
        pix_fmt_mp->plane_fmt[0].sizeimage = pix_fmt_mp->width*pix_fmt_mp->height*1.5;
        pix_fmt_mp->plane_fmt[0].bytesperline = pix_fmt_mp->width;
    }
    else {
        pix_fmt_mp->num_planes = 1;
        pix_fmt_mp->plane_fmt[0].sizeimage = ES_BUFFER_MAX_SIZE;
        pix_fmt_mp->plane_fmt[0].bytesperline = 0;
    }

    V4L2Stream *strm = v4l2_get_stream(fh, fmt->type);

    V4L2LOGD("[vidioc] [%s]\n", STREAM_NAME(strm->strm_type));

    ret = v4l2_stream_set_format(strm, fmt);
    show_v4l2fmt(&strm->format);

    return ret;
}

static int vidioc_venc_g_fmt(void *fh, void *p)
{
    int ret = 0;
    struct v4l2_format *fmt = (struct v4l2_format *)p;
    V4L2Stream *strm = v4l2_get_stream(fh, fmt->type);

    V4L2LOGD("[vidioc] [%s]\n", STREAM_NAME(strm->strm_type));

    ret = v4l2_stream_get_format(strm, fmt);

    show_v4l2fmt(fmt);
    return ret;
}


static int vidioc_venc_try_fmt(void *fh, void *p)
{
    int i=0;

    struct v4l2_format *f = (struct v4l2_format *)p;
    struct v4l2_pix_format_mplane *pix_fmt_mp = &f->fmt.pix_mp;
    int idx = V4L2_TYPE_IS_OUTPUT(f->type) ? DEFAULT_OUT_FMT_ID : DEFAULT_CAP_FMT_ID;

    V4L2VidFmt *fmt = venc_find_format(f);
    if(!fmt) {
        pix_fmt_mp->pixelformat = venc_video_format[idx].fourcc;
        fmt = &venc_video_format[idx];
    }

    if(!V4L2_TYPE_IS_OUTPUT(f->type)) {
        pix_fmt_mp->num_planes = 1;
        pix_fmt_mp->plane_fmt[0].bytesperline = 0;
    } else {
        pix_fmt_mp->num_planes = fmt->num_planes;
        pix_fmt_mp->width = VENC_MIN_W;
        pix_fmt_mp->height = VENC_MIN_H;  //TODO: check this data if valid
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

static int vidioc_venc_qbuf(void *fh, void *p)
{
    int ret = 0;
    struct v4l2_buffer *buf = (struct v4l2_buffer *)p;

    ret = v4l2_buffer_QBuff(fh, buf);
    venc_try_encode(fh);
    return ret;
}

static int vidioc_venc_dqbuf(void *fh, void *p)
{
    int ret = 0;
    struct v4l2_buffer *buf = (struct v4l2_buffer *)p;

    ret = v4l2_buffer_DQBuff(fh, buf);
    return ret;
}

static int vidioc_venc_querybuf(void *fh, void *p)
{
    struct v4l2_buffer *buf = (struct v4l2_buffer *)p;

    return v4l2_buffer_QueryBuff(fh, buf);
}

static int vidioc_venc_reqbufs(void *fh, void *p)
{
    struct v4l2_requestbuffers * req;
    req = (struct v4l2_requestbuffers *)p;

    V4L2Stream *strm = v4l2_get_stream(fh, req->type);

    return v4l2_stream_ReqBuff(strm, req);
}

static int vidioc_venc_subscribe_event(void *fh, void *p)
{
    struct v4l2_event_subscription* sub;
    sub = (struct v4l2_event_subscription*)p;

    switch(sub->type) {
        case V4L2_EVENT_EOS:
            return v4l2_event_subscribe(fh, sub, 2, NULL);
        default :
            return v4l2_event_subscribe(fh, sub, 0, NULL);
        //TODO: are we going to support control sub event?
    }
}
static int vidioc_venc_unsubscribe_event(void *fh, void *p)
{
    struct v4l2_event_subscription* sub;
    sub = (struct v4l2_event_subscription*)p;

    return v4l2_event_unsubscribe(fh, sub);
}

static int vidioc_venc_dqevent(void *fh, void *p)
{
    M2MCTX(fh)
    struct v4l2_event* e = (struct v4l2_event *)p;

    return v4l2_event_dequeue(fh, p, ctx->nonblocking);
}

HRESULT v4l2_encoder_init(V4L2M2MContext *fh)
{
    HRESULT ret = SUCCESS;


    V4L2_API_ENTRY();

    //create venc comp
    AMP_COMPONENT venc;
    AMP_RPC(ret, AMP_FACTORY_CreateComponent,
                fh->pV4l2Ctx->amp_factory,
                AMP_COMPONENT_VENC, 0,
                &venc);
    ASSERT_RET_SHOW(ret, "create venc failed");

    //fillin m2m ctx
    fh->comp = venc;

    //! only need to override venc own function
    //! buffer
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_QBUF, vidioc_venc_qbuf);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_DQBUF, vidioc_venc_dqbuf);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_QUERYBUF, vidioc_venc_querybuf);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_REQBUFS, vidioc_venc_reqbufs);
    //! fomat
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_ENUM_FMT, vidioc_venc_enum_fmt);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_TRY_FMT, vidioc_venc_try_fmt);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_G_FMT, vidioc_venc_g_fmt);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_S_FMT, vidioc_venc_s_fmt);
    //! event
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_SUBSCRIBE_EVENT, vidioc_venc_subscribe_event);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_UNSUBSCRIBE_EVENT, vidioc_venc_unsubscribe_event);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_DQEVENT, vidioc_venc_dqevent);
    //! control
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_QUERYCTRL, vidioc_venc_querryctrl);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_S_EXT_CTRLS, vidioc_venc_SetExtCtrls);
    //! param
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_S_PARM, vidioc_venc_set_param);
    //! selection(crop)
    //OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_G_SELECTION, vidioc_venc_g_selection);
    //OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_S_SELECTION, vidioc_venc_s_selection);
    //! frame
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_ENUM_FRAMESIZES, vidioc_venc_enum_framesizes);
    //! stream
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_STREAMON, vidioc_venc_streamon);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_STREAMOFF, vidioc_venc_streamoff);
     //! encoder
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_ENCODER_CMD, vidioc_encoder_cmd);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_TRY_ENCODER_CMD, vidioc_try_encoder_cmd);

    createFlushBDBuffer((void *)fh);

    V4L2_API_EXIT(ret);
    return ret;

}

HRESULT v4l2_encoder_deinit(V4L2M2MContext *fh)
{
    HRESULT ret = SUCCESS;

    V4L2_API_ENTRY();
    AMP_COMPONENT venc = fh->comp;

    ret = venc_setHWstate(fh, AMP_IDLE);
    ASSERT_RET_SHOW(ret, "venc comp set idle failed");

    venc_unregister_notify(fh);

    ret = AMP_DisconnectApp(venc, AMP_PORT_INPUT, 0, venc_callback);
    ASSERT_RET_SHOW(ret, "disconnect input failed");

    ret = AMP_DisconnectApp(venc, AMP_PORT_OUTPUT, 0, venc_callback);
    ASSERT_RET_SHOW(ret, "disconnect output failed");

    AMP_RPC(ret, AMP_VENC_Close, venc);
    ASSERT_RET_SHOW(ret, "close failed");

    AMP_RPC(ret, AMP_VENC_Destroy, venc);
    ASSERT_RET_SHOW(ret, "distroy venc failed");

    deleteFlushBDBuffer(fh->output.flush_buf->hBD);

    V4L2_API_EXIT(ret);
    return ret;
}



