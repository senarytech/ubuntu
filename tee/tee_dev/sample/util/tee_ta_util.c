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

#include "config.h"
#include "tee_internal_api.h"
#include "util_cmd.h"
#include "log.h"

TEE_Result TA_EXPORT TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

void TA_EXPORT TA_DestroyEntryPoint(void)
{
}

TEE_Result TA_EXPORT TA_OpenSessionEntryPoint(
	uint32_t		paramTypes,
	TEE_Param		params[4],
	void**			sessionContext)
{
	return TEE_SUCCESS;
}

void TA_EXPORT TA_CloseSessionEntryPoint(void*  sessionContext)
{
}

static TEE_Result MemCopy(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_MEMREF_INPUT ||
		TEE_PARAM_TYPE_GET(paramTypes, 1) != TEE_PARAM_TYPE_MEMREF_INOUT)
		return TEE_ERROR_BAD_PARAMETERS;

	TEE_Result res;

	size_t len = params[0].memref.size;
	uint8_t *in = params[0].memref.buffer;
	uint8_t *out = params[1].memref.buffer;

	if (params[1].memref.size < len) {
		/* in these example, all output size is same as input size */
		error("output length must be >= input length\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}
	res = TEE_CheckMemoryTransferRights(out, len, in, len, TEE_MT_COPY);
	if (res == TEE_SUCCESS) {
		TEE_MemMove(out, in, len);
		/* output size is same as input size */
		params[1].memref.size = len;
	} else {
		params[1].memref.size = 0;
	}

	return res;
}

TEE_Result TA_EXPORT TA_InvokeCommandEntryPoint(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	TEE_Result res = TEE_ERROR_NOT_SUPPORTED;

	trace("commandID=%d, paramTypes=0x%08x\n", commandID, paramTypes);

	switch (commandID) {
	case CMD_MEM_COPY:
		res = MemCopy(sessionContext, paramTypes, params);
		break;
	}

	return res;
}

TA_DEFINE_BEGIN
	.uuid			= UTILTA_UUID,
	.name			= "UtilTA",
	.singleInstance		= false,
	.multiSession		= false,
	.instanceKeepAlive	= false,
TA_DEFINE_END
