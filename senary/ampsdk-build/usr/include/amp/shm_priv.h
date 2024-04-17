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
//! \file shm_priv.h
//!
//! \brief This file define all private data structure, enum and API for MV
//!        share memory management, it is a lower level of amp_shm.
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef _MV_OSAL_SHM_PRIV_H
#define _MV_OSAL_SHM_PRIV_H

///////////////////////////////////////////////////////////////////////////////
//! [Part1] Header file include
///////////////////////////////////////////////////////////////////////////////
#include "OSAL_api.h"
#include "shm_type.h"
#include "ion_debug.h"
///////////////////////////////////////////////////////////////////////////////
//! [Part2] Declaration of external variables or functions
///////////////////////////////////////////////////////////////////////////////
//#define BACKTRACE_RECORD                1

#if defined(AMP_RELEASE_BUILD)
#define shm_printf(l, ...)
#else
#if defined(__ANDROID__) || defined(LOGCAT)
#include <log/log.h>
#define MODULE_SHM          0x44
#define LOG_HSZ             256

#define shm_printf(l, ...)     do {                             \
        if (mv_shm.cbLog != NULL) {                             \
            char head[LOG_HSZ];                                 \
            snprintf(head,LOG_HSZ,"Amp%d@@[MODULE_SHM:%s():%d]",l, __FUNCTION__, __LINE__);     \
            mv_shm.cbLog(MODULE_SHM, l, head, __VA_ARGS__);     \
        }  \
    } while (0)
#else
#define shm_printf(l, ...)      \
    do{                         \
        if (l < SHM_LOG_HIGH) {  \
            printf(__VA_ARGS__);\
            printf("\n"); \
        } \
    } while(0)
#endif
#endif


#define MV_OSAL_SHM_API_ENTER(k)    //shm_printf(SHM_LOG_INFO, "enter")

#define MV_OSAL_SHM_API_EXIT(k, r)  //shm_printf(SHM_LOG_INFO, "leave with (0x%x).", r)

#define MV_OSAL_SHM_LOG(l, ...)    do {                 \
        shm_printf(l, __VA_ARGS__);                     \
         if (l < SHM_LOG_HIGH) {                        \
            ion_dump_backtrace("MV_OSAL_SHM");          \
        }                                               \
    } while (0)


#define MV_OSAL_SHM_LOCK()      do {                    \
        HRESULT ret;                                    \
        ret = MV_OSAL_Mutex_Lock(mv_shm.hCacheLock);    \
        assert(ret == SUCCESS);                         \
    } while (0)

#define MV_OSAL_SHM_UNLOCK()    do {                    \
        HRESULT ret;                                    \
        ret = MV_OSAL_Mutex_Unlock(mv_shm.hCacheLock);  \
        assert(ret == SUCCESS);                         \
    } while (0)
HRESULT cache_performance_monitor(UINT32 action, UINT32 uiSize, MV_TimeSpec_t* ts_delta);

#define MV_OSAL_SHM_PERF_START()                        \
        MV_TimeSpec_t time_start, time_2, ts_delta; \
        if (mv_shm.stDbgCtrl.pCtrlInfo->bCachePerfEnable) { \
            clock_gettime(CLOCK_MONOTONIC, &time_start);    \
        }


#define MV_OSAL_SHM_PERF_END(action)                    \
        if (mv_shm.stDbgCtrl.pCtrlInfo->bCachePerfEnable) { \
            clock_gettime(CLOCK_MONOTONIC, &time_2);      \
            MV_TimeSpec_Normaized(&ts_delta,                \
                                time_2.tv_sec-time_start.tv_sec,  \
                                time_2.tv_nsec-time_start.tv_nsec);   \
            cache_performance_monitor(action, uiSize, &ts_delta);   \
        }

