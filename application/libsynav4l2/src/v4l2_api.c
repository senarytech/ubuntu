/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2017 Synaptics Incorporated. All rights reserved.
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
#include "linux/videodev2.h"

#include "v4l2_api.h"
#include "v4l2_types.h"
#include "v4l2_stream.h"
#include "v4l2_m2m.h"
#include "v4l2_misc.h"
#include "v4l2_ioctrl.h"
#include "v4l2_log.h"

#include "cam/cam_config_reader.h"

#include "OSAL_api.h"
#include "amp_client.h"

static char *client_argv[] = { (char *)"client",
                               (char *)"iiop:1.0//127.0.0.1:999/AMP::FACTORY/factory" };
static V4L2_CONTEXT context = {0};

int SynaV4L2_Init(void )
{
    HRESULT result = V4L2_SUCCESS;

    V4L2_API_ENTRY("\n");

    if(context.inited) {
        context.refcnt++;
        result = V4L2_SUCCESS;
        V4L2LOGH("V4L2 Inited\n");
        goto INITED;
    }

    result = MV_OSAL_Init();
    if (result != V4L2_SUCCESS) {
        V4L2LOGE("OSAL init failed(result=0x%x)\n", result);
        result = ERROR_AMPINIT;
        goto ERROR;
    }
    else {
        V4L2LOGD("OSAL init done\n");
    }

    /* Initialize AMP (connect with AMP server) */
    result = AMP_Initialize(2, client_argv, &(context.amp_factory));
    if (result != V4L2_SUCCESS) {
        V4L2LOGE("Can't initialize AMP(result=0x%x)!\n", result);
        result = ERROR_AMPINIT;
        goto ERROR;
    }
    else {
        V4L2LOGD("AMP_Initialize done\n");
    }

    result = v4l2_shm_init();
    if(result != V4L2_SUCCESS) {
        V4L2LOGE("Can't open amp-shm!\n");
        result = ERROR_AMPINIT;
        goto ERROR;
    }
    context.inited = TRUE;
    context.refcnt ++;
INITED:
ERROR:
    V4L2_API_EXIT(result);
    return (int)result;
}

int SynaV4L2_Deinit(void )
{
    HRESULT result = V4L2_SUCCESS;

    V4L2_API_ENTRY("\n");
    if(!context.inited) {
        V4L2LOGH("v4l2 device already deinit, do nothing\n");
        goto exit;
    }
    context.refcnt--;
    if(context.refcnt > 0) {
        V4L2LOGH("v4l2 device refcnt= %d, do nothing\n", context.refcnt);
        goto exit;
    }

    result = v4l2_shm_deinit();
    if (V4L2_SUCCESS != result) {
        V4L2LOGE("Can't close amp-shm!\n");
    }

    result = AMP_Deinitialize();
    if (V4L2_SUCCESS != result) {
        V4L2LOGE("Can't deinitialize AMP successful!\n");
    }

    result = MV_OSAL_Exit();
    if (V4L2_SUCCESS != result) {
        V4L2LOGE("Can't deinitialize osal successful!\n");
    }
    context.inited = FALSE;
    context.refcnt = 0;
exit:
    V4L2_API_EXIT(result);
    return (int)result;
}


int SynaV4L2_GetInfo(unsigned int type, SynaV4l2Info *pInfo) {
    if(type == CAMERA && pInfo!= NULL) {
        bool isConnected = false;
        memset(pInfo, 0, sizeof(SynaV4l2Info));
        pInfo->type = CAMERA;

        cam_read_hardware_config();
        int sensorCount = cam_get_total_sensors();
        for(int idx = 0; idx < sensorCount; idx++){
            int dev, addr;
            cam_get_i2c_addr(idx, &dev, &addr);
            isConnected = cam_is_connected(dev, addr);

            if(isConnected == true) {
                int width, height, bitDepth, orientation;
                cam_get_resolution(idx, &width, &height, &bitDepth);
                cam_get_orientation(idx, &orientation);

                pInfo->data.cam.isAvailable = 1;
                pInfo->data.cam.width = width;
                pInfo->data.cam.height = height;
                pInfo->data.cam.bitDepth = bitDepth;
                pInfo->data.cam.orientation = orientation;
                break;
            }
        }
    }

    return 0;
}

