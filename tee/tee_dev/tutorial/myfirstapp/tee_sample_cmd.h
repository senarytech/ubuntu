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
#define TASAMPLE_UUID	{ 0x12345678, 0xabcd, 0x1234, \
	{0x56, 0x78, 0xa1, 0xb2, 0xc3, 0xd4, 0x04, 0x20} }

#endif /* _TEE_SAMPLE_COMMON_H_ */
