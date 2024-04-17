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

#ifndef SRC_CAM_CAM_STREAM_H_
#define SRC_CAM_CAM_STREAM_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "amp_client.h"
#include <stdbool.h>

#if(BERLIN_CHIP_VERSION == BERLIN_VS680_A0)
#define CAM_SENSOR_W 3840
#define CAM_SENSOR_H 2160
#else
#define CAM_SENSOR_W 1152
#define CAM_SENSOR_H 1536
#endif

#if defined(VIDEO_WALL_MODE)
#undef CAM_SENSOR_W
#undef CAM_SENSOR_H
#define CAM_SENSOR_W 1920
#define CAM_SENSOR_H 1080
#endif

typedef enum _enum_stream_id{
    CAM_STREAM_0 = 0,
    CAM_STREAM_1,
    CAM_STREAM_2,
    CAM_STREAM_3,
    CAM_STREAM_MAX,
    CAM_STREAM_INVALID
}CAM_STREAM_ID;

typedef struct _tag_cam_stream_capabilities {
    int    maxWidth;
    int    maxHeight;
    bool   hasRotate;
    bool   hasScaling;
    bool   hasDewarp;
}CamStreamCaps;

int cam_stream_findId(int width, int height);

void *cam_stream_create(int streamId, AMP_FACTORY ampFactory, int cameraId,
                        int isDirectOut);
int cam_stream_config(void *obj, int outFmt, int width, int height);
int cam_stream_pushFrame(void *obj, AMP_BD_HANDLE hBD);
int cam_stream_getFrameSize(void *obj, unsigned int *pYSize, unsigned int *pUvSize);
int cam_stream_setCropWindow(void *obj, int x, int y, int w, int h);
int cam_stream_start(void *obj);
int cam_stream_pause(void *obj);
int cam_stream_stop(void *obj);
int cam_stream_close(void *obj);

typedef void (*FrameReadyCallback)(int streamId, AMP_BD_HANDLE hBD, void *pUsrData);
int cam_stream_regCallback(void *obj, FrameReadyCallback cb, void *pUsrData);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* SRC_CAM_CAM_STREAM_H_ */
