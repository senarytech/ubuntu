#include "config.h"
#include "tee_ca_test_common.h"
#include "log.h"

int TestRawCommand(uint32_t commandID)
{
	TEEC_Session *session = TestGetSession();

	TEEC_Result result;

	/* Start the arithmetic operation within the TEE application. */
	result = TEEC_InvokeCommand(
			session,
			commandID,
			NULL,
			NULL);

	return result;
}

int TestCallback(void)
{
	return TestRawCommand(TEST_CMD_CALLBACK);
}

int TestMemRegion(void)
{
	return TestRawCommand(TEST_CMD_MEMREGION);
}

int TestMemTransfer(void)
{
	return TestRawCommand(TEST_CMD_TRANSFER);
}

int TestMutex(void)
{
	return TestRawCommand(TEST_CMD_MUTEX);
}
