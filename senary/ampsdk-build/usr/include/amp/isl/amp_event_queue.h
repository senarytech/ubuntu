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
//! \file amp_event_queue.h
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

#ifndef __AMP_EVENT_QUEUE_H__
#define __AMP_EVENT_QUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "OSAL_api.h"
#include "amp_malloc.h"
#include "amp_types.h"
#include "isl/amp_logger.h"

/** \typedef enum AMP_EVENT_CODE
    \brief The event code is used for formated definition of an event type
           modules can define their event type for private usage, but it's
           required to use EVENTCODE_GEN to generate final event code.
 */
typedef UINT32 AMP_EVENT_CODE;

/** \def internal use only */
#define EVENTCODE_MAGIC             (0xbead0000)
#define EVENTCODE_TYPE_OFF          (0)
#define EVENTCODE_TYPE_SIZ          (8)
#define EVENTCODE_ZONE_OFF          (8)
#define EVENTCODE_ZONE_SIZ          (8)
#define EVENTCODE_MAGI_OFF          (16)
#define EVENTCODE_MAGI_SIZ          (16)
#define EVENTCODE_GEN(Zone, Base)   \
    (EVENTCODE_MAGIC | ((Zone & 0xff)<<8) | (Base & 0xff))

/** \def AMP_EVENT_GENCODE this macro is deprecated to use outside later.
    \brief use AMP_COMPONENT_TYPE as zone and event type to generate final event
           code.
 */
#define AMP_EVENT_GENCODE(Zone, Base)   EVENTCODE_GEN(MODULE_GENERIC, Base)

/** \def AMP_EVENT_TYPE
    \brief generic/templete event type for component share
 */
typedef enum {
    /** \brief Invalidate event ID.
     *
     *  This event is used to reserve the event code of "0". Don't use it.
     */
    AMP_EVENT_INVALID               = EVENTCODE_GEN(MODULE_GENERIC, 0x00),
    /** \brief Event for component Close() API.
     *
     *  This event is used in AMP server, and shouldn't used by application.
     */
    AMP_EVENT_API_CLOSE             = EVENTCODE_GEN(MODULE_GENERIC, 0x01),
    /** \brief Event for component SetState() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_SETSTATE          = EVENTCODE_GEN(MODULE_GENERIC, 0x03),
    /** \brief Event for component GetState() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_GETSTATE          = EVENTCODE_GEN(MODULE_GENERIC, 0x04),
    /** \brief Event for component QueryInfo() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_QUERYINFO         = EVENTCODE_GEN(MODULE_GENERIC, 0x05),
    /** \brief Event for component SetupPort() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_SETUPPORT         = EVENTCODE_GEN(MODULE_GENERIC, 0x06),
    /** \brief Event for component ClearPort() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_CLEARPORT         = EVENTCODE_GEN(MODULE_GENERIC, 0x07),
    /** \brief Event for component QueryPort() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_QUERYPORT         = EVENTCODE_GEN(MODULE_GENERIC, 0x08),
    /** \brief Event for component ClearPortBuf() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_CLEARPORTBUF      = EVENTCODE_GEN(MODULE_GENERIC, 0x09),
    /** \brief Event for component DebugDump() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_DEBUGDUMP         = EVENTCODE_GEN(MODULE_GENERIC, 0x0B),
    /** \brief Event for component SetLogLevel() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose. SetLogLevel() API doesn't be
     *  implemented at current stage, so this event is un-useful.
     */
    AMP_EVENT_API_SETLOGLEVEL       = EVENTCODE_GEN(MODULE_GENERIC, 0x0A),
    /** \brief Event for component RegisterNotify() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_REGISTERNOTIFY    = EVENTCODE_GEN(MODULE_GENERIC, 0x0C),
    /** \brief Event for component UnregisterNotify() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_UNREGISTERNOTIFY  = EVENTCODE_GEN(MODULE_GENERIC, 0x0D),
    /** \brief Event for component RegisterBD() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_REGISTERBD        = EVENTCODE_GEN(MODULE_GENERIC, 0x0E),
    /** \brief Event for component UnregisterBD() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_UNREGISTERBD      = EVENTCODE_GEN(MODULE_GENERIC, 0x0F),
    /** \brief Event for component PushBuf() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_PUSHBUF           = EVENTCODE_GEN(MODULE_GENERIC, 0x10),
    /** \brief Event for component ReleaseBuf() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_RELEASEBUF        = EVENTCODE_GEN(MODULE_GENERIC, 0x11),
    /** \brief Event for component internal Freeze() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_FREEZE            = EVENTCODE_GEN(MODULE_GENERIC, 0x12),
    /** \brief Event for component internal Unfreeze() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_UNFREEZE          = EVENTCODE_GEN(MODULE_GENERIC, 0x13),
    /** \brief Event for component DynamicSetupPort() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_DYNAMICSETUPPORT  = EVENTCODE_GEN(MODULE_GENERIC, 0x14),
    /** \brief Event for component RegisterNotifyDetail() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_REGISTERNOTIFYDETAIL    = EVENTCODE_GEN(MODULE_GENERIC, 0x15),
    /** \brief Event for component UnregisterNotifyDetail() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_UNREGISTERNOTIFYDETAIL  = EVENTCODE_GEN(MODULE_GENERIC, 0x16),
    /** \brief Event for component DisconnectPorts() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_CONTROLPORT             = EVENTCODE_GEN(MODULE_GENERIC, 0x17),
    /** \brief Event for component QueryPortConn() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_QUERYPORTCONN         = EVENTCODE_GEN(MODULE_GENERIC, 0x18),
    /** \brief Event for component's low watermark reached message.
     *
     * This event is used by VDEC/ADEC component to notify application that the
     * input buffer reached the low watermark.
     */
    AMP_EVENT_NOTIFY_LOW_WATERMARK  = EVENTCODE_GEN(MODULE_GENERIC, 0x21),
    /** \brief Event for component's high watermark reached message.
     *
     * This event is used by VDEC/ADEC component to notify application that the
     * input buffer reached the high watermark.
     * Notes:
     * . By default component don't detect and generate both of low watermark
     and high watermark notification for input port.
     * . Component will begin detect and generate the watermark event only
     * after the event has been registered by call RegisterNotify() and
     * application called SetParameters(set_[low/high]_watermark).
     * . For low watermark: the application will tell component the low
     * watermark data size by call SetParameters(set_high_watermark).
     * Anytime the un-processed input buffer size changed from above
     * low watermark to below, the component will generate the low
     * watermark event to application. It means the component need moniter
     * the buffer status when comsuming the input data.
     * . For high watermark: the application will tell component the high
     * watermark data size by call SetParameters(set_high_watermark).
     * Anytime the un-processed input buffer size changed from below
     * high watermark to above, the component will generte the high
     * watermark event to applicatoin. It means the component need
     * moniter the buffer status when new date input to port.
     * . normal watermark to tell application that buffer level it to normal level
     * after some operation.
     * . The high watermark should always big that the low water mark, else
     * the SetParameters() should return error code.
     */
    AMP_EVENT_NOTIFY_HIGH_WATERMARK = EVENTCODE_GEN(MODULE_GENERIC, 0x22),
    /** \brief Event for component's normal watermark reached message.
     *
     * This event is used by VDEC/ADEC component to notify application that the
     * input buffer back to normal watermark.
     */
    AMP_EVENT_NOTIFY_NORMAL_WATERMARK = EVENTCODE_GEN(MODULE_GENERIC, 0x23),
    /** \brief Event to notify audio pipeline to enable pass through mode.
     *
     *  This event is used in AMP server audio pipeline only. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_NOTIFY_PASSTHRU       = EVENTCODE_GEN(MODULE_GENERIC, 0x24),
    /** \brief Event for component FlushPortBuf() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_FLUSHPORTBUF      = EVENTCODE_GEN(MODULE_GENERIC, 0x25),
    /** \brief Event for threadmonitor timeout.
     *
     * This event is used by threadmonitor to notify client that the
     * service catch exeption somewhere, need to restart.
     */
    AMP_EVENT_NOTIFY_THREADMONITOR_TIMEOUT = EVENTCODE_GEN(MODULE_GENERIC, 0x30),
    /** \brief Event for threadmonitor heart beat.
     *
     * This event is used by threadmonitor to notify client heart beat in every
     * specified period, the period is 1 second currently
     */
    AMP_EVENT_NOTIFY_THREADMONITOR_HEARTBEAT = EVENTCODE_GEN(MODULE_GENERIC, 0x31),
    /** \brief Event for listener task to exit
     *
     * This event is used by event service to notify it's ready for listener
     * task to exit. It shouldn't be used by application or for other purpose.
     */
    AMP_EVENT_NOTIFY_LISTENER_EXIT = EVENTCODE_GEN(MODULE_GENERIC, 0x32),

    /** \brief Event for power state change
     *
     * This event is used by event service to notify amp module has a power state
     * change. It can be used as power managerment event.
     * Refer to AMP_POWER_STATE for details
     */
    AMP_EVENT_NOTIFY_POWER_STATE_CHANGE = EVENTCODE_GEN(MODULE_GENERIC, 0x33),

    /** \brief The max value for system common events.
     */
    AMP_EVENT_TYPE_MAX              = 0x80,
} AMP_EVENT_TYPE;

