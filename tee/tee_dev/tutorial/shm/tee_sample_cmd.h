/*
 * Copyright (c) 2013-2014 Marvell International Ltd. and its affiliates.
 * All rights reserved.
 *
 * This software file (the "File") is owned and distributed by Marvell
 * International Ltd. and/or its affiliates ("Marvell") under the following
 * licensing terms.
 *
 * If you received this File from Marvell and you have entered into a
 * commercial license agreement (a "Commercial License") with Marvell, the
 * File is licensed to you under the terms of the applicable Commercial
 * License.
 */

#ifndef _TEE_SAMPLE_COMMON_H_
#define _TEE_SAMPLE_COMMON_H_

/* UUID of the TA.
 *
 * under linux, can use bellow application to generate it.
 * $ uuid
 * 88da5306-8461-11e3-afd9782bcb5cf3e3
 */
#define TASAMPLE_UUID	{ 0xb7932660, 0x4eb3, 0x11e4, \
	{0xa8, 0xea, 0x78, 0x2b, 0xcb, 0x5c, 0xf3, 0xe8} }

enum {
	CMD_VAL_ADD,
	CMD_VAL_SUB,
	CMD_MEM_REVERSE,
	CMD_MAX
};

#endif /* _TEE_SAMPLE_COMMON_H_ */
