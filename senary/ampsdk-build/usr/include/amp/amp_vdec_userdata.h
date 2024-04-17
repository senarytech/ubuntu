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
//! \file amp_vdec_userdata.h
//!
//! \brief Header file for user data parsed by vdec.
//!
//! Purpose:
//!
//!     Version    Date                     Author
//!     V 1.00,    May 23 2013,             Lei Wang
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef __AMP_VDEC_USERDATA_H__
#define __AMP_VDEC_USERDATA_H__

#define SLICE_LEVEL_USER_DATA           0
#define FIELD_LEVEL_USER_DATA           1
#define FRAME_LEVEL_USER_DATA           2
#define ENTRY_POINT_LEVEL_USER_DATA     3
#define SEQUENCE_LEVEL_USER_DATA        4

#define VIDEO_USER_DATA_TYPE_START      0x0
#define VIDEO_USER_DATA_TYPE_CC         0x0         ///<: Closed Caption
#define VIDEO_USER_DATA_TYPE_AFD        0x1         ///<: Active Format Description
#define VIDEO_USER_DATA_TYPE_BAR        0x2         ///<: BAR
#define VIDEO_USER_DATA_TYPE_GSM        0x3         ///<: GOP Structure Map
#define VIDEO_USER_DATA_TYPE_ESM        0x4         ///<: EPS Structure Map
#define VIDEO_USER_DATA_TYPE_SEQ        0x5         ///<: Sequence Header
#define VIDEO_USER_DATA_TYPE_OMT        0x6         ///<: Offset metadata
#define VIDEO_USER_DATA_TYPE_YCC        0x7
#define VIDEO_USER_DATA_TYPE_BD_CC      0x8
#define VIDEO_USER_DATA_TYPE_FP         0x9
#define VIDEO_USER_DATA_TYPE_THDR       0xA         ///<: Technicolor HDR metadata
#define VIDEO_USER_DATA_TYPE_ALTERNATIVE_TC 0xB     ///<: Alternative TC for HEVC HDR
#define VIDEO_USER_DATA_TYPE_CONTENT_LIGHT_INFO 0xC ///<: Content Light Info for HEVC HDR
#define VIDEO_USER_DATA_TYPE_AV1_HDR    0xD         ///<: AV1 HDR metadata
#define VIDEO_USER_DATA_TYPE_PTS        0xE         ///<: presentation_time_stamp of DSS stream
#define VIDEO_USER_DATA_TYPE_SUBT       0xF         ///<: subtitle of DSS stream
#define VIDEO_USER_DATA_TYPE_AV1_DOVI   0x10        ///<: Dolby Vision AV1 metadata OBU
#define VIDEO_USER_DATA_TYPE_END        0x11
#define VIDEO_USER_DATA_TYPE_MAX        VIDEO_USER_DATA_TYPE_END

/** \struct USER_DATA_BLOCK_HEADER
 *  \brief Header followed by user data.
 */
typedef struct _user_data_block_header_ {
    ///<: Reserved for VDEC internal use.
    unsigned char   m_level;
    ///<: Reserved for VDEC internal use. Sub type of user data.
    ///<: so far only use for CC, 1-dvd cc 0-atsc/dvb/bd cc.
    unsigned char   m_sub_type;
    ///<: type of user data, defined as above.
    unsigned char   m_type;
    ///<: Reserved for VDEC internal use.
    unsigned char   m_valid;
    ///<: Reserved for VDEC internal use.
    unsigned char   m_matched;
    ///<: The byte offset of user data payload to start of the header.
    unsigned char   m_body_offset;
    ///<: The number of bytes of user data payload;
    unsigned short  m_length;
    ///<: Reserved for VDEC internal use.
    unsigned int  m_pos;
    ///<: Reserved for VDEC internal use.
    unsigned char*  m_pStart;
    ///<: only apply to interlaced frame, 1: top field first out
    unsigned char   m_is_top_field_first;
    ///<: only apply to interlaced frame, 1: repeat first field.
    unsigned char   m_is_repeat_first_field;
    ///<: The index of the associated decoded frame.
    unsigned int  m_cnt;
} USER_DATA_BLOCK_HEADER;

#endif /* __AMP_VDEC_USERDATA_H__ */
