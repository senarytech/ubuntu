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
#include "linux/videodev2.h"

#include "v4l2_types.h"
#include "v4l2_m2m.h"
#include "v4l2_ioctrl.h"
#include "v4l2_misc.h"
#include "amp_client.h"

#define V4L2_VERSION(a,b,c)     (((a) << 16) + ((b) << 8) + (c))
#define V4L2_DRIVER             "syna:v4l2"
#define V4L2_CARD               "syna:card"
#define V4L2_BUSINFO            "syna:bus"


static int vidioc_querycap(void *fh, void *p)
{
    HRESULT ret = V4L2_SUCCESS;
    struct v4l2_capability *cap = (struct v4l2_capability *)p;

    V4L2LOGH("vidioc_querycap() enter\n");
    memcpy(cap->driver, V4L2_DRIVER, strlen(V4L2_DRIVER)+1);
    memcpy(cap->card, V4L2_CARD, strlen(V4L2_CARD)+1);
    memcpy(cap->bus_info, V4L2_BUSINFO, strlen(V4L2_BUSINFO)+1);
    cap->version = V4L2_VERSION(4, 1, 14);
    cap->capabilities = V4L2_CAP_VIDEO_CAPTURE_MPLANE \
                        | V4L2_CAP_VIDEO_OUTPUT_MPLANE \
                        | V4L2_CAP_VIDEO_M2M_MPLANE \
                        | V4L2_CAP_STREAMING;
    cap->device_caps = V4L2_CAP_VIDEO_CAPTURE_MPLANE \
                       | V4L2_CAP_VIDEO_OUTPUT_MPLANE \
                       | V4L2_CAP_VIDEO_M2M_MPLANE \
                       | V4L2_CAP_STREAMING;
    return ret;
}

/* VIDIOC_QUERYCAP handler */
//DEFINE_IOCTL_STD_FNC(vidioc_querycap)
/* VIDIOC_ENUM_FMT handlers */
DEFINE_IOCTL_STD_FNC(vidioc_enum_fmt)
/* VIDIOC_G_FMT handlers */
DEFINE_IOCTL_STD_FNC(vidioc_g_fmt)
/* VIDIOC_S_FMT handlers */
DEFINE_IOCTL_STD_FNC(vidioc_s_fmt)
/* VIDIOC_TRY_FMT handlers */
DEFINE_IOCTL_STD_FNC(vidioc_try_fmt)
/* Buffer handlers */
DEFINE_IOCTL_STD_FNC(vidioc_reqbufs)
DEFINE_IOCTL_STD_FNC(vidioc_querybuf)
DEFINE_IOCTL_STD_FNC(vidioc_qbuf)
DEFINE_IOCTL_STD_FNC(vidioc_expbuf)
DEFINE_IOCTL_STD_FNC(vidioc_dqbuf)
/* Stream on/off */
DEFINE_IOCTL_STD_FNC(vidioc_streamon)
DEFINE_IOCTL_STD_FNC(vidioc_streamoff)
/* Input handling */
DEFINE_IOCTL_STD_FNC(vidioc_enum_input)
DEFINE_IOCTL_STD_FNC(vidioc_g_input)
DEFINE_IOCTL_STD_FNC(vidioc_s_input)
/* Output handling */
DEFINE_IOCTL_STD_FNC(vidioc_enum_output)
DEFINE_IOCTL_STD_FNC(vidioc_g_output)
DEFINE_IOCTL_STD_FNC(vidioc_s_output)
/* Control handling */
DEFINE_IOCTL_STD_FNC(vidioc_queryctrl)
DEFINE_IOCTL_STD_FNC(vidioc_g_ctrl)
DEFINE_IOCTL_STD_FNC(vidioc_s_ctrl)
DEFINE_IOCTL_STD_FNC(vidioc_g_ext_ctrls)
DEFINE_IOCTL_STD_FNC(vidioc_s_ext_ctrls)
DEFINE_IOCTL_STD_FNC(vidioc_try_ext_ctrls)
DEFINE_IOCTL_STD_FNC(vidioc_querymenu)
/* Crop ioctls */
DEFINE_IOCTL_STD_FNC(vidioc_cropcap)
DEFINE_IOCTL_STD_FNC(vidioc_g_crop)
DEFINE_IOCTL_STD_FNC(vidioc_s_crop)
DEFINE_IOCTL_STD_FNC(vidioc_g_selection)
DEFINE_IOCTL_STD_FNC(vidioc_s_selection)
/* Compression ioctls */
DEFINE_IOCTL_STD_FNC(vidioc_encoder_cmd)
DEFINE_IOCTL_STD_FNC(vidioc_try_encoder_cmd)
DEFINE_IOCTL_STD_FNC(vidioc_decoder_cmd)
DEFINE_IOCTL_STD_FNC(vidioc_try_decoder_cmd)
/* Stream type-dependent parameter ioctls */
DEFINE_IOCTL_STD_FNC(vidioc_g_parm)
DEFINE_IOCTL_STD_FNC(vidioc_s_parm)
/* frame ioctrl */
DEFINE_IOCTL_STD_FNC(vidioc_enum_framesizes)
DEFINE_IOCTL_STD_FNC(vidioc_enum_frameintervals)
/* event ioctrl */
DEFINE_IOCTL_STD_FNC(vidioc_dqevent)
DEFINE_IOCTL_STD_FNC(vidioc_subscribe_event)
DEFINE_IOCTL_STD_FNC(vidioc_unsubscribe_event)

