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

#ifndef _ALIGEN_H_
#define _ALIGEN_H_

#include "types.h"

#ifndef __KERNEL__

#define IS_POWER2(x)		(((x) & ((x) - 1)) == 0)
#define ALIGN_MASK(x, mask)	(((x) + (mask)) & ~(mask))

/* align x to a, a must be power of 2 */
#define ALIGN2(x, a)		ALIGN_MASK(x, (typeof(x))(a) - 1)
#define PTR_ALIGN2(p, a)	((typeof(p))ALIGN2((intptr_t)(p), a))
#define IS_ALIGNED2(x, a)	(((x) & ((typeof(x))(a) - 1)) == 0)

/* generic align x to a, we suppose align to power of 2 too */
#if 0
#define ALIGN(x, a)		(((x) + (a) - 1) / (a) * (a))
#define IS_ALIGNED(x, a)	(((x) % (a)) == 0)
#define PTR_ALIGN(p, a)		((typeof(p))ALIGN((unsigned long)(p), a))
#else
#define ALIGN(x, a)		ALIGN2(x, a)
#define PTR_ALIGN(p, a)		PTR_ALIGN2(p, a)
#define IS_ALIGNED(x, a)	IS_ALIGNED2(x, a)
#endif

#define ROUND_UP(x, y)		((((x)-1) | (typeof(x))((y)-1))+1)
#define ROUND_DOWN(x, y)	((x) & ~(typeof(x))((y)-1))

#endif /* __KERNEL__ */

#endif /* _ALIGEN_H_ */
