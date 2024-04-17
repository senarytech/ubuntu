#include "tee_client_api.h"
#include "tee_ta_entry.h"
#include "tee_ca_rpmb_perf.h"
#include "string.h"
#include "memory.h"
#include "log.h"

#define RPMB_PERF_TA_FILE_NAME "/home/galois/ta/librpmbperf.ta"

static const TEEC_UUID TA_RPMB_PERF_UUID = RPMB_PERF_UUID;
static TEEC_Context context;
static TEEC_Session session;

int rpmbPerfInitialize(void)
{
	TEEC_Result result;
	int tried_flag = 0;

	result = TEEC_InitializeContext( NULL, &context);
	if (result != TEEC_SUCCESS) {
		error("TEEC_InitializeContext failed with result =0x%08x\n", result);
		goto cleanup1;
	}

retry:
	/* Open a Session with the TEE application. */
	result = TEEC_OpenSession(
			&context,
			&session,
			&TA_RPMB_PERF_UUID,
			TEEC_LOGIN_USER,
			NULL,
			NULL,
			NULL);
	if (result != TEEC_SUCCESS) {
		if (tried_flag == 0) {
			TEEC_LoadTA(&context, RPMB_PERF_TA_FILE_NAME);
			tried_flag = 1;
			goto retry;
		} else {
			error("TEEC_OpenSession failed with result = 0x%8x\n", result);
			goto cleanup2;
		}
	}

	return result;

cleanup2:
	TEEC_FinalizeContext(&context);
cleanup1:
	return result;
}

void rpmbPerfFinalize(void)
{
	TEEC_CloseSession(&session);
	TEEC_FinalizeContext(&context);
}

int rpmbPerfTest(int op_mode)
{
	TEEC_Result result;
	TEEC_Operation operation = {0};
	uint32_t cmdID = 0;

	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_VALUE_INOUT,
			TEEC_NONE,
			TEEC_NONE,
			TEEC_NONE);
	if(op_mode == 1)
		cmdID = RPMB_PERF_WRITE;
	else
		cmdID = RPMB_PERF_READ;

	/* Start the operation within the TEE application. */
	result = TEEC_InvokeCommand(
			&session,
			cmdID,
			&operation,
			NULL);

	if (result != TEEC_SUCCESS)
	{
		printf("rpmb perf test failed\n");
		return result;
	}

	printf("rpmb perf test success\n");
	if(op_mode == 1)
		printf("rpmb write perf: %d ms\n", operation.params[0].value.a);
	else
		printf("rpmb read perf: %d ms\n", operation.params[0].value.a);

	return 0;
}
