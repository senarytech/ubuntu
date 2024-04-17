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
//! \file amp_mem_com.h
//!
//! \brief This file define all common data structure, enum and API for AMP
//!        share memory management.
//!
//! Purpose:
//!
//!
//! Version, Date and Author :
//!   V 1.00,    December 25 2012,   Weizhao Jiang
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef __AMP_MEM_COM_H__
#define __AMP_MEM_COM_H__

#include "OSAL_api.h"
#include "amp_types.h"
#include "amp_malloc.h"
#include "isl/amp_shm.h"
#include "isl/amp_logger.h"
#include "isl/amp_buf_desc.h"

/*******************************************************************************
    Macro Defined
*/

#define AMP_SHM_DEBUG       (1)

/**
 *  Below are debug log  definition
 */
#define SHM_SRV_LOG_TAG      "[SHM/BUF:Server] "
#define SHM_USR_LOG_TAG      "[SHM/BUF:User] "

#if AMP_SHM_DEBUG
#if defined(BUILDIN_AMP_CORE)
#define SHM_FATAL_LOG(...)      \
        AMP_Log(MODULE_MEM, AMP_LOG_FATAL, SHM_SRV_LOG_TAG __VA_ARGS__)
#define SHM_ERROR_LOG(...)      \
        AMP_Log(MODULE_MEM, AMP_LOG_ERROR, SHM_SRV_LOG_TAG __VA_ARGS__)
#define SHM_HIGH_LOG(...)       \
        AMP_Log(MODULE_MEM, AMP_LOG_HIGH, SHM_SRV_LOG_TAG __VA_ARGS__)
#define SHM_DGB_LOG(...)        \
        AMP_Log(MODULE_MEM, AMP_LOG_MID, SHM_SRV_LOG_TAG __VA_ARGS__)
#define SHM_TRACE_LOG(...)        \
        AMP_Log(MODULE_MEM, AMP_LOG_LOW, SHM_SRV_LOG_TAG __VA_ARGS__)
#define SHM_INFO_LOG(...)       \
        AMP_Log(MODULE_MEM, AMP_LOG_INFO, SHM_SRV_LOG_TAG __VA_ARGS__)
#define SHM_API_PARAM_LOG(...)  \
      AMP_Log(MODULE_MEM, AMP_LOG_INFO, SHM_SRV_LOG_TAG "@@@Param:" __VA_ARGS__)
#else
#define SHM_FATAL_LOG(...)      \
        AMP_Log(MODULE_MEM, AMP_LOG_FATAL, SHM_USR_LOG_TAG __VA_ARGS__)
#define SHM_ERROR_LOG(...)      \
        AMP_Log(MODULE_MEM, AMP_LOG_ERROR, SHM_USR_LOG_TAG __VA_ARGS__)
#define SHM_HIGH_LOG(...)       \
        AMP_Log(MODULE_MEM, AMP_LOG_HIGH, SHM_USR_LOG_TAG __VA_ARGS__)
#define SHM_DGB_LOG(...)        \
        AMP_Log(MODULE_MEM, AMP_LOG_MID, SHM_USR_LOG_TAG __VA_ARGS__)
#define SHM_TRACE_LOG(...)        \
        AMP_Log(MODULE_MEM, AMP_LOG_LOW, SHM_USR_LOG_TAG __VA_ARGS__)
#define SHM_INFO_LOG(...)       \
        AMP_Log(MODULE_MEM, AMP_LOG_INFO, SHM_USR_LOG_TAG __VA_ARGS__)
#define SHM_API_PARAM_LOG(...)  \
      AMP_Log(MODULE_MEM, AMP_LOG_INFO, SHM_USR_LOG_TAG "@@@Param:" __VA_ARGS__)
#endif

#else
#define SHM_FATAL_LOG(...)      do {} while(0)
#define SHM_ERROR_LOG(...)      do {} while(0)
#define SHM_HIGH_LOG(...)       do {} while(0)
#define SHM_DGB_LOG(...)        do {} while(0)
#define SHM_TRACE_LOG(...)      do {} while(0)
#define SHM_INFO_LOG(...)       do {} while(0)
#define SHM_API_PARAM_LOG(...)  do {} while(0)
#endif
/**
 *  SHM&BUF API trace log
 */
