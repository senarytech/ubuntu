/*
 * CamBuffer.cpp
 *
 *  Created on: Apr 20, 2021
 *      Author:
 */

#include "CamBuffer.h"

#include "amp_client.h"
#include "OSAL_api.h"
#include "v4l2_log.h"

AMP_BD_HANDLE CamBuffer::createBD() {
    HRESULT ret = SUCCESS;

    AMP_BD_HANDLE hBD = NULL;
    AMP_BDTAG_MEMINFO memInfo;
    AMP_BDTAG_STREAM_INFO streamInfo;
    AMP_BGTAG_FRAME_INFO frameInfo;
    AMP_BDTAG_ISPSENSOR_INFO sensorInfo;

    ret = AMP_BD_Allocate(&hBD);
    if(ret != SUCCESS) {
        goto error;
    }

    ret = AMP_BD_Ref(hBD);
    if(ret != SUCCESS) {
        goto error;
    }

    AMP_BDTAG_MEMINFO_INIT(&memInfo);
    ret = AMP_BDTAG_Append(hBD, (UINT8 *)&memInfo, NULL, NULL);
    if(ret != SUCCESS) {
        goto error;
    }

    AMP_BDTAG_STREAMINFO_INIT(&streamInfo);
    ret = AMP_BDTAG_Append(hBD, (UINT8 *)&streamInfo, NULL, NULL);
    if(ret != SUCCESS) {
        goto error;
    }


    AMP_BDTAG_FRAMEINFO_INIT(&frameInfo);
    ret = AMP_BDTAG_Append(hBD, (UINT8*) &frameInfo, NULL, NULL);
    if (ret != SUCCESS) {
        goto error;
    }

    AmpMemSet(&sensorInfo, 0, sizeof(sensorInfo));
    sensorInfo.Header.eType = AMP_BDTAG_ISP_SENSOR_META;
    sensorInfo.Header.uLength = sizeof(AMP_BDTAG_ISPSENSOR_INFO);
    ret = AMP_BDTAG_Append(hBD, (UINT8 *)&sensorInfo, NULL, NULL);
    if (ret != SUCCESS) {
        goto error;
    }


    return hBD;

error:
    if(hBD) {
        AMP_BD_Unref(hBD);
        AMP_BD_Free(hBD);
    }
    return nullptr;
}

AMP_BD_HANDLE CamBuffer::copyBD(AMP_BD_HANDLE hBDOut, AMP_BD_HANDLE hBDIn) {

    if(hBDOut == nullptr || hBDIn == nullptr) {
        V4L2LOGE("unable to create BD\n");
    }

    AMP_BDTAG_MEMINFO *pInMemInfo = nullptr;
    AMP_BDTAG_MEMINFO *pOutMemInfo = nullptr;
    AMP_BDTAG_GetWithType(hBDIn, AMP_BDTAG_ASSOCIATE_MEM_INFO, nullptr, (void **)&pInMemInfo);
    AMP_BDTAG_GetWithType(hBDOut, AMP_BDTAG_ASSOCIATE_MEM_INFO, nullptr, (void **)&pOutMemInfo);
    if(pInMemInfo != nullptr && pOutMemInfo != nullptr) {
        memcpy(pOutMemInfo, pInMemInfo, sizeof(AMP_BDTAG_MEMINFO));
    }

    AMP_BDTAG_STREAM_INFO *pInStreamInfo = nullptr;
    AMP_BDTAG_STREAM_INFO *pOutStreamInfo = nullptr;
    AMP_BDTAG_GetWithType(hBDIn, AMP_BDTAG_STREAM_INFO_META, nullptr, (void **)&pInStreamInfo);
    AMP_BDTAG_GetWithType(hBDOut, AMP_BDTAG_STREAM_INFO_META, nullptr, (void **)&pOutStreamInfo);
    if(pInStreamInfo != nullptr && pOutStreamInfo != nullptr) {
        memcpy(pOutStreamInfo, pInStreamInfo, sizeof(AMP_BDTAG_STREAM_INFO));
    }

    AMP_BGTAG_FRAME_INFO *pInFrameInfo = nullptr;
    AMP_BGTAG_FRAME_INFO *pOutFrameInfo = nullptr;
    AMP_BDTAG_GetWithType(hBDIn, AMP_BGTAG_FRAME_INFO_META, nullptr, (void **)&pInFrameInfo);
    AMP_BDTAG_GetWithType(hBDOut, AMP_BGTAG_FRAME_INFO_META, nullptr, (void **)&pOutFrameInfo);
    if(pInFrameInfo != nullptr && pOutFrameInfo != nullptr) {
        memcpy(pOutFrameInfo, pInFrameInfo, sizeof(AMP_BGTAG_FRAME_INFO));
    }

    AMP_BDTAG_ISPSENSOR_INFO *pInSensorInfoTag = nullptr;
    AMP_BDTAG_ISPSENSOR_INFO *pOutSensorInfoTag = nullptr;
    AMP_BDTAG_GetWithType(hBDIn, AMP_BDTAG_ISP_SENSOR_META, nullptr, (void **)&pInSensorInfoTag);
    AMP_BDTAG_GetWithType(hBDOut, AMP_BDTAG_ISP_SENSOR_META, nullptr, (void **)&pOutSensorInfoTag);
    if (pInSensorInfoTag != nullptr && pOutSensorInfoTag!= nullptr) {
        memcpy(pOutSensorInfoTag, pInSensorInfoTag, sizeof(AMP_BDTAG_ISPSENSOR_INFO));
    }

    return hBDOut;
}

int CamBuffer::destroyBD(AMP_BD_HANDLE hBD) {
    AMP_BD_Unref(hBD);
    AMP_BD_Free(hBD);

    return 0;
}
