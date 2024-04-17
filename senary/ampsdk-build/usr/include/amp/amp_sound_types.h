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
//! \file amp_sound_types.h
//!
//! \brief sound service related functions.
//!
//!
//! Purpose:
//!
//!
//! Version     Date            Author
//!   V 1.00    Jan 8 2012      Ye Liu
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef _SOUND_TYPES_H_
#define _SOUND_TYPES_H_

///////////////////////////////////////////////////////////////////////////////
//! [Part1] Header file include
///////////////////////////////////////////////////////////////////////////////
#include "amp_types.h"
#include "isl/amp_bd_tag.h"

///////////////////////////////////////////////////////////////////////////////
//! [Part2] Declaration of external variables or functions
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//! [Part3] Local macros, type definitions
///////////////////////////////////////////////////////////////////////////////

#ifdef CONFIG_AMP_IP_AUDIO_SRS_USE_ZSP
#define SND_CLIENT_SRS_PORT         0
#define SND_MIXER_PATH_OUTPUT_MIN   1// path 0: multi path reserved for SRS
#else
#define SND_CLIENT_SRS_PORT         0
#define SND_MIXER_PATH_OUTPUT_MIN   0
#endif

#define SND_MIXER_PATH_OUTPUT_MAX   4

#define SND_INPUT_PORT_MAX          16
#define SND_CLIENT_MAX              64

#define SND_INPUT_PORT_MAX          16
#define SND_CLIENT_MAX              64
#define SND_CHANNEL_MAX             AMP_AUD_MAX_BUF_NR

/** \enum SND_E_NOMEM
    \brief Sound service error code
 */
typedef enum {
    SND_E_NOMEM                   = HRESULT_GEN(SNDSRV, ERR_NOMEM),
    SND_E_NOTREADY                = HRESULT_GEN(SNDSRV, ERR_SWSTATEWRONG),
    SND_E_NOSWRSC                 = HRESULT_GEN(SNDSRV, ERR_NOSWRSC),
    SND_E_BADSTATE                = HRESULT_GEN(SNDSRV, ERR_HWSTATEWRONG),
    SND_E_BADPARAM                = HRESULT_GEN(SNDSRV, ERR_ERRPARAM),
    SND_E_NOTPERM                 = HRESULT_GEN(SNDSRV, ERR_PRIVATE_BASE|0x1),
    SND_E_ALREADYEXIST            = HRESULT_GEN(SNDSRV, ERR_PRIVATE_BASE|0x2),
    SND_E_NOTEXIST                = HRESULT_GEN(SNDSRV, ERR_PRIVATE_BASE|0x3),
} AMP_SND_ERROR;

/** \enum AMP_SND_BITDEPTH
 *  \brief Audio Output PCM bits per samp
 */
typedef enum {
    AMP_SND_PCMS8 = 0x10008L,
    AMP_SND_PCMS16 = 0x10010L,
    AMP_SND_PCMS24 = 0x10018L,
    AMP_SND_PCMS32 = 0x10020L,
    AMP_SND_PCMU8 = 0x20008L,
    AMP_SND_PCMU16 = 0x20010L,
    AMP_SND_PCMU24 = 0x20018L,
    AMP_SND_PCMU32 = 0x20020L,
} AMP_SND_BITDEPTH;

/** \enum AMP_SND_PATH
    \brief Audio Output audio interface, value should start from 0
           and be continous. we use that as array index.
 */
typedef enum {
    AMP_SND_PATH_71 = 0,
    AMP_SND_PATH_20 = 1,
    AMP_SND_PATH_SPDIF = 2,
    AMP_SND_PATH_HDMI = 3,
    AMP_SND_PATH_BT = 4,
    AMP_SND_PATH_LOOPBACK = 5,
    AMP_SND_PATH_MAX
} AMP_SND_PATH;

/** \enum AMP_SND_CHANNEL
    \brief audio output channel
 */
typedef enum {
    AMP_SND_CHANNEL_FRONT_LEFT          = 1,
    AMP_SND_CHANNEL_FRONT_RIGHT         = 2,
    AMP_SND_CHANNEL_CENTER              = 3,
    AMP_SND_CHANNEL_LEFT_SURROUND       = 4,
    AMP_SND_CHANNEL_RIGHT_SURROUND      = 5,
    AMP_SND_CHANNEL_SUBWOOFER           = 6,
    AMP_SND_CHANNEL_LEFT_BACK           = 7,
    AMP_SND_CHANNEL_RIGHT_BACK          = 8,
} AMP_SND_CHANNEL;

