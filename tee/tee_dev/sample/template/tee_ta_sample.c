#include "config.h"
#include "tee_internal_api.h"
#include "sample_cmd.h"
#include "tee_ta_aes.h"
#include "log.h"

static uint32_t totalSessionNum = 0;

/* secret key and IV data */
static const uint8_t keyData[16] =
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
static const uint8_t IV[]  =
	{0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad};
static TEE_ObjectHandle key = NULL;

TEE_Result TA_CreateEntryPoint(void)
{
	/* Prepare the key */
	return AESGenKey(keyData, sizeof(keyData) * 8, &key);
}

void TA_DestroyEntryPoint(void)
{
}

TEE_Result TA_OpenSessionEntryPoint(
	uint32_t		paramTypes,
	TEE_Param		params[4],
	void**			sessionContext)
{
	totalSessionNum++;
	*sessionContext = (void *)totalSessionNum;

	trace("total session %d\n", totalSessionNum);
	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void*  sessionContext)
{
	if (totalSessionNum > 0)
		totalSessionNum--;

	trace("total session %d\n", totalSessionNum);
}

/*
 * params[0]: input virables (a, b)
 * params[1]: output results
 */
static TEE_Result OperateValue(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_VALUE_INPUT
			|| TEE_PARAM_TYPE_GET(paramTypes, 1) != TEE_PARAM_TYPE_VALUE_OUTPUT)
		return TEE_ERROR_BAD_PARAMETERS;

	switch (commandID) {
	case CMD_VAL_ADD:
		params[1].value.a = params[0].value.a + params[0].value.b;
		break;
	case CMD_VAL_SUB:
		params[1].value.a = params[0].value.a - params[0].value.b;
		break;
	}
	return TEE_SUCCESS;
}

static TEE_Result OperateMemory(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_MEMREF_INPUT ||
		TEE_PARAM_TYPE_GET(paramTypes, 1) != TEE_PARAM_TYPE_MEMREF_INOUT)
		return TEE_ERROR_BAD_PARAMETERS;

	TEE_Result res = TEE_ERROR_NOT_SUPPORTED;

	size_t len = params[0].memref.size;
	uint8_t *in = params[0].memref.buffer;
	uint8_t *out = params[1].memref.buffer;

	if (params[1].memref.size < len) {
		/* in these example, all output size is same as input size */
		error("output length must be >= input length\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	switch (commandID) {
	case CMD_MEM_REVERSE:
		{
			size_t i;
			/* reverse the data */
			for (i = 0; i < len; i++)
				out[i] = ~in[i];
			res = TEE_SUCCESS;
		}
		break;
	case CMD_AES_ENCRYPT:
		res = AESCipher(out, in, len, TEE_MODE_ENCRYPT,
			key, IV, sizeof(IV));
		break;
	case CMD_AES_DECRYPT:
		res = AESCipher(out, in, len, TEE_MODE_DECRYPT,
			key, IV, sizeof(IV));
		break;
	}
	/* output size is same as input size */
	params[1].memref.size = len;
	return res;
}

TEE_Result TA_InvokeCommandEntryPoint(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	TEE_Result res = TEE_ERROR_NOT_SUPPORTED;

	trace("commandID=%d, paramTypes=0x%08x\n", commandID, paramTypes);

	switch (commandID) {
	case CMD_VAL_ADD:
	case CMD_VAL_SUB:
		res = OperateValue(sessionContext, commandID, paramTypes, params);
		break;
	case CMD_MEM_REVERSE:
	case CMD_AES_ENCRYPT:
	case CMD_AES_DECRYPT:
		res = OperateMemory(sessionContext, commandID, paramTypes, params);
		break;
	}

	return res;
}

TA_DEFINE_BEGIN
	.uuid			= TA_UUID,
	.name			= "SampleTA",
	.singleInstance		= false,
	.multiSession		= false,
	.instanceKeepAlive	= false,
TA_DEFINE_END
