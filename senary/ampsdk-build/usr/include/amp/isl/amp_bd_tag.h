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
//! \file amp_bd_tag.h
//!
//! \brief This file used to define all the control info and meta data with the
//!        tag structure.
//!
//! Purpose:
//!
//!
//! Version, Date and Author :
//!   V 1.00,    December 19 2012,  Weizhao Jiang
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////


#ifndef __AMP_BD_TAG_H__
#define __AMP_BD_TAG_H__

/************* Header file include ********************************************/
#include "amp_types.h"
#include "isl/amp_shm.h"
#include "amp_video_api.h"
#include "amp_vdec_userdata.h"

/************* Macros, type definitions****************************************/

/** \def    BDTAG_MAGIC_NUM
 *  \brief  Description for macro BD tag magic number
 */
#define BDTAG_MAGIC_NUM       0xBD

/** \def    GEN_CTRL_MASK
 *  \brief  Description for mask macro for gen control info
 */
#define GEN_CTRL_MASK         0x0

/** \def    DMX_CTRL_MASK
 *  \brief  Description for mask macro for DMX control info
 */
#define DMX_CTRL_MASK         0x1

/** \def    VID_CTRL_MASK
 *  \brief  Description for mask macro for VIDEO control info
 */
#define VID_CTRL_MASK         0x2

/** \def    AUD_CTRL_MASK
 *  \brief  Description for mask macro for AUDIO control info
 */
#define AUD_CTRL_MASK         0x3

/** \def    SYNC_CTRL_MASK
 *  \brief  Description for mask macro for SYNC control info
 */
#define SYNC_CTRL_MASK        0x4

/** \def    AVIN_CTRL_MASK
 *  \brief  Description for mask macro for AVIN control info
 */
#define AVIN_CTRL_MASK        0x5

/** \def    GEN_META_MASK
 *  \brief  Description for mask macro for GEN meta data
 */
#define GEN_META_MASK         0x8

/** \def    DMX_META_MASK
 *  \brief  Description for mask macro for DMX meta data
 */
#define DMX_META_MASK         0x9

/** \def    VID_META_MASK
 *  \brief  Description for mask macro for VIDEO meta data
 */
#define VID_META_MASK         0xa

/** \def    AUD_META_MASK
 *  \brief  Description for mask macro for AUDIO meta data
 */
#define AUD_META_MASK         0xb

/** \def    SYNC_META_MASK
 *  \brief  Description for mask macro for SYNC meta data
 */
#define SYNC_META_MASK        0xc

/** \def    AVIN_META_MASK
 *  \brief  Description for mask macro for AVIN meta data
 */
#define AVIN_META_MASK        0xd

/** \def    DVS_CTRL_MASK
 *  \brief  Description for mask macro for DVS control info
 */
#define DVS_CTRL_MASK         0xe

/** \def    ISP_META_MASK
 *  \brief  Description for mask macro for ISP subsystem meta data
 */
#define ISP_META_MASK         0xf


/** \def    BDTAG_TYPE(Mask, Num)
 *  \brief  Description for package BD tag
 */
#define BDTAG_TYPE(Mask, Num)   (((BDTAG_MAGIC_NUM)<<24)   | \
                                 ((Mask)<<16)              | \
                                 (Num))

/** \def    AMP_BDTAG_VALID(Tag)
 *  \brief  Description for BD tag magic checking
 */
#define AMP_BDTAG_VALID(Tag)    (Tag & ((BDTAG_MAGIC_NUM)<<24))

/** \def    AMP_MAX_USER_DATA_TYPE
 *  \brief  Description for AMP_MAX_USER_DATA_TYPE
 */
#define AMP_MAX_USER_DATA_TYPE    VIDEO_USER_DATA_TYPE_MAX

/** \def    AMP_AUD_MAX_BUF_NR
 *  \brief  Max buffer number for audio module using
 */
#define AMP_AUD_MAX_BUF_NR        AMP_MAX_CHANNELS

/** \def    TIME_STAMP_VALID_MASK
 *  \brief  Description for the mask of valid time stamp
 */
#define TIME_STAMP_VALID_MASK 0x80000000


