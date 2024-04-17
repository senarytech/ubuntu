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
//! \file amp_sound_api.h
//!
//! \brief sound service related functions.
//!
//!
//! Purpose:
//!
//!
//! Version     Date            Author
//!   V 1.00    Jan 8 2012      Ye Liu
//!   V 1.01    Feb 5 2012      Ye Liu
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef _AMP_SOUND_API_H_
#define _AMP_SOUND_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "amp_types.h"
#include "amp_client_support.h"
#include "amp_sound_types.h"
#include "isl/amp_logger.h"
#include "isl/amp_buf_desc.h"
#include "shm_api.h"
/** \typedef SND_FUNC_CFG
    \brief This structure define function configuration for sound service
 */
typedef struct sound_user_config {
    UINT32 uiConfig_a;
    UINT32 uiConfig_b;
} SND_USER_CFG;

typedef enum {
    SND_CONN_TUNNEL,
    SND_CONN_NONTUNNEL,
} SND_CONN_TYPE;

typedef struct {
    AMP_BDCHAIN *pBDWorkQ;
    AMP_BDCHAIN *pBDReleaseQ;
    AMP_BD_HANDLE hEndBD;
#ifdef AMP_SND_LATENCY_PROF
    AMP_TOP stSndsts;
#endif
    MV_OSAL_HANDLE_SEM_t hBDReleaseQSem;
} SND_STRM_BUF;

typedef enum {
    SND_STRM_IDLE   = 0,
    SND_STRM_EXEC   = 1,
    SND_STRM_PAUSE  = 2,
    SND_STRM_STOP   = 3,
} SND_STRM_STATE;

typedef struct {
    BOOL bAttrUpdate;
    UINT32 uiStreamVolume;
    UINT32 uiPrevPosMs;   //prev stream position in the uint of millisecond
    BOOL bStreamMute;
    AMP_SND_STRM_ATTR stStrmAttr;
    SND_CONN_TYPE ConnectType;

    AMP_SNDSINK hSndSinkComp;

    // context for tunnel
    UINT32 uiPortIdx;
    AMP_PORT_CONN PeerPortConn;

    // context for nontunnel
    SND_STRM_BUF stStrmBuf;
    HANDLE hInterHandle;
    HANDLE hListener;
    AMP_CONNECT_CALLBACK pCallback;
    void *pUserData;
    SND_STRM_STATE eState;

#ifdef AMP_SND_LATENCY_PROF
    AMP_TOP stStreamStatistics;
#endif
    FILE *pfStream;

    AMP_TIMESPEC stPrevAudioPos;

    BOOL bInitStartTime;
    MV_TimeSpec_t stAudioPosStart;
    MV_TimeSpec_t stSysTimeStart;

} SND_STRM;

/** \func
    \brief This API initialize sound service context
    \param no param
    \return SUCCESS for success\n
            For other code, refer to amp_types.h
 */
HRESULT AMP_SND_Init(void);

/** \func
    \brief This API deinitialize sound service context
    \return SUCCESS for success\n
            For other code, refer to amp_types.h
 */
HRESULT AMP_SND_Deinit(void);

/** \func
    \brief This API get default stream attribute
    \param pAttr stream attribute for parameter setting
    \return SUCCESS for success\n
 */
HRESULT AMP_SND_GetDefaultStreamAttribute(AMP_SND_STRM_ATTR *pAttr);

/** \func
    \brief This API create context for one input stream
    \param pAttr stream attribute for parameter setting, if NULL, sound uses the
           default setting (2-ch, 44KHz, interleaved)
    \param phStream pointer to returned stream handle
    \return SUCCESS for success\n
            For other code, refer to amp_types.h
 */
HRESULT AMP_SND_CreateStream(AMP_IN AMP_SND_STRM_ATTR *pAttr,
                             AMP_OUT HANDLE *phStream);

/** \func
    \brief This API destroy context that created by AMP_SND_CreateStream()
    \param hStream pointer to stream handle to be destroyed
    \return SUCCESS for success\n
         For other code, refer to amp_types.h
 */
HRESULT AMP_SND_DestroyStream(AMP_IN HANDLE hStream);

/** \func AMP_SND_CreateStream_RingTone(AMP_IN AMP_SND_STRM_ATTR *pAttr,
*                                      AMP_OUT HANDLE *phStream)
*    \brief The API create context for key feedback ring tone input stream
*    \param pAttr stream attribute for parameter setting
*    \param phStream pointer to returned stream handle
*    \return SUCCESS for success
*/
HRESULT AMP_SND_CreateStream_RingTone(AMP_IN AMP_SND_STRM_ATTR *pAttr,
                                      AMP_OUT HANDLE *phStream);

/** \func AMP_SND_DestroyStream_RingTone(AMP_IN HANDLE hStream)
*     \brief The API destroy context that crated by AMP_SND_CreateStream_RingTone()
*     \param hStream pointer to stream handle to be destroyed
*     \return SUCCESS for success
*/
HRESULT AMP_SND_DestroyStream_RingTone(AMP_IN HANDLE hStream);

/** \func
    \brief alias of AMP_SND_Init, deprecated to use later
    \param factory, amp factory
    \param phSnd pointer to sound handle to be destroyed
    \return SUCCESS for success\n
         For other code, refer to amp_types.h
 */
HRESULT AMP_SND_Initialize(AMP_IN AMP_FACTORY factory, AMP_OUT HANDLE * phSnd);

/** \func AMP_SND_Deinitialize()
    \brief alias of AMP_SND_Deinit, deprecated to use later
    \param hSnd pointer to sound handle to be destroyed
    \return SUCCESS for success\n
         For other code, refer to amp_types.h
 */
HRESULT AMP_SND_Deinitialize(AMP_IN HANDLE hSnd);

/** \func
    \brief Feed pcm data to play sound.
           The function will be blocked if data amount within audio pipeline
           is greater then the threshold (currently it is 60ms) until data is
           consumed to less than the threshold. Because user of this API may
           continue push zero data util data can't be pushed. If the constrain
           doesn't exist, the lantency will be increased due to the extra zero
           data.One example is fast mixer of android framwork.
    \param hStream pointer to stream handle to play data
    \param pPcmData pointer to buffer
    \param uiDataSize buffer size
 */
HRESULT AMP_SND_PushPCM(AMP_IN HANDLE hStream,
                        AMP_IN VOID *pPcmData,
                        AMP_IN UINT32 uiDataSize);
/** \func
    \brief Feed pcm data to play sound
           This function doesn't have the constrain as AMP_SND_PushPCM, it will
           return immediately as long as audio pipeline has space.
    \param hStream pointer to stream handle to play data
    \param pPcmData pointer to buffer
    \param uiDataSize buffer size
 */
HRESULT AMP_SND_PushPCM_NonBlock(HANDLE hStream,
                                 VOID * pPcmData,
                                 UINT32 uiDataSize);
/** \func
    \brief Feed compress data to play sound.
           The function will be blocked if data amount within audio pipeline
           is greater then the threshold (currently it is 60ms) until data is
           consumed to less than the threshold. Because user of this API may
           continue push zero data util data can't be pushed. If the constrain
           doesn't exist, the lantency will be increased due to the extra zero
           data.One example is fast mixer of android framwork.
    \param pSource  Sources Port config
    \param pData Pointer to compress data buffer
    \param uiDataSize Buffer size
 */
HRESULT AMP_SND_PushCompressData(AMP_IN AMP_SND_PORT_CFG *pSource,
                                 AMP_IN VOID *pData,
                                 AMP_IN UINT32 uiDataSize);

/** \func
    \brief setup tunnel pipeline with hUpCompObj component
    \param hUpCompObj up component handle
    \param uiUpPortIdx port index of up component
    \param phTunnel returned tunnel handle
 */
HRESULT AMP_SND_SetupTunnel(AMP_IN AMP_COMPONENT hUpCompObj,
                            AMP_IN UINT32 uiUpPortIdx,
                            AMP_OUT HANDLE *phTunnel);

/** \func
    \brief remove tunnel pipeline
    \param hTunnel tunnel handle to be removed
 */
HRESULT AMP_SND_RemoveTunnel(AMP_IN HANDLE hTunnel);

/** \func
    \brief Register callback function to a stream, it will be called when
    \     stream receive release_buffer event from down component.
    \param hStream pointer to stream handle
    \param pCallback user callback function pointer
    \param pUserData user callback context
 */
HRESULT AMP_SND_RegisterBufferReleaseCallback(HANDLE hStream,
                                      AMP_CONNECT_CALLBACK pCallback,
                                      void *pUserData);
/** \func
    \brief Unregister callback function to a stream
    \param hStream pointer to stream handle
 */
HRESULT AMP_SND_UnregisterCallback(HANDLE hStream);

/** \func
    \brief ONLY used by AMP Test Codes, setup tunnel pipeline with hUpCompObj component
    \param hSnd sound service handle
    \param hUpCompObj up component handle
    \param uiUpPortIdx port index of up component
    \param phTunnel returned tunnel handle
 */
HRESULT AMP_SND_SetupInputTunnel(AMP_IN HANDLE hSnd,
                                 AMP_IN AMP_COMPONENT hUpCompObj,
                                 AMP_IN UINT32 uiUpPortIdx,
                                 AMP_OUT HANDLE *phTunnel);

/** \func
    \brief ONLY used by AMP Test Codes, remove tunnel pipeline
    \param hTunnel tunnel handle to be removed
 */
HRESULT AMP_SND_RemoveInputTunnel(AMP_IN HANDLE hSnd, AMP_IN HANDLE hTunnel);

/** \func HRESULT AMP_SND_SetupTunnel_RingTone(AMP_COMPONENT hUpCompObj,
*                   UINT32 uiUpPortIdx,
*                   HANDLE * phTunnel)
*   \brief setup tunnel pipeline with ring tone and key effect input
*   \param hUpCompObj up componetn handle
*   \param uiUpPortIdx port index of up component
*   \param phTunnel returned tunnel handle
*/
HRESULT AMP_SND_SetupTunnel_RingTone(AMP_COMPONENT hUpCompObj,
                                                    UINT32 uiUpPortIdx,
                                                    HANDLE * phTunnel);

/** \func HRESULT AMP_SND_RemoveTunnel_RingTone(UINT32 hTunnel)
*   \brief remove tunnel pipeline with ring tone and key effect input
*   \param phTunnel tunnel handle to be removed
*/
HRESULT AMP_SND_RemoveTunnel_RingTone(HANDLE  hTunnel);

/** \func
    \brief This API stop the tunnel pipe line within sound service.
    \param hTunnel Tunnel handle returned from AMP_SND_SetupTunnel.
 */
HRESULT AMP_SND_StopTunnel(AMP_IN HANDLE hTunnel);

/** \func
    \brief This API pause the tunnel pipe line within sound service.
    \param hTunnel Tunnel handle returned from AMP_SND_SetupTunnel.
 */
HRESULT AMP_SND_PauseTunnel(AMP_IN HANDLE hTunnel);

/** \func
    \brief This API start the tunnel pipeline from pause state.
    \param hTunnel Tunnel handle returned from AMP_SND_SetupTunnel.
 */
HRESULT AMP_SND_StartTunnel(AMP_IN HANDLE hTunnel);

/** \func
    \brief This API stop the sound stream within sound service.
    \param hStream Stream handle returned from AMP_SND_CreateStream.
 */
HRESULT AMP_SND_StopStream(HANDLE hStream);

/** \func
    \brief This API pause the sound stream within sound service.
    \param hStream Stream handle returned from AMP_SND_CreateStream.
 */
HRESULT AMP_SND_PauseStream(HANDLE hStream);

/** \func
    \brief This API start the sound stream from pause state.
    \param hStream Stream handle returned from AMP_SND_CreateStream.
 */
HRESULT AMP_SND_StartStream(HANDLE hStream);

/** \func
    \brief This API associate the main programme sound with audio description.
    \param hSnd pointer to sound handle
    \param hMainTunnel Main audio stream
    \param hADTunnel AD audio stream
 */
HRESULT AMP_SND_AssociateADPort(AMP_IN HANDLE hSnd,
                                AMP_IN HANDLE hMainTunnel,
                                AMP_IN HANDLE hADTunnel);

/** \func
    \brief This API dis-associate the main sound with audio description.
    \param hSnd pointer to sound handle
    \param hMainTunnel Main audio stream
    \param hADTunnel AD audio stream
 */
HRESULT AMP_SND_DisAssociateADPort(AMP_IN HANDLE hSnd,
                                   AMP_IN HANDLE hMainTunnel,
                                   AMP_IN HANDLE hADTunnel);

/** \func
    \brief This API register BT service
    \param no param
    \return SUCCESS for success\n
 */
HRESULT AMP_SND_Register_BT_Service(void);

