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
//! \file amp_debug.h
//!
//! \brief This file defines helper macros facilatate amp debugging.
//!
//! Purpose:
//!
//!
//!     Version    Date                     Author
//!     V 1.00,    Aug 27 2016,             Jun Ma
//!
////////////////////////////////////////////////////////////////////////////////

#ifndef __AMP_DEBUG_H__
#define __AMP_DEBUG_H__


/*
 * The AMP_ASSERT helper macro is for debug build only.
 * In a release build, the macro expands to nothing, and thus
 * takes no effects.
 * A release build is defined by following statement in the
 * corresponding profile at the "Options" section.
 * defined AMP_RELEASE_BUILD
 */

#ifndef AMP_RELEASE_BUILD
#include "isl/amp_logger.h"

#define AMP_ASSERT(x) \
            do{if(!(x)) { \
                AMPLOGE("AMP ASSERTION FAILURE AT %s LINE %d. (%s)\n", \
                __FUNCTION__, __LINE__, #x); \
                assert(0);} \
            }while(0)

#else /* AMP_RELEASE_BUILD */

#define AMP_ASSERT(x)   do{} while(0)

#endif /* AMP_RELEASE_BUILD */

#define AMP_CHECK_RETURN(Ret) do {               \
    if (Ret != SUCCESS) {                    \
        AMPLOGE("ASSERT ERROR Ret=%x", Ret); \
        return Ret;                          \
    }                                        \
} while(0)

#endif /* __AMP_DEBUG_H__ */

