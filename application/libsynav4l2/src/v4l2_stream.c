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
#include "v4l2_buffer.h"
#include "v4l2_m2m.h"
#include "v4l2_log.h"

/*****************************************************************************
*
*             V4L2 Stream Interface
*
*****************************************************************************/
//#define SHM_TEST_DEBUG
void print_debug_info()
{
#ifdef SHM_TEST_DEBUG
    printf("==========================================\n");
    //system("cat /sys/kernel/debug/dma_buf/bufinfo");
    system("cat /sys/kernel/debug/ion/ion_berlin_heap_49c00000");
    //system("cat /sys/kernel/debug/shm/gid");
    printf("==========================================\n");
#endif
}

static HRESULT EnqueueInputBuffer_hw(V4L2M2MContext *ctx, PushBDFuc pFunc)
{
    V4L2Buffer *buff;
    HRESULT ret;
    V4L2Stream *strm = &ctx->output;

    if(!ctx->output.streamon)
        return V4L2_SUCCESS;

    if(ctx->codec.state == STATE_SEEK)
        return V4L2_SUCCESS;

    while(1) {
        std_list_pop_front(strm->queued_list, (void **)&buff);
        if(NULL == buff) {
            break;
        }

        //caputre buffer send flush bd
        if(strm->eos_sent == 0) {
            if(buff == strm->flush_buf) {
                strm->eos_sent = 1;
            }
            V4L2_DATAFLOW_TRACE("[%s][Buff:%d] push [BD:%p] to Input Port [Buffer Queued:%d], eos[%d]\n",
                            STREAM_NAME(strm->strm_type), buff->buf.index,
                            buff->hBD, strm->queued_count, strm->eos_sent);
            DUMP_BD_MEMINFO(buff->hBD);
            ret = (*pFunc)((void *)ctx, AMP_PORT_INPUT, buff->hBD);
        }
        else {
            break;
        }
    }
    return ret;
}

static HRESULT EnqueueOutputBuffer_hw(V4L2M2MContext *ctx, PushBDFuc pFunc)
{
    V4L2Buffer *buff;
    HRESULT ret;
    V4L2Stream *strm = &ctx->capture;

    if(!ctx->capture.streamon)
        return V4L2_SUCCESS;

    if(ctx->codec.state == STATE_STOP)
        return V4L2_SUCCESS;

    while(1) {
        std_list_pop_front(strm->queued_list, (void **)&buff);
        if((NULL == buff) || (NULL == buff->hBD)) {
            break;
        }

        V4L2_DATAFLOW_TRACE("[%s][Buff:%d] push [BD:%p] to Output Port [Buffer Queued:%d]\n",
                        STREAM_NAME(strm->strm_type), buff->buf.index,
                        buff->hBD, strm->queued_count);
        DUMP_BD_MEMINFO(buff->hBD);
        ret = (*pFunc)((void *)ctx, AMP_PORT_OUTPUT, buff->hBD);
    }
    return ret;
}

void v4l2_stream_enqueue_hw(void *ctx, PushBDFuc pFunc)
{

    V4L2M2MContext *pCtx = (V4L2M2MContext *)ctx;
    if(pCtx->codec.state == STATE_UNINIT ||
       pCtx->codec.state == STATE_INIT ||
       pCtx->codec.state == STATE_STOP ||
       pCtx->codec.state == STATE_RESET)
    {
       return;
    }

    EnqueueOutputBuffer_hw(pCtx, pFunc);
    EnqueueInputBuffer_hw(pCtx, pFunc);
}

V4L2Stream* v4l2_get_stream(void *ctx, enum v4l2_buf_type type)
{
    V4L2M2MContext *p = (V4L2M2MContext *)ctx;
    if(V4L2_TYPE_IS_OUTPUT(type))
        return &p->output;
    return &p->capture;
}

HRESULT v4l2_stream_on(V4L2Stream* strm)
{
    int ret = V4L2_SUCCESS;

    /*TODO on/off stream*/
    strm->streamon = TRUE;
    strm->last_buff_sent = 0;
    strm->eos_sent = 0;

    return ret;
}

HRESULT v4l2_stream_off(V4L2Stream* strm)
{
    V4L2Buffer *buffer;

    strm->streamon = FALSE;

    if(strm->queued_count == 0)
        return V4L2_SUCCESS;

    /*drop the buffer when stream is off*/
    MV_OSAL_Mutex_Lock(strm->done_lock);
    std_list_clear(strm->done_list);
    MV_OSAL_Mutex_Unlock(strm->done_lock);

    std_list_clear(strm->queued_list);
    strm->queued_count = 0;

    return V4L2_SUCCESS;
}

HRESULT v4l2_stream_get_format(V4L2Stream* strm, struct v4l2_format *fmt)
{

    memcpy(fmt, &strm->format, sizeof(struct v4l2_format));

    return 0;
}

