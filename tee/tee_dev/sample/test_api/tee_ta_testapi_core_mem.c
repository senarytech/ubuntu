/*
 * test tee core api
 */
#include "tee_testapi.h"
#include "testapi_cmd.h"
#include "tee_common.h"
#include "tee_internal_core_common.h"
#include "tee_internal_core_mem_api.h"
#include "tee_internal_private_api.h"

#define TEST_MALLOC_MAGIC	(0x54452a4d)    /* 'TE*M' */
#define MAX_REGION_NUM		6

static const TEE_UUID		TAChen_UUID = TACHEN_UUID;
static const TEE_UUID		TAFake_UUID = TAFAKE_UUID;

static TEE_MemRegion		TestMemRegionList[MAX_REGION_NUM];
static uint32_t			attrVal[MAX_REGION_NUM] = {
	TEE_MR_ATTR(TZ_SRW_NNA, 0, 0, MR_MEM_WRITEBACK_RAWA, 0),
	TEE_MR_ATTR(TZ_SNA_NNA, 0, 0, MR_MEM_WRITEBACK_RAWA, 0),
	TEE_MR_ATTR(TZ_SNA_NNA, 0, 0, MR_MEM_WRITEBACK_RAWA, 0),
	TEE_MR_ATTR(TZ_SRW_NRW, 0, 1, MR_MEM_WRITEBACK_RAWA, 0),
	TEE_MR_ATTR(TZ_SRW_NRW, 0, 1, MR_MEM_WRITEBACK_RAWA, 0),
	TEE_MR_ATTR(TZ_SRW_NRW, 0, 1, MR_MEM_WRITEBACK_RAWA, 0)
};

static void FillMem(void *buf, unsigned char pattern, int length)
{
	int 		i;
	int 		*v = (int*)buf;
	unsigned char 	*ev;
	int 		extra = length & 3;


	ev = (unsigned char*)buf;
	for(i = 0; i < length; i++) {
		*ev = pattern;
		ev++;

	}


}

