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

#include "CamPipeline.h"

#include <string.h>
#include <linux/videodev2.h>
#include <list>
#include <algorithm>

#include "CamCompIntf.h"
#include "CamIspComp.h"
#include "CamDewarpComp.h"
#include "CamRotateComp.h"
#include "CamDnsComp.h"
#include "CamBuffer.h"
#include "v4l2_log.h"

#if defined(ANDROID)
#include <cutils/properties.h>
#endif

#define START_FRAME_SKIP_COUNT_MAX 4

CamPipeline *CamPipeline::instance[CAM_PIPE_PRIVATE_MAX] = {nullptr, nullptr, nullptr};
pthread_mutex_t CamPipeline::camPipelineLock = PTHREAD_MUTEX_INITIALIZER;

//FIXME: use headerfile
extern "C" AMP_BD_HANDLE v4l2_create_bd(int codec_type, int buff_type, int width, int height, int srcFmt, int shm_size, AMP_SHM_HANDLE *pShm);
#define CAMERA 2

CamPipeline *CamPipeline::getInstance(AMP_FACTORY ampFactory, int outFmt, int width,
                                      int height, int cameraId,
                                      bool isDirect) {
    //Set the default to MP path
    int pipeId, newWidth, newHeight;
    bool isDewarp = false;

//TODO: Linux
#if defined(ANDROID)
    char property[PROPERTY_VALUE_MAX];
    property_get("ro.enable.ispss.dewarp", property, "false");
    if (property[0] == '1') {
        isDewarp = true;
        V4L2LOGD("Camera pipeline with Dewarp output...!\n");
    }
#endif

//-------------------------------------------------------------------//
//   | Direct/Processed Stream |     Dewarp   |     PipeId      |    //
//-------------------------------------------------------------------//
//   |  Direct                 |    Enabled   |   MP (Pipe 0)   |    //
//-------------------------------------------------------------------//
//   |  Processed              |    Enabled   |   SP1(Pipe 1)   |    //
//-------------------------------------------------------------------//
//   |  Direct                 |    Disabled  |   SP1(Pipe 1)   |    //
//-------------------------------------------------------------------//
//   |  Processed              |    Disabled  |   MP (Pipe 0)   |    //
//-------------------------------------------------------------------//

    if (isDirect && isDewarp) {
        pipeId = CAM_PIPE_P0;
        newWidth = width;
        newHeight = height;
    } else if (isDirect && !isDewarp) {
        pipeId = CAM_PIPE_P0;
        newWidth = width;
        newHeight = height;
    } else if (!isDirect && isDewarp) {
        pipeId = CAM_PIPE_P1;
        newWidth = CAM_SENSOR_W;
        newHeight = CAM_SENSOR_H;
    } else if (!isDirect && !isDewarp) {
        pipeId = CAM_PIPE_P0;
        newWidth = CAM_SENSOR_W;
        newHeight = CAM_SENSOR_H;
    }

    V4L2LOGE("PrimaryPipeline pipeId:%d w:%d h:%d isDirect:%d isDewarp:%d cameraId:%d",
           pipeId, newWidth, newHeight, isDirect, isDewarp, cameraId);

    bool needRotate = false;
    if((width > height && CAM_SENSOR_W < CAM_SENSOR_H) || (width < height && CAM_SENSOR_W > CAM_SENSOR_H)){
        needRotate = true;
    }

    if(needRotate) {
        newWidth = newWidth ^ newHeight;
        newHeight = newWidth ^ newHeight;
        newWidth = newWidth ^ newHeight;
    }

    /* output is always tiled as the frames are sent to dns pipeline */
    pthread_mutex_lock(&camPipelineLock);
    if (!instance[pipeId]) {
        instance[pipeId] = new CamPipeline(ampFactory, pipeId, cameraId, isDirect);
        instance[pipeId]->setDewarp(isDewarp);
        instance[pipeId]->config(outFmt, newWidth, newHeight);
    }
    instance[pipeId]->setCameraId(cameraId, width, height);
    pthread_mutex_unlock(&camPipelineLock);

    return instance[pipeId];
}