/** \func
    \brief This API switch BT path to on/off/subwoofer/audio system.
         To turn on BT path, call AMP_SND_BT_Switch(AMP_BTON), default on SUBWOOFER path
         To turn off BT path, call AMP_SND_BT_Switch(AMP_BTOFF)
         To switch to audio system path, call AMP_SND_BT_Switch(AMP_BTAUDIO)
         To switch to subwoofer path, call AMP_SND_BT_Switch(AMP_BTSUBWOOFER)

    \param uiSwitch switch option of BT path, see AMP_BTSWITCH
         AMP_BTOFF: turn off BT path
         AMP_BTON: turn on BT path, if no path is specified before, apply SUBWOOFER path
         AMP_BTAUDIO: if BT is turn on, switch to audio system, else save as config
         AMP_BTSUBWOOFER: if BT is turn on, switch to subwoofer, else save as config
 */
HRESULT AMP_SND_BT_Switch(AMP_IN UINT32 uiSwitch);

/** \func
    \brief Set sample rate
    \param hStream Stream/tunnel handle
    \param uiSampleRate SampleRate value to be set, [min..max][8000..192000]
 */
HRESULT AMP_SND_SetSampleRate(HANDLE hStream, UINT32 uiSampleRate);

/** \func
    \brief Set bit depth
    \param hStream Stream/tunnel handle
    \param uiBitDepth BitDepth value to be set, [min..max][16 or 32]
 */
HRESULT AMP_SND_SetBitDepth(HANDLE hStream, UINT32 uiBitDepth);

/** \func
 *  \brief Set pcm type
 *  \param hStream Stream/tunnel handle
 *  \param uiPcmType Pcm type to be set, define in amp_component.idl AUDIO_BITDEPTH_TYPE
 */
HRESULT AMP_SND_SetPcmType(HANDLE hStream, UINT32 uiPcmType);

/** \func
 *  \brief Set channel number and mask
 *  \param hStream Stream/tunnel handle
 *  \param uiChanNr Channels number
 *  \param pChanMask Pointer to channel mask array, UINT32 uDataMask[AMP_AUD_MAX_BUF_NR]
 */
HRESULT AMP_SND_SetChannelMask(HANDLE hStream, UINT32 uiChanNr, UINT32 *pChanMask);

/** \func
    \brief Set master volume
    \param uiVolume Volume value to be set, [min..max][0..100]
 */
HRESULT AMP_SND_SetMasterVolume(UINT uiVolume);

/** \func
    \brief Set SRC main stream. SRC will use this stream's sample rate as final output.
    \param hStream Stream/tunnel handle
 */
HRESULT AMP_SND_SetSrcMainStream(HANDLE hStream);

/** \func
    \brief Set audio patch volume
    \param pSource  Sources Port config
    \param uiVolume Volume value to be set, [min..max][0..100]
 */
HRESULT AMP_SND_SetPatchVolume(AMP_SND_PORT_CFG *pSource, UINT uiVolume);

/** \func
    \brief Get audio patch volume
    \param pSource  Sources Port config
    \param puiVolume Volume value to be get, [min..max][0..100]
 */
HRESULT AMP_SND_GetPatchVolume(AMP_SND_PORT_CFG *pSource, UINT *puiVolume);

/** \func
    \brief Set stream/tunnel volume
    \param hStream Stream/tunnel handle
    \param uiVolume Volume value to be set, [min..max][0..100]
 */
HRESULT AMP_SND_SetStreamVolume(HANDLE hStream, UINT uiVolume);

/** \func
    \brief Get stream/tunnel volume
    \param hStream Stream/tunnel handle
    \param puiVolume Volume value to be get, [min..max][0..100]
 */
HRESULT AMP_SND_GetStreamVolume(HANDLE hStream, UINT *puiVolume);

/** \func
    \brief Set stream/tunnel volume in dB.
    \param hStream Stream/tunnel handle
    \param fVolume value to be set
 */
HRESULT AMP_SND_SetStreamVolumeIndB(AMP_IN HANDLE hStream, AMP_IN float fVolume);

/** \func
    \brief Get the sample position of current audio pipeline output of this stream
                since last AMP_SND_StartStream.
           The audio pipeline output sample position is calculated as all samples
                pushed to SoundService minus samples buffering inside SoundService.
           If this stream was PAUSE/STOP, the sample position will be reset to 0.
           Note that the position is defined relative to the stream playing at
                1x forward rate; positions do not scale with changes in playback rate.
    \param hStream Stream/tunnel handle
    \param pMsec Pointer to a location to receive the position of the playback
                head relative to the beginning of this stream, expressed in milliseconds.
           This must be non-NULL.
*/
HRESULT AMP_SND_GetSamplePosition(HANDLE hStream, UINT *pMsec);

/**\func AMP_SND_GetSamplePosition_Mix(AMP_SND_PORT_CFG * stSource,UINT * pMsec)
*    \brief The API is used to get sample postion for audio patch,
*                and the type of source port must be mix.
*    \param stSource The pointer of source port configure.
*    \param pMsec Pointer to a location to receive the position of the playback
*               head relative to the beginning of this stream, expressed in milliseconds.
*    \return SUCCESS if success
*/
HRESULT AMP_SND_GetSamplePosition_Mix(AMP_SND_PORT_CFG * stSource,UINT * pMsec);

/**\func AMP_SND_GetSamplePosition_Device(AMP_SND_DEVICE_CFG * stDeviceCfg, UINT * pMsec,
*                                                                    UINT32 uiUpPortIdx);
*    \brief The API is used to get sample postion for audio patch,
*                and the type of source port must be device.
*    \param stDeviceCfg The pointer of device configure.
*    \param pMsec Pointer to a location to receive the position of the playback
*               head relative to the beginning of this stream, expressed in milliseconds.
*    \param uiUpPortIdx OutPort index of AREN component:
*                            uiUpPortIdx = 0, for PCM output;
*                            uiUpPortIdx = 1, for SPDIF pass through output;
*                            uiUpPortIdx = 2, for HDMI pass through output;
*    \return SUCCESS if success
*/
HRESULT AMP_SND_GetSamplePosition_Device(AMP_SND_DEVICE_CFG * stDeviceCfg,
                                                   UINT * pMsec,
                                                   UINT32 uiUpPortIdx);
/**\func AMP_SND_GetStreamOutPTS(HANDLE hStream, UINT *puiPtsHigh, UINT *puiPtsLow)
*    \brief The API is used to get AOUT PTS for stream.
*    \param hStream Stream/tunnel handle
*    \param puiPtsHigh & puiPtsLow Pointer to location to receive PTS on AOUT side for stream.
*    \return SUCCESS if success
*/
HRESULT AMP_SND_GetStreamOutPTS(HANDLE hStream, UINT *puiPtsHigh, UINT *puiPtsLow);

/** \func
*    \brief Get the stream position of current audio pipeline output of this stream
*                since last AMP_SND_StartStream.
*           The audio pipeline output stream position is calculated on AOUT side by interrupt.
*    \param hStream Stream/tunnel handle
*    \param pPosInTS Pointer to a location to receive the position of the playback
*                head relative to the beginning of this stream, expressed in time spec.
*           This must be non-NULL.
*/

HRESULT AMP_SND_GetStreamPosition(HANDLE hStream, AMP_TIMESPEC *pPosInTS);

/** \func
*    \brief The API is used to get stream postion in time spec for audio patch,
*                and the type of source port must be device.
*    \param stSource The pointer of device configure.
*          The audio pipeline output stream position is calculated on AOUT side by interrupt.
*    \param pPosInTS Pointer to a location to receive the position of the playback
*           head relative to the beginning of this stream, expressed in time spec.
*           This must be non-NULL.
*/

HRESULT AMP_SND_GetStreamPosition_Mix(AMP_SND_PORT_CFG *stSource,
                                      AMP_TIMESPEC *pPosInTS);

/** \func
    \brief Apply for each specific path
 */
HRESULT AMP_SND_SetPathVolume(AMP_IN AMP_SND_PATH Path,
                              AMP_IN AMP_APP_PARAVOLUME *stVolume);
HRESULT AMP_SND_GetPathVolume(AMP_IN AMP_SND_PATH Path,
                              AMP_OUT AMP_APP_PARAVOLUME *stVolume);

/** \func AMP_SND_SetStreamMute(AMP_IN HANDLE hStream, AMP_IN BOOL bMute)
*   \brief Mute or recover stream volume.
*   \param hStream The Stream handle.
*   \param bMute Mute the stream or recover
*   \return SUCCESS if success.
*/
HRESULT AMP_SND_SetStreamMute(AMP_IN HANDLE hStream, AMP_IN BOOL bMute);

/** \func AMP_SND_GetStreamMute(AMP_IN HANDLE hStream, AMP_OUT BOOL *bMute)
*   \brief Get mute status.
*   \param hStream The stream handle.
*   \param bMute The pointer used to get mute status.
*   \return SUCCESS if success.
*/
HRESULT AMP_SND_GetStreamMute(AMP_IN HANDLE hStream, AMP_OUT BOOL *bMute);

/** \func HRESULT AMP_SND_SetMute(AMP_IN AMP_SND_PATH Path, AMP_IN BOOL bMute)
*   \brief Mute or recover path volume.
*   \param Path The choosing path.
*   \bMute Mute the path or recover from mute state.
*   \return SUCCESS if success.
*/
HRESULT AMP_SND_SetMute(AMP_IN AMP_SND_PATH Path, AMP_IN BOOL bMute);

/** \func HRESULT AMP_SND_GetMute(AMP_IN AMP_SND_PATH Path, AMP_OUT BOOL *bMute)
*   \brief Get mute status.
*   \Path The choosing path.
*   \bMute The pointer used to get mute status.
*   \return SUCCESS if success.
*/
HRESULT AMP_SND_GetMute(AMP_IN AMP_SND_PATH Path, AMP_OUT BOOL *bMute);

/** \func HRESULT AMP_SND_SetStreamVolume_RingTone(HANDLE hStream, UINT uiVolume)
*     \brief Set ring tone stream volume
*     \param hStream The stream handle
*     \param uiVolume Volume value to be set
*     \return SUCCESS if success
*/
HRESULT AMP_SND_SetStreamVolume_RingTone(AMP_IN HANDLE hStream,
                                                           AMP_IN  UINT uiVolume);

/** \func HRESULT AMP_SND_GetStreamVolume_RingTone(AMP_IN HANDLE hStream,
*                                         AMP_OUT UINT *pVolume)
*     \brief Get ring tone stream volume
*     \param hStream The stream handle
*     \pVolume The pointer used to get volume
*     \return SUCCESS if success
*/
HRESULT AMP_SND_GetStreamVolume_RingTone(AMP_IN HANDLE hStream,
                                                           AMP_OUT UINT *pVolume);

/** \func HRESULT AMP_SND_SetOutPutVolumeMultiMixer(AMP_IN AMP_APP_PARAVOLUME *pVolume)
*     \brief Set output volume of mulitMixer, the default port index is 0
*     \param pVolume The pointer of volume used to be set
*     \return SUCCESS if success
*/
HRESULT AMP_SND_SetOutPutVolumeMultiMixer(AMP_IN AMP_APP_PARAVOLUME *pVolume);

/** \func HRESULT AMP_SND_GetOutPutVolumeMultiMixer(AMP_OUT AMP_APP_PARAVOLUME *pVolume)
*     \brief Get output volume of mulitMixer, the default portidx is 0
*     \param pVolume The pointer used to get volume
*     \return SUCCESS if success
*/
HRESULT AMP_SND_GetOutPutVolumeMultiMixer(AMP_OUT AMP_APP_PARAVOLUME *pVolume);

/** \func HRESULT AMP_SND_SetOutPutVolumeMultiMixerIndB(AMP_IN float fVolume)
*     \brief Set output volume of mulitMixer in dB, the default port index is 0
*     \param fVolume The volume value in dB to be set
*     \return SUCCESS if success
*/
HRESULT AMP_SND_SetOutPutVolumeMultiMixerIndB(AMP_IN float fVolume);

/** \func HRESULT AMP_SND_GetOutPutVolumeMultiMixerIndB(AMP_OUT float fVolume)
*     \brief Get output volume of multiMixer in dB, the default port index is 0
*     \param pVolume The pointer used to get volume in dB
*     \return  SUCCESS if success
*/
HRESULT AMP_SND_GetOutPutVolumeMultiMixerIndB(AMP_OUT float *pVolume);

HRESULT AMP_SND_SetSpkDelay(AMP_IN AMP_SND_PATH Path,
                            AMP_IN AMP_APP_PARADELAY *stDelay);
HRESULT AMP_SND_GetSpkDelay(AMP_IN AMP_SND_PATH Path,
                            AMP_IN AMP_APP_PARADELAY *stDelay);
HRESULT AMP_SND_SetStereoMode(AMP_IN AMP_SND_PATH Path,
                              AMP_IN AMP_APP_PARASTEREO *stStereo);
