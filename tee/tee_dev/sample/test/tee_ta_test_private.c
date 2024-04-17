/*
 * test tee private api
 */
#include "config.h"
#include "tee_ta_test.h"
#include "macros.h"
#include "log.h"

TEE_Result Test_TEE_Callback(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])

{
	TEE_Result result;
	uint32_t cb_cmdID = 0xdeadbeef;
	uint32_t cb_paramTypes = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
			TEE_PARAM_TYPE_VALUE_INOUT,
			TEE_PARAM_TYPE_VALUE_INOUT,
			TEE_PARAM_TYPE_VALUE_INOUT);

	TEE_Param cb_params[4];
	cb_params[0].value.a = 0x11111111;
	cb_params[0].value.b = 0x22222222;
	cb_params[1].value.a = 0x33333333;
	cb_params[1].value.b = 0x44444444;
	cb_params[2].value.a = 0x55555555;
	cb_params[2].value.b = 0x66666666;
	cb_params[3].value.a = 0x77777777;
	cb_params[3].value.b = 0x88888888;

	result = TEE_Callback(cb_cmdID, cb_paramTypes, cb_params, 0, NULL);

	/* check result, must revert the input */
	if (cb_params[0].value.a != ~0x11111111 ||
			cb_params[0].value.b != ~0x22222222 ||
			cb_params[1].value.a != ~0x33333333 ||
			cb_params[1].value.b != ~0x44444444 ||
			cb_params[2].value.a != ~0x55555555 ||
			cb_params[2].value.b != ~0x66666666 ||
			cb_params[3].value.a != ~0x77777777 ||
			cb_params[3].value.b != ~0x88888888) {
		error("callback return: commandID=0x%08x, paramTypes=0x%08x, "
			"{0x%08x,0x%08x}, {0x%08x,0x%08x}, "
			"{0x%08x,0x%08x}, {0x%08x,0x%08x}\n",
			cb_cmdID,
			cb_paramTypes,
			cb_params[0].value.a,
			cb_params[0].value.b,
			cb_params[1].value.a,
			cb_params[1].value.b,
			cb_params[2].value.a,
			cb_params[2].value.b,
			cb_params[3].value.a,
			cb_params[3].value.b);
	}

	return result;
}

TEE_Result Test_TEE_MemRegion(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	TEE_Result result = TEE_SUCCESS;
	TEE_MemRegion *mr = NULL;
	uint32_t i, mrc = 0;
	mrc = TEE_GetMemRegionCount(0, 0);
	info("region count=%d\n", mrc);
	if (mrc == 0)
		return TEE_ERROR_ACCESS_CONFLICT;
	mr = malloc(mrc * sizeof(*mr));
	if (!mr) {
		error("fail to allocate memory\n");
		return TEE_ERROR_OUT_OF_MEMORY;
	}
	TEE_GetMemRegionList(mr, mrc, 0, 0);
	for (i = 0; i < mrc; i++)
		info("region %d, base=0x%08x, size=0x%08x, name=%s\n",
				i, mr[i].base, mr[i].size, mr[i].name);
	free(mr);
	return result;
}

TEE_Result Test_TEE_CheckMemoryTransferRights(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	TEE_Result result = TEE_SUCCESS;

	struct nameval {
		uint32_t value;
		const char *name;
	};

	const struct nameval mem[] = {
		{0x03000000, "secure"},
		{0x40000000, "nonsecure"},
		{0x00900000, "restricted"}
	};

	const struct nameval perm[] = {
		{TEE_SECURE, "secure"},
		{TEE_NONSECURE, "nonsecure"},
		{TEE_RESTRICTED, "restricted"}
	};

	const struct nameval op[] = {
		{TEE_MT_COPY, "copy"},
		{TEE_MT_ENCRYPT, "encrypt"},
		{TEE_MT_DECRYPT, "decrypt"}
	};

	int i, j, k;

	info("-------------- TEE_CheckMemoryTransferRights -------------\n");
	for (i = 0; i < ARRAY_SIZE(mem); i++) {
		for (j = 0; j < ARRAY_SIZE(mem); j++) {
			for (k = 0; k < ARRAY_SIZE(op); k++) {
				result = TEE_CheckMemoryTransferRights(
						(void *)mem[j].value, 4,
						(void *)mem[i].value, 4,
						op[k].value);
				info("%s %s to %s: %s\n",
						mem[i].name,
						op[k].name,
						mem[j].name,
						(result == TEE_SUCCESS) ?
						"SUCCESS" : "FAIL");
			}
		}
	}

	info("-------------- TEE_CheckMemoryOutputRights -------------\n");
	for (i = 0; i < ARRAY_SIZE(mem); i++) {
		for (j = 0; j < ARRAY_SIZE(perm); j++) {
			for (k = 0; k < ARRAY_SIZE(op); k++) {
				result = TEE_CheckMemoryOutputRights(
						(void *)mem[i].value, 4,
						perm[j].value,
						op[k].value);
				info("%s %s to %s: %s\n",
						perm[j].name,
						op[k].name,
						mem[i].name,
						(result == TEE_SUCCESS) ?
						"SUCCESS" : "FAIL");
			}
		}
	}

	info("-------------- TEE_CheckMemoryInputRights -------------\n");
	for (i = 0; i < ARRAY_SIZE(mem); i++) {
		for (j = 0; j < ARRAY_SIZE(perm); j++) {
			for (k = 0; k < ARRAY_SIZE(op); k++) {
				result = TEE_CheckMemoryInputRights(
						(void *)mem[i].value, 4,
						perm[j].value,
						op[k].value);
				info("%s %s to %s: %s\n",
						mem[i].name,
						op[k].name,
						perm[j].name,
						(result == TEE_SUCCESS) ?
						"SUCCESS" : "FAIL");
			}
		}
	}
	return TEE_SUCCESS;
}