HRESULT v4l2_stream_set_format(V4L2Stream* strm, struct v4l2_format *fmt)
{

    /*TODO: Aplly fmt to amp comp*/
    memcpy(&strm->format, fmt, sizeof(struct v4l2_format));

    return 0;
}

HRESULT v4l2_stream_ReqBuff(V4L2Stream* strm,
                    struct v4l2_requestbuffers *reg)
{
    HRESULT ret = V4L2_SUCCESS;
    UINT32 i = 0;
    V4L2Buffer *buff = NULL;


    if(reg->count > 0) {
        //! allocate buffer
        V4L2LOGD("[STREAM][%s][num_buffers=%d][%s] Allocate \n", STREAM_NAME(strm->strm_type),
                                reg->count, mem2str(reg->memory));
        strm->buff_type = reg->type;
        strm->num_buffers = reg->count;
        strm->buffers = (V4L2Buffer *)malloc(sizeof(V4L2Buffer)*strm->num_buffers);
        if(strm->buffers == NULL) {
            ret = ERROR_NOMEM;
            goto error;
        }
        memset(strm->buffers, 0, sizeof(V4L2Buffer)*strm->num_buffers);
        for(i=0; i<strm->num_buffers; i++) {
            buff = &strm->buffers[i];
            buff->stream = (void *)strm;
            buff->buf.index = i;
            buff->buf.memory = reg->memory;
            buff->buf.type = reg->type;
            buff->buf.reserved = reg->reserved[0];
            ret = v4l2_buffer_init(buff, i);
            if(ret != V4L2_SUCCESS) {
                goto error;
            }
        }
    }
    else {
        //! free buffer
        V4L2LOGD("[STREAM][%s][num_buffers=%d][%s] Free\n", STREAM_NAME(strm->strm_type),
                                strm->num_buffers, mem2str(reg->memory));
        for(i=0; i<strm->num_buffers; i++) {
            buff = &strm->buffers[i];
            ret = v4l2_buffer_deinit(buff);
            if(ret != V4L2_SUCCESS) {
                goto error;
            }
        }
        if(strm->buffers) {
            free(strm->buffers);
            strm->buffers = NULL;
        }
        strm->num_buffers = 0;
    }
    //reg->capabilities |= V4L2_BUF_CAP_SUPPORTS_MMAP;
    print_debug_info();

    return V4L2_SUCCESS;
error:
    if(strm->buffers) {
        for(i=0; i<strm->num_buffers; i++) {
            buff = &strm->buffers[i];
            v4l2_buffer_deinit(buff);
        }
        free(strm->buffers);
        strm->buffers = NULL;
    }
    V4L2LOGE("v4l2_stream_ReqBuff(ret=0x%x)\n", ret);
    return ret;
}

HRESULT v4l2_stream_deinit(V4L2Stream* strm)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2Buffer *buff = NULL;
    UINT32 i = 0;

    if(!strm) {
        return ERROR_ERRPARAM;
    }

    if(strm->buffers) {
        for(i=0; i<strm->num_buffers; i++) {
            buff = &strm->buffers[i];
            v4l2_buffer_deinit(buff);
        }
        free(strm->buffers);
        strm->buffers = NULL;
    }

    if(strm->flush_buf) {
        free(strm->flush_buf);
        strm->flush_buf = NULL;
    }
    if(strm->queued_list) std_list_dtor(strm->queued_list);
    if(strm->done_list) std_list_dtor(strm->done_list);
    if(strm->done_lock) MV_OSAL_Mutex_Destroy(&strm->done_lock);

    return ret;
}

HRESULT v4l2_stream_init(V4L2Stream* strm, StreamType type)
{
    HRESULT ret = V4L2_SUCCESS;

    memset(strm, 0, sizeof(V4L2Stream));
    strm->strm_type = type;

    ret = MV_OSAL_Mutex_Create(&strm->done_lock);
    ASSERT_RET_SHOW(ret, "error!! failed create mutex");

    strm->queued_list = std_list_ctor(NULL, NULL);
    if(!strm ->queued_list) {
        V4L2LOGE("failed to creat incoming_queue\n");
        goto error;
    }

    strm->done_list = std_list_ctor(NULL, NULL);
    if(!strm ->done_list) {
        V4L2LOGE("failed to creat outgoing_queue\n");
        goto error;
    }

    //create buffer here, let decoder or encoder allocate
    //flush(eos) bd
    strm->flush_buf = malloc(sizeof(V4L2Buffer));
    if(!strm->flush_buf){
        V4L2LOGE( "Create flush buffer fail 0x%x", ret);
        goto error;
    }

    if(type == CAPTURE) {
        strm->flush_buf->buf.flags |= V4L2_BUF_FLAG_LAST;
        strm->flush_buf->buf.bytesused = 0;
    }

    return V4L2_SUCCESS;

error:
    if(strm->queued_list) std_list_dtor(strm->queued_list);
    if(strm->done_list) std_list_dtor(strm->done_list);
    if(strm->done_lock) MV_OSAL_Mutex_Destroy(&strm->done_lock);
    return ret;
}
