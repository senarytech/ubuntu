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
#include <linux/fcntl.h>
#include "v4l2_types.h"
#include "v4l2_stream.h"
#include "v4l2_m2m.h"
#include "v4l2_ioctrl.h"
#include "v4l2_m2m_dec.h"
#include "v4l2_m2m_enc.h"
#include "v4l2_m2m_cam.h"
#include "v4l2_log.h"

static void feed_data_proc(V4L2_HANDLE *handle )
{
    V4L2M2MContext *ctx = (V4L2M2MContext *) handle;

    V4L2LOGH("feed_data_proc() ctx->DataFeedTaskRunning(=%d)\n", ctx->DataFeedTaskRunning);
    while(ctx->DataFeedTaskRunning) {
        //vdec_try_decode(ctx);
        MV_OSAL_Task_Sleep(1000);
        V4L2LOGH("feed_data_proc() is alive...\n");
    }

    return;
}


/******************************************************************************
 *
 *                  V4L2 M2M Interface
 *
 ******************************************************************************/

HRESULT v4l2_m2m_open(V4L2_CONTEXT *pCtx, V4L2CodecType type,
                    UINT32 v4l2_pixfmt, V4L2_HANDLE *handle)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2M2MContext *ctx = NULL;

    ctx = malloc(sizeof(V4L2M2MContext));
    if (!ctx)
        return ERROR_NOMEM;

    memset(ctx, 0, sizeof(V4L2M2MContext));
    ctx->type = type;
    ctx->pV4l2Ctx = pCtx;
    ctx->fourcc = v4l2_pixfmt;
    ctx->nonblocking = 1; //set to 1 as default.

    //! init inctrl to get default ops setting
    v4l2_ioctrl_init(ctx);

    //! init output stream
    ret = v4l2_stream_init(&ctx->output, OUTPUT);
    if(ret != V4L2_SUCCESS) {
        V4L2LOGE("failed to init output stream(ret=0x%x)\n", ret);
        goto error;
    }
    ctx->output.m2m_ctx = (void *)ctx;
    //! init capture stream
    ret = v4l2_stream_init(&ctx->capture, CAPTURE);
    if(ret != V4L2_SUCCESS) {
        V4L2LOGE("failed to init capture stream(ret=0x%x)\n", ret);
        goto error;
    }
    ctx->capture.m2m_ctx = (void *)ctx;

    //! init event ctx
    ret = v4l2_event_init(&ctx->event);
    if(ret != V4L2_SUCCESS) {
        V4L2LOGE("failed to init event ctx (ret=0x%x)\n", ret);
        goto error;
    }

   // init codec by type
    if(type == DECODER) {
        ret = v4l2_decoder_init(ctx);
    } else if (type == ENCODER){
       ret = v4l2_encoder_init(ctx);
    } else if (type == CAMERA){
        ret = v4l2_camera_init(ctx);
    } else {
        V4L2LOGE("unsupported codec type %d\n", type);
        goto error;
    }

    if(ret != V4L2_SUCCESS) {
        V4L2LOGE("failed to init codec %d (ret=0x%x)\n", type, ret);
        goto error;
    }

    //! create poll semaphore
    ret = MV_OSAL_Sem_Create(&ctx->poll_sem, 0);
    if(ret != V4L2_SUCCESS) {
        V4L2LOGE("failed to create poll semaphore(ret=0x%x)\n", ret);
        goto error;
    }

    ctx->nonblocking = pCtx->flag & O_NONBLOCK;

    //! create data_feed thread
    //ctx->DataFeedTaskRunning = 1;
    //MV_OSAL_Task_Create(&ctx->hDataFeedTask, feed_data_proc, ctx);

    *handle = ctx;
    return V4L2_SUCCESS;
error:
    if(ctx) free(ctx);
    return ret;
}

