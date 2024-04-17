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

TEE_Result TA_EXPORT TA_InvokeCommandEntryPoint(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	TEE_LOGI("commandID=%d, paramTypes=0x%08x\n", commandID, paramTypes);

	return TEE_SUCCESS;
}

TA_DEFINE_BEGIN
	.uuid			= TASAMPLE_UUID,
	.name			= "SampleTA",
	.singleInstance		= false,
	.multiSession		= false,
	.instanceKeepAlive	= false,
TA_DEFINE_END
