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

#ifndef SRC_CAM_CAMISPCOMP_H_
#define SRC_CAM_CAMISPCOMP_H_

#include "CamCompIntf.h"
#include <set>
#include <map>

typedef enum _enum_isppath_id {
    CAM_ISPPATH_MAIN = AMP_ISP_OUT_MP,
    CAM_ISPPATH_SP1 = AMP_ISP_OUT_SP1,
    CAM_ISPPATH_SP2 = AMP_ISP_OUT_SP2,
    CAM_ISPPATH_MAX
} CAM_ISPPATH_ID;

typedef struct _cam_isp_comp_info {
    AMP_COMPONENT hComp;
    int state;
}CamIspCompInfo;

class CamIspComp : public CamCompIntf {
private:
    int pathId;
    static CamIspComp     *instance[CAM_ISPPATH_MAX];
    static CamIspCompInfo ispCompInfo;
    std::set<int> cameraIdList;
    std::map<int, std::pair<int,int> > cameraIdMap;

public:

    static CamIspComp *getInstance(AMP_FACTORY ampFactory, bool tunnelMode, CAM_ISPPATH_ID pathId);
    CamIspComp(bool tunnelMode, CAM_ISPPATH_ID pathId);
    virtual ~CamIspComp();

    int config(unsigned int outFmt, unsigned int width, unsigned int height);
    int pushFrame(AMP_PORT_IO port, AMP_BD_HANDLE hBD);
    int getFrameSize(unsigned int *pYSize, unsigned int *pUvSize);
    int setState(int state);
    int flushPorts(void);
    int getPathId() { return this->pathId; };
    int connectPorts(void);
    void setCameraId(int id);
    void updateCameraInfo(int id, int width, int height);
};

#endif /* SRC_CAM_CAMISPCOMP_H_ */
