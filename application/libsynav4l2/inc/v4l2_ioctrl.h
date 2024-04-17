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

#ifndef _V4L2_IOCTL_H
#define _V4L2_IOCTL_H

#include <linux/videodev2.h>
#include "v4l2_log.h"

typedef struct v4l2_ioctl_ops {
    /* ioctl callbacks */

    /* VIDIOC_QUERYCAP handler */
    int (*vidioc_querycap)(void *fh, struct v4l2_capability *cap);

    /* VIDIOC_ENUM_FMT handlers */
    int (*vidioc_enum_fmt)(void *fh, struct v4l2_fmtdesc *f);

    /* VIDIOC_G_FMT handlers */
    int (*vidioc_g_fmt)(void *fh, struct v4l2_format *f);

    /* VIDIOC_S_FMT handlers */
    int (*vidioc_s_fmt)(void *fh, struct v4l2_format *f);

    /* VIDIOC_TRY_FMT handlers */
    int (*vidioc_try_fmt)(void *fh, struct v4l2_format *f);

    /* Buffer handlers */
    int (*vidioc_reqbufs)(void *fh, struct v4l2_requestbuffers *b);
    int (*vidioc_querybuf)(void *fh, struct v4l2_buffer *b);
    int (*vidioc_qbuf)(void *fh, struct v4l2_buffer *b);
    int (*vidioc_expbuf)(void *fh, struct v4l2_exportbuffer *e);
    int (*vidioc_dqbuf)(void *fh, struct v4l2_buffer *b);

    /* Stream on/off */
    int (*vidioc_streamon)(void *fh, void *arg);
    int (*vidioc_streamoff)(void *fh, void *arg);

    /* Input handling */
    int (*vidioc_enum_input)(void *fh, struct v4l2_input *inp);
    int (*vidioc_g_input)(void *fh, void *arg);
    int (*vidioc_s_input)(void *fh, void *arg);

    /* Output handling */
    int (*vidioc_enum_output)(void *fh, struct v4l2_output *a);
    int (*vidioc_g_output)(void *fh, void *arg);
    int (*vidioc_s_output)(void *fh, void *arg);

    /* Control handling */
    int (*vidioc_queryctrl)(void *fh, struct v4l2_queryctrl *a);
    int (*vidioc_g_ctrl)(void *fh, struct v4l2_control *a);
    int (*vidioc_s_ctrl)(void *fh, struct v4l2_control *a);
    int (*vidioc_g_ext_ctrls)(void *fh, struct v4l2_ext_controls *a);
    int (*vidioc_s_ext_ctrls)(void *fh,  struct v4l2_ext_controls *a);
    int (*vidioc_try_ext_ctrls)(void *fh, struct v4l2_ext_controls *a);
    int (*vidioc_querymenu)(void *fh, struct v4l2_querymenu *a);

    /* Crop ioctls */
    int (*vidioc_cropcap)(void *fh, struct v4l2_cropcap *a);
    int (*vidioc_g_crop)(void *fh, struct v4l2_crop *a);
    int (*vidioc_s_crop)(void *fh, const struct v4l2_crop *a);
    int (*vidioc_g_selection)(void *fh, struct v4l2_selection *s);
    int (*vidioc_s_selection)(void *fh, struct v4l2_selection *s);

    /* Compression ioctls */
    int (*vidioc_encoder_cmd)(void *fh, struct v4l2_encoder_cmd *a);
    int (*vidioc_try_encoder_cmd)(void *fh, struct v4l2_encoder_cmd *a);
    int (*vidioc_decoder_cmd)(void *fh, struct v4l2_decoder_cmd *a);
    int (*vidioc_try_decoder_cmd)(void *fh, struct v4l2_decoder_cmd *a);

    /* Stream type-dependent parameter ioctls */
    int (*vidioc_g_parm)(void *fh, struct v4l2_streamparm *a);
    int (*vidioc_s_parm)(void *fh, struct v4l2_streamparm *a);

    /* frame handler */
    int (*vidioc_enum_framesizes)(void *fh, struct v4l2_frmsizeenum *fsize);
    int (*vidioc_enum_frameintervals)(void *fh, struct v4l2_frmivalenum *fival);

    /* event handler */
    int (*vidioc_dqevent)(void *fh, const struct v4l2_event *evt);
    int (*vidioc_subscribe_event)(void *fh,
             const struct v4l2_event_subscription *sub);
    int (*vidioc_unsubscribe_event)(void *fh,
             const struct v4l2_event_subscription *sub);
}V4L2_IOCTL_OPS;

typedef struct v4l2_ioctl_st {
    unsigned int ioctl;
    const char * const name;
    int (*func)(void *fh, void *p);
    long count;
} V4L2_IOCTL_ST;

#define DEFINE_IOCTL_STD_FNC(_vidioc)                       \
    static int __vidioc_ ## _vidioc ## _fnc(                \
            void *fh, void *p) {                            \
        V4L2LOGD("std ioctrl -- %s() called\n", __FUNCTION__);\
        return V4L2_SUCCESS;                                \
    }

#define IOCTL_INFO_FNC(cmd, _func)                      \
    [_IOC_NR(cmd)] = {                                  \
        .ioctl = cmd,                                   \
        .name = #cmd,                                   \
        .func = _func,                                  \
    }

#define IOCTL_INFO_STD(_ioctl, _vidioc)                 \
    IOCTL_INFO_FNC(_ioctl, __vidioc_ ## _vidioc ## _fnc)

#define OVERRIDE_IOCTL_FNC(ioctrl, cmd, _func)      \
    do {                                            \
        ioctrl[_IOC_NR(cmd)].ioctl = cmd;           \
        ioctrl[_IOC_NR(cmd)].func = _func;          \
    } while(0)


HRESULT v4l2_ioctrl_execute(V4L2_HANDLE handle, INT32 cmd, void* arg);
HRESULT v4l2_ioctrl_init(void *fh);
void v4l2_ioctrl_deinit(void *fh);


#endif /* _V4L2_IOCTL_H */
