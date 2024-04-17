/*
 * test tee core api
 */
#include "tee_testapi.h"
#include "testapi_cmd.h"
#include "tee_common.h"
#include "tee_internal_core_common.h"

static const TEE_UUID TAFake_UUID = TAFAKE_UUID;
static const TEE_UUID TATest_UUID = TATEST_UUID;



void TestCoreClientOpenSession(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		cmdId = TEST_CMD_ADD;
	uint32_t		case_index = 0;
	TEE_Result		result;
	uint32_t 		cancellationTimeout = 0;
	uint32_t 		core_paramTypes;
	TEE_Param		core_params[4] = {0};
	uint32_t		returnOrigin;

	struct MODULE_Cfg	*test_cfg;


	TEE_TASessionHandle session;


	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;
	core_paramTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE);


	/*case: parameter error*/
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	result = TEE_OpenTASession(&TAFake_UUID, cancellationTimeout,
			core_paramTypes, core_params, &session, &returnOrigin);

	if(result == TEE_SUCCESS){
		printf("parameter error: open a fake session sucess?!,check it.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		TEE_CloseTASession(session);

	}
	/*case: normal case*/
	case_index++;
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	result = TEE_OpenTASession(&TATest_UUID, cancellationTimeout,
			core_paramTypes, core_params, &session, &returnOrigin);

	if(result != TEE_SUCCESS){
		printf("normal case: open session fail.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		goto cleanup;
	}


	TEE_CloseTASession(session);


cleanup:
	return;


}

void TestCoreClientCloseSession(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		cmdId = TEST_CMD_ADD;
	uint32_t		case_index = 0;
	TEE_Result		result;
	uint32_t 		cancellationTimeout = 0;
	uint32_t 		core_paramTypes;
	TEE_Param		core_params[4] = {0};
	uint32_t		returnOrigin;

	TEE_TASessionHandle 	session;
	struct MODULE_Cfg	*test_cfg;


	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;

	core_paramTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE);



	/* init the session.*/
	result = TEE_OpenTASession(&TATest_UUID, cancellationTimeout,
			core_paramTypes, core_params, &session, &returnOrigin);

	if(result != TEE_SUCCESS){
		printf(" init the session fail. can't continue...\n");
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup;
	}

	/*case: normal case*/

	TEE_CloseTASession(session);

	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

#if 0
	/* because of the assert in the API, we can't do the verify */

	/* invoke a command to verify if this seesion is closed?*/


	core_paramTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
			TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE);
	core_params[0].value.a = 3;
	core_params[0].value.b = 5;

	cmdId = TEST_CMD_ADD;



	result = TEE_InvokeTACommand(session, cancellationTimeout, cmdId,
			core_paramTypes, core_params,&returnOrigin);

	if(result == TEE_SUCCESS){
		printf("normal case: tee_invoke success, but we have close \
			the session. add result is %d\n",
			core_params[1].value.a);
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}
#endif

cleanup:
	return;


}

void TestCoreClientInvokeCmd(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		cmdId = TEST_CMD_ADD;
	uint32_t		case_index = 0;
	TEE_Result		result;
	uint32_t 		cancellationTimeout = 0;
	uint32_t 		core_paramTypes;
	TEE_Param		core_params[4] = {0};
	uint32_t		returnOrigin;

	TEE_TASessionHandle 	session;
	struct MODULE_Cfg	*test_cfg;



	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;

	core_paramTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE);



	result = TEE_OpenTASession(&TATest_UUID, cancellationTimeout,
			core_paramTypes, core_params, &session, &returnOrigin);

	if(result != TEE_SUCCESS){
		printf("init the session fail, can't continue....\n");
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup;
	}

	/*case: parameter types error*/

	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	core_paramTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
			TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE);
	core_params[0].value.a = 3;
	core_params[0].value.b = 5;

	cmdId = TEST_CMD_ADD;



	result = TEE_InvokeTACommand(session, cancellationTimeout, cmdId,
			core_paramTypes, core_params,&returnOrigin);


	if(result == TEE_SUCCESS){
		printf("parameter error: invoke command success, but with \
				the error parameter types.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	/*case: parameter cmdID error*/
	case_index++;
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	core_paramTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
			TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE);
	core_params[0].value.a = 3;
	core_params[0].value.b = 5;

	cmdId = 100;



	result = TEE_InvokeTACommand(session, cancellationTimeout, cmdId,
			core_paramTypes, core_params,&returnOrigin);


	if(result == TEE_SUCCESS){
		printf("parameter error: invoke command success, but with \
				the error parameter cmd_id.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	/*case: normal case*/
	case_index++;
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	core_paramTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
			TEE_PARAM_TYPE_VALUE_OUTPUT, TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE);
	core_params[0].value.a = 3;
	core_params[0].value.b = 5;

	cmdId = TEST_CMD_ADD;



	result = TEE_InvokeTACommand(session, cancellationTimeout, cmdId,
			core_paramTypes, core_params,&returnOrigin);


	if(result != TEE_SUCCESS){
		printf("nomal case: invoke command fail.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	TEE_CloseTASession(session);

cleanup:
	return;


}

TEE_Result TestCoreClient(
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

	switch(commandID) {

	case TEST_TEE_CORE_OPENSESSION:
		TestCoreClientOpenSession(sessionContext, paramTypes, params);
		break;

	case TEST_TEE_CORE_CLOSESESSION:
		TestCoreClientCloseSession(sessionContext, paramTypes, params);
		break;

	case TEST_TEE_CORE_INVOKECMD:
		TestCoreClientInvokeCmd(sessionContext, paramTypes, params);
		break;
	default:
		break;

	}

	return TEE_SUCCESS;
}
