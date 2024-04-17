/*
 * test tee core api
 */
#include "tee_testapi.h"
#include "testapi_cmd.h"
#include "tee_common.h"
#include "tee_internal_core_common.h"

static const TEE_UUID TAFake_UUID = TAFAKE_UUID;
static const TEE_UUID TATest_UUID = TATEST_UUID;



void TestCoreCancelGetFlag(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		case_index = 0;
	bool			result = false;
	uint32_t 		cancellationTimeout = 0;

	struct MODULE_Cfg	*test_cfg;
	int			timeout = 0;



	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;


	/*case: normal case*/
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	while(timeout++ < 10){
		result = TEE_GetCancellationFlag();
		if(result)
			break;

		mdelay(1000);
	}

	if(false == result){
		printf("tee get cancel flag fail, 10s timeout.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	return;

}

void TestCoreCancelMask(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		case_index = 0;
	bool			result = false;
	uint32_t 		cancellationTimeout = 0;

	struct MODULE_Cfg	*test_cfg;
	int			timeout = 0;

	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;

	/*case: normal case*/
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	while(timeout++ < 10){
		result = TEE_GetCancellationFlag();
		if(result)
			break;

		mdelay(1000);
	}

	if(false == result){
		printf("tee get cancel flag fail, 10s timeout.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	TEE_MaskCancellation();

	result = TEE_GetCancellationFlag();

	if(false != result){
		printf("tee mask cancel flag fail.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}


cleanup:
	return;


}

void TestCoreCancelUnmask(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		case_index = 0;
	bool			result = false;
	uint32_t 		cancellationTimeout = 0;

	struct MODULE_Cfg	*test_cfg;
	int			timeout = 0;

	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;

	/*case: normal case*/
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	TEE_MaskCancellation();

	result = TEE_GetCancellationFlag();

	if(false != result){
		printf("tee mask cancel flag fail.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}


	TEE_UnmaskCancellation();

	while(timeout++ < 10){
		result = TEE_GetCancellationFlag();
		if(result)
			break;

		mdelay(1000);
	}

	if(false == result){
		printf("tee get cancel flag fail, after unmask and 10s timeout.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}



cleanup:
	return;


}


TEE_Result TestCoreCancel(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	uint32_t		flag;

	uint32_t		commandID;

	struct MODULE_Cfg	*test_cfg;


	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_MEMREF_INOUT) {
		printf("input test parameter error, can't continue....\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}


	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;
	commandID = test_cfg->type;
	printf("test core cancel, id is %d\n", commandID);
	switch(commandID) {

	case TEST_TEE_CORE_CANCEL_GETFLAG:
		TestCoreCancelGetFlag(sessionContext, paramTypes, params);
		break;

	case TEST_TEE_CORE_CANCEL_UNMASK:
		TestCoreCancelUnmask(sessionContext, paramTypes, params);
		break;

	case TEST_TEE_CORE_CANCEL_MASK:
		TestCoreCancelMask(sessionContext, paramTypes, params);
		break;
	default:
		break;

	}

	return TEE_SUCCESS;
}
