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
//! \file amp_buf_com.h
//!
//! \brief This file define all common data structure, enum and API for AMP
//!        share memory management.
//!
//! Purpose:
//!
//!
//! Version, Date and Author :
//!   V 1.00,    December 25 2012,   Weizhao Jiang
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef __AMP_BUF_COM_H__
#define __AMP_BUF_COM_H__

#include "amp_types.h"
#include "isl/amp_buf_desc.h"

typedef struct _buf_desc_h_ {
    volatile UINT32 uiBDId;      /**< The SHM handle */
    volatile UINT64 uiAllocVA;   /**< The virtual address in alloctor process */

    volatile UINT32 uMagic;      /**< The magic number of BD Bode */
    volatile UINT32 uTagNum;     /**< The number of descriptor tag in this
                                      descriptor */
    volatile UINT32 uTotalSpace; /**< The total bytes in buffer descriptor */
    volatile UINT32 uDataSpace;  /**< The available bytes of desc tag space */

    volatile AMP_SHM_HANDLE hSubBDShm;  /**< The sub BD share memory handle */

    CHAR strName[AMP_BD_NAME_LEN]; /**< The Name of BD */
    volatile UINT32 uReserved[4];/**< The reserved space (for BD debug),
                                   bit map array to mark BD push to multiple ports
                                   support max port num is 128 = 32 * 4 */
} BUF_DESC_H;

#endif /* __AMP_BUF_COM_H__ */
