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
//! \file amp_dmx_bdtag.h
//!
//! \brief This file used to define all the control info and meta data with the
//!        tag structure for DMX component.
//!
//! Purpose:
//!
//!
//! Version, Date and Author :
//!   V 1.00,    Jun 29 2012,  Weizhao Jiang
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////


#ifndef __DMX_BDTAG_H__
#define __DMX_BDTAG_H__

/************* Header file include ********************************************/
#include "amp_types.h"
#include "isl/amp_bd_tag.h"

/************* Macros, type definitions****************************************/
/** enumerate all tag type of dmx control type in AMP. */
typedef enum _ampdmx_ctrl_t_ {
    AMPDMX_CTRL_INVALID_TYPE         = 0x0, /**< Invalid DMX control */
    AMPDMX_CTRL_WM_PAYLOAD_TYPE      = 0x1, /**< WM payload control */
    AMPDMX_CTRL_PR_PAYLOAD_TYPE      = 0x2, /**< Playready payload control */
    AMPDMX_CTRL_ML_PAYLOAD_TYPE      = 0x3, /**< Marlin payload control */
    AMPDMX_CTRL_WV_PAYLOAD_TYPE      = 0x4, /**< Widevine payload control */
    AMPDMX_CTRL_HLS_PAYLOAD_TYPE     = 0x5, /**< HLS payload control */
    AMPDMX_CTRL_RC_PAYLOAD_TYPE      = 0x6, /**< Reenctryption payload control*/
    AMPDMX_CTRL_HDCP_PAYLOAD_TYPE    = 0x7, /**< HDCP payload control*/
    AMPDMX_CTRL_DTCPIP_PAYLOAD_TYPE  = 0x8, /**< DTCPIP payload control*/
    AMPDMX_CTRL_TYPE_MAX
} AMPDMX_CTRL_T;

typedef enum _dmx_ctrl_t_ {
   //! payload ctrl info
    eDRM_PAYLOAD_CTRL_BEGIN           = 0x10,
    eDRM_PLAYREADY_PAYLOAD_CTRL       = 0x11,
    eDRM_MARLIN_PAYLOAD_CTRL          = 0x12,
    eDRM_WIDEVINE_PAYLOAD_CTRL        = 0x13,
    eDRM_HLS_PAYLOAD_CTRL             = 0x14,
    eDRM_REENCRYPT_PAYLOAD_CTRL       = 0x15,
    eDRM_HDCP_PAYLOAD_CTRL            = 0x16,
    eDRM_DTCPIP_PAYLOAD_CTRL          = 0x17,
    eDRM_MOBITV_PAYLOAD_CTRL          = 0x18,
    eDRM_PAYLOAD_CTRL_END,

    //! control info from bitstream
    eBS_UNITSTART                     = 0xa0,
    eBS_UNITEND                       = 0xa1,
 }  DMX_CTRL_T;

/** \struct AMPDMX_TIMESTAMP
 *  \brief Definition for DMX time stamp in AMP.
 */
typedef struct {
    UINT32    uiStampHigh;          /**< The highest bit indicates the time
                                         stamp is valid or not.*/
    UINT32    uiStampLow;           /**< The 32bits of 33-bits time stamp */
} AMPDMX_TIMESTAMP;

/** \struct AMPDMX_CTRL_DUMMY_INFO
 *  \brief Definition for DMX dummy ctrl in AMP.
 */
typedef struct {
    UINT32    m_Rsvd;               /**< Reserved */
} AMPDMX_CTRL_DUMMY_INFO;

/** \struct AMPDMX_CTRL_BDINDICATOR
 *  \brief Definition for DMX dummy ctrl in AMP.
 */
typedef struct {
    UINT32    m_Size;               /**< Reserved */
} AMPDMX_CTRL_BDINDICATOR;

/** \struct AMPDMX_CTRL_UNITSTART
 *  \brief Definition for DMX unit start ctrl in AMP.
 */
typedef struct {
    AMPDMX_TIMESTAMP    m_PtsValue; /**< The highest bit indicates the time
                                         stamp is valid or not.*/
    AMPDMX_TIMESTAMP    m_DtsValue; /**< The 32bits of 33-bits time stamp */
} AMPDMX_CTRL_UNITSTART;

/** \struct AMPDMX_CTRL_HDR
 *  \brief Definition for DMX unit Hdr ctrl in AMP.
 */
