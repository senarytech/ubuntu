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

static TEEC_Result OperateValue(int cmdId, int *out, int a, int b)
{
	TEEC_Result result;
	TEEC_Operation operation;

	if (!out)
		return TEEC_ERROR_BAD_PARAMETERS;

	memset(&operation, 0, sizeof(operation));
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_VALUE_INPUT,
			TEEC_VALUE_OUTPUT,
			TEEC_NONE,
			TEEC_NONE);
	operation.params[0].value.a = a;
	operation.params[0].value.b = b;

	/* clear result */
	operation.params[1].value.a = 0xdeadbeef;

	/* Start the arithmetic operation within the TEE application. */
	result = TEEC_InvokeCommand(
			&session,
			cmdId,
			&operation,
			NULL);

	if (result != TEEC_SUCCESS)
		*out = 0xdeadbeef;
	else
		*out = operation.params[1].value.a;

	return result;
}

int Add(int a, int b)
{
	int out;
	OperateValue(CMD_VAL_ADD, &out, a, b);

	return out;
}

int Sub(int a, int b)
{
	int out;
	OperateValue(CMD_VAL_SUB, &out, a, b);

	return out;
}


void *AllocateSharedMemory(unsigned int size, char **buffer)
{
	TEEC_Result result;
	TEEC_SharedMemory *shm = NULL;

	if (buffer)
		*buffer = NULL;

	shm = malloc(sizeof(*shm));
	if (NULL == shm)
		return shm;

	memset(shm, 0, sizeof(*shm));

	shm->size = size;
	shm->flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
	/* Use TEE Client API to allocate the underlying memory buffer. */
	result = TEEC_AllocateSharedMemory(
			&context,
			shm);
	if (result != TEEC_SUCCESS) {
		printf("ret=0x%08x\n", result);
		free(shm);
		return NULL;
	}

	if (buffer)
		*buffer = shm->buffer;

	return shm;
}

void ReleaseSharedMemory(void *shm)
{
	if (NULL == shm)
		return;

	TEEC_ReleaseSharedMemory(shm);
	free(shm);
}

char *GetSharedMemoryInfo(void *shm, unsigned int *size)
{
	TEEC_SharedMemory *s = shm;
	if (NULL == s)
		return NULL;

	if (size)
		*size = s->size;

	return s->buffer;
}


static int OperateMemory(int cmdId, void *outShm, void *inShm, int len)
{
	TEEC_Result result;
	TEEC_SharedMemory *in = inShm;
	TEEC_SharedMemory *out = outShm;
	TEEC_Operation operation;

	if (!in || !out || (size_t)len > in->size)
		return TEEC_ERROR_BAD_PARAMETERS;

	memset(&operation, 0, sizeof(operation));
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_MEMREF_PARTIAL_INPUT,
			TEEC_MEMREF_WHOLE,
			TEEC_NONE,
			TEEC_NONE);

	operation.params[0].memref.parent = in;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = len;

	operation.params[1].memref.parent = out;
	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = 0;

	/* Start the echo operation within the TEE application. */
	result = TEEC_InvokeCommand(
			&session,
			cmdId,
			&operation,
			NULL);

	return result;
}

int Reverse(void *outShm, void *inShm, int len)
{
	return OperateMemory(CMD_MEM_REVERSE, outShm, inShm, len);
}