enum HRESULT_VALUE {
    SUCCESS         = 0x0000,   /**< Success. */
    ERR_NOTIMPL     = 0x0001,   /**< Function/feature not implementated. */
    ERR_ERRPARAM    = 0x0002,   /**< Function parameter error. */
    ERR_NOMEM       = 0x0003,   /**< Not enough memory. */
    ERR_NOSHM       = 0x0004,   /**< Not enough share memory. */
    ERR_TIMEOUT     = 0x0005,   /**< Operation timeout. */
    ERR_ERRSYSCALL  = 0x0006,   /**< Syscall error. */
    ERR_IOFAIL      = 0x0007,   /**< Perepheral IO fail. */
    ERR_EVENTFULL   = 0x0008,   /**< Event queue full. */
    ERR_HWBUSY      = 0x0009,   /**< Hardware busy. */
    ERR_HWFAIL      = 0x000A,   /**< Hardware fail. */
    ERR_OSALFAIL    = 0x000B,   /**< OSAL fail. */
    ERR_NOSWRSC     = 0x000C,   /**< Not enough software resource. */
    ERR_NOHWRSC     = 0x000D,   /**< Not enough hardware resource. */
    ERR_SWSTATEWRONG= 0x000E,   /**< Don't permit in current software state. */
    ERR_HWSTATEWRONG= 0x000F,   /**< Don't permit in current hardware state. */
    ERR_RCPERROR    = 0x0010,   /**< RPC error. */
    ERR_SWMODEWRONG = 0x0011,   /**< Software mode wrong. */
    ERR_HWMODEWRONG = 0x0012,   /**< Hardware mode wrong. */
    ERR_CONNCLEARED = 0x0013,   /**< Connection has been cleared error. */
    ERR_RANGEPARAM  = 0x0014,   /**< Parameter is out of range. */
    ERR_CANCELLED   = 0x0015,   /**< Operation is cancelled. */
    ERR_BDERROR     = 0x0016,   /**< BD data error. */

    ERR_PRIVATE_BASE= 0x1000,   /**< The module private error value base. */
};

#define SHM_TRACE_CS_NUM        (5)

#define MV_OSAL_SHM_CACHE_BLOCK(shm)    ((shm) >> MV_OSAL_SHM_CACHE_BLOCK_BITS)
#define MV_OSAL_SHM_CACHE_INDEX(shm)    ((shm) & MV_OSAL_SHM_CACHE_BLOCK_MASK)

#define SHM_PID_LIST_NUM        10
#define SHM_ION_POOL_NUM        4
typedef struct SHM_PID_INFO {
    unsigned int                pid;
    char                        name[PID_NAME_LEN];
    unsigned int                uiTotalSize;
    unsigned int                uiPoolSize[SHM_ION_POOL_NUM];
} SHM_PID_INFO;


#define MV_OSAL_SHM_CACHE_BLOCK_BITS    13
#define MV_OSAL_SHM_CACHE_BLOCK_SIZE    (1 << MV_OSAL_SHM_CACHE_BLOCK_BITS)
#define MV_OSAL_SHM_CACHE_BLOCK_MASK    (MV_OSAL_SHM_CACHE_BLOCK_SIZE - 1)
#define MV_OSAL_SHM_CACHE_MAX           0x10000
#define MV_OSAL_SHM_CACHE_BLOCKS        (MV_OSAL_SHM_CACHE_MAX >>           \
                                     MV_OSAL_SHM_CACHE_BLOCK_BITS)

/* Registry file record (need cache line alignment?) */
typedef struct MV_OSAL_SHM_RECORD {
    UINT32                      uiMagic;          /* Magic code */
    MV_OSAL_SHM_TYPE            eMemType;         /* SHM type */
    UINT32                      uiSize;           /* SHM size */
    BOOL                        bTailAdd;         /* SHM tail added */
    MV_OSAL_SHM_INFO            stShmInfo;        /* SHM info */
} MV_OSAL_SHM_RECORD;

typedef struct SHM_REC_INFO {
    int                         hShmFile;    /* Globle SHM debug file */
    MV_OSAL_SHM_RECORD          *pShmInfo;
} SHM_REC_INFO;

typedef struct SHM_INFO {
    unsigned int                hShm;
    unsigned int                eType;
    unsigned int                phyAddr;
    unsigned int                uiSize;
    unsigned int                uiAlloclen;
    unsigned int                uiDupCnt;
    unsigned int                uiClientPid;
} SHM_INFO;

#ifdef BACKTRACE_RECORD
#define SHM_INFO_ITEM_SIZE      1024
#else
#define SHM_INFO_ITEM_SIZE      256
#endif

