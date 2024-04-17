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

#include "CamDnsComp.h"
#include "v4l2_log.h"

CamDnsComp::CamDnsComp(AMP_FACTORY ampFactory, bool tunnelMode) {
    HRESULT ret;

    memset(&compInfo, 0, sizeof(compInfo));
    compInfo.type = AMP_COMPONENT_DNS;
    compInfo.tunnelMode = tunnelMode;
    compInfo.inPortIdx = 0;
    compInfo.outPortIdx = 0;
    compInfo.cameraId = AMP_ISP_INPUT_SENSOR1;

    x = 0;
    y = 0;
    w = 0;
    h = 0;

    createComp(ampFactory);
}

CamDnsComp::~CamDnsComp() {
    HRESULT ret;

    AMP_RPC(ret, AMP_DNS_Close, compInfo.comp);
    if(ret != SUCCESS) {
        V4L2LOGE("dns: close failed");
    }

    AMP_RPC(ret, AMP_DNS_Destroy, compInfo.comp);
    if(ret != SUCCESS) {
        V4L2LOGE("dns: destroy failed");
    }
}

int CamDnsComp::config(unsigned int outFmt, unsigned int outWidth, unsigned int outHeight) {
    HRESULT ret;

    AMP_COMPONENT_CONFIG config;
    AmpMemClear(&config, sizeof(AMP_COMPONENT_CONFIG));
    config._d = AMP_COMPONENT_DNS;

    if(compInfo.tunnelMode){
        config._u.pDNS.mode = AMP_TUNNEL;
    }else{
        config._u.pDNS.mode = AMP_NON_TUNNEL;
    }

    AMP_RPC(ret, AMP_DNS_Open, compInfo.comp, &config);
    ASSERT_RET_SHOW(ret, "dns: open failed");

    AMP_RPC(ret, AMP_DNS_SetSensorType, compInfo.comp, compInfo.cameraId);
    ASSERT_RET_SHOW(ret, "dns: set sensor type failed");

    AMP_RPC(ret, AMP_DNS_SetOutputFormat, compInfo.comp, outWidth, outHeight, 8, outFmt);
    ASSERT_RET_SHOW(ret, "dns: unable to set output format");

    compInfo.outFmt = outFmt;
    compInfo.outWidth = outWidth;
    compInfo.outHeight = outHeight;

    return 0;
}

int CamDnsComp::pushFrame(AMP_PORT_IO port, AMP_BD_HANDLE hBD) {
    HRESULT ret;

#if 1
    AMP_BGTAG_FRAME_INFO *pFrameInfo = nullptr;
    AMP_BDTAG_GetWithType(hBD, AMP_BGTAG_FRAME_INFO_META, nullptr, (void **)&pFrameInfo);
    if(x==0 && y==0 && w==0 && h==0){
        pFrameInfo->iActiveX = 0;
        pFrameInfo->iActiveY = 0;
        pFrameInfo->uiActiveW = pFrameInfo->uiContentW;
        pFrameInfo->uiActiveH = pFrameInfo->uiContentH;
    }else {
        pFrameInfo->iActiveX = x;
        pFrameInfo->iActiveY = y;
        pFrameInfo->uiActiveW = w;
        pFrameInfo->uiActiveH = h;
    }
#endif

    this->updateFrameCount(port, hBD);
    AMP_RPC(ret, AMP_DNS_PushBD, compInfo.comp, port, 0, hBD);
    ASSERT_RET_SHOW(ret, "dns: push buffer failed");

    return 0;
}

int CamDnsComp::getFrameSize(unsigned int *pYSize, unsigned int *pUvSize) {
    HRESULT ret;
    AMP_RPC(ret, AMP_DNS_GetFrameSize, compInfo.comp, compInfo.outWidth,
            compInfo.outHeight, 8, compInfo.outFmt, pYSize, pUvSize);

    return 0;
}

int CamDnsComp::setState(int state) {
    HRESULT ret;

    AMP_RPC(ret, AMP_DNS_SetState, compInfo.comp, state);
    ASSERT_RET_SHOW(ret, "dns: set state failed");

    return ret;
}

int CamDnsComp::flushPorts(void) {
    HRESULT ret;

    AMP_RPC(ret, AMP_DNS_ClearPortBuf, compInfo.comp, AMP_PORT_INPUT, 0);
    ASSERT_RET_SHOW(ret, "dns: clear input port failed");

    AMP_RPC(ret, AMP_DNS_ClearPortBuf, compInfo.comp, AMP_PORT_OUTPUT, 0);
    ASSERT_RET_SHOW(ret, "dns: clear output port failed");

    return 0;
}

int CamDnsComp::setCropWindow(int x, int y, int w, int h) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    return 0;
}

