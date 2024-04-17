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

#include <cassert>

#include "CamIspComp.h"
#include "v4l2_log.h"

#include <linux/videodev2.h>

CamIspComp *CamIspComp::instance[CAM_ISPPATH_MAX] = {nullptr, nullptr, nullptr};

CamIspCompInfo CamIspComp::ispCompInfo = {nullptr, -1};

CamIspComp *CamIspComp::getInstance(AMP_FACTORY ampFactory, bool tunnelMode, CAM_ISPPATH_ID pathId) {
    HRESULT ret;
    if(pathId >= CAM_ISPPATH_MAX) {
        V4L2LOGE("isp: invalid path id %d", pathId);
    }

/* ISP amp comp is shared across CamIspComp instances*/

    if (!ispCompInfo.hComp) {
        AMP_RPC(ret, AMP_FACTORY_CreateComponent, ampFactory, AMP_COMPONENT_ISP, 0,
                &ispCompInfo.hComp);
        if (ret != SUCCESS) {
            V4L2LOGE("Failed to create isp component");
        }
        AMP_RPC(ret, AMP_ISP_SetFrameMode, ispCompInfo.hComp, AMP_ISP_FRAME_MODE_STREAMING, 0);
        if(ret != SUCCESS){
            V4L2LOGE("isp: unable to set the frame mode");
            assert(0);
        }
    }

    if (!instance[pathId]) {
        instance[pathId] = new CamIspComp(tunnelMode, pathId);
    }
    return instance[pathId];
}

CamIspComp::CamIspComp(bool tunnelMode, CAM_ISPPATH_ID pathId) {
    HRESULT ret;

    this->pathId = pathId;

    memset(&compInfo, 0, sizeof(compInfo));
    compInfo.type = AMP_COMPONENT_ISP;
    compInfo.tunnelMode = tunnelMode;
    compInfo.inPortIdx = 0;
    compInfo.outPortIdx = pathId;
    compInfo.comp = ispCompInfo.hComp;
}

CamIspComp::~CamIspComp() {
    HRESULT ret;

    instance[pathId] = nullptr;

    bool bDestroy = true;
    for (int i = 0; i < CAM_ISPPATH_MAX; ++i) {
        if (instance[i]) {
            bDestroy = false;
        }
    }

    if (bDestroy) {

        /* Check ISP open status before closing */
        if (ispCompInfo.state != -1) {
            AMP_RPC(ret, AMP_ISP_Close, ispCompInfo.hComp);
            if(ret != SUCCESS){
                V4L2LOGE("isp: close failed");
                assert(0);
            }
        }

        AMP_RPC(ret, AMP_ISP_Destroy, ispCompInfo.hComp);
        if(ret != SUCCESS){
            V4L2LOGE("isp: destroy failed");
            assert(0);
        }
        ispCompInfo.hComp = nullptr;
        ispCompInfo.state = -1;
    }
}

int CamIspComp::config(unsigned int outFmt, unsigned int outWidth, unsigned int outHeight) {

    compInfo.outFmt = outFmt;
    compInfo.outWidth = outWidth;
    compInfo.outHeight = outHeight;

    return 0;
}

void CamIspComp::updateCameraInfo(int id, int width, int height)
{
    if (this->cameraIdMap.find(id) == this->cameraIdMap.end()) {
        this->cameraIdMap.insert(std::pair<int, std::pair<int, int> >(id, \
                                                                std::make_pair(width,height)));
    } else {
        V4L2LOGE("physical camera:%d already created for previous stream",id);
    }
}

void CamIspComp::setCameraId(int id) {
    this->cameraIdList.insert(id);
}

int CamIspComp::pushFrame(AMP_PORT_IO port, AMP_BD_HANDLE hBD) {
    HRESULT ret;
    UINT32 count = 0;

    this->updateFrameCount(port, hBD);
    AMP_RPC(ret, AMP_ISP_PushBD, compInfo.comp, port, pathId, hBD);
    ASSERT_RET_SHOW(ret, "isp: push buffer failed");

    return ret;
}

int CamIspComp::getFrameSize(unsigned int *pYSize, unsigned int *pUvSize) {
    HRESULT ret;
    AMP_RPC(ret, AMP_ISP_GetFrameSize, compInfo.comp, compInfo.outFmt,
            AMP_DISP_ORDER_UYVY, compInfo.outWidth, compInfo.outHeight, 8,
            pYSize, pUvSize);

    return ret;
}

