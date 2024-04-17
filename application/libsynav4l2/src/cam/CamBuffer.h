/*
 * CamBuffer.h
 *
 *  Created on: Apr 20, 2021
 *      Author:
 */

#ifndef SRC_CAM_CAMBUFFER_H_
#define SRC_CAM_CAMBUFFER_H_

#include "amp_client.h"

class CamBuffer {
public:
    static AMP_BD_HANDLE createBD();
    static AMP_BD_HANDLE copyBD(AMP_BD_HANDLE hBDOut, AMP_BD_HANDLE hBDIn);
    static int destroyBD(AMP_BD_HANDLE hBD);
};

#endif /* SRC_CAM_CAMBUFFER_H_ */
