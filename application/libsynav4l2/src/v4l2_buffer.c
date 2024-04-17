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

#include <sys/mman.h>
#include <linux/videodev2.h>
#include "v4l2_types.h"
#include "v4l2_buffer.h"
#include "v4l2_m2m.h"
#include "v4l2_stream.h"
#include "v4l2_log.h"
#include "OSAL_api.h"
#include "amp_client.h"
#include "v4l2_shm.h"

#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <poll.h>

#define VDEC_EOS_FLUSHING_SIZE (1024)

#define PAGE_ALIGN(addr)    (((addr) + ((typeof(addr))(4095))) & ~((typeof(addr))(4095)))
/**
 * From kernel drivers/media/v4l2-core/v4l2-mem2mem.c
 * Offset base for buffers on the destination queue - used to distinguish
 * between source and destination buffers when mmapping - they receive the same
 * offsets but for different queues
 */
#define DST_QUEUE_OFF_BASE  (1 << 30)

#define AMP_SHM_DEV "/dev/amp-shm"
static int amp_shm_fd = -1;

/* NOTE: check for m2m and CAPTURE before call this */
static void v4l2_m2m_adjust_mem_offset(struct v4l2_buffer *buf)
{
    if (buf->memory == V4L2_MEMORY_MMAP) {
    /* FIXME: syna v4l2 would use multiple planes format and queue */
        unsigned int i;
        for (i = 0; i < buf->length; ++i) {
            buf->m.planes[i].m.mem_offset
                    += DST_QUEUE_OFF_BASE;
        }
    }
}

static void __setup_offsets(struct V4L2Buffer *vb)
{
    const struct V4L2Stream *stream = vb->stream;
    unsigned int plane;
    unsigned long off = 0;

    if (vb->buf.index) {
        V4L2Buffer *prev = &stream->buffers[vb->buf.index - 1];
        struct v4l2_plane *p = &prev->planes[prev->num_planes - 1];

        off = p->m.mem_offset + p->length;
    }

    for (plane = 0; plane < vb->num_planes; ++plane) {
        vb->planes[plane].m.mem_offset = off;
        off += vb->planes[plane].length;
    }
}

static int __find_plane_by_offset(struct V4L2Stream *stream, unsigned long off,
                                  unsigned int *_buffer, unsigned int *_plane)
{
    struct V4L2Buffer *vb;
    unsigned int buffer, plane;

    for (buffer = 0; buffer < stream->num_buffers; ++buffer) {
        vb = &stream->buffers[buffer];

        for (plane = 0; plane < vb->num_planes; ++plane) {
            if (vb->planes[plane].m.mem_offset == off) {
                *_buffer = buffer;
                *_plane = plane;
                return 0;
            }
        }
    }

    return -EINVAL;
}

//For VDEC, Need allocate FLush Buffer with Special pattern
HRESULT createFlushBDBuffer(void *fh)
{
    HRESULT ret = SUCCESS;
    V4L2M2MContext *m2m_ctx = (V4L2M2MContext *)fh;
    AMP_BDTAG_MEMINFO memInfo;
    AMP_BD_HANDLE hBD;
    AMP_SHM_HANDLE hShm;
    UINT32 i = 0;
    UINT8 *pVirtAddr = NULL;

    ret = AMP_BD_Allocate(&hBD);
    ASSERT_RET_SHOW(ret, "allocate bd failed");

    ret = AMP_BD_Ref(hBD);
    ASSERT_RET_SHOW(ret, "ref bd failed");

    AMP_BDTAG_MEMINFO_INIT(&memInfo);
    memInfo.uFlag |= AMP_MEMINFO_FLAG_EOS_MASK;

    if(m2m_ctx->type == DECODER) {
        /*decoder needs shm but no EOS flag*/
        ret = AMP_SHM_Allocate(AMP_SHM_DYNAMIC, VDEC_EOS_FLUSHING_SIZE,
                1, &hShm);
        ASSERT_RET_SHOW(ret, "shm allocate failed");
        memInfo.uMemHandle = hShm;
    }
    ret = AMP_BDTAG_Append(hBD,
            (UINT8 *)&memInfo, NULL, NULL);
    ASSERT_RET_SHOW(ret, "bd tag append failed");

    m2m_ctx->output.flush_buf->hBD = hBD;
    V4L2LOGD("allocate flush hBD=%p bdid=%d hShm=%d size=%d\n",
              hBD, AMP_BD_GET_BDID(hBD), hShm,
              VDEC_EOS_FLUSHING_SIZE);
    return ret;
}

HRESULT deleteFlushBDBuffer(AMP_BD_HANDLE hBD)
{
    HRESULT ret = V4L2_SUCCESS;
    AMP_BDTAG_MEMINFO *pMemInfo;

    if(hBD == NULL)
        return ret;

    ret = AMP_BDTAG_GetWithIndex(hBD, 0, (void **)&pMemInfo);
    ASSERT_RET_SHOW(ret, "bd tag get meminfo failed");

    if(pMemInfo->uMemHandle) {
        ret = AMP_SHM_Release(pMemInfo->uMemHandle);
        ASSERT_RET_SHOW(ret, "shm free failed");
    }

    ret = AMP_BD_Unref(hBD);
    ASSERT_RET_SHOW(ret, "bd unref failed");

    ret = AMP_BD_Free(hBD);
    ASSERT_RET_SHOW(ret, "bd free failed");
    hBD = NULL;
    return ret;
}

