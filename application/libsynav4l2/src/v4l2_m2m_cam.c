/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2020 Synaptics Incorporated. All rights reserved.
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

#include <errno.h>
#include <linux/videodev2.h>

#include "amp_types.h"

#include "v4l2_types.h"
#include "v4l2_m2m.h"
#include "v4l2_log.h"
#include "v4l2_misc.h"

#include "cam/cam_config_reader.h"

#define DTAG "[camera]"
#define DTAG_BUF DTAG":[buf] "

#define CAM_MIN_W 64
#define CAM_MIN_H 64

#define CAM_MAX_W  CAM_SENSOR_W
#define CAM_MAX_H  CAM_SENSOR_H

#define CAM_ISP_COMPBUFS 5
#define CAM_DEFAULT_COMPBUFS 3

#define CAM_DEFAULT_PATH AMP_ISP_OUT_SP1
#define CAM_DEFAULT_FMT AMP_DISP_SRCFMT_YUV420SP
#define CAM_DEFAULT_FMT_ORDER AMP_DISP_ORDER_UYVY

#define CAM_ENABLE_TUNNEL_MODE

#define CAM_STEP  16

#define DEFAULT_W   CAM_MIN_W
#define DEFAULT_H   CAM_MIN_H

#define DEFAULT_OUT_FMT_ID 0
#define DEFAULT_CAP_FMT_ID 3
#define INVALID_CODEC_ID    -1

#define ALIGN_4K(x)  ((x) + 4095) / 4096 * 4096

static void *gStream[CAM_STREAM_MAX];