/** \enum AMP_SND_SPDIF_FORMAT
    \brief SPDIF audio output format
 */
typedef enum {
    AMP_SND_SPDIF_FORMAT_INVALID        = 0,
    AMP_SND_SPDIF_FORMAT_DISABLE        = 1,
    AMP_SND_SPDIF_FORMAT_RAW_MAIN       = 2,
    AMP_SND_SPDIF_FORMAT_RAW_ENC_DTS    = 3,
    AMP_SND_SPDIF_FORMAT_RAW_ENC        = AMP_SND_SPDIF_FORMAT_RAW_ENC_DTS,
    AMP_SND_SPDIF_FORMAT_RAW_ENC_AC3    = 4,
    AMP_SND_SPDIF_FORMAT_STEREO_PCM     = 5,
    AMP_SND_SPDIF_FORMAT_MULTI_PCM_LR   = 6,
    AMP_SND_SPDIF_FORMAT_MULTI_PCM_CLFE = 7,
    AMP_SND_SPDIF_FORMAT_MULTI_PCM_LSRS = 8,
    AMP_SND_SPDIF_FORMAT_MULTI_PCM_LBRB = 9,
    AMP_SND_SPDIF_FORMAT_RAW_ENC_MP3    = 10,
    AMP_SND_SPDIF_FORMAT_RAW_MAIN_SEC   = 11,
    AMP_SND_SPDIF_FORMAT_MAX,
} AMP_SND_SPDIF_FORMAT;

/** \enum AMP_SND_HDMI_FORMAT
    \brief HDMI audio output format
 */
typedef enum {
    AMP_SND_HDMI_FORMAT_INVALID         = 0,
    AMP_SND_HDMI_FORMAT_RAW_MAIN        = 1,    /* Compressed audio data */
    AMP_SND_HDMI_FORMAT_PCM_MULTI       = 2,    /* PCM data transmittered from primary port */
    AMP_SND_HDMI_FORMAT_PCM_STREO       = 3,    /* PCM 2 stereo */
    AMP_SND_HDMI_FORMAT_DTSENC          = 4,    /* DTS Re-Encode */
    AMP_SND_HDMI_FORMAT_AC3ENC          = 5,    /* AC3 Re-Encode */
    AMP_SND_HDMI_FORMAT_MAX,
} AMP_SND_HDMI_FORMAT;

/** \enum AMP_SND_ENC_FORMAT
    \brief audio encoder output format
 */
typedef enum {
    AMP_SND_ENC_FORMAT_INVALID = 0,
    AMP_SND_ENC_FORMAT_DTS,            /* DTS Encode */
    AMP_SND_ENC_FORMAT_AC3,            /* AC3 Encode */
    AMP_SND_ENC_FORMAT_DDP,            /* DDP Encode */
    AMP_SND_ENC_FORMAT_MAT,            /* MAT Encode */
    AMP_SND_ENC_FORMAT_AAC,            /* AAC Encode */
    AMP_SND_ENC_FORMAT_OGG,            /* OGG Encode */
    AMP_SND_ENC_FORMAT_MAX,
} AMP_SND_ENC_FORMAT;

enum {
    SND_M6DB = 0x20000000,
    SND_M3DB = 0x2d3f7ced,
    SND_UNITY = 0x40000000,
    SND_M1_2DB = 0x37BDDE11,    /* -1.2dB */
    SND_M1_5DB = 0x35d13f32L,   /* 0.840896415 */
    //M3DB = 0x2d413cccL,     /* 0.707106781 */
    SND_M4_5DB = 0x260dfc13L,   /* 0.594603557 */
    SND_M6_2DB = 0x1F588942,    /* -6.2dB */
    SND_ZERO = 0,
};

#ifdef CONFIG_AMP_AUDIO_GAIN_RANGE_EXTENSION
enum {
    SND_UNITY_Q29 = 0x20000000
};
#endif

typedef enum {
    AMP_SND_STREAM_OUT_PCM          = 0,
    AMP_SND_STREAM_OUT_SPDIF        = 1,
    AMP_SND_STREAM_OUT_HDMI         = 2,
    AMP_SND_STREAM_OUT_FORMAT_MAX   = 3,
} AMP_SND_STREAM_OUT_FORMAT;