HRESULT AMP_SND_GetStereoMode(AMP_IN AMP_SND_PATH Path,
                              AMP_OUT AMP_APP_PARASTEREO *stStereo);
HRESULT AMP_SND_SetDualMonoMode(AMP_IN AMP_SND_PATH Path,
                                AMP_IN AMP_APP_PARADUALMODE *stDualMode);
HRESULT AMP_SND_GetDualMonoMode(AMP_IN AMP_SND_PATH Path,
                                AMP_OUT AMP_APP_PARADUALMODE *stDualMode);
/** \func
    \brief Apply only for the MUTLI path
 */
HRESULT AMP_SND_SetEQParamter(AMP_IN AMP_APP_PARAGEQ *stGEQ);
HRESULT AMP_SND_GetEQParamter(AMP_OUT AMP_APP_PARAGEQ *stGEQ);

HRESULT AMP_SND_SetBalance(AMP_IN AMP_APP_PARABALANCE *stBalance);
HRESULT AMP_SND_GetBalance(AMP_OUT AMP_APP_PARABALANCE *stBalance);

/**\func AMP_SND_SetBalance_Ext(AMP_IN AMP_SND_PATH Path, AMP_IN AMP_APP_PARABALANCE * stBalance)
*   \brief Setting the balance parameters of app in output port path
*   \Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*                    AMP_SND_PATH_SPDIF = 2,
*                    AMP_SND_PATH_HDMI = 3,
*
*   \stBalance The pointer of balance paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetBalance_Ext(AMP_IN AMP_SND_PATH Path, AMP_IN AMP_APP_PARABALANCE * stBalance);

/**\func AMP_SND_GetBalance_Ext(AMP_IN AMP_SND_PATH Path, AMP_OUT AMP_APP_PARABALANCE * stBalance)
*   \brief Getting the balance parameters of app in output port path
*   \Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*                    AMP_SND_PATH_SPDIF = 2,
*                    AMP_SND_PATH_HDMI = 3,
*
*   \stBalance The pointer of balance paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetBalance_Ext(AMP_IN AMP_SND_PATH Path, AMP_OUT AMP_APP_PARABALANCE * stBalance);

HRESULT AMP_SND_SetDelay(AMP_IN AMP_SND_PATH Path,
                         AMP_APP_PARADELAY *stDelay);
HRESULT AMP_SND_GetDelay(AMP_IN AMP_SND_PATH Path,
                         AMP_OUT AMP_APP_PARADELAY *stDelay);

HRESULT AMP_SND_SetSpkMode(AMP_IN AMP_APP_PARASPKMODE *stSpkMode);
HRESULT AMP_SND_GetSpkMode(AMP_OUT AMP_APP_PARASPKMODE *stBalance);

HRESULT AMP_SND_SetLFEDmxMode(AMP_IN AMP_APP_PARALFEDMX *stLfeDmx);
HRESULT AMP_SND_GetLFEDmxMode(AMP_OUT AMP_APP_PARALFEDMX *stLfeDmx);

HRESULT AMP_SND_SetBass(AMP_IN AMP_APP_PARATREBLEBASS *stBass);
HRESULT AMP_SND_GetBass(AMP_OUT AMP_APP_PARATREBLEBASS *stBass);

HRESULT AMP_SND_SetTreble(AMP_IN AMP_APP_PARATREBLEBASS *stTreble);
HRESULT AMP_SND_GetTreble(AMP_OUT AMP_APP_PARATREBLEBASS *stTreble);

HRESULT AMP_SND_SetHPFMode(AMP_IN AMP_APP_PARAHPF *stHpf);
HRESULT AMP_SND_GetHPFMode(AMP_OUT AMP_APP_PARAHPF *stHpf);

HRESULT AMP_SND_SetSRSMode(AMP_IN AMP_APP_PARASRSGEN *stSrsGen);
HRESULT AMP_SND_GetSRSMode(AMP_OUT AMP_APP_PARASRSGEN *stSrsGen);

HRESULT AMP_SND_SetSRSTrueVolMode(AMP_IN AMP_APP_PARASRSTRUVOL *stSrsTruVol);
HRESULT AMP_SND_GetSRSTrueVolMode(AMP_OUT AMP_APP_PARASRSTRUVOL *stSrsTruVol);

HRESULT AMP_SND_SetSRSHardLimiter(AMP_IN AMP_APP_PARASRSHDLIM *stSrsHdLim);
HRESULT AMP_SND_GetSRSHardLimiter(AMP_OUT AMP_APP_PARASRSHDLIM *stSrsHdLim);

HRESULT AMP_SND_SetSRSTrueSrdMode(AMP_IN AMP_APP_PARASRSTRUSUR *stSrsTruSur);
HRESULT AMP_SND_GetSRSTrueSrdMode(AMP_OUT AMP_APP_PARASRSTRUSUR *stSrsTruSur);

HRESULT AMP_SND_SetSRSTrueEQParam(AMP_IN AMP_APP_PARASRSTRUEQ *stSrsTruEQ);
HRESULT AMP_SND_GetSRSTrueEQParam(AMP_OUT AMP_APP_PARASRSTRUEQ *stSrsTruEQ);

HRESULT AMP_SND_SetSRSTrueEQFilter(AMP_IN AMP_APP_PARASRSTRUEQFLT *stSrsTruEQFlt);
HRESULT AMP_SND_GetSRSTrueEQFilter(AMP_OUT AMP_APP_PARASRSTRUEQFLT *stSrsTruEQFlt);

HRESULT AMP_SND_SetMixerGain(HANDLE hStream, AMP_APP_PARAMIXGAIN *pMixerGain);
HRESULT AMP_SND_GetMixerGain(HANDLE hStream, AMP_APP_PARAMIXGAIN *pMixerGain);

/** \func
    \brief Apply for SPDIF path
 */
HRESULT AMP_SND_SetSpdifFormat(AMP_SND_SPDIF_FORMAT Format);
HRESULT AMP_SND_GetSpdifFormat(AMP_SND_SPDIF_FORMAT *pFormat);
HRESULT AMP_SND_SetSpdifFreq(UINT32 Frequency);
HRESULT AMP_SND_GetSpdifFreq(UINT32 *Frequency);

/** \func
    \brief Apply for HDMI path
 */
HRESULT AMP_SND_SetHDMIFormat(AMP_SND_HDMI_FORMAT Format);
HRESULT AMP_SND_GetHDMIFormat(AMP_SND_HDMI_FORMAT *pFormat);

