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

#ifndef SYNA_V4L2_API_H
#define SYNA_V4L2_API_H

typedef struct _synaV4l2CamInfo {
    unsigned int isAvailable;
    unsigned int width;
    unsigned int height;
    unsigned int bitDepth;
    unsigned int orientation;
}SynaV4l2CamInfo;

typedef struct _synaV4l2Info {
    unsigned int type;
    union {
        SynaV4l2CamInfo cam;
    }data;
}SynaV4l2Info;

int SynaV4L2_Init(void );
int SynaV4L2_IsAvailable(unsigned int type);
int SynaV4L2_GetInfo(unsigned int type, SynaV4l2Info *pInfo);
int SynaV4L2_Deinit(void );
int SynaV4L2_Open(void **handle, unsigned int type,
                        unsigned int v4l2_pixfmt, unsigned int flag);
int SynaV4L2_Close(void * handle);
int SynaV4L2_Ioctl(void * handle, int cmd, void* arg);
int SynaV4L2_Poll(void * handle,
                               int poll_device,
                               int* event_pending);
int SynaV4L2_SetDevicePollInterrupt(void * handle);
int SynaV4L2_ClearDevicePollInterrupt(void * handle);
void* SynaV4L2_Mmap(void * handle,
                             void* addr,
                             unsigned int length,
                             int prot,
                             int flags,
                             unsigned int offset);
int SynaV4L2_Munmap(void * handle, void* addr, unsigned int length);
#endif      //!SYNA_V4L2_API_H
