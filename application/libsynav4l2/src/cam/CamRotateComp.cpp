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

#include "CamRotateComp.h"
#include "v4l2_log.h"
#include "cam_config_reader.h"

CamRotateComp::CamRotateComp(AMP_FACTORY ampFactory, bool tunnelMode) {
    HRESULT ret;

    memset(&compInfo, 0, sizeof(compInfo));
    compInfo.type = AMP_COMPONENT_ROTATE;
    compInfo.tunnelMode = tunnelMode;
    compInfo.inPortIdx = AMP_ROTATE_IN;
    compInfo.outPortIdx = AMP_ROTATE_OUT;

    createComp(ampFactory);
}

CamRotateComp::~CamRotateComp() {
    HRESULT ret;

    AMP_RPC(ret, AMP_ROTATE_Close, compInfo.comp);
    if(ret != SUCCESS) {
        V4L2LOGE("rotate: close failed");
    }

    AMP_RPC(ret, AMP_ROTATE_Destroy, compInfo.comp);
    if(ret != SUCCESS) {
        V4L2LOGE("rotate: destroy failed");
    }
}

int CamRotateComp::config(unsigned int outFmt, unsigned int outWidth, unsigned int outHeight) {
    HRESULT ret;

    AMP_COMPONENT_CONFIG config;
    AmpMemClear(&config, sizeof(AMP_COMPONENT_CONFIG));
    config._d = AMP_COMPONENT_ROTATE;

    if(compInfo.tunnelMode){
        config._u.pROTATE.mode = AMP_TUNNEL;
    }else{
        config._u.pROTATE.mode = AMP_NON_TUNNEL;
    }

    int orientation = 0;
    bool isConnected = false;
    cam_read_hardware_config();
    int sensorCount = cam_get_total_sensors();
    for(int idx = 0; idx < sensorCount; idx++){
        int dev, addr;
        cam_get_i2c_addr(idx, &dev, &addr);
        isConnected = cam_is_connected(dev, addr);
        if(isConnected == true) {
            cam_get_orientation(idx, &orientation);
            break;
        }
    }

    int rotateAngle = AMP_ROTATE_ANGLE_0;
    if(orientation == 0)
        rotateAngle = AMP_ROTATE_ANGLE_0;
    else if(orientation == 90)
        rotateAngle = AMP_ROTATE_ANGLE_90;
    else if(orientation == 180)
        rotateAngle = AMP_ROTATE_ANGLE_180;
    else if(orientation == 270)
        rotateAngle = AMP_ROTATE_ANGLE_270;

    if(rotateAngle == AMP_ROTATE_ANGLE_90 || rotateAngle == AMP_ROTATE_ANGLE_270) {
        AMP_RPC(ret, AMP_ROTATE_SetOutputFormat, compInfo.comp, rotateAngle, outHeight, outWidth, 8, outFmt);
    }else {
        AMP_RPC(ret, AMP_ROTATE_SetOutputFormat, compInfo.comp, rotateAngle, outWidth, outHeight, 8, outFmt);
    }
    ASSERT_RET_SHOW(ret, "rotate: unable to set output format");

    AMP_RPC(ret, AMP_ROTATE_Open, compInfo.comp, &config);
    ASSERT_RET_SHOW(ret, "rotate: open failed");

    compInfo.outFmt = outFmt;
    compInfo.outWidth = outWidth;
    compInfo.outHeight = outHeight;
    return 0;
}

int CamRotateComp::pushFrame(AMP_PORT_IO port, AMP_BD_HANDLE hBD) {
    HRESULT ret;

    this->updateFrameCount(port, hBD);
    AMP_RPC(ret, AMP_ROTATE_PushBD, compInfo.comp, port, 0, hBD);
    ASSERT_RET_SHOW(ret, "rotate: push buffer failed");

    return 0;
}


int CamRotateComp::getFrameSize(unsigned int *pYSize, unsigned int *pUvSize) {
    HRESULT ret;
    AMP_RPC(ret, AMP_ROTATE_GetFrameSize, compInfo.comp, compInfo.outWidth,
            compInfo.outHeight, 8, compInfo.outFmt, pYSize, pUvSize);

    return 0;
}

int CamRotateComp::setState(int state) {
    HRESULT ret;

    AMP_RPC(ret, AMP_ROTATE_SetState, compInfo.comp, state);
    ASSERT_RET_SHOW(ret, "rotate: set state failed");

    return ret;
}

int CamRotateComp::flushPorts(void) {
    HRESULT ret;

    AMP_RPC(ret, AMP_ROTATE_ClearPortBuf, compInfo.comp, AMP_PORT_INPUT, 0);
    ASSERT_RET_SHOW(ret, "rotate: clear input port failed");

    AMP_RPC(ret, AMP_ROTATE_ClearPortBuf, compInfo.comp, AMP_PORT_OUTPUT, 0);
    ASSERT_RET_SHOW(ret, "rotate: clear output port failed");

    return 0;
}