static int CmpMem(void *src, void *dst, int length)
{
	int 		i;

	unsigned char 	*evsrc, *evdst;

	evsrc = src;
	evdst = dst;

	for(i = 0; i < length; i++) {
		if(*evsrc != *evdst) {
			return 1;
		}
		evsrc++;
		evdst++;
	}
	return 0;


}
void TestCoreMemRealloc(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		case_index = 0;
	TEE_Result		result;

	struct MODULE_Cfg	*test_cfg;

	void			*test_buf;
	uint32_t		length = 1024;

	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;


	test_buf = TEE_Malloc(length, 0);


	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	if(NULL == test_buf) {
		printf(" TEE_Malloc return NULL. can't continue...\n");
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup;
	}


	test_buf = TEE_Realloc(test_buf, 2048);
	if(NULL == test_buf) {
		printf(" TEE_Realloc return NULL. can't continue...\n");
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup;
	}

	/* verify the magic num*/

	if((*(int*)(test_buf-16)) != TEST_MALLOC_MAGIC){
		printf("TEE_Realloc magic num not match\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	TEE_Free(test_buf);

cleanup:
	return;

}
void TestCoreMemAlloc(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		case_index = 0;
	TEE_Result		result;

	struct MODULE_Cfg	*test_cfg;

	void			*test_buf;
	uint32_t		length = 1024;

	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;


	test_buf = TEE_Malloc(length, 0);


	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	if(NULL == test_buf) {
		printf(" TEE_Malloc return NULL. can't continue...\n");
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup;
	}

	/* verify the magic num*/
	if((*(int*)(test_buf-16)) != TEST_MALLOC_MAGIC){
		printf("TEE_Malloc magic num not match\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	TEE_Free(test_buf);

cleanup:
	return;

}


void TestCoreMemFree(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		case_index = 0;
	TEE_Result		result;

	struct MODULE_Cfg	*test_cfg;

	void			*test_buf;
	uint32_t		length = 1024;

	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;


	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	test_buf = TEE_Malloc(length, 0);


	if(NULL == test_buf) {
		printf(" TEE_Malloc return NULL. can't continue...\n");
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup;
	}

	TEE_Free(test_buf);
	if((*(int*)(test_buf-4)) == TEST_MALLOC_MAGIC){
		printf("TEE_Free magic num is match\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}


cleanup:
	return;

}


void TestCoreMemMove(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		case_index = 0;
	TEE_Result		result;

	struct MODULE_Cfg	*test_cfg;

	void			*src_buf, *dst_buf;
	uint32_t		length = 1024;
	unsigned char		pattern = 0x5a;

	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;


	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	src_buf = TEE_Malloc(length, 0);
	dst_buf = TEE_Malloc(length, 0);

	if(NULL == dst_buf || NULL == dst_buf) {
		printf(" TEE_Malloc return NULL. can't continue...\n");
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup;
	}

	/*case 1: not word align length*/
	length = 64 + 3;

	FillMem(src_buf, pattern, length);

	TEE_MemMove(dst_buf, src_buf, length);

	result = CmpMem(src_buf, dst_buf, length);

	if(0 != result) {
		printf("Move mem fail when not word align.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;

	}

	/*case 2: word align length*/

	case_index++;
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	length = 128;

	FillMem(src_buf, pattern, length);

	TEE_MemMove(dst_buf, src_buf, length);

	result = CmpMem(src_buf, dst_buf, length);

	if(0 != result) {
		printf("Move mem fail when word align.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;

	}


	TEE_Free(src_buf);
	TEE_Free(dst_buf);


cleanup:
	return;

}


void TestCoreMemCmp(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		case_index = 0;
	TEE_Result		result;

	struct MODULE_Cfg	*test_cfg;

	void			*src_buf, *dst_buf;
	uint32_t		length = 1024;
	unsigned char		pattern = 0x5a;

	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;


	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	src_buf = TEE_Malloc(length, 0);
	dst_buf = TEE_Malloc(length, 0);

	if(NULL == dst_buf || NULL == dst_buf) {
		printf(" TEE_Malloc return NULL. can't continue...\n");
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup;
	}

	FillMem(src_buf, pattern, length);
	FillMem(dst_buf, pattern, length);


	result = TEE_MemCompare(dst_buf, src_buf, length);

	if(0 != result) {
		printf("TEE memory compare mem fail.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;

	}

	TEE_Free(src_buf);
	TEE_Free(dst_buf);


cleanup:
	return;

}
void TestCoreMemFill(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		case_index = 0;
	TEE_Result		result;

	struct MODULE_Cfg	*test_cfg;

	void			*src_buf, *dst_buf;
	uint32_t		length = 1024;
	unsigned char		pattern = 0x5a;

	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;


	test_cfg->cases[case_index].result = TEST_RESULT_PASS;

	src_buf = TEE_Malloc(length, 0);
	dst_buf = TEE_Malloc(length, 0);

	if(NULL == dst_buf || NULL == dst_buf) {
		printf(" TEE_Malloc return NULL. can't continue...\n");
		test_cfg->cases[case_index].result = TEST_RESULT_CANT;
		goto cleanup;
	}

	FillMem(src_buf, pattern, length);


	TEE_MemFill(dst_buf, pattern, length);

	result = CmpMem(src_buf, dst_buf, length);

	if(0 != result) {
		printf("TEE memory fill fail.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;

	}

	TEE_Free(src_buf);
	TEE_Free(dst_buf);


cleanup:
	return;

}

void TestCoreMemAccessCheck(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{

	uint32_t		case_index = 0;
	TEE_Result		result;

	struct MODULE_Cfg	*test_cfg;

	int			i, regionNum;

	test_cfg = (struct MODULE_Cfg*)params[0].memref.buffer;

	/*initial the memory region list*/

	for(i = 0; i < MAX_REGION_NUM; i++) {
		regionNum = TEE_GetMemRegionList(&TestMemRegionList[i], 1, 0xffffffff,
				attrVal[i]);
		if(regionNum == 0) {
			printf("init region list fail, ret is %x, can't continue...\n",
					regionNum);
			test_cfg->cases[case_index].result = TEST_RESULT_CANT;
			goto cleanup;

		}
		/*
		printf("index = %d, base = %x, size = %x\n", i,
				TestMemRegionList[i].base, TestMemRegionList[i].size);
		*/
	}

	/* case 1 : check the 1st region, */
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	result = TEE_CheckMemoryAccessRights(TEE_ACCESS_READ | TEE_ACCESS_WRITE,
			(void*)TestMemRegionList[case_index].base,
			TestMemRegionList[case_index].size);

	if(TEE_SUCCESS != result){
		printf(" check zsp region fail.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}


	/* case 2 : check the 2nd region, */
	case_index++;
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	result = TEE_CheckMemoryAccessRights(TEE_ACCESS_READ | TEE_ACCESS_WRITE,
			(void*)TestMemRegionList[case_index].base,
			TestMemRegionList[case_index].size);

	if(TEE_SUCCESS == result){
		printf(" check zsp region fail.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	/* case 3 : check the 3rd region, */
	case_index++;
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	result = TEE_CheckMemoryAccessRights(TEE_ACCESS_READ | TEE_ACCESS_WRITE,
			(void*)TestMemRegionList[case_index].base,
			TestMemRegionList[case_index].size);

	if(TEE_SUCCESS == result){
		printf(" check sys_init region fail.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}


	/* case 4 : check the 4th region, */
	case_index++;
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	result = TEE_CheckMemoryAccessRights(TEE_ACCESS_READ | TEE_ACCESS_WRITE,
			(void*)TestMemRegionList[case_index].base,
			TestMemRegionList[case_index].size);

	if(TEE_SUCCESS != result){
		printf(" check bootloader region fail.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	/* case 5 : check the 5th region, */
	case_index++;
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	result = TEE_CheckMemoryAccessRights(TEE_ACCESS_READ | TEE_ACCESS_WRITE,
			(void*)TestMemRegionList[case_index].base,
			TestMemRegionList[case_index].size);

	if(TEE_SUCCESS != result){
		printf(" check system region fail.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}

	/* case 6 : check the 6th region, */
	case_index++;
	test_cfg->cases[case_index].result = TEST_RESULT_PASS;
	result = TEE_CheckMemoryAccessRights(TEE_ACCESS_READ | TEE_ACCESS_WRITE,
			(void*)TestMemRegionList[case_index].base,
			TestMemRegionList[case_index].size);

	if(TEE_SUCCESS != result){
		printf(" check shm region fail.\n");
		test_cfg->cases[case_index].result = TEST_RESULT_FAIL;
	}


cleanup:
	return;

}

TEE_Result TestCoreMem(
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

	case TEST_TEE_CORE_MEM_ALLOC:
		TestCoreMemAlloc(sessionContext, paramTypes, params);
		break;

	case TEST_TEE_CORE_MEM_REALLOC:
		TestCoreMemRealloc(sessionContext, paramTypes, params);
		break;

	case TEST_TEE_CORE_MEM_MOVE:
		TestCoreMemMove(sessionContext, paramTypes, params);
		break;
	case TEST_TEE_CORE_MEM_CMP:
		TestCoreMemCmp(sessionContext, paramTypes, params);
		break;
	case TEST_TEE_CORE_MEM_FILL:
		TestCoreMemFill(sessionContext, paramTypes, params);
		break;
	case TEST_TEE_CORE_MEM_FREE:
		TestCoreMemFree(sessionContext, paramTypes, params);
		break;
	case TEST_TEE_CORE_MEM_ACCESS_CHECK:
		TestCoreMemAccessCheck(sessionContext, paramTypes, params);
		break;
	default:
		break;

	}

	return TEE_SUCCESS;
}
