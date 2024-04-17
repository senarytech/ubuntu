/*
 * Copyright (c) 2017-2022 Synaptics International Ltd. and its affiliates.
 * All rights reserved.
 *
 * This software file (the "File") is owned and distributed by Synaptics
 * International Ltd. and/or its affiliates ("Synaptics") under the following
 * licensing terms.
 *
 * If you received this File from Synaptics and you have entered into a
 * commercial license agreement (a "Commercial License") with Synaptics, the
 * File is licensed to you under the terms of the applicable Commercial
 * License.
 */

#include <stdio.h>
#include <stdlib.h>
#include "tee_ca_sample.h"

int main(int argc, const char *argv[])
{
	const char *taName = NULL;
        const char *custblob_path = NULL;
	if (argc != 3) {
             printf("  %s libsample_ta_path CustBlob_path\n", argv[0]);
             return -1;
        }
        else {
	     taName = argv[1];
             custblob_path = argv[2];
        }

	/* initialize functional API environment */
	if (Initialize(taName)) {
		printf("fail to open device\n");
		return -1;
	}

	InvokeCommand(CUSTBLOB_VERIFY_SECURESTORE, custblob_path);

cleanup1:
	Finalize();
	return 0;
}
