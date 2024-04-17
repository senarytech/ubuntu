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