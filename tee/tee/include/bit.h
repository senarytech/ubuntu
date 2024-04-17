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

#ifndef _BIT_H_
#define _BIT_H_

/*
 * bit definition
 */

/** Define a bit							*/
#define BIT(k)			(1UL << (k))
#define BIT64(k)		(1ULL << (k))

/** Define a continuous bit field with NUM bits				*/
#define BITS(start, num)	((~((-1UL) << (num))) << (start))
#define BITS64(start, num)	((~((-1ULL) << (num))) << (start))

/*
 * bit operation
 */

/** Clear a bit								*/
#define bit_clear(src, bit)	\
	((src) & (~BIT(bit)))

/** Set a bit								*/
#define bit_set(src, bit)	\
	((src) & BIT(bit))

/** Get a bit								*/
#define bit_get(src, bit)	\
	(((src) >> (bit)) & 0x1)

/** Check a bit whether it's set (0x1)					*/
#define bit_check(src, bit)	\
	((src) & BIT(bit))


/*
 * bits operation
 */

/** Clear a masked area in a value					*/
#define bits_clear(src, mask)	\
	((src) & (~(mask)))

/** Set a masked area to a value					*/
#define bits_set(src, val, mask, shift) \
	((((src) & ~(mask)) | (((val) << (shift)) & (mask))))

/** Get a field in a register						*/
#define bits_get(src, mask, shift)	\
	(((src) & (mask)) >> (shift))

/** Check whether the field is not 0					*/
#define bits_check(src, mask)	\
	((src) & (mask))

#endif /* _BIT_H_ */
