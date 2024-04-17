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

#ifndef SYNA_V4L2_BUFFER_H
#define SYNA_V4L2_BUFFER_H
#include "shm.h"
#include "isl/amp_buf_desc.h"

enum V4L2Buffer_status {
    V4L2BUF_AVAILABLE,
    V4L2BUF_IN_DRIVER,
    V4L2BUF_RET_USER,
};

/**
 * V4L2Buffer (wrapper for v4l2_buffer management)
 */
typedef struct V4L2Buffer {
    UINT32 uInited;

    /* each buffer needs to have a reference to its stream */
    void *stream;
    int num_planes;

    /* the v4l2_buffer buf.m.planes pointer uses the planes[] mem */
    struct v4l2_buffer buf;
    struct v4l2_plane planes[VIDEO_MAX_PLANES];

    int flags;
    enum V4L2Buffer_status status;

    /* amp shm handle */
    AMP_SHM_HANDLE   hShm;
    int shm_fd;
    UINT32 shm_size;
    UINT32 offset;
    void *va;
    AMP_BD_HANDLE hBD;
} V4L2Buffer;

HRESULT v4l2_buffer_init(V4L2Buffer* buff, int index);
HRESULT v4l2_buffer_deinit(V4L2Buffer* buff);
HRESULT v4l2_buffer_QBuff(void *ctx, struct v4l2_buffer *buff);
HRESULT v4l2_buffer_DQBuff(void *ctx, struct v4l2_buffer *buff);
HRESULT v4l2_buffer_QueryBuff(void *ctx, struct v4l2_buffer *buff);
HRESULT v4l2_buffer_EnqueBD(void *ctx, AMP_BD_HANDLE hBD);
void *v4l2_buffer_mmap(void *addr, size_t length, int prot, int flags,
                       void *handle, off_t offset);
HRESULT v4l2_buffer_munmap(void *handle, void *addr, UINT32 length);
HRESULT v4l2_buffer_expbuf(void *strm, void *eb);

HRESULT createFlushBDBuffer(void *fh);
HRESULT deleteFlushBDBuffer(AMP_BD_HANDLE hBD);

//flush
HRESULT v4l2_buffer_EnqueFlushBuf(void *ctx);
HRESULT v4l2_buffer_queued_clean(void *ctx);

HRESULT v4l2_shm_init(void);
HRESULT v4l2_shm_deinit(void);

AMP_BD_HANDLE v4l2_create_bd(int codec_type, int buff_type, int width, int height, int srcFmt, int shm_size, AMP_SHM_HANDLE *pShm, int strideWidth);

#endif //SYNA_V4L2_BUFFER_H
