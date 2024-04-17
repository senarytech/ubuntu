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
#ifndef SYNA_V4L2_MISC_H
#define SYNA_V4L2_MISC_H

#include <linux/videodev2.h>

#include "v4l2_types.h"
#include "amp_client.h"
#include "std_list.h"

//#define EN_DATA_DUMP
#ifdef EN_DATA_DUMP
#define DATA_DIR            "/tmp/"
#endif

typedef struct __v4l2_context__ {
    UINT32 inited;
    AMP_FACTORY amp_factory;
    UINT32 flag;
    UINT32 refcnt;
} V4L2_CONTEXT;

//list helper function
BOOL list_is_empty(std_list *list);
UINT list_size(std_list *list);

void show_v4l2fmt(struct v4l2_format *fmt);
void dump_buff_cntx(struct v4l2_buffer *buf, const char *fun, UINT32 line);
void dump_BDInfo(AMP_BD_HANDLE hBD,const char *fun, UINT32 line);
void dump_StreamBuff(void *fh, struct v4l2_buffer *buf);
void dump_PortBD(void *fh, AMP_BD_HANDLE hBD);
HRESULT BD_GetFrameInfo(AMP_BD_HANDLE hBD, AMP_BGTAG_FRAME_INFO **ppTag);
HRESULT BD_GetMemInfo(AMP_BD_HANDLE hBD, AMP_BDTAG_MEMINFO **ppTag);
BOOL is_validDispBD(AMP_BD_HANDLE hBD);

char* cmd2str(UINT32 cmd);
char* fmt2str(UINT32 fmt);
char* buf2str(UINT32 type);
char* event2str(UINT32 type);
char* state2str(UINT32 state);
char* m2mType2str(UINT32 type);
char* mem2str(UINT32 type);

UINT32 align(UINT32 num, UINT32 block);

#define DUMP_BUFF_CNXT(buf)      //dump_buff_cntx(buf, __FUNCTION__, __LINE__)
#define DUMP_BD_MEMINFO(hBD)     //dump_BDInfo(hBD, __FUNCTION__, __LINE__)

#endif   //SYNA_V4L2_MISC_H
