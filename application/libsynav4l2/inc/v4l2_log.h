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
////////////////////////////////////////////////////////////////////////////////
//! \file v4l2_log.h
//! \brief This file defined common types of V4L2 log.
//!
//! Purpose:
//!
//! Version, Date and Author :
//!   V 1.00,    March 2020,    Weizhao Jiang
//!  Note:
////////////////////////////////////////////////////////////////////////////////


#ifndef __V4L2_LOG_H__
#define __V4L2_LOG_H__
#include "amp_client.h"

//#define V4L2_TIMING_PROFILE
#define V4L2_DEBUG
//#define AMP_LOGCAT

#ifdef V4L2_DEBUG
#if defined(ANDROID)
#include <log/log.h>
#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "V4L2"
#define V4L2LOGF    ALOGE
#define V4L2LOGE    ALOGE
#define V4L2LOGH    ALOGW
#define V4L2LOGD    ALOGD
#elif defined(AMP_LOGCAT)
#define V4L2LOGF    AMPLOGF
#define V4L2LOGE    AMPLOGE
#define V4L2LOGH    AMPLOGH
#define V4L2LOGD    AMPLOGD
#else       //! AMP_LOGCAT
#define V4L2LOGF(...)    do{printf("%25s() :%6d  ", __FUNCTION__, __LINE__); printf(__VA_ARGS__);} while(0)
#define V4L2LOGE(...)    do{printf("%25s() :%6d  ", __FUNCTION__, __LINE__); printf(__VA_ARGS__);} while(0)
#define V4L2LOGH(...)    do{printf("%25s() :%6d  ", __FUNCTION__, __LINE__); printf(__VA_ARGS__);} while(0)
#define V4L2LOGD(...)    //do{printf("%25s() :%6d  ", __FUNCTION__, __LINE__); printf(__VA_ARGS__);} while(0)
#endif      //! AMP_LOGCAT

#else       //! V4L2_DEBUG
#define V4L2LOGF
#define V4L2LOGE
#define V4L2LOGH
#define V4L2LOGD
#endif      //! V4L2_DEBUG

#ifdef V4L2_TIMING_PROFILE
#define TIMING_PROFILE_BEGIN    struct timespec tm_start, tm_end, ts_delta; \
                                AMP_GetCurrentTime(&tm_start);
#else
#define TIMING_PROFILE_BEGIN
#endif


#define V4L2_API_ENTRY(...)     TIMING_PROFILE_BEGIN \
                                V4L2LOGD("[API_TRACE] Enter "__VA_ARGS__)

#define V4L2_IOCTRL_ENTRY(cmd)  TIMING_PROFILE_BEGIN \
                                V4L2LOGD("[API_TRACE] %s\tEnter\n", cmd2str(cmd))

#ifdef V4L2_TIMING_PROFILE
#define V4L2_API_EXIT(ret)      do {                                        \
                                    AMP_GetCurrentTime(&tm_end);            \
                                    MV_TimeSpec_Normaized(&ts_delta,        \
                                    tm_end.tv_sec-tm_start.tv_sec,          \
                                    tm_end.tv_nsec-tm_start.tv_nsec);       \
                                    V4L2LOGH("[API_TRACE] Exit(ret=0x%x) take %ld(s), %ld(us)\n", ret, ts_delta.tv_sec, ts_delta.tv_nsec/1000);  \
                                } while(0)
#define V4L2_IOCTRL_EXIT(cmd, ret) do {                                        \
                                    AMP_GetCurrentTime(&tm_end);            \
                                    MV_TimeSpec_Normaized(&ts_delta,        \
                                    tm_end.tv_sec-tm_start.tv_sec,          \
                                    tm_end.tv_nsec-tm_start.tv_nsec);       \
                                    V4L2LOGH("[API_TRACE] %s\tExit(ret=0x%x) take %ld(s), %ld(us)\n", cmd2str(cmd), ret, ts_delta.tv_sec, ts_delta.tv_nsec/1000);  \
                                } while(0)
#else
#define V4L2_API_EXIT(ret)      do {                                        \
                                    V4L2LOGD("[API_TRACE] Exit(ret=0x%x)\n", ret);  \
                                } while(0)
#define V4L2_IOCTRL_EXIT(cmd, ret) do {                                        \
                                    V4L2LOGD("[API_TRACE] %s\tExit(ret=0x%x)\n", cmd2str(cmd), ret);  \
                                } while(0)
#endif
#define V4L2_DATAFLOW_TRACE(...) V4L2LOGD("[DATAFLOW_TRACE] "__VA_ARGS__)

#define ASSERT_RET_SHOW(rt, fmt)               \
      if (rt != SUCCESS) {                     \
         V4L2LOGE( fmt " (ret=%x)\n", rt);      \
         return rt;                            \
     }

#define CHECK_EQ(s , d)       \
        if((s) != (d)) {          \
            return;           \
        }

#endif