HRESULT ResetFlushBDBuffer(AMP_BD_HANDLE hBD)
{
    HRESULT ret = V4L2_SUCCESS;
    AMP_BDTAG_MEMINFO *pMemInfo;

    if(hBD == NULL)
        return ret;

    ret = AMP_BDTAG_GetWithIndex(hBD, 0, (void **)&pMemInfo);
    ASSERT_RET_SHOW(ret, "bd tag get meminfo failed");

    pMemInfo->uSize = 0;
    pMemInfo->uFlag |= AMP_MEMINFO_FLAG_EOS_MASK;

    return ret;
}


static int vb2_wait_for_done_vb(V4L2Stream *strm, int nonblocking)
{
    int ret;

    for (;;) {

        if (!strm->streamon) {
            V4L2LOGE("streaming off, will not wait for buffers\n");
            return ERROR_EINVAL;
        }
        if (strm->streamon == FALSE) {
            V4L2LOGH("stream is not on, no buffer expected\n");
            return ERROR_EIO;
        }

        if (strm->last_buff_sent == 3) {
            V4L2LOGH("last buffer has been sent out, no more buffer\n");
            return ERROR_EPIPE;
        }

        UINT size = 0;
        std_list_size(strm->done_list, &size);

        if (size) {
             // Found a buffer that we were waiting for.
             break;
        }

        if (nonblocking) {
            V4L2LOGD("[%s]nonblocking and no buffers to dequeue, will not wait\n",
                    STREAM_NAME(strm->strm_type));
            errno = EAGAIN;
            return ERROR_EAGAIN;
        }
    }
    return 0;
}

static int vb2_get_done_vb(V4L2Stream *strm, struct v4l2_buffer *buff,
                  int nonblocking)
{
    int ret = 0;
    V4L2Buffer *buffer = NULL;

    //Wait for at least one buffer to become available on the done_list.
    ret = vb2_wait_for_done_vb(strm, nonblocking);
    if (ret)
        return ret;

    MV_OSAL_Mutex_Lock(strm->done_lock);
    std_list_pop_front(strm->done_list, (void **)&buffer);
    MV_OSAL_Mutex_Unlock(strm->done_lock);

    memcpy(buff, &buffer->buf, sizeof(struct v4l2_buffer));

    return ret;
}

static V4L2Buffer* bd_to_v4l2buff(V4L2Stream *strm, AMP_BD_HANDLE hBD,
                            UINT32 *index)
{
    UINT32 i = 0;

    //! flush buffer
    if(strm->flush_buf->hBD == hBD) {
        *index = 0xF;
        return strm->flush_buf;
    }

    for(i=0; i<strm->num_buffers; i++) {
        if(strm->buffers[i].hBD == hBD) {
            *index = i;
            return &(strm->buffers[i]);
        }
    }
    return NULL;
}

static HRESULT update_StrideInfo(V4L2Buffer* buff, struct v4l2_buffer *in_buffer)
{
    HRESULT ret = V4L2_SUCCESS;
    struct v4l2_buffer *local_buffer = &buff->buf;
    AMP_BGTAG_FRAME_INFO *pInfo;

    ret = BD_GetFrameInfo(buff->hBD, &pInfo);
    if (pInfo->uiBufStride != in_buffer->m.planes[0].reserved[0]) {
        pInfo->uiBufStride  = in_buffer->m.planes[0].reserved[0];
        /*TODO: Support for format other than 420SP*/
        buff->buf.m.planes[0].length = (pInfo->uiBufStride * pInfo->uiContentH);
        buff->buf.m.planes[1].length = (pInfo->uiBufStride * pInfo->uiContentH / 2);
    }
    return ret;
}

static HRESULT update_BDMemInfo(V4L2Buffer* buff, struct v4l2_buffer *in_buffer)
{
    HRESULT ret = V4L2_SUCCESS;
    struct v4l2_buffer *local_buffer = &buff->buf;
    AMP_BDTAG_MEMINFO *pMemInfo;
    UINT32 i = 0;
    UINT32 size = 0;
    UINT8 *ptr = (UINT8 *)buff->va;

    ret = AMP_BDTAG_GetWithIndex(buff->hBD, 0, (void **)&pMemInfo);
    ASSERT_RET_SHOW(ret, "AMP_BDTAG_GetWithIndex() error");

    /**
     *  if it is V4L2_MEMORY_USERPTR, it is a userspace pointer to the memory
     *  allocated for this plane by an application.
     *
     *  driver need to copy the data to shm.
     */
    for(i=0; i<in_buffer->length; i++) {
        if(in_buffer->memory == V4L2_MEMORY_USERPTR) {
            memcpy((void *)(ptr + size),
                    (void *)(in_buffer->m.planes[i].m.userptr),
                    in_buffer->m.planes[i].bytesused);
        }
        size += in_buffer->m.planes[i].bytesused;
    }

    pMemInfo->uSize = size;
    ret = AMP_BDTAG_Update(buff->hBD, 0, (void *)pMemInfo);
    ASSERT_RET_SHOW(ret, "AMP_BDTAG_Update() error");

    return ret;
}

