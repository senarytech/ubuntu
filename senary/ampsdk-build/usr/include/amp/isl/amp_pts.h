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
//! \file amp_pts.h
//!
//! \brief This file defines playback PTS related helper macros and constants
//!        for AMP_PTS manipulation.
//!
//! Purpose:
//!
//!
//!     Version    Date                     Author
//!     V 1.00,    Dec 24 2014,             Jun Ma
//!
//! Note: All these helper macros are for 64-bit AMP_PTS
////////////////////////////////////////////////////////////////////////////////

#ifndef __AMP_PTS_H__
#define __AMP_PTS_H__

#include "amp_types.h"

/*
 * A constant value for invalid AMP_PTS
 */
#define AMP_INVALID_PTS             ((AMP_PTS)0)

/*
 * A constant value for testing if an AMP_PTS is valid
 * All AMP_PTS with the msb cleared are regarded as invalid
 */
#define AMP_PTS_VALID_MASK          (0x8000000000000000ULL)

/*
 * A constant value for getting the 33-bit PTS value
 */
#define AMP_PTS_VAL_MASK            (0x00000001FFFFFFFFULL)

/*
 * Helper macro to test if an AMP_PTS is valid
 */
#define AMP_IS_PTS_VALID(x)         (((x) & AMP_PTS_VALID_MASK) != 0)

/*
 * Helper macro to set an AMP_PTS as valid
 */
#define AMP_SET_PTS_VALID(x)        ((x) |= AMP_PTS_VALID_MASK)

/*
 * Helper macro to get the 33 bit AMP_PTS value only
 * This macro returns the lower 33 bits of AMP_PTS
 * The upper 31 bits of AMP_PTS will be replaced as 0
 * in the returned value
 */
#define AMP_GET_PTS_VAL(x)          ((x) & AMP_PTS_VAL_MASK)

/*
 * Helper macro to get the high/low DWORD of the AMP_PTS
 * It returns all upper/lower 32 bits unchanged
 */
#define AMP_GET_PTS_HIGH(x)         ((UINT32)((x) >> 32))
#define AMP_GET_PTS_LOW(x)          ((UINT32)(x))

/*
 * Helper macro to get the 33 bit AMP_PTS value in dual 32 bit DWORD
 * As only lower 33 bits of AMP_PTS are valid for PTS value, this macro
 * returns the lower 33 bits of AMP_PTS in dual 32 bit DWORD
 * The upper 31 bits of AMP_PTS will not be in the upper DWORD returned.
 * 31 zeros will be filled instead.
 */
#define AMP_GET_PTS_VAL_HIGH(x)     ((UINT32)(AMP_GET_PTS_VAL(x) >> 32))
#define AMP_GET_PTS_VAL_LOW(x)      ((UINT32)(x))

/*
 * Helper macro to make a PTS in the type of AMP_PTS
 * from two 32-bit DWORD
 */
#define AMP_MAKE_PTS64(high, low)   (((AMP_PTS)(high) << 32)+ (low))


#endif /* __AMP_PTS_H__ */

