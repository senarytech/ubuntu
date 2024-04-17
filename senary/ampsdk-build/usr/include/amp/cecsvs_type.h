/*******************************************************************************
*                Copyright 2012, MARVELL SEMICONDUCTOR, LTD.                   *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL.                      *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
*                                                                              *
* MARVELL COMPRISES MARVELL TECHNOLOGY GROUP LTD. (MTGL) AND ITS SUBSIDIARIES, *
* MARVELL INTERNATIONAL LTD. (MIL), MARVELL TECHNOLOGY, INC. (MTI), MARVELL    *
* SEMICONDUCTOR, INC. (MSI), MARVELL ASIA PTE LTD. (MAPL), MARVELL JAPAN K.K.  *
* (MJKK), MARVELL ISRAEL LTD. (MSIL).                                          *
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//! \file cecsvs_type.h
//!
//! \Brief Description: This file defines macros which are common to Box and TV CEC SErvice .
//!
//! Purpose:
//!
//!
//!     Version          Date                   Author
//!  V 0.00,      April 29. 2013,               Nishchal: Added for AMP
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef _CEC_SVS_TYPE_H_
#define _CEC_SVS_TYPE_H_

#define MAX_USB_DEV 5

#define CECSVS_STATUS_SUCCESS                                0
#define CECSVS_STATUS_ERR_INVALID_PARAM                      1
#define CECSVS_STATUS_ERR_INVALID_MODE                       2
#define CECSVS_STATUS_ERR_MSG_UNSUPPORTED                    3
#define CECSVS_STATUS_ERR_INVALID_FRAME_LEN                  4
#define CECSVS_STATUS_ERR_MSG_IGNORED                        5
#define CECSVS_STATUS_TX_ERR_COMM_FAIL                       6
#define CECSVS_STATUS_RX_ERR_COMM_FAIL                       7
#define CECSVS_STATUS_TX_ERR_WAIT_FALL                       8
#define  CECSVS_STATUS_TX_ERR_RETRY_FAIL                     9

/*NISHCHAL:: Not in AMP framework*/
#define     CECSVS_FEATURE_ONE_TOUCH_PLAY              0x00000001
#define     CECSVS_FEATURE_ROUTING_CONTROL             0x00000002
#define     CECSVS_FEATURE_SYSTEM_STANDBY              0x00000004
#define     CECSVS_FEATURE_ONE_TOUCH_RECORD            0x00000008
#define     CECSVS_FEATURE_SYSTEM_INFO                 0x00000010
#define     CECSVS_FEATURE_DECK_CONTROL                0x00000020
#define     CECSVS_FEATURE_TUNER_CONTROL               0x00000040
#define     CECSVS_FEATURE_VENDOR_SPEC_CMDS            0x00000080
#define     CECSVS_FEATURE_OSD_STATUS_DISPLAY          0x00000100
#define     CECSVS_FEATURE_DEVICE_OSD_NAME_TX          0x00000200
#define     CECSVS_FEATURE_DEVICE_MENU_CONTROL         0x00000400
#define     CECSVS_FEATURE_REMOTE_CONTROL_PASS_THRU    0x00000800
#define     CECSVS_FEATURE_DEVICE_POWER_STATUS         0x00001000

#define     CECSVS_FEATURE_SYSTEM_AUDIO_CONTROL        0x00002000
#define     CECSVS_FEATURE_AUDIO_RATE_CONTROL          0x00004000
#define     CECSVS_FEATURE_TIMER_PROGRAMMING           0x00008000
#define     CECSVS_FEATURE_AUDIO_RETURN_CHANNEL        0x00010000

#define CECSVS_MENU_STATE_ACTIVATED              (0)
#define CECSVS_MENU_STATE_DEACTIVATED            (1)

/*HDMI CEC Message Opcode */
#define CECSVS_OPCODE_UNDEFINED                    0xFFFF
// General Protocol messages
#define CECSVS_OPCODE_FEATURE_ABORT                0x00
#define CECSVS_OPCODE_ABORT                        0xFF