/** enumerate all tag type of control info and meta data in AMP. */
typedef enum {
    AMP_BDTAG_INVALID                  = 0x0,     /**< Invalid */

    AMP_BDTAG_DUMMY_INFO               = BDTAG_TYPE(GEN_CTRL_MASK, 0x1),
                                        /**< For gen dummy, deprecated */
    AMP_BDTAG_ASSOCIATE_MEM_INFO       = BDTAG_TYPE(GEN_CTRL_MASK, 0x1),
                                        /**< For associate memory info */
    AMP_BDTAG_PATH_META                = BDTAG_TYPE(GEN_CTRL_MASK, 0x2),
                                        /**< For path info, used by DISPLAY to
                                         * identify the source of BD(VOUT)*/
    AMP_BDTAG_DV_INFO_META             = BDTAG_TYPE(GEN_CTRL_MASK, 0x3),
                                        /**< For dv info */
    AMP_BDTAG_HDR_INFO_META            = BDTAG_TYPE(GEN_CTRL_MASK, 0x4),
                                        /**< For video HDR info */
    /**
     *  Control info tag
     */
    /** Demux control tag */
    AMP_BDTAG_PAYLOAD_DUMMY_CTRL       = BDTAG_TYPE(DMX_CTRL_MASK, 0x0),
                                        /**< For payload dummy */
    AMP_BDTAG_PAYLOAD_WM_CTRL          = BDTAG_TYPE(DMX_CTRL_MASK, 0x1),
                                        /**< For WM payload */
    AMP_BDTAG_PAYLOAD_PLAYREADY_CTRL   = BDTAG_TYPE(DMX_CTRL_MASK, 0x2),
                                        /**< For playready payload */
    AMP_BDTAG_PAYLOAD_MARLIN_CTRL      = BDTAG_TYPE(DMX_CTRL_MASK, 0x3),
                                        /**< For marlin payload */
    AMP_BDTAG_PAYLOAD_WIDEVINE_CTRL    = BDTAG_TYPE(DMX_CTRL_MASK, 0x4),
                                        /**< For widevine payload */
    AMP_BDTAG_PAYLOAD_HLS_CTRL         = BDTAG_TYPE(DMX_CTRL_MASK, 0x5),
                                        /**< For HLS payload */
    AMP_BDTAG_PAYLOAD_REENCRYPT_CTRL   = BDTAG_TYPE(DMX_CTRL_MASK, 0x6),
                                        /**< For reencrypt payload */
    AMP_BDTAG_PAYLOAD_HDCP_CTRL        = BDTAG_TYPE(DMX_CTRL_MASK, 0x7),
                                        /**< For HDCP payload */
    AMP_BDTAG_PAYLOAD_DTCPIP_CTRL      = BDTAG_TYPE(DMX_CTRL_MASK, 0x8),
                                        /**< For DTCPIP payload */
    AMP_BDTAG_PAYLOAD_CTRL_BUFFER      = BDTAG_TYPE(DMX_CTRL_MASK, 0x9),
                                        /**< For CTRL buffer */
    AMP_BDTAG_INPUT_FLUSH_BUFFER       = BDTAG_TYPE(DMX_CTRL_MASK, 0x10),

    AMP_BDTAG_PAYLOAD_MOBITV_CTRL      = BDTAG_TYPE(DMX_CTRL_MASK, 0x11),

    AMP_BDTAG_PAYLOAD_HDCP20_CTRL      = BDTAG_TYPE(DMX_CTRL_MASK, 0x12),
                                        /**< For Flush Input Buffer */
    AMP_BDTAG_BS_DUMMY_CTRL            = BDTAG_TYPE(DMX_CTRL_MASK, 0x80),
                                        /**< For BS dummy */
    AMP_BDTAG_BS_UNITSTART_CTRL        = BDTAG_TYPE(DMX_CTRL_MASK, 0x81),
                                        /**< For unit start */
    AMP_BDTAG_BS_UNITEND_CTRL          = BDTAG_TYPE(DMX_CTRL_MASK, 0x82),
                                        /**< For unit end */
    AMP_BDTAG_BS_TRANSPORTERROR_ctrl   = BDTAG_TYPE(DMX_CTRL_MASK, 0x83),
                                        /**< For transport error */
    AMP_BDTAG_BS_CONTINUITYERROR_CTRL  = BDTAG_TYPE(DMX_CTRL_MASK, 0x84),
                                        /**< For continuity error */
    AMP_BDTAG_BS_PCR_INFO              = BDTAG_TYPE(DMX_CTRL_MASK, 0x85),
                                        /**< For PCR info */
    AMP_BDTAG_BS_DISCONTINDICATE_CTRL  = BDTAG_TYPE(DMX_CTRL_MASK, 0x86),
                                        /**< For discontinue indicat*/
    AMP_BDTAG_BS_TSSCRAMCHG_CTRL       = BDTAG_TYPE(DMX_CTRL_MASK, 0x87),
                                        /**< For TS scramble change */
    AMP_BDTAG_BS_PESSCRAMCHG_CTRL      = BDTAG_TYPE(DMX_CTRL_MASK, 0x88),
                                        /**< For PES scramble change*/
    AMP_BDTAG_BS_CRCERROR_CTRL         = BDTAG_TYPE(DMX_CTRL_MASK, 0x89),
                                        /**< For CRC error */
    AMP_BDTAG_BS_CRYPTO_CTRL           = BDTAG_TYPE(DMX_CTRL_MASK, 0x90),
                                        /**< For crypto setting */
    AMP_BDTAG_BS_BUFFSTAT_CTRL         = BDTAG_TYPE(DMX_CTRL_MASK, 0x91),
                                        /**< For buffer full */
    AMP_BDTAG_BS_PVR_INDEX_CTRL         = BDTAG_TYPE(DMX_CTRL_MASK, 0x92),
                                        /**< For TS recording index */
    AMP_BDTAG_BS_AUD_DESCRIPTOR_CTRL     = BDTAG_TYPE(DMX_CTRL_MASK, 0x93),
                                        /**< For Audio Description */
    AMP_BDTAG_BS_INDICATOR_CTRL          = BDTAG_TYPE(DMX_CTRL_MASK, 0x94),
                                        /**< For strm indicator(strm tag) Description */

    /** Video control type */
    AMP_BDTAG_VID_DUMMY_CTRL           = BDTAG_TYPE(VID_CTRL_MASK, 0x0),
                                        /**< For video ctrl dummy */

    /** Audio control type */
    AMP_BDTAG_AUD_DUMMY_CTRL           = BDTAG_TYPE(AUD_CTRL_MASK, 0x0),
                                        /**< For audio ctrl dummy */
    AMP_BDTAG_AUD_STREAMCHANGE_CTRL    = BDTAG_TYPE(AUD_CTRL_MASK, 0x1),
                                        /**< For audio stream format change */
    AMP_BDTAG_AUD_METADATA_CTRL        = BDTAG_TYPE(AUD_CTRL_MASK, 0x2),
                                        /**< For audio meta data */
    AMP_BDTAG_AUD_MS12_METADATA_CTRL   = BDTAG_TYPE(AUD_CTRL_MASK, 0x3),
                                        /**< For MS audio meta data */
    AMP_BDTAG_AUD_ROUTE_CTRL           = BDTAG_TYPE(AUD_CTRL_MASK, 0x4),
                                        /**< For audio route info */

    /** Sync control type */
    AMP_BDTAG_SYNC_DUMMY_CTRL          = BDTAG_TYPE(SYNC_CTRL_MASK, 0x0),
                                        /**< For av sync ctrl dummy */
    /** AVIN control type */
    AMP_BDTAG_AVIN_DUMMY_CTRL          = BDTAG_TYPE(AVIN_CTRL_MASK, 0x0),
                                        /**< For avin ctrl dummy */
    AMP_BDTAG_AVIN_VBI_CTRL            = BDTAG_TYPE(AVIN_CTRL_MASK, 0x1),
                                        /**< For avin ctrl VBI */

    /**
     *  Media data tag
     */
    /**  Generic meta data tag */
    AMP_BDTAG_GENERIC_DUMMY_META       = BDTAG_TYPE(GEN_META_MASK, 0x0),
                                        /**< For generic meta dummy */
    AMP_BGTAG_FRAME_INFO_META          = BDTAG_TYPE(GEN_META_MASK, 0x1),
                                        /**< For video and graphic frame info*/
    AMP_BDTAG_STREAM_INFO_META         = BDTAG_TYPE(GEN_META_MASK, 0x2),
                                        /**< For video stream info*/
    /**  Demux meta data tag */
    AMP_BDTAG_DMX_DUMMY_META           = BDTAG_TYPE(DMX_META_MASK, 0x0),
                                        /**< For demux meta dummy */

    /**  Video meta data tag */
    AMP_BDTAG_VID_DUMMY_META           = BDTAG_TYPE(VID_META_MASK, 0x0),
                                        /**< For video meta dummy */
    /** vMeta buffer descriptor tag*/
    AMP_BDTAG_VID_VMETA_DESC           = BDTAG_TYPE(VID_META_MASK, 0x1),

    /**  Audio meta data tag */
    AMP_BDTAG_AUD_DUMMY_META           = BDTAG_TYPE(AUD_META_MASK, 0x0),
                                        /**< For audio meta dummy */
    AMP_BDTAG_AUD_STREAM_META          = BDTAG_TYPE(AUD_META_MASK, 0x0),
                                        /**< For audio stream meta data info */

    AMP_BDTAG_AUD_FRAME_CTRL           = BDTAG_TYPE(AUD_META_MASK, 0x1),
                                        /**< For audio data */
    AMP_BDTAG_AMIX_PTS_META            = BDTAG_TYPE(AUD_META_MASK, 0x2),
                                        /**< For mixer info */
    AMP_BDTAG_AUD_REALAUDIO_DEC        = BDTAG_TYPE(AUD_META_MASK, 0x3),
                                        /**< For Real audio decode info */
    AMP_BDTAG_AUD_NEURAL_META          = BDTAG_TYPE(AUD_META_MASK, 0x4),
                                        /**< For DTS Neural audio descriptor */
    AMP_BDTAG_AUD_EXT_META             = BDTAG_TYPE(AUD_META_MASK, 0x5),
                                        /**< For audio meta data transferring
                                             through shared memory */
    AMP_BDTAG_AUD_GRAPH_PTS_META       = BDTAG_TYPE(AUD_META_MASK, 0x6),
                                        /**< For PTS passing through the audio
                                             graph */

    AMP_BDTAG_AUD_LOCAL_PTS_META       = BDTAG_TYPE(AUD_META_MASK, 0x7),
                                        /**< For local PTS passing through the audio pipeline */
                                        /**< The PTS is generated by sndsink, and is used for
                                         * calculating pipeline fullness */

    /**  AVSync meta data tag */
    AMP_BDTAG_SYNC_DUMMY_META          = BDTAG_TYPE(SYNC_META_MASK, 0x0),
                                        /**< For av sync meta dummy */
    AMP_BDTAG_SYNC_PTS_META            = BDTAG_TYPE(SYNC_META_MASK, 0x1),
                                        /**< For av sync PTS tag */

    /** AVIN meta data tag */
    AMP_BDTAG_AVIN_DUMMY_META          = BDTAG_TYPE(AVIN_META_MASK, 0x0),
                                        /**< For avin meta dummy */

    /** ISP Sensor metadata tag */
    AMP_BDTAG_ISP_SENSOR_META          = BDTAG_TYPE(ISP_META_MASK, 0x0),
                                        /**< For ISP meta data */
    AMP_BDTAG_MAX,                        /**< The max value */
} AMP_BDTAG_T;

