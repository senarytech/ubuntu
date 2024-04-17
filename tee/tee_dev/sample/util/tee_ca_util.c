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
#include "tee_client_api.h"
#include "util_cmd.h"
#include "stdlib.h"
#include "stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static const TEEC_UUID UtilTA_UUID = UTILTA_UUID;

static bool initialized = false;
static TEEC_Context context;
static TEEC_Session session;

static TEEC_Result RegisterTA(const char *filename)
{
	int fd;
	struct stat f_stat;
	TEEC_SharedMemory shm;
	TEEC_Parameter parameter;
	TEEC_Result result = TEEC_SUCCESS;

	if (!filename)
		return TEEC_ERROR_BAD_PARAMETERS;

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		result = TEEC_ERROR_BAD_PARAMETERS;
		printf("failed to open TA file %s\n", filename);
		goto cleanup1;
	}

	fstat(fd, &f_stat);

	shm.size = f_stat.st_size;
	shm.flags = TEEC_MEM_INPUT;
	/* Use TEE Client API to allocate the underlying memory buffer. */
	result = TEEC_AllocateSharedMemory(
			&context,
			&shm);
	if (result != TEEC_SUCCESS) {
		printf("fail to ret=0x%08x\n", result);
		goto cleanup2;
	}

	read(fd, shm.buffer, shm.size);

	parameter.memref.parent = &shm;
	parameter.memref.size = shm.size;
	parameter.memref.offset = 0;

	result = TEEC_RegisterTA(&context, &parameter,
			TEEC_MEMREF_PARTIAL_INPUT);

	if (result != TEEC_SUCCESS)
		printf("fail to register TA, error=0x%08x\n", result);

cleanup3:
	TEEC_ReleaseSharedMemory(&shm);
cleanup2:
	close(fd);
cleanup1:
	return result;
}

int TEEU_Initialize(const char *taFile)
{
	TEEC_Result result = TEEC_SUCCESS;

	if (initialized)
		return result;

	/* [1] Connect to TEE */
	result = TEEC_InitializeContext(
			NULL,
			&context);
	if (result != TEEC_SUCCESS) {
		printf("fail to initalize TEE, error=0x%08x\n", result);
		goto cleanup1;
	}

	/* [2] Register TA if necessary
	 * if taFile==NULL, we assume it has been registered.
	 */
	if (taFile) {
		result = RegisterTA(taFile);
		if (result != TEEC_SUCCESS)
			goto cleanup2;
	}

	/* [3] Open session with TEE application */
	result = TEEC_OpenSession(
			&context,
			&session,
			&UtilTA_UUID,
			TEEC_LOGIN_USER,
			NULL,	/* No connection data needed for TEEC_LOGIN_USER. */
			NULL,	/* No payload, and do not want cancellation. */
			NULL);
	if (result != TEEC_SUCCESS) {
		printf("fail to open session, error=0x%08x\n", result);
		goto cleanup2;
	}

	initialized = true;

	return TEEC_SUCCESS;
cleanup2:
	TEEC_FinalizeContext(&context);
cleanup1:
	return result;
}

void TEEU_Finalize(void)
{
	if (!initialized)
		return;
	initialized = false;

	TEEC_CloseSession(&session);
	TEEC_FinalizeContext(&context);
}

static TEEC_Result RegisterSharedMemoryWithPhyAddr(TEEC_SharedMemory *shm,
		void *phyAddr, size_t size, uint32_t flags)
{
	/* must set buffer to NULL */
	shm->buffer = NULL;
	shm->phyAddr = phyAddr;
	shm->size = size;
	shm->flags = flags;

	return TEEC_RegisterSharedMemory(&context, shm);

}

int TEEU_MemCopy(void *dstPhyAddr, void *srcPhyAddr, unsigned long len)
{
	TEEC_Result result;
	TEEC_Operation operation;
	TEEC_SharedMemory in, out;

	if (!dstPhyAddr || !srcPhyAddr || len == 0) {
		printf("bad parameters: dst=%p, src=%p, len=%d\n",
				dstPhyAddr, srcPhyAddr, len);
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	result = RegisterSharedMemoryWithPhyAddr(&out, dstPhyAddr,
			len, TEEC_MEM_OUTPUT);
	if (result != TEEC_SUCCESS) {
		printf("failed to register destination shared memory, "
				"printf=0x%08x\n", result);
		return result;
	}

	result = RegisterSharedMemoryWithPhyAddr(&in, srcPhyAddr,
			len, TEEC_MEM_INPUT);
	if (result != TEEC_SUCCESS) {
		printf("failed to register source shared memory, "
				"printf=0x%08x\n", result);
		return result;
	}

	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_MEMREF_PARTIAL_INPUT,
			TEEC_MEMREF_WHOLE,
			TEEC_NONE,
			TEEC_NONE);

	operation.params[0].memref.parent = &in;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = len;

	operation.params[1].memref.parent = &out;
	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = 0;

	/* Start the echo operation within the TEE application. */
	result = TEEC_InvokeCommand(
			&session,
			CMD_MEM_COPY,
			&operation,
			NULL);

	TEEC_ReleaseSharedMemory(&in);
	TEEC_ReleaseSharedMemory(&out);

	if (result != TEEC_SUCCESS)
		printf("failed to copy from 0x%p to 0x%p, printf=0x%08x\n",
				srcPhyAddr, dstPhyAddr, result);

	return result;
}