// One Touch Play
#define CECSVS_OPCODE_ACTIVE_SOURCE                0x82
#define CECSVS_OPCODE_IMAGE_VIEW_ON                0x04
#define CECSVS_OPCODE_TEXT_VIEW_ON                 0x0D

// Routing control
#define CECSVS_OPCODE_REQUEST_ACTIVE_SOURCE        0x85
#define CECSVS_OPCODE_SET_STREAM_PATH              0x86
#define CECSVS_OPCODE_ROUTING_CHANGE               0x80
#define CECSVS_OPCODE_ROUTING_INFO                 0x81
#define CECSVS_OPCODE_INACTIVE_SOURCE              0x9D

// Standby
#define CECSVS_OPCODE_STANDBY                      0x36

// One touch record
#define CECSVS_OPCODE_RECORD_OFF                   0x0B
#define CECSVS_OPCODE_RECORD_ON                    0x09
#define CECSVS_OPCODE_RECORD_STATUS                0x0A
#define CECSVS_OPCODE_RECORD_TV_SCREEN             0x0F

// System information
#define CECSVS_OPCODE_GET_MENU_LANG                0x91
#define CECSVS_OPCODE_GIVE_PHY_ADDR                0x83
#define CECSVS_OPCODE_REPORT_PHY_ADDR              0x84
#define CECSVS_OPCODE_SET_MENU_LANG                0x32
#define CECSVS_OPCODE_CEC_VERSION                  0x9E
#define CECSVS_OPCODE_GET_CEC_VERSION              0x9F

// Deck control
#define CECSVS_OPCODE_DECK_STATUS                  0x1B
#define CECSVS_OPCODE_GIVE_DECK_STATUS             0x1A
#define CECSVS_OPCODE_DECK_CONTROL                 0x42
#define CECSVS_OPCODE_PLAY                         0x41

// Tuner control
#define CECSVS_OPCODE_GIVE_TUNER_DEVICE_STATUS     0x08
#define CECSVS_OPCODE_SELECT_ANALOGUE_SERVICE      0x92
#define CECSVS_OPCODE_SELECT_DIGITAL_SERVICE       0x93
#define CECSVS_OPCODE_TUNER_STEP_DECREMENT         0x06
#define CECSVS_OPCODE_TUNER_STEP_INCREMENT         0x05
#define CECSVS_OPCODE_TUNER_DEVICE_STATUS          0x07

// Vendor specific commands
#define CECSVS_OPCODE_DEVICE_VENDOR_ID             0x87
#define CECSVS_OPCODE_GIVE_DEVICE_VENDOR_ID        0x8C
#define CECSVS_OPCODE_VENDOR_COMMAND               0x89
#define CECSVS_OPCODE_VENDOR_COMMAND_WITH_ID       0xA0
#define CECSVS_OPCODE_VENDOR_REMOTE_BTN_DOWN       0x8A
#define CECSVS_OPCODE_VENDOR_REMOTE_BTN_UP         0x8B

// OSD status display
#define CECSVS_OPCODE_SET_OSD_STRING               0x64

// Device OSD name transfer
#define CECSVS_OPCODE_GIVE_OSD_NAME                0x46
#define CECSVS_OPCODE_SET_OSD_NAME                 0x47

// Device menu control, Remote control pass-through
#define CECSVS_OPCODE_USER_CONTROL_PRESSED         0x44
#define CECSVS_OPCODE_USER_CONTROL_RELEASED        0x45
#define CECSVS_OPCODE_MENU_REQUEST                 0x8D

// Device power status
#define CECSVS_OPCODE_GIVE_DEVICE_POWER_STATUS     0x8F
#define CECSVS_OPCODE_REPORT_POWER_STATUS          0x90
#define CECSVS_OPCODE_MENU_STATUS              0x8E

// Power Status
#define CECSVS_PWR_STS_ON                     (0)
#define CECSVS_PWR_STS_STANDBY                (1)
#define CECSVS_PWR_STS_IN_TRANS_STANDBY_TO_ON (2)
#define CECSVS_PWR_STS_IN_TRANS_ON_TO_STANDBY (3)

