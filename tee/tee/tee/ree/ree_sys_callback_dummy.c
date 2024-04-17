#include "config.h"
#include "ree_sys_callback_cmd.h"
#include "tee_comm.h"
#include "tee_ta_mgr.h"
#include "delay.h"
#include "clock_source.h"
#include "log.h"

/*
 * Command functions
 */

static TEEC_Result REESysCmd_GetTime(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_VALUE_OUTPUT)
		return TEEC_ERROR_BAD_PARAMETERS;

	uint32_t s, us;

	clocksource_read_timestamp(&s, &us);

	params[0].value.a = s;
	params[0].value.b = us;

	return TEEC_SUCCESS;
}

static TEEC_Result REESysCmd_Wait(
		void*			userdata,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_VALUE_INPUT)
		return TEEC_ERROR_BAD_PARAMETERS;

	unsigned long delay_ms = params[0].value.a;
	mdelay(delay_ms);

	return TEEC_SUCCESS;
}

TEEC_Result REE_InvokeSysCommandEntryPoint(
		void*			userdata,
		uint32_t		commandID,
		uint32_t		paramTypes,
		TEE_Param		params[4],
		void*			param_ext,
		uint32_t		param_ext_size)
{
	TEEC_Result res = TEEC_ERROR_NOT_SUPPORTED;

	trace("commandID=%d, paramTypes=0x%08x\n", commandID, paramTypes);

	switch (commandID) {
	case REE_CMD_TIME_GET:
		res = REESysCmd_GetTime(userdata, paramTypes,
				params, param_ext, param_ext_size);
		break;
	case REE_CMD_TIME_WAIT:
		res = REESysCmd_Wait(userdata, paramTypes,
				params, param_ext, param_ext_size);
		break;
	}

	return res;
}
