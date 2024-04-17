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
#include<stdio.h>
#include "linux/videodev2.h"

#include "v4l2_types.h"
#include "v4l2_misc.h"
#include "v4l2_ioctrl.h"
#include "v4l2_m2m.h"

#include "OSAL_api.h"
#include "amp_client.h"
#include "amp_client_support.h"

char* fmt2str(UINT32 fmt)
{
    switch (fmt) {
        case V4L2_PIX_FMT_H264:
            return "V4L2_PIX_FMT_H264";
        case V4L2_PIX_FMT_VP8:
            return "V4L2_PIX_FMT_VP8";
        case V4L2_PIX_FMT_VP9:
            return "V4L2_PIX_FMT_VP9";
        case V4L2_PIX_FMT_NV12:
            return "V4L2_PIX_FMT_NV12";
        case V4L2_PIX_FMT_UYVY:
            return "V4L2_PIX_FMT_UYVY";
        case V4L2_PIX_FMT_YUV420:
            return "V4L2_PIX_FMT_YUV420";
        case V4L2_PIX_FMT_YVU420:
            return "V4L2_PIX_FMT_YVU420";
        default:
            V4L2LOGH("fmt=0x%x\n", fmt);
            return "Unknown pixelformat";
    }
}

char* buf2str(UINT32 type)
{
    switch (type) {
        case V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE:
            return "CAPTURE";
        case V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE:
            return "OUTPUT";
        default:
            V4L2LOGH("buftype=%d\n", type);
            return "Unknown pixelformat";
    }
}

char* event2str(UINT32 type)
{
    switch(type) {
        case V4L2_EVENT_SOURCE_CHANGE:
            return "V4L2_EVENT_SOURCE_CHANGE";
        case V4L2_EVENT_EOS:
            return "V4L2_EVENT_EOS";
        default:
            return "unknown event";
    }
}

char* cmd2str(UINT32 cmd)
{
    switch(cmd) {
        case VIDIOC_QUERYCAP:
            return "VIDIOC_QUERYCAP";
        case VIDIOC_ENUM_FMT:
            return "VIDIOC_ENUM_FMT";
        case VIDIOC_G_FMT:
            return "VIDIOC_G_FMT";
        case VIDIOC_S_FMT:
            return "VIDIOC_S_FMT";
        case VIDIOC_REQBUFS:
            return "VIDIOC_REQBUFS";
        case VIDIOC_QUERYBUF:
            return "VIDIOC_QUERYBUF";
        case VIDIOC_EXPBUF:
            return "VIDIOC_EXPBUF";
        case VIDIOC_QBUF:
            return "VIDIOC_QBUF";
        case VIDIOC_DQBUF:
            return "VIDIOC_DQBUF";
        case VIDIOC_STREAMON:
            return "VIDIOC_STREAMON";
        case VIDIOC_STREAMOFF:
            return "VIDIOC_STREAMOFF";
        case VIDIOC_QUERYCTRL:
            return "VIDIOC_QUERYCTRL";
        case VIDIOC_G_CTRL:
            return "VIDIOC_G_CTRL";
        case VIDIOC_S_CTRL:
            return "VIDIOC_S_CTRL";
        case VIDIOC_G_EXT_CTRLS:
            return "VIDIOC_G_EXT_CTRLS";
        case VIDIOC_S_EXT_CTRLS:
            return "VIDIOC_S_EXT_CTRLS";
        case VIDIOC_TRY_EXT_CTRLS:
            return "VIDIOC_TRY_EXT_CTRLS";
        case VIDIOC_QUERYMENU:
            return "VIDIOC_QUERYMENU";
        case VIDIOC_CROPCAP:
            return "VIDIOC_CROPCAP";
        case VIDIOC_G_CROP:
            return "VIDIOC_G_CROP";
        case VIDIOC_S_CROP:
            return "VIDIOC_S_CROP";
        case VIDIOC_G_SELECTION:
            return "VIDIOC_G_SELECTION";
        case VIDIOC_S_SELECTION:
            return "VIDIOC_S_SELECTION";
        case VIDIOC_ENCODER_CMD:
            return "VIDIOC_ENCODER_CMD";
        case VIDIOC_TRY_ENCODER_CMD:
            return "VIDIOC_TRY_ENCODER_CMD";
        case VIDIOC_DECODER_CMD:
            return "VIDIOC_DECODER_CMD";
        case VIDIOC_TRY_DECODER_CMD:
            return "VIDIOC_TRY_DECODER_CMD";
        case VIDIOC_G_PARM:
            return "VIDIOC_G_PARM";
        case VIDIOC_S_PARM:
            return "VIDIOC_S_PARM";
        case VIDIOC_ENUM_FRAMESIZES:
            return "VIDIOC_ENUM_FRAMESIZES";
        case VIDIOC_ENUM_FRAMEINTERVALS:
            return "VIDIOC_ENUM_FRAMEINTERVALS";
        case VIDIOC_DQEVENT:
            return "VIDIOC_DQEVENT";
        case VIDIOC_SUBSCRIBE_EVENT:
            return "VIDIOC_SUBSCRIBE_EVENT";
        case VIDIOC_UNSUBSCRIBE_EVENT:
            return "VIDIOC_UNSUBSCRIBE_EVENT";
        default:
            V4L2LOGH("unsupported cmd 0x%x\n", cmd);
            return "Unkown cmd";
    }
}

