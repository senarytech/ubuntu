#include "tee_internal_api.h"
#include "tee_ta_entry.h"
#include "tee_ta_rpmb_perf.h"

TEE_Result TA_EXPORT TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

void TA_EXPORT TA_DestroyEntryPoint(void)
{
	return;
}

TEE_Result TA_EXPORT TA_OpenSessionEntryPoint(uint32_t paramTypes,
                                              TEE_Param params[4],
                                              void **sessionContext)
{
	return TEE_SUCCESS;
}

void TA_EXPORT TA_CloseSessionEntryPoint(void *sessionContext)
{
	return;
}

TEE_Result TA_EXPORT TA_InvokeCommandEntryPoint(void *sessionContext,
                                                uint32_t commandID,
                                                uint32_t paramTypes,
                                                TEE_Param params[4])
{
	TEE_Result result = TEE_ERROR_GENERIC;

	uint32_t expectParamTypes = TEE_PARAM_TYPES(
			TEE_PARAM_TYPE_VALUE_INOUT,
			TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE);

	if(expectParamTypes != paramTypes)
		return TEE_ERROR_BAD_PARAMETERS;

	switch(commandID) {
		case RPMB_PERF_READ:
			result = RPMB_Read_Perf_Test(params);
			break;
		case RPMB_PERF_WRITE:
			result = RPMB_Write_Perf_Test(params);
			break;
		default:
			break;
	}

	return result;
}

TA_DEFINE_BEGIN
	.uuid                   = RPMB_PERF_UUID,
	.name                   = "RPMB_PERF_TEST",
	.singleInstance         = true,
	.multiSession           = true,
	.instanceKeepAlive      = true,
TA_DEFINE_END