/**\func AMP_SND_SetPathVolumeIndB(AMP_IN AMP_SND_PATH Path, AMP_IN float fVolume)
*   \brief Set the volume in dB for dedicated output path.
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*                    AMP_SND_PATH_SPDIF = 2,
*                    AMP_SND_PATH_HDMI = 3,
*
*   \param fVolume Volume in dB valume. Range (-infinite, +12dB)
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetPathVolumeIndB(AMP_IN AMP_SND_PATH Path,
                                  AMP_IN float fVolume);

/**\func AMP_SND_GetPathVolumeIndB(AMP_IN AMP_SND_PATH Path, AMP_OUT float *fVolume)
*   \brief Get the volume in dB for dedicated output path.
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*                    AMP_SND_PATH_SPDIF = 2,
*                    AMP_SND_PATH_HDMI = 3,
*
*   \param *fVolume Volume in dB valume. Range (-infinite, +12dB)
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetPathVolumeIndB(AMP_IN AMP_SND_PATH Path,
                                  AMP_OUT float *fVolume);

HRESULT AMP_SND_SetOutSampleRate(AMP_IN AMP_SND_PATH Path,
                                        AMP_IN UINT32 uiOutSampleRate);

/**\func AMP_SND_SetPathHeadRoomIndB(AMP_IN AMP_SND_PATH Path, AMP_IN float fVolume)
*   \brief Set the volume in dB for dedicated output path.
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*                    AMP_SND_PATH_SPDIF = 2,
*                    AMP_SND_PATH_HDMI = 3,
*                    AMP_SND_PATH_BT = 4,
*
*   \param fVolume Headroom in dB valume. Range (-infinite, 0dB)
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetPathHeadRoomIndB(AMP_IN AMP_SND_PATH Path,
                                    AMP_IN float fVolume);

/**\func AMP_SND_GetPathHeadRoomIndB(AMP_IN AMP_SND_PATH Path, AMP_OUT float *fVolume)
*   \brief Get the volume in dB for dedicated output path.
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*                    AMP_SND_PATH_SPDIF = 2,
*                    AMP_SND_PATH_HDMI = 3,
*                    AMP_SND_PATH_BT = 4,
*
*   \param *fVolume Headroom in dB valume. Range (-infinite, 0dB)
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetPathHeadRoomIndB(AMP_IN AMP_SND_PATH Path,
                                    AMP_OUT float *fVolume);


/**\func HRESULT AMP_SND_SetBalanceIndB(AMP_IN AMP_SND_PATH Path,
*                                       AMP_IN float iLeftVolume,
*                                       AMP_IN float iRghtVolume)
*   \brief Set the balance parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*                    AMP_SND_PATH_SPDIF = 2,
*                    AMP_SND_PATH_HDMI = 3,
*
*   \param fLeftVolume The volume value of left channel.
*   \param fRhgtVoluem The volume value of right channel.
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetBalanceIndB(AMP_IN AMP_SND_PATH Path,
                               AMP_IN float fLeftVolume,
                               AMP_IN float fRghtVolume);

/**\func HRESULT AMP_SND_GetBalanceIndB(AMP_IN AMP_SND_PATH Path,
*                                       AMP_IN float *fLeftVolume,
*                                       AMP_IN float *fRghtVolume)
*   \brief Get the balance parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*                    AMP_SND_PATH_SPDIF = 2,
*                    AMP_SND_PATH_HDMI = 3,
*
*   \param fLeftVolume The pointer of left channel volume.
*   \param fRhgtVolume The pointer of right channel volume.
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetBalanceIndB(AMP_IN AMP_SND_PATH Path,
                               AMP_OUT float *fLeftVolume,
                               AMP_OUT float *fRghtVolume);

/**\func HRESULT AMP_SND_SetBass_Ext(AMP_IN AMP_SND_PATH Path,
*                                    AMP_IN AMP_APP_PARATREBLEBASS *stBass);
*   \brief Set bass parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stBass The pointer of bass paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetBass_Ext(AMP_IN AMP_SND_PATH Path,
                            AMP_IN AMP_APP_PARATREBLEBASS *stBass);

/**\func HRESULT AMP_SND_GetBass_Ext(AMP_IN AMP_SND_PATH Path,
*                                    AMP_OUT AMP_APP_PARATREBLEBASS *stBass);
*   \brief Get bass parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stBass The pointer of bass paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetBass_Ext(AMP_IN AMP_SND_PATH Path,
                            AMP_OUT AMP_APP_PARATREBLEBASS *stBass);

/**\func HRESULT AMP_SND_SetTreble_Ext(AMP_IN AMP_SND_PATH Path,
*                                      AMP_IN AMP_APP_PARATREBLEBASS *stTreble);
*   \brief Set treble parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stTreble The pointer of treble paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetTreble_Ext(AMP_IN AMP_SND_PATH Path,
                              AMP_IN AMP_APP_PARATREBLEBASS *stTreble);

/**\func HRESULT AMP_SND_SetTreble_Ext(AMP_IN AMP_SND_PATH Path,
*                                      AMP_IN AMP_APP_PARATREBLEBASS *stTreble);
*   \brief Get treble parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stTreble The pointer of treble paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetTreble_Ext(AMP_IN AMP_SND_PATH Path,
                              AMP_OUT AMP_APP_PARATREBLEBASS *stTreble);

/**\func HRESULT AMP_SND_SetSRSMode_Ext(AMP_IN AMP_SND_PATH Path,
*                                       AMP_IN AMP_APP_PARASRSGEN *stSrsGen);
*   \brief Set SRS generic parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stSrsGen The pointer of SRS generic paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetSRSMode_Ext(AMP_IN AMP_SND_PATH Path,
                               AMP_IN AMP_APP_PARASRSGEN *stSrsGen);

/**\func HRESULT AMP_SND_GetSRSMode_Ext(AMP_IN AMP_SND_PATH Path,
*                                       AMP_OUT AMP_APP_PARASRSGEN *stSrsGen);
*   \brief Get SRS generic parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stSrsGen The pointer of generic paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetSRSMode_Ext(AMP_IN AMP_SND_PATH Path,
                               AMP_OUT AMP_APP_PARASRSGEN *stSrsGen);

/**\func
*   HRESULT AMP_SND_SetSRSTrueVolMode_Ext(AMP_IN AMP_SND_PATH Path,
*                                         AMP_IN AMP_APP_PARASRSTRUVOL *stSrsTruVol);
*   \brief Set SRS true volume parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stSrsTruVol The pointer of SRS true volume paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetSRSTrueVolMode_Ext(AMP_IN AMP_SND_PATH Path,
                                      AMP_IN AMP_APP_PARASRSTRUVOL *stSrsTruVol);
/**\func
*   HRESULT AMP_SND_GetSRSTrueVolMode_Ext(AMP_IN AMP_SND_PATH Path,
*                                         AMP_IN AMP_APP_PARASRSTRUVOL *stSrsTruVol);
*   \brief Get SRS true volume parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stSrsTruVol The pointer of SRS true volume paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetSRSTrueVolMode_Ext(AMP_IN AMP_SND_PATH Path,
                                      AMP_OUT AMP_APP_PARASRSTRUVOL *stSrsTruVol);

/**\func
*   HRESULT AMP_SND_SetSRSHardLimiter_Ext(AMP_IN AMP_SND_PATH Path,
*                                      AMP_IN AMP_APP_PARASRSHDLIM *stSrsHdLim);
*
*   \brief Set SRS hard limiter parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stSrsHdLim The pointer of SRS hard limiter paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetSRSHardLimiter_Ext(AMP_IN AMP_SND_PATH Path,
                                      AMP_IN AMP_APP_PARASRSHDLIM *stSrsHdLim);

/**\func
*   HRESULT AMP_SND_GetSRSHardLimiter_Ext(AMP_IN AMP_SND_PATH Path,
*                                      AMP_OUT AMP_APP_PARASRSHDLIM *stSrsHdLim);
*
*   \brief Get SRS hard limiter parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stSrsHdLim The pointer of SRS hard limiter paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetSRSHardLimiter_Ext(AMP_IN AMP_SND_PATH Path,
                                      AMP_OUT AMP_APP_PARASRSHDLIM *stSrsHdLim);

/**\func
*   HRESULT AMP_SND_SetSRSTrueSrdMode_Ext(AMP_IN AMP_SND_PATH Path,
*                                  AMP_IN AMP_APP_PARASRSTRUSUR *stSrsTruSur);
*
*   \brief Set SRS true sourround parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stSrsTruSur The pointer of SRS true surround paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetSRSTrueSrdMode_Ext(AMP_IN AMP_SND_PATH Path,
                                  AMP_IN AMP_APP_PARASRSTRUSUR *stSrsTruSur);

/**\func
*   HRESULT AMP_SND_GetSRSTrueSrdMode_Ext(AMP_IN AMP_SND_PATH Path,
*                                      AMP_IN AMP_APP_PARASRSTRUSUR *stSrsTruSur);
*
*   \brief Get SRS true sourround parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stSrsTruSur The pointer of SRS true surround paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetSRSTrueSrdMode_Ext(AMP_IN AMP_SND_PATH Path,
                                   AMP_OUT AMP_APP_PARASRSTRUSUR *stSrsTruSur);

/**\func
*   HRESULT AMP_SND_SetSRSTrueEQParam_Ext(AMP_IN AMP_SND_PATH Path,
*                                      AMP_IN AMP_APP_PARASRSTRUEQ *stSrsTruEQ);
*   \brief Set SRS true EQ parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stSrsTruEQ The pointer of SRS true EQ paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetSRSTrueEQParam_Ext(AMP_IN AMP_SND_PATH Path,
                                      AMP_IN AMP_APP_PARASRSTRUEQ *stSrsTruEQ);

/**\func
*   HRESULT AMP_SND_GetSRSTrueEQParam_Ext(AMP_IN AMP_SND_PATH Path,
*                                     AMP_OUT AMP_APP_PARASRSTRUEQ *stSrsTruEQ);
*   \brief Get SRS true EQ parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stSrsTruEQ The pointer of SRS True EQ parameters.
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetSRSTrueEQParam_Ext(AMP_IN AMP_SND_PATH Path,
                                      AMP_OUT AMP_APP_PARASRSTRUEQ *stSrsTruEQ);

/**\func
*   HRESULT AMP_SND_SetSRSTrueEQFilter_Ext(AMP_IN AMP_SND_PATH Path,
*                                AMP_IN AMP_APP_PARASRSTRUEQFLT *stSrsTruEQFlt);
*   \brief Set SRS true EQ filter parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stSrsTruEQFlt The pointer of SRS true EQ filter paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetSRSTrueEQFilter_Ext(AMP_IN AMP_SND_PATH Path,
                                 AMP_IN AMP_APP_PARASRSTRUEQFLT *stSrsTruEQFlt);

/**\func
*   HRESULT AMP_SND_GetSRSTrueEQFilter_Ext(AMP_IN AMP_SND_PATH Path,
*                              AMP_OUT AMP_APP_PARASRSTRUEQFLT *stSrsTruEQFlt);
*   \brief Get SRS true EQ filter parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stSrsTruEQFlt The pointer of SRS true EQ filter paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetSRSTrueEQFilter_Ext(AMP_IN AMP_SND_PATH Path,
                                AMP_OUT AMP_APP_PARASRSTRUEQFLT *stSrsTruEQFlt);

/**\func
*   HRESULT AMP_SND_SetSRSTruDial_Ext(AMP_IN AMP_SND_PATH Path,
*                               AMP_IN AMP_APP_PARASRSTRUDIAL *stSrsTruEQFlt);
*   \brief Set SRS true dialoge parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stSrsTruDial The pointer of SRS true EQ filter paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetSRSTruDial_Ext(AMP_IN AMP_SND_PATH Path,
                                  AMP_IN AMP_APP_PARASRSTRUDIAL *stSrsTruDial);

/**\func
*   HRESULT AMP_SND_GetSRSTruDial_Ext(AMP_IN AMP_SND_PATH Path,
*                               AMP_OUT AMP_APP_PARASRSTRUDIAL *stSrsTruEQFlt);
*   \brief Get SRS true dialoge parameters of app in output port path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*
*   \param stSrsTruDial The pointer of SRS true EQ filter paramters
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetSRSTruDial_Ext(AMP_IN AMP_SND_PATH Path,
                                  AMP_OUT AMP_APP_PARASRSTRUDIAL *stSrsTruDial);

/**\func
*   HRESULT AMP_SND_SetSRSCS_Ext(AMP_IN AMP_SND_PATH Path,
*                                     AMP_IN AMP_APP_PARASRSCS *stSrsCS);
*   \brief Set SRS CS Decoder to dedicated output path
*   \param Path The output port index of app, the index should be one of:
*                       AMP_SND_PATH_71 = 0,
*                        AMP_SND_PATH_20 = 1,
*   \param  stSrsCS The pointer of SRS CD decoder paramters
*   \return  SUCCESS if success
*/
HRESULT AMP_SND_SetSRSCS_Ext(AMP_IN AMP_SND_PATH Path,
                                          AMP_IN AMP_APP_PARASRSCS *stSrsCS);

/**\func
*   HRESULT AMP_SND_GetSRSCS_Ext(AMP_IN AMP_SND_PATH Path,
*                                     AMP_OUT AMP_APP_PARASRSCS *stSrsCS);
*   \brief Get SRS CS Decoder from dedicated output path
*   \param Path The output port index of app, the index should be one of:
*                       AMP_SND_PATH_71 = 0,
*                        AMP_SND_PATH_20 = 1,
*   \param  stSrsCS The pointer of SRS CD decoder paramters
*   \return  SUCCESS if success
*/
HRESULT AMP_SND_GetSRSCS_Ext(AMP_IN AMP_SND_PATH Path,
                                          AMP_OUT AMP_APP_PARASRSCS *stSrsCS);

/**\func     AMP_SND_SetSRSHPFConfig_Ext(AMP_IN AMP_SND_PATH Path,
*                                       AMP_IN AMP_APP_PARASRSHPFCONFIG *strSrsHPFCfg);
*   \brief Set SRS HPF config to dedicated output path
*   \param Path The output port of app, the index should be one of:
*                           AMP_SND_PATH_71 = 0,
*                           AMP_SND_PATH_20 = 1,
*   \param stSrsHPFCfg  The pionter of SRS HPF config
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetSRSHPFConfig_Ext(AMP_IN AMP_SND_PATH Path,
                                AMP_IN AMP_APP_PARASRSHPFCONFIG *stSrsHPFCfg);

/**\func    HRESULT AMP_SND_GetSRSHPFConfig_Ext(AMP_IN AMP_SND_PATH Path,
*                                       AMP_IN AMP_APP_PARASRSHPFCONFIG *stSrsHPFCfg);
*   \brief Get SRS HPF config from dedicated output path
*   \param  Path The output port of app, the index should be one of:
*                           AMP_SND_PATH_71 = 0,
*                           AMP_SND_PATH_20 = 1,
*   \param stSrsHPFCfg  The pointer of SRS HPF config
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetSRSHPFConfig_Ext(AMP_IN AMP_SND_PATH Path,
                                AMP_OUT AMP_APP_PARASRSHPFCONFIG *stSrsHPFCfg);

/**\ func
*   HRESULT AMP_SND_SetSRSHPFFilter_Ext(AMP_IN AMP_SND_PATH Path,
*                                                AMP_IN AMP_APP_PARASRSHPFFLT *stSrsHPF);
*   \brief Set SRS HPF filter coefficient to dedicated output path
*   \param Path The output port index of app, the index should be one of:
*                           AMP_SND_PATH_71 = 0,
*                           AMP_SND_PATH_20 = 1,
*   \param stSrsHPFFlt The pointer of SRS HPF filter coefficient
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetSRSHPFFilter_Ext(AMP_IN AMP_SND_PATH Path,
                              AMP_IN AMP_APP_PARASRSHPFFLT *stSrsHPFFlt);

/**\func   AMP_SND_GetSRSHPFFilter_Ext(AMP_IN AMP_SND_PATH Path,
*                               AMP_IN AMP_APP_PARASRSHPFFLT *stSrsHPFFlt);
*   \brief Get SRS HPF filter coefficient from dedicated output path
*   \param Path The output port index of app, the index should be one of:
*                           AMP_SND_PATH_71 = 0,
*                           AMP_SND_PATH_20 = 1,
*   \param stSrsHPFFlt The pointer of SRS HPF filter coefficient
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetSRSHPFFilter_Ext(AMP_IN AMP_SND_PATH Path,
                              AMP_OUT AMP_APP_PARASRSHPFFLT *stSrsHPFFlt);

/**\func    HRESULT AMP_SND_SetSRSGEQ_Ext(AMP_IN AMP_SND_PATH Path,
*                                            AMP_IN AMP_APP_PARASRSGEQ *stSrsGEQ);
*   \brief Set SRS GEQ to dedicated output path
*   \param Path The output port index of app, the index should be one of:
*                           AMP_SND_PATH_71 = 0,
*                           AMP_SND_PATH_20 = 1,
*   \param stSrsGEQ The pointer of SRS GEQ
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetSRSGEQ_Ext(AMP_IN AMP_SND_PATH Path,
                                AMP_IN AMP_APP_PARASRSGEQ *stSrsGEQ);

/**\func    HRESULT AMP_SND_GetSRSGEQ_Ext(AMP_IN AMP_SND_PATH Path,
                                        AMP_OUT AMP_APP_PARASRSGEQ *stSrsGEQ);
*   \brief Get SRS GEQ from dedicated output path
*   \param Path The output port index of app, the index should be one of:
*                           AMP_SND_PATH_71 = 0,
*                           AMP_SND_PATH_20 = 1,
*   \param stSrsGEQ The pointer of SRS GEQ
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetSRSGEQ_Ext(AMP_IN AMP_SND_PATH Path,
                                AMP_OUT AMP_APP_PARASRSGEQ *stSrsGEQ);

/**\func
*   HRESULT AMP_SND_SetBiquadFilter(AMP_IN AMP_SND_PATH Path,
*                               AMP_IN INT ChannelIdx,
*                               AMP_IN INT FilterIndex,
*                               AMP_IN AMP_APP_PARABIQUAFILTER *stBiqudFilter);
*   \brief Set biquad filter parameters to dedicated outptu path.
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_BT = 4,
*
*   \param ChannelIdx The channel index of which biquad is applied on. Only left
*                     and right channel are supported.
*                     The index value should be one of the following.
*                     AMP_SND_CHANNEL_FRONT_LEFT and
*                     AMP_SND_CHANNEL_FRONT_RIGHT.
*   \param FilterIndex The filter index of whole biquad filter block. There are
*                      total eight filters for each of channels. So the index
*                      ranges from 0 to 7.
*   \param stBiqudFilter The pointer of biquad filter which contains all the
*                        paramters of one biquad filter.
*
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetBiquadFilter(AMP_IN AMP_SND_PATH Path,
                                AMP_IN AMP_APP_PARABIQUAFILTER *stBiqudFilter);

/**\func
*   HRESULT AMP_SND_GetBiquadFilter(AMP_IN AMP_SND_PATH Path,
*                               AMP_IN INT ChannelIdx,
*                               AMP_IN INT FilterIndex,
*                               AMP_OUT AMP_APP_PARABIQUAFILTER *stBiqudFilter);
*   \brief Get biquad filter parameters of dedicated outptu path.
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_BT = 4,
*
*   \param ChannelIdx The channel index of which biquad is applied on. Only left
*                     and right channel are supported.
*                     The index value should be one of the following.
*                     AMP_SND_CHANNEL_FRONT_LEFT and
*                     AMP_SND_CHANNEL_FRONT_RIGHT.
*   \param FilterIndex The filter index of whole biquad filter block. There are
*                      total eight filters for each of channels. So the index
*                      ranges from 0 to 7.
*   \param stBiqudFilter The pointer of biquad filter which contains all the
*                        paramters of one biquad filter.
*
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetBiquadFilter(AMP_IN AMP_SND_PATH Path,
                                AMP_INOUT AMP_APP_PARABIQUAFILTER *stBiqudFilter);

/**\func HRESULT AMP_SND_SetBiquadFilterGain(AMP_IN AMP_SND_PATH Path,
*                          AMP_IN AMP_APP_PARABIQUAFILTERGAIN * stBiquadFilterGain);
*   \brief Set biquad filter gain to dedicated output path.
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_BT = 4,
*   \param stBiquadFilterGain The pointer of biquad filter gain
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetBiquadFilterGain(AMP_IN AMP_SND_PATH Path,
                    AMP_IN AMP_APP_PARABIQUAFILTERGAIN * stBiquadFilterGain);

/**\func HRESULT AMP_SND_GetBiquadFilterGain(AMP_IN AMP_SND_PATH Path,
*               AMP_INOUT AMP_APP_PARABIQUAFILTERGAIN * stBiquadFilterGain);
*   \brief Get biquad filter gain of dedicated output path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_BT = 4,
*   \param stBiquadfilterGain The pointer of biquad filter gain
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetBiquadFilterGain(AMP_IN AMP_SND_PATH Path,
            AMP_INOUT AMP_APP_PARABIQUAFILTERGAIN * stBiquadFilterGain);

/**\func HRESULT AMP_SND_SetBiquadFilterCoef(AMP_IN AMP_SND_PATH Path,
*                 AMP_IN AMP_APP_PARABIQUAFILTERCOEF * stBiquadFilterCoef);
*   \brief Set biquad filter coefficients to dedicated output path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_BT = 4,
*   \param stBiquadfilterCoef The pointer of biquad filter coefficients
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetBiquadFilterCoef(AMP_IN AMP_SND_PATH Path,
        AMP_IN AMP_APP_PARABIQUAFILTERCOEF * stBiquadFilterCoef);

/**\func HRESULT AMP_SND_GetBiquadFilterCoef(AMP_IN AMP_SND_PATH Path,
*                   AMP_INOUT AMP_APP_PARABIQUAFILTERCOEF * stBiquadFilterCoef);
*   \brief Get biquad filter coefficients of dedicated output path
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_BT = 4,
*   \param stBiquadfilterCoef The pointer of biquad filter coefficients
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetBiquadFilterCoef(AMP_IN AMP_SND_PATH Path,
                AMP_INOUT AMP_APP_PARABIQUAFILTERCOEF * stBiquadFilterCoef);

/**\func HRESULT AMP_SND_GetGainFractionalBit(AMP_OUT UINT32 * uiFractionalBit)
*   \brief  Get the fractional bits of fixed-point gain level used in sound service
*   \param uiFixPoint The pionter used to get number of fixed piont
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetGainFractionalBit(AMP_OUT UINT32 * uiFractionalBit);

/**\func HRESULT AMP_SND_QueryStreamStat(HANDLE hStream,
*                           AMP_OUT AMP_AUD_PIPELINE_STAT* pPipeLineStat);)
*   \brief  Get stream/tunnel status.
*   \param hStream Stream/tunnel handle
*   \param pPipeLineStat every component status in pipeline.
*   \return SUCCESS if success
*/
HRESULT AMP_SND_QueryStreamStat(AMP_IN HANDLE hStream,
                    AMP_OUT AMP_AUD_PIPELINE_STAT* pPipeLineStat);