char* state2str(UINT32 state)
{
    switch(state) {
        case STATE_UNINIT:
            return "STATE_UNINIT";
        case STATE_INIT:
            return "STATE_INIT";
        case STATE_WORKING:
            return "STATE_WORKING";
        case STATE_HEADER:
            return "STATE_HEADER";
        case STATE_STOP:
            return "STATE_STOP";
        case STATE_SEEK:
            return "STATE_SEEK";
        case STATE_DRAIN:
            return "STATE_DRAIN";
        case STATE_RESET:
            return "STATE_RESET";
        default:
            return "unknown_state";
    }
}

char* m2mType2str(UINT32 type)
{
    switch(type) {
        case DECODER:
            return "decoder";
        case ENCODER:
            return "encoder";
        default:
            return "Unknown m2m type";

    }
}

char* mem2str(UINT32 type)
{
    switch (type) {
        case V4L2_MEMORY_MMAP:
            return "V4L2_MEMORY_MMAP";
        case V4L2_MEMORY_USERPTR:
            return "V4L2_MEMORY_USERPTR";
        case V4L2_MEMORY_DMABUF:
            return "V4L2_MEMORY_DMABUF";
        case V4L2_MEMORY_OVERLAY:
            return "V4L2_MEMORY_OVERLAY";
        default:
            return "UnKnow memory";
    }
}

BOOL list_is_empty(std_list *list)
{
    UINT size = 0;
    std_list_size(list, &size);
    return (size == 0);
}

UINT list_size(std_list *list)
{
    UINT size = 0;
    std_list_size(list, &size);
    return size;
}

void show_v4l2fmt(struct v4l2_format *fmt)
{
    UINT32 i = 0;

    if(!fmt) {
        V4L2LOGE("NULL v4l2 format");
        return ;
    }

    if (V4L2_TYPE_IS_MULTIPLANAR(fmt->type)) {
        struct v4l2_pix_format_mplane *p = &fmt->fmt.pix_mp;
        V4L2LOGD("\t[v4l2 format info:]\n");
        V4L2LOGD("\t[type:%s][multi-planar][pixformat:%s][num_plane:%d]\n",
                            buf2str(fmt->type), fmt2str(p->pixelformat), p->num_planes);
        V4L2LOGD("\t[resolution(%d x %d )]\n", p->width, p->height);
        for(i=0; i<p->num_planes; i++) {
            V4L2LOGD("\t[plane:%d][stride:%d][size:%d]\n", i,
                                p->plane_fmt[i].bytesperline,
                                p->plane_fmt[i].sizeimage);
        }
        V4L2LOGD("\t[color:%d][hsv:%d][quantize:%d]\n", p->colorspace, p->hsv_enc, p->quantization);
        V4L2LOGD("\t[field:%d][flags:%d][xref_func:%d]\n", p->field, p->flags, p->xfer_func);
    }
    else {
        struct v4l2_pix_format *p = &fmt->fmt.pix;
        V4L2LOGD("v4l2 fomat info:\n");
        V4L2LOGD("\t[type:%s][multi-planar][pixformat:%s]\n",
                            buf2str(fmt->type), fmt2str(p->pixelformat));
        V4L2LOGD("\t[resolution(%d x %d )]\n", p->width, p->height);
        V4L2LOGD("\t[color:%d][hsv:%d][quantize:%d]\n", p->colorspace, p->hsv_enc, p->quantization);
        V4L2LOGD("\t[field:%d][flags:%d][xref_func:%d]\n\n", p->field, p->flags, p->xfer_func);

    }
}

