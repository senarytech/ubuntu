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

#ifndef _MEM_TYPE_H_
#define _MEM_TYPE_H_

/*
 * memory type definitions
 *
 * note: kernel space must be secure. so 11xxb are invalid.
 */
#define MT_M_KERN		(0x08)
#define MT_M_NONSECURE		(0x04)
#define MT_M_TYPE		(0x03)

#define MT_USER(type)		((type))
#define MT_KERN(type)		((type) | MT_M_KERN)

#define MT_STRONGORDER		(0x00)	/* access in order */
#define MT_DEVICE		(0x01)	/* access in order */
#define MT_UNCACHED		(0x02)	/* access out-of-order */
#define MT_CACHED		(0x03)	/* access out-of-order */
#define MT_UNKNOWN		(0x0f)	/* unknown attribute */

/* memory types for end users */
#define MT_USER_STRONGORDER_S	MT_USER(MT_STRONGORDER)
#define MT_USER_DEVICE_S	MT_USER(MT_DEVICE)
#define MT_USER_UNCACHED_S	MT_USER(MT_UNCACHED)
#define MT_USER_CACHED_S	MT_USER(MT_CACHED)
#define MT_USER_STRONGORDER_NS	MT_USER(MT_M_NONSECURE | MT_STRONGORDER)
#define MT_USER_DEVICE_NS	MT_USER(MT_M_NONSECURE | MT_DEVICE)
#define MT_USER_UNCACHED_NS	MT_USER(MT_M_NONSECURE | MT_UNCACHED)
#define MT_USER_CACHED_NS	MT_USER(MT_M_NONSECURE | MT_CACHED)

#define MT_KERN_STRONGORDER	MT_KERN(MT_STRONGORDER)
#define MT_KERN_DEVICE		MT_KERN(MT_DEVICE)
#define MT_KERN_UNCACHED	MT_KERN(MT_UNCACHED)
#define MT_KERN_CACHED		MT_KERN(MT_CACHED)
/*not enable user mode*/
#define MT_KERN_STRONGORDER_NS	MT_KERN(MT_M_NONSECURE | MT_STRONGORDER)
#define MT_KERN_DEVICE_NS	MT_KERN(MT_M_NONSECURE | MT_DEVICE)
#define MT_KERN_UNCACHED_NS	MT_KERN(MT_M_NONSECURE | MT_UNCACHED)
#define MT_KERN_CACHED_NS	MT_KERN(MT_M_NONSECURE | MT_CACHED)

#endif /* _MEM_TYPE_H_ */
