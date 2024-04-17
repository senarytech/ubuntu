#include "config.h"
#include "tee_ca_test_common.h"
#include "log.h"

static int TestExpect(int cmdId, int a, int b)
{
	int r = 0x12345678;
	switch (cmdId) {
	case TEST_CMD_INC:
		r = a + 1;
		break;
	case TEST_CMD_DEC:
		r = a - 1;
		break;
	case TEST_CMD_ADD:
		r = a + b;
		break;
	case TEST_CMD_SUB:
		r = a - b;
		break;
	case TEST_CMD_MUL:
		r = a * b;
		break;
	case TEST_CMD_DIV:
		r = a / b;
		break;
	}
	return r;
}

static void TestDump(int cmdId, int a, int b, int result)
{
#if 0
	const char * cmd[] = {"inc", "dec", "add", "sub", "mul", "div"};
	int expected = TestExpect(cmdId, a, b);

	printf("\t%s(%d, %d) = %d, expected %d, %s\n", cmd[cmdId], a, b,
		result, expected, (result == expected) ? "PASS" : "FAIL");
#endif
}

static int TestOperate(int cmdId, int a, int b)
{
	TEEC_Session *session = TestGetSession();

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

	TestDump(cmdId, a, b, operation.params[1].value.a);

	/*
	 * get the result
	 * FIXME: need handle the error (result != TEEC_SUCCESS)
	 */
	return operation.params[1].value.a;
}

int TestAdd(int a, int b)
{
	return TestOperate(TEST_CMD_ADD, a, b);
}
int TestSub(int a, int b)
{
	return TestOperate(TEST_CMD_SUB, a, b);
}
int TestMul(int a, int b)
{
	return TestOperate(TEST_CMD_MUL, a, b);
}
int TestDiv(int a, int b)
{
	return TestOperate(TEST_CMD_DIV, a, b);
}
int TestInc(int a)
{
	return TestOperate(TEST_CMD_INC, a, 0);
}
int TestDec(int a)
{
	return TestOperate(TEST_CMD_DEC, a, 0);
}

int TestMem(void *shm_handle, unsigned int pattern)
{
	TEEC_Session *session = TestGetSession();
	TEEC_SharedMemory *shm = shm_handle;

	TEEC_Result result;
	TEEC_Operation operation = {0};

	assert(shm);
	assert(shm->buffer);
	assert(shm->size);

	uint32_t i;
	uint32_t count = shm->size / sizeof(uint32_t);
	uint32_t *word = shm->buffer;

	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_VALUE_INOUT,
			TEEC_MEMREF_WHOLE,
			TEEC_NONE,
			TEEC_NONE);

	/* generate inputs */
	for (i = 0; i < count; i++)
		word[i] = (pattern == 0) ? ((uint32_t)shm->phyAddr + i * 4) : pattern;

	operation.params[0].value.a = pattern;
	operation.params[0].value.b = 0;
	operation.params[1].memref.parent = shm;
	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = 0;

	/* Start the echo operation within the TEE application. */
	result = TEEC_InvokeCommand(
			session,
			TEST_CMD_MEM,
			&operation,
			NULL);

	/* verity the result */
	if (result != TEEC_SUCCESS) {
		error("verify fail! result is not success: %d\n", result);
		return result;
	}

	/* verify outputs */
	for (i = 0; i < count; i++) {
		uint32_t got = word[i];
		uint32_t expect = ~((pattern == 0) ? ((uint32_t)shm->phyAddr + i * 4) : pattern);
		if (got != expect) {
			error("input data error @ %d word, 0x%08x != ~0x%08x\n",
					i, got, expect);
			return TEEC_ERROR_COMMUNICATION;
		}
	}

	return TEEC_SUCCESS;
}

int TestReverse(void *shm_handle)
{
	TEEC_Session *session = TestGetSession();

	TEEC_SharedMemory *shm = shm_handle;

	assert(shm);
	assert(shm->buffer);
	assert(shm->size);

	TEEC_Result result;
	TEEC_Operation operation = {0};

	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_VALUE_INOUT,
			TEEC_MEMREF_PARTIAL_INOUT,
			TEEC_NONE,
			TEEC_NONE);

	operation.params[0].value.a = shm->size;
	operation.params[0].value.b = 0;

	operation.params[1].memref.parent = shm;
	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = shm->size;

	/* Start the echo operation within the TEE application. */
	result = TEEC_InvokeCommand(
			session,
			TEST_CMD_REVERSE,
			&operation,
			NULL);
	/* get the result */
	if (result != TEEC_SUCCESS)
		error("error code 0x%08x\n", result);

	return result;
}
