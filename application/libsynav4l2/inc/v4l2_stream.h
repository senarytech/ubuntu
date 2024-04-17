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

#ifndef SYNA_V4L2_STREAM_H
#define SYNA_V4L2_STREAM_H

#include <linux/videodev2.h>
#include "v4l2_buffer.h"
#include "shm.h"
#include "std_list.h"

typedef enum _stream_type_ {
    OUTPUT = 0,
    CAPTURE = 1,
    StreamTypeMax
} StreamType;


typedef struct V4L2Stream {
    /**
     * which m2m context belong to
     */
    void *m2m_ctx;

    /**
     * stream type.
     */
    StreamType strm_type;

    /**
     * Type of this buffer stream.
     * See V4L2_BUF_TYPE_VIDEO_* in videodev2.h
     * Readonly after init.
     */
    enum v4l2_buf_type buff_type;

    /**
     * Format returned by the driver after initializing the buffer stream.
     * Readonly after init.
     */
    struct v4l2_format format;

    /**
     * Width and height of the frames it produces (in case of a capture stream, e.g. when decoding)
     * or accepts (in case of an output stream, e.g. when encoding).
     */
    int width, height;

    /**
     * Indexed array of V4L2Buffers
     */
    V4L2Buffer *buffers;

    /*
    *flush buffer, with eos bd in ouput and last buf flag in caputre
    */
    V4L2Buffer *flush_buf;
    /**
     * Readonly after init.
     */
    int num_buffers;

    /**
     * Whether the stream has been started (VIDIOC_STREAMON has been sent).
     */
    int streamon;

    /**
     *  Either no more buffers available or an unrecoverable error was notified
     *  by the V4L2 kernel driver: once set the stream has to be exited.
     */
    int done;

    /**
     *  0 -- flush doesn't get done from amp comp.
     *  1 -- flush gets done but not set **LAST** to buffer.
     *  2 -- set **LAST** to buffer but has not sent to client yet.
     *  3 -- sent last buffer to client.
     */
    int last_buff_sent;
    int eos_sent;
    /**
     *  client call QBUFF() to push the buff to incoming_queue.
     *  the buff in incoming_queue is available for m2m consumption.
     *  after consuming, the buff goes to outgoing_queue;
     */
    std_list *queued_list;
    unsigned int  queued_count;
    /**
     *  the buff in incoming_queue is available to return to client.
     *  client call DQBUFF() to return the buff to client.
     */
    std_list *done_list;
    unsigned int done_sequence;
    MV_OSAL_HANDLE_MUTEX_t  done_lock;

    /**
     *  dump the streaming data to file
     */
    FILE *fp;
    /**
     *  dump the port BD to file
     */
    FILE *port_fp;

    unsigned int qbuff_count;
    unsigned int dqbuff_success_count;
    unsigned int dqbuff_fail_count;
} V4L2Stream;

#define STREAM_NAME(type)       ((type)==OUTPUT)?"output":(((type)==CAPTURE)? \
                                    "capture":"unknow")


/**
 define push BD callback for stream
 */
typedef HRESULT (*PushBDFuc)(void *fh, AMP_PORT_IO, AMP_BD_HANDLE);

/**
 * Initializes a V4L2Stream.
 *
 * @param[in] strm A pointer to a V4L2Stream. See V4L2Stream description for required variables.
 * @return 0 in case of success, a negative value representing the error otherwise.
 */
HRESULT v4l2_stream_init(V4L2Stream* strm, StreamType type);
HRESULT v4l2_stream_deinit(V4L2Stream* strm);
HRESULT v4l2_stream_ReqBuff(V4L2Stream* strm,
                    struct v4l2_requestbuffers *reg);

HRESULT v4l2_stream_on(V4L2Stream* strm);
HRESULT v4l2_stream_off(V4L2Stream* strm);

HRESULT v4l2_stream_get_format(V4L2Stream* strm, struct v4l2_format *fmt);
HRESULT v4l2_stream_set_format(V4L2Stream* strm, struct v4l2_format *fmt);

V4L2Stream* v4l2_get_stream(void *ctx, enum v4l2_buf_type type);
void v4l2_stream_enqueue_hw(void *ctx, PushBDFuc pFunc);


#endif  //SYNA_V4L2_STREAM_H