/**\func HRESULT AMP_SND_QueryPortStat(AMP_IN AMP_PORT_IO ePortIo,
*                           AMP_IN UINT32 uiPortIdx,
*                           AMP_OUT AMP_AUD_PIPELINE_STAT* pPipeLineStat);
*   \brief  Get sound port status.
*   \param ePortIo input or output.
*   \param uiPortIdx port ID.
*   \param pPipeLineStat every component status in pipeline.
*   \return SUCCESS if success
*/
HRESULT AMP_SND_QueryPortStat(AMP_IN AMP_PORT_IO ePortIo,
                        AMP_IN UINT32 uiPortIdx,
                        AMP_OUT AMP_AUD_PIPELINE_STAT* pPipeLineStat);

/**\func HRESULT AMP_SND_QueryStreamLatency(HANDLE hStream,
*                           AMP_OUT UINT32* pUiLatency)
*   \brief  Get stream/tunnel status.
*   \param hStream Stream/tunnel handle
*   \param pUiLatency all components delay time of the stream.
*   \return SUCCESS if success
*/
HRESULT AMP_SND_QueryStreamLatency(HANDLE hStream,
                    AMP_OUT UINT32* pUiLatency);

/**\func HRESULT AMP_SND_QueryLatency_Mix(AMP_SND_PORT_CFG * stSource, UINT32 * pUiLatency);
*    \brief Get latency time in sound service for audio patch,
*            and the type of source port must be mix.
*    \param stSource The pointer of source port configure.
*    \param pUiLatency Pointer that used to get snd delay time, unit is ms
*    \return SUCCESS if success
*/
HRESULT AMP_SND_QueryLatency_Mix(AMP_SND_PORT_CFG * stSource, UINT32 *pUiLatency);

/**\func HRESULT AMP_SND_QueryStatus_Mix(AMP_SND_PORT_CFG * stSource,
*                                        UINT32 *pUiLatency, UINT32 *pStatFlag);
*    \brief Get status in sound service for audio patch,
*            and the type of source port must be mix.
*    \param stSource The pointer of source port configure.
*    \param pUiLatency Pointer that used to get snd delay time, unit is ms
*    \param pStatFlag Pointer that used to get status, unit is ms
*    \return SUCCESS if success
*/
HRESULT AMP_SND_QueryStatus_Mix(AMP_SND_PORT_CFG * stSource, UINT32 *pUiLatency, UINT32 *pStatFlag);

/**\func HRESULT AMP_SND_QueryLatency_Device(AMP_SND_DEVICE_CFG * stDeviceCfg,
*                                   UINT32 * pUiLatency,UINT32 uiUpPortIdx);
*    \brief Get latency time in sound service for audio patch,
*                and the type of source port must be device.
*    \param stDeviceCfg The pointer of device configure.
*    \param pUiLatency Pointer that used to get snd delay time, unit is ms
*    \param uiUpPortIdx OutPort index of AREN component:
*                            uiUpPortIdx = 0, for PCM output;
*                            uiUpPortIdx = 1, for SPDIF pass through output;
*                            uiUpPortIdx = 2, for HDMI pass through output;
*    \return SUCCESS if success
*/
HRESULT AMP_SND_QueryLatency_Device(AMP_SND_DEVICE_CFG * stDeviceCfg,
                                              UINT32 * pUiLatency,UINT32 uiUpPortIdx);

/**\func HRESULT AMP_SND_GetSNDDelayTimeInMs(AMP_OUT UINT32 *uiSNDDelay);
*   \brief Get delay time in sound service
*   \brief param uiSNDDelay pointer that used to get snd delay time, unit is ms
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetSNDDelayTimeInMs(AMP_OUT UINT32 *uiSNDDelay);

/**\func HRESULT AMP_SND_SetMultiToBTMode(AMP_IN AMP_APP_PARAMULTI2BT *stMulti2BT)
*   \brief  Setting of switching 7.1 path to BT path
*   \param stMulti2BT switching option
*       TRUE: stop BT path, 7.1 output switching to BT ouput
*       FALSE: recover 7.1 output that is switched to BT path, and BT path restart.
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetMultiToBTMode(AMP_IN AMP_APP_PARAMULTI2BT *stMulti2BT);

/**\func HRESULT AMP_SND_SetSRSEnable(AMP_IN AMP_SND_PATH Path,
*                                   AMP_IN AMP_APP_PARASRSENABLE * stSRSEnable);
*   \brief add/remove SRS component in pipeline
*   \param stSRSEnable Add(stSRSEnable.bSRSEnable = TRUE) or
*                remove(stSRSEnable.bSRSEnable = FALSE)  SRS in pipeline
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetSRSEnable(AMP_IN AMP_SND_PATH Path,
                                AMP_IN AMP_APP_PARASRSENABLE * stSRSEnable);
/**\func HRESULT AMP_SND_GetSRSEnable(AMP_IN AMP_SND_PATH Path,
*                               AMP_OUT AMP_APP_PARASRSENABLE * stSRSEnable);
*   \brief Get SRS enable status
*   \param stSRSEnable pointer to get SRS enable status
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetSRSEnable(AMP_IN AMP_SND_PATH Path,
                                AMP_OUT AMP_APP_PARASRSENABLE * stSRSEnable);
/**\func AMP_SND_SetAoutDump(AMP_IN AMP_SND_PATH Path,
*                               UINT32 state);
*   \brief aout data dump control
*      To stop aout data dump, call AMP_SND_SetAoutDump(path, AOUT_DUMPSTOP) default
*      To start or continue aout data dump, call AMP_SND_SetAoutDump(path, AOUT_DUMPSTART)
*      To pause aout data dump, call AMP_SND_SetAoutDump(path, AOUT_DUMPPAUSE)
*   \param  Path: The output port index of app, the index should be one of :
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*                    AMP_SND_PATH_SPDIF = 2,
*                    AMP_SND_PATH_HDMI = 3,

*          state: AOUT_DUMPSTOP: Stop data dump
*                 AOUT_DUMPSTART: Start or Continue data dump
*                 AOUT_DUMPPAUSE: Pause data dump
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetAoutDump(AMP_IN AMP_SND_PATH Path,
                               AMP_IN UINT32 state);

/**\func HRESULT AMP_SND_SetNeuralEnable(AMP_IN AMP_APP_PARANEURALENABLE * stNeuralEnable);
*   \brief enable/disable DTS Neural support
*   \param stNeuralEnable enable(stNeuralEnable.bNeuralEnable = TRUE) or
*                disable(stNeuralEnable.bNeuralEnable = FALSE) DTS Neural support
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetNeuralEnable(AMP_IN AMP_APP_PARANEURALENABLE * stNeuralEnable);

/**\func HRESULT AMP_SND_SetStreamNeural(AMP_IN HANDLE hStream,
*                                  AMP_IN AMP_APP_PARANEURALENABLE * stNeuralEnable);
*   \brief enable/disable DTS Neural support for stream
*   \param stNeuralEnable enable(stNeuralEnable.bNeuralEnable = TRUE) or
*                disable(stNeuralEnable.bNeuralEnable = FALSE) DTS Neural support
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetStreamNeural(AMP_IN HANDLE hStream,
                                   AMP_IN AMP_APP_PARANEURALENABLE * stNeuralEnable);

/**\func HRESULT AMP_SND_GetNeuralEnable(AMP_OUT AMP_APP_PARANEURALENABLE * stNeuralEnable);
*   \brief Get DTS Neural enable status
*   \param stNeuralEnable pointer to get DTS Neural enable status
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetNeuralEnable(AMP_OUT AMP_APP_PARANEURALENABLE * stNeuralEnable);

/**\func HRESULT AMP_SND_SetNS227Cfg(AMP_IN AMP_APP_PARANS227CFG * stNS227Cfg);
*   \brief Set settings for DTS Neural NS227
*   \param stNS227Cfg: pointer of NS227 settings to set
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetNS227Cfg(AMP_IN AMP_APP_PARANS227CFG * stNS227Cfg);

/**\func HRESULT AMP_SND_GetNS227Cfg(AMP_IN AMP_APP_PARANS227CFG * stNS227Cfg);
*   \brief Get settings for DTS Neural NS227
*   \param stNS227Cfg: pointer of NS227 settings to get
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetNS227Cfg(AMP_IN AMP_APP_PARANS227CFG * stNS227Cfg);

/**\func HRESULT AMP_SND_SetNS527Cfg(AMP_IN AMP_APP_PARANS527CFG * stNS527Cfg);
*   \brief Set settings for DTS Neural NS527
*   \param stNS527Cfg: pointer of NS527 settings to set
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetNS527Cfg(AMP_IN AMP_APP_PARANS527CFG * stNS527Cfg);

/**\func HRESULT AMP_SND_GetNS527Cfg(AMP_IN AMP_APP_PARANS527CFG * stNS527Cfg);
*   \brief Get settings for DTS Neural NS527
*   \param stNS527Cfg: pointer of NS527 settings to get
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetNS527Cfg(AMP_IN AMP_APP_PARANS527CFG * stNS527Cfg);

/**\func AMP_SND_CreateAudioPatch(AMP_IN AMP_SND_PORT_CFG *stSources,
*                                 AMP_IN UINT32 uiSourcesNum,
*                                 AMP_IN AMP_SND_PORT_CFG *Sinks, AMP_IN UINT32 uiSinksNum,
*                                 AMP_OUT AMP_SND_PATCH_HANDLE *phHandle);
*    \brief Create audio patch
*    \param   AMP_SND_PORT_CFG *stSources  =   Sources Port config
*                    UINT32  uiSourcesNum   = Sources Number cannot>1
*                    AMP_SND_PORT_CFG *stSinks  =  Sinks Port config array
*                    UINT32  uiSinksNum  =  Sinks Number
*                    AMP_SND_PATCH_HANDLE *phHandle  =  Audio Patch Handle
*    \return SUCCESS if success
*/
HRESULT AMP_SND_CreateAudioPatch(AMP_IN AMP_SND_PORT_CFG *stSources,
                                 AMP_IN UINT32 uiSourcesNum,
                                 AMP_IN AMP_SND_PORT_CFG *stSinks, AMP_IN UINT32 uiSinksNum,
                                 AMP_OUT AMP_SND_PATCH_HANDLE *phHandle);