/** \def AMP_EVENT_TYPE extension
    \brief event type for component specific
 */
enum {

    /* DUM component */
    /** \brief Event for DUM component ConfigPlayback() API.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_API_DUM_CONFIGPLAYBACK= EVENTCODE_GEN(MODULE_DUM, 1),

    /** \brief Event for DUM component timer.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_TIMER_DUM_PLAYBACK    = EVENTCODE_GEN(MODULE_DUM, 2),

    /* DMX component event type*/
    /** \brief Event for DMX SYNC byte lost. Not used
     */
    AMP_EVENT_DMX_SYNC_LOST         = EVENTCODE_GEN(MODULE_DMX, 1),
    /** \brief Event for DMX SYNC byte lock. Not used
     */
    AMP_EVENT_DMX_SYNC_LOCK         = EVENTCODE_GEN(MODULE_DMX, 2),
    /** \brief Event for scramble state change in TS packet
      * payload: AMP_DMX_EVENT_SCRAMINFO
     */
    AMP_EVENT_DMX_SCRAMBLE_CHANGED  = EVENTCODE_GEN(MODULE_DMX, 3),
    /** \brief Event for PES head in TS packet
      * payload: AMP_DMX_EVENT_PESHEAD
     */
    AMP_EVENT_DMX_PES_HEAD          = EVENTCODE_GEN(MODULE_DMX, 4),
    /** \brief Event for DMX input data size is bigger than high water mark.
      * high watermark is register by AMP_DMX_InputPropSet with
      * AMP_DMX_SRC_PROP_HIGHWATER_MASK
      * payload: AMP_DMX_IOBUFF_STATE
     */
    AMP_EVENT_DMX_INBUFF_OVERFLOW   = EVENTCODE_GEN(MODULE_DMX, 5),
    /** \brief Event for DMX input data size is less than low water mark.
      * low watermark is register by AMP_DMX_InputPropSet with
      * AMP_DMX_SRC_PROP_LOWWATER_MASK
      * payload: AMP_DMX_IOBUFF_STATE
     */
    AMP_EVENT_DMX_INBUFF_UNDERFLOW  = EVENTCODE_GEN(MODULE_DMX, 6),
    /** \brief Event for DMX output data size is more than high water mark.
      * high watermark is register by AMP_DMX_ChannelPropSet with
      * AMP_DMX_CH_PROP_HIGHWATER_MASK
      * payload: AMP_DMX_IOBUFF_STATE
     */
    AMP_EVENT_DMX_OUTBUFF_OVERFLOW  = EVENTCODE_GEN(MODULE_DMX, 7),
    /** \brief Event for DMX output data size is less than low water mark.
      * low watermark is register by AMP_DMX_ChannelPropSet with
      * AMP_DMX_CH_PROP_LOWWATER_MASK
      * payload: AMP_DMX_IOBUFF_STATE
     */
    AMP_EVENT_DMX_OUTBUFF_UNDERFLOW = EVENTCODE_GEN(MODULE_DMX, 8),
    /** \brief Event to notify discontinuity in TS stream.
      * payload: AMP_DMX_EVENT_DISCONTINUITY_COUNT
     */
    AMP_EVENT_DMX_DISCONTINUITY  = EVENTCODE_GEN(MODULE_DMX, 9),
    /** \brief Event to notify PTS in TS stream.
      * payload: AMP_DMX_PTS_OUT_INFO
     */
    AMP_EVENT_DMX_PTSOUT  = EVENTCODE_GEN(MODULE_DMX, 10),
    /** \brief Event for Secure Stop error
     */
    AMP_EVENT_DMX_SECURE_STOP       = EVENTCODE_GEN(MODULE_DMX, 11),
    /** \brief Event for HDCP Compatibility error
     */
    AMP_EVENT_DMX_INSUFFICIENT_HDCP = EVENTCODE_GEN(MODULE_DMX, 12),
    /** \brief Event to notify PCR value to application
     */
    AMP_EVENT_DMX_PCROUT = EVENTCODE_GEN(MODULE_DMX, 13),
    /** \brief Event to notify buffer property change to application
     */
    AMP_EVENT_DMX_OUTBUFF_PROP_CHANGE = EVENTCODE_GEN(MODULE_DMX, 14),
    /** \brief Event for M2M async update
     */
    AMP_EVENT_M2M_ASYNC_UPDATE = EVENTCODE_GEN(MODULE_M2MCRYPTO, 1),

