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

#ifndef SRC_CAM_CAMPIPELINE_H_
#define SRC_CAM_CAMPIPELINE_H_

#include <map>
#include <queue>
#include <vector>

#include "CamIspComp.h"
#include "CamCompIntf.h"
#include "cam_stream.h"
#include "amp_client.h"

typedef enum _enum_cam_pipe_id {
    CAM_PIPE_P0 = 0, //main
    CAM_PIPE_P1,     //sp1
    CAM_PIPE_P2,     //sp2
    CAM_PIPE_PRIVATE_MAX,
    CAM_PIPE_S0 = CAM_PIPE_PRIVATE_MAX,
    CAM_PIPE_S1,
    CAM_PIPE_S2,
    CAM_PIPE_S3,
    CAM_PIPE_MAX
} CAM_PIPE_ID;

typedef struct _tag_cam_pipeline_config {
    int    pipeId;
    bool   tunnelMode;

    bool   enabled;
    int    width;
    int    height;

    std::vector<CamCompIntf *> pipeline;

    FrameReadyCallback       frameReadyCallback;
    void                     *frameReadyData;

    bool isDirect;
    bool isDewarp;
    int cameraId;
}CamPipelineConfig;

class CamPipeline {
private:
    /* These are primary paths sp1->dewarp->rotate/main/sp2 */
    static CamPipeline     *instance[CAM_PIPE_PRIVATE_MAX];
    static CamPipeline     *getInstance(AMP_FACTORY ampFactory, int outFmt, int width,
                                        int height, int cameraId = 1,
                                        bool isDirect = false);

    AMP_FACTORY ampFactory;
    CamPipelineConfig pipelineConfig;

    static int addComp(std::vector<CamCompIntf *> *pipeline, CamCompIntf *prevComp);
    static void sendProcessedFrameReadyEvent(CamCompIntf *camComp, CamCompEventType event, void *p);
    static void sendDirectFrameReadyEvent(CamCompIntf *camComp, CamCompEventType event, void *p);

public:
    int state;
    int connectPipelineState;
    AMP_BDCHAIN                      *pOutBDChain;
    CamPipeline                      *parentPipeline;
    std::vector<CamPipeline *>       connectedPipelines;
    static pthread_mutex_t camPipelineLock;

    std::queue<AMP_BD_HANDLE>        bufferQueue;
    std::map<AMP_BD_HANDLE, AMP_BD_HANDLE>  bufferMap;

    int frameSkipCount;

    CamPipeline(AMP_FACTORY ampFactory, int pipeId, int cameraId = 1,
                bool isDirect = false);
    virtual ~CamPipeline();

    CamPipelineConfig *getPipelineConfig(void);
    int destroyPipeline();

    int getFrameSize(unsigned int *pYSize, unsigned int *pUvSize);
    int setCropWindow(int x, int y, int w, int h);

    int config(int outFmt, int width, int height);

    int pushFrame(AMP_BD_HANDLE hBD);
    void regFrameReadyCallback(FrameReadyCallback cb, void *pUsrData);
    int numConnectPipeline();
    int connectPipeline(CamPipeline *camPipeline);
    int disconnectPipeline(CamPipeline *camPipeline);

    int start(void);
    int pause(void);
    void setDewarp(bool isDewarp = false);
    void setCameraId(int id, int width, int height);
};

#endif /* SRC_CAM_CAMPIPELINE_H_ */