// System Audio Control
#define CECSVS_OPCODE_GIVE_AUDIO_STATUS            0x71
#define CECSVS_OPCODE_GIVE_SYS_AUDIO_MODE_STATUS   0x7D
#define CECSVS_OPCODE_REPORT_AUDIO_STATUS          0x7A
#define CECSVS_OPCODE_SET_SYS_AUDIO_MODE           0x72
#define CECSVS_OPCODE_SYS_AUDIO_MODE_REQUEST       0x70
#define CECSVS_OPCODE_SYS_AUDIO_MODE_STATUS        0x7E
#define CECSVS_OPCODE_REQUEST_SHORT_AUDIO_DESC     0xA4 // new added
#define CECSVS_OPCODE_REPORT_SHORT_AUDIO_DESC      0xA3

// Audio Rate Control
#define CECSVS_OPCODE_SET_AUDIO_RATE               0x9A

// Timer Programming
#define CECSVS_OPCODE_CLR_ANALOGUE_TIMER           0x33
#define CECSVS_OPCODE_CLR_DIGITAL_TIMER            0x99
#define CECSVS_OPCODE_CLR_EXTERNAL_TIMER           0xA1
#define CECSVS_OPCODE_SET_ANALOGUE_TIMER           0x34
#define CECSVS_OPCODE_SET_DIGITAL_TIMER            0x97
#define CECSVS_OPCODE_SET_EXTERNAL_TIMER           0xA2
#define CECSVS_OPCODE_SET_TIMER_PGM_TITLE          0x67
#define CECSVS_OPCODE_TIMER_CLEARED_STATUS         0x43
#define CECSVS_OPCODE_TIMER_STATUS                 0x35

//Audio Return Channel

#define CECSVS_OPCODE_INITIATE_ARC                 0xC0
#define CECSVS_OPCODE_REPORT_ARC_INITIATED         0xC1
#define CECSVS_OPCODE_REPORT_ARC_TERMINATED        0xC2
#define CECSVS_OPCODE_REQUEST_ARC_INITIATION       0xC3
#define CECSVS_OPCODE_REQUEST_ARC_TERMINATION      0xC4
#define CECSVS_OPCODE_TERMINATE_ARC                0xC5

// Feature abort reason
#define CECSVS_ABORT_REASON_UNREC_OPCODE         (0)
#define CECSVS_ABORT_REASON_INV_MODE_TO_RESPOND  (1)
#define CECSVS_ABORT_REASON_CANNOT_PROVIDE_SRC   (2)
#define CECSVS_ABORT_REASON_INV_OPCODE           (3)
#define CECSVS_ABORT_REASON_REFUSED          (4)

/*HDMI CEC Message enums and structres*/
// Logical device types
#define CECSVS_DEVICE_TV             (0)
#define CECSVS_DEVICE_REC            (1)
#define CECSVS_DEVICE_RES            (2)
#define CECSVS_DEVICE_TUNER          (3)
#define CECSVS_DEVICE_PB             (4)
#define CECSVS_DEVICE_AUDIO_SYS      (5)

// Status Request
#define CECSVS_STATUS_REQUEST_ON             (0x01)
#define CECSVS_STATUS_REQUEST_OFF            (0x02)
#define CECSVS_STATUS_REQUEST_ONCE           (0x03)

// CEC Version
#define CECSVS_CEC_VERSION_1_1   (0)
#define CECSVS_CEC_VERSION_1_2   (1)
#define CECSVS_CEC_VERSION_1_2a  (2)
#define CECSVS_CEC_VERSION_1_3   (3)
#define CECSVS_CEC_VERSION_1_3a  (4)
#define CECSVS_CEC_VERSION_1_4  (5)
#define CECSVS_CEC_VERSION_1_4a  (5)

// Menu request type
#define CECSVS_MENU_REQUEST_ACTIVATE             (0)
#define CECSVS_MENU_REQUEST_DEACTIVATE           (1)
#define CECSVS_MENU_REQUEST_QUERY                (2)

