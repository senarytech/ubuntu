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

////////////////////////////////////////////////////////////////////////////////
//! \file isl/amp_event_queue.h
//!
//! \brief This file declares all common data structure, enum and API for AMP
//!        event and event queue.
//!
//! Purpose:
//!
//!
//! Version, Date and Author :
//!   V 1.00,    Dec 17 2012,    Ye Liu
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef __AMP_TIME_H__
#define __AMP_TIME_H__

#include <time.h>
#include "GaloisTime.h"

typedef struct {
    UINT32 record_ts;
    UINT32 record_data;
} TOP_STATICS_ITEM;

#define DEFAULT_TOP_STATICS_ITEMS               512

#if defined(ANDROID) && !defined(AMP_RELEASE_BUILD)
#define AMP_API_TIMING_DEBUG                    1
#else
#define AMP_API_TIMING_DEBUG                    0
#endif
#if(AMP_API_TIMING_DEBUG == 1)
#define AMP_API_TIME_DEFINITION()               struct timespec tm_start, tm_end, ts_delta;
#define AMP_API_GET_START_TIME()                AMP_GetCurrentTime(&tm_start);
#define AMP_API_PRINT_PERIOD(_mod,_lvl)   do {                                 \
                AMP_GetCurrentTime(&tm_end);                                   \
                MV_TimeSpec_Normaized(&ts_delta,                               \
                        tm_end.tv_sec-tm_start.tv_sec,                         \
                        tm_end.tv_nsec-tm_start.tv_nsec);                      \
                AMPLOG(_mod, _lvl, "[Timing Trace] take %ld(s), %ld(us)\n",    \
                        ts_delta.tv_sec, ts_delta.tv_nsec/1000);               \
    } while (0)
#else
#define AMP_API_TIME_DEFINITION()               while(0) {}
#define AMP_API_GET_START_TIME()                while(0) {}
#define AMP_API_PRINT_PERIOD(_mod,_lvl)         while(0) {}
#endif


typedef struct {
    UINT32 last_record_ts;
    UINT32 last_interval_ts;

    INT32 loop_max_ms;
    INT32 loop_min_ms;

    INT32 record_max_5s;
    INT32 record_min_5s;
    UINT32 record_max_5s_offset;
    UINT32 record_min_5s_offset;
    UINT32 record_cnt_5s;

    UINT32 loop_cnt_total;

    TOP_STATICS_ITEM a[DEFAULT_TOP_STATICS_ITEMS];
    UINT32 loop_wr_off;
    UINT32 loop_rd_off;

} AMP_TOP;

VOID AMP_GetCurrentTime(MV_TimeSpec_t *tm);

UINT32 AMP_GetCurrentTimeMS();

VOID AMP_Time_GenStatis(AMP_TOP *statis, UINT32 delay, UINT32 start, CHAR *usr_data);

#endif  //__AMP_TIME_H__