static V4L2VidFmt cam_video_format[] = {
#ifdef V4L2_PIX_FMT_NV12
{
    .fourcc = V4L2_PIX_FMT_NV12,
    .type = FMT_FRAME,
    .num_planes = 2,
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
};

static V4L2FrameSize cam_video_framesizes[] = {
#ifdef V4L2_PIX_FMT_NV12
    {
        .fourcc = V4L2_PIX_FMT_NV12,
        .stepwise = {CAM_MIN_W, CAM_MAX_W, 16,
                     CAM_MIN_H, CAM_MAX_H, 16 },
    },
#endif
#ifdef V4L2_PIX_FMT_UYVY
    {
        .fourcc = V4L2_PIX_FMT_UYVY,
        .stepwise = {CAM_MIN_W, CAM_MAX_W, 16,
                     CAM_MIN_H, CAM_MAX_H, 16 },
    },
#endif
 };

#define CAM_NUM_FORMAT  sizeof(cam_video_format)/sizeof(cam_video_format[0])
#define CAM_NUM_FRAMESIZE  sizeof(cam_video_framesizes)/sizeof(cam_video_framesizes[0])
static HRESULT cam_setHWstate(V4L2M2MContext *ctx, AMP_STATE eState);


static void frameReadyEvent(int streamId, AMP_BD_HANDLE hBD, void *pUsrData) {
    V4L2Stream *strm;
    V4L2M2MContext *ctx = (V4L2M2MContext*)pUsrData;

    strm = &ctx->capture;
    v4l2_buffer_EnqueBD(strm, hBD);

#if 0
    V4L2LOGD("%s Return BD %p, %d\n", (ePortIo == 0)?
                "AMP_PORT_INPUT":"AMP_PORT_OUTPUT", hBD, AMP_BD_GET_BDID(hBD));

    if((ePortIo == AMP_PORT_OUTPUT) && is_validDispBD(hBD)){
        dump_PortBD(&ctx->capture, hBD);
    }
#endif

    return;
}

static HRESULT cam_push_bd(void *fh, AMP_PORT_IO port, AMP_BD_HANDLE hBD)
{
    HRESULT ret;
    V4L2M2MContext *ctx = (V4L2M2MContext *)fh;
    V4L2CameraCtx *cctx = &ctx->codec.ctx.cam;

    V4L2LOGD("\t\t%s Push BD %p \n", (port == 0)?
                    "AMP_PORT_INPUT":"AMP_PORT_OUTPUT", hBD);

    if(cctx->streamId != CAM_STREAM_INVALID) {
        cam_stream_pushFrame(gStream[cctx->streamId], hBD);
    }

    return ret;
}

static HRESULT cam_setHWstate(V4L2M2MContext *ctx, AMP_STATE eState)
{
    V4L2CameraCtx *cctx = &ctx->codec.ctx.cam;
    HRESULT ret = V4L2_SUCCESS;

    V4L2LOGH("set HWstate %s \n",
        (eState == AMP_EXECUTING)? "AMP_EXECUTING":"AMP_IDLE");
    ctx->codec.hwState = eState;

    if(cctx->streamId == CAM_STREAM_INVALID)
        return ERROR_EINVAL;

    if(eState == AMP_EXECUTING || eState == AMP_IDLE) {

        if(eState == AMP_EXECUTING) {
            cam_stream_start(gStream[cctx->streamId]);
        } else {
            cam_stream_pause(gStream[cctx->streamId]);
        }
    } else {
        V4L2LOGE("Unsupported HW state set %d\n", eState);
        ret = ERROR_EINVAL;
    }
    return ret;
}

static void cam_set_ctx_default_params(V4L2M2MContext *fh)
{
    struct v4l2_pix_format_mplane *pix_fmt_mp = NULL;
    struct v4l2_plane_pix_format *plane = NULL;
    V4L2Stream *strm;

    // FOR M2M operations
    //src: output
    strm = &fh->output;
    strm->width = DEFAULT_W;
    strm->height = DEFAULT_H;

    pix_fmt_mp = &strm->format.fmt.pix_mp;
    pix_fmt_mp->colorspace = V4L2_COLORSPACE_REC709;
    pix_fmt_mp->field = V4L2_FIELD_NONE;
    pix_fmt_mp->pixelformat = V4L2_PIX_FMT_NV12;

    pix_fmt_mp->width = DEFAULT_W;
    pix_fmt_mp->height = DEFAULT_H;
    //output planes
    pix_fmt_mp->num_planes = 2; //2 for nv12 format
    plane = &pix_fmt_mp->plane_fmt[0];
    plane->bytesperline = 0;
    plane->sizeimage = 1024*1024;
    plane = &pix_fmt_mp->plane_fmt[1];
    plane->bytesperline = 0;
    plane->sizeimage = 1024*1024/2;

    //dst :capture
    strm = &fh->capture;
    pix_fmt_mp = &strm->format.fmt.pix_mp;
    pix_fmt_mp->colorspace = V4L2_COLORSPACE_REC709;
    pix_fmt_mp->field = V4L2_FIELD_NONE;
    pix_fmt_mp->width = CAM_SENSOR_W;
    pix_fmt_mp->height = CAM_SENSOR_H;
    pix_fmt_mp->pixelformat = V4L2_PIX_FMT_NV12;
    //output planes
    pix_fmt_mp->num_planes = 2; //2 for nv12 format
    HRESULT ret = SUCCESS;
    UINT32 ySize = 0, uvSize = 0;
    ySize = pix_fmt_mp->width * pix_fmt_mp->height;
    uvSize = pix_fmt_mp->width * pix_fmt_mp->height / 2;

    //y plane
    plane = &pix_fmt_mp->plane_fmt[0];
    plane->bytesperline = pix_fmt_mp->width;
    plane->sizeimage = ySize;
    //uv plane
    plane = &pix_fmt_mp->plane_fmt[1];
    plane->bytesperline = pix_fmt_mp->width / 2;
    plane->sizeimage = uvSize;

    return;
}

static V4L2VidFmt *cam_find_format(struct v4l2_format *f)
{
  V4L2VidFmt *fmt;
  unsigned int i;

  for (i = 0; i < CAM_NUM_FORMAT; i++) {
    fmt = &cam_video_format[i];
    if (fmt->fourcc == f->fmt.pix_mp.pixelformat)
      return fmt;
  }
  return NULL;
}

static int cam_try_capture(V4L2M2MContext *fh)
{

    V4L2_API_ENTRY();
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
                cam_setHWstate(fh, AMP_EXECUTING);
            }
            v4l2_stream_enqueue_hw((void *)fh, (PushBDFuc)cam_push_bd);
            break;
        default:
            V4L2LOGH("state wrong(%d)\n", fh->codec.state);
            break;
    }

    return 0;
 try_fail:
    return -1;
}

static int cam_try_stop_decode(V4L2M2MContext *fh)
{
    V4L2_API_ENTRY();
    if((fh->capture.streamon == FALSE)
        && (fh->output.streamon == FALSE)) {
        cam_setHWstate(fh, AMP_IDLE);
    }
    return 0;
}