// User Control Code
#define CECSVS_UC_SELECT               (0x00)
#define CECSVS_UC_UP                   (0x01)
#define CECSVS_UC_DOWN                 (0x02)
#define CECSVS_UC_LEFT                 (0x03)
#define CECSVS_UC_RIGHT                (0x04)
#define CECSVS_UC_RIGHT_UP             (0x05)
#define CECSVS_UC_RIGHT_DOWN           (0x06)
#define CECSVS_UC_LEFT_UP              (0x07)
#define CECSVS_UC_LEFT_DOWN            (0x08)
#define CECSVS_UC_ROOT_MENU            (0x09)
#define CECSVS_UC_SETUP_MENU           (0x0A)
#define CECSVS_UC_CONTENTS_MENU        (0x0B)
#define CECSVS_UC_FAV_MENU             (0x0C)
#define CECSVS_UC_EXIT                 (0x0D)
#define CECSVS_UC_MEDIA_TOP_MENU       (0x10)
#define CECSVS_UC_MEDIA_CONTEXT_SENSITIVE_MENU                 (0x11)
#define CECSVS_UC_NUM_ENTRY_MODE       (0x1D)
#define CECSVS_UC_NUM_11               (0x1E)
#define CECSVS_UC_NUM_12               (0x1F)
#define CECSVS_UC_NUM_00               (0x20)
#define CECSVS_UC_NUM_01               (0x21)
#define CECSVS_UC_NUM_02               (0x22)
#define CECSVS_UC_NUM_03               (0x23)
#define CECSVS_UC_NUM_04               (0x24)
#define CECSVS_UC_NUM_05               (0x25)
#define CECSVS_UC_NUM_06               (0x26)
#define CECSVS_UC_NUM_07               (0x27)
#define CECSVS_UC_NUM_08               (0x28)
#define CECSVS_UC_NUM_09               (0x29)
#define CECSVS_UC_DOT                  (0x2A)
#define CECSVS_UC_ENTER                (0x2B)
#define CECSVS_UC_CLEAR                (0x2C)
#define CECSVS_UC_NEXT_FAV             (0x2F)
#define CECSVS_UC_CH_UP                (0x30)
#define CECSVS_UC_CH_DOWN              (0x31)
#define CECSVS_UC_PREV_CH              (0x32)
#define CECSVS_UC_SOUND_SEL            (0x33)
#define CECSVS_UC_INPUT_SEL            (0x34)
#define CECSVS_UC_DISP_INFO            (0x35)
#define CECSVS_UC_HELP                 (0x36)
#define CECSVS_UC_PAGE_UP              (0x37)
#define CECSVS_UC_PAGE_DOWN            (0x38)
#define CECSVS_UC_POWER                (0x40)
#define CECSVS_UC_VOL_UP               (0x41)
#define CECSVS_UC_VOL_DOWN             (0x42)
#define CECSVS_UC_MUTE                 (0x43)
#define CECSVS_UC_PLAY                 (0x44)
#define CECSVS_UC_STOP                 (0x45)
#define CECSVS_UC_PAUSE                (0x46)
#define CECSVS_UC_RECORD               (0x47)
#define CECSVS_UC_REWIND               (0x48)
#define CECSVS_UC_FAST_FORWARD         (0x49)
#define CECSVS_UC_EJECT                (0x4A)
#define CECSVS_UC_FORWARD              (0x4B)
#define CECSVS_UC_BACKWARD             (0x4C)
#define CECSVS_UC_STOP_REC             (0x4D)
#define CECSVS_UC_PAUSE_REC            (0x4E)
#define CECSVS_UC_ANGLE                (0x50)
#define CECSVS_UC_SUB_PICT             (0x51)
#define CECSVS_UC_VIDEO_ON_DEMAND      (0x52)
#define CECSVS_UC_EPG                  (0x53)
#define CECSVS_UC_TIMER_PGM            (0x54)
#define CECSVS_UC_INIT_CFG             (0x55)
#define CECSVS_UC_SEL_BRD_TYPE_FN      (0x56)
#define CECSVS_UC_SEL_SOUND_PPT_FN     (0x57)
#define CECSVS_UC_PLAY_FN              (0x60)
#define CECSVS_UC_PAUSE_PLAY_FN        (0x61)
#define CECSVS_UC_RECORD_FN            (0x62)
#define CECSVS_UC_PAUSE_RECORD_FN      (0x63)
#define CECSVS_UC_STOP_FN              (0x64)
#define CECSVS_UC_MUTE_FN              (0x65)
#define CECSVS_UC_RESTORE_VOL_FN       (0x66)
#define CECSVS_UC_TUNE_FN              (0x67)
#define CECSVS_UC_SEL_DISK_FN          (0x68)
#define CECSVS_UC_SEL_AV_INPUT_FN      (0x69)
#define CECSVS_UC_SEL_AUDIO_INPUT_FN   (0x6A)
#define CECSVS_UC_POWER_TOGGLE_FN      (0x6B)
#define CECSVS_UC_POWER_OFF_FN         (0x6C)
#define CECSVS_UC_POWER_ON_FN          (0x6D)
#define CECSVS_UC_F1_BLUE              (0x71)
#define CECSVS_UC_F2_RED               (0x72)
#define CECSVS_UC_F3_GREEN             (0x73)
#define CECSVS_UC_F4_YELLOW            (0x74)
#define CECSVS_UC_F5                   (0x75)
#define CECSVS_UC_DATA                 (0x76)

