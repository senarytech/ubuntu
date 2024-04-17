#include <string.h>
#include <stdio.h>
#include "tee_common.h"
#include "tee_internal_common.h"
#include "tee_internal_core_common.h"
#include "tee_internal_api.h"
#include "tee_internal_private_api.h"
#include "tee_ta_entry.h"
#include "tee_ta_rpmb_perf.h"

#define  TA_POBJ_STORAGE_MEDIUM TEE_STORAGE_PRIVATE

TEE_Result RPMB_Read_Perf_Test(TEE_Param params[4])
{
	TEE_Result res = TEE_SUCCESS;
	TEE_ObjectHandle TrustStorageTest;
	TEE_Time begin, time;
	uint32_t count;

	/* test case:
	 * 1. open/create persistent object
	 * 2. read object data
	 * 3. close persistent object
	 * */
	res = TEE_OpenPersistentObject (TA_POBJ_STORAGE_MEDIUM,
			RPMB_PERF_TEST_TA_TRUST_STORAGE_ID, strlen(RPMB_PERF_TEST_TA_TRUST_STORAGE_ID),
			TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE,
			&TrustStorageTest);
	if (res == TEE_ERROR_ITEM_NOT_FOUND) {
		printf("The persistent object not present..creating...! \n");
		TEE_CloseAndDeletePersistentObject (TrustStorageTest);
		res = TEE_CreatePersistentObject(TA_POBJ_STORAGE_MEDIUM,
				RPMB_PERF_TEST_TA_TRUST_STORAGE_ID, strlen(RPMB_PERF_TEST_TA_TRUST_STORAGE_ID),
				TEE_DATA_FLAG_ACCESS_READ |TEE_DATA_FLAG_ACCESS_WRITE,
				0,
				&begin, sizeof(begin),
				&TrustStorageTest);
		if (res) {
			printf("TEE_CreatePersistentObject & TEE_OpenPersistentObject failed with res = 0x%08x\n", (unsigned int)res);
			return res;
		}
	}

	TEE_SeekObjectData(TrustStorageTest, 0,TEE_DATA_SEEK_SET);
	TEE_GetSystemTime(&begin);
	res = TEE_WriteObjectData(TrustStorageTest, &begin, sizeof(begin));
	if (res) {
		printf("TEE_WriteObjectData failed with res = 0x%08x\n", (unsigned int)res);
		TEE_CloseObject(TrustStorageTest);
		return res;
	}

	TEE_SeekObjectData(TrustStorageTest, 0,TEE_DATA_SEEK_SET);
	res = TEE_ReadObjectData(TrustStorageTest, &time, sizeof(time), &count);
	if (res) {
		printf("TEE_ReadObjectData failed with res = 0x%08x\n", (unsigned int)res);
		TEE_CloseObject(TrustStorageTest);
		return res;
	}

	if (memcmp(&begin, &time, sizeof(begin))) {
		printf("Read object failed with data mismatch\n");
		return TEE_ERROR_GENERIC;
	}

	TEE_CloseObject(TrustStorageTest);

	return res;
}

TEE_Result RPMB_Write_Perf_Test(TEE_Param params[4])
{
	TEE_Result res = TEE_SUCCESS;
	TEE_ObjectHandle TrustStorageTest;
	TEE_Time begin, end, time;
	uint32_t count;

	/* test case:
	 * 1. open/create persistent object
	 * 2. write object data
	 * 3. close persistent object
	 * */
	res = TEE_OpenPersistentObject(TA_POBJ_STORAGE_MEDIUM,
			RPMB_PERF_TEST_TA_TRUST_STORAGE_ID, strlen(RPMB_PERF_TEST_TA_TRUST_STORAGE_ID),
			TEE_DATA_FLAG_ACCESS_WRITE_META | TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE,
			&TrustStorageTest);
	if (res == TEE_ERROR_ITEM_NOT_FOUND) {
		printf("The persistent object not present..creating...! \n");
		TEE_CloseAndDeletePersistentObject(TrustStorageTest);
		res = TEE_CreatePersistentObject(TA_POBJ_STORAGE_MEDIUM,
				RPMB_PERF_TEST_TA_TRUST_STORAGE_ID, strlen(RPMB_PERF_TEST_TA_TRUST_STORAGE_ID),
				TEE_DATA_FLAG_ACCESS_WRITE_META | TEE_DATA_FLAG_ACCESS_READ |TEE_DATA_FLAG_ACCESS_WRITE,
				0,
				&begin, sizeof(begin),
				&TrustStorageTest);
		if (res) {
			printf("TEE_CreatePersistentObject/TEE_OpenPersistentObject failed with res = 0x%08x\n", (unsigned int)res);
			return res;
		}
	}

	TEE_SeekObjectData(TrustStorageTest, 0,TEE_DATA_SEEK_SET);
	TEE_GetSystemTime(&begin);
	res = TEE_WriteObjectData(TrustStorageTest, &begin, sizeof(begin));
	if (res) {
		printf("TEE_WriteObjectData failed with res = 0x%08x\n", (unsigned int)res);
		TEE_CloseObject(TrustStorageTest);
		return res;
	}
	TEE_GetSystemTime(&end);

	params[0].value.a = (end.seconds * 1000 + end.millis) - (begin.seconds * 1000 + begin.millis);
	printf("TEE_WriteObjectData used time = %d ms\n", (int)params[0].value.a);

	TEE_CloseObject(TrustStorageTest);

	/* test case:
	 * 1. open persistent object
	 * 2. read object data
	 * 3. close&delete persistent object
	 * */
	res = TEE_OpenPersistentObject(TA_POBJ_STORAGE_MEDIUM,
			RPMB_PERF_TEST_TA_TRUST_STORAGE_ID, strlen(RPMB_PERF_TEST_TA_TRUST_STORAGE_ID),
			TEE_DATA_FLAG_ACCESS_WRITE_META | TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE,
			&TrustStorageTest);
	if (res) {
		printf("TEE_OpenPersistentObject failed with res = 0x%08x\n", (unsigned int)res);
		return res;
	}

	TEE_SeekObjectData(TrustStorageTest, 0,TEE_DATA_SEEK_SET);
	TEE_GetSystemTime(&begin);
	res = TEE_ReadObjectData(TrustStorageTest, &time, sizeof(time), &count);
	if (res) {
		printf("TEE_ReadObjectData failed \n");
		TEE_CloseAndDeletePersistentObject(TrustStorageTest);
		return res;
	}
	TEE_GetSystemTime(&end);

	printf("TEE_ReadObjectData used time = %d ms\n", (int)((end.seconds * 1000 + end.millis) - (begin.seconds * 1000 + begin.millis)));

	TEE_CloseAndDeletePersistentObject(TrustStorageTest);

	return TEE_SUCCESS;
}
