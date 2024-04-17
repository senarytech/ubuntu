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

#include "CamDewarpComp.h"
#include "v4l2_log.h"


CamDewarpComp::CamDewarpComp(AMP_FACTORY ampFactory, bool tunnelMode) {
    HRESULT ret;

    memset(&compInfo, 0, sizeof(compInfo));
    compInfo.type = AMP_COMPONENT_DEWARP;
    compInfo.tunnelMode = tunnelMode;
    compInfo.inPortIdx = AMP_DEWARP_IN;
    compInfo.outPortIdx = AMP_DEWARP_OUT;

    createComp(ampFactory);
}

CamDewarpComp::~CamDewarpComp() {
    HRESULT ret;

    AMP_RPC(ret, AMP_DEWARP_Close, compInfo.comp);
    if(ret != SUCCESS) {
        V4L2LOGE("dewarp");
    }

    AMP_RPC(ret, AMP_DEWARP_Destroy, compInfo.comp);
    if(ret != SUCCESS) {
        V4L2LOGE("dewarp: destroy failed");
    }
}

int CamDewarpComp::config(unsigned int outFmt, unsigned int outWidth, unsigned int outHeight){
    HRESULT ret;

    CamCompInfo *pInCompInfo = compInfo.in->getComponentInfo();

    AMP_RPC(ret, AMP_DEWARP_SetInputFormat, compInfo.comp, pInCompInfo->outWidth, pInCompInfo->outHeight, 8, pInCompInfo->outFmt);
    ASSERT_RET_SHOW(ret, "dewarp: unable to set input format");

    AMP_RPC(ret, AMP_DEWARP_SetOutputFormat, compInfo.comp, outWidth, outHeight, 8, outFmt);
    ASSERT_RET_SHOW(ret, "dewarp: unable to set output format");

    AMP_COMPONENT_CONFIG config;
    AmpMemClear(&config, sizeof(AMP_COMPONENT_CONFIG));
    config._d = compInfo.type;

    if(compInfo.tunnelMode){
        config._u.pDEWARP.mode = AMP_TUNNEL;
    }else{
        config._u.pDEWARP.mode = AMP_NON_TUNNEL;
    }

    AMP_RPC(ret, AMP_DEWARP_Open, compInfo.comp, &config);
    ASSERT_RET_SHOW(ret, "dewarp: open failed");

    compInfo.outFmt = outFmt;
    compInfo.outWidth = outWidth;
    compInfo.outHeight = outHeight;
    return 0;
}

int CamDewarpComp::pushFrame(AMP_PORT_IO port, AMP_BD_HANDLE hBD) {
    HRESULT ret;

    this->updateFrameCount(port, hBD);
    AMP_RPC(ret, AMP_DEWARP_PushBD, compInfo.comp, port, 0, hBD);
    ASSERT_RET_SHOW(ret, "dewarp: push buffer failed");

    return 0;
}

int CamDewarpComp::getFrameSize(unsigned int *pYSize, unsigned int *pUvSize) {
    // Dewarp doesn't modify the buffer so same as previous component
    CamCompIntf *prevComp = compInfo.in;
    prevComp->getFrameSize(pYSize, pUvSize);

    return 0;
}

int CamDewarpComp::setState(int state) {
    HRESULT ret;

    AMP_RPC(ret, AMP_DEWARP_SetState, compInfo.comp, state);
    ASSERT_RET_SHOW(ret, "dewarp: set state failed");

    return ret;
}

int CamDewarpComp::flushPorts(void) {
    HRESULT ret;

    AMP_RPC(ret, AMP_DEWARP_ClearPortBuf, compInfo.comp, AMP_PORT_INPUT, 0);
    ASSERT_RET_SHOW(ret, "dewarp: clear input port failed");

    AMP_RPC(ret, AMP_DEWARP_ClearPortBuf, compInfo.comp, AMP_PORT_OUTPUT, 0);
    ASSERT_RET_SHOW(ret, "dewarp: clear output port failed");

    return 0;
}
