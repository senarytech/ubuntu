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

cleanup1:
	Finalize();
	return 0;
}
