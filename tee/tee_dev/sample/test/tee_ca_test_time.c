#include "config.h"
#include "tee_ca_test_common.h"
#include "log.h"

int TestTime(void)
{
	TEEC_Session *session = TestGetSession();

	TEEC_Result result;
	TEEC_Operation operation = {0};
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_VALUE_OUTPUT,
			TEEC_NONE,
			TEEC_NONE,
			TEEC_NONE);

	/* Start the arithmetic operation within the TEE application. */
	result = TEEC_InvokeCommand(
			session,
			TEST_CMD_TIME,
			&operation,
			NULL);

	return result;
}
