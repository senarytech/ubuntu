/*
 * Copyright (c) 2013-2014 Marvell International Ltd. and its affiliates.
 * All rights reserved.
 *
 * This software file (the "File") is owned and distributed by Marvell
 * International Ltd. and/or its affiliates ("Marvell") under the following
 * licensing terms.
 *
 * If you received this File from Marvell and you have entered into a
 * commercial license agreement (a "Commercial License") with Marvell, the
 * File is licensed to you under the terms of the applicable Commercial
 * License.
 */

#include "tee_internal_api.h"
#include "tee_sample_cmd.h"
#include "tee_log.h"

TEE_Result TA_EXPORT TA_CreateEntryPoint(void)
{
	TEE_LOGI("\n");
	return TEE_SUCCESS;
}

void TA_EXPORT TA_DestroyEntryPoint(void)
{
	TEE_LOGI("\n");
}

TEE_Result TA_EXPORT TA_OpenSessionEntryPoint(
	uint32_t		paramTypes,
	TEE_Param		params[4],
	void**			sessionContext)
{
	TEE_LOGI("\n");
	return TEE_SUCCESS;
}

void TA_EXPORT TA_CloseSessionEntryPoint(void*  sessionContext)
{
	TEE_LOGI("\n");
}

static TEE_Result OperateValue(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_VALUE_INPUT ||
		TEE_PARAM_TYPE_GET(paramTypes, 1) != TEE_PARAM_TYPE_VALUE_OUTPUT)
		return TEE_ERROR_BAD_PARAMETERS;

	switch (commandID) {
	case CMD_VAL_ADD:
		params[1].value.a = params[0].value.a + params[0].value.b;
		break;
	case CMD_VAL_SUB:
		params[1].value.a = params[0].value.a - params[0].value.b;
		break;
	default:
		return TEE_ERROR_NOT_SUPPORTED;
	}
	return TEE_SUCCESS;
}

TEE_Result TA_EXPORT TA_InvokeCommandEntryPoint(
	void*		   sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param	       params[4])
{
	TEE_Result result = TEE_ERROR_NOT_SUPPORTED;

	TEE_LOGI("commandID=%d, paramTypes=0x%08x\n", commandID, paramTypes);

	switch (commandID) {
	case CMD_VAL_ADD:
	case CMD_VAL_SUB:
		result = OperateValue(sessionContext, commandID, paramTypes, params);
		break;
	}

	return result;
}

TA_DEFINE_BEGIN
	.uuid			= TASAMPLE_UUID,
	.name			= "SampleTA",
	.singleInstance		= false,
	.multiSession		= false,
	.instanceKeepAlive	= false,
TA_DEFINE_END
