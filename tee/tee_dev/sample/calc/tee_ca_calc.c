#include "config.h"
#include "tee_ca_calc.h"
#include "tee_client_api.h"
#include "calc_cmd.h"
#include "string.h"
#include "memory.h"
#include "log.h"

#ifdef __LINUX__
#define get_cpu_id()		(0)
#endif /* __LINUX__ */

typedef struct PercpuContext {
	bool initialized;
	TEEC_Session session;
	TEEC_SharedMemory shm;
} PercpuContext;

static const TEEC_UUID TACalc_UUID = TACALC_UUID;

static bool initialized = false;
static TEEC_Context context;
static PercpuContext percpuContext[CONFIG_CPU_NUM];

int CalcPercpuInitialize(void)
{
	TEEC_Result result;

	int cpuId = get_cpu_id();
	TEEC_Session *session = &percpuContext[cpuId].session;
	TEEC_SharedMemory *shm = &percpuContext[cpuId].shm;

	percpuContext[cpuId].initialized = true;

	/* ========================================================================
	   [2] Open session with TEE application
	   ======================================================================== */
	/* Open a Session with the TEE application. */
	result = TEEC_OpenSession(
			&context,
			session,
			&TACalc_UUID,
			TEEC_LOGIN_USER,
			NULL,	/* No connection data needed for TEEC_LOGIN_USER. */
			NULL,	/* No payload, and do not want cancellation. */
			NULL);
	if (result != TEEC_SUCCESS) {
		error("ret=0x%08x\n", result);
		goto cleanup2;
	}

	/* ========================================================================
	   [3] Initialize the Shared Memory buffers
	   ======================================================================== */
	/* always clear it before allocation shm or register shm */
	memset(shm, 0, sizeof(*shm));
	shm->size = 1024;
	shm->flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
	/* Use TEE Client API to allocate the underlying memory buffer. */
	result = TEEC_AllocateSharedMemory(
			&context,
			shm);
	if (result != TEEC_SUCCESS) {
		error("ret=0x%08x\n", result);
		goto cleanup3;
	}

	return TEEC_SUCCESS;

cleanup3:
	TEEC_CloseSession(session);
cleanup2:
	return result;
}

int CalcInitialize(void)
{
	TEEC_Result result;

	if (!initialized) {
		initialized = true;

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
	}

	result = CalcPercpuInitialize();

	if (result != TEEC_SUCCESS) {
		error("ret=0x%08x\n", result);
		goto cleanup2;
	}

	return result;

cleanup2:
	TEEC_FinalizeContext(&context);
cleanup1:
	return result;
}

void CalcPercpuFinalize(void)
{
	int cpuId = get_cpu_id();
	TEEC_Session *session = &percpuContext[cpuId].session;
	TEEC_SharedMemory *shm = &percpuContext[cpuId].shm;

	if (percpuContext[cpuId].initialized) {
		TEEC_ReleaseSharedMemory(shm);
		TEEC_CloseSession(session);
		memset(&percpuContext[cpuId], 0, sizeof(percpuContext[cpuId]));
		percpuContext[cpuId].initialized = false;
	}
}


void CalcFinalize(void)
{
	if (!initialized)
		return;
	initialized = false;

	CalcPercpuFinalize();
	TEEC_FinalizeContext(&context);
}

static int CalcExpect(int cmdId, int a, int b)
{
	int r = 0x12345678;
	switch (cmdId) {
	case CALC_CMD_INC:
		r = a + 1;
		break;
	case CALC_CMD_DEC:
		r = a - 1;
		break;
	case CALC_CMD_ADD:
		r = a + b;
		break;
	case CALC_CMD_SUB:
		r = a - b;
		break;
	case CALC_CMD_MUL:
		r = a * b;
		break;
	case CALC_CMD_DIV:
		r = a / b;
		break;
	}
	return r;
}

static void CalcDump(int cmdId, int a, int b, int result)
{
#if 0
	const char * cmd[] = {"inc", "dec", "add", "sub", "mul", "div"};
	int expected = CalcExpect(cmdId, a, b);

	printf("\t%s(%d, %d) = %d, expected %d, %s\n", cmd[cmdId], a, b,
		result, expected, (result == expected) ? "PASS" : "FAIL");
#endif
}