void dump_buff_cntx(struct v4l2_buffer *buf, const char *fun, UINT32 line)
{
    UINT32 i = 0;

    V4L2LOGD("v4l2 buffer info(%s, %d):\n", fun, line);

    if(!buf) {
        V4L2LOGE("NULL buf\n");
        return ;
    }

    V4L2LOGD("\t[index:%d][type:0x%x][isMultiPlane:%d][flags:0x%x][field:0x%x]\n",
            buf->index, buf->type, V4L2_TYPE_IS_MULTIPLANAR(buf->type),
            buf->flags, buf->field);
    V4L2LOGD("\t[timestamp:sec:%lu, usec:%lu]\n",
            buf->timestamp.tv_sec, buf->timestamp.tv_usec);
    if (V4L2_TYPE_IS_MULTIPLANAR(buf->type)) {
        struct v4l2_plane *plane = buf->m.planes;
        for(i=0; i<buf->length; i++) {
            V4L2LOGD("\t[plane#%d][bytesused:%d][length:%d]"
                        "[m.mem_offset:0x%x][m.userptr:0x%lx][data_offset:0x%x]\n",
                        i, plane->bytesused, plane->length,
                        plane->m.mem_offset, plane->m.userptr, plane->data_offset);
            plane ++;
        }
    }
    else {
        V4L2LOGD("\t[bytesused:%d][m.offset:0x%x][m.userptr:0x%lx][length:%d]\n\n",
                buf->bytesused, buf->m.offset, buf->m.userptr, buf->length);
    }
}

void dump_BDInfo(AMP_BD_HANDLE hBD,const char *fun, UINT32 line)
{
    AMP_BDTAG_MEMINFO *pMemInfo = NULL;
    AMP_BDTAG_STREAM_INFO *pStreamInfo = NULL;
    UINT32 i = 0, uNum = 0;

    V4L2LOGD("\n");
    V4L2LOGD("v4l2 BD Meminfo Dump [hBD:%p](%s, %d):\n", hBD, fun, line);

    AMP_BDTAG_GetNum(hBD, &uNum);
    for(i = 0; i < uNum; i++){
        AMP_BDTAG_GetWithIndex(hBD, i, (void **)&pMemInfo);
        if(AMP_BD_TAG_TYPE(pMemInfo) == AMP_BDTAG_ASSOCIATE_MEM_INFO) {
            V4L2LOGD("\tMemInfo:[uMemHandle:0x%x][uMemOffset:0x%x][uSize:0x%x]\n",
                pMemInfo->uMemHandle, pMemInfo->uMemOffset, pMemInfo->uSize);
        }
        else if(AMP_BD_TAG_TYPE(pMemInfo) == AMP_BDTAG_STREAM_INFO_META){
            pStreamInfo = (AMP_BDTAG_STREAM_INFO *)pMemInfo;
            V4L2LOGD("\tStreamInfo:[pts(H,L):0x%08x,0x%08x][res(%dx%d)][%s]\n",
                pStreamInfo->uiPtsHigh, pStreamInfo->uiPtsLow,
                pStreamInfo->uiWidth, pStreamInfo->uiHeight,
                pStreamInfo->uType?"Non-IDR frame":"IDR frame");
        }
   }
   V4L2LOGD("\n");
}