static int vidioc_cam_streamon(void *fh, void *p)
{
    enum v4l2_buf_type type = *(unsigned int *)p;

    V4L2LOGH("%s on\n", buf2str(type));
    V4L2Stream *strm = v4l2_get_stream(fh, type);
    v4l2_stream_on(strm);

    v4l2_setState(fh, STATE_WORKING);

    cam_try_capture(fh);

    return 0;
}

static int vidioc_cam_streamoff(void *fh, void *p)
{
    enum v4l2_buf_type type = *(unsigned int *)p;
    V4L2Stream *strm = v4l2_get_stream(fh, type);

    V4L2LOGH("%s off\n", buf2str(type));
    if(V4L2_TYPE_IS_OUTPUT(type))
        v4l2_setState(fh, STATE_SEEK);
    else
        v4l2_setState(fh, STATE_STOP);

    v4l2_stream_off(strm);

    cam_try_stop_decode((V4L2M2MContext*)fh);

    return 0;
}


static int vidioc_cam_g_selection(void *fh, void *p)
{
    M2MCTX(fh)
    struct v4l2_selection *s = (struct v4l2_selection *)p;
    V4L2CameraCtx *cctx = &ctx->codec.ctx.cam;

    if (s->type != V4L2_BUF_TYPE_VIDEO_CAPTURE){
        errno = EINVAL;
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

    switch (s->target) {
        case V4L2_SEL_TGT_CROP:
            s->r.left = cctx->picinfo.crop_l;
            s->r.top = cctx->picinfo.crop_t;
            s->r.width = cctx->picinfo.crop_w;
            s->r.height = cctx->picinfo.crop_h;
            break;
        default:
            errno = EINVAL;
            return ERROR_EINVAL;
    }

    V4L2LOGH("g_selection %d, %d, %d, %d\n", s->r.left, s->r.top,
             s->r.width, s->r.height);

    return 0;
}

static int vidioc_cam_s_selection(void *fh, void *p)
{
    M2MCTX(fh)
    struct v4l2_selection *s = (struct v4l2_selection *)p;
    V4L2CameraCtx *cctx = &ctx->codec.ctx.cam;

    if (s->type != V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE) {
        errno = EINVAL;
        return ERROR_EINVAL;
    }

    switch (s->target) {
    case V4L2_SEL_TGT_CROP:
        cctx->picinfo.crop_l = s->r.left;
        cctx->picinfo.crop_t = s->r.top;
        cctx->picinfo.crop_w = s->r.width;
        cctx->picinfo.crop_h = s->r.height;
        cam_stream_setCropWindow(gStream[cctx->streamId], s->r.left, s->r.top, s->r.width, s->r.height);
        break;
    default:
        errno = EINVAL;
        return ERROR_EINVAL;
    }

    return 0;
}


static int vidioc_cam_enum_framesizes(void *fh, void *p)
{
    struct v4l2_frmsizeenum *fsize = (struct v4l2_frmsizeenum *)p;
    int i = 0;

    if (fsize->index != 0){
        errno = EINVAL;
        return ERROR_EINVAL;
    }

    for (i = 0; i < CAM_NUM_FRAMESIZE; ++i) {
        if (fsize->pixel_format == cam_video_framesizes[i].fourcc) {
            fsize->type = V4L2_FRMSIZE_TYPE_STEPWISE;
            fsize->stepwise = cam_video_framesizes[i].stepwise;
            return 0;
        }
    }
    errno = EINVAL;
    return ERROR_EINVAL;
}

static int vidioc_cam_enum_frameintervals(void *fh, void *p)
{
    struct v4l2_frmivalenum *fival = (struct v4l2_frmivalenum *)p;
    int i = 0;

    if (fival->index != 0){
        errno = EINVAL;
        return ERROR_EINVAL;
    }

    for (i = 0; i < CAM_NUM_FRAMESIZE; ++i) {
        if (fival->pixel_format == cam_video_framesizes[i].fourcc) {
            fival->type = V4L2_FRMIVAL_TYPE_DISCRETE;
            fival->discrete.numerator = 1;
            fival->discrete.denominator = 30;
            return 0;
        }
    }

    errno = EINVAL;
    return ERROR_EINVAL;
}

static int __vidioc_cam_enum_fmt(struct v4l2_fmtdesc *f, bool bOutput)
{
    V4L2VidFmt* fmt;
    int i, j=0;

    const unsigned sz = sizeof(f->description);
    for (i=0; i<CAM_NUM_FORMAT; i++) {
        if(bOutput && cam_video_format[i].type != FMT_ES)
            continue;
        if(!bOutput && cam_video_format[i].type != FMT_FRAME)
            continue;
        if(j == f->index)
            break;
        ++j;
    }
    if(i == CAM_NUM_FORMAT){
        errno = EINVAL;
        return ERROR_EINVAL;
    }

    fmt = &cam_video_format[i];

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

static int vidioc_cam_enum_fmt(void *fh, void *p)
{
    struct v4l2_fmtdesc *f = (struct v4l2_fmtdesc *)p;
    if(V4L2_TYPE_IS_OUTPUT(f->type))
        return __vidioc_cam_enum_fmt(f, true);
    return __vidioc_cam_enum_fmt(f, false);
}

static int vidioc_cam_s_fmt(void *fh, void *p)
{
    M2MCTX(fh)
    V4L2CameraCtx *cctx = &ctx->codec.ctx.cam;
    int ret = 0, i = 0, stride = 0, is_direct_out = 0 ,physical_camera_id = 1;
    struct v4l2_format *fmt = (struct v4l2_format *)p;
    struct v4l2_pix_format_mplane *dst_pix_mp, *src_pix_mp;

    if(!V4L2_TYPE_IS_MULTIPLANAR(fmt->type)) {
        V4L2LOGE("Unsupported v4l2 format %d\n", fmt->type);
        errno = EINVAL;
        return ERROR_EINVAL;
    }

    V4L2VidFmt *vidfmt = cam_find_format(fmt);
    if(vidfmt == NULL){
        V4L2LOGE("Unable to get format details %d\n", fmt->type);
        errno = EINVAL;
        return ERROR_EINVAL;
    }

    stride = fmt->fmt.pix.xfer_func;
    is_direct_out = fmt->fmt.pix.field; // No post processing needed for direct stream
    physical_camera_id = fmt->fmt.pix.quantization;

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
        unsigned int ySize=0, uvSize=0;

        // terminate previous stream
        if(cctx->streamId != CAM_STREAM_INVALID) {
            cam_stream_stop(gStream[cctx->streamId]);
            gStream[cctx->streamId] = NULL;
            cctx->streamId = CAM_STREAM_INVALID;
        }

        // configure stream
        int streamId = cam_stream_findId(src_pix_mp->width, src_pix_mp->height);
        if(streamId != CAM_STREAM_INVALID) {
            gStream[streamId] = cam_stream_create(streamId, ctx->pV4l2Ctx->amp_factory,
                                                  physical_camera_id, is_direct_out);
            cam_stream_config(gStream[streamId], AMP_DISP_SRCFMT_YUV420SP, src_pix_mp->width, src_pix_mp->height);
            cam_stream_regCallback(gStream[streamId], frameReadyEvent, fh);
//Direct stream don't have components in secondary pipeline
            if (!is_direct_out) {
                cam_stream_getFrameSize(gStream[streamId], &ySize, &uvSize);
            }
            if (stride <= 0) {
                stride = src_pix_mp->width;
            }
            ySize = stride * src_pix_mp->height;
            uvSize = (stride * src_pix_mp->height) / 2;
        }
        cctx->streamId = streamId;

        strm->format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        dst_pix_mp->pixelformat = src_pix_mp->pixelformat;

        V4L2VidFmt *vidfmt = cam_find_format(fmt);
        dst_pix_mp->num_planes = vidfmt->num_planes;
        dst_pix_mp->width = src_pix_mp->width;
        dst_pix_mp->height = src_pix_mp->height;

        dst_pix_mp->plane_fmt[0].bytesperline = stride;
        dst_pix_mp->plane_fmt[0].sizeimage = ySize;
        if(vidfmt->num_planes == 2) {
            dst_pix_mp->plane_fmt[1].bytesperline = stride / 2;
            dst_pix_mp->plane_fmt[1].sizeimage = uvSize;
        }
        for(i=0; i<dst_pix_mp->num_planes; i++){
            memset(&dst_pix_mp->plane_fmt[i].reserved, 0, sizeof(dst_pix_mp->plane_fmt[0].reserved));
        }
    }
    show_v4l2fmt(&strm->format);
    return ret;
}

static int vidioc_cam_g_fmt(void *fh, void *p)
{
    int ret = 0;
    struct v4l2_format *fmt = (struct v4l2_format *)p;
    V4L2Stream *strm = v4l2_get_stream(fh, fmt->type);

    ret = v4l2_stream_get_format(strm, fmt);
    show_v4l2fmt(fmt);
    return ret;
}

//only found it in in image processer for chome gpu v4l2 code.
static int vidioc_cam_try_fmt(void *fh, void *p)
{
    int i=0;

    struct v4l2_format *f = (struct v4l2_format *)p;
    struct v4l2_pix_format_mplane *pix_fmt_mp = &f->fmt.pix_mp;
    int idx = V4L2_TYPE_IS_OUTPUT(f->type) ? DEFAULT_OUT_FMT_ID : DEFAULT_CAP_FMT_ID;

    V4L2VidFmt *fmt = cam_find_format(f);
    if(!fmt) {
        pix_fmt_mp->pixelformat= cam_video_format[idx].fourcc;
        fmt = &cam_video_format[idx];
    }

    if(V4L2_TYPE_IS_OUTPUT(f->type)) {
        pix_fmt_mp->num_planes = 1;
        pix_fmt_mp->plane_fmt[0].bytesperline = 0;
    } else {
        pix_fmt_mp->num_planes = fmt->num_planes;
        pix_fmt_mp->width = DEFAULT_W;
        pix_fmt_mp->height = DEFAULT_H;  //TODO: chekc this data if valid
        HRESULT ret = SUCCESS;
        UINT32 ySize = 0, uvSize = 0;
        ySize =  pix_fmt_mp->width * pix_fmt_mp->height;
        uvSize = pix_fmt_mp->width * pix_fmt_mp->height / 2;

        pix_fmt_mp->plane_fmt[0].bytesperline = pix_fmt_mp->width;
        pix_fmt_mp->plane_fmt[0].sizeimage = ySize;
        if(fmt->num_planes == 2) {
            pix_fmt_mp->plane_fmt[1].bytesperline = pix_fmt_mp->width;
            pix_fmt_mp->plane_fmt[1].sizeimage = uvSize;
        }
        for(i=0; i<pix_fmt_mp->num_planes; i++){
            memset(&pix_fmt_mp->plane_fmt[i].reserved, 0, sizeof(pix_fmt_mp->plane_fmt[0].reserved));
        }
    }
    pix_fmt_mp->field = V4L2_FIELD_NONE;
    memset(&pix_fmt_mp->reserved, 0 , sizeof(pix_fmt_mp->reserved));

    return 0;
}

static int vidioc_cam_qbuf(void *fh, void *p)
{
    struct v4l2_buffer *buf = (struct v4l2_buffer *)p;

    v4l2_buffer_QBuff(fh, buf);
    cam_try_capture(fh);

    return 0;
}

static int vidioc_cam_dqbuf(void *fh, void *p)
{
    struct v4l2_buffer *buf = (struct v4l2_buffer *)p;

    return v4l2_buffer_DQBuff(fh, buf);
}

static int vidioc_cam_querybuf(void *fh, void *p)
{
    struct v4l2_buffer *buf = (struct v4l2_buffer *)p;

    return v4l2_buffer_QueryBuff(fh, buf);
}

static int vidioc_cam_reqbufs(void *fh, void *p)
{
    struct v4l2_requestbuffers * req;
    req = (struct v4l2_requestbuffers *)p;

    V4L2Stream *strm = v4l2_get_stream(fh, req->type);

    int count = req->count;

    // free previous buffers
    req->count = 0;
    v4l2_stream_ReqBuff(strm, req);

    req->count = count;
    return v4l2_stream_ReqBuff(strm, req);
}

static int vidioc_cam_expbuf(void *fh, void *eb)
{
    return v4l2_buffer_expbuf(fh, eb);
}

static int vidioc_cam_subscribe_event(void *fh, void *p)
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
static int vidioc_cam_unsubscribe_event(void *fh, void *p)
{
    struct v4l2_event_subscription* sub;
    sub = (struct v4l2_event_subscription*)p;

    return v4l2_event_unsubscribe(fh, sub);
}

static int vidioc_cam_dqevent(void *fh, void *p)
{
    M2MCTX(fh)
    struct v4l2_event* e = (struct v4l2_event *)p;

    return v4l2_event_dequeue(fh, e, ctx->nonblocking);
}

static int vidioc_cam_g_ctrl(void *fh, void *p)
{
    int ret = 0;
    M2MCTX(fh)
    struct v4l2_control *c = (struct v4l2_control *)p;
    V4L2CameraCtx *cctx = &ctx->codec.ctx.cam;
    switch(c->id){
        case V4L2_CID_MIN_BUFFERS_FOR_CAPTURE:
            c->value = cctx->picinfo.buf_num;
        break;
        default:
            errno = EINVAL;
            ret = ERROR_EINVAL;
        break;

    }

    V4L2LOGD("getctrl id %d val %d", c->id, c->value);
    return ret;
}

static int vidioc_cam_s_ctrl(void *fh, void *p)
{
    int ret = 0;
    M2MCTX(fh)
    struct v4l2_control *c = (struct v4l2_control *)p;
    V4L2CameraCtx *cctx = &ctx->codec.ctx.cam;
    //TODO: use this function for adjusting the PQ controls
    switch(c->id){
        default:
            errno = EINVAL;
            ret = ERROR_EINVAL;
        break;

    }

    V4L2LOGD("setctrl id %d val %d", c->id, c->value);
    return ret;
}

HRESULT v4l2_camera_init(V4L2M2MContext *fh)
{
    HRESULT ret = SUCCESS;

    V4L2_API_ENTRY();
    fh->codec.hwState = AMP_IDLE;

    fh->codec.ctx.cam.streamId = CAM_STREAM_INVALID;

    //default pix_mp value
    cam_set_ctx_default_params(fh);
    //flush BD
    ret = createFlushBDBuffer((void *)fh);
    ASSERT_RET_SHOW(ret, "unable to allocate flush buffer");

    //! only need to override vdec own function
    //! buffer
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_QBUF, vidioc_cam_qbuf);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_DQBUF, vidioc_cam_dqbuf);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_QUERYBUF, vidioc_cam_querybuf);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_REQBUFS, vidioc_cam_reqbufs);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_EXPBUF, vidioc_cam_expbuf);
    //! fomat
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_ENUM_FMT, vidioc_cam_enum_fmt);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_TRY_FMT, vidioc_cam_try_fmt);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_G_FMT, vidioc_cam_g_fmt);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_S_FMT, vidioc_cam_s_fmt);
    //! event
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_SUBSCRIBE_EVENT, vidioc_cam_subscribe_event);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_UNSUBSCRIBE_EVENT, vidioc_cam_unsubscribe_event);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_DQEVENT, vidioc_cam_dqevent);
    //control
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_G_CTRL, vidioc_cam_g_ctrl);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_S_CTRL, vidioc_cam_s_ctrl);
    //selection(crop)
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_G_SELECTION, vidioc_cam_g_selection);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_S_SELECTION, vidioc_cam_s_selection);
    //frame
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_ENUM_FRAMESIZES, vidioc_cam_enum_framesizes);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_ENUM_FRAMEINTERVALS, vidioc_cam_enum_frameintervals);
    //stream
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_STREAMON, vidioc_cam_streamon);
    OVERRIDE_IOCTL_FNC(fh->ioctls, VIDIOC_STREAMOFF, vidioc_cam_streamoff);

    V4L2_API_EXIT(ret);
    return ret;

}

HRESULT v4l2_camera_deinit(V4L2M2MContext *fh)
{
    M2MCTX(fh)
    V4L2CameraCtx *cctx = &ctx->codec.ctx.cam;
    HRESULT ret = SUCCESS;

    V4L2_API_ENTRY();
    AMP_COMPONENT isp = fh->comp;

    if(cctx->streamId != CAM_STREAM_INVALID) {
        cam_stream_close(gStream[cctx->streamId]);
        gStream[cctx->streamId] = NULL;
        cctx->streamId = CAM_STREAM_INVALID;
    }

    deleteFlushBDBuffer(fh->output.flush_buf->hBD);

    V4L2LOGH(DTAG"destroy instance %p\n", isp);
    V4L2_API_EXIT(ret);
    return ret;
}
