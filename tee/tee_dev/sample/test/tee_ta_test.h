#ifndef _TEE_TA_TEST_H_
#define _TEE_TA_TEST_H_

#include "test_cmd.h"
#include "tee_internal_api.h"

TEE_Result TestOperate(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4]);

TEE_Result TestMem(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4]);

TEE_Result TestReverse(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4]);

TEE_Result Test_TEE_Time(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4]);

TEE_Result Test_TEE_Callback(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4]);

TEE_Result Test_TEE_GetMemRegionList(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4]);

TEE_Result Test_TEE_CheckMemoryTransferRights(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4]);

TEE_Result Test_TEE_Mutex(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4]);

#endif /* _TEE_TA_TEST_H_ */