HRESULT v4l2_shm_init(void)
{
    HRESULT ret = V4L2_SUCCESS;

    if(amp_shm_fd > 0) {
        V4L2LOGD("inited shm fd =%d\n", amp_shm_fd);
        return ret;
    }
    amp_shm_fd = open(AMP_SHM_DEV, O_RDWR, 0);
    if(amp_shm_fd < 0) {
        ret = ERROR_NOSWRSC;
        V4L2LOGE("open amp shm failed\n");
    }
    return ret;
}

HRESULT v4l2_shm_deinit(void)
{
    HRESULT ret = V4L2_SUCCESS;

    ret = close(amp_shm_fd);
    ASSERT_RET_SHOW(ret, "error!! close shm fd failed");
    amp_shm_fd = -1;

    return ret;
}

int v4l2_shm_getFD(UINT32 gid)
{
    int ret=0;
    struct shm_fd_data p;
    p.gid = gid;
    ret = ioctl(amp_shm_fd, SHM_IOC_GET_FD, &p);
    return p.fd;
}

int v4l2_shm_put(UINT32 gid)
{
    int ret=0;
    struct shm_fd_data p;
    p.gid = gid;
    ret = ioctl(amp_shm_fd, SHM_IOC_PUT, &p);
    ASSERT_RET_SHOW(ret, "error!! put fd failed");

    return ret;
}

static double GetBitFactor(UINT32 ispBitDepth) {
    double bitsFrac = 1.0;

    switch (ispBitDepth) {
    case 8:
        bitsFrac = 1.0;
        break;
    case 10:
        bitsFrac = 1.25;
        break;
    case 12:
        bitsFrac = 1.5;
        break;
    default:
        bitsFrac = 1.0;
    }

    return bitsFrac;
}

double GetSizeFactor(UINT32 ispFmt, UINT32 bitDepth) {
    double bitFactor = GetBitFactor(bitDepth);

    double sizeFactor = 4;
    switch (ispFmt) {
    case AMP_DISP_SRCFMT_YUV420SP:
        sizeFactor = 1.5;
        break;
    case AMP_DISP_SRCFMT_YUV420SP_TILED_V8H8:
        sizeFactor = 1.5;
        break;
    default:
        sizeFactor = 4.0;
        break;
    }
    return sizeFactor * bitFactor;
}

double GetStrideFactor(UINT32 ispFmt, UINT32 ispBitDepth) {
    double bitFactor = GetBitFactor(ispBitDepth);

    double strideFactor = 1.0;
    switch (ispFmt) {
    case AMP_DISP_SRCFMT_YUV420SP:
        strideFactor = 1.0;
        break;
    case AMP_DISP_SRCFMT_YUV420SP_TILED_V8H8:
        strideFactor = 1.0;
        break;
    default:
        strideFactor = 4.0;
        break;
    }
    return strideFactor * bitFactor;
}

UINT32 fb_buf_alloc(int uiWidth, int uiHeight, int wstride, int size, AMP_SHM_HANDLE *pShm)
{
    HRESULT ret = SUCCESS;
    UINT32 sizeY = 0;

    sizeY = align(wstride * uiHeight, 4096);

    AMP_SHM_TYPE eMemType = AMP_SHM_VIDEO_FB;
    AMP_BDTAG_MEMINFO memInfo;
    AMP_SHM_CLIENTS stWrites;
    AMP_SHM_CLIENTS stReaders;
    AMP_SHM_CONFIG  stShmConfig;

    memset(&stWrites, 0x0, sizeof(AMP_SHM_CLIENTS));
    memset(&stReaders, 0x0, sizeof(AMP_SHM_CLIENTS));
    memset(&stShmConfig, 0x0, sizeof(AMP_SHM_CONFIG));

    stWrites.uiAmpClients = AMP_SHM_INTERNAL_CLIENT_VDEC;
    stReaders.uiAmpClients = AMP_SHM_INTERNAL_CLIENT_DISPLAY;
    stShmConfig.eBufType = AMP_SHM_SCATTER_UV_ALIGN;
    stShmConfig.uiSizeY = sizeY;

    ret = AMP_SHM_V1_0_Allocator(1, AMP_CONTENT_TYPE_VIDEO_FRAME, eMemType,
                                    stWrites, stReaders, stShmConfig, size,
                                    pShm);
    if(ret != 0) {
        V4L2LOGE("fb buffer allocate faild 0x%x", ret);
    }
    return ret;
}

