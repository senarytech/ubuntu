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
//! \file amp_component.h
//!
//! \brief This file define all common data structure, enum and API for AMP
//!        component, component port.
//!
//! Purpose:
//!
//!
//! Version, Date and Author :
//!   V 1.00,    November 7 2012,    Justin Wu
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef __AMP_COMPONENT_H__
#define __AMP_COMPONENT_H__

///////////////////////////////////////////////////////////////////////////////
//! [Part1] Header file include
///////////////////////////////////////////////////////////////////////////////
#include "amp_types.h"
#include "isl/amp_event_queue.h"
#include "isl/amp_pts.h"

///////////////////////////////////////////////////////////////////////////////
//! [Part2] Declaration of external variables or functions
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//! [Part3] Local macros, type definitions
///////////////////////////////////////////////////////////////////////////////

/** \def AMP_MEDIA_CAP_SET_ALL(capability, media_type)
 *
 *  \brief This macro function used to set all media type into media capability.
 *
 *         This macro works just a sample to set up the media capability
 *         array. After this call, it will set all 1 for all media bit mask.
 *
 *  \param[in] capability The media capability array pointer.
 *
 *  \retval No return value.
 *
 *  \sa AMP_MEDIA_TYPE
 *  \sa AMP_MEDIA_CAP
 *  \sa AMP_MEDIA_CAP_SET
 */
#define AMP_MEDIA_CAP_SET_ALL(capability)    do {               \
        int xyz;                                                \
        for (xyz = 0; xyz < (AMP_MEDIA_MAX >> 5); xyz++) {      \
            (capability)[xyz] = 0xFFFFFFFF;                     \
        }                                                       \
    } while (0)

/** \def AMP_MEDIA_CAP_SET(capability, media_type)
 *
 *  \brief This macro function used to set a media type into media capability.
 *
 *  \param[in] capability The media capability pointer.
 *  \param[in] media_type The media type to be set into media capablity.
 *
 *  \retval No return value.
 *
 *  \sa AMP_MEDIA_TYPE
 *  \sa AMP_MEDIA_CAP
 *  \sa AMP_MEDIA_CAP_CLEAR
 *  \sa AMP_MEDIA_CAP_CHECK
 */
#define AMP_MEDIA_CAP_SET(capability, media_type)    \
    ((capability)->uiBitMask[(media_type) / 32] | (1 << ((media_type) & 0x1F)))

/** \def AMP_MEDIA_CAP_CLEAR(capability, media_type)
 *
 *  \brief This function used to clear a media type from media capability.
 *
 *  \param[in] capability The media capability pointer.
 *  \param[in] media_type The media type to be cleared from media capablity.
 *
 *  \retval No return value.
 *
 *  \sa AMP_MEDIA_TYPE
 *  \sa AMP_MEDIA_CAP
 *  \sa AMP_MEDIA_CAP_SET
 *  \sa AMP_MEDIA_CAP_CHECK
 */
#define AMP_MEDIA_CAP_CLEAR(capability, media_type)  \
    ((capability)->uiBitMask[(media_type) / 32] & ~(1 << ((media_type) & 0x1F)))

/** \def AMP_MEDIA_CAP_CHECK
 *
 *  \brief This macro function used to check a media type in media capability.
 *
 *  \param[in] capability The media capability pointer.
 *  \param[in] media_type The media type to be checked from media capablity.
 *
 *  \retval Return non-zero value if the specified media type was set in the
 *          media capability, else return zero.
 *
 *  \sa AMP_MEDIA_TYPE
 *  \sa AMP_MEDIA_CAP
 *  \sa AMP_MEDIA_CAP_SET
 *  \sa AMP_MEDIA_CAP_CLEAR
 */
#define AMP_MEDIA_CAP_CHECK(capability, media_type)  \
    ((capability)->uiBitMask[(media_type) / 32] & (1 << ((media_type) & 0x1F)))

///////////////////////////////////////////////////////////////////////////////
// Component port
//
/** \enum AMP_PORT_FLAGS
 *
 *  \brief The port attribute flags.
 */
typedef enum {
    AUDIO_INTERLEAVE    =0x00000001,        /**< Audio sample is interleave. */
} AMP_PORT_FLAGS;

///////////////////////////////////////////////////////////////////////////////
//! [Part4] Declaration of local/global variables
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif
///////////////////////////////////////////////////////////////////////////////
//! [Part5] Declarations of local static functions
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//! [Part6] Global functions
///////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
}
#endif


#endif  /* __AMP_COMPONENT_H__ */
