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
//! \file amp_logger_isr.h
//!
//! \brief This file declares API and common type of logger. This logger is
//!        intent to be used in atomic conext with less overload than print or
//!        logcat.
//!
//! Purpose:
//!
//!
//!     Version    Date                     Author
//!     V 1.00,    Jan 6 2013,              Ye Liu
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef __AMP_LOGGER_ISR_H__
#define __AMP_LOGGER_ISR_H__

#include "amp_types.h"


/*
 * Debug Print Function Declaration
 */
#ifdef __cplusplus
extern "C"
{
#endif

/** \func
    \brief The function create a logger context for atomic context
    \param uiLines Max number of lines in buffer
    \param uiLengh Lenth in byte for each print line
    \param uiFlags Optional flags
 */
HRESULT AMP_CreateISRLogger(AMP_IN UINT32 uiLines,
                            AMP_IN UINT32 uiLengh,
                            AMP_IN UINT32 uiFlags);

/** \func
    \brief The function destroy isr logger context
 */
HRESULT AMP_DestroyISRLogger(void);

/** \func
    \brief The function print a formated character message in atomic context
    \param uiFlags Optional flags: ISR_LOGGER_TIMESTAMP is logged with
           timestamp, ISR_LOGGER_LOGCAT logged in logcat, by default is logged
           in standard output
    \param fmt Format in printf-style
 */
UINT AMP_ISRLog(AMP_IN UINT32 uiFlags, AMP_IN CHAR* fmt, ...);

#ifdef __cplusplus
}
#endif

/* log flags */
/** \def ISR_LOGGER_TIMESTAMP
    \brief The macro indicate logger to log with timestamp
 */
#define ISR_LOGGER_TIMESTAMP        0x4000
/** \def ISR_LOGGER_LOGCAT
    \brief The macro indicate logger to use logcat
 */
#define ISR_LOGGER_LOGCAT           0x8000

/** \def AMPLOGi
    \brief The macro print a formated character message in atomic context with
           timestamp
    \param fmt - format in printf-style
 */
#define AMPLOGi(fmt,...)    AMP_ISRLog(ISR_LOGGER_TIMESTAMP, fmt, ##__VA_ARGS__)

#endif /* __AMP_LOGGER_ISR_H__ */

