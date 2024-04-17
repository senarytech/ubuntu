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
//! \file amp_atrace.h
//!
//! \brief This file defines helper macros facilatate amp atrace.
//!
//! Purpose:
//!
//!
//!     Version    Date                     Author
//!     V 1.00,    May 28, 2018,            pengyy
//!
////////////////////////////////////////////////////////////////////////////////

#ifndef __AMP_ATRACE_H__
#define __AMP_ATRACE_H__


/*
 * The AMP_ATRACE macros are for debug build only.
 * In a release build, the macro expands to nothing, and thus
 * takes no effects.
 * A release build is defined by following statement in the
 * corresponding profile at the "Options" section.
 * MV_AMP_RELEASE_BUILD=true
 */

#if defined(HAVE_ANDROID_OS) && !defined(AMP_RELEASE_BUILD)

#define ENABLE_AMP_ATRACE
#define ATRACE_TAG      ATRACE_TAG_VIDEO
#include <cutils/trace.h>
#define AMP_ATRACE_BEGIN(name)     ATRACE_BEGIN(name);
#define AMP_ATRACE_END             ATRACE_END();
#define AMP_ATRACE_INT(name,value) ATRACE_INT(name,value);
#else /* AMP_RELEASE_BUILD */

#define AMP_ATRACE_BEGIN(NAME)     do{} while(0);
#define AMP_ATRACE_END             do{} while(0);
#define AMP_ATRACE_INT(name,value) do{} while(0);

#endif /* AMP_RELEASE_BUILD */

#endif /* __AMP_ATRACE_H__ */

