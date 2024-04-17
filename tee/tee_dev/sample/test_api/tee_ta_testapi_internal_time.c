/*
 * test tee core api
 */
#include "tee_testapi.h"
#include "testapi_cmd.h"
#include "tee_common.h"
#include "tee_internal_core_common.h"
#include "tee_internal_time_api.h"
static const TEE_UUID TAFake_UUID = TAFAKE_UUID;
static const TEE_UUID TATest_UUID = TATEST_UUID;



void TestInternalGetSysTime(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		case_index = 0;
	bool			result = false;

	struct MODULE_Cfg	*test_cfg;
	int			timeout = 0;

	TEE_Time		systime1, systime2;


	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;


	/*case: normal case*/
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	TEE_GetSystemTime(&systime1);
	TEE_GetSystemTime(&systime2);





	if(systime2.seconds < systime1.seconds){
		printf("tee get system time not meet requirement.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}else if(systime2.seconds == systime1.seconds
			&& systime2.millis < systime1.millis) {
		printf("tee get system time not meet requirement.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;

	}

	return;

}

void TestInternalGetReeTime(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		case_index = 0;
	bool			result = false;

	struct MODULE_Cfg	*test_cfg;
	int			timeout = 0;
	TEE_Time		systime;

	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;

	/*case: normal case*/
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	TEE_GetREETime(&systime);

	if(systime.seconds - params[1].value.a > 3) {
		printf("tee get ree time fail, because the gap beyond 10s.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;

	}
	return;
}

void TestInternalTimeWait(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		case_index = 0;
	bool			result = false;

	struct MODULE_Cfg	*test_cfg;
	int			diff_ms = 0;
	TEE_Time		start,end;

	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;

	/*case: normal case*/
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	TEE_GetSystemTime(&start);

	TEE_Wait(100);

	TEE_GetSystemTime(&end);

	diff_ms = (end.seconds - start.seconds) * 1000 + end.millis - start.millis;
	printf("end(%d, %d), start(%d, %d).\n", end.seconds, end.millis,
			start.seconds, start.millis);
	if(diff_ms > 110 || diff_ms < 100) {
		printf("tee time wait 100 ms fail, diff_ms %d does not meet the \
				requirement\n", diff_ms);
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	return;


}

void TestInternalSetPersistTime(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		case_index = 0;
	bool			result = false;

	struct MODULE_Cfg	*test_cfg;
	int			timeout = 0;
	TEE_Time		start;

	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;

	/*case: normal case*/

	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	start.seconds = 0xffffffff - 10;
	start.millis = 0;

	result = TEE_SetTAPersistentTime(&start);

	if(TEE_SUCCESS != result){
		printf("tee set persist time fail.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}
	TEE_Wait(20000);

	result = TEE_GetTAPersistentTime(&start);
	if(TEE_ERROR_OVERFLOW != result){
		printf("tee get persist time overflow fail. (%x, %x)\n",
				start.seconds, start.millis);
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}



	return;


}

void TestInternalGetPersistTime(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		case_index = 0;
	bool			result = false;

	struct MODULE_Cfg	*test_cfg;
	int			timeout = 0;
	TEE_Time		start,end;

	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;

	/*case 1: get before set*/
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	result = TEE_GetTAPersistentTime(&start);
	if(TEE_ERROR_TIME_NOT_SET != result){
		printf("tee get persist time before set fail. ret = %x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	/*case 2: normal case*/
	case_index++;
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	start.seconds = start.millis = 0;
	result = TEE_SetTAPersistentTime(&start);

	if(TEE_SUCCESS != result){
		printf("tee set persist time fail.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}
	TEE_Wait(100);

	result = TEE_GetTAPersistentTime(&end);
	if(TEE_SUCCESS != result){
		printf("tee get persist time fail.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}



	return;


}


TEE_Result TestInternalTime(
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
	printf("test internal time, id is %d\n", commandID);
	switch(commandID) {

	case TEST_TEE_INTERNAL_GET_SYSTIME:
		TestInternalGetSysTime(sessionContext, paramTypes, params);
		break;
	case TEST_TEE_INTERNAL_GET_REETIME:
		if (TEE_PARAM_TYPE_GET(paramTypes, 1) != TEE_PARAM_TYPE_VALUE_INPUT) {
			printf("input test parameter error, can't continue....\n");
			return TEE_ERROR_BAD_PARAMETERS;
		}
		TestInternalGetReeTime(sessionContext, paramTypes, params);
		break;

	case TEST_TEE_INTERNAL_TIME_WAIT:
		TestInternalTimeWait(sessionContext, paramTypes, params);
		break;

	case TEST_TEE_INTERNAL_SET_PERSIST_TIME:
		TestInternalSetPersistTime(sessionContext, paramTypes, params);
		break;
	case TEST_TEE_INTERNAL_GET_PERSIST_TIME:
		TestInternalGetPersistTime(sessionContext, paramTypes, params);
		break;
	default:
		break;

	}

	return TEE_SUCCESS;
}