CamPipeline::CamPipeline(AMP_FACTORY ampFactory, int pipeId, int cameraId,
                         bool isDirect) {
    // Initialize stream config
    this->pipelineConfig.pipeId = pipeId;

    this->pipelineConfig.isDirect = isDirect;

    this->pipelineConfig.isDewarp = false;

    this->frameSkipCount = 1;

    this->pipelineConfig.cameraId = cameraId;

#if defined(ANDROID)
    if (pipeId < CAM_PIPE_PRIVATE_MAX) {
        char property[PROPERTY_VALUE_MAX];
        memset(property, '\0', PROPERTY_VALUE_MAX);
        property_get("ro.enable.ispss.skipstartframecount", property, "false");
        //property_get returns asci character('f') if property is not set.
        //check whether skipstartframecount is a valid digit or not. If not use the default count
        if (property[0] >= '0' && property[0] <= '9') {
            this->frameSkipCount = atoi(property);
            if (this->frameSkipCount > START_FRAME_SKIP_COUNT_MAX) {
                this->frameSkipCount = START_FRAME_SKIP_COUNT_MAX;
            }
        }
    }
#endif

    if(pipeId < CAM_PIPE_PRIVATE_MAX)
        this->pipelineConfig.tunnelMode = true;
    else
        this->pipelineConfig.tunnelMode = false;

    this->pipelineConfig.enabled = false;
    this->pipelineConfig.width = 0;
    this->pipelineConfig.height = 0;
    this->pipelineConfig.pipeline.clear();
    this->pipelineConfig.frameReadyCallback = nullptr;
    this->pipelineConfig.frameReadyData = nullptr;

    this->parentPipeline = nullptr;
    this->ampFactory = ampFactory;
    this->state = AMP_IDLE;
    this->connectPipelineState = 0;

    AMP_BDCHAIN_Create(TRUE, &pOutBDChain);
}

CamPipeline::~CamPipeline() {
    pthread_mutex_lock(&camPipelineLock);
    if (this->parentPipeline) {
        this->parentPipeline->disconnectPipeline(this);
    }

    destroyPipeline();

    AMP_BD_HANDLE hBD = nullptr;
    while(!this->bufferQueue.empty()) {
        hBD = this->bufferQueue.front();
        this->bufferQueue.pop();
        CamBuffer::destroyBD(hBD);
    }

    // Return the unused buffers
    do {
        hBD = nullptr;
        HRESULT ret = AMP_BDCHAIN_PopItem(pOutBDChain, &hBD);
        if(hBD != nullptr) {
            pipelineConfig.frameReadyCallback(pipelineConfig.pipeId, hBD, pipelineConfig.frameReadyData);
        }
    }while(hBD);
    AMP_BDCHAIN_Destroy(pOutBDChain);
    pipelineConfig.pipeline.clear();
    connectedPipelines.clear();

    if (this->pipelineConfig.pipeId < CAM_PIPE_PRIVATE_MAX) {
        CamPipeline::instance[this->pipelineConfig.pipeId] = nullptr;
    }
    pthread_mutex_unlock(&camPipelineLock);

}

void CamPipeline::setDewarp(bool isDewarp) {
    this->pipelineConfig.isDewarp = isDewarp;
}

void CamPipeline::setCameraId(int id, int width, int height) {

    if (this->pipelineConfig.pipeId < CAM_PIPE_PRIVATE_MAX) {
        //Primary pipeline can produce output from list of camera Id's
        std::vector<CamCompIntf *> *pipeline = &pipelineConfig.pipeline;
        if (!pipeline->empty()) {
            CamCompIntf *camComp = pipeline->front();
            if (camComp->compInfo.type == AMP_COMPONENT_ISP) {
                camComp->setCameraId(id);

                #if defined(VIDEO_WALL_MODE)
                CamIspComp* pIspComp = (CamIspComp*)camComp;
                pIspComp->updateCameraInfo(id, width, height);
                #endif
            } else {
                V4L2LOGE("unsupported operation");
            }
        } else {
            V4L2LOGE("pipeline is empty");
        }
    } else {
        //Secondary pipeline suppose to process request from single cameraId
        this->pipelineConfig.cameraId = id;
    }
}

