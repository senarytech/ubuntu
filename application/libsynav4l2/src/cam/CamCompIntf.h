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

#ifndef SRC_CAM_CAMCOMPINTF_H_
#define SRC_CAM_CAMCOMPINTF_H_

#include "amp_client.h"

#define DTAG "[camera]"

typedef enum _enum_comp_type_ {
    CAM_COMP_START = 0,
    CAM_COMP_ISP = CAM_COMP_START,
    CAM_COMP_DEWARP,
    CAM_COMP_ROTATE,
    CAM_COMP_DNS,
    CAM_COMP_MAX,
    CAM_COMP_INVALID
}CamCompType;

typedef enum _enum_comp_event_type_ {
    CAM_COMP_EVENT_FRAMEREADY = 0,
    CAM_COMP_EVENT_FRAMERECYCLE,
    CAM_COMP_EVENT_MAX,
}CamCompEventType;

class CamCompIntf;
typedef void (*EventCallback)(CamCompIntf *pCamComp, CamCompEventType event, void *p);

typedef struct _cam_comp_info {
    AMP_COMPONENT         comp;
    AMP_COMPONENT_TYPE    type;

    bool                  tunnelMode;

    int                   inPortIdx;
    int                   outPortIdx;

    unsigned int          outFmt;
    unsigned int          outWidth;
    unsigned int          outHeight;

    CamCompIntf           *in;
    CamCompIntf           *out;

    EventCallback         eventCallback;
    void                  *eventData;

    unsigned int          bufferCount[AMP_PORT_INVALIDE_IO];

    int cameraId;
}CamCompInfo;

class CamCompIntf {
public:
    CamCompInfo           compInfo;
    MV_OSAL_HANDLE_MUTEX_t compInfoLock;

    AMP_BDCHAIN           *pOutBDChain;
    AMP_BDCHAIN           *pInBDChain;

public:
    CamCompIntf();
    virtual ~CamCompIntf();

    int createComp(AMP_FACTORY ampFactory);

    void regEventCallback(EventCallback cb, void *pData);
    virtual int config(unsigned int outFmt, unsigned int width, unsigned int height){return 0;};
    virtual int pushFrame(AMP_PORT_IO port, AMP_BD_HANDLE hBD){return 0;};
    virtual int getFrameSize(unsigned int *pYSize, unsigned int *pUvSize){return 0;};
    virtual int setState(int state){return 0;};
    virtual int flushPorts(void){return 0;};

    int updateFrameCount(AMP_PORT_IO port, AMP_BD_HANDLE hBD);

    CamCompInfo * getComponentInfo(void);
    int setInComponent(CamCompIntf *pInComp);
    int setOutComponent(CamCompIntf *pOutComp);
    virtual int connectPorts(void);
    int disconnectPorts(void);
    virtual void setCameraId(int id);

    HRESULT compCallback(CORBA_Object hCompObj, AMP_PORT_IO ePortIo,
                                 UINT32 uiPortIdx, struct AMP_BD_ST *hBD,
                                 AMP_IN void *pUserData);
};

#endif /* SRC_CAM_CAMCOMPINTF_H_ */