HRESULT v4l2_m2m_close(V4L2_HANDLE handle)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2M2MContext *ctx = (V4L2M2MContext *)handle;

    V4L2LOGH("[%s]calling summary:\n", ctx->type == ENCODER?"encoder":"decoder");
    V4L2LOGH("poll_api_count: %ld\n", ctx->poll_api_count);
    V4L2LOGH("SetDevicePollInterrupt_api_count: %ld\n", ctx->SetDevicePollInterrupt_api_count);
    V4L2LOGH("ClearDevicePollInterrupt_api_count: %ld\n", ctx->ClearDevicePollInterrupt_api_count);
    V4L2LOGH("Mmap_api_count: %ld\n", ctx->Mmap_api_count);
    V4L2LOGH("Munmap_api_count: %ld\n", ctx->Munmap_api_count);
    print_ioctrl_count(handle);
    V4L2LOGH("output qbuff(): %d\n", ctx->output.qbuff_count);
    V4L2LOGH("output dqbuff_success(): %d\n", ctx->output.dqbuff_success_count);
    V4L2LOGH("output dqbuff_fail(): %d\n", ctx->capture.dqbuff_fail_count);
    V4L2LOGH("capture qbuff(): %d\n", ctx->capture.qbuff_count);
    V4L2LOGH("capture dqbuff_success(): %d\n", ctx->capture.dqbuff_success_count);
    V4L2LOGH("capture dqbuff_fail(): %d\n", ctx->capture.dqbuff_fail_count);

    if(ctx->type == DECODER) {
        ret = v4l2_decoder_deinit(handle);
    }
    else if(ctx->type == ENCODER){
        ret = v4l2_encoder_deinit(handle);
    }
    else if(ctx->type == CAMERA){
        ret = v4l2_camera_deinit(handle);
    }
    ASSERT_RET_SHOW(ret, "deinit codec failed");

    //! free client side memory
    AMP_FACTORY_Release(ctx->comp);

    //! deinit output stream
    v4l2_stream_deinit(&ctx->output);

    //! deinit capture stream
    v4l2_stream_deinit(&ctx->capture);

    //! deinit event ctx
    v4l2_event_deinit(&ctx->event);

    //! deinit ioctrl
    v4l2_ioctrl_deinit(ctx);

    //! destroy poll semaphore
    MV_OSAL_Sem_Destroy(&ctx->poll_sem);

    //ctx->DataFeedTaskRunning = 0;
    //MV_OSAL_Task_Join(&ctx->hDataFeedTask);
    free(ctx);

    return ret;
}

HRESULT v4l2_m2m_WaitForPollSem(V4L2_HANDLE handle,
                               int poll_device,
                               int* event_pending)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2M2MContext *ctx = (V4L2M2MContext *)handle;
    V4L2Stream *src = &ctx->output;
    V4L2Stream *dst = &ctx->capture;

    ctx->poll_device = poll_device;

    ctx->poll_api_count ++;
    if(ctx->poll_interrupt == 1) {
         return ret;
     }

    if(ctx->event.num_available
        || !list_is_empty(src->done_list)
        || !list_is_empty(dst->done_list)) {
       *event_pending = ctx->recieve_event;
        return ret;
    }


    ctx->WaitingForSem = 1;
    ctx->recieve_event = 0;
    ret = MV_OSAL_Sem_Wait(ctx->poll_sem);
    *event_pending = ctx->recieve_event;
    ctx->WaitingForSem = 0;

    return ret;
}

HRESULT v4l2_m2m_PutPollSem(V4L2_HANDLE handle, const char *func, UINT32 line)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2M2MContext *ctx = (V4L2M2MContext *)handle;
    V4L2Stream *src = &ctx->output;
    V4L2Stream *dst = &ctx->capture;
    bool bSemPostFlag = false;

    if (ctx->type == CAMERA) {
        bSemPostFlag = (ctx->WaitingForSem == 1);
    } else { //! Encoder/decoder cases
        bSemPostFlag = (ctx->WaitingForSem == 1) &&
                (list_size(src->done_list) > 0 ||
                 list_size(dst->done_list) > 0 ||
                 ctx->event.num_available);
    }

    if (bSemPostFlag) {
        ctx->WaitingForSem = 0;
        ret = MV_OSAL_Sem_Post(ctx->poll_sem);
    }

    return ret;
}

HRESULT v4l2_m2m_SetDevicePollInterrupt(void *handle)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2M2MContext *ctx = (V4L2M2MContext *)handle;
    ctx->SetDevicePollInterrupt_api_count ++;
    ctx->poll_interrupt = 1;
    if(ctx->WaitingForSem == 1) MV_OSAL_Sem_Post(ctx->poll_sem);

    return ret;
}

HRESULT v4l2_m2m_ClearDevicePollInterrupt(void *handle)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2M2MContext *ctx = (V4L2M2MContext *)handle;

    ctx->ClearDevicePollInterrupt_api_count ++;
    ctx->poll_interrupt = 0;

    return ret;
}