AMP_BD_HANDLE v4l2_create_bd(int codec_type, int buff_type, int width, int height, int srcFmt, int shm_size, AMP_SHM_HANDLE *pShm, int strideWidth) {
    HRESULT ret = V4L2_SUCCESS;
    AMP_BD_HANDLE hBD = NULL;
    AMP_BDTAG_MEMINFO memInfo;
    AMP_BDTAG_STREAM_INFO streamInfo;

    //! shm allocation.
#if 1 //VS680, VS640 and later
    if((codec_type == DECODER) && (!V4L2_TYPE_IS_OUTPUT(buff_type))) {
        //decoder capture buffer (frame buffer) use iommu
        ret = fb_buf_alloc(width, height, strideWidth, shm_size, pShm);

    } else {
        ret = AMP_SHM_Allocate(AMP_SHM_DYNAMIC, shm_size, 1024, pShm);
    }
#else
    //bg5ct and legacy chip
    ret = AMP_SHM_Allocate(AMP_SHM_DYNAMIC, shm_size, 1024, pShm);
#endif

    if(ret != V4L2_SUCCESS) {
        goto error;
    }

    ret = AMP_BD_Allocate(&hBD);
    if(ret != V4L2_SUCCESS) {
        goto error;
    }
    ret = AMP_BD_Ref(hBD);
    if(ret != V4L2_SUCCESS) {
        goto error;
    }

    memInfo.Header.eType = AMP_BDTAG_ASSOCIATE_MEM_INFO;
    memInfo.Header.uLength = sizeof (AMP_BDTAG_MEMINFO);
    memInfo.uMemHandle = *pShm;
    memInfo.uMemOffset = 0;
    memInfo.uSize = shm_size;
    memInfo.uFlag = 0;
    ret = AMP_BDTAG_Append(hBD, (UINT8 *)&memInfo, NULL, NULL);
    if(ret != V4L2_SUCCESS) {
        goto error;
    }

    /*only encoder capture stream need streaminfo tag*/
    if(!V4L2_TYPE_IS_OUTPUT(buff_type)) {
        if(codec_type == ENCODER) {
            memset(&streamInfo, 0, sizeof(AMP_BDTAG_STREAM_INFO));
            streamInfo.Header.eType = AMP_BDTAG_STREAM_INFO_META;
            streamInfo.Header.uLength = sizeof(AMP_BDTAG_STREAM_INFO);

            ret = AMP_BDTAG_Append(hBD, (UINT8 *)&streamInfo, NULL, NULL);
            if(ret != V4L2_SUCCESS) {
                goto error;
            }
        }
    }

    if(codec_type == CAMERA){
        AMP_BGTAG_FRAME_INFO frameInfo;

        void *phyAddr = 0;
        AMP_SHM_GetPhysicalAddress(*pShm, 0, (void**) &phyAddr);

        double sizeFact = GetSizeFactor(srcFmt, 8);
        double strideFact = GetStrideFactor(srcFmt, 8);
        unsigned int stride = strideWidth * strideFact; //Stride aligned to 128 bytes

        AMP_BDTAG_FRAMEINFO_INIT(&frameInfo);
        frameInfo.hShm = *pShm;
        frameInfo.iContentOff = 0;
        frameInfo.uiBufSize = shm_size;
        frameInfo.uiSrcFmt = srcFmt;
        frameInfo.uiContentH = height;
        frameInfo.uiContentW = width;
        frameInfo.pBufStart = (UINT32) (uintptr_t) phyAddr;
        frameInfo.uiPbufStartUV = (UINT32)(uintptr_t) phyAddr + (stride * height);
        frameInfo.uiBufStride = stride;
        frameInfo.uiBufStrideUV = stride;
        frameInfo.uiBitsPerPixel = strideFact * 8;
        frameInfo.uiActiveW = width;
        frameInfo.uiActiveH = height;
        frameInfo.iActiveX = 0;
        frameInfo.iActiveY = 0;
        frameInfo.uiFrNum = 60000;
        frameInfo.uiFrDen = 1001;
        frameInfo.uStatus = AMP_FRAME_ST_DECODED;
        frameInfo.uiIsFs = 1;

        ret = AMP_BDTAG_Append(hBD, (UINT8*) &frameInfo, NULL, NULL);
        if (ret != SUCCESS) {
            goto error;
        }
    }

    V4L2LOGD("[BUFFER] memInfo[hShm:0x%x][offset:0x%x][size:0x%x]\n",
                memInfo.uMemHandle, memInfo.uMemOffset, memInfo.uSize);

    return hBD;

error:
    if(*pShm) AMP_SHM_Release(*pShm);
    if(hBD) {
        AMP_BD_Unref(hBD);
        AMP_BD_Free(hBD);
    }
    return NULL;
}