#define BACKTRACE_SIZE          (SHM_INFO_ITEM_SIZE - sizeof(SHM_INFO))
typedef struct SHM_INFO_BLOCK {
    SHM_INFO                    head;
    char                        callstack[BACKTRACE_SIZE];
} SHM_INFO_BLOCK;

typedef struct SHM_DBG_CALLER {
    int                         hShmDbgFile;    /* Globle SHM debug file */
    SHM_INFO_BLOCK              *pShmDbgInfo;
} SHM_DBG_CALLER;

typedef struct SHM_TRACE_CALLER {
    int                         hShmTraceFile;  /* Globle SHM trace file */
} SHM_TRACE_CALLER;

typedef struct SHM_DBG_CTRL_INFO {
    BOOL                        bBktPrtDisable;
    BOOL                        bTailEnable;
    BOOL                        bCachePerfEnable;
} SHM_DBG_CTRL_INFO;


typedef struct SHM_DBG_CTRL {
    int                         hDbgCtrlFile;    /* Globle SHM debug file */
    int                         hShmPerfFile;
    SHM_DBG_CTRL_INFO           *pCtrlInfo;
} SHM_DBG_CTRL;

/* Local cache item (as short as posible) */
typedef struct MV_OSAL_SHM_CACHE {
    struct ion_handle           *pIon;            /* SHM ion_handle at allocator*/
    UINT32                      isOrig;          /* SHM is allocated/imported */
    void                        *pVa;             /* SHM virtual address */
    void                        *pPa;             /* SHM PHY address */
    void                        *pPgta;           /* SHM PHY address of page table*/
    UINT32                      mem_id;          /* SHM MEM ID in TEE*/
    UINT32                      uiRefCnt;        /* Local ref counter */
    uint8_t                     is_pt_created;   /* SHM is page table created */
} MV_OSAL_SHM_CACHE;

typedef void (*SHM_PRINT)(UINT32 mod, UINT32 level, const char *pHead, const char *pMsg, ...);

typedef struct SHM_MEM_INFO {
    BOOL                        bEnable;
    SHM_ION_MEM_INFO            stIONInfo;
} SHM_MEM_INFO;


typedef struct SHM_MEM_MAX_INFO {
    int                         hShmMemFile;    /* Globle SHM debug file */
    SHM_MEM_INFO                *pMemInfo;
} SHM_MEM_MAX_INFO;

typedef struct SHM_CACHE_PERF_INFO {
    UINT32                      uTotalCnt;
    UINT32                      uTotalTime;
    UINT64                      uTotalSize;
} SHM_CACHE_PERF_INFO;


typedef enum __shm_cache_group__ {
    SHM_CACHE_SIZE_16K,
    SHM_CACHE_SIZE_64K,
    SHM_CACHE_SIZE_128K,
    SHM_CACHE_SIZE_512K,
    SHM_CACHE_SIZE_2X1024K,
    SHM_CACHE_SIZE_16X1024K,
    SHM_CACHE_SIZE_32X1024K,
    SHM_CACHE_SIZE_MAX
} SHM_CACHE_GROUP;


typedef struct SHM_PERF_INFO {
    SHM_CACHE_PERF_INFO         stInfo[/*SHM_CACHE_ACTION_MAX*/2][SHM_CACHE_SIZE_MAX];
} SHM_PERF_INFO;

typedef struct __mv_shm___{
    SHM_REC_INFO                stRecInfo;
    SHM_DBG_CALLER              stDbgCaller;
    SHM_TRACE_CALLER            stTraceCaller;
    SHM_DBG_CTRL                stDbgCtrl;
    int                         iIonFd;         /* ion driver device fd */
    int                         iHeapFd;        /* ion driver device fd */
    MV_OSAL_HANDLE_MUTEX_t      hCacheLock;     /* Lock for local cache */
    UINT32                      uiCacheNr;      /* Local cache number */
    MV_OSAL_SHM_CACHE           *pCache[MV_OSAL_SHM_CACHE_BLOCKS]; /* Local cache */
    SHM_PRINT                   cbLog;
    SHM_MEM_MAX_INFO            stMemMaxInfo;
    SHM_PERF_INFO               *pstPerf;
} MV_OSAL_SHM_ST;