    /** \brief Event for AMP_DMX_AddInput() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_ADD_INPUT     = EVENTCODE_GEN(MODULE_DMX, 32),
    /** \brief Event for AMP_DMX_RmInput() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_RM_INPUT      = EVENTCODE_GEN(MODULE_DMX, 33),
    /** \brief Event for AMP_DMX_ResetInput() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_RESET_INPUT   = EVENTCODE_GEN(MODULE_DMX, 34),
    /** \brief Event for AMP_DMX_InputPropSet() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_SET_INPUT_PROP= EVENTCODE_GEN(MODULE_DMX, 35),
    /** \brief Event for AMP_DMX_InputPropGet() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_GET_INPUT_PROP= EVENTCODE_GEN(MODULE_DMX, 36),
    /** \brief Event for AMP_DMX_AddChannel() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_ADD_CHNL      = EVENTCODE_GEN(MODULE_DMX, 37),
    /** \brief Event for AMP_DMX_RmChannel() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_RM_CHNL       = EVENTCODE_GEN(MODULE_DMX, 38),
    /** \brief Event for AMP_DMX_ChannelPropSet() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_SET_CHNL_PROP = EVENTCODE_GEN(MODULE_DMX, 39),
    /** \brief Event for AMP_DMX_ChannelPropGet() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_GET_CHNL_PROP = EVENTCODE_GEN(MODULE_DMX, 40),
    /** \brief Event for AMP_DMX_ChannelControl() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_CTRL_CHNL     = EVENTCODE_GEN(MODULE_DMX, 41),
    /** \brief Event for AMP_DMX_AddSectionFilter() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_ADD_SECFLT    = EVENTCODE_GEN(MODULE_DMX, 42),
    /** \brief Event for AMP_DMX_RmSectionFilter() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_RM_SECFLT     = EVENTCODE_GEN(MODULE_DMX, 43),
    /** \brief Event for AMP_DMX_SectionFilterPropSet() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_SET_SECFLT_PROP= EVENTCODE_GEN(MODULE_DMX, 44),
    /** \brief Event for AMP_DMX_SectionFilterPropGet() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_GET_SECFLT_PROP= EVENTCODE_GEN(MODULE_DMX, 45),
    /** \brief Event for AMP_DMX_SectionFilterControl() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_CTRL_SECFLT   = EVENTCODE_GEN(MODULE_DMX, 46),
    /** \brief Event for AMP_DMX_ConfigPushMode() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_CONFIG_PUSHMODE = EVENTCODE_GEN(MODULE_DMX, 47),

    /** \brief Event for AMP_DMX_AddPrimaryPID() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_ADD_PRIMARY_PID      = EVENTCODE_GEN(MODULE_DMX, 48),
    /** \brief Event for AMP_DMX_RmPrimaryPID() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_RM_PRIMARY_PID       = EVENTCODE_GEN(MODULE_DMX, 49),
    /** \brief Event for AMP_DMX_SetSecondaryPID() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_SET_SECONDARY_PID     = EVENTCODE_GEN(MODULE_DMX, 50),
    /** \brief Event for AMP_DMX_RmSecondaryPID() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_RM_SECONDARY_PID       = EVENTCODE_GEN(MODULE_DMX, 51),

    /** \brief Event for AMP_DMX_AddDSSPktFilter() API.
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_ADD_DSSFLT   = EVENTCODE_GEN(MODULE_DMX, 52),
    /** \brief Event for AMP_DMX_RmDSSPktFilter() API.
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_RM_DSSFLT    = EVENTCODE_GEN(MODULE_DMX, 53),
    /** \brief Event for AMP_DMX_StartDSSPktFilter() API.
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_START_DSSFLT = EVENTCODE_GEN(MODULE_DMX, 54),
    /** \brief Event for AMP_DMX_StopDSSPktFilter() API.
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_STOP_DSSFLT  = EVENTCODE_GEN(MODULE_DMX, 55),
    /** \brief Event for AMP_DMX_AddKeyToken() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_ADD_KEYTOKEN      = EVENTCODE_GEN(MODULE_DMX, 56),
    /** \brief Event for AMP_DMX_RemoveKeyToken() API.
     *
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_API_DMX_RM_KEYTOKEN       = EVENTCODE_GEN(MODULE_DMX, 57),

    /* ADEC component event type*/
    /** \brief event for adec to set decoder mode, not in use. */
    AMP_EVENT_API_ADEC_SETDECMODE       = EVENTCODE_GEN(MODULE_ADEC, 1),
    /** \brief event for adec to change format, not in use. */
    AMP_EVENT_API_ADEC_FMTCHANGE        = EVENTCODE_GEN(MODULE_ADEC, 2),
    /** \brief event for notify adec hw processor decoder done.
    * It shouldn't used by application */
    AMP_EVENT_ADEC_PROCDONE             = EVENTCODE_GEN(MODULE_ADEC, 3),
    /** \brief event for notify adec hw processor decoder done in LLD layer.
    * It shouldn't used by application */
    AMP_EVENT_ADEC_LLD_DONE             = EVENTCODE_GEN(MODULE_ADEC, 4),
    /** \brief event for notify callback the adec stream information.
      * payload: AMP_ADEC_STRMINFO_EVENT
    */
    AMP_EVENT_ADEC_CALLBACK_STRMINFO    = EVENTCODE_GEN(MODULE_ADEC, 5),

    /** \brief event for notify callback the adec decoder wrong.
      * payload: AMP_ADEC_WRGCODEC_EVENT
    */
    AMP_EVENT_ADEC_WRONG_CODEC          = EVENTCODE_GEN(MODULE_ADEC, 6),

