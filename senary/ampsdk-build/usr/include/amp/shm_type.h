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

#ifndef __shm_type_h__
#define __shm_type_h__

#include <stdint.h>

/* It is used to enhance the print of shm allocatoin status when allocate failed,
   the value should be true as default for user-debug version and false for release */
/* To avoid the big maintaining effort to fix symbol errors with prebuilt libs and bins after
   enable SHM_DEBUG_ENHANCEMENT, so we just enable it for Android project currently */
#if !defined(AMP_RELEASE_BUILD)
#define SHM_DEBUG_ENHANCEMENT           1
#else
#define SHM_DEBUG_ENHANCEMENT           0
#endif

/**< Invalidate MV share memory pool. */
#define MV_OSAL_SHM_INVALIDATE          0

/** \def MV_OSAL_SHM_HANDLE_INVALIDATE
 *
 *  \brief Invalidate AMP share memory handle.
 *
 *  \deprecated MV_OSAL_SHM_HANDLE_INVALID indicate the shm handle is invalid.
 *              This version of the library still contains a backward-compatible,
 *              but this definition will be removed by the end of 2018.
 */
#define MV_OSAL_SHM_HANDLE_INVALIDATE   0

/**< Invalid MV share memory handle. */
#define MV_OSAL_SHM_HANDLE_INVALID      0

/** \typedef MV_OSAL_SHM_HANDLE
 *  \brief MV SHM handle is the key that operate the share memory.
 */
typedef unsigned int                  MV_OSAL_SHM_HANDLE;
typedef unsigned int                  MV_OSAL_SHM_TYPE;

typedef uint32_t MV_OSAL_MEDIA_SESSION_ID;

#define MV_OSAL_MAX_HEAP_NAME 32

/** MV osal share memory classes */
#define MV_OSAL_SHM_CLASS_CONTROL       0   /**< For control data structures */
#define MV_OSAL_SHM_CLASS_VIDEO         1   /**< For video software frame buffers */
#define MV_OSAL_SHM_CLASS_GRAPHICS      2   /**< For graphics buffers */
#define MV_OSAL_SHM_CLASS_OTHERS        3   /**< For other usage */
#define MV_OSAL_SHM_CLASS_HW_VIDEO      4   /**< For video hardware frame buffers */

/** MV osal share memory attribute flags */
#define MV_OSAL_SHM_FLAG_SECURE                 0x01    /**< Secure memory or not */
#define MV_OSAL_SHM_FLAG_CACHEABLE              0x02    /**< Cacheable memory or not */
#define MV_OSAL_SHM_FLAG_DYNAMIC                0x04    /**< Dynamic usage or not */

#define MV_OSAL_SHM_TYPE_MAGIC      0xDEAD  /**< The magic value for SHM type */
#define MV_OSAL_SHM_TYPE_INVALIDATE     0   /**< The invalidate SHM type */


/** \def MV_OSAL_SHM_TYPE_GEN(c, f)
 *
 *  \brief This macro function is used to generate MV SHM type based on
 *         memory class and memory attribute flag.
 *
 *  \param[in] c The share memory class.
 *  \param[in] f The share memory attribute flag. It can combinate several MV
 *               share memory flags with "|" operator.
 *
 *  \sa MV_OSAL_SHM_CLASS_CONTROL
 *  \sa MV_OSAL_SHM_CLASS_VIDEO
 *  \sa MV_OSAL_SHM_CLASS_GRAPHICS
 *  \sa MV_OSAL_SHM_CLASS_OTHERS
 *  \sa MV_OSAL_SHM_FLAG_SECURE
 *  \sa MV_OSAL_SHM_FLAG_CACHEABLE
 *  \sa MV_OSAL_SHM_FLAG_DYNAMIC
 */
#define MV_OSAL_SHM_TYPE_GEN(c, f)  \
    ((MV_OSAL_SHM_TYPE)((MV_OSAL_SHM_TYPE_MAGIC << 16) | (((c) & 0x7) << 3) | ((f) & 0x7)))


/** \def MV_OSAL_SHM_TYPE_GET_INDEX(t)
 *
 *  \brief This macro function is used to get pool index from SHM type.
 *
 *  \param[in] t The share memory type.
 *
 *  \sa MV_OSAL_SHM_TYPE_GEN
 */
#define MV_OSAL_SHM_TYPE_GET_INDEX(t)   ((t) & 0x3F)

/** \def MV_OSAL_SHM_TYPE_GET_MAGIC(t)
 *
 *  \brief This macro function is used to get magic code from SHM type.
 *
 *  \param[in] t The share memory type.
 *
 *  \sa MV_OSAL_SHM_TYPE_GEN
 */
#define MV_OSAL_SHM_TYPE_GET_MAGIC(t)   ((t) >> 16)

