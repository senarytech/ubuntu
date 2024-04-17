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

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "types.h"

#define MAKE_FOURCC(ch0, ch1, ch2, ch3) \
	((unsigned int)(char)(ch0) | ((unsigned int)(char)(ch1) << 8) | \
	((unsigned int)(char)(ch2) << 16) | ((unsigned int)(char)(ch3) << 24))

/*
 * Generic Image Format:
 *
 * +----------------------------+
 * | image_header		|
 * +----------------------------+
 * | chunk 0			|
 * +----------------------------+
 * | ...			|
 * +----------------------------+
 * | chunk n			|
 * +----------------------------+
 */

#define IMAGE_HEADER_MAGIC_NUM		MAKE_FOURCC('I', 'M', '*', 'H')
#define IMAGE_HEADER_VERSION		(0)

struct image_header {
	uint32_t header_magic_num;	/* 'IM*H' */
	uint32_t header_size;
	uint32_t header_version;
	uint32_t header_reserved;
	char     image_name[32];
	uint32_t image_version;
	uint32_t reserved[2];
	uint32_t chunk_num;
	struct {
		uint32_t id;
		uint32_t offset;	/* start from header, 16bytes aligned */
		uint32_t size;
		uint32_t attr0;		/* bit24:31 - compress method; */
		/* data can be in (dest_start, dest_start + dest_size)
		 * if dest_size == 0, then chunk data must always place at
		 * dest_start.
		 */
		uint64_t dest_start;
		uint32_t dest_size;
		uint32_t attr1;		/* for compressed data, it's original_size */
	} chunk[0];
};

/*
 * predefined chunk ID.
 */

#define IMAGE_CHUNK_ID_BOOTLOADER		MAKE_FOURCC('B', 'T', 'L', 'R')
#define IMAGE_CHUNK_ID_SYSTEM_MANAGER		MAKE_FOURCC('S', 'M', '*', '*')
#define IMAGE_CHUNK_ID_UBOOT			MAKE_FOURCC('U', 'B', 'T', '*')

#define IMAGE_CHUNK_ID_LINUX_BOOTIMG		MAKE_FOURCC('L', 'N', 'X', 'B')
#define IMAGE_CHUNK_ID_LINUX_DTB		MAKE_FOURCC('L', 'D', 'T', 'B')

#define IMAGE_CHUNK_ID_TZ_KERNEL		MAKE_FOURCC('T', 'Z', 'K', '*')
#define IMAGE_CHUNK_ID_TZ_BOOT_PARAM		MAKE_FOURCC('T', 'Z', 'B', 'P')

#define IMAGE_CHUNK_ID_TZ_LOADABLE_TA		MAKE_FOURCC('T', 'Z', 'T', 'A')

#define IMAGE_CHUNK_ATTR_COMPRESS(attr)		((attr & 0xff000000) >> 24)

#endif /* _IMAGE_H_ */
