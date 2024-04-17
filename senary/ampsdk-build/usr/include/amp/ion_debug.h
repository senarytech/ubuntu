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

#ifndef _ION_DEBUG_H_
#define _ION_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ION_KERNEL_GID_PATH             "/sys/kernel/debug/shm"

#define ION_PROFILING_POOL_NUM          3

#define PID_NAME_LEN                    16

#define ION_POOL_0_HEAP_ID              6
#define ION_POOL_0_NAME                 8
#define ION_POOL_0_PABASE_IDX           21
#define ION_POOL_0_USED_MEMINFO_IDX     27
#define ION_POOL_0_FREE_MEMINFO_IDX     31
#define ION_POOL_1_HEAP_ID              34
#define ION_POOL_1_NAME                 36
#define ION_POOL_1_PABASE_IDX           49
#define ION_POOL_1_USED_MEMINFO_IDX     55
#define ION_POOL_1_FREE_MEMINFO_IDX     59
#define ION_POOL_2_HEAP_ID              62
#define ION_POOL_2_NAME                 64
#define ION_POOL_2_PABASE_IDX           77
#define ION_POOL_2_USED_MEMINFO_IDX     83
#define ION_POOL_2_FREE_MEMINFO_IDX     87
#define ION_POOL_3_HEAP_ID              90
#define ION_POOL_3_NAME                 92
#define ION_POOL_3_PABASE_IDX           105
#define ION_POOL_3_USED_MEMINFO_IDX     111
#define ION_POOL_3_FREE_MEMINFO_IDX     115




typedef struct SHM_ION_GID {
    unsigned int                uiNo;
    unsigned int                uiGid;
    unsigned int                uiRef;
    unsigned int                uiPid;
    char                        sPsName[PID_NAME_LEN];
    unsigned int                uiTid;
    char                        sTdName[PID_NAME_LEN];
    unsigned int                uiPa;
    unsigned int                uiSize;
    unsigned int                uiAllocLen;
    unsigned int                uiHeapId;
}SHM_ION_GID;


typedef struct SHM_ION_GID_LIST {
    unsigned int                uiTotalCount;
    SHM_ION_GID**               gid_list;
}SHM_ION_GID_LIST;


typedef struct SHM_USAGE_INFO {
    unsigned int                uiTotalSize0;  // Total size of secure & cacheable memocy
    unsigned int                uiTotalSize1;  // Total size of non-secure & cacheable memory.
    unsigned int                uiTotalSize2;  // Total size of non-secure & uncacheabld memory.
#if (ION_PROFILING_POOL_NUM == 4)
    unsigned int                uiTotalSize3;
#endif
    unsigned int                uiCount;
    char                        sTdName[PID_NAME_LEN];
}SHM_USAGE_INFO;


typedef struct ION_MEM_INFO {
    unsigned int                uiHeapId;
    char                        sName[PID_NAME_LEN];
    unsigned int                uiPhyAddr;
    unsigned int                uiUsedSize;
    unsigned int                uiFreeSize;
    unsigned int                uiBlockSize;  // The largest block size that can be allocated.
}ION_MEM_INFO;


typedef struct SHM_USAGE_LIST {
    unsigned int                uiTotalSize;
    unsigned int                uiTotalCount;
    ION_MEM_INFO                stMemInfo[ION_PROFILING_POOL_NUM];
    SHM_USAGE_INFO**            shm_list;
}SHM_USAGE_LIST;



typedef struct SHM_ION_MEM_INFO {
    ION_MEM_INFO                stInfo[ION_PROFILING_POOL_NUM];
}SHM_ION_MEM_INFO;



void ion_dbg_get_gid_usage_start(char* sGidPath);
void ion_dbg_get_num_gid_usage_list(unsigned int* total);
void ion_dbg_get_gid_usage_info(unsigned int index, SHM_USAGE_INFO *pShmInfo);
void ion_dbg_get_ion_mem_info(SHM_ION_MEM_INFO *pMemInfo);
void ion_dbg_get_gid_usage_end();

#ifdef __cplusplus
}
#endif

#endif