typedef struct {
    void* pMDBuf;                   /**< hdr meta data buffer*/
} AMPDMX_CTRL_HDR;

/** \struct AMPDMX_CTRL_UNITEND
 *  \brief Definition for DMX unit end ctrl in AMP.
 */
typedef struct {
    UINT32    m_Rsvd;               /**< Reserved */
} AMPDMX_CTRL_UNITEND;

/** \struct AMPDMX_CTRL_CONTINUITYERROR
 *  \brief Definition for DMX continuity error ctrl in AMP.
 */
typedef struct {
    UINT32    m_Rsvd;               /**< Reserved */
} AMPDMX_CTRL_CONTINUITYERROR;

/** \struct AMPDMX_CTRL_CRCERROR
 *  \brief Definition for DMX crc error ctrl in AMP.
 */
typedef struct {
    UINT32    m_Rsvd;               /**< Reserved */
} AMPDMX_CTRL_CRCERROR;


/** \struct AMPDMX_CTRL_VESHDR_INFO
 *  \brief Definition for DMX VES Header info in AMP.
 */
typedef struct {
    UINT32    m_HdrLen;               /**< Header Length */
    UINT32    m_HdrData[4];             /**< Header Data, 16 bytes */
}AMPDMX_CTRL_VESHDR_INFO;


/** \struct AMPDMX_CTRL_PCRINFO
 *  \brief Definition for DMX PCR info ctrl in AMP.
 */
typedef struct {
    AMPDMX_TIMESTAMP    m_PcrValue; /**< The value of PCR */
    AMPDMX_TIMESTAMP    m_StcValue; /**< The value of STC */
} AMPDMX_CTRL_PCRINFO;

/** \struct AMPDMX_CTRL_DISCONT_INDICATE
 *  \brief Definition for DMX discontinue indicate ctrl in AMP.
 */
typedef struct {
    UINT32    m_PcrPid;             /**< The PCR PID */
    UINT32    m_StartPts;           /**< The 32bits of start pts */
    UINT32    m_EndPts;             /**< The 32bits of end pts */
} AMPDMX_CTRL_DISCONT_INDICATE;

/** \struct AMPDMX_CTRL_AUDIO_DISCRIPTOR
 *  \brief Definition for DMX AUDIO_DISCRIPTOR ctrl in AMP.
 */
typedef struct {
    UINT32	 ADFadeByte		    :  8; /**< The ADFadeByte */
    UINT32   ADPanByte		    :  8; /**< The ADPanByte */
    UINT32	 ADGainByteCenter	:  8; /**< The ADGainByteCenter*/
    UINT32	 ADGainByteFront	:  8; /**< The ADGainByteFront */

    UINT32	 ADGainByteSurround :  8; /**< The ADGainByteSurround */
    UINT32	 Reserved			: 24; /**< The Reserved */
} AMPDMX_CTRL_AUDIO_DISCRIPTOR;


/** \struct AMPDMX_CTRL_PCR_JUMP
 *  \brief Definition for DMX pcr jump ctrl in AMP.
 */
typedef struct {
    UINT32    m_Rsvd;               /**< Reserved */
} AMPDMX_CTRL_PCR_JUMP;

/** \struct AMPDMX_CTRL_TIMEBASE_NOTIF
 *  \brief Definition for DMX timebase notify ctrl in AMP.
 */
typedef struct {
    UINT32    m_IsNewTimeBase;     /**< The new time base */
} AMPDMX_CTRL_TIMEBASE_NOTIF;

/** \struct AMPDMX_CTRL_SCRAMBLE_CHG
 *  \brief Definition for DMX scambler change ctrl in AMP.
 */
typedef struct {
    UINT32    m_NewMode;            /**< The new scramble mode */
} AMPDMX_CTRL_SCRAMBLE_CHG;

/** \struct AMPDMX_CTRL_WM_PAYLOAD
 *  \brief Definition for DMX wm payload ctrl in AMP.
 */
typedef struct
{
    UINT32      m_IsEncrypted;      /**< Indicate it's encrypted or not */
    UINT32      m_PayloadSize:32;   /**< Payload size */
    UINT32      m_Seed[2];          /**< Seed[2] */
}AMPDMX_CTRL_WM_PAYLOAD;

/** \struct AMPDMX_CTRL_PLAYREADY_PAYLOAD
 *  \brief Definition for DMX payready payload ctrl in AMP.
 */