/** \def MV_OSAL_SHM_TYPE_IS_VALID(t)
 *
 *  \brief This macro function is used to check the SHM type is valid or not.
 *
 *  \param[in] t The share memory type.
 *
 *  \sa MV_OSAL_SHM_TYPE_GEN
 *  \sa MV_OSAL_SHM_TYPE_GET_MAGIC
 */
#define MV_OSAL_SHM_TYPE_IS_VALID(t)    \
    (MV_OSAL_SHM_TYPE_GET_MAGIC(t) == MV_OSAL_SHM_TYPE_MAGIC)


///////////////////////////////////////////////////////////////////////////////
//! [Part3] Local macros, type definitions
///////////////////////////////////////////////////////////////////////////////
/**< For buffer descriptor usage. */
#define MV_OSAL_SHM_BUFDESC                     \
    MV_OSAL_SHM_TYPE_GEN(MV_OSAL_SHM_CLASS_CONTROL, \
                     MV_OSAL_SHM_FLAG_CACHEABLE | MV_OSAL_SHM_FLAG_DYNAMIC)

/**< For uncacheable usage and from MV SHM.
     The space usually will be used by hardware device like TSP, DRMDMX
     Figo. The memory will allocated from hardware init, and be kept till
     deinit. */
#define MV_OSAL_SHM_UNCACHEABLE                 \
    MV_OSAL_SHM_TYPE_GEN(MV_OSAL_SHM_CLASS_OTHERS, 0)

/**< Similar with MV_OSAL_SHM_UNCACHEABLE but it is cacheable */
#define MV_OSAL_SHM_STATIC                      \
    MV_OSAL_SHM_TYPE_GEN(MV_OSAL_SHM_CLASS_OTHERS, MV_OSAL_SHM_FLAG_CACHEABLE)

/**< This memory will be dynamically allocated for component input/out
     buffer, like ES buffer etc. */
#define MV_OSAL_SHM_DYNAMIC                     \
    MV_OSAL_SHM_TYPE_GEN(MV_OSAL_SHM_CLASS_OTHERS,  \
        MV_OSAL_SHM_FLAG_CACHEABLE | MV_OSAL_SHM_FLAG_DYNAMIC)

/**< Similar with MV_OSAL_SHM_DYNAMIC but the required size is bigger than
     others. */
#define MV_OSAL_SHM_VIDEO_FB                    \
    MV_OSAL_SHM_TYPE_GEN(MV_OSAL_SHM_CLASS_VIDEO,   \
        MV_OSAL_SHM_FLAG_CACHEABLE | MV_OSAL_SHM_FLAG_DYNAMIC)

/**< Similar with MV_OSAL_SHM_VIDEO_FB, leave the type here to make the definition in pairs. */
#define MV_OSAL_SHM_HW_ONLY         MV_OSAL_SHM_VIDEO_FB

/**< Similar with MV_OSAL_SHM_VIDEO_FB but it is for graphics. */
#define MV_OSAL_SHM_GRAPHICS                    \
    MV_OSAL_SHM_TYPE_GEN(MV_OSAL_SHM_CLASS_GRAPHICS,   \
        MV_OSAL_SHM_FLAG_CACHEABLE | MV_OSAL_SHM_FLAG_DYNAMIC)



/**< Similar with MV_OSAL_SHM_UNCACHEABLE but it is secure memory space. */
#define MV_OSAL_SHM_SECURE_UNCACHEABLE          \
    MV_OSAL_SHM_TYPE_GEN(MV_OSAL_SHM_CLASS_OTHERS,  MV_OSAL_SHM_FLAG_SECURE)

/**< Similar with MV_OSAL_SHM_STATIC but it is secure memory space. */
#define MV_OSAL_SHM_SECURE_STATIC               \
    MV_OSAL_SHM_TYPE_GEN(MV_OSAL_SHM_CLASS_OTHERS,  \
        MV_OSAL_SHM_FLAG_CACHEABLE | MV_OSAL_SHM_FLAG_SECURE)

/**< Similar with MV_OSAL_SHM_DYNAMIC but it is secure memory space. */
#define MV_OSAL_SHM_SECURE_DYNAMIC              \
    MV_OSAL_SHM_TYPE_GEN(MV_OSAL_SHM_CLASS_OTHERS,  \
        MV_OSAL_SHM_FLAG_DYNAMIC | MV_OSAL_SHM_FLAG_CACHEABLE | MV_OSAL_SHM_FLAG_SECURE)

/**< Similar with MV_OSAL_SHM_VIDEO_FB but it is secure memory space. */
#define MV_OSAL_SHM_SECURE_VIDEO_FB             \
    MV_OSAL_SHM_TYPE_GEN(MV_OSAL_SHM_CLASS_VIDEO,   \
        MV_OSAL_SHM_FLAG_DYNAMIC | MV_OSAL_SHM_FLAG_CACHEABLE | MV_OSAL_SHM_FLAG_SECURE)