/** \struct AMP_BDTAG_H
 *  \brief The common header of BD tag is defined for T(Tag) - L(Length) -
 *         V(Value) packeting structure.
 *
 */
typedef struct {
    AMP_BDTAG_T eType;              /**< The type of buffer tag */
    UINT32      uLength;            /**< The length of tag body +
                                         sizeof(AMP_BDTAG_H)*/
} AMP_BDTAG_H;

/** \def AMP_BD_TAG_TYPE(pTag)
 *  \brief  Get Tag type from BD Tag
 */
#define AMP_BD_TAG_TYPE(pTag)       (((AMP_BDTAG_H *)(pTag))->eType)


/** \struct AMP_BDTAG_DUMMY
 *  \brief Using T(Tag) - L(Length) - V(Value) packeting structure to define AMP
 *         BD tag. It covers both control info and meta data.
 *         AMP_BDTAG_DUMMY is an example for reference.
 *
 */
typedef struct {
    AMP_BDTAG_H Header;             /**< The header of BD Tag */
    UINT32      uTagBody;           /**< The value of BD tag */
} AMP_BDTAG_DUMMY;


/** \struct AMP_BDTAG_VMETA_DESC
 *  \brief This tag is used to pad vMeta buffer descriptor to BD.
 */
typedef struct {
    AMP_BDTAG_H Header;             /**< The header of BD Tag */
    /**
     * bd tag body
     */
    UINT32      uDesc;             /**< The address of buffer sent to vMeta */
} AMP_BDTAG_VMETA_DESC;

/** enumerate all memory info flag masks supported in AMP. */
typedef enum {
    AMP_MEMINFO_FLAG_SOU_MASK   = 0x00000001,   /**< for the start of unit.*/
    AMP_MEMINFO_FLAG_EOU_MASK   = 0x00000002,   /**< for the end of unit.*/
    AMP_MEMINFO_FLAG_SOS_MASK   = 0x00000004,   /**< for the start of stream.*/
    AMP_MEMINFO_FLAG_EOS_MASK   = 0x00000008,   /**< for the end of stream.*/

    AMP_MEMINFO_FLAG_DATAVALID_MASK = 0x00000010,
                                                /**< Indicated data is valid in
                                                     cache or physical memory.
                                                     Set if it's memory valid.
                                                     Unset means cache valid */
    AMP_MEMINFO_FLAG_DROPBD_MASK   = 0x00000020,/**< This BD is a drop BD.*/
    AMP_MEMINFO_FLAG_DATAERR_MASK  = 0x00000040,/**< This Data has error.*/
    AMP_MEMINFO_FLAG_CRCERR_MASK   = 0x00000080,/**< This Data with CRC error.*/
    AMP_MEMINFO_FLAG_UNDERFLOW_MASK = 0x00000100,   /**< probably underflow */
    AMP_MEMINFO_FLAG_DISC_MASK      = 0x00000200,   /**< discontinue */
    AMP_MEMINFO_FLAG_DATAINVALID_MASK = 0x00000400, /**< to indicate data is valid or not*/

    /**< This mask indicates data is in DRAM. HW producers should set this flag
         before pushing BD downstream. It also should be set if flushing has
         been done before pushing downstream. SW producers should clear this
         flag before pushing BD downstream.

         Consumers check this flag to see if flushing is needed for keeping
         cache consistency. It's a requisite before data processing for HW
         consumers if FALSE was returned. SW consumers may ignore this check.
     */
    AMP_MEMINFO_FLAG_DATA_IN_DRAM_MASK = 0x00000800,

    /**< This mask indicates data is in CPU cache. SW producers should set this
         flag before pushing BD downstream. It also should be set if
         invalidating has been done before pushing downstream. HW producers
         should clear this flag before pushing BD downstream.

         Consumers check flag to see if invalidating is needed for keeping
         cache consistency. It's a requisite before data processing for SW
         consumers if FALSE was returned. HW consumers may ignore this check.
     */
    AMP_MEMINFO_FLAG_DATA_IN_CACHE_MASK = 0x00001000,
    AMP_MEMINFO_FLAG_FLUSH_MASK         = 0x00002000,/**< This BD is a flush BD.*/
    AMP_MEMINFO_FLAG_SECTION_POLARITY_MASK = 0xC0000000, /**< for the section polarity*/
} AMP_MEMINFO_FLAG_MASK;

/** \struct AMP_BDTAG_MEMINFO
 *  \brief This tag is used to cover the memory info, which incldues the memory
 *         handle and the start offset.
 *
 */
typedef struct {
    AMP_BDTAG_H Header;             /**< The header of BD Tag */

    /**
     * bd tag body
     */
    UINT32      uMemHandle;        /**< The handle of memory space */
    UINT32      uMemOffset;        /**< The start offset of memory space */
    UINT32      uSize;             /**< The size of memory space */
    UINT32      uFlag;             /**< The flag of memory space */
} AMP_BDTAG_MEMINFO;

#define AMP_BDTAG_MEMINFO_INIT(pMemTag)                           \
    do {                                                          \
        AmpMemClear((pMemTag), sizeof(AMP_BDTAG_MEMINFO));        \
        (pMemTag)->Header.eType   = AMP_BDTAG_ASSOCIATE_MEM_INFO; \
        (pMemTag)->Header.uLength = sizeof(AMP_BDTAG_MEMINFO);    \
    } while (0)

/** \struct AMP_BDTAG_DV_INFO
 *  \brief This tag is used to indicate the present mode of dolby vision memory.
 */
typedef struct {
    AMP_BDTAG_H Header;             /**< The header of BD Tag */

    UINT32      uDvPresentMode;     /**< The Present Mode of Dolby Vision Mem Info, BL Only;BL/EL/MD;BL/MD*/
    UINT32      uDvStatus;       /**< The status of DV BD */
    AMP_BD_ST   DvElBD;          /**< The enhance layer BD pointer. if zero, it means not present. */
    AMP_BD_ST   DvRpuBD;         /**< The parsed Rpu (meta data) BD pointer. if zero, it means not present. */
} AMP_BDTAG_DV_INFO;