    /** \brief event for notify callback the stream format if unknown.
      * payload: AMP_ADEC_FMTNOTIFY_EVENT
    */
    AMP_EVENT_ADEC_FMT_NOTIFY           = EVENTCODE_GEN(MODULE_ADEC, 7),

    /* AENC component event type*/
    /** \brief event for notify aenc hw processor encoder done.
    * It shouldn't used by application */
    AMP_EVENT_AENC_PROC_DONE        = EVENTCODE_GEN(MODULE_AENC, 1),

    /** \brief event for set aenc process parameters.
     *  This event is used in AMP server for event driven, and shouldn't used by application.
     */
    AMP_EVENT_AENC_SET_PARAM        = EVENTCODE_GEN(MODULE_AENC, 2),

    /** \brief event for get aenc process parameters.
    *  This event is used in AMP server for event driven, and shouldn't used by application.
    */
    AMP_EVENT_AENC_GET_PARAM        = EVENTCODE_GEN(MODULE_AENC, 3),

    /* APP component event type */
    /** \brief event for notify app hw processor process done.
    * It shouldn't used by application */
    AMP_EVENT_APP_PROC_DONE         = EVENTCODE_GEN(MODULE_APP, 1),

    /** \brief event for notify app hw processor suspend done.
    * It shouldn't used by application */
    AMP_EVENT_APP_SUSPEND_DONE      = EVENTCODE_GEN(MODULE_APP, 2),

    /** \brief event for notify app hw processor resume done.
    * It shouldn't used by application */
    AMP_EVENT_APP_RESUME_DONE       = EVENTCODE_GEN(MODULE_APP, 3),

    /** \brief event for set app process parameters.
    *  This event is used in AMP server for event driven, and shouldn't used by application.
    */
    AMP_EVENT_APP_SET_PARAM         = EVENTCODE_GEN(MODULE_APP, 4),

    /** \brief event for get app process parameters.
    *  This event is used in AMP server for event driven, and shouldn't used by application.
    */
    AMP_EVENT_APP_GET_PARAM         = EVENTCODE_GEN(MODULE_APP, 5),

    /** \brief app event to notify HDMI hot plug. */
    AMP_EVENT_APP_HDMI_HOTPLUG     = EVENTCODE_GEN(MODULE_APP, 6),

    /** \brief app event for ms12 reopen. */
    AMP_EVENT_APP_REOPEN            = EVENTCODE_GEN(MODULE_APP, 7),

    /** \brief app schedule event for process trigger. */
    AMP_EVENT_APP_SCHEDULE          = EVENTCODE_GEN(MODULE_APP, 8),

    /** \brief app query port state. */
    AMP_EVENT_APP_QUERY_PORTSTAT    = EVENTCODE_GEN(MODULE_APP, 9),

    /* AREN component event type */
    /** \brief AREN notification callback. */
    AMP_EVENT_API_AREN_CALLBACK     = EVENTCODE_GEN(MODULE_AREN, 0),

    /** \brief delay event from app to update delay value.
    * It shouldn't used by application.*/
    AMP_EVENT_AREN_DELAY            = EVENTCODE_GEN(MODULE_AREN, 1),

    /* SNDSRV component event type */
    /** \brief avsync event for sndsrv notify callback. not in use.
     */
    AMP_EVENT_API_SND_ASYNC         = EVENTCODE_GEN(MODULE_SNDSRV, 0),

    /** \brief clear bt path pts.
     */
    AMP_EVENT_SNDEXP_CLEAR_PTS = EVENTCODE_GEN(MODULE_SNDSRV, 1),

    /** \brief sync bt path pts.
     */
    AMP_EVENT_SNDEXP_SYNC_PTS = EVENTCODE_GEN(MODULE_SNDSRV, 2),

    /** \brief standby bt socket.
    */
    AMP_EVENT_SNDEXP_BT_STANDBY = EVENTCODE_GEN(MODULE_SNDSRV, 3),

    /** \brief resume bt socket.
    */
    AMP_EVENT_SNDEXP_BT_RESUME = EVENTCODE_GEN(MODULE_SNDSRV, 4),

    /* AOUT component event type */
    /** \brief event for aout set HDMI configure.
    * It shouldn't used by application.*/
    AMP_EVENT_AOUT_SET_HDMI_CONFIG  = EVENTCODE_GEN(MODULE_AOUT, 1),

    /** \brief event for aout under run. not in use */
    AMP_EVENT_API_AOUT_UNRUN        = EVENTCODE_GEN(MODULE_AOUT, 2),

    /** \brief event for aout reset AVPLL if needed.
    *  This event is used in AMP server for event driven. it shouldn't be used
    *  by application or for other purpose.
    */
    AMP_EVENT_AOUT_RESETAVPLL   = EVENTCODE_GEN(MODULE_AOUT, 3),

    /** \brief event for aout set volume.
     *  This event is used in AMP server audio pipeline only.
     *  it shouldn't be used by application or for other purpose*/
    AMP_EVENT_AOUT_SET_VOLUME   = EVENTCODE_GEN(MODULE_AOUT, 4),

    /** \brief event for aout set path volume.
    *  This event is used in AMP server audio pipeline only.
    *  it shouldn't be used by application or for other purpose */
    AMP_EVENT_AOUT_SET_STREAM_VOLUME = EVENTCODE_GEN(MODULE_AOUT, 5),

    /** \brief event for aout set stream PTS invalid.
    *  This event is used in AMP server audio pipeline only.
    *  it shouldn't be used by application or for other purpose*/
    AMP_EVENT_AOUT_INVALID_PTS   = EVENTCODE_GEN(MODULE_AOUT, 6),

    /** \brief event for aout clear data event type
    *  This event is used in AMP server audio pipeline only.
    *  it shouldn't be used by application or for other purpose*/
    AMP_EVENT_AOUT_CLEAR = EVENTCODE_GEN(MODULE_AOUT, 7),

    /** \brief event for aout info update acknowledge event type
    *  This event is used in AMP server audio pipeline only.
    *  it shouldn't be used by application or for other purpose*/
    AMP_EVENT_AOUT_INFOUPDATE_ACK = EVENTCODE_GEN(MODULE_AOUT, 8),

