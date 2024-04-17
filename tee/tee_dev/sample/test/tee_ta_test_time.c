/*
 * test tee time api
 */
#include "config.h"
#include "tee_ta_test.h"
#include "log.h"

TEE_Result Test_TEE_Wait(void)
{
	int i;
	for (i = 0; i < 10; i++) {
		TEE_Wait(i * 1000);
		info("wait %d ms\n", i * 1000);
	}

	return TEE_SUCCESS;
}

TEE_Result Test_TEE_GetREETime(void)
{
	TEE_Time time;

	TEE_GetREETime(&time);

	trace("%d.%d\n", time.seconds, time.millis);

	return TEE_SUCCESS;
}

TEE_Result Test_TEE_Time(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
#if 0
	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_VALUE_INPUT
			|| TEE_PARAM_TYPE_GET(paramTypes, 1) != TEE_PARAM_TYPE_VALUE_OUTPUT)
		return TEE_ERROR_BAD_PARAMETERS;
#endif

	TEE_Time start, end;
	uint32_t wait_ms = 100;
	uint32_t diff_ms = 0;

	TEE_GetREETime(&start);
	TEE_Wait(wait_ms);
	TEE_GetREETime(&end);

	diff_ms = (end.seconds - start.seconds) * 1000 + end.millis - start.millis;

	/* diff_ms should be (wait_ms-50, wait_ms+50) */
	if (diff_ms < (wait_ms - 50) || diff_ms > (wait_ms + 50)) {
#if 0
		error("diff (%dms) is too large. start %d.%ds, wait %dms, end %d.%ds.\n",
				diff_ms, start.seconds, start.millis,
				wait_ms, end.seconds, end.millis);
#endif
		return TEE_ERROR_TIMEOUT;
	}
	return TEE_SUCCESS;
}