#define INVALID_DV_PRESENT_MODE              0
#define AMP_BDTAG_DV_INFO_INIT(pDV_Info)                               \
    do {                                                               \
        AmpMemClear((pDV_Info), sizeof(AMP_BDTAG_DV_INFO));            \
        (pDV_Info)->Header.eType   = AMP_BDTAG_DV_INFO_META;           \
        (pDV_Info)->Header.uLength = sizeof(AMP_BDTAG_DV_INFO);        \
        (pDV_Info)->uDvPresentMode = INVALID_DV_PRESENT_MODE;          \
        (pDV_Info)->uDvStatus      = AMP_FRAME_ST_INVALID;             \
    } while (0)

/** \struct AMP_BDTAG_HDR_INFO
 *  \brief This tag is used to pass HDR meta data
 */
typedef struct {
    AMP_BDTAG_H Header;             /**< The header of BD Tag */

    UINT32      uStrmPos;           /**< stream postion of this tag */
    AMP_HDR_INFO sHdrInfo;
} AMP_BDTAG_HDR_INFO;

#define AMP_BDTAG_HDR_INFO_INIT(pHdrInfo)                             \
    do {                                                              \
        AmpMemClear((pHdrInfo), sizeof(AMP_BDTAG_HDR_INFO));          \
        (pHdrInfo)->Header.eType   = AMP_BDTAG_HDR_INFO_META;         \
        (pHdrInfo)->Header.uLength = sizeof(AMP_BDTAG_HDR_INFO);      \
        (pHdrInfo)->sHdrInfo.type = AMP_HDR_TYPE_INVALID;             \
    } while (0)


/** \struct AMP_BDTAG_PATH_INFO
 *  \brief This tag is used to path info for BD in pipeline
 *         When want indentify BD from (or pass through) which one component, add this tag
 */
typedef struct {
    AMP_BDTAG_H Header;
    /**
     * bd tag body
     */
    VOID *handle;                 /**< The handle of passed component */
    UINT32 compType;              /**< type of passed compoenet */
} AMP_BDTAG_PATH_INFO;

/** \struct AMP_BDTAG_TVPCTRLBUF
 *  \brief This tag is used to cover the TVP control buffer.
 *
 */
typedef struct {
    AMP_BDTAG_H Header;             /**< The header of BD Tag */

    /**
     * bd tag body
     */
    UINT32      uMemHandle;        /**< The handle of memory space */
    UINT32      uTotal;            /**< The number of ctrl info */
    UINT32      uStart;            /**< The index of start ctrl info */
    UINT32      uSize;             /**< The size of ctrl infos */
} AMP_BDTAG_TVPCTRLBUF;

/** \struct AMP_BDTAG_UNITSTART
 *  \brief This tag is used in DMX component to send PTS/DTS info with BD.
 *
 */
typedef struct {
    AMP_BDTAG_H Header;             /**< The header of BD Tag */

    /**
     * bd tag body
     */
    UINT32      uPtsHigh;        /**< The valid mask and the MSB of PTS */
    UINT32      uPtsLow;         /**< 32 bits of PTS lower part */
    UINT32      uDtsHigh;        /**< The valid mask and the MSB of DTS */
    UINT32      uDtsLow;         /**< 32 bits of DTS lower part */
    UINT32      uStrmPos;
    UINT32      uReserved;       /**< 32bit flag for VDEC internal use */
    /**< uStrmPos is the stream position the tag applies.
         There is a 32-bit data counter for each output port of DMX component.
         The counter is in the unit of byte.
         (1) When one DMX output channel maps to one output port (in the case of
         ES/PES/Section output), this counter will be initialized and set to 0
         when the channel is added using DMX API "AddChannel". The counter will
         increase by every byte output from the port after user sets the channel
         to "ON" state using DMX API "ChannelControl" with "CH_CTRL_ON"
         parameter. It will be reset back to 0 when user sets the channel to
         "OFF" state using DMX API "ChannelControl" with "CH_CTRL_OFF" parameter.
         (2) When multiple DMX output channel maps to one output port (in the
         case of TS recording where different PID goes to one port), the 32-bit
         data counter will be initialized and set to 0 when the first channel
         that output to the port is added using "AddChannel" interface. The
         counter increases when data output from the port and is set back to 0
         when all channels that output to the port is set to "OFF" by user by
         using DMX API "ChannelControl" with "CH_CTRL_OFF" parameter.
    */

} AMP_BDTAG_UNITSTART;

/** \enum   PVR_INDEX_TYPE
 *  \brief  Enumerate all indexing info types.
 */
typedef enum {
    INDEX_INVALID      = 0,
    INDEX_SEQ_HEADER   = 1,      /**< Indexing event type: Sequence header.*/
    INDEX_SEQ_PIC_I    = 2,      /**< Indexing event type: Sequence header followed by I picture.*/
    INDEX_PIC_I        = 3,      /**< Indexing event type: I picture.*/
    INDEX_PIC_P        = 4,      /**< Indexing event type: P picture.*/
    INDEX_PIC_B        = 5,      /**< Indexing event type: B picture.*/
    INDEX_H265_SPS     = 6,      /**< Indexing event type: H.265 SPS */
    INDEX_H265_AUD     = 7,      /**< Indexing event type: H.265 AUD */
    INDEX_MAX          = 8,
} PVR_INDEX_TYPE;

/** \enum   H265_I_FRAME_TYPE
 *  \brief  Enumerate H.265 specific I frame types.
 */
typedef enum {
    TYPE_INVALID       = 0,
    TYPE_BLA_W_LP      = 1,      /**< I frame type: BLA_W_LP */
    TYPE_BLA_W_RADL    = 2,      /**< I frame type: BLA_W_RADL */
    TYPE_BLA_N_LP      = 3,      /**< I frame type: BLA_N_LP */
    TYPE_IDR_W_RADL    = 4,      /**< I frame type: IDR_W_RADL */
    TYPE_IDR_N_LP      = 5,      /**< I frame type: IDR_N_LP */
    TYPE_CRA           = 6,      /**< I frame type: CRA */
    TYPE_MAX           = 7,
} H265_I_FRAME_TYPE;

/** \struct AMP_BDTAG_DISCONT_INDICATE
 *  \brief This tag is used in DMX component to send DISCONTINUE info with BD.
 *
 */

typedef struct {
    AMP_BDTAG_H Header;             /**< The header of BD Tag */
    UINT32    m_PcrPid;             /**< The PCR PID */
    UINT32    m_StartPts;           /**< The 32bits of start pts */
    UINT32    m_EndPts;             /**< The 32bits of end pts */
} AMP_BDTAG_DISCONT_INDICATE;

/** \struct AMP_BDTAG_AUDIO_DISCRIPTOR
 *  \brief This tag is used in DMX component to send AUDIO_DISCRIPTOR info with BD.
 *
 */