int CamPipeline::destroyPipeline() {
    std::vector<CamCompIntf *> *pipeline = &pipelineConfig.pipeline;

    // Stop the pipeline
    if (this->state != AMP_IDLE) {
        for (int i=0; i < pipeline->size(); i++) {
            pipeline->at(i)->setState(AMP_IDLE);
        }
    }
    /* Check pipeline connect status before disconnecting */
    if (this->connectPipelineState == 1) {
        // Flush all the buffers
        for (int i=0; i < pipeline->size(); i++) {
            pipeline->at(i)->flushPorts();
        }

        // disconnect ports
        for (int i=0; i < pipeline->size(); i++) {
            pipeline->at(i)->disconnectPorts();
        }
    }

    this->connectPipelineState = 0;

    // remove all the components
    for (int i=pipeline->size()-1; i>=0; i--) {
        delete pipeline->at(i);
    }

    pipeline->clear();

    return 0;
}

CamPipelineConfig *CamPipeline::getPipelineConfig() {
    return &pipelineConfig;
}

int CamPipeline::numConnectPipeline() {
    return connectedPipelines.size();
}

int CamPipeline::connectPipeline(CamPipeline *camPipeline) {
    pthread_mutex_lock(&camPipelineLock);
    connectedPipelines.push_back(camPipeline);
    pthread_mutex_unlock(&camPipelineLock);
    return 0;
}

int CamPipeline::disconnectPipeline(CamPipeline *camPipeline) {
    connectedPipelines.erase(find(connectedPipelines.begin(),connectedPipelines.end(),camPipeline));
    return 0;
}

static int getRefCount(AMP_BD_HANDLE hBD) {
    BOOL valid;
    UINT32 refCnt = 0;
    AMP_BD_GetStatus(hBD, &valid, &refCnt);
    return refCnt;
}

