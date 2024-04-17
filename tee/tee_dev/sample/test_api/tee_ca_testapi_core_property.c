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




void TestCoreProperty(struct MODULE_Cfg* test_cfg)
{
	TEEC_Session 		session;
	TEEC_Context 		context;
	TEEC_SharedMemory 	*shm = NULL;
	TEEC_Result 		result;
	TEEC_Operation 		operation;

	pthread_t 		cancel_thread;

	int 			cmdId = TEST_CMD_CORE_PROPERTY;
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

	shm = malloc(sizeof(*shm));
	if (NULL == shm){
		error("malloc shm fail, can't continue...ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_MEMOUT;
		goto cleanup2;
	}

	memset(shm, 0, sizeof(*shm));

	shm->size = sizeof(*test_cfg);

	shm->flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;

	result = TEEC_AllocateSharedMemory(
			&context,
			shm);
	if (result != TEEC_SUCCESS) {
		error("teec alloc shm fail, can't continue...ret=0x%08x\n",
				result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup2;
	}

	memcpy(shm->buffer, test_cfg, sizeof(*test_cfg));

	memset(&operation, 0, sizeof(operation));
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_MEMREF_WHOLE,
			TEEC_NONE,
			TEEC_NONE,
			TEEC_NONE);

	operation.params[0].memref.parent = shm;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = 0;

	result = TEEC_InvokeCommand(
			&session,
			cmdId,
			&operation,
			NULL);


	if (result != TEEC_SUCCESS) {
		error("invoke cmd fail,ret=0x%08x\n", result);
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup3;
	}



	memcpy(test_cfg, shm->buffer, sizeof(*test_cfg));

cleanup3:
	TEEC_ReleaseSharedMemory(shm);
	free(shm);

cleanup2:
	TEEC_CloseSession(&session);
cleanup1:
	TEEC_FinalizeContext(&context);
cleanup:
	return;

}


void TestCorePropertyAPI(struct MODULE_Cfg *test_cfg)
{
	TEST_Result	result;
	int 		test_type,test_times;
	bool		test_on;
	char 		*test_name;
	int 		i;
	int 		test_index = 0;

	printf("*******enter core property api test*********\n");

	while(1) {

		test_name = test_cfg[test_index].name;
		test_type = test_cfg[test_index].type;
		test_times = test_cfg[test_index].times;
		test_on = test_cfg[test_index].enable;

		printf("test api name %s, type is %d, times is %d, enable is %d\n",
				test_name, test_type, test_times, test_on);

		/* the tail of test list*/
		if(test_type == -1)
			break;

		/* disable the test*/
		if(!test_on){
			test_index++;
			continue;
		}

		for(i = 0; i < test_times;i++) {
			TestCoreProperty(&test_cfg[test_index]);
		}

		show_result(&test_cfg[test_index], i);
		test_index++;

	}
	return;
}