typedef struct {
    AMP_BDTAG_H Header;             /**< The header of BD Tag */
    UINT32	 ADFadeByte		    :  8; /**< The ADFadeByte */
    UINT32   ADPanByte		    :  8; /**< The ADPanByte */
    UINT32	 ADGainByteCenter	:  8; /**< The ADGainByteCenter*/
    UINT32	 ADGainByteFront	:  8; /**< The ADGainByteFront */

    UINT32	 ADGainByteSurround :  8; /**< The ADGainByteSurround */
    UINT32	 Reserved			: 24; /**< The Reserved */
} AMP_BDTAG_AUDIO_DISCRIPTOR;

/** \struct AMP_BDTAG_PVR_INDEX
 *  \brief This tag is used to send PVR index info with BD.
 *
 */
typedef struct {
    AMP_BDTAG_H Header;          /**< The header of BD Tag */

    /**
     * bd tag body
     */
    UINT32             uStrmPos;   /**< The Stream Position the the tag applies.
                                        It works the same way as the "uStrmPos"
                                        in the AMP_BDTAG_UNITSTART data
                                        structure. */
    PVR_INDEX_TYPE     IndexInfo;  /**< The type of the pvr index info */
    H265_I_FRAME_TYPE  H265IFrameType; /**< The H.265 specific I frame type.
                                            This field is only valid when
                                            the video stream is H.265 and
                                            IndexInfo == INDEX_PIC_I */
} AMP_BDTAG_PVR_INDEX;

/** enumerate all tag type of control info and meta data in AMP.  */
typedef enum {
    AMP_FRAME_ST_INVALID          = -1,    /**< Invalid */
    AMP_FRAME_ST_ALLOCATED        = 0,     /**< Frame buffer is allocated */
    AMP_FRAME_ST_DECODED          = 1,     /**< Frame buffer is filled */
    AMP_FRAME_ST_MODIFIED         = 2,     /**< Frame buffer is modified */
    AMP_FRAME_ST_DISPLAYED        = 3,     /**< Frame buffer is displayed */
    AMP_FRAME_ST_MAX              = 4,     /**< The max value */
} AMP_FRAME_ST_T;


/** \struct AMP_BGTAG_FRAME_INFO
 *  \brief frame information for both video and graphic frames
 */
typedef struct {
    AMP_BDTAG_H Header;            /**< The header of BD Tag */

    /**
    *  bd tag body
    */
    UINT32      uiMagicNum;        /**< magic number of the frame buffer. */
    AMP_FRAME_ST_T    uStatus;     /**< The status of the frame */
    UINT32      pBufStart;         /**< base address of buffer */
    AMP_SHM_HANDLE hShm;           /**< 1. it's YUV buffer handle when */
                                   /**<    bYseparateWithUV is false */
                                   /**< 2. it's Y buffer handle when */
                                   /**<    bYseparateWithUV is true */
    UINT32      uiESType;          /**< Element stream codec type. */
    UINT32      uiNumExtraBD;      /**< extra BD allocated */
    UINT32      uiNumTotalBD;      /**< total BD allocated */

    /**
    *  data format of the frame buffer
    */
    UINT32      uiSrcFmt;          /**< ARGB, YCBCR, .. etc. */
    UINT32      uiOrder;           /**< ARGB, ABGR, BGRA, ... etc. */
    UINT32      uiBpp;             /**< number of bytes per pixel. */

    /**
    *  data size of the frame buffer
    */
    UINT32      uiBufStride;       /**< line stride (in bytes). */
    UINT32      uiContentW;        /**< picture full content width. */
    UINT32      uiContentH;        /**< picture full content height. */
    INT32       iContentOff;       /**< offset (in bytes) of picture
                                    * full content in frame buffer. */
    UINT32      uiBufSize;         /**< total size of Y+UV. */
    UINT32      uiBufSizeUv;       /**< UV size of seperate pixel. */

    /**
    *  active window size of the frame buffer
    */
    INT32       iActiveX;          /**< x-coordination (in pixels) of active
                                    * window top left in reference window. */
    INT32       iActiveY;          /**< y-coordination (in pixels) of active
                                    * window top left in reference window. */
    UINT32      uiActiveW;         /**< width of active in pixels. */
    UINT32      uiActiveH;         /**< height of active data in pixels. */
    INT32       iActiveOff;        /**< Offset (in bytes) of active data
                                    * to be displayed. */

    /**
    *  aspect ratio width and height
    */
    UINT32      uiSarWidth;
    UINT32      uiSarHeight;

    /**
    *  per-frame scaling information
    */
    INT32       iSrcWinX;          /**< source window x-coordination. */
    INT32       iSrcWinY;          /**< source window y-coordination. */
    UINT32      uiSrcWinW;         /**< source window width. */
    UINT32      uiSrcWinH;         /**< source window hegith. */
    INT32       iDestWinX;         /**< destination window x-coordin. */
    INT32       iDestWinY;         /**< destination window y-coordin. */
    UINT32      uiDestWinW;        /**< destination window width. */
    UINT32      uiDestWinH;        /**< destination window hegith. */

    /**
    *  time stamp of the frame buffer
    */
    UINT32      uiPtsHigh;         /**< bit 33 of pts. */
    UINT32      uiPtsLow;          /**< bit 0-32 of pts. */
    UINT32      uiRawPtsLow;       /**< pts in userdata of raw es. */

    /**
    *  video frame information
    */
    UINT32      uiIsFs;         /**< none zero: frame-only sequence. */
    UINT32      uiIsTff;        /**< top field first, only apply to
                                 * interlaced frame,
                                 * none zero: top field first out. */
    UINT32      uiIsRff;        /**< repeat first field, only apply to
                                 * interlaced frame,
                                 * none zero: repeat first field. */
    UINT32      uiIsPp;         /**< none zero: progressive picture. */
    UINT32      uiIsIwm;        /**< interlace weave mode, indicate whether
                                 * weave mode used for interlace. */
    UINT32      uiIsPs;         /**< progressive stream, indicate whether
                                 * it is a progressive stream,
                                 * it's different with amp_frame_is_fs
                                 * because amp_frame_is_fs may be changed
                                 * based on film detection. */
    UINT32      uiPar;          /**< pixel aspect ratio(PAR) 0: unknown,
                                 * High[31:16] 16bits,Numerator(width),
                                 * Low[15:0] 16bits, Denominator(heigh). */
    UINT32      uiFrNum;        /**< Numerator of frame rate if available. */
    UINT32      uiFrDen;        /**< Denominator if frame rate if available. */
    UINT32      uiFrType;       /* < Frame type: I/P/B >*/

    /**
    *  graphic frame color look-up table
    */
    UINT32      uiPreMult;      /**< none zero: pre-multiplied frame. */
    UINT32      uiClutKey;      /**< shm key of color lookup table for CI8
                                 * valid when element amp_frame_bpp==1. */
    UINT32      uiClutStart;    /**< start index in IO map to copy the clut. */
    UINT32      uiClutLength;   /**< length of the clut,
                                 * number of clut entries. */

    /**
    *  3D information
    */
    UINT32      ui3dRvValid;    /**< none zero: right eye view valid. */
    UINT32      p3dRvStart;     /**< the physical address for right view. */
    UINT32      p3dRvBd;        /**< used to recycle the right view bd. */
    UINT32      ui3dSourceFmt;  /**< format of the 3D frame, frame packing
                                 * side by side / top and bottom. */
    UINT32      ui3dIntpre;     /**< interpretation type. */
    UINT32      ui3dSbsSm;      /**< sbs sampling mode. */
    UINT32      ui3dSeiCv;      /**< convert_mode from SEI message. */
    UINT32      ui3dSwitchView; /**< switch left and RightView. */

    /**
    * 4K frame support
    */
    UINT32      uiBitsPerPixel; /**< bit number per pixel. */
    UINT32      uiPbufStartUV;  /**< start Address of UV data in 420SP format. */
    UINT32      uiBufStrideUV;  /**< stride of UV data in 420SP format. */

    /**
    *  notify AVS to enable IO sync;
    */
    UINT8       uiDisableIOsyncFlag; /**< 0: enable; 1: disable */

    /**
    *  user data tagged to the frame buffer;
    */
    UINT32      uiXvYCCValid;   /**< xyYCC data is in user data */
    UINT32      uiIsUHDframe;

    UINT32      uiBufFlag;
    UINT32      uiGFXFrameFlag;
    UINT8       early_disp;     /**< turbo low latency. */

    /**
    * HDR information for HDMI spec
    */
    AMP_HDR_INFO sHdrInfo;

    /**
     * Technicolor HDR information
     **/
    UINT32      uiThdrPresentMode;    /** Whether techincolor is present or not   **/
    AMP_BD_ST   thdrMetaBD;             /** Pointer to technicolor metadata **/
    INT32     m_iDisplayOETF;
    INT32     m_iPeakLuminance;
    INT32     m_iProcessMode;


    UINT32      uibuiltinFrame;
    /**
     * Dolby HDR information
     **/
    UINT32      uiDvPresentMode; /**< Whether the dolby vision is present and the configuatation */
                                 /**< use bit-wise mask to specify the presence of each elements */
                                 /**< bit 0: BL present bit 1: EL present, bit 2: Meta Data present */
                                 /**< all others bits are reserved */
    AMP_BD_ST   DvElBD;          /**< The enhance layer BD pointer. if zero, it means not present. */
    AMP_BD_ST   DvRpuBD;         /**< The parsed Rpu (meta data) BD pointer. if zero, it means not present. */
    UINT32      uiDvStatus;      /**< Exception flag bits for dolby vision */
                                 /**< current definitions: bit 0: 1-the meta data is invalid (error); 0-OK  */
                                 /**< all other bits are reserved. */
    /**
     * MTR information
    **/
    UINT32      uiIsMtr;         /**< whether this BD is in compressed format or not */
    UINT32      uiLumaLeftOfst;
    UINT32      uiLumaTopOfst;
    UINT32      uiChromaLeftOfst;
    UINT32      uiChromaTopOfst;
    UINT32      uiLumaMtrrCfg[7];        /**< MTRR configure for luma */
    UINT32      uiChromaMtrrCfg[7];      /**< MTRR configure for chroma */
    AMP_SHM_HANDLE hShmMeta;             /**< shm handle for meta buffer */

    /**
      * Extra frame for Tile&420SP Auto Mode and Tile Multi mode
      */
    BOOL        uiIsExtFrameValid;
    UINT32      uiSrcFmt1;      /**< ARGB, YCBCR, .. etc. */
    UINT32      pBufStart1;    /**< base address of buffer */
    UINT32      uiPbufStartUV1; /**< start Address of UV data in 420SP format. */
    UINT32      uiBufStrideUV1; /**< stride of UV data in 420SP format. */
    UINT32      uiBufStride1;   /**< line stride (in bytes). */
    UINT32      uiVScaling;     /**< 2 indicate 1/2 420sp downscale,4 indicate 1/4 */
    UINT32      uiHScaling;     /**< 2 indicate 1/2 420sp downscale,4 indicate 1/4 */

    BOOL bYseparateWithUV;  /**< TRUE: Y and UV are 2 separate buffer */
                            /**< FALSE: Y and UV are in one buffer */
    AMP_SHM_HANDLE hUvShm;  /**< It's UV buffer handle when bYseparateWithUV */
                            /**< is true, the value is invalid in other case */
    UINT8      uiBufSrc;  /**< Hold the ID of buffer source */
} AMP_BGTAG_FRAME_INFO;