/**< Similar with MV_OSAL_SHM_HW_ONLY but it is secure memory space. */
#define MV_OSAL_SHM_SECURE_HW_ONLY             \
    MV_OSAL_SHM_TYPE_GEN(MV_OSAL_SHM_CLASS_HW_VIDEO,   \
        MV_OSAL_SHM_FLAG_DYNAMIC | MV_OSAL_SHM_FLAG_CACHEABLE | MV_OSAL_SHM_FLAG_SECURE)

/**< Similar with MV_OSAL_SHM_GRAPHICS but it is secure memory space. */
#define MV_OSAL_SHM_SECURE_GRAPHICS             \
    MV_OSAL_SHM_TYPE_GEN(MV_OSAL_SHM_CLASS_GRAPHICS,   \
        MV_OSAL_SHM_FLAG_DYNAMIC | MV_OSAL_SHM_FLAG_CACHEABLE | MV_OSAL_SHM_FLAG_SECURE)

typedef struct __osal_shm_sw_cfg__{
    char              cLogDir[64];
    unsigned int      EnUsageDbg;
    unsigned int      EnTraceDbg;
} OSAL_SHM_SW_CFG;

typedef struct __osal_shm_hw_cfg__ {
    unsigned int      Info;
} OSAL_SHM_HW_CFG;


typedef struct __osal_shm_drv_cfg__ {
    OSAL_SHM_HW_CFG      HWCfg;
    OSAL_SHM_SW_CFG      SWCfg;
} MV_OSAL_SHM_DRV_CFG;


typedef enum __shm_cache_action__ {
    SHM_CACHE_CLEAN           =   0x0,
    SHM_CACHE_INVALIDATE,
    SHM_CACHE_ACTION_MAX,
} SHM_CACHE_ACTION;

typedef enum __shm_dbg_ctrl_cmd__ {
    SHM_DBG_HW_POOL           =   0x0,
    SHM_DBG_ION_BACKTRACE,
    SHM_DBG_DUMP,
    SHM_DBG_REG_LOG_PRT,
    SHM_DBG_TAIL_ENABLE,
    SHM_DBG_POOL_MONITOR,
    SHM_DBG_CACHE_PERF_ENABLE,
    SHM_DBG_MAX,
} SHM_DBG_CTRL_CMD;

typedef enum __shm_trace_action__ {
    SHM_TRACE_NON           =   0x0,
    SHM_TRACE_ALLOC,
    SHM_TRACE_FREE,
    SHM_TRACE_REF,
    SHM_TRACE_UNREF,
    SHM_TRACE_MAP,
    SHM_TRACE_UNMAP,
    SHM_TRACE_IMPORT,
    SHM_TRACE_DISCARD,
    SHM_TRACE_BD_PUSH,
    SHM_TRACE_BD_REL,
    SHM_TRACE_ACTION_MAX,
} SHM_TRACE_ACTION;

typedef enum __shm_mmu_type__ {
    /* generic physical scatter buffer */
    MV_OSAL_SHM_MMU_SCATTER_GENERIC,
    /* UV entry also page aligned, must set total size and y size together */
    MV_OSAL_SHM_MMU_SCATTER_UV_ALIGN
} MV_OSAL_SHM_MMU_TYPE;

/* extra parameter for allocated buffer */
typedef struct __shm_fb_config__ {
    MV_OSAL_SHM_MMU_TYPE eType;
    unsigned int         uiSizeY;
} MV_OSAL_SHM_CONFIG;