/** \def AMP_SND_STRM_ATTR
    \brief This structure define stream creation attribute
 */
typedef struct {
    UINT32  uiBlkNum;
    UINT32  uiBlkLen;
    UINT32  uiChannels;
    UINT32  uiBitDepth;
    UINT32  uiPcmType;
    UINT32  uiSampleRate;
    UINT32  uiBufNr;
    UINT32  uiVolume;
    UINT32  uiPathMask;
    UINT32  uiChanNr;
    UINT32  uiChanMask[SND_CHANNEL_MAX];
    INT32   iRatio;            /**< For compress data sample count usage */
    UINT32  Format;
    UINT32  uMuteBeforeLink; /* 0: normal, stream will be mixed,
                               1: will not be mixed until be linked with patchSink,*/
} AMP_SND_STRM_ATTR;

/** \def AMP_SND_HDMI_CFG
    \brief HDMI Audio configuration enums and structres
 */
typedef struct
{
    UINT32     uiNumChan;
    UINT32     uiPortNum;
    UINT32     uiSampFreq;
    UINT32     uiSampSize;
    UINT32     uiClkFactor;
    UINT32     uiAudioFmt;
    UINT32     uiHbrAudio;
} AMP_SND_HDMI_CFG;


/** \def AMP_SND_HDMI_CFG
    \brief HDMI Audio configuration enums and structres
 */
typedef enum {
    SPDIF_CGMSA_PERMITTED,
    SPDIF_CGMSA_NOTUSED,
    SPDIF_CGMSA_ONCE,
    SPDIF_CGMSA_NOPERMITTED,
} SPDIF_CGMSA_ENUM;


/** \def AMP_SND_VOLUME_MAX
    \brief Sound volume maximum value 100
 */
#define AMP_SND_VOLUME_MAX          (100)

/** \def AMP_SND_VOLUME_MIN
    \brief Sound volume minimum value 0
 */
#define AMP_SND_VOLUME_MIN          (0)
#ifdef CONFIG_AMP_AUDIO_GAIN_RANGE_EXTENSION
#define AMP_SND_VOLE_MAX            (0x20000000)
#else
#define AMP_SND_VOLE_MAX            (0x40000000)
#endif
#define AMP_SND_VOLE_MIN            (0x0)
#define AMP_SND_VOL_L2E(v)          \
    ((UINT)((((float)MIN(v, AMP_SND_VOLUME_MAX))/100)*AMP_SND_VOLE_MAX))
#define AMP_SND_VOL_E2L(v)         ((UINT)(((float)(v)*100)/AMP_SND_VOLE_MAX))

#ifdef CONFIG_AMP_AUDIO_GAIN_RANGE_EXTENSION
#define AMP_SND_UNITY               (0x20000000)
#else
#define AMP_SND_UNITY               (0x40000000)
#endif

/*MI stands for Media Intelligence*/
#define AMP_SND_MS12_DAP_MI_DISABLE (0)
#define AMP_SND_MS12_DAP_MI_ENABLE  (1)

/*DE stands for Dialogue Enhancer*/
#define AMP_SND_MS12_DAP_DE_AMOUNT_MIN (0)
#define AMP_SND_MS12_DAP_DE_AMOUNT_MAX (16)

/*VL stands for Volume Leveler*/
#define AMP_SND_MS12_DAP_VL_AMOUNT_MIN (0)
#define AMP_SND_MS12_DAP_VL_AMOUNT_MAX (10)

/*Single OA stands for Single Object Audio*/
#define AMP_SND_MS12_Single_OA_DISABLE (0)
#define AMP_SND_MS12_Single_OA_ENABLE  (1)

/** \enum AMP_SND_PORT_ROLE
    \brief This structure define audio port role? none, source or sink
*/
typedef enum {
    AMP_SND_PORT_ROLE_NONE,
    AMP_SND_PORT_ROLE_SOURCE,
    AMP_SND_PORT_ROLE_SINK
} AMP_SND_PORT_ROLE;

/** \enum AMP_SND_PORT_TYPE
    \brief This structure define audio port type?
*                                      none, device(hardware),
*                                      mix(software) or session
*/
typedef enum {
    AMP_SND_PORT_TYPE_NONE,
    AMP_SND_PORT_TYPE_DEVICE,
    AMP_SND_PORT_TYPE_MIX,
    AMP_SND_PORT_TYPE_SESSION
} AMP_SND_PORT_TYPE;

