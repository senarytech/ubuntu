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
//! \file bd_trace.h
//!
//! \brief This file declare the BD trace logger core functions.
//!
//! Purpose: Provide BD trace log funtions.
//!
//!
//!     Version    Date                     Author
//!     V 1.00,    May 10 2016,       Weizhao Jiang. Initial bd_trace.h.
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef __BD_TRACE_H__
#define __BD_TRACE_H__

#include "amp_types.h"
#include "isl/amp_logger.h"
#include "isl/amp_buf_desc.h"

/** \typedef BD_ACTION
 *  \brief BD action definition allocate/free/ref/unref/map/unmap etc.
 */
typedef enum {
    BD_NON = 0,                     /**< non */
    BD_ALLOC,                       /**< Allocate BD */
    BD_FREE,                        /**< Free BD */
    BD_REF,                         /**< Ref BD */
    BD_UNREF,                       /**< Unref BD */
    BD_SHARE,                       /**< Share BD */
    BD_MAP,                         /**< Map BD */
    BD_UNMAP,                       /**< Unmap BD */
    BD_PUSH,                        /**< push BD */
    BD_REL,                         /**< Release BD */
    BD_ACTION_MAX                   /**< The max value*/
} BD_ACTION;


/** \def BD_TRACE
 *  \brief This MACRO is used to trace BD when it transfer across modules
 */
#define BD_TRACE(hBD, eAction)                        \
do {                                                                        \
    extern void AMPBuf_Trace(AMP_BD_HANDLE hBD, BD_ACTION eAction);         \
    AMPBuf_Trace((hBD), (eAction));               \
} while(0)

#endif /* __BD_TRACE_H__ */