typedef enum {
    MV_OSAL_SHM_INTERNAL_CLIENT_NONE           = 0x00000000, /**< None */
    MV_OSAL_SHM_INTERNAL_CLIENT_DMX            = 0x00000001, /**< DMX */
    MV_OSAL_SHM_INTERNAL_CLIENT_M2M            = 0x00000002, /**< M2M */
    MV_OSAL_SHM_INTERNAL_CLIENT_VDEC           = 0x00000004, /**< VDEC */
    MV_OSAL_SHM_INTERNAL_CLIENT_VENC           = 0x00000008, /**< VENC */
    MV_OSAL_SHM_INTERNAL_CLIENT_DEINT          = 0x00000010, /**< DEINT */
    MV_OSAL_SHM_INTERNAL_CLIENT_DVS            = 0x00000020, /**< DVS */
    MV_OSAL_SHM_INTERNAL_CLIENT_SCALER         = 0x00000040, /**< SCALER */
    MV_OSAL_SHM_INTERNAL_CLIENT_DISPLAY        = 0x00000080, /**< DISPLAY */
    MV_OSAL_SHM_INTERNAL_CLIENT_ADEC           = 0x00000100, /**< ADEC */
    MV_OSAL_SHM_INTERNAL_CLIENT_AENC           = 0x00000200, /**< AENC */
    MV_OSAL_SHM_INTERNAL_CLIENT_AREN           = 0x00000400, /**< AREN */
    MV_OSAL_SHM_INTERNAL_CLIENT_APP            = 0x00000800, /**< APP */
    MV_OSAL_SHM_INTERNAL_CLIENT_AVIN           = 0x00001000, /**< AVIN */
    MV_OSAL_SHM_INTERNAL_CLIENT_AOUT           = 0x00002000, /**< AOUT */
    MV_OSAL_SHM_INTERNAL_CLIENT_SOUNDSINK      = 0x00004000, /**< SOUNDSINK */
    MV_OSAL_SHM_INTERNAL_CLIENT_DATA_EXPORTER  = 0x00008000, /**< DATA_EXPORTER */
    MV_OSAL_SHM_INTERNAL_CLIENT_GPU            = 0x00010000, /**< GPU */
    MV_OSAL_SHM_INTERNAL_CLIENT_NPU            = 0x00020000, /**< NPU */
    MV_OSAL_SHM_INTERNAL_CLIENT_ISP            = 0x00040000, /**< ISP */
    MV_OSAL_SHM_INTERNAL_CLIENT_DEWARP         = 0x00080000, /**< DEWARP */
    MV_OSAL_SHM_INTERNAL_CLIENT_TA             = 0x00100000, /**< TA */
    MV_OSAL_SHM_INTERNAL_CLIENT_BCM            = 0x00200000, /**< BCM */
    MV_OSAL_SHM_INTERNAL_CLIENT_DRM            = 0x00400000, /**< DRM */
    MV_OSAL_SHM_INTERNAL_CLIENT_APPLICATION    = 0x00800000, /**< APPLICATION */
} MV_OSAL_SHM_INTERNAL_CLIENT;

typedef struct {
    uint32_t timeLow;
    uint16_t timeMid;
    uint16_t timeHiAndVersion;
    uint8_t clockSeqAndNode[8];
} MV_OSAL_TA_UUID;

typedef struct {
  uint32_t _maximum;
  uint32_t _length;
  MV_OSAL_TA_UUID *_buffer;
  unsigned char _release;
} MV_OSAL_SEQUENCE_AMP_TA_UUID;

typedef struct {
    MV_OSAL_SHM_INTERNAL_CLIENT uiAmpClients;
    MV_OSAL_SEQUENCE_AMP_TA_UUID stTrustedApps;
} MV_OSAL_SHM_CLIENTS;

typedef enum {
    MV_OSAL_SHM_CONTENT_TYPE_NONE,
    MV_OSAL_SHM_CONTENT_TYPE_VIDEO_FRAME,
    MV_OSAL_SHM_CONTENT_TYPE_AUDIO_FRAME,
    MV_OSAL_SHM_CONTENT_TYPE_ENCRYPTED_VIDEO_ES,
    MV_OSAL_SHM_CONTENT_TYPE_ENCRYPTED_AUDIO_ES,
    MV_OSAL_SHM_CONTENT_TYPE_CLEAR_VIDEO_ES,
    MV_OSAL_SHM_CONTENT_TYPE_CLEAR_AUDIO_ES,
    MV_OSAL_SHM_CONTENT_TYPE_TRANSPORT_STREAM,
    MV_OSAL_SHM_CONTENT_TYPE_DATA,
    MV_OSAL_SHM_CONTENT_TYPE_MAX
} MV_OSAL_SHM_CONTENT_TYPE;

typedef enum {
    MV_OSAL_SHM_ZONE_NON_CONTIGUOUS,
    MV_OSAL_SHM_ZONE_CONTIGUOUS,
    MV_OSAL_SHM_ZONE_MAX
} MV_OSAL_SHM_ZONE_TYPE;

typedef struct {
    void    *phy_addr;
    size_t   phy_size;
    void    *pt_addr;
    size_t   pt_size;
    void    *virt_addr;
    size_t   mem_id;
} MV_OSAL_SHM_ADDRESS_HANDLE;

typedef struct {
    MV_OSAL_SHM_ZONE_TYPE eType;
    uint8_t ruleEnable;
    uint8_t zoneEnable;
    uint8_t flag;
    char heapName[MV_OSAL_MAX_HEAP_NAME];
    uint8_t heapID;
    uint8_t isScatter;
    uint8_t is_pt_created;
} MV_OSAL_SHM_INFO;

typedef struct {
    MV_OSAL_SHM_INFO shm_info;
    MV_OSAL_SHM_ADDRESS_HANDLE addr_handle;
} MV_OSAL_SHM_ZONE;

#endif /* __shm_type_h__ */