int SynaV4L2_IsAvailable(unsigned int type) {
    bool isConnected = true;

    if(type == CAMERA) {
        cam_read_hardware_config();
        int sensorCount = cam_get_total_sensors();
        for(int idx = 0; idx < sensorCount; idx++){
            int dev, addr;
            cam_get_i2c_addr(idx, &dev, &addr);
            isConnected = cam_is_connected(dev, addr);
            if(isConnected == true)
                break;
        }
    }

    if(!isConnected)
        return 0;

    return 1;
}

int SynaV4L2_Open(void **handle, UINT32 type, UINT32 v4l2_pixfmt, UINT32 flag)
{
    HRESULT result = V4L2_SUCCESS;

    V4L2_API_ENTRY("\n");

    cam_read_hardware_config();
    context.flag = flag;
    result = v4l2_m2m_open(&context, type, v4l2_pixfmt, handle);

    V4L2LOGD("new v4l2 m2m %s instance %p\n", m2mType2str(type), *handle);
    V4L2_API_EXIT(result);
    return (int)result;
}

int SynaV4L2_Close(void *handle)
{
    HRESULT result = V4L2_SUCCESS;

    V4L2_API_ENTRY("\n");
    V4L2LOGD("destroy v4l2 m2m instance %p\n", handle);
    result = v4l2_m2m_close(handle);

    V4L2_API_EXIT(result);
    return (int)result;
}

int SynaV4L2_Ioctl(void *handle, INT32 cmd, void* arg)
{
    HRESULT result = V4L2_SUCCESS;

    V4L2_IOCTRL_ENTRY(cmd);

    result = v4l2_ioctrl_execute(handle, cmd, arg);

    V4L2_IOCTRL_EXIT(cmd, result);
    return (int)result;
}

int SynaV4L2_Poll(void *handle,
                               int poll_device,
                               int* event_pending)
{
    HRESULT result = V4L2_SUCCESS;

    V4L2_API_ENTRY("[poll_device:%d]\n", poll_device);

    result = v4l2_m2m_WaitForPollSem(handle, poll_device, event_pending);

    V4L2_API_EXIT(result);
    return (int)result;
}

int SynaV4L2_SetDevicePollInterrupt(void *handle)
{
    HRESULT result = V4L2_SUCCESS;

    V4L2_API_ENTRY("\n");

    result = v4l2_m2m_SetDevicePollInterrupt(handle);

    V4L2_API_EXIT(result);
    return (int)result;
}

int SynaV4L2_ClearDevicePollInterrupt(void *handle)
{
    HRESULT result = V4L2_SUCCESS;

    V4L2_API_ENTRY("\n");

    result = v4l2_m2m_ClearDevicePollInterrupt(handle);

    V4L2_API_EXIT(result);
    return (int)result;
}

void* SynaV4L2_Mmap(void *handle,
                             void* addr,
                             unsigned int length,
                             INT32 prot,
                             INT32 flags,
                             UINT32 offset)
{
    void *va;

    V4L2_API_ENTRY("\n");

    va = v4l2_buffer_mmap(addr, length, prot, flags, handle, offset);

    V4L2_API_EXIT(va != MAP_FAILED);
    return va;
}

int SynaV4L2_Munmap(void *handle, void* addr, unsigned int  length)
{
    HRESULT result = V4L2_SUCCESS;

    V4L2_API_ENTRY("\n");

    result = v4l2_buffer_munmap(handle, addr, length);

    V4L2_API_EXIT(result);
    return (int)result;
}
