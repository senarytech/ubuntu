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

#ifndef _TEE_CA_PTM_H_
#define _TEE_CA_PTM_H_

#if defined(TRUSTZONE) && defined(BUILDIN_AMP_CORE)

#include "tee_client_type.h"
//#include "ctypes.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define TA_PTM_UUID {0x1316a183, 0x894d, 0x43fe, {0x98, 0x93, 0xbb, 0x94, 0x6a, 0xe1, 0x04, 0x2d}}

enum {
        PTM_INVALID,
        PTM_DEBUG,
        PTM_CONSTRUCT_PT,
        PTM_FREE_PT,
        PTM_REF_PT,
        PTM_UNREF_PT,
        PTM_QUERY_PT,
        PTM_DUMP_PT,
        PTM_CMD_MAX
};

TEEC_Result PtmTzInitialize(void);
void PtmTzFinalize(void);
TEEC_Result tz_ptm_dump(uint32_t uSrcAddr, uint32_t uSize, uint32_t uDstAddr);

#ifdef __cplusplus
}
#endif

#endif  /*#ifdef TRUSTZONE*/

#endif /* _TEE_CA_PTM_H_ */
