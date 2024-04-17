/*
 * Copyright (c) 2013-2014 Marvell International Ltd. and its affiliates.
 * All rights reserved.
 *
 * MARVELL CONFIDENTIAL
 *
 * The source code contained or described herein and all documents related to
 * the source code ("Material") are owned by Marvell International Ltd or its
 * suppliers or licensors. Title to the Material remains with Marvell
 * International Ltd or its suppliers and licensors. The Material contains
 * trade secrets and proprietary and confidential information of Marvell or
 * its suppliers and licensors.
 * The Material is protected by worldwide copyright and trade secret laws and
 * treaty provisions. No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Marvell's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery
 * of the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be
 * express and approved by Marvell in writing.
 */

#ifndef __MEM_REGION_USERDATA_H__
#define __MEM_REGION_USERDATA_H__

/* Warning: if you want change following MACROs, please make sure you already
 * reviewed and updated following places at same time:
 * 1. ion.h in OSAL: about ION_A_XX definition;
 * 2. ion.h in kernel: about ION_A_XX definition;
 * 3. mem_region_userdata.h in bootloader: ION_A_XX and memory region userdata
 *                                         definition.
 * 4. mem_region_userdata.h in tee: ION_A_XX and memory region userdata
 *                                  definition.
 */

/* The ion memory pool attribute flag bits */
#define ION_A_FS                0x0001      /* For secure memory */
#define ION_A_NS                0x0002      /* For non-secure memory */
#define ION_A_FC                0x0004      /* For cacheable memory */
#define ION_A_NC                0x0008      /* For non-cacheable memory */
#define ION_A_FD                0x0010      /* For dynamic memory */
#define ION_A_ND                0x0020      /* For static memory */
#define ION_A_CC                0x0100      /* For control (class) memory */
#define ION_A_CV                0x0200      /* For video (class) memory */
#define ION_A_CG                0x0400      /* For graphics (class) memory */
#define ION_A_CO                0x0800      /* For other (class) memory */
#define ION_A_CH                0x1000      /* For hardware (class) memory */

/* Memory region user definiation that used in userdata[0] */
#define TEE_MR_USER_TZCORE      0x00000000  /* The region is for TZ core */
#define TEE_MR_USER_ZSP         0x01000000  /* The region is for ZSP */
#define TEE_MR_USER_GPU         0x02000000  /* The region is for GPU */
#define TEE_MR_USER_SI          0x03000000  /* The region is for sysinit */
#define TEE_MR_USER_BL          0x04000000  /* The region is for bootloader */
#define TEE_MR_USER_SYSTEM      0x05000000  /* The region is for linyx system */
#define TEE_MR_USER_ION         0x06000000  /* The region is for ion pool */
#define TEE_MR_USER_REGISTER    0x07000000  /* The region is for register */
#define TEE_MR_USER_BCM         0x08000000  /* The region is for BCM */

/* MACRO funtions for userdata processing */
#define TEE_MR_USER(mr)                 (((mr)->userdata[0] & 0xFF000000))
#define TEE_MR_USER_IS_TZCORE(mr)       (TEE_MR_USER(mr) == TEE_MR_USER_TZCORE)
#define TEE_MR_USER_IS_ZSP(mr)          (TEE_MR_USER(mr) == TEE_MR_USER_ZSP)
#define TEE_MR_USER_IS_GPU(mr)          (TEE_MR_USER(mr) == TEE_MR_USER_GPU)
#define TEE_MR_USER_IS_SI(mr)           (TEE_MR_USER(mr) == TEE_MR_USER_SI)
#define TEE_MR_USER_IS_BL(mr)           (TEE_MR_USER(mr) == TEE_MR_USER_BL)
#define TEE_MR_USER_IS_SYSTEM(mr)       (TEE_MR_USER(mr) == TEE_MR_USER_SYSTEM)
#define TEE_MR_USER_IS_ION(mr)          (TEE_MR_USER(mr) == TEE_MR_USER_ION)

/* MACRO functions for ION memory region userdata processing */
#define TEE_MR_ION_ALG(mr)              (((mr)->userdata[0] & 0x00FFFFFF))
#define TEE_MR_ION_ATTRIB(mr)           ((mr)->userdata[1])

/* MACRO functions for ION pool attributes */
#define TEE_MR_ION_FOR_SECURE(mr)       (((mr)->userdata[1] & ION_A_FS))
#define TEE_MR_ION_FOR_NONSECURE(mr)    (((mr)->userdata[1] & ION_A_NS))
#define TEE_MR_ION_FOR_CACHEABLE(mr)    (((mr)->userdata[1] & ION_A_FC))
#define TEE_MR_ION_FOR_NONCACHEABLE(mr) (((mr)->userdata[1] & ION_A_NC))
#define TEE_MR_ION_FOR_DYNAMIC(mr)      (((mr)->userdata[1] & ION_A_FD))
#define TEE_MR_ION_FOR_STATIC(mr)       (((mr)->userdata[1] & ION_A_ND))
#define TEE_MR_ION_FOR_CONTROL(mr)      (((mr)->userdata[1] & ION_A_CC))
#define TEE_MR_ION_FOR_VIDEO(mr)        (((mr)->userdata[1] & ION_A_CV))
#define TEE_MR_ION_FOR_GRAPHICS(mr)     (((mr)->userdata[1] & ION_A_CG))
#define TEE_MR_ION_FOR_OTHER(mr)        (((mr)->userdata[1] & ION_A_CO))

#define TEE_ZONE_ACPU  0x0
#define TEE_ZONE_BCM   0x1
#define TEE_ZONE_RSV   0x2

#define TEE_MR_ACPU_ZONE(mr)        (((mr)->userdata[3] == TEE_ZONE_ACPU))
#define TEE_MR_RSV_ZONE(mr)        (((mr)->userdata[3] == TEE_ZONE_RSV))

#endif	/* __MEM_REGION_USERDATA_H__ */