static void sendDirectFrameReadyEvent(CamCompIntf *pCamComp, CamCompEventType event, void *p) {
    HRESULT ret;

    if(p == nullptr)
        return;

    CamPipeline *stream = (CamPipeline *) p;

    if (event == CAM_COMP_EVENT_FRAMEREADY) {
        AMP_BD_HANDLE hBD;
        ret = AMP_BDCHAIN_PopItem(pCamComp->pOutBDChain, &hBD);

        if (stream->getPipelineConfig()->pipeId < CAM_PIPE_PRIVATE_MAX) { //Primary pipeline
            if (stream->frameSkipCount <= 0 ) {
                for (int i = 0; i < stream->connectedPipelines.size(); ++i) {
                    CamPipeline *connectedStream = stream->connectedPipelines.at(i);

                    if (connectedStream->state != AMP_EXECUTING) {
                        continue;
                    }

                    unsigned int numOutFrames = 0;
                    AMP_BDCHAIN_GetItemNum(connectedStream->pOutBDChain, &numOutFrames);
                    if (numOutFrames == 0) {
                        continue;
                    }

                    CamPipelineConfig *pipelineConfig = connectedStream->getPipelineConfig();
                    std::vector<CamCompIntf *> *pipeline = &pipelineConfig->pipeline;
                    if (pipeline->empty()) {

                        AMP_BDTAG_ISPSENSOR_INFO *InSensorInfoTag = NULL;
                        ret = AMP_BDTAG_GetWithType(hBD, AMP_BDTAG_ISP_SENSOR_META,
                                                    NULL, (void **)&InSensorInfoTag);
                        if (ret == SUCCESS) {
                            if (InSensorInfoTag->uSensorIndex != pipelineConfig->cameraId) {
                                continue;
                            } else {
                                V4L2LOGD("Found matching DNS. pushing bd from camera id %d \
                                          to direct stream",InSensorInfoTag->uSensorIndex);
                            }
                        } else {
                            V4L2LOGD("Sensor info met tag not present in input bd");
                        }

                        AMP_BD_HANDLE hOutBD = nullptr; //Copy data from hBD to hOutBD
                        AMP_BDCHAIN_PopItem(connectedStream->pOutBDChain, &hOutBD);

                        AMP_BGTAG_FRAME_INFO *pInFrameInfo = NULL;
                        AMP_BGTAG_FRAME_INFO *pOutFrameInfo = NULL;
                        void *inVirtAddr_Y = NULL, *inVirtAddr_UV = NULL, *outVirtAddr_Y = NULL,
                             *outVirtAddr_UV = NULL;
                        AMP_SHM_HANDLE in_shm_handle;
                        AMP_SHM_HANDLE out_shm_handle;

                        HRESULT result = SUCCESS;
                        result = AMP_BDTAG_GetWithType(hBD, AMP_BGTAG_FRAME_INFO_META, NULL,
                                                        (void **)&pInFrameInfo);
                        if (result != SUCCESS) {
                            V4L2LOGE("failed to get input frame info");
                            continue;
                        }

                        result = AMP_BDTAG_GetWithType(hOutBD, AMP_BGTAG_FRAME_INFO_META, NULL,
                                                        (void **)&pOutFrameInfo);
                        if (result != SUCCESS) {
                            V4L2LOGE("failed to get output frame info");
                            continue;
                        }

//TODO: Remove memcpy once ISP component handles MMU

                        in_shm_handle = pInFrameInfo->hShm;
                        out_shm_handle = pOutFrameInfo->hShm;
                        uint32_t inputOffset = (((UINT32)(uintptr_t)(pInFrameInfo->uiPbufStartUV)) -
                                                 ((UINT32)(uintptr_t)(pInFrameInfo->pBufStart)));
                        uint32_t outputOffset = (((UINT32)(uintptr_t)(pOutFrameInfo->uiPbufStartUV)) -
                                                 ((UINT32)(uintptr_t)(pOutFrameInfo->pBufStart)));
                        void *buff = NULL;
                        AMP_SHM_Ref(pInFrameInfo->hShm, (void **)&buff);
                        AMP_SHM_InvalidateCache(pInFrameInfo->hShm, 0, pInFrameInfo->uiBufSize);
                        if( (AMP_SHM_GetVirtualAddress(in_shm_handle, 0,
                                                       (void**)&inVirtAddr_Y) == SUCCESS) &&
                            (AMP_SHM_GetVirtualAddress(out_shm_handle, 0,
                                                       (void**)&outVirtAddr_Y) == SUCCESS)) {
                            if (inVirtAddr_Y && outVirtAddr_Y) {
                                //Direct path memcpy starts
                                memcpy( outVirtAddr_Y, inVirtAddr_Y, outputOffset); //y size
                                inVirtAddr_UV  = (uint8_t*)inVirtAddr_Y  + inputOffset;
                                outVirtAddr_UV = (uint8_t*)outVirtAddr_Y + outputOffset;
                                memcpy( outVirtAddr_UV, inVirtAddr_UV,
                                        (pOutFrameInfo->uiBufSize - outputOffset)); //uv size
                                //Direct path memcpy ends
                                pipelineConfig->frameReadyCallback(pipelineConfig->pipeId, hOutBD,
                                                                   pipelineConfig->frameReadyData);
                            }
                        } else {
                            V4L2LOGE("Error getting virtual address for direct stream processing");
                        }
                        AMP_SHM_Unref(pInFrameInfo->hShm);
                    } else {
                        V4L2LOGE("TODO: Secondary Path callback cannot push to down stream component");
                    }
                }
            } else {
                stream->frameSkipCount -= 1;
            }
            stream->pushFrame(hBD); //Push back direct path bd
        } else {
            V4L2LOGE("Unexpected state as there is no down stream component for direct output");
        }
    } else {
        V4L2LOGE("Unexpected event in direct path:%d",event);
    }
}