HRESULT v4l2_buffer_init(V4L2Buffer* buff, int index)
{
    HRESULT ret = V4L2_SUCCESS;
    int i;
    UINT32 shm_size = 0;
    UINT8 *va = NULL;
    UINT32 offset = 0;
    struct v4l2_buffer *buffer = &buff->buf;
    V4L2Stream* strm = (V4L2Stream* )buff->stream;
    struct v4l2_plane_pix_format *plane_fmt = NULL;

    buffer->index = index;
    buffer->bytesused = 0;
    buffer->flags = 0;
    buffer->field = V4L2_FIELD_NONE;

    if (V4L2_TYPE_IS_MULTIPLANAR(strm->buff_type)) {
        struct v4l2_pix_format_mplane *pix_mp = &strm->format.fmt.pix_mp;
        buffer->m.planes = buff->planes;
        buff->num_planes = pix_mp->num_planes;
        /* in MP, the V4L2 API states that buf.length means num_planes */
        buffer->length = buff->num_planes;
    }
    else {
        ret = ERROR_TYPENOTSUPPORT;
        V4L2LOGE("signle plane, not support...\n");
        goto error;
    }

    V4L2LOGD("[BUFFER][%s][index:%d][num_planes:%d]\n",
                            STREAM_NAME(strm->strm_type),
                            buffer->index, buff->num_planes);

    if(buff->num_planes > 2) {
        V4L2LOGE("[BUFFER] error!! not support more than 2 planes\n");
        ret = ERROR_TYPENOTSUPPORT;
        goto error;
    }
    for (i = 0; i < buff->num_planes; i++) {
        plane_fmt = &strm->format.fmt.pix_mp.plane_fmt[i];
        shm_size += plane_fmt->sizeimage;
        buffer->m.planes[i].length = plane_fmt->sizeimage;

        V4L2LOGD("[BUFFER][%s][index:%d][sizeimage:%d][bytesperline:%d]\n",
                            STREAM_NAME(strm->strm_type), buffer->index,
                            plane_fmt->sizeimage, plane_fmt->bytesperline);
    }

    V4L2LOGD("[BUFFER][%s] [index:%d] [num_planes:%d][shm_size:0x%x]\n",
                            STREAM_NAME(strm->strm_type), buffer->index,
                            buff->num_planes, shm_size);

    buff->shm_size = shm_size;
    buff->status = V4L2BUF_AVAILABLE;

    V4L2M2MContext *m2m_ctx = (V4L2M2MContext *)strm->m2m_ctx;
    int srcFmt = AMP_DISP_SRCFMT_YUV420SP;
    int width = strm->format.fmt.pix_mp.width;
    int stride = width;
    if (buff->num_planes > 0) {
        plane_fmt = &strm->format.fmt.pix_mp.plane_fmt[0];
        stride = plane_fmt->bytesperline;
    }
    int height = strm->format.fmt.pix_mp.height;

    buff->hBD = v4l2_create_bd(m2m_ctx->type, strm->buff_type, width, height,
                               srcFmt, shm_size, &buff->hShm, stride);
    if(buff->hBD == NULL){
        goto error;
    }

    ret = AMP_SHM_GetVirtualAddress(buff->hShm, 0, (void **)&va);
    if(ret != V4L2_SUCCESS) {
        goto error;
    }

    buff->va = va;
    buff->shm_size = shm_size;
    if (buffer->memory == V4L2_MEMORY_MMAP) {
        __setup_offsets(buff);
    } else if (buffer->memory == V4L2_MEMORY_USERPTR) {
        for (i = 0; i < buff->num_planes; i++) {
            buffer->m.planes[i].m.userptr = (unsigned long)va;
            va += buffer->m.planes[i].length;
        }
    } else {
        V4L2LOGE("Error!! Unsupported memory type\n");
    }

    buff->offset = buff->planes[0].m.mem_offset;
    UINT32 refCnt;
    BOOL valid;
    AMP_BD_GetStatus(buff->hBD, &valid, &refCnt);
    V4L2LOGD("[BUFFER][hShm:%d][hBD:%p %d][VA:%p][offset:0x%x] refcount=%d Allocate\n",
             buff->hShm, buff->hBD, AMP_BD_GET_BDID(buff->hBD), buff->va,
             buff->offset, refCnt);

    DUMP_BUFF_CNXT(buffer);

    buff->uInited = TRUE;
    return V4L2_SUCCESS;
error:
    if(buff->hShm) AMP_SHM_Release(buff->hShm);
    if(buff->hBD) AMP_BD_Free(buff->hBD);
    return ret;
}

HRESULT v4l2_buffer_deinit(V4L2Buffer* buff)
{
    HRESULT ret = V4L2_SUCCESS;
    if(buff->uInited) {
        UINT32 refCnt;
        BOOL valid;
        AMP_BD_GetStatus(buff->hBD, &valid, &refCnt);
        V4L2LOGD("[BUFFER][hShm:%d][hBD:%p][VA:%p][offset:0x%x] refcount=%d Free\n",
                    buff->hShm, buff->hBD, buff->va, buff->offset, refCnt);

        if(buff->hShm) AMP_SHM_Release(buff->hShm);
        if(buff->hBD) {
            AMP_BD_Unref(buff->hBD);
            AMP_BD_Free(buff->hBD);
        }
        buff->uInited = FALSE;
    }

    return ret;
}

