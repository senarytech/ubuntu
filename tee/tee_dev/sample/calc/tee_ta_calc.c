#include "config.h"
#include "tee_internal_api.h"
#include "calc_cmd.h"
#include "tee_ta_calc.h"
#include "string.h"
#define LOG_TAG "TA_CALC"
#include "tee_log.h"

static uint32_t totalTACalcSessionNum = 0;
static uint32_t totalTACalcSessionHandle = 0;

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
	totalTACalcSessionNum++;
	totalTACalcSessionHandle++;
	*sessionContext = (void *)totalTACalcSessionHandle;

	TEE_LOGI("total session %d\n", totalTACalcSessionNum);
	return TEE_SUCCESS;
}

void TA_EXPORT TA_CloseSessionEntryPoint(void*  sessionContext)
{
	if (totalTACalcSessionNum > 0)
		totalTACalcSessionNum--;

	TEE_LOGI("total session %d\n", totalTACalcSessionNum);
}

/*
 * params[0]: input virables (a, b)
 * params[1]: output results
 */
static TEE_Result CalcOperate(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_VALUE_INPUT
			|| TEE_PARAM_TYPE_GET(paramTypes, 1) != TEE_PARAM_TYPE_VALUE_OUTPUT)
		return TEE_ERROR_BAD_PARAMETERS;

	switch (commandID) {
	case CALC_CMD_INC:
		params[1].value.a = params[0].value.a + 1;
		break;
	case CALC_CMD_DEC:
		params[1].value.a = params[0].value.a - 1;
		break;
	case CALC_CMD_ADD:
		params[1].value.a = params[0].value.a + params[0].value.b;
		break;
	case CALC_CMD_SUB:
		params[1].value.a = params[0].value.a - params[0].value.b;
		break;
	case CALC_CMD_MUL:
		params[1].value.a = params[0].value.a * params[0].value.b;
		break;
	case CALC_CMD_DIV:
		params[1].value.a = params[0].value.a / params[0].value.b;
		break;
	}
	return TEE_SUCCESS;
}

static TEE_Result CalcEcho(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_MEMREF_INOUT)
		return TEE_ERROR_BAD_PARAMETERS;

	TEE_LOGI("%s", params[0].memref.buffer);

	params[0].memref.size = strlen(params[0].memref.buffer);

	return TEE_SUCCESS;
}

static TEE_Result CalcReverse(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_VALUE_INOUT ||
		TEE_PARAM_TYPE_GET(paramTypes, 1) != TEE_PARAM_TYPE_MEMREF_INOUT)
		return TEE_ERROR_BAD_PARAMETERS;

	uint32_t i;
	uint32_t len = params[0].value.a;
	uint8_t *buf = (uint8_t *)params[1].memref.buffer;

	/* reverse the data */
	for (i = 0; i < len; i++)
		buf[i] = ~buf[i];

	params[0].value.b = len;

	return TEE_SUCCESS;
}

static TEE_Result CalcConvert(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	TEE_Result res = TEE_SUCCESS;
	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_VALUE_INPUT
			|| TEE_PARAM_TYPE_GET(paramTypes, 1) != TEE_PARAM_TYPE_MEMREF_OUTPUT)
		return TEE_ERROR_BAD_PARAMETERS;

	switch (params[0].value.b) {
	case 10:
		params[1].memref.size = sprintf(params[1].memref.buffer, "%d", params[0].value.a);
		break;
	case 16:
		params[1].memref.size = sprintf(params[1].memref.buffer, "%08x", params[0].value.a);
		break;
	default:
		res = TEE_ERROR_NOT_SUPPORTED;
	}

	return res;
}

static void CalcDump(int cmdId, int a, int b, int result)
{
	const char * cmd[] = {"inc", "dec", "add", "sub", "mul",
				"div", "echo", "convert", "reverse"};

	TEE_LOGI("\t%s(%d, %d) = 0x%08x\n", cmd[cmdId], a, b, result);
}

TEE_Result TA_EXPORT TA_InvokeCommandEntryPoint(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	TEE_Result res = TEE_ERROR_NOT_SUPPORTED;

	TEE_LOGI("commandID=%d, paramTypes=0x%08x\n", commandID, paramTypes);

	switch (commandID) {
	case CALC_CMD_INC:
	case CALC_CMD_DEC:
	case CALC_CMD_ADD:
	case CALC_CMD_SUB:
	case CALC_CMD_MUL:
	case CALC_CMD_DIV:
		res = CalcOperate(sessionContext, commandID, paramTypes, params);
		break;
	case CALC_CMD_ECHO:
		res = CalcEcho(sessionContext, paramTypes, params);
		break;
	case CALC_CMD_CONVERT:
		res = CalcConvert(sessionContext, paramTypes, params);
		break;
	case CALC_CMD_REVERSE:
		res = CalcReverse(sessionContext, paramTypes, params);
		break;
	}

#if 0
	CalcDump(commandID, params[0].value.a, params[0].value.b, params[1].value.a);
#endif
	return res;
}

TA_DEFINE_BEGIN
	.uuid			= TACALC_UUID,
	.name			= "Calculator",
	.singleInstance		= false,
	.multiSession		= false,
	.instanceKeepAlive	= false,
TA_DEFINE_END