#define SHM_API_ENTER()  \
    { \
        SHM_INFO_LOG("Enter %s()\n", __FUNCTION__); \
    }
#define SHM_API_EXIT(ret)  \
    { \
        if(HRESULT_VALUE(ret) == SUCCESS) {    \
            SHM_INFO_LOG("Exit %s() without error\n", __FUNCTION__, \
                                        HRESULT_VALUE(ret)); \
        }   \
        else { \
            SHM_ERROR_LOG("Exit %s() with error(0x%x)\n", __FUNCTION__,   \
                                        HRESULT_VALUE(ret)); \
        } \
    }

/**
 *  Return code definition
 */
#define SHM_RET_CODE(value)  ((UINT32)value == (UINT32)SUCCESS ? value : HRESULT_GEN(MEM, (value)))
#define BUF_RET_CODE(value)  ((UINT32)value == (UINT32)SUCCESS ? value : HRESULT_GEN(BUF, (value)))

#define NULL_POINTER(ptr)   (NULL == (ptr))

#define API_TIMING_DEBUG    0

#define SHM_CHECK_PARAM(x)    do {                          \
        if (!(x)) {                                         \
            SHM_ERROR_LOG("Parameter check (%s) failed\n", #x);   \
            ion_dump_backtrace("AMP_BD: ");                 \
            return SHM_RET_CODE(ERR_ERRPARAM);              \
        }                                                   \
    } while(0)


#if(API_TIMING_DEBUG == 1)
#define TIME_DEFINITION       struct timespec tm_start, tm_end, ts_delta;
#define GET_START_TIME        AMP_GetCurrentTime(&tm_start);
#define PRINT_API_PERIOD      {                                         \
                AMP_GetCurrentTime(&tm_end);                            \
                MV_TimeSpec_Normaized(&ts_delta,                        \
                        tm_end.tv_sec-tm_start.tv_sec,                  \
                        tm_end.tv_nsec-tm_start.tv_nsec);               \
                SHM_HIGH_LOG("%s() take %d ns\n", __FUNCTION__,           \
                        ts_delta.tv_nsec); \
           }
#else
#define TIME_DEFINITION     while(0) {}
#define GET_START_TIME      while(0) {}
#define PRINT_API_PERIOD    while(0) {}
#endif

#define AMP_SHM_CACHE_BLOCK_BITS    13
#define AMP_SHM_CACHE_BLOCK_SIZE    (1 << AMP_SHM_CACHE_BLOCK_BITS)
#define AMP_SHM_CACHE_BLOCK_MASK    (AMP_SHM_CACHE_BLOCK_SIZE - 1)
#define AMP_SHM_CACHE_MAX           0x10000
#define AMP_SHM_CACHE_BLOCKS        (AMP_SHM_CACHE_MAX >>           \
                                     AMP_SHM_CACHE_BLOCK_BITS)
#define AMP_SHM_CACHE_BLOCK(shm)    ((shm) >> AMP_SHM_CACHE_BLOCK_BITS)
#define AMP_SHM_CACHE_INDEX(shm)    ((shm) & AMP_SHM_CACHE_BLOCK_MASK)


/*******************************************************************************
    Data structure Defined
*/

typedef enum __ret__ {
    ERR_PTR             = ERR_PRIVATE_BASE | 0x1,
    ERR_NODE            = ERR_PRIVATE_BASE | 0x2,
    ERR_FAIL            = ERR_PRIVATE_BASE | 0x3,
    ERR_SHM             = ERR_PRIVATE_BASE | 0x4,
    ERR_SIZE            = ERR_PRIVATE_BASE | 0x5,
    ERR_TAG             = ERR_PRIVATE_BASE | 0x6,
    ERR_CHAIN_EMPTY     = ERR_PRIVATE_BASE | 0x7,
    ERR_POOL_HANDLE     = ERR_PRIVATE_BASE | 0x8,
    ERR_POOL_OPEN       = ERR_PRIVATE_BASE | 0x9,
    ERR_POOL_CLOSE      = ERR_PRIVATE_BASE | 0xa,
    ERR_POOL_ALLOC      = ERR_PRIVATE_BASE | 0xb,
    ERR_POOL_Free       = ERR_PRIVATE_BASE | 0xc,
} SHM_RET_CODE;


#endif /* __AMP_MEM_COM_H__ */
