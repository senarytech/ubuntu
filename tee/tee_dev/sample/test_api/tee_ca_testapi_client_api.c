#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include "config.h"
#include "tee_testapi.h"
#include "tee_client_api.h"
#include "testapi_cmd.h"
#include "string.h"
#include "memory.h"
#include "log.h"
#include "tee_ca_testapi_util.h"

static const TEEC_UUID TATest_UUID = TATEST_UUID;


void TestTeecInitContext(struct MODULE_Cfg* test_cfg)
{

	TEEC_Context 	context;
	TEEC_Result 	result;

	int 		case_index = 0;


	/* case 1 : parmeter error*/


	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	result = TEEC_InitializeContext(
			NULL,
			NULL);


	if (result == TEEC_SUCCESS) {
		error("ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = 1;
	}else{

	test_cfg->cases[case_index].result = 0;
	}
	/* case 2 : normal case*/

	case_index++;

	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	result = TEEC_InitializeContext(
			NULL,
			&context);
	if (result != TEEC_SUCCESS) {
		error("ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = 1;
		goto cleanup;
	}
	/* end test*/

	TEEC_FinalizeContext(&context);
cleanup:
	return;
}

void TestTeecFinalizeContext(struct MODULE_Cfg* test_cfg)
{

	TEEC_Context 	context;
	TEEC_Result 	result;
	TEEC_Session 	session;
	int		case_index = 0;

	/* first, initial it*/

	result = TEEC_InitializeContext(
			NULL,
			&context);
	if (result != TEEC_SUCCESS) {
		error("initial the context fail, can't continue...ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup;
	}


	/* case 1 : normal case*/

	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	TEEC_FinalizeContext(&context);


	/* open a session to verify the finalize context*/

	result = TEEC_OpenSession(
			&context,
			&session,
			&TATest_UUID,
			TEEC_LOGIN_USER,
			NULL,   /* No connection data needed for TEEC_LOGIN_USER. */
			NULL,   /* No payload, and do not want cancellation. */
			NULL);
	if (result == TEEC_SUCCESS) {
		error("normal case: open session success, but we have finalize the context.ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}


	/* case 2 : repeat finalize, can't verify this case, how do?*/
	/*
	result = TEEC_FinalizeContext(&context);
	if (result == TEEC_SUCCESS) {
		error("ret=0x%08x\n", result);
		return TEST_RESULT_FAIL;
	}

	*/
	/* end test*/

cleanup:
	return;
}

void TestTeecCloseSession(struct MODULE_Cfg* test_cfg)
{

	TEEC_Session 	session;
	TEEC_Context 	context;
	TEEC_Result 	result;
	TEEC_Operation 	operation;
	int 		cmdId;
	int 		case_index = 0;

	/* first, initial it*/

	result = TEEC_InitializeContext(
			NULL,
			&context);
	if (result != TEEC_SUCCESS) {
		error("initial the context fail, can't continue...ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup;
	}

	result = TEEC_OpenSession(
			&context,
			&session,
			&TATest_UUID,
			TEEC_LOGIN_USER,
			NULL,   /* No connection data needed for TEEC_LOGIN_USER. */
			NULL,   /* No payload, and do not want cancellation. */
			NULL);
	if (result != TEEC_SUCCESS) {
		error("open session fail, can't continue...ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup1;
	}

	/* case 1 : normal case*/
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	TEEC_CloseSession(&session);

	/* invoke a command to verify close session*/

	memset(&operation, 0, sizeof(operation));
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_VALUE_INPUT,
			TEEC_VALUE_OUTPUT,
			TEEC_NONE,
			TEEC_NONE);
	operation.params[0].value.a = 3;
	operation.params[0].value.b = 5;
	cmdId = TEST_CMD_ADD;

	/* clear result */
	operation.params[1].value.a = 0xdeadbeef;

	result = TEEC_InvokeCommand(
			&session,
			cmdId,
			&operation,
			NULL);


	if (result == TEEC_SUCCESS) {
		error("close session success, but we have close the session,ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	/*case 2: repeat close, but how to verify? ignore it*/

	/* end test*/
	TEEC_CloseSession(&session);
cleanup1:
	TEEC_FinalizeContext(&context);
cleanup:

	return;


}
void TestTeecOpenSession(struct MODULE_Cfg* test_cfg)
{

	TEEC_Session 	session;
	TEEC_Context 	context;
	TEEC_Result 	result;

	int 		case_index = 0;

	/* first, initial it*/

	result = TEEC_InitializeContext(
			NULL,
			&context);
	if (result != TEEC_SUCCESS) {
		error("initial the context fail, can't continue...ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup;
	}

	/*case 1: parameter error*/

	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	result = TEEC_OpenSession(
			NULL,
			&session,
			&TATest_UUID,
			TEEC_LOGIN_USER,
			NULL,   /* No connection data needed for TEEC_LOGIN_USER. */
			NULL,   /* No payload, and do not want cancellation. */
			NULL);
	if (result == TEEC_SUCCESS) {
		error("open session success, but parameter is error.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	result = TEEC_OpenSession(
			&context,
			NULL,
			&TATest_UUID,
			TEEC_LOGIN_USER,
			NULL,   /* No connection data needed for TEEC_LOGIN_USER. */
			NULL,   /* No payload, and do not want cancellation. */
			NULL);
	if (result == TEEC_SUCCESS) {
		error("open session success, but parameter is error.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	result = TEEC_OpenSession(
			&context,
			&session,
			NULL,
			TEEC_LOGIN_USER,
			NULL,   /* No connection data needed for TEEC_LOGIN_USER. */
			NULL,   /* No payload, and do not want cancellation. */
			NULL);
	if (result == TEEC_SUCCESS) {
		error("open session success, but parameter is error.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	/* case 2 : normal case*/
	case_index++;
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	result = TEEC_OpenSession(
			&context,
			&session,
			&TATest_UUID,
			TEEC_LOGIN_USER,
			NULL,   /* No connection data needed for TEEC_LOGIN_USER. */
			NULL,   /* No payload, and do not want cancellation. */
			NULL);
	if (result != TEEC_SUCCESS) {
		error("open session fail.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		goto cleanup1;
	}


	/* end test*/
	TEEC_CloseSession(&session);
cleanup1:
	TEEC_FinalizeContext(&context);
cleanup:

	return;


}


void TestTeecReleaseSharedMemory(struct MODULE_Cfg* test_cfg)
{

	TEEC_Session 		session;
	TEEC_Context 		context;
	TEEC_SharedMemory 	*shm = NULL;
	TEEC_Result 		result;
	int 			case_index=0;

	/* first, initial it*/

	result = TEEC_InitializeContext(
			NULL,
			&context);
	if (result != TEEC_SUCCESS) {
		error("initial the context fail, can't continue...ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup;
	}


	result = TEEC_OpenSession(
			&context,
			&session,
			&TATest_UUID,
			TEEC_LOGIN_USER,
			NULL,   /* No connection data needed for TEEC_LOGIN_USER. */
			NULL,   /* No payload, and do not want cancellation. */
			NULL);
	if (result != TEEC_SUCCESS) {
		error("open session fail, can't continue...ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup1;
	}



	shm = malloc(sizeof(*shm));
	if (NULL == shm){
		error("malloc shm fail, can't continue...ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_MEMOUT;
		goto cleanup2;
	}
	memset(shm, 0, sizeof(*shm));

	shm->size = 0x1000;
	shm->flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;

	/* case 1 : release shm, normal case*/
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	result = TEEC_AllocateSharedMemory(
			&context,
			shm);
	if (result != TEEC_SUCCESS) {
		error("teec alloc shm fail, can't continue...ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		free(shm);
		goto cleanup2;
	}



	TEEC_ReleaseSharedMemory(shm);

	/* verify the release shm, but how to?*/



	/* end test*/


	free(shm);
cleanup2:

	TEEC_CloseSession(&session);
cleanup1:
        TEEC_FinalizeContext(&context);
cleanup:
	return;
}

void TestTeecAllocSharedMemory(struct MODULE_Cfg* test_cfg)
{

	TEEC_Session 		session;
	TEEC_Context 		context;
	TEEC_SharedMemory 	*shm = NULL;
	TEEC_Result 		result;
	int 			case_index=0;

	/* first, initial it*/

	result = TEEC_InitializeContext(
			NULL,
			&context);
	if (result != TEEC_SUCCESS) {
		error("initial the context fail, can't continue...ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup;
	}


	result = TEEC_OpenSession(
			&context,
			&session,
			&TATest_UUID,
			TEEC_LOGIN_USER,
			NULL,   /* No connection data needed for TEEC_LOGIN_USER. */
			NULL,   /* No payload, and do not want cancellation. */
			NULL);
	if (result != TEEC_SUCCESS) {
		error("open session fail, can't continue...ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup1;
	}



	shm = malloc(sizeof(*shm));
	if (NULL == shm){
		error("malloc shm fail, can't continue...ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_MEMOUT;
		goto cleanup1;
	}
	memset(shm, 0, sizeof(*shm));

	shm->size = 0x1000;
	shm->flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;

	/* case 1 : parameter error, context NULL*/
	/* Use TEE Client API to allocate the underlying memory buffer. */
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	result = TEEC_AllocateSharedMemory(
			NULL,
			shm);
	if (result == TEEC_SUCCESS) {
		error("teec alloc shm success, but with error parameter, ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;

	}

	/* case 2 : parameter error, shm NULL*/

	result = TEEC_AllocateSharedMemory(
			&context,
			NULL);
	if (result == TEEC_SUCCESS) {
		error("teec alloc shm success, but with error parameter, ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	/* case 3 : parameter error, shm size is 0*/
	shm->size = 0;
	result = TEEC_AllocateSharedMemory(
			&context,
			shm);
	if (result == TEEC_SUCCESS) {
		error("teec alloc shm success, but with error parameter, ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}


	/* case 5 : normal case */
	case_index++;
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	shm->size = 0x10000;
	result = TEEC_AllocateSharedMemory(
			&context,
			shm);
	if (result != TEEC_SUCCESS) {
		error("teec alloc shm fail, ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		free(shm);
		goto cleanup2;
	}

	/* end test*/


	TEEC_ReleaseSharedMemory(shm);
	free(shm);
cleanup2:

	TEEC_CloseSession(&session);
cleanup1:
        TEEC_FinalizeContext(&context);
cleanup:
	return;
}




void TestTeecInvokeCommand(struct MODULE_Cfg* test_cfg)
{
	TEEC_Session 		session;
	TEEC_Context 		context;
	TEEC_SharedMemory 	*shm = NULL;
	TEEC_Result 		result;
	TEEC_Operation 		operation;

	int 			a = 3, b = 5;
	int 			cmdId = TEST_CMD_ADD;
	int 			case_index = 0;
	/* first, initial it*/

	result = TEEC_InitializeContext(
			NULL,
			&context);
	if (result != TEEC_SUCCESS) {
		error("initial the context fail, can't continue...ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup;
	}


	result = TEEC_OpenSession(
			&context,
			&session,
			&TATest_UUID,
			TEEC_LOGIN_USER,
			NULL,   /* No connection data needed for TEEC_LOGIN_USER. */
			NULL,   /* No payload, and do not want cancellation. */
			NULL);
	if (result != TEEC_SUCCESS) {
		error("open session fail, can't continue...ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup1;
	}


	memset(&operation, 0, sizeof(operation));
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_VALUE_INPUT,
			TEEC_VALUE_OUTPUT,
			TEEC_NONE,
			TEEC_NONE);
	operation.params[0].value.a = a;
	operation.params[0].value.b = b;

	/* clear result */
	operation.params[1].value.a = 0xdeadbeef;

	/* case 1 : parameter error*/

	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	result = TEEC_InvokeCommand(
			NULL,
			cmdId,
			&operation,
			NULL);


	if (result == TEEC_SUCCESS) {
		error("teec invoke cmd success, but with error parameter, ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	result = TEEC_InvokeCommand(
			&session,
			cmdId,
			NULL,
			NULL);


	if (result == TEEC_SUCCESS) {
		error("teec invoke cmd success, but with error parameter, ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	result = TEEC_InvokeCommand(
			&session,
			-1,
			&operation,
			NULL);


	if (result == TEEC_SUCCESS) {
		error("teec invoke cmd success, but with error parameter, ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}



	/* case 2 : normal case*/
	case_index++;
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	result = TEEC_InvokeCommand(
			&session,
			cmdId,
			&operation,
			NULL);


	if (result != TEEC_SUCCESS) {
		error("teec invoke cmd fail, ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}


	TEEC_CloseSession(&session);
cleanup1:
	TEEC_FinalizeContext(&context);
cleanup:
	return;
}



void TestTeecCancelLation(struct MODULE_Cfg* test_cfg)
{
	TEEC_Session 		session;
	TEEC_Context 		context;
	TEEC_SharedMemory 	*shm = NULL;
	TEEC_Result 		result;
	TEEC_Operation 		operation;

	pthread_t 		cancel_thread;

	int 			a = 3, b = 5;
	int 			cmdId = TEST_CMD_CANCELLATION;
	int 			case_index = 0;


	/* first, initial it*/

	result = TEEC_InitializeContext(
			NULL,
			&context);
	if (result != TEEC_SUCCESS) {
		error("initial the context fail, can't continue...ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup;
	}


	result = TEEC_OpenSession(
			&context,
			&session,
			&TATest_UUID,
			TEEC_LOGIN_USER,
			NULL,   /* No connection data needed for TEEC_LOGIN_USER. */
			NULL,   /* No payload, and do not want cancellation. */
			NULL);
	if (result != TEEC_SUCCESS) {
		error("open session fail, can't continue...ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup1;
	}

	/*case 1: normal case*/

	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	memset(&operation, 0, sizeof(operation));
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_VALUE_INPUT,
			TEEC_VALUE_OUTPUT,
			TEEC_NONE,
			TEEC_NONE);
	operation.params[0].value.a = a;
	operation.params[0].value.b = b;

	/* clear result */
	operation.params[1].value.a = 0xdeadbeef;


	/* create thread for cancel request*/

	pthread_create(&cancel_thread, NULL, thread_for_cancellation, (void*)(&operation));

	result = TEEC_InvokeCommand(
			&session,
			cmdId,
			&operation,
			NULL);


	if (result != TEEC_ERROR_CANCEL) {
		error("request cancel fail,ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}


	pthread_join(cancel_thread, NULL);


	TEEC_CloseSession(&session);
cleanup1:
	TEEC_FinalizeContext(&context);
cleanup:
	return;

}


void TestTeecGetError(struct MODULE_Cfg *test_cfg)
{
	return;
}


void TestTeecRegisterTa(struct MODULE_Cfg *test_cfg)
{
	return;
}

void TestClientAPI(struct MODULE_Cfg *test_cfg)
{
	TEST_Result	result;
	int 		test_type,test_times;
	bool		test_on;
	char 		*test_name;
	int 		i;
	int 		test_index = 0;

	printf("*******enter client api test*********\n");

	while(1) {

		test_name = test_cfg[test_index].name;
		test_type = test_cfg[test_index].type;
		test_times = test_cfg[test_index].times;
		test_on = test_cfg[test_index].enable;

		/* is tail ? */
		if(test_type == -1)
			break;

		if(!test_on){
			test_index++;
			continue;
		}

		for(i = 0; i < test_times;i++) {
			switch(test_type) {

			case	TEST_TEEC_INIT_CONTEXT:
				TestTeecInitContext(&test_cfg[test_index]);
				break;

			case	TEST_TEEC_FINIT_CONTEXT:
				TestTeecFinalizeContext(&test_cfg[test_index]);
				break;

			case	TEST_TEEC_ALLOC_SHM:
				TestTeecAllocSharedMemory(&test_cfg[test_index]);
				break;
			case	TEST_TEEC_RELEASE_SHM:
				TestTeecReleaseSharedMemory(&test_cfg[test_index]);
				break;

			case	TEST_TEEC_OPEN_SESSION:
				TestTeecOpenSession(&test_cfg[test_index]);
				break;
			case	TEST_TEEC_CLOSE_SESSION:
				TestTeecCloseSession(&test_cfg[test_index]);
				break;
			case	TEST_TEEC_INVOKECOMMAND:
				TestTeecInvokeCommand(&test_cfg[test_index]);
				break;
			case 	TEST_TEEC_REQUESTCANCELLATION:
				TestTeecCancelLation(&test_cfg[test_index]);
				break;
			case 	TEST_TEEC_GETERROR:
				TestTeecGetError(&test_cfg[test_index]);
				break;
			case	TEST_TEEC_REGISTERTA:
				TestTeecRegisterTa(&test_cfg[test_index]);
				break;

			default:
				break;

			}
		}

		show_result(&test_cfg[test_index], i);
		test_index++;

	}
	return;
}