typedef struct {
    UINT32      m_IsEncrypted;      /**< Indicate it's encrypted or not */
    UINT32      m_PayloadSize;      /**< Payload size */
    UINT64      SampleID;           /**< Sample ID*/
    UINT64      BlockOffset;        /**< Bloock offset */
    UINT16      ByteOffset;         /**< Bytes offset */
}AMPDMX_CTRL_PLAYREADY_PAYLOAD;

/** \struct AMPDMX_CTRL_MOBITV_PAYLOAD
 *  \brief Definition for DMX MOBITV payload ctrl in AMP.
 */
typedef struct {
    UINT32 m_PayloadSize;         /**< Indicate it's payload */
    UINT32 m_IsEncrypted;         /**< Indicate it's encrypted or not */
    UINT32 m_bInitIV:1;             /**< Indicate INIT IV flag*/
    UINT32 m_Rsvd:15;             /**< Reserved 15 bits*/
    UINT32 m_PatternEnc:8;        /**< Indicate encrypted data*/
    UINT32 m_PatternClr:8;        /**< Indicate clear data*/
    UINT8  m_IVValue[16];         /**< Indicate it's IV value */
}AMPDMX_CTRL_MOBITV_PAYLOAD;


/** \struct AMPDMX_CTRL_MARLIN_PAYLOAD
 *  \brief Definition for DMX Marlin payload ctrl in AMP.
 */
typedef struct {
    UINT32 m_IsEncrypted;           /**< Indicate it's encrypted or not */
    UINT32 m_PayloadSize;           /**< Payload size */
    UINT8  m_IVKey[16];             /**< The IV Key*/
}AMPDMX_CTRL_MARLIN_PAYLOAD;

/** \struct AMPDMX_CTRL_HLS_PAYLOAD
 *  \brief Definition for DMX HLS payload ctrl in AMP.
 */
typedef struct {
    UINT32 m_IsEncrypted;           /**< Indicate it's encrypted or not */
    UINT32 m_PayloadSize;           /**< Payload size */
    UINT8  m_IVKey[16];             /**< The IV Key*/
}AMPDMX_CTRL_HLS_PAYLOAD;

/** \struct AMPDMX_CTRL_WIDEVINE_PAYLOAD
 *  \brief Definition for DMX widevine payload ctrl in AMP.
 */
typedef struct {
    UINT32 m_IsEncrypted;           /**< Indicate it's encrypted or not */
    UINT32 m_PayloadSize;           /**< Payload size */
    UINT8  m_IVKey[16];             /**< The IV Key*/
}AMPDMX_CTRL_WIDEVINE_PAYLOAD;

/** \struct AMPDMX_CTRL_HDCP_PAYLOAD
 *  \brief Definition for DMX HDCP payload ctrl in AMP.
 */
typedef struct {
    UINT32 m_IsEncrypted;           /**< Indicate it's encrypted or not */
    UINT32 m_PayloadSize;           /**< Payload size */
    UINT32 m_StreamCtr;             /**< The Ctr of stream */
    UINT32 m_InputCtr[2];           /**< The Ctr of input */
    UINT32 m_Padding0;              /**< Padding */
    UINT32 m_IsAudio;               /**< LPCM Case, 1: Audio, 0: Video */
}AMPDMX_CTRL_HDCP_PAYLOAD;

/** \struct AMPDMX_CTRL_DTCPIP_PAYLOAD
 *  \brief Definition for DMX dtcpip payload ctrl in AMP.
 */
typedef struct {
    UINT32 m_IsEncrypted;           /**< Indicate it's encrypted or not */
    UINT32 m_PayloadSize;           /**< Payload size */
    UINT64 Nc;                      /**< NC */
    UINT32 EmiMode;                 /**< The EMI mode */
}AMPDMX_CTRL_DTCPIP_PAYLOAD;

/** \struct AMPDMX_CTRL_CRYPTO_PAYLOAD
 *  \brief Definition for DMX crypto payload ctrl in AMP.
 */
typedef struct {
    AMP_DRM_CRYPTO_ST   CryptInfo;      /**< The crypto info*/
}AMPDMX_CTRL_CRYPTO_PAYLOAD;

/** \struct AMPDMX_CTRL_REENCRYPT_PAYLOAD
 *  \brief Definition for DMX reencrypto payload ctrl in AMP.
 */
typedef struct {
    UINT32 m_PayloadSize;           /**< Payload size */
} AMPDMX_CTRL_REENCRYPT_PAYLOAD;

