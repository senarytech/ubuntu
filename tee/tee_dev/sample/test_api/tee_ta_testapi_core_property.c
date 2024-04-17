/*
 * test tee core api
 */
#include "tee_testapi.h"
#include "testapi_cmd.h"
#include "tee_common.h"
#include "tee_internal_core_common.h"
#include "tee_internal_core_prop_api.h"

typedef union {
	bool			b;
	uint32_t		u32;
	const char*		str;
	void*			bin;
	TEE_UUID*		uuid;
	TEE_Identity*		id;
} TestPropertyValue;


static const TEE_UUID 		TAFake_UUID = TAFAKE_UUID;
static const TEE_UUID 		TATest_UUID = TATEST_UUID;


static TEE_PropSetHandle	enumprop;
static TEE_PropSetHandle	propset = (TEE_PropSetHandle)TEE_PROPSET_ID_CURRENT_TA;
static char 			name[MAX_NAME_LEN];
static int			namelen;
static char			valuebuf[128];
static int			valuelen = 128;
static TEE_UUID			propuuid;
static TestPropertyValue		propvalue = {
				.str =		valuebuf,
				.bin =		valuebuf,
				.uuid =		&propuuid,
};

TEE_Result TestCoreProperty(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		case_index = 0;
	bool			result = false;
	uint32_t 		cancellationTimeout = 0;

	struct MODULE_Cfg	*test_cfg;
	int			timeout = 0;


	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_MEMREF_INOUT) {
		printf("input test parameter error, can't continue....\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;

	printf("test core property, id is %d\n", test_cfg->type);
	switch(test_cfg->type) {

	case TEST_TEE_CORE_ALLOC_PROPERTY:


		/*case 1: parameter NULL*/

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_AllocatePropertyEnumerator(NULL);

		if(TEE_SUCCESS == result){
			printf("tee alloc property success, but parameter is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 2: normal case*/
		case_index++;
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_AllocatePropertyEnumerator(&enumprop);

		if(TEE_SUCCESS != result){
			printf("tee alloc property fail.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}
		break;

	case TEST_TEE_CORE_RESET_PROPERTY:

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;
		TEE_ResetPropertyEnumerator(enumprop);
#if 0
		/*case 1: parameter NULL */
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_ResetPropertyEnumerator(NULL);
		if(TEE_SUCCESS == result){
			printf("tee reset property success, but parameter NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}
		/*case 2: reset enum property */

		case_index++;

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_ResetPropertyEnumerator(enumprop);
		if(TEE_SUCCESS != result){
			printf("tee reset property fail.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}
#endif
		break;

	case TEST_TEE_CORE_START_PROPERTY:

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;
		TEE_StartPropertyEnumerator(enumprop, propset);
#if 0
		/*case 1: parameter NULL */

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_StartPropertyEnumerator(NULL);
		if(TEE_SUCCESS == result){
			printf("tee start property success, but parameter NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}
		/*case 2: normal case */

		case_index++;

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_StartPropertyEnumerator(enumprop);
		if(TEE_SUCCESS != result){
			printf("tee start property fail.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}
#endif
		break;

	case TEST_TEE_CORE_GET_PROPERTY_NAME:

		/*case 1: parameter error*/

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		/* remove this case because of assert */
		/*result = TEE_GetPropertyName(NULL, name, &namelen);
		if(TEE_SUCCESS == result){
			printf("tee get property name success, but parameter property is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}*/

		/*case 2: parameter error*/
		case_index++;
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetPropertyName(enumprop, NULL, &namelen);
		if(TEE_SUCCESS == result){
			printf("tee get property name success, but parameter name is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 3: parameter error*/
		case_index++;
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;
		result = TEE_GetPropertyName(enumprop, name, NULL);
		if(TEE_SUCCESS == result){
			printf("tee get property name success, but parameter namelen is 0.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 4: parameter error */
		case_index++;

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;
		namelen = 0;
		result = TEE_GetPropertyName(enumprop, name, &namelen);
		if(TEE_SUCCESS == result){
			printf("tee get property name success. but namelen is 0\n", result);
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 5: */
		case_index++;

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;
		namelen = 120;
		result = TEE_GetPropertyName(enumprop, name, &namelen);
		if(TEE_SUCCESS != result){
			printf("tee get property name fail, ret = %x\n", result);
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		break;

	case TEST_TEE_CORE_GET_PROPERTY_AS_BOOL:
		/*case 1: parameter error*/

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetPropertyAsBool(NULL, name, &propvalue.b);
		if(TEE_SUCCESS == result){
			printf("tee get property as bool success, but parameter property is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 2: parameter error*/
		case_index++;
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetPropertyAsBool(enumprop, NULL, &propvalue.b);
		if(TEE_SUCCESS == result){
			printf("tee get property as bool success, but parameter name is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 3: parameter error*/
		case_index++;
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;
		result = TEE_GetPropertyAsBool(enumprop, name, NULL);
		if(TEE_SUCCESS == result){
			printf("tee get property as bool success, but parameter namelen is 0.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 4: */
		case_index++;

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetPropertyAsBool(enumprop, name, &propvalue.b);
		if(TEE_SUCCESS != result && TEE_ERROR_BAD_FORMAT != result){
			printf("tee get property as bool fail.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		break;

	case TEST_TEE_CORE_GET_PROPERTY_AS_STRING:
		/*case 1: parameter error*/

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetPropertyAsString(NULL, name, (void*)propvalue.str, &valuelen);
		if(TEE_SUCCESS == result){
			printf("tee get property as string success, but parameter property is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 2: parameter error*/
		case_index++;
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetPropertyAsString(enumprop, NULL, (void*)propvalue.str, &valuelen);
		if(TEE_SUCCESS == result){
			printf("tee get property as string success, but parameter name is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 3: parameter error*/
		case_index++;
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;
		result = TEE_GetPropertyAsString(enumprop, name, NULL, &valuelen);
		if(TEE_SUCCESS == result){
			printf("tee get property as string success, but parameter value buffer is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 4: parameter error*/
		case_index++;

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;
		result = TEE_GetPropertyAsString(enumprop, name, (void*)propvalue.str, NULL);
		if(TEE_SUCCESS == result){
			printf("tee get property as string success, but parameter valuelen is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 5: parameter error*/
		case_index++;

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;
		valuelen = 0;
		result = TEE_GetPropertyAsString(enumprop, name, (void*)propvalue.str, &valuelen);
		if(TEE_ERROR_SHORT_BUFFER != result){
			printf("tee get property as string success, but parameter valuelen is 0.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}
		/*case 6: */
		case_index++;

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;
		valuelen = 128;

		result = TEE_GetPropertyAsString(enumprop, name, (void*)propvalue.str, &valuelen);
		if(TEE_SUCCESS != result && TEE_ERROR_BAD_FORMAT != result){
			printf("tee get property as string fail.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		break;

	case TEST_TEE_CORE_GET_PROPERTY_AS_U32:
		/*case 1: parameter error*/

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetPropertyAsU32(NULL, name, &propvalue.u32);
		if(TEE_SUCCESS == result){
			printf("tee get property as u32 success, but parameter property is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 2: parameter error*/
		case_index++;
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetPropertyAsU32(enumprop, NULL, &propvalue.u32);
		if(TEE_SUCCESS == result){
			printf("tee get property as u32 success, but parameter name is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 3: parameter error*/
		case_index++;
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;
		result = TEE_GetPropertyAsU32(enumprop, name, NULL);
		if(TEE_SUCCESS == result){
			printf("tee get property as u32 success, but parameter valuelen is 0.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 4: */
		case_index++;

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetPropertyAsU32(enumprop, name, &propvalue.u32);
		if(TEE_SUCCESS != result && TEE_ERROR_BAD_FORMAT != result){
			printf("tee get property as u32 fail.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		break;

	case TEST_TEE_CORE_GET_PROPERTY_AS_UUID:
		/*case 1: parameter error*/

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetPropertyAsUUID(NULL, name, propvalue.uuid);
		if(TEE_SUCCESS == result){
			printf("tee get property as uuid success, but parameter property is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 2: parameter error*/
		case_index++;
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetPropertyAsUUID(enumprop, NULL, propvalue.uuid);
		if(TEE_SUCCESS == result){
			printf("tee get property as uuid success, but parameter name is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 3: parameter error*/
		case_index++;
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;
		result = TEE_GetPropertyAsUUID(enumprop, name, NULL);
		if(TEE_SUCCESS == result){
			printf("tee get property as uuid success, but parameter valuelen is 0.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 4: */
		case_index++;

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetPropertyAsUUID(enumprop, name, propvalue.uuid);
		if(TEE_SUCCESS != result && TEE_ERROR_BAD_FORMAT != result){
			printf("tee get property as uuid fail.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		break;

	case TEST_TEE_CORE_GET_PROPERTY_AS_ID:
		/*case 1: parameter error*/

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetPropertyAsIdentity(NULL, name, propvalue.id);
		if(TEE_SUCCESS == result){
			printf("tee get property as identity success, but parameter property is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 2: parameter error*/
		case_index++;
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetPropertyAsIdentity(enumprop, NULL, propvalue.id);
		if(TEE_SUCCESS == result){
			printf("tee get property as identity success, but parameter name is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 3: parameter error*/
		case_index++;
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;
		result = TEE_GetPropertyAsIdentity(enumprop, name, NULL);
		if(TEE_SUCCESS == result){
			printf("tee get property as identity success, but parameter namelen is 0.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 4: */
		case_index++;

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetPropertyAsIdentity(enumprop, name, propvalue.id);
		if(TEE_SUCCESS != result && TEE_ERROR_BAD_FORMAT != result){
			printf("tee get property as identity fail.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		break;

	case TEST_TEE_CORE_GET_PROPERTY_AS_BINBLOCK:
		/*case 1: parameter error*/

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetPropertyAsBinaryBlock(NULL, name, propvalue.bin, &valuelen);
		if(TEE_SUCCESS == result){
			printf("tee get property as binaryblock success, but parameter property is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 2: parameter error*/
		case_index++;
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetPropertyAsBinaryBlock(enumprop, NULL, propvalue.bin, &valuelen);
		if(TEE_SUCCESS == result){
			printf("tee get property as binaryblock success, but parameter name is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 3: parameter error*/
		case_index++;
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;
		result = TEE_GetPropertyAsBinaryBlock(enumprop, name, NULL, &valuelen);
		if(TEE_SUCCESS == result){
			printf("tee get property as binaryblock success, but parameter namelen is 0.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 4: parameter error*/
		case_index++;
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;
		result = TEE_GetPropertyAsBinaryBlock(enumprop, name, NULL, &valuelen);
		if(TEE_SUCCESS == result){
			printf("tee get property as binaryblock success, but parameter namelen is 0.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 5: parameter error*/
		case_index++;
		valuelen = 0;
		test_cfg->cases[case_index].result = TEST_RESULT_PASS;
		result = TEE_GetPropertyAsBinaryBlock(enumprop, name, propvalue.bin, &valuelen);
		if(TEE_SUCCESS == result){
			printf("tee get property as binaryblock success, but parameter namelen is 0.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		/*case 6: */
		case_index++;

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		valuelen = 128;
		result = TEE_GetPropertyAsBinaryBlock(enumprop, name, propvalue.bin, &valuelen);
		if(TEE_SUCCESS != result && TEE_ERROR_BAD_FORMAT != result){
			printf("tee get property as binaryblock fail.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}

		break;

	case TEST_TEE_CORE_GET_NEXT_PROPERTY:
		/*case 1: */
		case_index++;

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetNextProperty(enumprop);
		if(TEE_SUCCESS != result && TEE_ERROR_BAD_FORMAT != result){
			printf("tee get next property fail.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}
		/*case 2: parameter error*/

		/*test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		result = TEE_GetNextProperty(NULL);
		if(TEE_SUCCESS == result){
			printf("tee get next property success, but parameter property is NULL.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}*/


		break;

	case TEST_TEE_CORE_FREE_PROPERTY:

		/*case 1: */

		test_cfg->cases[case_index].result = TEST_RESULT_PASS;

		TEE_FreePropertyEnumerator(enumprop);

		/* because of the assert, we can't verify this case.*/
		/*result = TEE_GetNextProperty(enumprop);
		if(TEE_SUCCESS == result) {
			printf("tee free property success, but we have free it.\n");
			test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
		}*/

		break;
	default:
		break;



	}
	return TEE_SUCCESS;

}

