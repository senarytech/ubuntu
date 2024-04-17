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

#ifndef _BYTE_REV_H_
#define _BYTE_REV_H_

#include "types.h"

inline uint16_t byte_rev16(uint16_t n)
{
	return ((n << 8) & 0xff00) | ((n >> 8) & 0x00ff);
}

inline uint32_t byte_rev32(uint32_t n)
{
	return  ((n << 24) & 0xff000000) |
		((n <<  8) & 0x00ff0000) |
		((n >>  8) & 0x0000ff00) |
		((n >> 24) & 0x000000ff);
}


#endif /* _BYTE_REV_H_ */
