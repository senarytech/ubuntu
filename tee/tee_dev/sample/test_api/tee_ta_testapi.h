#ifndef _TEE_TA_TESTAPI_H_
#define _TEE_TA_TESTAPI_H_

#include "testapi_cmd.h"
#include "tee_internal_api.h"

TEE_Result TestOperate(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4]);

TEE_Result TestMem(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4]);

TEE_Result TestReverse(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4]);

#endif /* _TEE_TA_TESTAPI_H_ */
