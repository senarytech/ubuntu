/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (C) 2022 Synaptics Incorporated. All rights reserved.
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
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
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
//! \file bitop.h
//!
//! \brief  Macro Related to Bit Operations
//!
//! Purpose: - This file has Macro for bit operations.
//!
//!
//! Version, Date and Author :
//!   V 1.00,    Aug 02 2022,   Prem Anand
//!
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#define _bSETMASK_(b)                                      ((b)<32 ? (1<<((b)&31)) : 0)
#define _NSETMASK_(msb,lsb)                                (_bSETMASK_((msb)+1)-_bSETMASK_(lsb))
#define _bCLRMASK_(b)                                      (~_bSETMASK_(b))
#define _NCLRMASK_(msb,lsb)                                (~_NSETMASK_(msb,lsb))
#define _BFGET_(r,msb,lsb)                                 (_NSETMASK_((msb)-(lsb),0)&((r)>>(lsb)))
#define _BFSET_(r,msb,lsb,v)                               do{ (r)&=_NCLRMASK_(msb,lsb); (r)|=_NSETMASK_(msb,lsb)&((v)<<(lsb)); }while(0)