// Deck Info
#define CECSVS_DECK_INFO_PLAY                (0x11)
#define CECSVS_DECK_INFO_RECORD              (0x12)
#define CECSVS_DECK_INFO_PLAY_REV            (0x13)
#define CECSVS_DECK_INFO_STILL               (0x14)
#define CECSVS_DECK_INFO_SLOW                (0x15)
#define CECSVS_DECK_INFO_SLOW_REV            (0x16)
#define CECSVS_DECK_INFO_FAST_FWD            (0x17)
#define CECSVS_DECK_INFO_FAST_REV            (0x18)
#define CECSVS_DECK_INFO_NO_MEDIA            (0x19)
#define CECSVS_DECK_INFO_STOP                (0x1A)
#define CECSVS_DECK_INFO_WIND                (0x1B)
#define CECSVS_DECK_INFO_REWIND              (0x1C)
#define CECSVS_DECK_INFO_INDEX_SEARCH_FWD    (0x1D)
#define CECSVS_DECK_INFO_INDEX_SEARCH_REV    (0x1E)
#define CECSVS_DECK_INFO_OTHER_STS           (0x1F)

//TV CEC opcodes
// Recording Source
#define CECSVS_REC_SRC_OWN_SRC         (1)
#define CECSVS_REC_SRC_DIG_SRV         (2)
#define CECSVS_REC_SRC_ANA_SRV         (3)
#define CECSVS_REC_SRC_EXT_PLUG        (4)
#define CECSVS_REC_SRC_EXT_PHY_ADDR    (5)

// Display Control
#define CECSVS_DISPCTRL_DISP_FOR_DEF_TIME    (0x00)
#define CECSVS_DISPCTRL_DISP_UNTIL_CLEARED   (0x40)
#define CECSVS_DISPCTRL_CLR_PREV_MSG         (0x80)
#define CECSVS_DISPCTRL_RESERVED             (0xC0)

// Service Identification Method
#define CECSVS_SRV_IDEN_DIG_ID                 (0x00)
#define CECSVS_SRV_IDEN_CHN                    (0x01)

// Digital broadcast system
#define CECSVS_DIG_BROADCAST_SYS_ARIB_GEN      (0x00)
#define CECSVS_DIG_BROADCAST_SYS_ATSC_GEN      (0x01)
#define CECSVS_DIG_BROADCAST_SYS_DVB_GEN       (0x02)
#define CECSVS_DIG_BROADCAST_SYS_ARIB_BS       (0x08)
#define CECSVS_DIG_BROADCAST_SYS_ARIB_CS       (0x09)
#define CECSVS_DIG_BROADCAST_SYS_ARIB_T        (0x0A)
#define CECSVS_DIG_BROADCAST_SYS_ATSC_CAB      (0x10)
#define CECSVS_DIG_BROADCAST_SYS_ATSC_SAT      (0x11)
#define CECSVS_DIG_BROADCAST_SYS_ATSC_TER      (0x12)
#define CECSVS_DIG_BROADCAST_SYS_DVB_C         (0x18)
#define CECSVS_DIG_BROADCAST_SYS_DVB_S         (0x19)
#define CECSVS_DIG_BROADCAST_SYS_DVB_S2        (0x1A)
#define CECSVS_DIG_BROADCAST_SYS_DVB_T         (0x1B)