HRESULT v4l2_buffer_QBuff(void *ctx, struct v4l2_buffer *buff)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2M2MContext *pCtx = (V4L2M2MContext *)ctx;
    V4L2Stream *strm = NULL;
    V4L2Buffer *buffer = NULL;
    UINT32 i = 0;

    if(!V4L2_TYPE_IS_MULTIPLANAR(buff->type)) {
        ret = ERROR_EINVAL;
        V4L2LOGE("error!! buffer type wrong");
        return ret;
    }
    //! 1. identify which stream the buffer belong to;
    strm = v4l2_get_stream(ctx, buff->type);
    if(!strm) {
        ret = ERROR_EINVAL;
        V4L2LOGE("mapping to stream failed(buff->type=0x%x)\n", buff->type);
        goto error;
    }

    strm->qbuff_count ++;

    //! mapping out V4L2Buffer
    if(buff->index < strm->num_buffers) {
        buffer = &strm->buffers[buff->index];
    }
    else {
        ret = ERROR_EINVAL;
        V4L2LOGE("error: buff->index(%d) >= strm->num_buffers(%d)\n",
                    buff->index, strm->num_buffers);
        goto error;
    }

    //! change buffer flag;
    buff->flags |= V4L2_BUF_FLAG_MAPPED | V4L2_BUF_FLAG_QUEUED;
    buff->flags &= ~V4L2_BUF_FLAG_DONE;
    if(strm->strm_type == CAPTURE && strm->last_buff_sent == 1 ) {
        V4L2LOGD("set V4L2_BUF_FLAG_LAST to the last left buffer\n");
        buffer->buf.flags |= V4L2_BUF_FLAG_LAST;
        for(i=0; i<buffer->buf.length; i++) {
            buffer->buf.m.planes[i].bytesused = 0;
        }
        strm->last_buff_sent = 2;
    }

    //! update buffer
    if(buff->type == V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE) {
        update_BDMemInfo(buffer, buff);
    }
    if(buff->type == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE && pCtx->type == CAMERA) {
        /* Update stride details from gralloc buffer to v4l2buffer.
           Exploitting reserved field in v4l2_plane for passing stride info */
        if (buff->m.planes[0].reserved[0] != 0 &&
                buff->m.planes[1].reserved[0] != 0) {
            update_StrideInfo(buffer, buff);
        }
    }
    #if 0
    //! 4. push buff to incoming queue.
    if((pCtx->codec.state == STATE_DRAIN) &&
            V4L2_TYPE_IS_OUTPUT(buff->type)) {
        //V4L2_DECODE_CMD_STOP, return input directly
        std_list_push_back(strm->done_list, (void *)buffer);
        return V4L2_SUCCESS;
    }
    #endif
    std_list_push_back(strm->queued_list, (void *)buffer);
    strm->queued_count++;
    V4L2_DATAFLOW_TRACE("[%s][Buff:%d] Q to queued_list[Buffer Queued:%d]\n",
                STREAM_NAME(strm->strm_type), buff->index, strm->queued_count);
    DUMP_BUFF_CNXT(buff);

    if(V4L2_TYPE_IS_OUTPUT(buff->type))
        dump_StreamBuff((void *)strm, buff);

    return V4L2_SUCCESS;
error:
    return ret;
}

HRESULT v4l2_buffer_DQBuff(void *ctx, struct v4l2_buffer *buff)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2Stream *strm = NULL;
    UINT32 size = 0;
    V4L2Buffer *buffer = NULL;

    int nonblocking = 1;

    //! 1. identify which stream the buffer belong to;
    strm = v4l2_get_stream(ctx, buff->type);
    if(!strm) {
        ret = ERROR_EINVAL;
        V4L2LOGE("mapping to stream failed(buff->type=%d)\n", buff->type);
        goto error;
    }

    ret = vb2_get_done_vb(strm, buff, nonblocking);
    if(ret)
        goto error;
    //TODO: buffer status change to dequed ?
    buff->flags |= V4L2_BUF_FLAG_DONE;
    buff->flags &= ~V4L2_BUF_FLAG_QUEUED;
    buff->flags &= ~V4L2_BUF_FLAG_MAPPED;

    strm->queued_count--;
    if((strm->queued_count == 0) && (strm->last_buff_sent == 1)){
        buff->flags |= V4L2_BUF_FLAG_LAST;
        V4L2LOGH("tag Last buffer\n");
    }
    if(buff->flags & V4L2_BUF_FLAG_LAST) {
        strm->last_buff_sent = 3;
    }
    V4L2_DATAFLOW_TRACE("[%s][Buff:%d][isLast:%s] DQ from done_list [Buffer Queued:%d]\n",
                STREAM_NAME(strm->strm_type),buff->index,
                buff->flags&V4L2_BUF_FLAG_LAST?"true":"false",
                strm->queued_count);
    DUMP_BUFF_CNXT(buff);

    if(!V4L2_TYPE_IS_OUTPUT(buff->type)) {
        if(strm->last_buff_sent != 3)
            dump_StreamBuff((void *)strm, buff);
    }
    strm->dqbuff_success_count ++;
    return V4L2_SUCCESS;
error:
    strm->dqbuff_fail_count ++;
    return ret;
}

