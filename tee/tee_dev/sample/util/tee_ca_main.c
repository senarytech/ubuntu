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

#include "tee_ca_util.h"
#include "stdlib.h"
#include "stdio.h"

int main(int argc, const char *argv[])
{
	int res = 0;
	/* an example to show how TEE UTIL works. */
	void *srcPhyAddr = NULL;
	void *dstPhyAddr = NULL;
	unsigned long len = 0;
	const char *taFile = NULL;

	if (argc < 4) {
		printf("%s <srcPhyAddr> <dstPhyAddr> <len> [taPath]\n",
				argv[0]);
		return -1;
	}
	if (argc > 4)
		taFile = argv[4];

	srcPhyAddr = (void *)strtoul(argv[1], 0, NULL);
	dstPhyAddr = (void *)strtoul(argv[2], 0, NULL);
	len = strtoul(argv[3], 0, NULL);

	/* initialize functional API environment */
	if (TEEU_Initialize(taFile)) {
		printf("fail to open device\n");
		return -1;
	}

	res = TEEU_MemCopy(dstPhyAddr, srcPhyAddr, len);
	if (res)
		printf("fail to copy data: 0x%08x\n", res);

cleanup1:
	TEEU_Finalize();
	return 0;
}