/**\func AMP_SND_ReleaseAudioPatch(AMP_IN AMP_SND_PATCH_HANDLE phHandle);
*    \brief Release audio patch, delete source and sink port couple from Table
*    \param   AMP_SND_PATCH_HANDLE phHandle  =   Audio Patch Handle
*    \return SUCCESS if success
*/
HRESULT AMP_SND_ReleaseAudioPatch(AMP_IN AMP_SND_PATCH_HANDLE phHandle);

/**\func AMP_SND_AttachDevicePort(AMP_IN AMP_COMPONENT hUpCompObj,
*                                   AMP_IN AMP_SND_PORT_CFG *stSources);
*    \brief Connect device(hardware) patch, create pipeline according to Port Mapping Table
*    \param   AMP_COMPONENT hUpCompObj  =    upstream AREN
*                    AMP_SND_DEVICE_CFG *stDeviceCfg  =  Device config
*    \return SUCCESS if success
*/
HRESULT AMP_SND_AttachDevicePort(AMP_IN AMP_COMPONENT hUpCompObj,
                                   AMP_IN AMP_SND_DEVICE_CFG *stDeviceCfg);

/**\func AMP_SND_DetachDevicePort(AMP_IN AMP_SND_PORT_CFG *stSources);
*    \brief Release audio patch connection, delete pipeline
*    \param   AMP_SND_DEVICE_CFG *stDeviceCfg =  Device config
*    \return SUCCESS if success
*/
HRESULT AMP_SND_DetachDevicePort(AMP_IN AMP_SND_DEVICE_CFG *stDeviceCfg);

/**\func AMP_SND_WriteStream(AMP_IN AMP_SND_PORT_CFG *stSources,
*                                 AMP_IN VOID *pData, AMP_IN UINT32 uiDataSize);
*    \brief write stream to output sink through Sound Service
*    \param   AMP_SND_PORT_CFG *ststSources  =  Sources Port config
*                    VOID * pData  =  point to data
*                    UINT32 uiDataSize  =  Data Size
*    \return SUCCESS if success
*/
HRESULT AMP_SND_WriteStream(AMP_IN AMP_SND_PORT_CFG *stSources,
                                    AMP_IN VOID *pData, AMP_IN UINT32 uiDataSize);

/**\func AMP_SND_GetLatency(AMP_IN AMP_SND_PATCH_HANDLE phHandle,
*                                    AMP_OUT UINT32 uiLatency);
*    \brief get latency via audio patch information
*    \param   AMP_SND_PATCH_HANDLE *phHandle  =  audio patch handle
*                    UINT32 uiLatency  =  latency
*    \return SUCCESS if success
*/
HRESULT AMP_SND_GetLatency(AMP_IN AMP_SND_PATCH_HANDLE phHandle,
                                    AMP_OUT UINT32 uiLatency);

/**\func AMP_SND_SetPortVolume(AMP_IN AMP_SND_PORT_CFG *stAudioPort,
*                                    AMP_IN AMP_APP_PARAVOLUME *stVolume);
*    \brief set volume for source or sink through Sound Service
*    \param   AMP_SND_PORT_CFG *stAudioPort  =  source or sink
*                    AMP_APP_PARAVOLUME *stVolume  = point to volume value
*    \return SUCCESS if success
*/
HRESULT AMP_SND_SetPortVolume(AMP_IN AMP_SND_PORT_CFG *stAudioPort,
                                    AMP_IN AMP_APP_PARAVOLUME *stVolume);

/**\func AMP_SND_GetPortVolume(AMP_IN AMP_SND_PORT_CFG *stAudioPort,
                                    AMP_OUT AMP_APP_PARAVOLUME *stVolume)
*    \brief get volume for source or sink through Sound Service
*    \param   AMP_SND_PORT_CFG *stAudioPort  =  source or sink
*                    AMP_APP_PARAVOLUME *stVolume  =  point to volume value
*    \return SUCCESS if success
*/
HRESULT AMP_SND_GetPortVolume(AMP_IN AMP_SND_PORT_CFG *stAudioPort,
                                    AMP_OUT AMP_APP_PARAVOLUME *stVolume);

/**\func AMP_SND_SetPortVolumeIndB(AMP_IN AMP_SND_PORT_CFG *stAudioPort,
*                                    AMP_IN float fVolume);
*    \brief set volume(dB) for source or sink through Sound Service
*    \param   AMP_SND_PORT_CFG *stAudioPort  =  source or sink
*                    float fVolume  =  volume value (in dB)
*    \return SUCCESS if success
*/
HRESULT AMP_SND_SetPortVolumeIndB(AMP_IN AMP_SND_PORT_CFG *stAudioPort,
                                    AMP_IN float fVolume);

/**\func AMP_SND_GetPortVolumeIndB(AMP_IN AMP_SND_PORT_CFG *stAudioPort,
*                                    AMP_OUT float *fVolume);
*    \brief get volume(dB) for source or sink through Sound Service
*    \param   AMP_SND_PORT_CFG *stAudioPort  =  source or sink
*                    float *fVolume  =  point to volume value (in dB)
*    \return SUCCESS if success
*/
HRESULT AMP_SND_GetPortVolumeIndB(AMP_IN AMP_SND_PORT_CFG *stAudioPort,
                                    AMP_OUT float *fVolume);

/**\func AMP_SND_SetPortMute(AMP_IN AMP_SND_PORT_CFG *stAudioPort,
*                                    AMP_IN BOOL bMute);
*    \brief set mute for source or sink through Sound Service
*    \param   AMP_SND_PORT_CFG *stAudioPort  =  source or sink
*                    BOOL bMute  =  mute state
*    \return SUCCESS if success
*/
HRESULT AMP_SND_SetPortMute(AMP_IN AMP_SND_PORT_CFG *stAudioPort,
                                    AMP_IN BOOL bMute);

/**\func AMP_SND_GetPortMute(AMP_IN AMP_SND_PORT_CFG *stAudioPort,
*                                    AMP_OUT BOOL *bMute);
*    \brief get mute for source or sink through Sound Service
*    \param   AMP_SND_PORT_CFG *stAudioPort  =  source or sink
*                    BOOL bMute  =  point to mute state
*    \return SUCCESS if success
*/
HRESULT AMP_SND_GetPortMute(AMP_IN AMP_SND_PORT_CFG *stAudioPort,
                                    AMP_OUT BOOL *bMute);
/**\func AMP_SND_SetAudioPatchStatus(AMP_IN AMP_APP_PARAAUDIOPATCHSTATUS *stAudioPatchStatus);
*   \brief Set audio patch status, the default status of audio patch is disabled and the
           status must be set to enable before creating audio patch
*   \param stAudioPatchStatus pointer to set audio patch status
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetAudioPatchStatus(AMP_IN AMP_APP_PARAAUDIOPATCHSTATUS *stAudioPatchStatus);

/**\func AMP_SND_GetAudioPatchStatus(AMP_OUT AMP_APP_PARAAUDIOPATCHSTATUS *stAudioPatchStatus);
*   \brief Get audio patch status.
*   \param stAudioPatchStatus pointer to get audio patch status
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetAudioPatchStatus(AMP_OUT AMP_APP_PARAAUDIOPATCHSTATUS *stAudioPatchStatus);

/**\func AMP_SND_SetEncoder(AMP_IN AMP_SND_PATH Path, AMP_IN BOOL bEnable)
*   \brief Enable or disable the encoder on specific path.
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_SPDIF = 2,
*                    AMP_SND_PATH_HDMI = 3,
*
*   \param bEnable TRUE to enable, FALSE to disable.
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetEncoder(AMP_IN AMP_SND_PATH Path, AMP_IN BOOL bEnable);

/**\func HRESULT AMP_SND_GetEncoder(AMP_IN AMP_SND_PATH Path, AMP_IN BOOL *pEnable)
*   \brief Get the encoder status of one path.
*   \param Path The output port index of app, the index should be one of :
*                    AMP_SND_PATH_SPDIF = 2,
*                    AMP_SND_PATH_HDMI = 3,
*
*   \param pEnable The pointer to a BOOL value to indicate if encoder was enabled.
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetEncoder(AMP_IN AMP_SND_PATH Path, AMP_IN BOOL *pEnable);

/**\func AMP_SND_GetAppCapability(AMP_OUT AMP_APP_PARAIDXAPPCAP *stAppCap);
*    \brief get APP component capability.
*    \param   AMP_APP_PARAIDXAPPCAP *stAppCap  =  capability
*    \return SUCCESS if success
*/
HRESULT AMP_SND_GetAppCapability(AMP_OUT AMP_APP_PARAIDXAPPCAP *stAppCap);

/**\func AMP_SND_GetAoutCapability(AMP_OUT AMP_AOUT_PARAIDXAOUTPTCAP *pAoutPtCap);
*    \brief get AOUT component capability.
*    \param   AMP_AOUT_PARAIDXAOUTPTCAP *stAppCap  =  capability
*    \return SUCCESS if success
*/
HRESULT AMP_SND_GetAoutPtCap(AMP_IN AMP_AOUT_PARAIDXAOUTPTCAP *pAoutPtCap);

/**\func HRESULT AMP_SND_StopAudioPatchMIX(AMP_IN AMP_SND_PATCH_HANDLE stHandle);
*   \brief Stop audio patch with mix type
*   \param stHandle audio patch handle
*   \return SUCCESS if succee
*/
HRESULT AMP_SND_StopAudioPatchMIX(AMP_IN AMP_SND_PATCH_HANDLE stHandle);

/**\func HRESULT AMP_SND_StartAudioPatchMIX(AMP_IN AMP_SND_PATCH_HANDLE stHandle);
*   \brief Start audio patch with mix type
*   \param stHandle audio patch handle
*   \return SUCCESS if succee
*/
HRESULT AMP_SND_StartAudioPatchMIX(AMP_IN AMP_SND_PATCH_HANDLE stHandle);

/**\func HRESULT AMP_SND_PaudeAudioPatchMIX(AMP_IN AMP_SND_PATCH_HANDLE stHandle);
*   \brief Pause audio patch with mix type
*   \param stHandle audio patch handle
*   \return SUCCESS if succee
*/
HRESULT AMP_SND_PauseAudioPatchMIX(AMP_IN AMP_SND_PATCH_HANDLE stHandle);

/**\func AMP_SND_SetPathHwMute(AMP_IN AMP_SND_PATH ePath, AMP_IN BOOL bMute);
*   \brief Set AIO mute for specific path
*   \param  ePath The path index in AIO, the index should be one of:
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*                    AMP_SND_PATH_SPDIF = 2,
*                    AMP_SND_PATH_HDMI = 3,
*   \param  bMute   Mute status of AIO
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetPathHwMute(AMP_IN AMP_SND_PATH ePath, AMP_IN BOOL bMute);

/**\func HRESULT AMP_SND_GetPathHwMute(AMP_IN AMP_SND_PATH ePath, AMP_OUT BOOL *pMute);
*   \brief Get AIO mute status for specific path
*   \param  ePath The path index in AIO, the index should be one of:
*                    AMP_SND_PATH_71 = 0,
*                    AMP_SND_PATH_20 = 1,
*                    AMP_SND_PATH_SPDIF = 2,
*                    AMP_SND_PATH_HDMI = 3,
*   \param pMute The pointer to store mute status
*   \retrun SUCCESS if success
*/
HRESULT AMP_SND_GetPathHwMute(AMP_IN AMP_SND_PATH ePath, AMP_OUT BOOL *pMute);