static void sendProcessedFrameReadyEvent(CamCompIntf *pCamComp, CamCompEventType event, void *p) {
    HRESULT ret;

    if(p == nullptr)
        return;

    CamPipeline *stream = (CamPipeline *) p;
    pthread_mutex_lock(&stream->camPipelineLock);

    // frame from last component
    if(event == CAM_COMP_EVENT_FRAMEREADY) {
        AMP_BD_HANDLE hBD;
        ret = AMP_BDCHAIN_PopItem(pCamComp->pOutBDChain, &hBD);

        if(!(stream->getPipelineConfig()->pipeId < CAM_PIPE_PRIVATE_MAX)) { // Secondary pipeline
            CamCompInfo *pCompInfo = pCamComp->getComponentInfo();
            CamPipelineConfig *pipelineConfig = stream->getPipelineConfig();
            pipelineConfig->frameReadyCallback(pipelineConfig->pipeId, hBD,
                                               pipelineConfig->frameReadyData);
        }else { // Primary pipeline
            bool isBdPushed = false;
            if (stream->frameSkipCount <= 0) {
                for(int i = 0; i < stream->connectedPipelines.size(); i++) {
                    CamPipeline *connectedStream = stream->connectedPipelines.at(i);

                    if(connectedStream->state != AMP_EXECUTING) {
                        continue;
                    }

                    /* If there are no ouput frames in the connected component skip */
                    unsigned int numOutFrames = 0;
                    AMP_BDCHAIN_GetItemNum(connectedStream->pOutBDChain, &numOutFrames);
                    if(numOutFrames == 0) {
                        continue;
                    }

                    CamPipelineConfig *pipelineConfig = connectedStream->getPipelineConfig();
                    std::vector<CamCompIntf *> *pipeline = &pipelineConfig->pipeline;
                    if(!pipeline->empty()) {
                        CamCompIntf *camComp = pipeline->front();

                        AMP_BDTAG_ISPSENSOR_INFO *InSensorInfoTag = NULL;
                        ret = AMP_BDTAG_GetWithType(hBD, AMP_BDTAG_ISP_SENSOR_META,
                                                    NULL, (void **)&InSensorInfoTag);
                        if (ret == SUCCESS) {
                            if (InSensorInfoTag->uSensorIndex != camComp->compInfo.cameraId) {
                                continue;
                            } else {
                                V4L2LOGD("Found matching DNS. pushing bd to dns sensorid:%d",
                                          InSensorInfoTag->uSensorIndex);
                            }
                        } else {
                            V4L2LOGD("Sensor info met tag not present in input bd");
                        }

                        AMP_BD_HANDLE hOutBD = nullptr;
                        AMP_BDCHAIN_PopItem(connectedStream->pOutBDChain, &hOutBD);

                        if(hOutBD) {
                            /* Create duplicate BD before pushing to avoid
                               conflicting crop params between streams */
                            AMP_BD_HANDLE hNewBD = nullptr;
                            if(!stream->bufferQueue.empty()) {
                                hNewBD = stream->bufferQueue.front();
                                stream->bufferQueue.pop();
                            }else {
                                hNewBD = CamBuffer::createBD();
                            }

                            if(hNewBD) {
                                isBdPushed = true;
                                CamBuffer::copyBD(hNewBD, hBD);
                                // Insert the new to old buffer map
                                stream->bufferMap.insert(std::make_pair(hNewBD, hBD));

                                AMP_BD_Ref(hBD);
                                camComp->pushFrame(AMP_PORT_INPUT, hNewBD);
                                camComp = pipeline->back();
                                camComp->pushFrame(AMP_PORT_OUTPUT, hOutBD);
                            }
                        }
                    }
                }
            } else {
                stream->frameSkipCount -= 1;
            }

            /* If not pushed to any of the connected pipelines, recycle it */
            if(!isBdPushed) {
                stream->pushFrame(hBD);
            }

        }

    } else if(event == CAM_COMP_EVENT_FRAMERECYCLE) {
        CamPipeline *parentStream = stream->parentPipeline;
        AMP_BD_HANDLE hBD, hNewBD;

        ret = AMP_BDCHAIN_PopItem(pCamComp->pInBDChain, &hNewBD);

        hBD = parentStream->bufferMap[hNewBD];
        parentStream->bufferMap.erase(hNewBD);
        parentStream->bufferQueue.push(hNewBD);

        AMP_BD_Unref(hBD);

        if(getRefCount(hBD) <= 1) {
            CamPipelineConfig *pipelineConfig = parentStream->getPipelineConfig();
            std::vector<CamCompIntf *> *pipeline = &pipelineConfig->pipeline;
            if(!pipeline->empty()) {
                CamCompIntf *camComp = pipeline->back();
                camComp->pushFrame(AMP_PORT_OUTPUT, hBD);
            }
        }
    }

    pthread_mutex_unlock(&stream->camPipelineLock);
    return;
}