/** \enum AMP_SND_CONFIG_MASK
    \brief  audio patch port config mask
*/
typedef enum {
    AMP_SND_PORT_CONFIG_SAMPLE_RATE  =  0x1,
    AMP_SND_PORT_CONFIG_CHANNEL_MASK   =  0x2,
    AMP_SND_PORT_CONFIG_FORMAT  =   0x4,
    AMP_SND_PORT_CONFIG_GAIN    =   0x8,
    AMP_SND_PORT_CONFIG_ALL  =  (AMP_SND_PORT_CONFIG_SAMPLE_RATE | \
                               AMP_SND_PORT_CONFIG_CHANNEL_MASK | \
                               AMP_SND_PORT_CONFIG_FORMAT | \
                               AMP_SND_PORT_CONFIG_GAIN)
} AMP_SND_CONFIG_MASK;

/** \enum AMP_SND_PATCH_STATE
    \brief  audio patch state
*      AMP_SND_PATCH_CREATE: the audio patch is creating.
*      AMP_SND_PATCH_DISCONNECT: the audio patch has been created,
*                     but not connected yet.
*      AMP_SND_PATCH_CONNECT: the audio patch has been connected.
*/
typedef enum {
    AMP_SND_PATCH_CREATE,
    AMP_SND_PATCH_DISCONNECT,
    AMP_SND_PATCH_CONNECT
} AMP_SND_PATCH_STATE;

/** \enum AMP_SND_DEVICE_IN
    \brief  audio source type
*/
typedef enum {
    AMP_SND_DEVICE_NONE,
    AMP_SND_DEVICE_IN_TUNER,
    AMP_SND_DEVICE_IN_DIGITAL,
    AMP_SND_DEVICE_IN_SPDIF,
    AMP_SND_DEVICE_IN_BT,
    AMP_SND_DEVICE_IN_CALL,
    AMP_SND_DEVICE_IN_USB,
    AMP_SND_DEVICE_IN_MIC,
    AMP_SND_DEVICE_IN_HEADSET,
    AMP_SND_DEVICE_IN_LOOPBACK,
    AMP_SND_DEVICE_IN_4K,               //vitual 4k device
    AMP_SND_DEVICE_IN_OTHER,
    AMP_SND_DEVICE_IN_HDMI = AMP_SND_DEVICE_IN_DIGITAL
} AMP_SND_DEVICE_IN;

/** \enum AMP_SND_STREAM_TYPE
    \brief  audio patch mix stream type
*/
typedef enum {
    AMP_SND_STREAM_DEFAULT          = -1,
    AMP_SND_STREAM_MIN              = 0,
    AMP_SND_STREAM_VOICE_CALL       = 0,
    AMP_SND_STREAM_SYSTEM           = 1,
    AMP_SND_STREAM_RING             = 2,
    AMP_SND_STREAM_MUSIC            = 3,
    AMP_SND_STREAM_ALARM            = 4,
    AMP_SND_STREAM_NOTIFICATION     = 5,
    AMP_SND_STREAM_BLUETOOTH_SCO    = 6,
    AMP_SND_STREAM_ENFORCED_AUDIBLE = 7,
    /* Sounds that cannot be muted by user and must be routed to speaker */
    AMP_SND_STREAM_DTMF             = 8,
    AMP_SND_STREAM_TTS              = 9,
    AMP_SND_STREAM_CNT,
    AMP_SND_STREAM_MAX              = AMP_SND_STREAM_CNT - 1
} AMP_SND_STREAM_TYPE;

/** \enum AMP_SND_SOURCE_TYPE
    \brief  audio patch mix source type
*/
typedef enum {
    AMP_SND_SOURCE_DEFAULT             = 0,
    AMP_SND_SOURCE_MIC                 = 1,
    AMP_SND_SOURCE_VOICE_UPLINK        = 2,
    AMP_SND_SOURCE_VOICE_DOWNLINK      = 3,
    AMP_SND_SOURCE_VOICE_CALL          = 4,
    AMP_SND_SOURCE_CAMCORDER           = 5,
    AMP_SND_SOURCE_VOICE_RECOGNITION   = 6,
    AMP_SND_SOURCE_VOICE_COMMUNICATION = 7,
    AMP_SND_SOURCE_REMOTE_SUBMIX       = 8,
                                          /* Source for the mix to be presented remotely. */
                                          /* An example of remote presentation is Wifi Display */
                                          /*  where a dongle attached to a TV can be used to   */
                                          /*  play the mix captured by this audio source.      */
    AMP_SND_SOURCE_CNT,
    AMP_SND_SOURCE_MAX                 = AMP_SND_SOURCE_CNT - 1,
    AMP_SND_SOURCE_HOTWORD             = 1999
                                                /* A low-priority, preemptible audio source for
                                                for background software hotword detection.
                                                Same tuning as AUDIO_SOURCE_VOICE_RECOGNITION.
                                                Used only internally to the framework. Not exposed
                                                at the audio HAL. */
} AMP_SND_SOURCE_TYPE;

