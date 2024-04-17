#include "config.h"
#include "tee_ca_sample.h"
#include "tee_client_api.h"
#include "sample_cmd.h"
#include "log.h"

static const TEEC_UUID SampleTA_UUID = TA_UUID;

static bool initialized = false;
static TEEC_Context context;
static TEEC_Session session;

int Initialize(void)
{
	TEEC_Result result = TEEC_SUCCESS;

	if (initialized)
		return result;

	/* ========================================================================
	   [1] Connect to TEE
	   ======================================================================== */
	result = TEEC_InitializeContext(
			NULL,
			&context);
	if (result != TEEC_SUCCESS) {
		error("ret=0x%08x\n", result);
		goto cleanup1;
	}

	/* ========================================================================
	   [2] Open session with TEE application
	   ======================================================================== */
	/* Open a Session with the TEE application. */
	result = TEEC_OpenSession(
			&context,
			&session,
			&SampleTA_UUID,
			TEEC_LOGIN_USER,
			NULL,	/* No connection data needed for TEEC_LOGIN_USER. */
			NULL,	/* No payload, and do not want cancellation. */
			NULL);
	if (result != TEEC_SUCCESS) {
		error("ret=0x%08x\n", result);
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
		error("ret=0x%08x\n", result);
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

int OperateValue(int cmdId, int *out, int a, int b)
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

int OperateMemory(int cmdId, void *outShm, void *inShm, int len)
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

	/* get the result */
	if (result == TEEC_SUCCESS)
		return operation.params[1].memref.size;

	return result;
}
