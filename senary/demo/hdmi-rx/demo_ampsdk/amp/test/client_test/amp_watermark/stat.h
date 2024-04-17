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
#ifndef _STAT_H_
#define _STAT_H_

#include <stdbool.h>

#include "amp_types.h"

typedef struct SEQ_LIST {
    struct timeval seq;
    struct SEQ_LIST *prev;
    struct SEQ_LIST *next;
} SEQ_LIST_ST;

typedef struct {
    SEQ_LIST_ST *seqHead;
    SEQ_LIST_ST *stSeqList;
    SEQ_LIST_ST *seqTail;
    struct timeval startTime;
    UINT32 uiIdx;
    UINT32 uiReadIdx;
    double fEncTotalTime;
    double fEncOneFrameTime;
    double fEncOneFrameTimeAvg;
    double fEncOneFrameTimeMax;
    double fEncOneFrameTimeMin;
    double fAvgFPS;
    double fMinFPS;
    double fMaxFPS;
    char log_name[128];
    FILE *fp;
} Stat_T;

void stat_tic(Stat_T *p);
bool stat_summary(Stat_T *p);
Stat_T *stat_create(const char *log_name);
void stat_destroy(Stat_T *p);

#endif