HRESULT v4l2_buffer_QueryBuff(void *ctx, struct v4l2_buffer *buff)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2Stream *strm = NULL;
    V4L2Buffer *buffer = NULL;

    if (V4L2_TYPE_IS_MULTIPLANAR(buff->type)) {
        if (!buff->m.planes) {
            V4L2LOGE("missing multiple planes storage target\n");
            ret = ERROR_EINVAL;
            goto error;
        }
    }

    //! 1. identify which stream the buffer belong to;
    strm = v4l2_get_stream(ctx, buff->type);
    if(!strm) {
        ret = ERROR_EINVAL;
        V4L2LOGE("mapping to stream failed(buff->type=%d)\n", buff->type);
        goto error;
    }

    //! 2. mapping out V4L2Buffer
    if(buff->index < strm->num_buffers) {
        buffer = &strm->buffers[buff->index];
    } else {
        ret = ERROR_EINVAL;
        V4L2LOGE("error: buff->index(%d) >= strm->num_buffers(%d)\n",
                    buff->index, strm->num_buffers);
        goto error;
    }

    memcpy(buff->m.planes, buffer->buf.m.planes,
           buffer->buf.length * sizeof(struct v4l2_plane));

    buff->length = buffer->buf.length;
    if (strm->strm_type == CAPTURE)
        v4l2_m2m_adjust_mem_offset(buff);

    DUMP_BUFF_CNXT(buff);

    return V4L2_SUCCESS;
error:
    return ret;
}

//move the queued list items to done list
HRESULT v4l2_buffer_queued_clean(void *ctx)
{
    V4L2Stream *strm = (V4L2Stream *)ctx;
    V4L2Buffer *buffer;
    UINT32 i = 0;

    MV_OSAL_Mutex_Lock(strm->done_lock);
    do {
        std_list_pop_front(strm->queued_list, (void **)&buffer);
        if(buffer) {
            if(strm->strm_type == CAPTURE && strm->last_buff_sent == 1 ) {
                V4L2LOGD("set V4L2_BUF_FLAG_LAST to the last left buffer\n");
                buffer->buf.flags |= V4L2_BUF_FLAG_LAST;
                for(i=0; i<buffer->buf.length; i++) {
                    buffer->buf.m.planes[i].bytesused = 0;
                }
                strm->last_buff_sent = 2;
            }
            std_list_push_back(strm->done_list, (void *)buffer);
        }
    }while (buffer);

    MV_OSAL_Mutex_Unlock(strm->done_lock);

    return V4L2_SUCCESS;
}

HRESULT v4l2_buffer_EnqueBD(void *ctx, AMP_BD_HANDLE hBD)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2Stream *strm=(V4L2Stream*)ctx;
    V4L2M2MContext *m2m_ctx = (V4L2M2MContext *)strm->m2m_ctx;
    V4L2Buffer *buffer =  NULL;
    UINT32 index = 0;
    AMP_BDTAG_MEMINFO *pMemInfo;

    buffer = bd_to_v4l2buff(strm, hBD, &index);
    if(!buffer) {
        /*handle encoder capture stream eos case*/
        if(m2m_ctx->type == ENCODER && strm->strm_type == CAPTURE) {
            if(strm->queued_count > 0) {
                //return buffer queued after flush
                V4L2LOGD("return buffer queued after flush\n");
                v4l2_buffer_queued_clean(strm);
            }
            V4L2_DATAFLOW_TRACE("[%s][Buff:%x] [Flush done] [Buffer Queued:%d]\n",
                        STREAM_NAME(strm->strm_type), index, strm->queued_count);
        }
        return V4L2_SUCCESS;
    }

    //! recycle ouput quequed flush buffer
    if(buffer == strm->flush_buf) {
        strm->queued_count--;
        if(strm->queued_count > 0) {
            //return buffer queued after flush
            V4L2LOGD("return buffer queued after flush\n");
            v4l2_buffer_queued_clean(strm);
        }
        //reset the flush BD for reuse.
        ResetFlushBDBuffer(buffer->hBD);
        V4L2_DATAFLOW_TRACE("[%s][Buff:%x] [Flush done] [Buffer Queued:%d]\n",
                    STREAM_NAME(strm->strm_type), index, strm->queued_count);
    }
    else {
        if(strm->streamon == TRUE) {
            if(strm->strm_type == CAPTURE) {
                //! update byteused in buffer for capture stream
                ret = AMP_BDTAG_GetWithIndex(hBD, 0, (void **)&pMemInfo);
                ASSERT_RET_SHOW(ret, "AMP_BDTAG_GetWithIndex() error");
                buffer->buf.m.planes[0].bytesused = pMemInfo->uSize;
            }

            MV_OSAL_Mutex_Lock(strm->done_lock);
            std_list_push_back(strm->done_list, buffer);
            MV_OSAL_Mutex_Unlock(strm->done_lock);
            if(strm->strm_type == CAPTURE && (m2m_ctx->type == DECODER || m2m_ctx->type == CAMERA)) {
                if(!is_validDispBD(hBD)) {
                    buffer->buf.m.planes[0].bytesused = 0;
                } else {
                    buffer->buf.m.planes[0].bytesused = pMemInfo->uSize;
                    strm->done_sequence ++;
                }
                buffer->buf.timestamp.tv_sec = strm->done_sequence;
            }
            V4L2_DATAFLOW_TRACE("[%s][Buff:%d] EnqueBD to done_list [Buffer Queued:%d],[done %d]\n",
                    STREAM_NAME(strm->strm_type), index, strm->queued_count, strm->done_sequence);

            /*
             *  if Poll() is called, need to notify client when there is new data,
             *  or data has been consumed.
             *  so that client can call QBUFF() or DQBUFF().
             */
            if(strm->strm_type == CAPTURE){
                m2m_ctx->recieve_event |= POLLIN; //fill buffer done, there is data to read
            } else {
                m2m_ctx->recieve_event |= POLLOUT; //empty buffer done, writing not block
            }
            PUT_POLL_SEM((V4L2_HANDLE)strm->m2m_ctx);
        }
    }

    return V4L2_SUCCESS;
}