int CamIspComp::connectPorts(void) {
    HRESULT ret;
    if (ispCompInfo.state == -1) {

        int sensorId   = 0;
        int sensorMode = 0;
        for (int id : cameraIdList) {
            switch (id) {
                case 1:
                case 2:
                    sensorId = sensorId | (1 << (id -1));
                    break;
                default:
                    V4L2LOGE("Unsupported physical camera id:%d", id);
                    break;
            }
        }
        if (cameraIdList.size() == 2) {
            sensorMode = AMP_ISP_MODE_MCM_ISP_STREAM;
        } else {
            sensorMode = AMP_ISP_MODE_INLINE;
        }
        #if defined(VIDEO_WALL_MODE)
        sensorMode = AMP_ISP_MODE_MCM_STREAM;
        #endif
        AMP_RPC(ret, AMP_ISP_SetInputSource, ispCompInfo.hComp, sensorId);
        if(ret != SUCCESS){
            V4L2LOGE("isp: unable to set input source");
            assert(0);
        }
        AMP_RPC(ret, AMP_ISP_SetMode, ispCompInfo.hComp, sensorMode);
        if(ret != SUCCESS){
            V4L2LOGE("isp: unable to set mode");
            assert(0);
        }

        #if defined(VIDEO_WALL_MODE)
        int id = 0;
        AMP_ISP_SENSOR_CONFIG sens_config;
        sens_config.uiDphy = 0; // 0 => Keep default value - don't change
        for (auto itr : this->cameraIdMap) {
            id = itr.first;
            if (id != 1 && id != 2) {
                V4L2LOGE("Unsupported physical camera ID in resolution map id:%d",id);
                continue;
            }
            std::pair<int,int>& resPair = itr.second;
            sens_config.uiWidth  = resPair.first;
            sens_config.uiHeight = resPair.second;

            AMP_RPC(ret, AMP_ISP_SetSensor, compInfo.comp, id - 1, &sens_config);
            ASSERT_RET_SHOW(ret, "isp: unable to set camera resolution");

            V4L2LOGD("CameraId:%d w:%d h:%d", id - 1, sens_config.uiWidth, sens_config.uiHeight);
        }
        #endif

        for (int i = 0; i < CAM_ISPPATH_MAX; ++i) {
            if (instance[i]) {
                AMP_RPC(ret, AMP_ISP_SetOutputFormat, instance[i]->compInfo.comp,
                        instance[i]->getPathId(), instance[i]->compInfo.outFmt,
                        AMP_DISP_ORDER_UYVY, instance[i]->compInfo.outWidth,
                        instance[i]->compInfo.outHeight, 8);
                ASSERT_RET_SHOW(ret, "isp: unable to set output format");
            }
        }
        AMP_COMPONENT_CONFIG config;
        AmpMemClear(&config, sizeof(AMP_COMPONENT_CONFIG));
        config._d = AMP_COMPONENT_ISP;

        if(compInfo.tunnelMode){
            config._u.pISP.mode = AMP_TUNNEL;
        }else {
            config._u.pISP.mode = AMP_NON_TUNNEL;
        }
        /* All ISP path should be configured before ISP open. Hence moved Isp open
           to connectPorts */

        AMP_RPC(ret, AMP_ISP_Open, compInfo.comp, &config);
        ASSERT_RET_SHOW(ret, "isp: open failed");

        ispCompInfo.state = AMP_LOADED;
    }

    CamCompIntf::connectPorts();
    return 0;
}

int CamIspComp::setState(int state) {
    HRESULT ret;
    if (state != ispCompInfo.state) {
        AMP_RPC(ret, AMP_ISP_SetState, compInfo.comp, state);
        ASSERT_RET_SHOW(ret, "isp: set state failed");
        ispCompInfo.state = state;
    }

    return ret;
}

int CamIspComp::flushPorts(void) {
    HRESULT ret;

    //AMP_RPC(ret, AMP_ISP_ClearPortBuf, compInfo.comp, AMP_PORT_INPUT, pathId);
    //ASSERT_RET_SHOW(ret, "isp: clear input port failed");

    AMP_RPC(ret, AMP_ISP_ClearPortBuf, compInfo.comp, AMP_PORT_OUTPUT, pathId);
    ASSERT_RET_SHOW(ret, "isp: clear output port failed");

    return 0;
}


