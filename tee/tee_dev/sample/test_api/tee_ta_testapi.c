#include "testapi_cmd.h"
#include "tee_internal_api.h"
#include "config.h"
#include "log.h"

static uint32_t totalTATestSessionNum = 0;
static uint32_t totalTATestSessionHandle = 0;

TEE_Result TA_EXPORT TA_CreateEntryPoint(void)
{
	trace("\n");
	return TEE_SUCCESS;
}

void TA_EXPORT TA_DestroyEntryPoint(void)
{
	trace("\n");
}

TEE_Result TA_EXPORT TA_OpenSessionEntryPoint(
	uint32_t		paramTypes,
	TEE_Param		params[4],
	void**			sessionContext)
{
	totalTATestSessionNum++;
	totalTATestSessionHandle++;
	*sessionContext = (void *)totalTATestSessionHandle;

	trace("total session %d\n", totalTATestSessionNum);
	return TEE_SUCCESS;
}

void TA_EXPORT TA_CloseSessionEntryPoint(void*  sessionContext)
{
	if (totalTATestSessionNum > 0)
		totalTATestSessionNum--;

	trace("total session %d\n", totalTATestSessionNum);
}

void TestDump(int cmdId, int a, int b, int result)
{
	const char * cmd[] = {"inc", "dec", "add", "sub", "mul",
				"div", "mem", "reverse"};

	printf("\t%s(%d, %d) = 0x%08x\n", cmd[cmdId], a, b, result);
}

TEE_Result TA_EXPORT TA_InvokeCommandEntryPoint(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	TEE_Result res = TEE_ERROR_NOT_SUPPORTED;

	trace("commandID=%d, paramTypes=0x%08x\n", commandID, paramTypes);

	switch (commandID) {
	case TEST_CMD_INC:
	case TEST_CMD_DEC:
	case TEST_CMD_ADD:
	case TEST_CMD_SUB:
	case TEST_CMD_MUL:
	case TEST_CMD_DIV:
	case TEST_CMD_CANCELLATION:
		res = TestOperate(sessionContext, commandID, paramTypes, params);
		break;
	case TEST_CMD_MEM:
		res = TestMem(sessionContext, paramTypes, params);
		break;
	case TEST_CMD_REVERSE:
		res = TestReverse(sessionContext, paramTypes, params);
		break;
	case TEST_CMD_CACHEFLUSH:
		res = TestCache(sessionContext, paramTypes, params);
		break;
	case TEST_CMD_CORE_CLIENT:
		res = TestCoreClient(sessionContext,paramTypes, params);
		break;
	case TEST_CMD_CORE_MEM:
		res = TestCoreMem(sessionContext,paramTypes, params);
		break;
	case TEST_CMD_CORE_CANCEL:
		res = TestCoreCancel(sessionContext,paramTypes, params);
		break;
	case TEST_CMD_INTERNAL_TIME:
		res = TestInternalTime(sessionContext,paramTypes, params);
		break;
	case TEST_CMD_CORE_CRYPTO:
		res = TestCoreCrypto(sessionContext,paramTypes, params);
		break;
	case TEST_CMD_CORE_PROPERTY:
		res = TestCoreProperty(sessionContext,paramTypes, params);
		break;
	}

#if 0
	TestDump(commandID, params[0].value.a, params[0].value.b, params[1].value.a);
#endif
	return res;
}

TA_DEFINE_BEGIN
	.uuid			= TATEST_UUID,
	.name			= "TestApi",
	.singleInstance		= false,
	.multiSession		= false,
	.instanceKeepAlive	= false,
TA_DEFINE_END