/** \struct AMPDMX_CTRL_MAXSIZE_PAYLOAD
 *  \brief Definition for DMX ctrl with the max size in AMP.
 */
typedef struct {
    UINT32 m_Rsv[16];               /**< Not use */
} AMPDMX_CTRL_MAXSIZE_PAYLOAD;

/** \union AMPDMX_CTRL_UNION
 *  \brief Definition for DMX ctrl union in AMP.
 */
typedef union {
    AMPDMX_CTRL_DUMMY_INFO         m_DummyInfo;         /**< Dummy */
    AMPDMX_CTRL_BDINDICATOR        m_BdBoundaryIndicate;/**< Start Frame indicater */
    AMPDMX_CTRL_UNITSTART          m_UnitStart;         /**< Unit start */
    AMPDMX_CTRL_UNITEND            m_UnitEnd;           /**< Unit end */
    AMPDMX_CTRL_UNITEND            m_TransportError;    /**< Transport error */
    AMPDMX_CTRL_CONTINUITYERROR    m_ContinuityError;   /**< Continuity error */
    AMPDMX_CTRL_CRCERROR           m_CrcError;          /**< CRC error */
    AMPDMX_CTRL_VESHDR_INFO        m_VESHdr;            /**< VES header info */
    AMPDMX_CTRL_HDR                m_Hdr;               /**<  Hdr meta data info */

    AMPDMX_CTRL_PCRINFO            m_PCRInfo;           /**< PCR Info */
    AMPDMX_CTRL_DISCONT_INDICATE   m_DiscontIndicate;   /**< Discon indicate */
    AMPDMX_CTRL_AUDIO_DISCRIPTOR   m_Audio_Discriptor;  /**< Audio Discriptor*/
    AMPDMX_CTRL_PCR_JUMP           m_PCRJump;           /**< PCR Jump */
    AMPDMX_CTRL_TIMEBASE_NOTIF     m_TimeBaseNotif;     /**< Timebase notify */

    AMPDMX_CTRL_SCRAMBLE_CHG       m_ScramChg;          /**< scrambler change */

    AMPDMX_CTRL_WM_PAYLOAD         m_WmPayloadInfo;     /**< WM payload*/
    AMPDMX_CTRL_PLAYREADY_PAYLOAD  m_PrPayloadInfo;     /**< Playready payload*/
    AMPDMX_CTRL_MARLIN_PAYLOAD     m_MlPayloadInfo;     /**< Marlin payload*/
    AMPDMX_CTRL_HLS_PAYLOAD        m_HLSPayloadInfo;    /**< HLS payload*/
    AMPDMX_CTRL_WIDEVINE_PAYLOAD   m_WvPayloadInfo;     /**< widevine payload*/
    AMPDMX_CTRL_HDCP_PAYLOAD       m_HdcpPayloadInfo;   /**< HDCP payload*/
    AMPDMX_CTRL_DTCPIP_PAYLOAD     m_DtcpipPayloadInfo; /**< DTCPIP payload*/
    AMPDMX_CTRL_CRYPTO_PAYLOAD     m_CryptoPayloadInfo; /**< Crypto payload*/
    AMPDMX_CTRL_MOBITV_PAYLOAD     m_MobiTVPayloadInfo; /**< MobiTV payload*/
    AMPDMX_CTRL_MAXSIZE_PAYLOAD    m_MaxSizePayloadInfo;/**< Max size payload*/
} AMPDMX_CTRL_UNION;

/** \struct AMPDMX_CTRL_UNIT
 *  \brief Definition for DMX ctrl unit in AMP.
 */
typedef struct {
    UINT32        m_ControlInfoType    :8;      /**< Control type*/
    UINT32        m_Rsvd               :24;     /**< Reserved */
    UINT32        m_DataCount;                  /**< Data count */
    UINT32        m_PrivateDate;                /**< Private Data */
    UINT32        m_UserData;                   /**< User Data */
    AMPDMX_CTRL_UNION    m_Union;               /**< Control union */
} AMPDMX_CTRL_UNIT;

/** \struct AMP_BDTAG_DMX_CTRL_INFO
 *  \brief Definition for DMX BD TAG in AMP.
 */
typedef struct {
    AMP_BDTAG_H Header;                         /**< The header of BD Tag */
    /**
     * bd tag body
     */
    AMPDMX_CTRL_UNIT    m_CtrlUnit;             /**< Dmx control unit */
} AMP_BDTAG_DMX_CTRL_INFO;

#endif