static int CalcOperate(int cmdId, int a, int b)
{
	int cpuId = get_cpu_id();
	TEEC_Session *session = &percpuContext[cpuId].session;

	TEEC_Result result;
	TEEC_Operation operation = {0};
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
			session,
			cmdId,
			&operation,
			NULL);

	CalcDump(cmdId, a, b, operation.params[1].value.a);

	/*
	 * get the result
	 * FIXME: need handle the error (result != TEEC_SUCCESS)
	 */
	return operation.params[1].value.a;
}

int CalcAdd(int a, int b)
{
	return CalcOperate(CALC_CMD_ADD, a, b);
}
int CalcSub(int a, int b)
{
	return CalcOperate(CALC_CMD_SUB, a, b);
}
int CalcMul(int a, int b)
{
	return CalcOperate(CALC_CMD_MUL, a, b);
}
int CalcDiv(int a, int b)
{
	return CalcOperate(CALC_CMD_DIV, a, b);
}
int CalcInc(int a)
{
	return CalcOperate(CALC_CMD_INC, a, 0);
}
int CalcDec(int a)
{
	return CalcOperate(CALC_CMD_DEC, a, 0);
}

int CalcEcho(const char *str)
{
	int cpuId = get_cpu_id();
	TEEC_Session *session = &percpuContext[cpuId].session;
	TEEC_SharedMemory *shm = &percpuContext[cpuId].shm;

	int size = 0;
	TEEC_Result result;
	TEEC_Operation operation = {0};

	if (!str)
		return size;

	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_MEMREF_WHOLE,
			TEEC_NONE,
			TEEC_NONE,
			TEEC_NONE);

	strncpy(shm->buffer, str, shm->size);

	operation.params[0].memref.parent = shm;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 0;

	/* Start the echo operation within the TEE application. */
	result = TEEC_InvokeCommand(
			session,
			CALC_CMD_ECHO,
			&operation,
			NULL);
	/* get the result */
	if (result == TEEC_SUCCESS)
		size = operation.params[0].memref.size;

	return size;
}

int CalcReverse(void *out, const void *in, int len)
{
	int cpuId = get_cpu_id();
	TEEC_Session *session = &percpuContext[cpuId].session;
	TEEC_SharedMemory *shm = &percpuContext[cpuId].shm;

	int size = 0;
	TEEC_Result result;
	TEEC_Operation operation = {0};

	if (!in || !out || len > shm->size)
		return size;

	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_VALUE_INOUT,
			TEEC_MEMREF_PARTIAL_INOUT,
			TEEC_NONE,
			TEEC_NONE);

	memcpy(shm->buffer, in, len);

	operation.params[0].value.a = len;
	operation.params[0].value.b = 0;

	operation.params[1].memref.parent = shm;
	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = len;

	/* Start the echo operation within the TEE application. */
	result = TEEC_InvokeCommand(
			session,
			CALC_CMD_REVERSE,
			&operation,
			NULL);
	/* get the result */
	if (result == TEEC_SUCCESS) {
		size = operation.params[0].value.b;
		memcpy(out, shm->buffer, len);
	}

	return size;
}

int CalcConvert(char *buf, int a, int base)
{
	int cpuId = get_cpu_id();
	TEEC_Session *session = &percpuContext[cpuId].session;
	TEEC_SharedMemory *shm = &percpuContext[cpuId].shm;

	int size = 0;
	TEEC_Result result;
	TEEC_Operation operation = {0};

	if (!buf)
		return 0;

	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_VALUE_INPUT,
			TEEC_MEMREF_PARTIAL_OUTPUT,
			TEEC_NONE,
			TEEC_NONE);

	operation.params[0].value.a = a;
	operation.params[0].value.b = base;

	operation.params[1].memref.parent = shm;
	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = 64;

	/* Start the convert operation within the TEE application. */
	result = TEEC_InvokeCommand(
			session,
			CALC_CMD_CONVERT,
			&operation,
			NULL);

	/* get the result */
	if (result == TEEC_SUCCESS) {
		size = operation.params[1].memref.size;
		if (size > 0)
			memcpy(buf, shm->buffer, size);
	}

	return size;
}