    /** \brief event for aout stream dump to data control
    *  This event is used in AMP server audio pipeline only.
    *  it shouldn't be used by application or for other purpose*/
    AMP_EVENT_AOUT_DUMP_DATA = EVENTCODE_GEN(MODULE_AOUT, 9),

    /** \brief event for aout to set port passthrough from sndsrv
    *  This event is used in AMP server audio pipeline only.
    *  it shouldn't be used by application or for other purpose*/
    AMP_EVENT_AOUT_SND_PASSTHROUGH = EVENTCODE_GEN(MODULE_AOUT, 10),
    /** \brief event for aout to set AIO mute status */
    AMP_EVENT_AOUT_SET_AIO_MUTE = EVENTCODE_GEN(MODULE_AOUT, 11),
    /** \breif event for aout to get AIO mute status */
    AMP_EVENT_AOUT_GET_AIO_MUTE = EVENTCODE_GEN(MODULE_AOUT, 12),
    /** \brief event for aout clear stream position */
    AMP_EVENT_AOUT_CLEAR_STREAM_POSITION = EVENTCODE_GEN(MODULE_AOUT, 13),

    AMP_EVENT_AOUT_SYNC_STREAM_POSITION = EVENTCODE_GEN(MODULE_AOUT, 14),

    /** \brief event to set stream pause state on AOUT side */
    AMP_EVENT_AOUT_SET_STREAM_PAUSE_STATE = EVENTCODE_GEN(MODULE_AOUT, 15),
    /** \brief event to inform AOUT about HDMI hot plug info */
    AMP_EVENT_AOUT_HDMI_HOTPLUG = EVENTCODE_GEN(MODULE_AOUT, 16),
    /** \brief event to inform AOUT about multi path delay sample number */
    AMP_EVENT_AOUT_MULTI_SET_DELAY = EVENTCODE_GEN(MODULE_AOUT, 17),

    /* AVIN component event type */
    /** \brief It indicates that an input source is connected or disconnected.
      * payload: AMP_AVIN_CONNECT_STATUS_CHANGED_INFO
    */
    AMP_EVENT_API_AVIN_SRC_CONNECTION_CHANGED   = EVENTCODE_GEN(MODULE_AVIN, 0x01),
    /** \brief It indicates that the current lock status
    *  (DISCONNECTED/NO SIGNAL/NOT SUPPORTED/STABLE)
    *  of current selected input source changes.
    * payload: AMP_AVIN_LOCK_STATUS_CHANGED_INFO
    */
    AMP_EVENT_API_AVIN_LOCK_STATUS_CHANGED      = EVENTCODE_GEN(MODULE_AVIN, 0x02),
    /** \brief It indicates that the video parameters of current
    *  selected input source changes.
    * payload: AMP_AVIN_VIDEO_PARAMS_CHANGED_INFO
    */
    AMP_EVENT_API_AVIN_VIDEO_CHANGED            = EVENTCODE_GEN(MODULE_AVIN, 0x03),
    /** \brief  It indicates that the audio parameters of current
    *  selected input source changes.
    * payload: AMP_AVIN_AUDIO_PARAMS_CHANGED_INFO
    */
    AMP_EVENT_API_AVIN_AUDIO_CHANGED            = EVENTCODE_GEN(MODULE_AVIN, 0x04),
    /** \brief It indicates that the AV mute status (TRUE/FALSE)
    *  of current selected input source changes.
    * payload: AMP_AVIN_AVMUTE_CHANGED_INFO
    */
    AMP_EVENT_API_AVIN_AVMUTE_CHANGED           = EVENTCODE_GEN(MODULE_AVIN, 0x05),
    /** \brief It indicates that a GMD pkt comes from AVIN component
      * (enabled on HDMI Rx).
      * payload: AMP_AVIN_HDMI_PKT_GMD
      */
    AMP_EVENT_API_AVIN_HDMI_GMD_PKT             = EVENTCODE_GEN(MODULE_AVIN, 0x06),
    /** \brief It indicates that AVIN component stops sending GMD pkt
      * (enabled on HDMI Rx).
      * payload: AMP_AVIN_HDMI_PKT_GMD
      */
    AMP_EVENT_API_AVIN_HDMI_GMD_PKT_STOP        = EVENTCODE_GEN(MODULE_AVIN, 0x07),
    /** \brief It indicates that SD INT ON by VIP driver,
    *  and then 3D comb can be enabled. (NOT USED) */
    AMP_EVENT_API_AVIN_VIP_SD_INT_ON            = EVENTCODE_GEN(MODULE_AVIN, 0x08),
    /** \brief It indicates that a time consuming job has been completed.
      * payload: AMP_AVIN_COMPLETED_JOB_INFO
      */
    AMP_EVENT_API_AVIN_JOB_COMPLETED            = EVENTCODE_GEN(MODULE_AVIN, 0x09),
    /** \brief It indicates that the SIF audio status is changed.
      * payload: AMP_AVIN_SIF_STATUS
      */
    AMP_EVENT_API_AVIN_SIF_STATUS_CHANGED = EVENTCODE_GEN(MODULE_AVIN, 0x0A),

    /** \brief It indicates that HDR info frame comes from AVIN component
      * (enabled on HDMI Rx).
      * payload: AMP_AVIN_HDMI_PKT_HDR_INFOFRM
      */
    AMP_EVENT_API_AVIN_HDMI_HDR_INFO             = EVENTCODE_GEN(MODULE_AVIN, 0x0B),

     /** \brief It indicates that DV info change comes from AVIN component
      * (enabled on HDMI Rx).
      * payload: AMP_AVIN_HDMI_DV_INFO
      */
    AMP_EVENT_API_AVIN_HDMI_DV_CHANGED             = EVENTCODE_GEN(MODULE_AVIN, 0x0C),

    /* vdec component event type*/
    /** \brief VDEC notification callback.
      * callback is for: AMP_VDEC_EVENT_RES_CHANGE
      *                  AMP_VDEC_EVENT_FR_CHANGE
      *                  AMP_VDEC_EVENT_AR_CHANGE
      *                  AMP_VDEC_EVENT_1ST_I_DECODED
      *                  AMP_VDEC_EVENT_DECODE_DONE
      *                  AMP_VDEC_EVENT_ONE_FRAME_DECODED
      *                  AMP_VDEC_EVENT_DISP_CROP_CHANGE
      *                  AMP_VDEC_EVENT_SEQ_CHANGE
      *                  AMP_VDEC_EVENT_SCAN_TYPE_CHANGE
      *                  AMP_VDEC_EVENT_DECODER_FATAL
    */
    AMP_EVENT_API_VDEC_CALLBACK     = EVENTCODE_GEN(MODULE_VDEC, 0),

