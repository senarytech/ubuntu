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
#include "tee_client_api.h"
#include "tee_sample_cmd.h"
#include "tee_ca_sample.h"

static const TEEC_UUID SampleTA_UUID = TASAMPLE_UUID;

static bool initialized = false;
static TEEC_Context context;
static TEEC_Session session;

int Initialize(const char *taName)
{
	TEEC_Result result = TEEC_SUCCESS;

	if (initialized)
		return result;

	/* [1] Connect to TEE */
	result = TEEC_InitializeContext(
			NULL,
			&context);
	if (result != TEEC_SUCCESS) {
		printf("fail to initialize context. ret=0x%08x\n", result);
		goto cleanup1;
	}

	/* [2] Load the TA */
	if (taName) {
		result = TEEC_LoadTA(&context, taName);
		if (result == TEEC_ERROR_ACCESS_CONFLICT) {
			printf("the TA has been loaded\n");
		} else if (result != TEEC_SUCCESS) {
			printf("fail to load the TA, ret=0x%08x\n", result);
			goto cleanup2;
		}
	}

	/* [3] Open a Session with the TEE application. */
	result = TEEC_OpenSession(
			&context,
			&session,
			&SampleTA_UUID,
			TEEC_LOGIN_USER,
			NULL,	/* No connection data needed for TEEC_LOGIN_USER. */
			NULL,	/* No payload, and do not want cancellation. */
			NULL);

	if (result != TEEC_SUCCESS) {
		printf("fail to open session. ret=0x%08x\n", result);
		goto cleanup2;
	}

	initialized = true;

	return TEEC_SUCCESS;
cleanup2:
	TEEC_FinalizeContext(&context);
cleanup1:
	return result;
}

void Finalize(void)
{
	if (!initialized)
		return;
	initialized = false;

	TEEC_CloseSession(&session);
	TEEC_FinalizeContext(&context);
}

int InvokeCommand(int cmdId, const char *custblob_file)
{
	TEEC_Result result = TEEC_SUCCESS;
	TEEC_Operation operation;
        FILE *pFile = NULL;
        uint32_t lenCUSTBLOB = 0;
        uint8_t *pCUSTBLOB = NULL;
        TEEC_SharedMemory hShm;

        if (custblob_file == NULL) {
            printf("No custblob file\n");
            result = 1;
            goto __EXIT;
        }

        pFile = fopen((const char*)custblob_file, "rb");
        if (pFile == NULL) {
            printf("Faile to open:%s\n", custblob_file);
            result = 1;
            goto __EXIT;
        }

        fseek(pFile, 0, SEEK_END);
        lenCUSTBLOB = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);

        hShm.size = lenCUSTBLOB;
        hShm.flags = TEEC_MEM_INPUT;
        result = TEEC_AllocateSharedMemory(&context, &hShm);
        if (result != TEEC_SUCCESS) {
            printf("TEEC_AllocateSharedMemory hShm fail\n");
            result = 1;
            goto __EXIT;
        }
        pCUSTBLOB = (uint8_t*)hShm.buffer;
        if (lenCUSTBLOB != fread(pCUSTBLOB, 1, lenCUSTBLOB, pFile)) {
            printf("Fail to read %d data\n", lenCUSTBLOB);
            result = 1;
            goto __EXIT;
        }
 
        operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_PARTIAL_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
        operation.params[0].memref.parent = &hShm;
        operation.params[0].memref.offset = 0;
        operation.params[0].memref.size = lenCUSTBLOB;
        operation.started = 1;

	result = TEEC_InvokeCommand(
			&session,
			cmdId,
			&operation,
			NULL);

	if (result != TEEC_SUCCESS) {
		/* show error here */
		printf("fail to invoke command 0x%08x, ret=0x%08x\n",
				cmdId, result);
	}
        else printf("succeed to invoke command 0x%08x\n");

__EXIT:
        TEEC_ReleaseSharedMemory(&hShm);
	return result;
}