/** \struct AMP_BDTAG_STREAM_INFO
 *  \brief stream information for video stream.
 */
typedef struct {
    AMP_BDTAG_H Header;            /**< The header of BD Tag */

    /**
    *  bd tag body
    */
    UINT32      uType;             /**< The type of the frame */
    UINT32      uiPtsHigh;         /**< bit 33 of pts. */
    UINT32      uiPtsLow;          /**< bit 0-32 of pts. */
    UINT32      uUnitFlag;         /**< The start and end unit flag */
    UINT32      isHeader;          /**< 1: It's header; 0: Not */
    UINT32      uiWidth;
    UINT32      uiHeight;
} AMP_BDTAG_STREAM_INFO;

typedef union {
    UINT32  uFlag;
    struct {
        UINT32  uMode     : 3;
        UINT32  uHBR      : 1;
        UINT32  uFsCod    : 5;
        UINT32  uMulFs    : 4;
        UINT32  uChanNr   : 4;
        UINT32  uFmt      : 7;
        UINT32  uRatio    : 4;
        UINT32  uInvalid  : 1;
        UINT32  uRes      : 3;
    } stCfg;
} AMP_AUD_HDMI_INFO;

typedef union {
    UINT32  uFlag;
    struct {
        UINT32 uMode        : 3;
        UINT32 uFsCod       : 5;
        UINT32 uChanNr      : 4;
        UINT32 uFmt         : 7;
        UINT32 uDigitalData : 1;
        UINT32 uCopyRight   : 1;
        UINT32 uOriginal    : 1;
        UINT32 uRatio       : 4;
        UINT32 uRes         : 6;
    } stCfg;
} AMP_AUD_SPDIF_INFO;

typedef union {
    UINT32  uFlag;
    struct {
        UINT32  uFmt        : 7;
        UINT32  uFsCod      : 5;
        UINT32  uChanNr     : 4;
        UINT32  uBitRate    : 8;
        UINT32  uRes        : 8;
    } stInfo;
} AMP_AUD_COMP_ES_INFO;

typedef struct {
    UINT64      uiInBDPts;          /**< lastest pts of input BD */
    UINT32      uiInputFullness;    /**< byte fullness */
    UINT32      uiOutputFullness;   /**< sample fullness */
    UINT16      unInputBD;          /**< number of BDs in idle and busy chain */
    UINT16      unOutputBD;
} AMP_ADEC_AVSYNC_INFO;

typedef struct {
    UINT32      uiMixMask;
    UINT32      uiLinkMask;
} AMP_MIXER_AVSYNC_INFO;

typedef union {
    AMP_ADEC_AVSYNC_INFO    stAdecInfo;
    AMP_MIXER_AVSYNC_INFO   stMixerInfo;
} AMP_AUD_AVSYNC_INFO;

typedef struct {
    UINT32      uiMagicNum;         /**< BDTAG_MAGIC_NUM for valid */
    UINT32      uiDownMixType;      /**< DTS Neural down mix type*/
    UINT32      uiChannelCfg;       /**< DTS Neural original audio configure ID */
} AMP_AUD_NEURAL_INFO;

