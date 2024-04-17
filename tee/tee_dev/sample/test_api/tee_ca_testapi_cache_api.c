#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include "config.h"
#include "tee_testapi.h"
#include "tee_client_api.h"
#include "testapi_cmd.h"
#include "tee_ca_testapi_util.h"
#include "string.h"
#include "memory.h"
#include "log.h"


static const TEEC_UUID TATest_UUID = TATEST_UUID;


void FillShm(TEEC_SharedMemory *shm, int pattern)
{
	uint32_t *word = shm->buffer;

	uint32_t i;

	for (i = 0; i < shm->size/4; i++) {
		word[i] = pattern;
	}

	return;

}
TEST_Result TestCacheInvalid(struct MODULE_Cfg* test_cfg)
{
	return TEST_RESULT_PASS;
}
TEST_Result TestCacheFlush(struct MODULE_Cfg* test_cfg)
{

	TEEC_Session		session;
	TEEC_Context		context;
	TEEC_SharedMemory 	*shm = NULL;
	TEEC_Operation 		operation;
	TEEC_Result 		result;
	int 			case_index=0;
	int 			pattern = 0xa5a5a5a5;
	int 			cmdId = TEST_CMD_CACHEFLUSH;


	/* first, initial it*/

	result = TEEC_InitializeContext(
			NULL,
			&context);
	if (result != TEEC_SUCCESS) {
		error("ret=0x%08x\n", result);
		return TEST_RESULT_FAIL;
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
		error("ret=0x%08x\n", result);
		result = TEST_RESULT_FAIL;
		goto cleanup1;
	}

/*

	shm = malloc(sizeof(*shm));
	if (NULL == shm){
		result = TEST_RESULT_MEMOUT;
		goto cleanup1;
	}

	memset(shm, 0, sizeof(*shm));

	shm->size = 0x1000 + 0x1000;
	shm->flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;

	result = TEEC_AllocateSharedMemory(
			&context,
			shm);
	if (result != TEEC_SUCCESS) {
		error("ret=0x%08x\n", result);
		result = TEST_RESULT_FAIL;
		test_cfg->cases[case_index].result = 1;
		goto cleanup2;
	}

	FillShm(shm, pattern);
*/
	memset(&operation, 0, sizeof(operation));
	operation.paramTypes = TEEC_PARAM_TYPES(
			TEEC_VALUE_INOUT,
			TEEC_VALUE_INOUT,
			TEEC_VALUE_INOUT,
			TEEC_NONE);
	operation.params[0].value.a = 0X80000000;
	operation.params[0].value.b = 0X10000;
	operation.params[1].value.a = 0x80100000;
	operation.params[1].value.b = 0X10000;

	result = TEEC_InvokeCommand(
			&session,
			cmdId,
			&operation,
			NULL);


	if (result != TEEC_SUCCESS) {
		error("ret=0x%08x\n", result);
		result = TEST_RESULT_FAIL;
		test_cfg->cases[case_index].result = 1;
		goto cleanup3;
	}

	test_cfg->cases[case_index].result = 0;

	/* end test*/

	result = TEST_RESULT_PASS;

cleanup3:
	TEEC_ReleaseSharedMemory(shm);
	free(shm);

cleanup2:

	TEEC_CloseSession(&session);
cleanup1:
	TEEC_FinalizeContext(&context);
	return result;
}







void TestCacheAPI(struct MODULE_Cfg *test_cfg)
{
	TEST_Result	result = TEST_RESULT_PASS;
	int		test_type,test_times;
	bool		test_on;
	char		*test_name;
	int		i;
	int		test_index = 0;


	while(1) {

		test_name = test_cfg[test_index].name;
		test_type = test_cfg[test_index].type;
		test_times = test_cfg[test_index].times;
		test_on = test_cfg[test_index].enable;


		if(test_type == -1)
			break;

		if(!test_on){
			test_index++;
			continue;
		}

		for(i = 0; i < test_times;i++) {
			switch(test_type) {

			case	TEST_TEE_CACHE_FLUSH:
				result = TestCacheFlush(&test_cfg[test_index]);
				break;


			case	TEST_TEE_CACHE_INVALID:
				result = TestCacheInvalid(&test_cfg[test_index]);
				break;

			default:
				break;

			}
			if(result != TEST_RESULT_PASS)
				break;
		}

		show_result(&test_cfg[test_index], i);
		test_index++;

	}
	return;
}
