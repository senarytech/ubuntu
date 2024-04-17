/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (C) 2013-2017 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
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
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS", AND
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
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */

#ifndef _RING_BUF_
#define _RING_BUF_

#include "OSAL_api.h"

typedef struct  t_av_ring_buf {
    VOID *m_CmdCellStart;
    INT m_CmdRdIndex;
    INT m_CmdWrIndex;
    INT m_CmdSize;                  // total cell numbers
    INT m_CellSize;                 // Per cell size (in bytes)
    MV_OSAL_HANDLE lock;
} VDEC_QUEUE;                  // reference to com_type.h

VDEC_QUEUE * VdecQueCreate(INT cell_size, INT cmd_size);
VOID VdecQueReset(VDEC_QUEUE *pVdecQue);
VOID VdecQueRemove(VDEC_QUEUE **pVdecQue);
INT VdecQueGetFullness(VDEC_QUEUE *pVdecQue);
VOID VdecQueReadAdvance(VDEC_QUEUE *pVdecQue, INT step);
VOID VdecQueWriteAdvance(VDEC_QUEUE *pVdecQue, INT step);
INT VdecQuePush(VDEC_QUEUE *pVdecQue, VOID *pData);
VOID *VdecQuePop(VDEC_QUEUE *pVdecQue, INT bAdvance);
INT VdecQueGetSize(VDEC_QUEUE *pVdecQue);
VOID *VdecCmdPop(VDEC_QUEUE *pVdecQue, INT bAdvance);
INT VdecCmdPush(VDEC_QUEUE *pVdecQue, VOID *pData);
VOID* VdecQuePeek(VDEC_QUEUE *pVdecQue, INT uiIndex);
VOID* VdecCmdPeek(VDEC_QUEUE *pVdecQue, INT uiIndex);

#endif