static V4L2_IOCTL_ST default_v4l2_ioctls[] = {
    /* VIDIOC_QUERYCAP handler */
    IOCTL_INFO_FNC(VIDIOC_QUERYCAP, vidioc_querycap),

    /* VIDIOC_ENUM_FMT handlers */
    IOCTL_INFO_STD(VIDIOC_ENUM_FMT, vidioc_enum_fmt),

    /* VIDIOC_G_FMT handlers */
    IOCTL_INFO_STD(VIDIOC_G_FMT, vidioc_g_fmt),

    /* VIDIOC_S_FMT handlers */
    IOCTL_INFO_STD(VIDIOC_S_FMT, vidioc_g_fmt),

    /* VIDIOC_TRY_FMT handlers */
    IOCTL_INFO_STD(VIDIOC_TRY_FMT, vidioc_try_fmt),

    /* Buffer handlers */
    IOCTL_INFO_STD(VIDIOC_REQBUFS, vidioc_reqbufs),
    IOCTL_INFO_STD(VIDIOC_QUERYBUF, vidioc_querybuf),
    IOCTL_INFO_STD(VIDIOC_QBUF, vidioc_qbuf),
    IOCTL_INFO_STD(VIDIOC_EXPBUF, vidioc_expbuf),
    IOCTL_INFO_STD(VIDIOC_DQBUF, vidioc_dqbuf),

    /* Stream on/off */
    IOCTL_INFO_STD(VIDIOC_STREAMON, vidioc_streamon),
    IOCTL_INFO_STD(VIDIOC_STREAMOFF, vidioc_streamoff),

    /* Input handling */
    IOCTL_INFO_STD(VIDIOC_ENUMINPUT, vidioc_enum_input),
    IOCTL_INFO_STD(VIDIOC_G_INPUT, vidioc_g_input),
    IOCTL_INFO_STD(VIDIOC_S_INPUT, vidioc_s_input),

    /* Output handling */
    IOCTL_INFO_STD(VIDIOC_ENUMOUTPUT, vidioc_enum_output),
    IOCTL_INFO_STD(VIDIOC_G_OUTPUT, vidioc_g_output),
    IOCTL_INFO_STD(VIDIOC_S_OUTPUT, vidioc_s_output),

    /* Control handling */
    IOCTL_INFO_STD(VIDIOC_QUERYCTRL, vidioc_queryctrl),
    IOCTL_INFO_STD(VIDIOC_G_CTRL, vidioc_g_ctrl),
    IOCTL_INFO_STD(VIDIOC_S_CTRL, vidioc_s_ctrl),
    IOCTL_INFO_STD(VIDIOC_G_EXT_CTRLS, vidioc_g_ext_ctrls),
    IOCTL_INFO_STD(VIDIOC_S_EXT_CTRLS, vidioc_s_ext_ctrls),
    IOCTL_INFO_STD(VIDIOC_TRY_EXT_CTRLS, vidioc_try_ext_ctrls),
    IOCTL_INFO_STD(VIDIOC_QUERYMENU, vidioc_querymenu),

    /* Crop ioctls */
    IOCTL_INFO_STD(VIDIOC_CROPCAP, vidioc_cropcap),
    IOCTL_INFO_STD(VIDIOC_G_CROP, vidioc_g_crop),
    IOCTL_INFO_STD(VIDIOC_S_CROP, vidioc_s_crop),
    IOCTL_INFO_STD(VIDIOC_G_SELECTION, vidioc_g_selection),
    IOCTL_INFO_STD(VIDIOC_S_SELECTION, vidioc_s_selection),

    /* Compression ioctls */
    IOCTL_INFO_STD(VIDIOC_ENCODER_CMD, vidioc_encoder_cmd),
    IOCTL_INFO_STD(VIDIOC_TRY_ENCODER_CMD, vidioc_try_encoder_cmd),
    IOCTL_INFO_STD(VIDIOC_DECODER_CMD, vidioc_decoder_cmd),
    IOCTL_INFO_STD(VIDIOC_TRY_DECODER_CMD, vidioc_try_decoder_cmd),

    /* Stream type-dependent parameter ioctls */
    IOCTL_INFO_STD(VIDIOC_G_PARM, vidioc_g_parm),
    IOCTL_INFO_STD(VIDIOC_S_PARM, vidioc_s_parm),

    /* frame ioctrl */
    IOCTL_INFO_STD(VIDIOC_ENUM_FRAMESIZES, vidioc_enum_framesizes),
    IOCTL_INFO_STD(VIDIOC_ENUM_FRAMEINTERVALS, vidioc_enum_frameintervals),

    /* event ioctrl */
    IOCTL_INFO_STD(VIDIOC_DQEVENT, vidioc_dqevent),
    IOCTL_INFO_STD(VIDIOC_SUBSCRIBE_EVENT, vidioc_subscribe_event),
    IOCTL_INFO_STD(VIDIOC_UNSUBSCRIBE_EVENT, vidioc_unsubscribe_event),
};
#define V4L2_IOCTLS sizeof(default_v4l2_ioctls)/sizeof(default_v4l2_ioctls[0])

