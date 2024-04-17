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

#include <math.h>
#include <time.h>
#include <linux/videodev2.h>

#include "CamCompIntf.h"
#include "amp_client_rpc.h"
#include "v4l2_log.h"

//FIXME: use headerfile
extern "C" AMP_BD_HANDLE v4l2_create_bd(int codec_type, int buff_type, int width, int height, int srcFmt, int shm_size, AMP_SHM_HANDLE *pShm);

//FIXME: need to use v4l2_m2m.h headerfile
#define CAMERA 2
#define CAMP_COMP_MINBUFFERS 3
#define CAMP_ISP_MINBUFFERS 5

CamCompIntf::CamCompIntf() {
    AMP_BDCHAIN_Create(TRUE, &pOutBDChain);
    AMP_BDCHAIN_Create(TRUE, &pInBDChain);

    MV_OSAL_Mutex_Create(&compInfoLock);

    compInfo.bufferCount[AMP_PORT_INPUT] = 0;
    compInfo.bufferCount[AMP_PORT_OUTPUT] = 0;
}

CamCompIntf::~CamCompIntf() {
    AMP_BDCHAIN_Destroy(pOutBDChain);
    AMP_BDCHAIN_Destroy(pInBDChain);

    if(compInfoLock) {
        MV_OSAL_Mutex_Destroy(&compInfoLock);
    }
}

static HRESULT compCallback(CORBA_Object hCompObj, AMP_PORT_IO ePortIo,
                             UINT32 uiPortIdx, struct AMP_BD_ST *hBD,
                             AMP_IN void *pUserData) {
    HRESULT ret = SUCCESS;

    CamCompIntf *camComp = (CamCompIntf *)pUserData;
    if(camComp == nullptr) {
        return ret;
    }

    MV_OSAL_Mutex_Lock(camComp->compInfoLock);
    CamCompInfo *pCompInfo = camComp->getComponentInfo();
    if(hBD != NULL) {
        pCompInfo->bufferCount[ePortIo]--;
    }
    MV_OSAL_Mutex_Unlock(camComp->compInfoLock);

    if(pCompInfo->tunnelMode) {
        ret = AMP_BDCHAIN_PushItem(camComp->pOutBDChain, hBD);
        pCompInfo->eventCallback(camComp, CAM_COMP_EVENT_FRAMEREADY, pCompInfo->eventData);
    } else { // non-tunnel mode
        if(ePortIo == AMP_PORT_INPUT) {
            ret = AMP_BDCHAIN_PushItem(camComp->pInBDChain, hBD);
            pCompInfo->eventCallback(camComp, CAM_COMP_EVENT_FRAMERECYCLE, pCompInfo->eventData);
        }else if (ePortIo == AMP_PORT_OUTPUT) {
            ret = AMP_BDCHAIN_PushItem(camComp->pOutBDChain, hBD);
            pCompInfo->eventCallback(camComp, CAM_COMP_EVENT_FRAMEREADY, pCompInfo->eventData);
        }
    }

    return ret;
}

int CamCompIntf::createComp(AMP_FACTORY ampFactory) {
    HRESULT ret;

    // Create component
    AMP_RPC(ret, AMP_FACTORY_CreateComponent, ampFactory, compInfo.type, 0, &compInfo.comp);
    if (ret != SUCCESS) {
        V4L2LOGE(DTAG"unable to create amp component type=%d\n", compInfo.type);
        return ret;
    }

    return 0;
}

void CamCompIntf::regEventCallback(EventCallback cb, void *pData) {

    compInfo.eventCallback = cb;
    compInfo.eventData = pData;

}

static long int getTimeMS() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec*1000 + lround(t.tv_nsec/1.0e6);
}