/** \struct AMP_BDTAG_AUD_FRAME_INFO
*   \brief This tag is to transfer PCM or bit-stream data among audio modules.
*/
typedef struct {
    AMP_BDTAG_H Header;            /**< The header of BD Tag */
    /**
     * bd tag body
     */
    UINT32      uDataFmt;                       /**< The data format contained
                                                 * in the tag */
    UINT32      uDataCfg[2];                    /**< config of hdmi/spdif/etc */
    UINT16      uBufNr;                         /**< The buffer number within
                                                 * the tag */
    UINT16      uCfgIdx;                        /**< index of valid uDataCfg */
    UINT32      uChanNr;                        /**< The channel number within
                                                 * the tag */
    UINT32      uOutMaxNr;                      /**< Multi-path Max Channel */
    UINT32      uMemHandle;                     /**< The value of BD tag */
    UINT32      uMemOffset[AMP_AUD_MAX_BUF_NR]; /**< The start offset of each
                                                 * data buffer */
    UINT32      uDataMask[AMP_AUD_MAX_BUF_NR];  /**< The mask of each data
                                                 * buffer*/
    UINT32      uSize;                          /**< The total size of buffer */
    UINT32      uDataLength;                    /**< Length of data in buffer */
    UINT32      uBitDepth;                      /**< The bitdepth. */
    UINT32      uPcmType;                       /**< The pcm type. */
    UINT32      uSampleRate;                    /**< Sample rate. */
    UINT32      uBitRate;                       /**< Bit rate. */
    UINT32      uFlag;                          /**< the flag of memory space */
    UINT32      uAdecFmt;                       /**< ADEC decoder format */

    UINT32      uPushTime;                      /**< the flag of time stamp */
    UINT32      uNumTotalBD;                    /**< total BD allocated */

    AMP_AUD_AVSYNC_INFO stAVSyncInfo;
    AMP_AUD_NEURAL_INFO stNeuralInfo;
} AMP_BDTAG_AUD_FRAME_INFO;

/** \struct AMP_BDTAG_AUD_STREAM_INFO
*   \brief This tag is to transfer audio info for decoder.
*/
typedef struct {
    AMP_BDTAG_H Header;                         /**< The header of BD Tag */
    UINT32      uiMagicNum;                     /**< Magic number */
    UINT32      uiFormat;                       /**< audio format */
    UINT32      uiSampRate;                     /**< Sample rate */
    UINT32      uiBitRate;                      /**< Bit rate*/
    UINT8       cBitDepth;                      /**< Bit depth */
    UINT8       cChannelNum;                    /**< Channel number */
} AMP_BDTAG_AUD_STREAM_INFO;

/** \struct AMP_BDTAG_AUD_STREAM_CHANGE
*   \brief This tag is to indicate whether audio stream is changed. No use now.
*/
typedef struct {
    AMP_BDTAG_H Header;                         /**< The header of BD Tag */
    UINT8       cChangeIdx;                     /**< The index of change */
    UINT32      uiNewValue;                     /**< The new value*/
} AMP_BDTAG_AUD_STREAM_CHANGE;

/** \struct AMP_BDTAG_AUD_METADATA
*   \brief This tag is to indicate whether audio stream is changed. No use now.
*/
typedef struct {
    AMP_BDTAG_H Header;             /**< The header of BD Tag */
    /**
     * bd tag body
     */
    UINT32      uMetaDataType;      /**< The data format contained
                                      * in the tag */
    UINT32      uMemHandle;         /**< The value of BD tag */
    UINT32      uMemOffset;         /**< The start offset of each
                                      * data buffer */
    UINT32      uMetaDataSize;
} AMP_BDTAG_AUD_METADATA;

#define AMP_MS12_METADATA_MAX_PAYLOADS 3072 /* max evo frame */
/** \struct AMP_MS12_METADATA
*   \brief Dolby MS metadata struct
*/
typedef struct {
    INT32           iId;     /**< The ID is unique and determines how to interpret the payload pointer */
    INT32           iType;   /**< The type is either MD_TYPE_STATIC or MD_TYPE_TIMED and determined if the
                     time information in offset and length shall be interpreted or not. */
    INT32           iOffset;   /**< The offset relates to the PCM buffer to be retrieved next together with the metadata.
                        It is given in samples and shall be adjusted if metadata is processed or passed on. */
    INT32           iLength;   /**< The length relates to the duration that the metadata is valid relative to the offset. */
    INT32           iPayloadSize; /**< Size of the payload in bytes */
    VOID            *iPayload; /**< The pointer to the payload shall be casted to the proper type in case the id is known. */
} AMP_MS12_METADATA;

#define AMP_BDTAG_MAX_MS12_METADATA   64
/** \struct AMP_BDTAG_AUD_MS12_METADATA
*   \brief AMP_MS12_METADATA container
*/
typedef struct {
    AMP_BDTAG_H     Header;         /**< The header of BD Tag */
    /**
     * bd tag body
     */
    UINT32          uMetaDataNum;
    AMP_SHM_HANDLE  hShm;
    AMP_MS12_METADATA stMetaData[AMP_BDTAG_MAX_MS12_METADATA];
} AMP_BDTAG_AUD_MS12_METADATA;

/** \struct AMP_BDTAG_AUD_REALAUDIO
*   \brief This tag is to inform decoder the decode info in this BD.
*/
typedef struct {
    AMP_BDTAG_H Header;             /**< The header of BD Tag */
    /**
     * bd tag body
     */
    UINT32      m_BlockFlag;      /**< BlockFlag*/
    UINT32      m_BlockSize;      /**< BlockSize */
    UINT32      m_TimeStamp;      /**< TimeStamp*/
} AMP_BDTAG_AUD_REALAUDIO;

/** \struct AMP_BDTAG_AUD_NEURAL_DESC
*   \brief This tag is to transfer DTS Neural info.
*/
typedef struct {
    AMP_BDTAG_H         Header;        /**< The header of BD Tag */
    UINT32              uiDownMixType; /**< Moved to stInfo, will be removed*/
    AMP_AUD_NEURAL_INFO stInfo;        /**< DTS Neural info*/
} AMP_BDTAG_AUD_NEURAL_DESC;

/** \enum AMP_EXTINFO_AUD_T
 *  \brief Enumerate all types of audio extra info.
 */
typedef enum {
    AMP_EXTINFO_AUD_AAC_ESDS   = 0x00000001,   /**< for AAC ESDS from OMX */
} AMP_EXTINFO_AUD_T;

/** \struct AMP_BDTAG_AUD_EXTINFO
 *  \brief This tag is used to cover the memory info, which incldues the memory
 *         handle and the start offset.
 */
typedef struct {
    AMP_BDTAG_H Header;             /**< The header of BD Tag */

    /**
     * bd tag body
     */
    UINT32      uMemHandle;        /**< The handle of memory space */
    UINT32      uMemOffset;        /**< The start offset of memory space */
    UINT32      uSize;             /**< The size of memory space */
    UINT32      uFlag;             /**< The type of audio extra info */
} AMP_BDTAG_AUD_EXTINFO;


/** \struct AMP_BDTAG_AVS_PTS
 *  \brief This tag is used to send AVSync PTS info with BD.
 *
 */