    /** \brief VDEC need generate the UNDERFLOW event to application to
      * let application knows the video input data is underflow.
      */
    AMP_EVENT_API_VIDEO_UNDERFLOW   = EVENTCODE_GEN(MODULE_VDEC, 1),
    /**
      * 3D mode changed\n
      * payload: structure FP_INFO. \n
      */
    AMP_VDEC_EVENT_3D_MODE_CHANGE = EVENTCODE_GEN(MODULE_VDEC, 2),

    /* display component event type*/
    /** \brief notification of VSYNC from DISP to application.
      */
    AMP_EVENT_API_DISP_CALLBACK_VSYNC   = EVENTCODE_GEN(MODULE_DISPSRV, 0),
    /** \brief notification of HDMI connection to application.
      * pPayLoad[0] is AMP_DISP_EVENT_HDMI_SINK_CONNECTED means hdmi connected
      * event from display service; else means
      * hdmi disconnected event from display service
      */
    AMP_EVENT_API_DISP_CALLBACK_HDMI    = EVENTCODE_GEN(MODULE_DISPSRV, 1),
    /** \brief notification of CEC event to application
      * payload: AMP_DISP_CEC_EVENT_INFO
      */
    AMP_EVENT_API_DISP_CALLBACK_CEC     = EVENTCODE_GEN(MODULE_DISPSRV, 2),
    /** \brief notification of AUX capture
      * payload: AMP_DISP_VPP_AUX_FRAME_INFO
      */
    AMP_EVENT_API_DISP_CALLBACK_AUXCAPTURE = EVENTCODE_GEN(MODULE_DISPSRV, 3),
    /** \brief notification of resolution change start
      * payload: AMP_DISP_RES_CHANGE_EVENT_INFO
      */
    AMP_EVENT_API_DISP_CALLBACK_RES_CHANGE_START = EVENTCODE_GEN(MODULE_DISPSRV, 4),
    /** \brief notification of resolution change done
      * payload: AMP_DISP_RES_CHANGE_EVENT_INFO
      */
    AMP_EVENT_API_DISP_CALLBACK_RES_CHANGE_DONE  = EVENTCODE_GEN(MODULE_DISPSRV, 5),
    /** \brief notification of HDCP state
      * payload: AMP_DISP_HDCP_ERR_EVENT
      */
    AMP_EVENT_API_DISP_CALLBACK_HDCP    = EVENTCODE_GEN(MODULE_DISPSRV, 6),

    /** \brief notification of eARC state
      * payload: AMP_DISP_EARC_ERR_EVENT
      */
    AMP_EVENT_API_DISP_CALLBACK_EARC    = EVENTCODE_GEN(MODULE_DISPSRV, 7),

    /* clock component event type*/

    /** \brief Event indicates STC is going to be set. */
    AMP_EVENT_CLK_STARTSTC_BEFORE   = EVENTCODE_GEN(MODULE_CLK, 12),

    /** \brief Event indicates STC has been set. */
    AMP_EVENT_CLK_STARTSTC_AFTER    = EVENTCODE_GEN(MODULE_CLK, 13),

    /** \brief Event indicates clock rate is going to be set. */
    AMP_EVENT_CLK_SETCLOCKRATE_BEFORE = EVENTCODE_GEN(MODULE_CLK, 14),

    /** \brief Event indicates clock rate has been set. */
    AMP_EVENT_CLK_SETCLOCKRATE_AFTER  = EVENTCODE_GEN(MODULE_CLK, 15),

    /** \brief Event indicates a video frame has been released to display. */
    AMP_EVENT_CLK_VIDEO_FRAME_DISPLAY = EVENTCODE_GEN(MODULE_CLK, 16),

    /** \brief Event indicates STC is ready for use. */
    AMP_EVENT_CLK_STC_READY = EVENTCODE_GEN(MODULE_CLK, 17),

    /** \brief Event indicates the first video frame has been released to display. */
    AMP_EVENT_CLK_VIDEO_FIRST_FRAME_DISPLAY = EVENTCODE_GEN(MODULE_CLK, 18),

    /** \brief Event indicates the dynamic sync has started */
    AMP_EVENT_CLK_DYNAMIC_SYNC_START = EVENTCODE_GEN(MODULE_CLK, 19),

    /** \brief Event indicates a synchronized playback is now possible by disabling dynamic sync. */
    AMP_EVENT_CLK_DYNAMIC_SYNC_RESYNC_POSSIBLE = EVENTCODE_GEN(MODULE_CLK, 20),

    /** \brief Event indicates the playback is AV synchronized and dynamic sync is finished. */
    AMP_EVENT_CLK_DYNAMIC_SYNC_REACHED = EVENTCODE_GEN(MODULE_CLK, 21),

    /** /brief Event indicates playback is paused by the specified pause PTS. */
    AMP_EVENT_CLK_PAUSE_PTS_REACHED = EVENTCODE_GEN(MODULE_CLK, 22),

    /* vout component event type*/
    /** \brief event from VOUT to indicate end of stream
    */
    AMP_EVENT_API_VOUT_CALLBACK  = EVENTCODE_GEN(MODULE_VOUT, 0),
    /** \brief event from VOUT to indicate frame updated
      * payload: AMP_VOUT_DISP_FRAME_INFO
    */
    AMP_EVENT_API_VOUT_CALLBACK_FRAME_UPDATE        = EVENTCODE_GEN(MODULE_VOUT, 1),

    /** \brief Event for VOUT component CaptureScreen() API.
     *
     *  This event is used to notify application
     *  that the latest video frame is captured
     *  and copied to the buffer pre-defined by application.
     */
    AMP_EVENT_API_VOUT_CALLBACK_CAP_SCREEN   = EVENTCODE_GEN(MODULE_VOUT, 2),

    /** \brief vout component event type
     *  This event is used to notify application
     *  that the first displayed video frame.
     * playload: AMP_VOUT_DISP_FRAME_INFO
     */
    AMP_EVENT_API_VOUT_CALLBACK_FIRST_FRAME  = EVENTCODE_GEN(MODULE_VOUT, 3),

    /** \brief vout component event to copy and keep last frame
     *  This event is for vout internal use only
     */
    AMP_EVENT_API_VOUT_COPY_LAST_FRAME  = EVENTCODE_GEN(MODULE_VOUT, 4),