TEE_Result Test_TEE_Mutex(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	TEE_Result result = TEE_SUCCESS;

	void *lock = NULL;
	result = TEE_MutexCreate(&lock, NULL);
	if (result != TEE_SUCCESS)
		error("TEE_MutexCreate() fail. result=0x%08x\n", result);

	result = TEE_MutexLock(lock);
	if (result != TEE_SUCCESS)
		error("TEE_MutexLock() fail. result=0x%08x\n", result);

	result = TEE_MutexUnlock(lock);
	if (result != TEE_SUCCESS)
		error("TEE_MutexUnlock() fail. result=0x%08x\n", result);

	result = TEE_MutexDestroy(lock);
	if (result != TEE_SUCCESS)
		error("TEE_MutexDestroy() fail. result=0x%08x\n", result);

	return result;
}

TEE_Result TA_TestAdd(TEE_TASessionHandle session,
		uint32_t a, uint32_t b)
{
	uint32_t origin;
	TEE_Result result;

	TEE_Param params[4];
	uint32_t paramTypes = TEE_PARAM_TYPES(
			TEE_PARAM_TYPE_VALUE_INPUT,
			TEE_PARAM_TYPE_VALUE_OUTPUT,
			TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE);
	params[0].value.a = a;
	params[0].value.b = b;
	params[1].value.a = 0xdeadbeef;

	result = TEE_InvokeTACommand(
			session,
			0,
			TEST_CMD_ADD,
			paramTypes,
			params,
			&origin);

	if (result != TEE_SUCCESS) {
		error("fail to invoke command. result=0x%08x, origin=%d\n",
				result, origin);
		return result;
	}

	if (params[1].value.a != (a + b)) {
		error("%d + %d = %d is not correct\n",
				a, b, params[1].value.a);
		return TEE_ERROR_COMMUNICATION;
	}

	return TEE_SUCCESS;
}

/* invoke command to a session to test memory.
 *
 * @param session	session handle.
 * @param buf		buffer virtual address in current TA.
 * @param pbuf		buffer virtual address in service TA.
 *			most case, it should be same as buf.
 * @param size		size of buffer to test.
 * @param pattern	test pattern. 0 for default.
 */
TEE_Result TA_TestMem(TEE_TASessionHandle session,
		void *buf, void *pbuf,
		size_t size, unsigned int pattern)
{
	uint32_t i;
	uint32_t count = size / sizeof(uint32_t);
	uint32_t *word = buf;
	uint32_t origin;
	TEE_Result result;
	TEE_Param params[4];
	uint32_t paramTypes = TEE_PARAM_TYPES(
			TEE_PARAM_TYPE_VALUE_INOUT,
			TEE_PARAM_TYPE_MEMREF_INOUT,
			TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE);

	/* generate inputs */
	for (i = 0; i < count; i++)
		word[i] = (pattern == 0) ? TEE_VirtToPhys(&word[i]) : pattern;

	params[0].value.a = pattern;
	params[0].value.b = 0;
	params[1].memref.buffer = pbuf;	/* pass peer buffer address */
	params[1].memref.size = size;

	/* Start the echo operation within the TEE application. */
	result = TEE_InvokeTACommand(
			session,
			0,
			TEST_CMD_MEM,
			paramTypes,
			params,
			&origin);

	/* verity the result */
	if (result != TEE_SUCCESS) {
		error("verify fail! result is not success: %d\n", result);
		return result;
	}

	/* verify outputs */
	for (i = 0; i < count; i++) {
		uint32_t got = word[i];
		uint32_t expect = ~((pattern == 0) ? (uint32_t)TEE_VirtToPhys(&word[i]): pattern);
		if (got != expect) {
			error("input data error @ %d word, 0x%08x != ~0x%08x\n",
					i, got, expect);
			return TEE_ERROR_COMMUNICATION;
		}
	}

	return TEE_SUCCESS;
}


TEE_Result Test_TEE_SharedMemory(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	const TEE_UUID uuid = TATEST_UUID;
	TEE_TASessionHandle session = NULL;
	TEE_Result result = TEE_SUCCESS;
	uint32_t origin = 0;

	result = TEE_OpenTASession(&uuid, 0,
			TEE_PARAM_TYPE_NONE, NULL,
			&session, &origin);

	if (result != TEE_SUCCESS) {
		error("fail to open session. result=0x%08x, origin=%d\n",
				result, origin);
		goto out;
	}

	TEE_ObjectHandle shm = NULL;
	void *va = NULL;
	void *pva = NULL;
	const size_t size = 4096;

	result = TEE_AllocateSharedMemory(0, size, TEE_SHM_SESSION_PRIVATE,
			session, &shm, &va);
	assert(result == TEE_SUCCESS);

	result = TEE_MapSharedMemoryToPeer(session, shm, &pva);
	assert(result == TEE_SUCCESS);

	result = TA_TestMem(session, va, pva, size, 0);
	assert(result == TEE_SUCCESS);

	result = TEE_UnmapSharedMemoryFromPeer(session, shm, pva);
	assert(result == TEE_SUCCESS);

	result = TEE_ReleaseSharedMemory(shm);
	assert(result == TEE_SUCCESS);
out:
	if (session)
		TEE_CloseTASession(session);

	return result;
}
