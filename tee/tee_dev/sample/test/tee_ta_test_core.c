/*
 * test tee core api
 */
#include "config.h"
#include "tee_ta_test.h"
#include "test_cmd.h"
#include "log.h"

TEE_Result TestPanic(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_VALUE_INPUT
			|| TEE_PARAM_TYPE_GET(paramTypes, 1) != TEE_PARAM_TYPE_VALUE_OUTPUT)
		return TEE_ERROR_BAD_PARAMETERS;

	TEE_Panic(TEE_ERROR_BAD_STATE);

	return TEE_SUCCESS;
}

TEE_Result TestTAClient(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	const TEE_UUID uuid = TATEST_UUID;
	TEE_Result result = TEE_SUCCESS;
	uint32_t origin = 0;
	TEE_TASessionHandle session = NULL;
	uint32_t cmd;
	uint32_t pt = 0;
	TEE_Param p[4];

	static uint32_t a = 0, b = 0;
	a += 1;
	b += 100;

	result = TEE_OpenTASession(&uuid, 0, pt, p, &session, &origin);
	if (result != TEE_SUCCESS) {
		error("fail to open session. result=0x%08x, origin=%d\n",
				result, origin);
		goto out;
	}

	cmd = TEST_CMD_ADD;
	pt = TEE_PARAM_TYPES(
			TEE_PARAM_TYPE_VALUE_INPUT,
			TEE_PARAM_TYPE_VALUE_OUTPUT,
			TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE);
	p[0].value.a = a;
	p[0].value.b = b;
	p[1].value.a = 0xdeadbeef;

	result = TEE_InvokeTACommand(session, 0, cmd, pt, p, &origin);

	if (result != TEE_SUCCESS) {
		error("fail to invoke command. result=0x%08x, origin=%d\n",
				result, origin);
	}

	if (p[1].value.a != (a + b))
		error("%d + %d = %d is not correct\n", a, b, p[1].value.a);

	TEE_CloseTASession(session);

out:
	return result;
}