/** \enum AMP_SND_SESSION_TYPE
    \brief  audio patch session type
*/
typedef enum {
    /* session for effects attached to a particular output stream
     * (value must be less than 0)
     */
    AMP_SND_SESSION_OUTPUT_STAGE = -1,

    /* session for effects applied to output mix. These effects can
     * be moved by audio policy manager to another output stream
     * (value must be 0)
     */
    AMP_SND_SESSION_OUTPUT_MIX = 0,

    /* application does not specify an explicit session ID to be used,
     * and requests a new session ID to be allocated
     * TODO use unique values for AUDIO_SESSION_OUTPUT_MIX and AUDIO_SESSION_ALLOCATE,
     * after all uses have been updated from 0 to the appropriate symbol, and have been tested.
     */
    AMP_SND_SESSION_ALLOCATE = 0,
} AMP_SND_SESSION_TYPE;

/** \def AMP_SND_MIX_CFG
    \brief This structure define audio patch mix config
*/
typedef struct {
    UINT32 uiDeviceType; /*Out Device: AMP_SND_PATH
                           In Device: AMP_SND_DEVICE_IN*/
    UINT8   iHwAddress;
} AMP_SND_DEVICE_CFG;

/** \def AMP_SND_MIX_CFG
    \brief This structure define audio patch mix config
*/
typedef struct {
    union {
        AMP_SND_STREAM_TYPE uiStreamType;
        AMP_SND_SOURCE_TYPE  uiSourceType;
    } usecase;
    UINT32 iSwModule;
} AMP_SND_MIX_CFG;

/** \def AMP_SND_SESSION_CFG
    \brief This structure define audio patch session config
*/
typedef struct {
    AMP_SND_SESSION_TYPE  uiSessionType;
} AMP_SND_SESSION_CFG;

/** \def AMP_SND_PATCH_HANDLE
    \brief This structure define audio patch handle to return
*/
typedef struct {
    void *uiTag;     //unique ID
    AMP_SND_PATCH_STATE uiState;
} AMP_SND_PATCH_HANDLE;

/** \def AMP_SND_PORT_CFG
    \brief This structure define audio patch port config
*/
typedef struct {
    UINT32 uiPortID;
    AMP_SND_PORT_ROLE uiRole;
    AMP_SND_PORT_TYPE uiType;
    AMP_SND_CONFIG_MASK uiConfigMask;
    UINT32 uiSampleRate;
    UINT32 uiChannelMask[SND_CHANNEL_MAX];
    UINT32 uiFormat;  /*Audio format
                        SPDIF: AMP_SND_SPDIF_FORMAT
                        HDMI: AMP_SND_HDMI_FORMAT*/
    UINT32 uiPCMBitDepth; /* Bit depth for pcm data */
    UINT32 uiPCMType;       /* Pcm type for pcm data */
    AMP_APP_PARAMIXGAIN uiGain;
    union {
        AMP_SND_DEVICE_CFG    uiDevice;     /* device specific info */
        AMP_SND_MIX_CFG          uiMix;          /* mix specific info */
        AMP_SND_SESSION_CFG  uiSession;   /* session specific info */
    } ext;
    BOOL bStreamHandleAlloc;            /* add the parameter temporarily and remove it later */
    UINT32 uiChanNr;      /* channel number for the input data */
} AMP_SND_PORT_CFG;

/** \def AMP_SND_PCM_CHUNK
    \brief This structure define PCM data chunk
*/
typedef struct {
    UINT32 m_SampleNum;
    UINT32 m_ChanNum;
    UINT32 m_BitDepth;
    UINT32 m_SampleRate;
    VOID  *m_SampleData;
} AMP_SND_PCM_CHUNK;