static CAM_ISPPATH_ID getIspPathId(int pipeId) {
    if(pipeId == CAM_PIPE_P0) {
        return CAM_ISPPATH_MAIN;
    } else if(pipeId == CAM_PIPE_P1) {
        return CAM_ISPPATH_SP1;
    } else if(pipeId == CAM_PIPE_P2) {
        return CAM_ISPPATH_SP2;
    } else{
        V4L2LOGE("invalid stream id to isp path id conversion");
    }
    return CAM_ISPPATH_MAIN;
}

int CamPipeline::addComp(std::vector<CamCompIntf *> *pipeline, CamCompIntf *camComp) {
    CamCompIntf *prevComp = nullptr;

    if(!pipeline->empty()) {
        prevComp = pipeline->back();
        if(prevComp) {
            prevComp->setOutComponent(camComp);
        }
    }

    if(camComp) {
        camComp->setInComponent(prevComp);
        pipeline->push_back(camComp);
    }

    return 0;
}

int CamPipeline::config(int outFmt, int width, int height) {
    std::vector<CamCompIntf *> *pipeline = &pipelineConfig.pipeline;
    if(!pipeline->empty()) {
        destroyPipeline();
    }

    pipelineConfig.width = width;
    pipelineConfig.height = height;

    int camCompCount = 0;
    CamCompIntf *camIspComp = nullptr;
    CamCompIntf *camDewarpComp = nullptr;
    CamCompIntf *camRotateComp = nullptr;
    CamCompIntf *camDnsComp = nullptr;
    if(pipelineConfig.pipeId < CAM_PIPE_PRIVATE_MAX) {
        CAM_ISPPATH_ID ispPath = getIspPathId(pipelineConfig.pipeId);
        camIspComp = CamIspComp::getInstance(ampFactory, pipelineConfig.tunnelMode, ispPath);
        camCompCount++;
        if(ispPath == CAM_ISPPATH_SP1) {
            if (pipelineConfig.isDewarp) {
                camDewarpComp = new CamDewarpComp(ampFactory, pipelineConfig.tunnelMode);
                camCompCount++;
            }
#if(BERLIN_CHIP_VERSION != BERLIN_VS680_A0)
            if((width > height && CAM_SENSOR_W < CAM_SENSOR_H) || (width < height && CAM_SENSOR_W > CAM_SENSOR_H)){
                camRotateComp = new CamRotateComp(ampFactory, pipelineConfig.tunnelMode);
                camCompCount++;
            }
#endif
        }
    }else {
        if (!this->pipelineConfig.isDirect) {
            camDnsComp = new CamDnsComp(ampFactory, pipelineConfig.tunnelMode);
            camDnsComp->setCameraId(pipelineConfig.cameraId);
            camCompCount++;
        } else {
            V4L2LOGE("Direct stream. not going to create DNS instance");
        }
    }

    if(camIspComp) addComp(pipeline, camIspComp);
    if(camDewarpComp) addComp(pipeline, camDewarpComp);
    if(camRotateComp) addComp(pipeline, camRotateComp);
    if(camDnsComp) addComp(pipeline, camDnsComp);

    // Error occurred in creating one of the components.
    if(pipeline->size() != camCompCount) {
        for (int i=0; i < pipeline->size(); i++) {
            delete pipeline->at(i);
        }
        pipeline->clear();
        return -1;
    }
    addComp(pipeline, nullptr);


    // configure components
    for (int i=0; i < pipeline->size(); i++) {
        CamCompIntf *camComp = pipeline->at(i);
        CamCompInfo *pCompInfo = camComp->getComponentInfo();

        // If component is connected to another set the fmt to tiled
        // Note: DNS cannot do format conversion.
        int compFmt = outFmt;
        if(pCompInfo->out != NULL) {
            CamCompInfo *pNextCompInfo = pCompInfo->out->getComponentInfo();
            if(pNextCompInfo->type != AMP_COMPONENT_DNS) {
                compFmt = AMP_DISP_SRCFMT_YUV420SP_TILED_V8H8;
            }
        }

        if (pCompInfo->type == AMP_COMPONENT_DNS) {
            camComp->config(compFmt, width, height);
        }else {
            if (pCompInfo->type == AMP_COMPONENT_ISP && pipelineConfig.isDirect) {
                camComp->config(compFmt, width, height);
            } else {
                #if defined(VIDEO_WALL_MODE)
                compFmt = AMP_DISP_SRCFMT_YUV444;
                #endif
                camComp->config(compFmt, CAM_SENSOR_W, CAM_SENSOR_H);
            }
        }
    }

    if (pipeline->size() != 0) {
        CamCompIntf *camLastComp = pipeline->back();
        if (this->pipelineConfig.isDirect) {
            camLastComp->regEventCallback(::sendDirectFrameReadyEvent, this);
        } else {
            camLastComp->regEventCallback(::sendProcessedFrameReadyEvent, this);
        }
    } else {
        V4L2LOGE("Secondary pipeline for direct stream .no need to register comp callback");
    }

    pipelineConfig.enabled = true;


    if(!(pipelineConfig.pipeId < CAM_PIPE_PRIVATE_MAX)) {
        CamPipeline *primaryPipeline = getInstance(this->ampFactory, outFmt, width,
                                                   height, pipelineConfig.cameraId,
                                                   pipelineConfig.isDirect);
        primaryPipeline->connectPipeline(this);
        this->parentPipeline = primaryPipeline;
    }

    return 0;
}