    /** \brief vout component event to indicate vsync signal comes
     *  payload: none
     */
    AMP_EVENT_API_VOUT_CALLBACK_VSYNC = EVENTCODE_GEN(MODULE_VOUT, 5),

    /** \brief vout component event to indicate that the analysis has been completed
     *  payload: BD
     */
    AMP_EVENT_API_VOUT_CALLBACK_ANALYSIS_COMPLETE = EVENTCODE_GEN(MODULE_VOUT, 6),

    /* venc componet event type */
    /** \brief frame done event for VENC notify callback.
     *
     *  This event is used to notify DDL_VEnc (by DDL_VEnc callback)
     *  that one YUV frame is ready to be returned.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_VENC_CHECK_FRAME_DONE  = EVENTCODE_GEN(MODULE_VENC, 0),

    /* venc componet event type */
    /** \brief stream done event for VENC notify callback.
     *
     *  This event is used to notify DDL_VEnc (by DDL_VEnc callback)
     *  that one ES stream is ready to be returned.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_VENC_CHECK_STREAM_DONE = EVENTCODE_GEN(MODULE_VENC, 1),

    /* venc componet event type */
    /** \brief start of stream event for VENC notify callback.
     *
     *  This event is used to notify DDL_VEnc (by DDL_VEnc callback)
     *  that the stream is started.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_VENC_START_OF_STREAM   = EVENTCODE_GEN(MODULE_VENC, 2),

    /* venc componet event type */
    /** \brief end of stream event for VENC notify callback.
     *
     *  This event is used to notify DDL_VEnc (by DDL_VEnc callback)
     *  that the stream is end.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_VENC_END_OF_STREAM     = EVENTCODE_GEN(MODULE_VENC, 3),

    /* venc componet event type */
    /** \brief header data update event for VENC notify callback.
     *
     *  This event is used to notify DDL_VEnc (by DDL_VEnc callback)
     *  that the header data is updated.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_VENC_HEADER_DATA_UPDATE     = EVENTCODE_GEN(MODULE_VENC, 4),

    /* venc componet event type */
    /** \brief downscale done event for VENC notify callback.
     *
     *  This event is used to notify DDL_VEnc (by DDL_VEnc callback)
     *  that the header data is updated.
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_VENC_DS_YUV_DONE     = EVENTCODE_GEN(MODULE_VENC, 5),

    /* vcap component event type*/
    /** \brief event from VCAP
    */
    AMP_EVENT_API_VCAP_CALLBACK  = EVENTCODE_GEN(MODULE_VCAP, 0),

    /* sr component event type*/
    /** \brief event from SR
    */
    AMP_EVENT_API_SR_CALLBACK  = EVENTCODE_GEN(MODULE_SR, 0),

    /* data exporter componet event type */
    /** \brief raise when recognizing first unit or receive unrecognized the data.
     *
     *  Following events belong to this event type, amp_data_exporter.idl show
     *  the detail.
     *  EVENT_ERROR_UNKNOWN_DATA
     *  EVENT_FIRST_TELETEXT
     *  EVENT_FIRST_SUBTITLE
     *
     *  This event is used in AMP server for event driven. it shouldn't be used
     *  by application or for other purpose.
     */
    AMP_EVENT_DATA_EXPORTER_RESULT_OF_RECOGNIZE = EVENTCODE_GEN(MODULE_DATA_EXPORTER, 0),
};

/** \func EVENTCODE2TYPE
 *  \brief get event type base from event code.
 */
inline static UINT32 EVENTCODE2TYPE(AMP_EVENT_CODE code)
{return (code & 0xff);}

/** \func EVENTCODE_ISVALID
 *  \brief to check event code is valid or not.
 */
inline static UINT32 EVENTCODE_ISVALID(AMP_EVENT_CODE code)
{
    return (code && EVENTCODE2TYPE(code) < AMP_EVENT_TYPE_MAX);
}

#define AMP_EVENT_MAX_PARAMS1
#define _AMP_EVENT_PAYLOAD_OFFSET   (16)
#ifndef offset_of
#define offset_of(mem, type)        (type __resvd = (type*)0;__resvd->mem-0;)
#endif

typedef struct {
    AMP_EVENT_CODE  eEventCode;
    UINT32          uiParam1;
    UINT32          uiParam2;
    UINT32          uiReserverd;

} AMP_EVENT_HEAD;

#define AMP_EVENT_BODY_MAX          \
        ((MV_CC_MSGQ_DATA_MSGEXLEN_MAX - sizeof(AMP_EVENT_HEAD))/sizeof(UINT32))

/** \def AMP_EVENT_PAYLOAD_SIZE
    \brief The macro define the max size for event payload
 */
#define AMP_EVENT_PAYLOAD_SIZE      \
        (AMP_EVENT_BODY_MAX*sizeof(UINT32)-_AMP_EVENT_PAYLOAD_OFFSET)

#define AMP_EVENT_MAX_PARAMS        AMP_EVENT_PAYLOAD_SIZE/sizeof(UINT32)

typedef struct {
    UINT32          uiEventBodyArray[AMP_EVENT_BODY_MAX];
} AMP_EVENT_BODY;

typedef struct {
    AMP_EVENT_HEAD  stEventHead;
    AMP_EVENT_BODY  stEventBody;
} AMP_EVENT;

#define EVENT_BODY_OFFSET           sizeof(AMP_EVENT_HEAD)
#define EVENT_C2HEAD(event)         ((AMP_EVENT_HEAD*)(event))
#define EVENT_C2BODY(event)         \
            ((AMP_EVENT_BODY*)((char *)event+EVENT_BODY_OFFSET))
#define EVENT_C2PAY(event)          \
    ((UINT*)((char *)event+EVENT_BODY_OFFSET+_AMP_EVENT_PAYLOAD_OFFSET))

/** \def AMP_EVENT_GETTYPE this macro is deprecated to use outside later
    \brief The macro is to help get event code from event struct
    \param event - a pointer to event struct.
 */
#define AMP_EVENT_GETTYPE(e)        ((e).stEventHead.eEventCode)
/** \def AMP_EVENT_GETCODE
    \brief The macro is to help get event code from event struct
    \param event - a pointer to event struct
 */
#define AMP_EVENT_GETCODE(e)        ((e).stEventHead.eEventCode)
/** \def AMP_EVENT_GETPAR1
    \brief The macro is to help get event param1 from event struct
    \param event - a pointer to event struct
 */
