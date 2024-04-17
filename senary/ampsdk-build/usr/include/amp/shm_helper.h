/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (C) 2013-2020 Synaptics Incorporated. All rights reserved.
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

#ifndef __MV_OSAL_SHM_HELPER_H__
#define __MV_OSAL_SHM_HELPER_H__

#include "shm_type.h"
#include "OSAL_api.h"

typedef enum {
    MV_OSAL_SHM_ZONE_ID_A = 0x80, // CONTIGUOUS_ZONE_ID_A
    MV_OSAL_SHM_ZONE_ID_B,        // CONTIGUOUS_ZONE_ID_B
    MV_OSAL_SHM_ZONE_ID_C,        // CONTIGUOUS_ZONE_ID_C
    MV_OSAL_SHM_ZONE_ID_D,        // CONTIGUOUS_ZONE_ID_D
    MV_OSAL_SHM_ZONE_ID_E,        // CONTIGUOUS_ZONE_ID_E
    MV_OSAL_SHM_ZONE_ID_MAX
} MV_OSAL_SHM_ZONE_ID_ENUM;

typedef enum {
    MV_OSAL_SHM_RULE_ID_0, // NON_CONTIGUOUS_ZONE_ID_0
    MV_OSAL_SHM_RULE_ID_1, // NON_CONTIGUOUS_ZONE_ID_1
    MV_OSAL_SHM_RULE_ID_2, // NON_CONTIGUOUS_ZONE_ID_2
    MV_OSAL_SHM_RULE_ID_3, // NON_CONTIGUOUS_ZONE_ID_3
    MV_OSAL_SHM_RULE_ID_4, // NON_CONTIGUOUS_ZONE_ID_4
    MV_OSAL_SHM_RULE_MAX
} MV_OSAL_SHM_RULE_ID_ENUM;

typedef struct {
    MV_OSAL_SHM_CONTENT_TYPE eContentType;
    BOOL                     bSecureContent;
    MV_OSAL_SHM_CLIENTS      stWriters;
    MV_OSAL_SHM_CLIENTS      stReaders;
} MV_OSAL_SHM_ZONE_MAPPING_KEY;

typedef struct {
    MV_OSAL_SHM_ZONE_MAPPING_KEY  Key;
    uint8_t SHM_ID;
    /*
     * zoneEnable indicates the zone will be used
     * for page table construction or not
     * all memory with rule ID equals ruleEnabled
     * so we do not define ruleEnable field
     */
    uint8_t zoneEnable;
    const char *heapName;
    uint8_t heapID;
} MV_OSAL_ZONE_TABLE_ENTRY;

HRESULT LookupZone(MV_OSAL_SHM_CONTENT_TYPE  eContentType,
                   BOOL                      bSecureContent,
                   MV_OSAL_SHM_CLIENTS       stWriters,
                   MV_OSAL_SHM_CLIENTS       stReaders,
                   MV_OSAL_SHM_ZONE          *pMemoryZone);

#endif /* __MV_OSAL_SHM_HELPER_H__ */
