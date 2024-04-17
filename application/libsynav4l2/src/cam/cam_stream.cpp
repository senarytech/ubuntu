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

#include "v4l2_log.h"

#include "cam_stream.h"
#include "CamPipeline.h"

static void *gStreams[CAM_STREAM_MAX];

#ifdef __cplusplus
extern "C"
{
#endif

int cam_stream_findId(int width, int height) {

    for(int i = CAM_STREAM_0; i < CAM_STREAM_MAX; i++) {
        // check if the stream is available
        if(gStreams[i] != nullptr) {
            continue;
        }
        return i;
    }

    return CAM_STREAM_INVALID;
}

void *cam_stream_create(int streamId, AMP_FACTORY ampFactory, int cameraId,
                        int isDirectOut){
    if(gStreams[streamId] != NULL){
        V4L2LOGE("stream is already configured");
        return nullptr;
    }

    CamPipeline *pipe = new CamPipeline(ampFactory, streamId + CAM_PIPE_PRIVATE_MAX,
                                        cameraId,
                                        (isDirectOut > 0 ? true:false));
    gStreams[streamId] = pipe;

    return pipe;
}

int cam_stream_config(void *obj, int outFmt, int width, int height) {
    CamPipeline *pipe = (CamPipeline *)obj;
    pipe->config(outFmt, width, height);
    return 0;
}

int cam_stream_pushFrame(void *obj, AMP_BD_HANDLE hBD) {
    CamPipeline *pipe = (CamPipeline *)obj;
    pipe->pushFrame(hBD);
    return 0;
}

int cam_stream_getFrameSize(void *obj, unsigned int *pYSize, unsigned int *pUvSize) {
    CamPipeline *pipe = (CamPipeline *)obj;
    pipe->getFrameSize(pYSize, pUvSize);

    return 0;
}

int cam_stream_setCropWindow(void *obj, int x, int y, int w, int h) {
    CamPipeline *pipe = (CamPipeline *)obj;
    pipe->setCropWindow(x, y, w, h);

    return 0;
}

int cam_stream_start(void *obj) {
    CamPipeline *pipe = (CamPipeline *)obj;
    pipe->start();
    return 0;
}

int cam_stream_pause(void *obj) {
    CamPipeline *pipe = (CamPipeline *)obj;
    pipe->pause();
    return 0;
}

int cam_stream_close(void *obj) {
    CamPipeline *pipe = (CamPipeline *)obj;
    CamPipeline *parentPipeline = pipe->parentPipeline;
    CamPipelineConfig *pipelineConfig = pipe->getPipelineConfig();
    gStreams[pipelineConfig->pipeId - CAM_PIPE_PRIVATE_MAX] = NULL;

    delete (CamPipeline *)obj;

    if (parentPipeline && parentPipeline->numConnectPipeline() == 0 ) {
        if (parentPipeline->state != AMP_IDLE) {
            parentPipeline->pause();
        }
        delete parentPipeline;
    }
    return 0;
}

int cam_stream_stop(void *obj) {
    CamPipeline *pipe = (CamPipeline *)obj;
    CamPipelineConfig *pipelineConfig = pipe->getPipelineConfig();
    gStreams[pipelineConfig->pipeId - CAM_PIPE_PRIVATE_MAX] = NULL;

    delete (CamPipeline *)obj;
    return 0;
}

int cam_stream_regCallback(void *obj, FrameReadyCallback cb, void *pUsrData) {
    CamPipeline *pipe = (CamPipeline *)obj;
    pipe->regFrameReadyCallback(cb, pUsrData);
    return 0;
}

#ifdef __cplusplus
} // extern "C"
#endif