/**\func HRESULT HRESULT AMP_SND_SetSRCMode(
*                           AMP_IN INT uiMode, AMP_IN UINT32 uiPortIdx);
*   \brief  Change APP to  set SRC mode
*   \param stSrcCfg APP SRC configuration
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetSRCMode(AMP_IN INT uiMode, AMP_IN UINT32 uiPortIdx);

/**\func HRESULT AMP_SND_SetSonic_Speed(HANDLE hStream, float uSpeed);
 *   \brief Set Audio playback Speed:(e.g: 0.5X,0.8x,1.0X,1.2X,2X)
 *   \param hStream pointer to be configured Stream handle
 *   \param speed: (0.5X~6.0X)
 *   \return SUCCESS if success
*/
HRESULT AMP_SND_SetSonic_Speed(HANDLE hStream, float speed);
HRESULT AMP_SND_GetSonic_Speed(HANDLE hStream, float *speed);

/**\func HRESULT AMP_SND_SetAudio_Speed(HANDLE hStream,AMP_AUDIO_SPEED uSpeed);
 *   \brief Set Audio playback Speed:(e.g: 0.5X,0.8x,1.0X,1.2X,2X)
 *   \param hStream pointer to be configured Stream handle
 *   \ uSpeed:
 *       AMP_AUDIO_SPEED_800X   //0.8X
 *       AMP_AUDIO_SPEED_1000X  //1X
 *       AMP_AUDIO_SPEED_1200X  //1.2X
 *       AMP_AUDIO_SPEED_1500X  //1.5X
 *       AMP_AUDIO_SPEED_2000X  //2X
 *  (TODO: now only support 0.8X, 1X,1.2X, need extern this latter)
 *   \return SUCCESS if success
*/
HRESULT AMP_SND_SetAudio_Speed(HANDLE hStream, AMP_AUDIO_SPEED uSpeed);
HRESULT AMP_SND_GetAudio_Speed(HANDLE hStream, AMP_AUDIO_SPEED *uSpeed);

/**\func HRESULT AMP_SND_GetSndHDMISinkCap(AMP_OUT AMP_SNDSINK_HDMI_CAP * pSndHDMICap);
*   \brief Get hdmi capability
*   \param pSndHDMICap parameters used to store hdmi capability
*   \return SUCCESS if success
*/
HRESULT AMP_SND_GetSndHDMISinkCap(AMP_OUT AMP_SNDSINK_HDMI_CAP * pSndHDMICap);

/**\func HRESULT AMP_SND_SetSndHDMISinkCap(AMP_OUT AMP_SNDSINK_HDMI_CAP * pSndHDMICap);
*   \brief Set hdmi capability
*   \param pSndHDMICap parameters used to set hdmi capability
*   \return SUCCESS if success
*/
HRESULT AMP_SND_SetSndHDMISinkCap(AMP_IN AMP_SNDSINK_HDMI_CAP * pSndHDMICap);

/**\func AMP_SND_CreatePatchSink(AMP_IN AMP_SND_PORT_CFG *Sinks,
*                           AMP_IN UINT32 uiSinksNum,
*                           AMP_OUT UINT32 *hSinkHandle);
*    \brief Create audio patch Sink, , have no relation with stream.
*    \param          AMP_SND_PORT_CFG *stSinks  =  Sinks Port config array
*                    UINT32  uiSinksNum  =  Sinks Number
*                    HANDLE *phHandle  =  Audio Sink Handle
*    \return SUCCESS if success
*/
HRESULT AMP_SND_CreatePatchSink(AMP_IN AMP_SND_PORT_CFG *pSinks,
                                 AMP_IN UINT32 uiSinksNum,
                                 AMP_OUT UINT32 *hSinkHandle);

/**\func AMP_SND_ReleasePatchSink(AMP_IN UINT32 hSinkHandle);
*    \brief Release audio patch Sink, Just release sink port, have no relation with stream
*    \param  UINT32 hSinkHandle  =   Audio Sink Handle
*    \return SUCCESS if success
*/
HRESULT AMP_SND_ReleasePatchSink(AMP_IN UINT32 hSinkHandle);

/**\func AMP_SND_AttachAudioPatch(AMP_IN HANDLE hStream,
                                 AMP_IN UINT32 hSinkHandle);
*    \brief Connect sink(hardware) stream
*    \param   HANDLE hStream  =    stream handle
*             UINT32 hSinkHandle  =  sink handle
*    \return SUCCESS if success
*/
HRESULT AMP_SND_AttachAudioPatch(AMP_IN HANDLE hStream,
                                 AMP_IN UINT32 hSinkHandle);

/**\func AMP_SND_DetachAudioPatch(AMP_IN HANDLE hStream,
*                                 AMP_IN UINT32 hSinkHandle)
*    \brief disconnect stream and sink.
*           then stream will Not output on all outport.
*    \param   HANDLE hStream  =    stream handle
*             UINT32 hSinkHandle  =  sink handle
*    \return SUCCESS if success
*/
HRESULT AMP_SND_DetachAudioPatch(AMP_IN HANDLE hStream,
                                 AMP_IN UINT32 hSinkHandle);

/** \func AMP_SND_CreateStreamPassThrough(AMP_SND_STRM_ATTR *pAttr,
                                          AMP_SND_PATH eSndPath,
                                               HANDLE *phStream)
    \brief This API create context for one input passthrough stream
    \param pAttr stream attribute for parameter setting, if NULL, sound uses the
           default setting (2-ch, 44KHz, interleaved)
    \param eSndPath: AMP_SND_PATH_SPDIF or AMP_SND_PATH_HDMI
    \param phStream pointer to returned stream handle
    \return SUCCESS for success\n
            For other code, refer to amp_types.h
*/
HRESULT AMP_SND_CreateStreamPassThrough(AMP_SND_STRM_ATTR *pAttr,
                                        AMP_SND_PATH eSndPath,
                                        HANDLE *phStream);

/**\func AMP_SND_PushRawData(AMP_IN HANDLE hStream,
*                                 AMP_IN VOID *pData,
*                                 AMP_IN UINT32 uiDataSize);
*    \brief write raw data stream to output sink through Sound Service
*    \param phStream pointer to returned stream handle
*                    VOID * pData  =  point to data
*                    UINT32 uiDataSize  =  Data Size
*    \return SUCCESS if success
*/
HRESULT AMP_SND_PushRawData(AMP_IN HANDLE hStream,
                                 AMP_IN VOID *pData,
                                 AMP_IN UINT32 uiDataSize);

/**\func AMP_SND_LoopBack_SetChMask
 *   \details This function is used by app to set the channel
 *            mask of export component in APP(audio post process) output.
 *            User can use this API to filter the appropriate channel(s)
 *            by setting the corresponding bit as 1
 *   \param *pChannelMask is an array.
 *          One member represents only one channel to be filtered, which means
 *          multiple channels should use multiple array members. Only one bit
 *          is allowed to be set in each member of the array to represent the
 *          channel filtered.
 *          the table of channel mask is:
 *          AMP_AUDIO_CHMASK_LEFT = 0x00000001,
 *          AMP_AUDIO_CHMASK_RGHT  = 0x00000002,
 *          AMP_AUDIO_CHMASK_CNTR  = 0x00000004,
 *          AMP_AUDIO_CHMASK_SRRD_LEFT  = 0x00000008,
 *          AMP_AUDIO_CHMASK_SRRD_RGHT   = 0x00000010,
 *          AMP_AUDIO_CHMASK_LFE = 0x00000020,
 *          AMP_AUDIO_CHMASK_SRRD_CNTR   = 0x00000040,
 *   \param uiChanNum user input channel number to be filtered
 *
 */
HRESULT AMP_SND_LoopBack_SetChMask(UINT32 *pChannelMask,
                                                   UINT32 uiChanNum);
/** \func HRESULT AMP_SND_SetupTunnel_MS12(AMP_COMPONENT hUpCompObj,
*                   UINT32 uiUpPortIdx,
*                   UINT32 uiDownPortIdx,
*                   HANDLE * phTunnel)
*   \brief ms12 input and setup tunnel pipeline using ms12 components
*   \param hUpCompObj up componetn handle
*   \param uiUpPortIdx port index of up component
*   \param uiDownPortIdx select ms12 input port
*   \param phTunnel returned tunnel handle
*/
HRESULT AMP_SND_SetupTunnel_MS12(HANDLE hSnd, AMP_COMPONENT hUpCompObj,
                                                    UINT32 uiUpPortIdx,
                                                    UINT32 uiDownPortIdx,
                                                    HANDLE *phTunnel);

/** \func HRESULT AMP_SND_SetupTunnel_MS12_4_DDP(AMP_COMPONENT hUpCompObj,
*                   UINT32 uiUpPortIdx,
*                   UINT32 uiDownPortIdx,
*                   HANDLE * phTunnel)
*   \brief ms12 input and setup tunnel pipeline using ms12 components
*   \param hUpCompObj up componetn handle
*   \param uiUpPortIdx port index of up component
*   \param uiDownPortIdx select ms12 input port
*   \param phTunnel returned tunnel handle
*/
HRESULT AMP_SND_SetupTunnel_MS12_4_DDP(HANDLE hSnd, AMP_COMPONENT hUpCompObj,
                                                    UINT32 uiUpPortIdx,
                                                    UINT32 uiDownPortIdx,
                                                    HANDLE *phTunnel);

/** \func HRESULT AMP_SND_SetupTunnel_MS12_4_MAT(AMP_COMPONENT hUpCompObj,
*                   UINT32 uiUpPortIdx,
*                   UINT32 uiDownPortIdx,
*                   HANDLE * phTunnel)
*   \brief ms12 input and setup tunnel pipeline using ms12 components
*   \param hUpCompObj up componetn handle
*   \param uiUpPortIdx port index of up component
*   \param uiDownPortIdx select ms12 input port
*   \param phTunnel returned tunnel handle
*/
HRESULT AMP_SND_SetupTunnel_MS12_4_MAT(HANDLE hSnd, AMP_COMPONENT hUpCompObj,
                                                    UINT32 uiUpPortIdx,
                                                    UINT32 uiDownPortIdx,
                                                    HANDLE *phTunnel);

/** \func HRESULT AMP_SND_SetupTunnel_MS12_4_DAP(AMP_COMPONENT hUpCompObj,
*                   UINT32 uiUpPortIdx,
*                   UINT32 uiDownPortIdx,
*                   HANDLE * phTunnel)
*   \brief ms12 input and setup tunnel pipeline using ms12 components
*   \param hUpCompObj up componetn handle
*   \param uiUpPortIdx port index of up component
*   \param uiDownPortIdx select ms12 input port
*   \param phTunnel returned tunnel handle
*/
HRESULT AMP_SND_SetupTunnel_MS12_4_DAP(HANDLE hSnd, AMP_COMPONENT hUpCompObj,
                                                    UINT32 uiUpPortIdx,
                                                    UINT32 uiDownPortIdx,
                                                    HANDLE *phTunnel);

/** \func HRESULT AMP_SND_RemoveTunnel_MS12(UINT32 hTunnel)
*   \brief remove tunnel pipeline using ms12 components dap
*   \param phTunnel tunnel handle to be removed
*/
HRESULT AMP_SND_RemoveTunnel_MS12(HANDLE hSnd, HANDLE hTunnel);

/** \func AMP_SND_CreateStream_MS12(AMP_IN AMP_SND_STRM_ATTR *pAttr,
*                                   AMP_IN UINT32 uiPortIdx,
*                                      AMP_OUT HANDLE *phStream)
*    \brief The API create context for ms12 use cases
*    \param pAttr stream attribute for parameter setting
*    \param uiPortIdx is the port index of ms12 input port
*    \param phStream pointer to returned stream handle
*    \return SUCCESS for success
*/
HRESULT AMP_SND_CreateStream_MS12(AMP_IN AMP_SND_STRM_ATTR * pAttr,
                                         AMP_IN UINT32 uiPortIdx,
                                                      AMP_OUT HANDLE * phStream);


/** \func AMP_SND_DestroyStream_MS12(AMP_IN HANDLE hStream)
*     \brief The API destroy context that crated by AMP_SND_CreateStream_RingTone()
*     \param hStream pointer to stream handle to be destroyed
*     \return SUCCESS for success
*/
HRESULT AMP_SND_DestroyStream_MS12(AMP_IN HANDLE hStream);

/** \func AMP_SND_SetAudioRoute(HANDLE hStream, UINT32 uiSinkNum, UINT32 *pSinkPath)
*     \brief The API set audio route for native case
*     \param hStream pointer to stream handle to be destroyed
*     \param uiSinkNum route sink numbers
*     \param pSinkPath point to sink arrays
*     \return SUCCESS for success
*/
HRESULT AMP_SND_SetAudioRoute(HANDLE hStream, UINT32 uiSinkNum, UINT32 *pSinkPath);