// Analogue Broadcast Type
#define CECSVS_ANALOGUE_BROADCAST_CABLE       (0)
#define CECSVS_ANALOGUE_BROADCAST_SATELLITE   (1)
#define CECSVS_ANALOGUE_BROADCAST_TERRESTRIAL (2)

// Recording Sequence
#define CECSVS_REC_SEQUENCE_ONCE_ONLY  (0)
#define CECSVS_REC_SEQUENCE_SUN        (1)
#define CECSVS_REC_SEQUENCE_MON        (2)
#define CECSVS_REC_SEQUENCE_TUES       (4)
#define CECSVS_REC_SEQUENCE_WED        (8)
#define CECSVS_REC_SEQUENCE_THUR       (16)
#define CECSVS_REC_SEQUENCE_FRI        (32)
#define CECSVS_REC_SEQUENCE_SAT        (64)

// Broadcast system
#define CECSVS_BROADCAST_SYSTEM_PAL_BG      (0)
#define CECSVS_BROADCAST_SYSTEM_SECAM_LDASH (1)
#define CECSVS_BROADCAST_SYSTEM_PAL_M       (2)
#define CECSVS_BROADCAST_SYSTEM_NTSC_M      (3)
#define CECSVS_BROADCAST_SYSTEM_PAL_I       (4)
#define CECSVS_BROADCAST_SYSTEM_SECAM_DK    (5)
#define CECSVS_BROADCAST_SYSTEM_SECAM_BG    (6)
#define CECSVS_BROADCAST_SYSTEM_SECAM_L     (7)
#define CECSVS_BROADCAST_SYSTEM_PAL_DK      (8)
#define CECSVS_BROADCAST_SYSTEM_OTHER_SYS   (0x1F)

// External source specifier
#define CECSVS_EXT_SRC_PLUG      (4)
#define CECSVS_EXT_SRC_PHY_ADDR  (5)

// Play Mode
#define CECSVS_PLAY_MODE_FORWARD             (0x24)
#define CECSVS_PLAY_MODE_REVERSE             (0x20)
#define CECSVS_PLAY_MODE_STILL               (0x25)
#define CECSVS_PLAY_MODE_FAST_FWD_MIN_SPEED  (0x05)
#define CECSVS_PLAY_MODE_FAST_FWD_MED_SPEED  (0x06)
#define CECSVS_PLAY_MODE_FAST_FWD_MAX_SPEED  (0x07)
#define CECSVS_PLAY_MODE_FAST_REV_MIN_SPEED  (0x09)
#define CECSVS_PLAY_MODE_FAST_REV_MED_SPEED  (0x0A)
#define CECSVS_PLAY_MODE_FAST_REV_MAX_SPEED  (0x0B)
#define CECSVS_PLAY_MODE_SLOW_FWD_MIN_SPEED  (0x15)
#define CECSVS_PLAY_MODE_SLOW_FWD_MED_SPEED  (0x16)
#define CECSVS_PLAY_MODE_SLOW_FWD_MAX_SPEED  (0x17)
#define CECSVS_PLAY_MODE_SLOW_REV_MIN_SPEED  (0x19)
#define CECSVS_PLAY_MODE_SLOW_REV_MED_SPEED  (0x1A)
#define CECSVS_PLAY_MODE_SLOW_REV_MAX_SPEED  (0x1B)

// Deck control mode
#define CECSVS_DECK_CTRL_MODE_SKIPFWD_WINDFWD  (0x01)
#define CECSVS_DECK_CTRL_MODE_SKIPBWD_REWIND   (0x02)
#define CECSVS_DECK_CTRL_MODE_STOP             (0x03)
#define CECSVS_DECK_CTRL_MODE_EJECT            (0x04)

#endif