HRESULT v4l2_buffer_EnqueFlushBuf(void *ctx)
{
    V4L2Stream *strm = (V4L2Stream *)ctx;
    V4L2Buffer *buffer = strm->flush_buf;
    if(strm->strm_type == CAPTURE) {
        //do nothing
    } else {
        //send eos buf to queued list, flush buffer
        std_list_push_back(strm->queued_list, buffer);
        strm->queued_count ++;
        V4L2_DATAFLOW_TRACE("[%s][Buff:F] EnqueFlushBuf to queued_list [Buffer Queued:%d]\n",
                    STREAM_NAME(strm->strm_type), strm->queued_count);
    }

    return V4L2_SUCCESS;
}

HRESULT v4l2_buffer_getSharedFD(void *handle, UINT32 offset, int buf_type,
                                int *shared_fd)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2M2MContext *ctx = (V4L2M2MContext *)handle;
    V4L2Buffer *buffer = NULL;
    V4L2Stream *strm = NULL;
    UINT32 i, j;

    if (offset < DST_QUEUE_OFF_BASE) {
        strm = &ctx->output;
    } else {
        strm = &ctx->capture;
        offset -= DST_QUEUE_OFF_BASE;
    }

    if (__find_plane_by_offset(strm, offset, &i, &j))
        return ERROR_ERRPARAM;

    buffer = &strm->buffers[i];
    if (!buffer->shm_fd)
        AMP_SHM_Share(buffer->hShm, &buffer->shm_fd);

    if (buffer->shm_fd) {
        *shared_fd = buffer->shm_fd;
        V4L2LOGH("SYNA_DBUG: AMP_SHM_HANDLE(%x) shm_fd(%d) num_planes(%d)\n",
                 buffer->hShm, buffer->shm_fd, buffer->num_planes);
    } else {
        *shared_fd = -1;
        V4L2LOGE("error! buffer->shm_fd(%d)\n", buffer->shm_fd);
        ret = ERROR_ERRPARAM;
    }

    return ret;
}

void *v4l2_buffer_mmap(void *addr, size_t length, int prot, int flags,
                       void *handle, off_t offset)
{
    V4L2M2MContext *ctx = (V4L2M2MContext *)handle;
    V4L2Buffer *buffer =  NULL;
    V4L2Stream *strm = &ctx->output;
    UINT32 i, j;

    ctx->Mmap_api_count ++;
    V4L2LOGD("input [addr:%p][length:0x%x][prot:0x%x][flags:0x%x][offset:0x%x]\n",
             addr, length, prot, flags, offset);

    if (offset < DST_QUEUE_OFF_BASE) {
        strm = &ctx->output;
    } else {
        strm = &ctx->capture;
        offset -= DST_QUEUE_OFF_BASE;
    }

    if (__find_plane_by_offset(strm, offset, &i, &j))
        return MAP_FAILED;

    buffer = &strm->buffers[i];

    return (void *)((uintptr_t)buffer->va) + (buffer->planes[j].m.mem_offset
                                              - buffer->offset);
}

HRESULT v4l2_buffer_munmap(void *handle, void *addr, UINT32 length)
{
    V4L2M2MContext *ctx = (V4L2M2MContext *)handle;
    //! nothing to handle
    ctx->Munmap_api_count ++;

    return V4L2_SUCCESS;
}

HRESULT v4l2_buffer_expbuf(void *ctx, void *eb)
{
    HRESULT ret = V4L2_SUCCESS;
    V4L2Buffer *buffer;
    UINT32 idx = -1;
    int fd = -1;

    struct v4l2_exportbuffer *p = (struct v4l2_exportbuffer *)eb;
    V4L2Stream *strm = v4l2_get_stream(ctx, p->type);
    idx = p->index;

    if(idx >= strm->num_buffers) {
        V4L2LOGE("error!! expbuf wrong idx\n");
        errno = EINVAL;
        return -1;
    }

    V4L2LOGD("expbuf: %s, buf %d, idx %d, flag 0x%x\n",
        buf2str(p->type), p->index, p->plane, p->flags);

    buffer = &strm->buffers[idx];
    if(!buffer->hShm) {
        V4L2LOGE("error!! no shm for this idx %d\n", idx);
        errno = EINVAL;
        return -1;
    }

    if(p->plane == 0) {
        p->fd = buffer->shm_fd = v4l2_shm_getFD(buffer->hShm);
        if(p->fd < 0) {
            V4L2LOGE("error!! get shm fd failed\n");
            return ERROR_EBADR;
        }
        v4l2_shm_put(buffer->hShm);
    }else {
        p->fd = dup(buffer->shm_fd);
    }

    V4L2LOGD("fd = %d\n", p->fd);
    return V4L2_SUCCESS;
}