#define SHM_TRACE_CS_SIZE       (256)
#define SHM_TRACE_RECORD_NUM    (50)
#define SHM_TRACE_MAGIC_NUM     (0xBBBEEFFF)
#define SHM_BD_NAME_LEN         (8)


typedef struct __shm_trace_element__ {
    SHM_TRACE_ACTION    eAction;
    UINT32  uTime;
    UINT32  hShm;
    UINT32  eType;
    UINT32  uiSize;
    UINT32  isValid;
    UINT32  uiMagic;
    UINT32  uiRefCnt;
    void *  VA;

    UINT32  uTagNum;
    UINT32  uTotalSpace;
    UINT32  uDataSpace;
    CHAR    BDName[SHM_BD_NAME_LEN];
} SHM_TRACE_ELEMENT;

typedef struct __shm_trace_cs__ {
    UINT32  index;
    CHAR    chCS[SHM_TRACE_CS_NUM][SHM_TRACE_CS_SIZE];
} SHM_TRACE_CS;


typedef struct __shm_trace_info__ {
    UINT32              MagicNum;
    UINT32              RdOff;
    UINT32              WrOff;
    SHM_TRACE_ELEMENT   allocate_element;
    SHM_TRACE_ELEMENT   release_element;
    SHM_TRACE_ELEMENT   element[SHM_TRACE_RECORD_NUM];
    SHM_TRACE_CS        CS[SHM_TRACE_ACTION_MAX];
} SHM_TRACE_INFO;


typedef enum {
    SHM_LOG_FATAL = 0,          /**< For errors that system can not continue. */
    SHM_LOG_ERROR,              /**< For errors that system can still work. */
    SHM_LOG_HIGH,               /**< High level debug info. */
    SHM_LOG_MID,                /**< Middle level debug info. */
    SHM_LOG_LOW,                /**< Low level debug info. */
    SHM_LOG_INFO,               /**< Periodical and trivial debug info. */
    SHM_LOG_USER1,              /**< User specific debug info. */
    SHM_LOG_USER2,              /**< User specific debug info. */
    SHM_LOG_LEVEL_MAX = 8,      /**< MAX level should not exceed 7. */
} SHM_LOG_LEVEL;


#define UNUSED __attribute__((unused))



///////////////////////////////////////////////////////////////////////////////
//! [Part4] Declaration of local/global variables
///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif
///////////////////////////////////////////////////////////////////////////////
//! [Part5] Declarations of internal SHM functions
///////////////////////////////////////////////////////////////////////////////
HRESULT doFileOpen(int* fd, char* sName, int length);
HRESULT doFileClose();

void doDebugRemove(unsigned int key);
void doDebugInsert(unsigned int key, unsigned int type,
    unsigned int size, unsigned int pid, const char* fName, unsigned int fLine);
void doDebugDump(BOOL bFile, FILE *pFile);
void doTraceDump(int file, FILE *pFile);
void doPoolMonitor();

HRESULT doCacheInsert(UINT32 shm, MV_OSAL_SHM_CACHE **pShmItem);
HRESULT doCacheSearch(UINT32 shm, MV_OSAL_SHM_CACHE **pShmItem);
HRESULT doCacheRemove(UINT32 shm);

HRESULT doRegistryinit(int isServer, MV_OSAL_SHM_DRV_CFG *pSHM_Drv_Cfg);
HRESULT doRegistryClear(int shm);
HRESULT doRegistryWrite(int shm, MV_OSAL_SHM_RECORD *record);
HRESULT doRegistryRead(int shm, MV_OSAL_SHM_RECORD *record);

HRESULT doDumpStatus(char *sPath);
BOOL isTailType(MV_OSAL_SHM_TYPE eType);
HRESULT doTailChecking(MV_OSAL_SHM_HANDLE hShm, void *pVa);
HRESULT doUpdateType2Pool(BOOL bHWPool);

int doGetType2Pool(MV_OSAL_SHM_TYPE eType);

const char *doType2Str(UINT32 type);
const char *doAction2Str(UINT32 action);


///////////////////////////////////////////////////////////////////////////////
//! [Part6] Global functions
///////////////////////////////////////////////////////////////////////////////




#ifdef __cplusplus
}
#endif


#endif  /* _MV_OSAL_SHM_PRIV_H_ */