static BOOL is_known_ioctl(UINT32 cmd)
{
    if (_IOC_NR(cmd) >= V4L2_IOCTLS)
        return FALSE;
    return default_v4l2_ioctls[_IOC_NR(cmd)].ioctl == cmd;
}

HRESULT v4l2_ioctrl_execute(V4L2_HANDLE handle, INT32 cmd, void* arg)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2M2MContext *ctx = (V4L2M2MContext *)handle;

    if(is_known_ioctl(cmd)) {
        ret = ctx->ioctls[_IOC_NR(cmd)].func(handle, arg);
        ctx->ioctls[_IOC_NR(cmd)].count ++;
    }
    else {
        ret = ERROR_NOTIMPL;
    }

    return ret;
}

void print_ioctrl_count(V4L2_HANDLE handle )
{
    V4L2M2MContext *ctx = (V4L2M2MContext *)handle;
    UINT32 i = 0;

    for(i=0; i<V4L2_IOCTLS; i++) {
        UINT32 cmd = ctx->ioctls[i].ioctl;
        if(ctx->ioctls[i].count > 0)
            V4L2LOGH("%s() : %ld\n", cmd2str(cmd), ctx->ioctls[i].count);
    }

}

HRESULT v4l2_ioctrl_init(V4L2_HANDLE handle)
{
    V4L2M2MContext *ctx = (V4L2M2MContext *)handle;

    ctx->ioctls = (V4L2_IOCTL_ST *)malloc(sizeof(default_v4l2_ioctls));
    memcpy(ctx->ioctls, default_v4l2_ioctls, sizeof(default_v4l2_ioctls));

    return V4L2_SUCCESS;
}

void v4l2_ioctrl_deinit(V4L2_HANDLE handle)
{
    V4L2M2MContext *ctx = (V4L2M2MContext *)handle;

    if(ctx->ioctls) free(ctx->ioctls);
    ctx->ioctls = NULL;
}