/** \func AMP_SND_SetMS12SurrDecoder(HANDLE hStream, UINT32 enable)
*     \brief The API set ms12 surround decoder enable
*     \param hStream pointer to be configured Stream handle
*     \param enable/disable value
*     \return SUCCESS for success
*/
HRESULT AMP_SND_SetMS12SurrDecoder(HANDLE hStream, UINT32 enable);

/** \func AMP_SND_SetMS12SurrVirtualizer(HANDLE hStream, UINT32 mode, UINT32 level)
*     \brief The API set ms12 surround virtualizer mode and level
*     \param hStream pointer to be configured Stream handle
*     \param mode virtualizer mode
*     \param level virtualizer level
*     \return SUCCESS for success
*/
HRESULT AMP_SND_SetMS12SurrVirtualizer(HANDLE hStream, UINT32 mode, UINT32 level);

/** \func AMP_SND_SetMS12_DAP_GEQ(HANDLE hStream, AMP_APP_PARAMS12GEQ *pMS12GEQ)
*     \brief The API set ms12 graphic EQ
*     \param hStream pointer to be configured Stream handle
*     \param pMS12GEQ is the pointer to the structure of graphic EQ coef
*     \return SUCCESS for success
*/
HRESULT AMP_SND_SetMS12_DAP_GEQ(HANDLE hStream, AMP_APP_PARAMS12GEQ *pMS12GEQ);

/** \func AMP_SND_SetMS12AtmosLock(HANDLE hStream, UINT32 enable)
*     \brief The API sets MS12 Atmos lock feature which will lock&unlock
*     \Dolby Atmos on Dolby Digital Plus or Dolby MAT output
*     \param hStream pointer is to configure Stream handle, and it can be set as NULL
*     \param enable, enable or disalbe Atmos lock feature
*     \return SUCCESS for success
*/
HRESULT AMP_SND_SetMS12AtmosLock(HANDLE hStream, UINT32 enable);

/** \func AMP_SND_SetMS12DownmixOut(HANDLE hStream, AMP_APP_PARAMS12DOWNMIXOUT *stDownMixOut)
*     \brief The API sets MS12 Down-mix output related parameters, which are:
*           uiDownmixType, Downmix modes
*                          0 = Lt/Rt (Default)
*                          1 = Lo/Ro
*           uiDrcMode,     DRC modes (for downmixed output)
*                          0 = Line (Default)
*                          1 = RF
*           uiDrcBoost,    Scale factor for incoming DRC boost value
*                          0 - 100; Default = 100
*           uiDrcCut,      Scale factor for incoming DRC cut value
*                          0 - 100; Default = 100
*           bDownmixOut,   Specify 2ch or multi-ch output
*                          TRUE  - 2ch      output;
*                          FALSE - mutli-ch output;
*           uiDualMode,    Dual Mode type if channel mode is dual_mono
*                          0 = Stereo/Both channels (Default);
*                          1 = Left/First channel;
*                          2 = Right/Second channel;
*           bDualMono,     Specify channel mode is dual mono or not
*                          TRUE  - dual mono;
*                          FALSE - not dual mono;
*     \param hStream pointer is to configure Stream handle, and it can be set as NULL
*     \return SUCCESS for success
*/
HRESULT AMP_SND_SetMS12DownmixOut(HANDLE hStream, AMP_APP_PARAMS12DOWNMIXOUT *stDownMixOut);

/** \func AMP_SND_SetMS12AppReset(BOOL enable);
*     \brief The API set ms12 app reset
*     \param enable/disable value
*     \return SUCCESS for success
*/
HRESULT AMP_SND_SetMS12AppReset(BOOL enable);

/** \func AMP_SND_BTStandBy();
*     \brief The API set bt stand by
*     \return SUCCESS for success
*/
HRESULT AMP_SND_BTStandBy();

/** \func AMP_SND_BTResume();
*     \brief The API resume bt
*     \return SUCCESS for success
*/
HRESULT AMP_SND_BTResume();

/** \func AMP_SND_LoopBack_Switch(BOOL eState);
*     \brief The API switch loopback path state
*     \param eState TRUE for enable loopback path, FALSE for close
*     \return SUCCESS for success
*/
HRESULT AMP_SND_LoopBack_Switch(BOOL eState);

/** \func AMP_SND_SetMS12Gain(HANDLE hStream, INT32 PortIdx, INT32 tar, INT32 duration, INT32 type);
*     \brief The API set ms12 mixer gain
*     \param hStream pointer to be configured Stream handle, can set to NULL
*     \param PortIdx is the port index that will take effect
*     \param tar is the Target attenuation at end of ramp in dB (range: -96...0)
*     \param duration of ramp in milliseconds (range: 0...60000)
*     \param type Shape of the ramp (0: linear, 1: in cube, 2: out cube)
*     \return SUCCESS for success
*/
HRESULT AMP_SND_SetMS12Gain(HANDLE hStream, INT32 PortIdx, INT32 tar, INT32 duration, INT32 type);

/** \func AMP_SND_ReloadMS12TuningFile(char *tuningfile);
*     \brief The API reload tuning file
*     \param tuningfile is the new tuning file name
*     \return SUCCESS for success
*/
HRESULT AMP_SND_ReloadMS12TuningFile(char *tuningfile);

/** \func AMP_SND_SetContinuesAudio(HANDLE hStream, UINT32 enable)
*    \brief The API sets Continues Audio feature for application
*     \param enable, enable or disalbe Continues Audio feature
*     \return SUCCESS for success
*/
HRESULT AMP_SND_SetContinuesAudio(HANDLE hStream, UINT32 enable);

/** \func AMP_SND_SetMS12MediaIntelligence(UINT8 cEnable)
*     \brief The API sets MS12 Media Intelligence control which will
*     \steer the Intelligent Equalizer, Volume Leveler, Dialog Enhancer and Surround Compressor
*     \features of Dobly Audio Processing.
*     \
*     \param cEnable, enable or disable the Media Intelligence feature.
*     \#####INPUT RANGE
*     \     0, disable Media Intelligence;
*     \     1, enable  Media Intelligence;
*     \
*     \return SUCCESS for success
*/
HRESULT AMP_SND_SetMS12MediaIntelligence(UINT8 cEnable);

/** \func AMP_SND_SetMS12DialogEnhancer(UINT32 uiEnable, INT32 iAmount)
*     \brief The API sets MS12 Dialogue Enhancer which will
*     \1) enable or disalbe the Dialogue Enhancer feature
*     \2) if enabled, will also set the strength of the Dialogue Enhancer effect
*     \
*     \param uiEnable, enable or disable the Dialogue Enhancer feature.
*     \#####INPUT RANGE
*     \     0, disable Dialog Enhancer;
*     \     1, enable  Dialog Enhancer;
*     \
*     \param iAmount, set the strength of the Dialogue Enhancer effect.
*     \               Larger numbers result in more aggressive amplification of dialog.
*     \#####INPUT RANGE
*     \     [0, 16]
*     \
*     \     where,
*     \     0,  corresponds to the MIN effect
*     \     16, corresponds to the MAX effect
*     \
*     \return SUCCESS for success
*/
HRESULT AMP_SND_SetMS12DialogEnhancer(UINT32 uiEnable, INT32 iAmount);

/** \func AMP_SND_SetMS12VolumeLeveler(UINT32 uiSetting, UINT32 uiAmount)
*     \brief The API sets MS12 Volume Leveler which will
*     \1) configure 0:OFF, 1:ON, 2:AUTO for Volume Leveler feature
*     \2) if enabled, will also set the strength of the Volume Leveler effect
*     \
*     \param uiSetting, configure the Volume Leveler feature.
*     \#####INPUT RANGE
*     \     0, OFF;
*     \     1, ON;
*     \     2, AUTO;
*     \
*     \param uiAmount, set the strength of the Volume Leveler effect.
*     \               To maintain consistent playback levels for different content.
*     \#####INPUT RANGE
*     \     [0, 10]
*     \
*     \     where,
*     \     0,  corresponds to the MIN effect
*     \     10, corresponds to the MAX effect
*     \
*     \return SUCCESS for success
*/
HRESULT AMP_SND_SetMS12VolumeLeveler(UINT32 uiSetting, UINT32 uiAmount);

/** \func AMP_SND_SetMS12EnforceSingleOA(UINT32 uiEnable)
*     \brief The API sets MS12 Enforce Single OA which will
*     \Enforce single OA element in OAMD for MAT output when
*     \connected downstream AVR requires MAT hashing via HDMI SAD signaling.
*     \Degrades experience, but ensures interoperability with first generation Atmos capable AVR's
*     \
*     \param uiEnable, configure the Enforce Single OA feature.
*     \#####INPUT RANGE
*     \     0, disable;
*     \     1, enable;
*     \     (default: 0)
*     \
*     \return SUCCESS for success
*/
HRESULT AMP_SND_SetMS12EnforceSingleOA(UINT32 uiEnable);

/** \func AMP_SND_GetMS12VerCfg(void)
*     \brief The API get current MS12 Version and Config Mode
*     \param void
*     \return
*            pointer storing the info of MS12 Version and Config Mode
*/
CHAR * AMP_SND_GetMS12VerCfg(void);

/** \func AMP_SND_GetStreamHandleBySourceID(HANDLE *pHandle, UINT32 uiSourcePortID,
*        AMP_SND_STREAM_OUT_FORMAT eSndOutFmt)
*    \brief The API gets stream handle by source ID
*     \param pHandle, return stream handle
*     \param uiSourcePortID, source ID
*     \param eSndOutFmt, stream out format
*     \return SUCCESS for success
*/
HRESULT AMP_SND_GetStreamHandleBySourceID(HANDLE *pHandle, UINT32 uiSourcePortID,
        AMP_SND_STREAM_OUT_FORMAT eSndOutFmt);

/** \func AMP_SND_GetEncoderCapability(AMP_SND_ENCODER_CAP *pEncoderCap)
*    \brief The API gets sound service encoder capability
*     \param pEncoderCap, the point to sound service encoder capability structure
*     \return SUCCESS for success
*/
HRESULT AMP_SND_GetEncoderCapability(AMP_SND_ENCODER_CAP *pEncoderCap);

/** \func AMP_SND_GetPassThroughCapability(AMP_SND_PASSTRU_CAP *pPassTruCap)
*    \brief The API gets sound service direct pass through capability
*     \param pPassTruCap, the point to sound service direct passthrough capability structure
*     \return SUCCESS for success
*/
HRESULT AMP_SND_GetPassThroughCapability(AMP_SND_PASSTRU_CAP *pPassTruCap);

/** \func AMP_SND_SetAoutMultiDelay(UINT32 uiDelaySample)
*    \brief The API set aout multi-path delay sample number, used for internal speaker
*    \param uiDelaySample delay sample numbers, if delay 200ms, use 48 * 200 = 9600
*    \return SUCCESS for success
*/
HRESULT AMP_SND_SetAoutMultiDelay(UINT32 uiDelaySample);

/** \func AMP_SND_SetAudioCodecType(HANDLE hStream, UINT32 uiType)
*    \brief The API sets audio codec type
*     \param uiType refer to amp_component.idl ADEC_FORMAT
*     \return SUCCESS for success
*/
HRESULT AMP_SND_SetAudioCodecType(HANDLE hStream, UINT32 uiType);

/** \func AMP_SND_SetChmodLockingMode(HANDLE hStream, UINT32 uiMode)
*    \brief The API sets pipline channel locking mode for encoder output only, rely on MS12
*....\For MS12 V2 product, support both of DDP/DD encoder and MAT output
*....\For MS12 V1 product, only support DDP/DD encoder ouput
*    \param hStream could be NULL, which is reserved for future 2 encoder pipeline user case
*....\param uiMode, 0 - AUTO, 1 - locking to 5.1
*    \return SUCCESS for success
*/
HRESULT AMP_SND_SetChmodLockingMode(HANDLE hStream, UINT32 uiMode);

/** \func AMP_SND_SetSpdifCopyRight(UINT32 uiCopyRight)
*    \brief The API set spdif pack copy right
*     \param uiCopyRight, spdif copy right
*     \return SUCCESS for success
*/
HRESULT AMP_SND_SetSpdifCopyRight(UINT32 uiCopyRight);

/** \func AMP_SND_GetSpdifCopyRight(UINT32 *pCopyRight)
*    \brief The API get spdif pack copy right
*     \param pCopyRight, the point to spdif copy right
*     \return SUCCESS for success
*/
HRESULT AMP_SND_GetSpdifCopyRight(UINT32 *pCopyRight);

#ifdef __cplusplus
}
#endif

#endif //_AMP_SOUND_API_H_