#define AMP_EVENT_GETPAR1(e)        ((e).stEventHead.uiParam1)
/** \def AMP_EVENT_GETPAR2
    \brief The macro is to help get event param2 from event struct
    \param event - a pointer to event struct
 */
#define AMP_EVENT_GETPAR2(e)        ((e).stEventHead.uiParam2)
/** \def AMP_EVENT_ISVALID
    \brief The macro is to validate event struct
    \param event - a pointer to event struct
 */
#define AMP_EVENT_ISVALID(e)        \
            (EVENTCODE_ISVALID((e).stEventHead.eEventCode))

/** \def AMP_EVENT_HEAD_INIT
    \brief The macro is to help initialize event header part
    \param event - a pointer to event struct
    \param code - event code
    \param p1,p2 - two params attached
 */
#define AMP_EVENT_HEAD_INIT(event, code, p1, p2)        \
            do {                                        \
                EVENT_C2HEAD(event)->eEventCode = code; \
                EVENT_C2HEAD(event)->uiParam1 = p1;     \
                EVENT_C2HEAD(event)->uiParam2 = p2;     \
                memset(EVENT_C2BODY(event), 0, _AMP_EVENT_PAYLOAD_OFFSET);\
                } while(0)

/** \def AMP_EVENT_HEAD_INIT
    \brief The macro is to help initialize event payload part
    \param pEvent - An pointer to event struct
    \param pPayload - An pointer to payload data
    \param uiSize - Size of payload data, this size should be less than
           AMP_EVENT_PAYLOAD_SIZE, more than that is cut off.
 */
inline static UINT32 AMP_EVENT_PAYLOAD_INIT(AMP_EVENT *pEvent,
                                        UINT8  *pPayload, UINT32 uiSize)
{
    VOID *payload = EVENT_C2PAY(pEvent);
    UINT32  len = MIN(uiSize, AMP_EVENT_PAYLOAD_SIZE);
    if (pPayload) {
        AmpMemcpy(payload, pPayload, len); return len;
    }
    return 0;
}

/** \def AMP_EVENT_PAYLOAD_PTR
    \brief The macro is to get payload address of the specified event,
           use this address to assign value for payload
    \param pEvent - An pointer to event struct
 */
inline static VOID* AMP_EVENT_PAYLOAD_PTR(AMP_EVENT *pEvent)
{
    VOID *pPayload = EVENT_C2PAY(pEvent);
    return pPayload;
}

/** \func
    \brief The function create a event queue
    \param HANDLE *phEventQ - A pointer to handle of event queue created in
           this function
    \param sName - event queue name in string
    \param QueueSize - event queue size in byte
    \param Flags - event queue option
 */
HRESULT AMP_EventQ_Create(AMP_OUT HANDLE *phEventQ,
                          AMP_IN CHAR *sName,
                          AMP_IN UINT QueueSize,
                          AMP_IN UINT Flags);

/** \func AMP_EventQ_Destroy
    \brief The function create a event queue
    \param phEventQ - A handle of event queue to destroy, and the handle
           will be set a NULL
 */
HRESULT AMP_EventQ_Destroy(AMP_INOUT HANDLE *phEventQ);

/** \func
    \brief The function send an event to the event queue
    \param hEventQ - A handle of event queue
    \param pEvent - An pointer to event struct
    \param bSync - TRUE for blocking call that requires event receiver
            acknowledge, FALSE for nonblocking call
 */
HRESULT AMP_EventQ_SendEvent(AMP_IN HANDLE hEventQ,
                             AMP_IN AMP_EVENT *pEvent,
                             AMP_IN BOOL bSync);

/** \func
    \brief The function receive an event from the event queue, wait on the
           event queue if no new event occurs.
    \param hEventQ - A handle of event queue
    \param pEvent - An pointer to event struct received
 */
HRESULT AMP_EventQ_ReadEvent(AMP_IN HANDLE hEventQ, AMP_IN AMP_EVENT *pEvent);

/** \func
    \brief The function receive an event from the event queue, if not event in
           queue, it will return immediately.
    \param hEventQ - A handle of event queue
    \param pEvent - An pointer to event struct received
 */
HRESULT AMP_EventQ_TryReadEvent(AMP_IN HANDLE hEventQ, AMP_IN AMP_EVENT *pEvent);

/** \func
    \brief The function receive an event from the event queue, wait on the
           event queue, until timedout. when timedout, return E_TIMEOUT
    \param hEventQ - A handle of event queue
    \param pEvent - An pointer to event struct received
    \param uiTimedOutMS - wait timedout value
 */
HRESULT AMP_EventQ_TimedReadEvent(AMP_IN HANDLE hEventQ,
                                  AMP_IN AMP_EVENT *pEvent,
                                  AMP_IN UINT32 uiTimedOutMS);

/** \func
    \brief The function acknowledge an event arrival from sender if the
           sender call send event with sync option, no effect for async option.
    \param hEventQ - A handle of event queue
    \param pEvent - An pointer to event struct received
 */
HRESULT AMP_EventQ_AcknowledgeEvent(AMP_IN HANDLE hEventQ,
                                    AMP_IN AMP_EVENT *pEvent);

/** \func
    \brief The function is used to get the event string.
           It's mainly for debug purpose.
    \param eEventCode - An event code.
 */
const CHAR *AMP_EventQ_GetEventString(AMP_IN AMP_EVENT_CODE eEventCode);

#define EVLOG(_lvl, pMsg, ...)    \
    AMPLOG(MODULE_EVENT, _lvl, LOG_TAG " " pMsg, ##__VA_ARGS__)
#define EVLOGV(pMsg, ...)         \
    EVLOG(AMP_LOG_INFO, "DBG " pMsg, ##__VA_ARGS__)
#define EVLOGD(pMsg, ...)         \
    EVLOG(AMP_LOG_LOW, "DBG " pMsg, ##__VA_ARGS__)
#define EVLOGI(pMsg, ...)         \
    EVLOG(AMP_LOG_MID, "INF " pMsg, ##__VA_ARGS__)
#define EVLOGW(pMsg, ...)         \
    EVLOG(AMP_LOG_HIGH, "WRN " pMsg, ##__VA_ARGS__)
#define EVLOGE(pMsg, ...)         \
    EVLOG(AMP_LOG_ERROR, pMsg, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* __AMP_EVENT_QUEUE_H__ */