#define SND_BIQUAD_FILTER_UNIT_NUM (8)
#define SND_BIQUAD_FILTER_CHANNEL_NUM (2)
#define SND_BIQUAD_FILTER_TEMP_BUFFER_SIZE (256)

typedef enum {
    SND_BIQUAD_FILTER_FREQ_32K,
    SND_BIQUAD_FILTER_FREQ_44K,
    SND_BIQUAD_FILTER_FREQ_48K,
    SND_BIQUAD_FILTER_FREQ_NUM,
} SND_BIQUAD_FILTER_FREQ;


//TODO: To be removed, use BIQUAD_FILTER in amp_sound_types.h
typedef struct {
    INT32  iBiqFilterEnable;
    UINT32 uiGain0;
    UINT32 uiGain1;
    UINT32 uiGain2;
    UINT32 uiCoefA0;
    UINT32 uiCoefA1;
    UINT32 uiCoefA2;
    UINT32 uiCoefB1;
    UINT32 uiCoefB2;
} SND_BIQUAD_FILTER;

typedef UINT32 SND_BIQUAD_FILTER_UPDATE[SND_BIQUAD_FILTER_FREQ_NUM]
                                              [SND_BIQUAD_FILTER_CHANNEL_NUM]
                                              [SND_BIQUAD_FILTER_UNIT_NUM];

typedef UINT32 (*SND_BIQUAD_FILTER_UPDATE_PTR)[SND_BIQUAD_FILTER_CHANNEL_NUM]
                                                      [SND_BIQUAD_FILTER_UNIT_NUM];

typedef struct {
    SND_BIQUAD_FILTER_UPDATE bBiqFilterUpdate;
    SND_BIQUAD_FILTER_UPDATE bBiqFilterGainUpdate;
    SND_BIQUAD_FILTER_UPDATE bBiqFilterCoefUpdate;
    SND_BIQUAD_FILTER stBiqFilter[SND_BIQUAD_FILTER_FREQ_NUM]\
                                 [SND_BIQUAD_FILTER_CHANNEL_NUM]\
                                 [SND_BIQUAD_FILTER_UNIT_NUM];
} SND_BIQUAD_FILTER_CFG;


typedef enum {
    AMP_AUDIO_SPEED_1000X = 0,
    AMP_AUDIO_SPEED_800X,
    AMP_AUDIO_SPEED_1200X,
    AMP_AUDIO_SPEED_1500X,
    AMP_AUDIO_SPEED_2000X,
}AMP_AUDIO_SPEED;

typedef struct {
    BOOL bMATSuppot;
    BOOL bDDPSupport;
    BOOL bDDSupport;
    BOOL bDTSSupport;
} AMP_SND_ENCODER_CAP;

typedef struct {
    BOOL bMATSuppot;
    BOOL bDDPSupport;
    BOOL bDDSupport;
    BOOL bDTSSupport;
    BOOL bMPEGHSupport;
    BOOL bIEC61937Support;
} AMP_SND_PASSTRU_CAP;

struct a2dp_config {
  UINT32 sample_rate;
  UINT32 channel_mask;
  BOOL bIsStereoToMono;  // True if fetching Stereo and mixing into Mono
  INT format;
};

typedef enum {
    AUDIO_DEVICE_TYPE_BLUETOOTH_A2DP,
    AUDIO_DEVICE_TYPE_BLUETOOTH_A2DP_HEADPHONES,
    AUDIO_DEVICE_TYPE_BLUETOOTH_A2DP_SPEAKER,
    AUDIO_DEVICE_TYPE_HEARING_AID
} audio_device;

typedef enum {
    eAUD_Freq_8K = 8000,
    eAUD_Freq_11K = 11025,
    eAUD_Freq_12K = 12000,
    eAUD_Freq_16K = 16000,
    eAUD_Freq_22K = 22050,
    eAUD_Freq_24K = 24000,
    eAUD_Freq_32K = 32000,
    eAUD_Freq_44K = 44100,
    eAUD_Freq_48K = 48000,
    eAUD_Freq_64K = 64000,
    eAUD_Freq_88K = 88200,
    eAUD_Freq_96K = 96000,
    eAUD_Freq_176K = 176400,
    eAUD_Freq_192K = 192000,
    eAUD_Freq_384K = 384000,
    eAUD_Freq_768K = 768000,
} eAudioFreqCfg;

#endif /* _SOUND_TYPES_H_ */