void dump_StreamBuff(void *fh, struct v4l2_buffer *buf)
{
#ifdef EN_DATA_DUMP
    V4L2Stream *strm = (V4L2Stream *)fh;
    char path[128];
    UINT8 *ptr = NULL;
    UINT32 size = 0, i = 0;
    V4L2M2MContext *m2mCtx = (V4L2M2MContext *)strm->m2m_ctx;

    if(!strm->fp) {
        sprintf(path, "%s/%s_%s_%p.dat", DATA_DIR,
            m2mCtx->type==DECODER?"decoder":"encoder",
            STREAM_NAME(strm->strm_type), strm);
        strm->fp = fopen(path, "w");
        if(!strm->fp) {
            V4L2LOGE("fail to open %s\n", path);
            return;
        }
    }

    //! buf->length == plane number
    for(i=0; i<buf->length; i++) {
        ptr = (UINT8 *)buf->m.planes[i].m.userptr;
        size = buf->m.planes[i].bytesused;
        V4L2LOGD("write to file: ptr:%p, size:%d\n", ptr, size);
        fwrite(ptr, 1, size, strm->fp);
    }

    fflush(strm->fp);
#endif
}

void dump_PortBD(void *fh, AMP_BD_HANDLE hBD)
{
#ifdef EN_DATA_DUMP
    V4L2Stream *strm = (V4L2Stream *)fh;
    AMP_BDTAG_MEMINFO *pMemInfo = NULL;
    char path[128];
    UINT8 *ptr = NULL;
    UINT32 size = 0, i = 0;
    V4L2M2MContext *m2mCtx = (V4L2M2MContext *)strm->m2m_ctx;

    if(!strm->port_fp) {
        sprintf(path, "%s/%s_%s_%p.dat", DATA_DIR,
                m2mCtx->type==DECODER?"decoder":"encoder",
                strm->strm_type==OUTPUT?"in":"out", strm);
        strm->port_fp = fopen(path, "w");
        if(!strm->port_fp) {
            V4L2LOGE("fail to open %s\n", path);
            return;
        }
    }

    AMP_BDTAG_GetWithIndex(hBD, 0, (void **)&pMemInfo);

    if(pMemInfo && AMP_BD_TAG_TYPE(pMemInfo) == AMP_BDTAG_ASSOCIATE_MEM_INFO) {
        AMP_SHM_GetVirtualAddress(pMemInfo->uMemHandle,
                                pMemInfo->uMemOffset,
                                (void **)&ptr);
        size = pMemInfo->uSize;
        V4L2LOGD("write to file: ptr:%p, size:%d\n", ptr, size);
        fwrite(ptr, 1, size, strm->port_fp);
    }
    fflush(strm->port_fp);
#endif
}

HRESULT BD_GetFrameInfo(AMP_BD_HANDLE hBD, AMP_BGTAG_FRAME_INFO **ppTag) {
    AMP_ASSERT(hBD != NULL);
    AMP_ASSERT(ppTag != NULL);

    HRESULT ret = SUCCESS;
    AMP_BGTAG_FRAME_INFO *pTag = NULL;
    ret = AMP_BDTAG_GetWithType(hBD, AMP_BGTAG_FRAME_INFO_META, NULL, (void **)&pTag);
    *ppTag = (AMP_BGTAG_FRAME_INFO *)pTag;

    return ret;
}

HRESULT BD_GetMemInfo(AMP_BD_HANDLE hBD, AMP_BDTAG_MEMINFO **ppTag) {
    AMP_ASSERT(hBD != NULL);
    AMP_ASSERT(ppTag != NULL);

    HRESULT ret = SUCCESS;
    AMP_BDTAG_MEMINFO *pTag;
    ret = AMP_BDTAG_GetWithType(hBD, AMP_BDTAG_ASSOCIATE_MEM_INFO, NULL, (void **)&pTag);
    *ppTag = (AMP_BDTAG_MEMINFO *)pTag;

    return ret;
}

BOOL is_validDispBD(AMP_BD_HANDLE hBD)
{
    HRESULT ret = V4L2_SUCCESS;
    AMP_BGTAG_FRAME_INFO *pInfo;

    ret = BD_GetFrameInfo(hBD, &pInfo);
    ASSERT_RET_SHOW(ret, "error!! get frameinfo failed");

    if(pInfo->uiContentW && pInfo->uiContentH) {
        return TRUE;
    } else {
        V4L2LOGD("invalid dispBD\n");
        return FALSE;
    }
}

UINT32 align(UINT32 num, UINT32 block)
{
    // block must be power of 2.
    AMP_ASSERT(block != 0);
    AMP_ASSERT((block & (block - 1)) == 0);
    return (num + block - 1) & (~(block - 1));
}