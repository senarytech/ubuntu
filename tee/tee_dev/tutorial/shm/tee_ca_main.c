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

#include <stdio.h>
#include <stdlib.h>
#include "tee_ca_sample.h"

static void DumpData(const char *data, int len, const char *info)
{
	int i;

	printf("------------------- %s dump start -----------------\n", info);
	for (i = 0; i < len; i++) {
		printf("%02x ", data[i]);
		if (((i + 1) % 16) == 0)
			printf("\n");
	}
	printf("------------------- %s dump end --------------------\n", info);
}

void TestMem(void)
{
	void *inShm = NULL;
	void *outShm = NULL;
	char *in = NULL;
	char *out = NULL;
	int inSize = 256;
	int outSize = 256;
	int inLen = inSize;
	int outLen = 0;
	int i;

	/* prepare to test memory operations */
	inShm = AllocateSharedMemory(inSize, &in);
	outShm = AllocateSharedMemory(outSize, &out);

	if (!inShm || !outShm)
		goto cleanup2;

	for (i = 0; i < inSize; i++)
		in[i] = i;

	/* test reverse */
	outLen = Reverse(outShm, inShm, inLen);
	if (outLen < 0) {
		printf("fail to reverse memory\n");
		goto cleanup2;
	}

	/* check result */
	for (i = 0; i < inSize; i++) {
		if (out[i] != (char)(~in[i])) {
			printf("fail to reverse memory @ byte %d, "
					"0x%02x != ~0x%02x\n",
					i, out[i], in[i]);
			DumpData(in, inLen, "reverse input");
			DumpData(out, outLen, "reverse output");
			goto cleanup2;
		}
	}

	printf("succeed to reverse memory.\n");

cleanup2:
	ReleaseSharedMemory(inShm);
	ReleaseSharedMemory(outShm);
}

int main(int argc, const char *argv[])
{
	const char *taName = NULL;
	if (argc > 1)
		taName = argv[1];

	/* initialize functional API environment */
	if (Initialize(taName)) {
		printf("fail to open device\n");
		return -1;
	}

	int a = 1, b = 2, c;
	c = Add(a, b);
	printf("%d + %d = %d, %s\n", a, b, c, (c == a + b) ? "PASS" : "FAIL");

	TestMem();

cleanup1:
	Finalize();
	return 0;
}