int CamCompIntf::disconnectPorts() {
    HRESULT ret;

    long int start = getTimeMS();
    while (true) {
        long int curr = getTimeMS();
        long int diff = curr - start;
        /* wrap around */
        if(diff < 0){
            start = getTimeMS();
            continue;
        }

        // timeout in 2 sec
        if(diff > 2000){
            break;
        }

        if (compInfo.bufferCount[AMP_PORT_INPUT] == 0 && compInfo.bufferCount[AMP_PORT_OUTPUT] == 0) {
            break;
        }
        V4L2LOGD("comp type:%d wait for frames input=%d output=%d time=%ld ms\n",
                compInfo.type, compInfo.bufferCount[AMP_PORT_INPUT],
                compInfo.bufferCount[AMP_PORT_OUTPUT], diff);
        usleep(170000);// 17ms
    }

    if(compInfo.bufferCount[AMP_PORT_INPUT] != 0 || compInfo.bufferCount[AMP_PORT_OUTPUT] != 0){
        // TODO: error handling
        V4L2LOGE("comp type:%d frames not returned input=%d output=%d\n",
                compInfo.type, compInfo.bufferCount[AMP_PORT_INPUT],
                compInfo.bufferCount[AMP_PORT_OUTPUT]);
    }

    // Disconnect output port
    if(compInfo.out == NULL || !compInfo.tunnelMode){
        ret = AMP_DisconnectApp(compInfo.comp, AMP_PORT_OUTPUT, compInfo.outPortIdx, ::compCallback);
        ASSERT_RET_SHOW(ret, "comp: disconnect app output failed");
    }else{
        CamCompInfo *pOutCompInfo = compInfo.out->getComponentInfo();
        ret = AMP_DisconnectComp(compInfo.comp, 0, pOutCompInfo->comp, 0);
        ASSERT_RET_SHOW(ret, "comp: disconnect comp output failed");
    }

    // Disconnect input port
    if((compInfo.in == NULL || !compInfo.tunnelMode) && (compInfo.type != AMP_COMPONENT_ISP)) {
        ret = AMP_DisconnectApp(compInfo.comp, AMP_PORT_INPUT, compInfo.inPortIdx, ::compCallback);
        ASSERT_RET_SHOW(ret, "comp: disconnect app input failed");
    }

    return 0;
}

int CamCompIntf::connectPorts() {
    HRESULT ret;

    CamCompIntf *pInComp = compInfo.in;
    CamCompIntf *pOutComp = compInfo.out;

    if((pInComp == NULL || !compInfo.tunnelMode) && (compInfo.type != AMP_COMPONENT_ISP)) {
        ret = AMP_ConnectApp(compInfo.comp, AMP_PORT_INPUT, compInfo.inPortIdx, ::compCallback, this);
        ASSERT_RET_SHOW(ret, "comp: connect app input failed");
    }

    if(pOutComp == NULL || !compInfo.tunnelMode){
        ret = AMP_ConnectApp(compInfo.comp, AMP_PORT_OUTPUT, compInfo.outPortIdx, ::compCallback, this);
        ASSERT_RET_SHOW(ret, "comp: connect app output failed");
    }else{
        CamCompInfo * pOutCompInfo = pOutComp->getComponentInfo();
        ret = AMP_ConnectComp(compInfo.comp, compInfo.outPortIdx, pOutCompInfo->comp, 0);
        ASSERT_RET_SHOW(ret, "comp: connect comp output failed");
    }

    return 0;
}

int CamCompIntf::setInComponent(CamCompIntf *pInComp) {
    HRESULT ret;

    compInfo.in = pInComp;

    return 0;
}

void CamCompIntf::setCameraId(int id) {

    compInfo.cameraId = id;

    return;
}

int CamCompIntf::setOutComponent(CamCompIntf *pOutComp) {
    HRESULT ret;

    compInfo.out = pOutComp;

    return 0;
}

CamCompInfo * CamCompIntf::getComponentInfo(void) {
    return &compInfo;
}

int CamCompIntf::updateFrameCount(AMP_PORT_IO port, AMP_BD_HANDLE hBD) {
    MV_OSAL_Mutex_Lock(compInfoLock);
    compInfo.bufferCount[port]++;
    MV_OSAL_Mutex_Unlock(compInfoLock);
    return 0;
}


