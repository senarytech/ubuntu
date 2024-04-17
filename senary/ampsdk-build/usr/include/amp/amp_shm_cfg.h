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
//! \file amp_shm_cfg.h
//!
//! \brief This file define basic type and api of the app component.
//!
//! Purpose:
//!
//!
//!     Version    Date                     Author
//!     V 1.00,    Jul 12 2016              Weizhao Jiang
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef __AMP_SHM_CFG_H__
#define __AMP_SHM_CFG_H__

#include "amp_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct __shm_sw_cfg__{
    char        cLogDir[64];
    UINT32      EnUsageDbg;
    UINT32      EnTraceDbg;
} SHM_SW_CFG;

typedef struct __shm_hw_cfg__ {
    UINT32      Info;
} SHM_HW_CFG;

typedef struct __shm_drv_cfg__ {
    SHM_HW_CFG      HWCfg;
    SHM_SW_CFG      SWCfg;
} SHM_DRV_CFG;

HRESULT AMP_SHM_Initialize_Server(SHM_DRV_CFG *pSHM_Cfg);

#ifdef __cplusplus
}
#endif

#endif /* __AMP_SHM_CFG_H__ */

