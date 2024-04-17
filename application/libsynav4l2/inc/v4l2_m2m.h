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
#ifndef SYNA_V4L2_M2M_H
#define SYNA_V4L2_M2M_H

#include <linux/videodev2.h>
#include "v4l2_stream.h"
#include "v4l2_misc.h"
#include "v4l2_ioctrl.h"
#include "v4l2_event.h"
#include "v4l2_m2m_dec.h"
#include "v4l2_m2m_enc.h"
#include "v4l2_m2m_cam.h"

#include "OSAL_api.h"
#include "amp_client.h"

typedef enum _codec_type_ {
    DECODER = 0,
    ENCODER = 1,
    CAMERA = 2
} V4L2CodecType;

typedef enum _fmt_type_ {
    FMT_ES = 0,
    FMT_FRAME = 1
}FmtType;

typedef struct _v4l2_video_fmt {
    int fourcc;
    FmtType type;
    int num_planes;
    const char *desc;
}V4L2VidFmt;

typedef struct _v4l2_codec_framesize {
    int fourcc;
    struct v4l2_frmsize_stepwise stepwise;
    const char *desc;
}V4L2FrameSize;

typedef enum _v4l2_codec_state {
   STATE_UNINIT     = 0,    //decoder/encoder uninitialized
   STATE_INIT       = 1,    //decoder/encoder initialized
   STATE_WORKING    = 2,    //capture stream and  output stream on
   STATE_HEADER     = 3,    //sps/pps decoded or encoded
   STATE_STOP       = 4,    //decoder capture stream off or encoder output stream off
   STATE_SEEK       = 5,    //decoder output stream off
   STATE_DRAIN      = 6,    //V4L2_CMD_STOP
   STATE_RESET      = 7,    //encoder capture stream off
   STATE_MAX                //invalid state
}V4L2CodecState;

typedef struct _v4l2_codec_ctx {
    AMP_BD_HANDLE flushBD; //! eos bd for input queue
    V4L2CodecState state;
    AMP_STATE hwState;
    union {
        V4L2DecoderCtx dec;
        V4L2EncoderCtx enc;
        V4L2CameraCtx cam;
    }ctx;

}V4L2CodecCtx;

typedef struct _v4l2_m2m_context {
    V4L2_CONTEXT *pV4l2Ctx;
    V4L2CodecType type;
    UINT32 fourcc;
    bool nonblocking;  //device open with nonblocking flag.

    /* amp codec comp */
    AMP_COMPONENT comp;
    HANDLE hListener;

    /* the codec context queues */
    V4L2Stream capture;
    V4L2Stream output;

    /*the ioctrl ops*/
    V4L2_IOCTL_ST *ioctls;

    /* null frame/packet received */
    int draining;

    /*
     *  poll_sem will wait when client call Poll(), client task will goto
     *  sleep until either:
     *  1. SetDevicePollInterrupt() is called (on another thread),
     *  2. there is new data to be read by the device, or an event to the
     *     device has arrived; in the latter case |*event_pending| will be set
     *     to true.
     */
    MV_OSAL_HANDLE_SEM_t poll_sem;
    UINT32 poll_device;
    UINT32 recieve_event;
    V4L2EventCtx event;
    BOOL poll_interrupt;
    //spin_lock  event_lock

    V4L2CodecCtx codec;

    long poll_api_count;
    long SetDevicePollInterrupt_api_count;
    long ClearDevicePollInterrupt_api_count;
    long Mmap_api_count;
    long Munmap_api_count;

    int WaitingForSem;

    MV_OSAL_HANDLE_TASK_t  hDataFeedTask;
    int DataFeedTaskRunning;

} V4L2M2MContext;

HRESULT v4l2_m2m_open(V4L2_CONTEXT *pCtx, V4L2CodecType type,
                    UINT32 v4l2_pixfmt, V4L2_HANDLE *handle);
HRESULT v4l2_m2m_close(V4L2_HANDLE handle);
HRESULT v4l2_m2m_WaitForPollSem(V4L2_HANDLE handle,
                               int poll_device,
                               int* event_pending);
HRESULT v4l2_m2m_PutPollSem(V4L2_HANDLE handle,
                    const char *func, UINT32 line);
HRESULT v4l2_m2m_SetDevicePollInterrupt(void *handle);
HRESULT v4l2_m2m_ClearDevicePollInterrupt(void *handle);

HRESULT v4l2_decoder_init(V4L2M2MContext *fh);
HRESULT v4l2_decoder_deinit(V4L2M2MContext *fh);
HRESULT v4l2_encoder_init(V4L2M2MContext *fh);
HRESULT v4l2_encoder_deinit(V4L2M2MContext *fh);
HRESULT v4l2_camera_init(V4L2M2MContext *fh);
HRESULT v4l2_camera_deinit(V4L2M2MContext *fh);
void print_ioctrl_count(V4L2_HANDLE handle );

#define     M2MCTX(p)                   \
        V4L2M2MContext* ctx;           \
        ctx = (V4L2M2MContext*)(p);

#define v4l2_setState(fh, s)   do {                                     \
        V4L2M2MContext *ctx = (V4L2M2MContext *)fh;                     \
        if(ctx) {                                                       \
            V4L2LOGD("[%s --> %s] done\n",             \
                    state2str(ctx->codec.state), state2str(s));         \
            ctx->codec.state = (s);                                     \
        }                                                               \
        else {                                                          \
            V4L2LOGD("[?? --> %s] failed\n", state2str(s));             \
        }                                                               \
    } while(0)

inline static V4L2CodecState v4l2_getState(V4L2M2MContext *ctx) {
    return ctx->codec.state;
}

#define  PUT_POLL_SEM(handle)   v4l2_m2m_PutPollSem(handle, __FUNCTION__, __LINE__)
#endif //SYNA_V4L2_M2M_H
