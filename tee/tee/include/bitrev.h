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

#ifndef _BIT_REV_H_
#define _BIT_REV_H_

#include "types.h"

/** Reverse 8 bits.
 *
 * @param n                 Data to reverse.
 *
 * @return unsigned char    The reversed data.
 *
 * @sa reverse_bit16(), reverse_bit32()
 */
inline uint8_t bit_rev8(uint8_t n)
{
	n = ((n >> 1) & 0x55) | ((n << 1) & 0xaa);
	n = ((n >> 2) & 0x33) | ((n << 2) & 0xcc);
	n = ((n >> 4) & 0x0f) | ((n << 4) & 0xf0);

	return n;
}

/** Reverse 16 bits.
 *
 * @param n                 Data to reverse.
 *
 * @return unsigned short   The reversed data.
 *
 * @sa reverse_bit16(), reverse_bit32()
 */
inline uint16_t bit_rev16(uint16_t n)
{
	n = ((n >> 1) & 0x5555) | ((n << 1) & 0xaaaa);
	n = ((n >> 2) & 0x3333) | ((n << 2) & 0xcccc);
	n = ((n >> 4) & 0x0f0f) | ((n << 4) & 0xf0f0);
	n = ((n >> 8) & 0x00ff) | ((n << 8) & 0xff00);

	return n;
}

/** Reverse 32 bits.
 *
 * @param n                 Data to reverse.
 *
 * @return unsigned int    The reversed data.
 *
 * @sa reverse_bit16(), reverse_bit32()
 */
inline uint32_t bit_rev32(uint32_t n)
{
	n = ((n >> 1) & 0x55555555) | ((n << 1) & 0xaaaaaaaa);
	n = ((n >> 2) & 0x33333333) | ((n << 2) & 0xcccccccc);
	n = ((n >> 4) & 0x0f0f0f0f) | ((n << 4) & 0xf0f0f0f0);
	n = ((n >> 8) & 0x00ff00ff) | ((n << 8) & 0xff00ff00);
	n = ((n >> 16) & 0x0000ffff) | ((n << 16) & 0xffff0000);

	return n;
}

#endif /* _BIT_REV_H_ */