int CamPipeline::pushFrame(AMP_BD_HANDLE hBD) {
    std::vector<CamCompIntf *> *pipeline = &pipelineConfig.pipeline;

    if(pipelineConfig.pipeId < CAM_PIPE_PRIVATE_MAX) {
        // Push frame to output port of last component
        CamCompIntf *camComp = pipeline->back();
        camComp->pushFrame(AMP_PORT_OUTPUT, hBD);
    }else {
        HRESULT ret;
        ret = AMP_BDCHAIN_PushItem(pOutBDChain, hBD);
    }

    return 0;
}

void CamPipeline::regFrameReadyCallback(FrameReadyCallback cb, void *pUsrData) {

    pipelineConfig.frameReadyCallback = cb;
    pipelineConfig.frameReadyData = pUsrData;

    return;
}

int CamPipeline::getFrameSize(unsigned int *pYSize, unsigned int *pUvSize) {
    std::vector<CamCompIntf *> *pipeline = &pipelineConfig.pipeline;

    // Get frame size from the last component
    CamCompIntf *camComp = pipeline->back();
    camComp->getFrameSize(pYSize, pUvSize);

    return 0;
}

int CamPipeline::setCropWindow(int x, int y, int w, int h) {
    std::vector<CamCompIntf *> *pipeline = &pipelineConfig.pipeline;

    CamCompIntf *camComp = pipeline->back();
    if(camComp->compInfo.type == AMP_COMPONENT_DNS) {
        CamDnsComp *dns = (CamDnsComp *)camComp;
        dns->setCropWindow(x, y, w, h);
    }

    return 0;
}


int CamPipeline::start(void) {
    std::vector<CamCompIntf *> *pipeline = &pipelineConfig.pipeline;
    if (this->connectPipelineState == 0) {
        for (int i=0; i < pipeline->size(); i++) {
            CamCompIntf *pipeComp = pipeline->at(i);
            pipeComp->connectPorts();
        }
        this->connectPipelineState = 1;
    }

    /* start the ISP in the parent last */
    for (int i=pipeline->size()-1; i >= 0; i--) {
        pipeline->at(i)->setState(AMP_EXECUTING);
    }

    this->state = AMP_EXECUTING;

    /* start parent pipeline if not running */
    if(this->parentPipeline) {
        pthread_mutex_lock(&camPipelineLock);
        if (this->parentPipeline->state == AMP_IDLE) {
            this->parentPipeline->start();
        }
        pthread_mutex_unlock(&camPipelineLock);
    }

    return 0;
}

int CamPipeline::pause(void) {
    std::vector<CamCompIntf *> *pipeline = &pipelineConfig.pipeline;

    /* set the ISP to idle first */
    for (int i=0; i < pipeline->size(); i++) {
        pipeline->at(i)->setState(AMP_IDLE);
    }
    this->state = AMP_IDLE;

    return 0;
}