typedef struct {
    AMP_BDTAG_H Header;             /**< The header of BD Tag */

    /**
     * bd tag body
     */
    UINT32      uPtsHigh;        /**< The validity mask (bit31),
                                  * discontinuity indicator (bit30)
                                  * and the MSB of PTS. */
    UINT32      uPtsLow;         /**< The LSB of PTS. */
} AMP_BDTAG_AVS_PTS;

#define AMP_AUD_MAX_STRM    5
typedef struct {
    AMP_BDTAG_H Header;             /**< The header of BD Tag */

    /**
     * bd tag body
     */
    UINT32      uPtsMask;
    UINT32      uPtsHigh[AMP_AUD_MAX_STRM]; /**< The validity mask (bit31),
                                              * discontinuity indicator (bit30)
                                              * and the MSB of PTS. */
    UINT32      uPtsLow[AMP_AUD_MAX_STRM];  /**< The LSB of PTS. */
} AMP_BDTAG_AMIX_PTS_INFO;

/** \struct AMP_BDTAG_AUD_GRAPH_PTS
 *  \brief This tag is used to pass the PTS info through the audio graph.
 *
 */
typedef struct {
    AMP_BDTAG_H Header;             /**< The header of BD Tag */

    /**
     * bd tag body
     */
    UINT32      uiStreamMask;                   /**< Bitwise ORed bits indicating
                                                    which streams are linked. */
    UINT32      uiUpdateMask;                   /**< Bitwise ORed bits indicating
                                                    which streams have PTS/data update. */
    UINT32      uiMixStreamMask;                /**< Use to indicate which stream are mixed */
    UINT32      uiPtsHigh[AMP_AUD_STREAMS_MAX]; /**< The validity mask (bit31),
                                                  * discontinuity indicator (bit30)
                                                  * and the MSB of PTS. */
    UINT32      uiPtsLow[AMP_AUD_STREAMS_MAX];  /**< The LSB of PTS. */
} AMP_BDTAG_AUD_GRAPH_PTS;


/** \struct AMP_BDTAG_AUD_LOCAL_PTS
 *  \brief This tag is used to pass the PTS info through the audio pipeline.
 *
 */
typedef struct {
    AMP_BDTAG_H Header;             /**< The header of BD Tag */

    /**
     * bd tag body
     */
    /**< Bitwise ORed bits indicating
     * which streams are contained. */
    UINT32      uiStreamMask;
    UINT32      uiSamples;
    /**<  stream counter for each sndsink port */
    UINT32      uiStreamCnt[AMP_AUD_STREAMS_MAX];
    /**<  start pts of bd */
    double      pts[AMP_AUD_STREAMS_MAX];
    /**<  time length of one single sample in millisecond
     * singleSampleMs may not equal 1/samplerate in many cases ! */
    double      singleSampleMs[AMP_AUD_STREAMS_MAX];
} AMP_BDTAG_AUD_LOCAL_PTS;

/** \struct AMP_BDTAG_AUD_ROUTE
*   \brief This tag is to indicate which sinks to route.
*/
typedef struct {
    AMP_BDTAG_H Header;                         /**< The header of BD Tag */
    BOOL bToSinks[AMP_AUD_SINKS_MAX];
} AMP_BDTAG_AUD_ROUTE;

/** \struct AMP_BDTAG_VBI_META_INFO
 *  \brief This tag is used to send VBI stream info with BD.
 *
 */
typedef struct {
    AMP_BDTAG_H Header;             /**< The header of BD Tag */

    /**
     * bd tag body
     */
    UINT32      isTop;        /**< field status of interlace mode */
} AMP_BDTAG_VBI_META_INFO;


/** enumerate all buffer status flag masks supported in AMP. */
typedef enum {
    AMP_BUFFSTAT_FLAG_UNDERFLOW_MASK   = 0x00000001,/**< Buffer Underflow */
    AMP_BUFFSTAT_FLAG_OVERFLOW_MASK   = 0x00000002, /**< Buffer Overflow */
    AMP_BUFFSTAT_FLAG_EMPTY_MASK   = 0x00000004,    /**< Buffer Empty */
    AMP_BUFFSTAT_FLAG_FULL_MASK   = 0x00000008,     /**< Buffer Full */
} AMP_BUFFSTAT_MASK;

/** \struct AMP_BDTAG_BUFF_STATUS
 *  \brief This tag is used to indicate the status of buffer.
 *
 */
typedef struct {
    AMP_BDTAG_H Header;         /**< The header of BD Tag */

    /**
     * bd tag body
     */
    UINT32      uBuffTag;        /**< The tag of buffer */
    UINT32      uBuffStat;       /**< The status of buffer */
} AMP_BDTAG_BUFFSTAT_INFO;

#define AMP_BDTAG_FRAMEINFO_INIT(pFrameInfo)                                \
    do {                                                                    \
        AmpMemClear((pFrameInfo), sizeof(AMP_BGTAG_FRAME_INFO));            \
        (pFrameInfo)->Header.eType   = AMP_BGTAG_FRAME_INFO_META;           \
        (pFrameInfo)->Header.uLength = sizeof(AMP_BGTAG_FRAME_INFO);        \
        (pFrameInfo)->uiClutKey      = -1;                                  \
    } while (0)

#define AMP_BDTAG_STREAMINFO_INIT(pStreamInfo)                               \
    do {                                                                     \
        AmpMemClear((pStreamInfo), sizeof(AMP_BDTAG_STREAM_INFO));           \
        (pStreamInfo)->Header.eType   = AMP_BDTAG_STREAM_INFO_META;          \
        (pStreamInfo)->Header.uLength = sizeof(AMP_BDTAG_STREAM_INFO);       \
    } while (0)

/** \struct AMP_BDTAG_INDICATOR_INFO
 *  \brief This tag is used to indicate the tag of strm that associated with this buffer.
 *
 */
typedef struct {
    AMP_BDTAG_H Header;         /**< The header of BD Tag */

    /**
     * bd tag body
     */
    UINT32      uTag;        /**< The tag that associated with uStrmTag of stream */
} AMP_BDTAG_INDICATOR_INFO;

/** \struct AMP_BDTAG_TS_ERR_INFO
 *  \brief This tag is used to indicate the number of errors encountered in TS
 *  when DMX parsing the data contained in this buffer.
 *  Currently supported error type:
 *      AMP_BDTAG_BS_TRANSPORTERROR_ctrl indicates the error bit is set in TS header
 *      AMP_BDTAG_BS_CONTINUITYERROR_CTRL indicates the TS packet continuity counter is wrong
 *
 */
typedef struct {
    AMP_BDTAG_H Header;         /**< The header of BD Tag */

    /**
     * bd tag body
     */
    UINT32      uErrCnt;        /**< number of errors of the specified type encountered*/
} AMP_BDTAG_TS_ERR_INFO;

/** \struct AMP_BDTAG_ISPSENSOR_INFO
 *  \brief This tag is used to provide the sensor information
 *
 */
typedef struct {
    AMP_BDTAG_H Header;         /**< The header of BD Tag */

    /**
     * bd tag body
     */
    UINT32      uSensorIndex;        /**< Sensor Index = AMP_ISP_INPUT_SENSOR1/SENSOR2 */
    UINT32      uRotationAngle;
} AMP_BDTAG_ISPSENSOR_INFO;

#endif
