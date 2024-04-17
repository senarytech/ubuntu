#include "config.h"
#include "tee_ta_test.h"
#include "log.h"

/*
 * params[0]: input virables (a, b)
 * params[1]: output results
 */
TEE_Result TestOperate(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_VALUE_INPUT
			|| TEE_PARAM_TYPE_GET(paramTypes, 1) != TEE_PARAM_TYPE_VALUE_OUTPUT)
		return TEE_ERROR_BAD_PARAMETERS;

	switch (commandID) {
	case TEST_CMD_INC:
		params[1].value.a = params[0].value.a + 1;
		break;
	case TEST_CMD_DEC:
		params[1].value.a = params[0].value.a - 1;
		break;
	case TEST_CMD_ADD:
		params[1].value.a = params[0].value.a + params[0].value.b;
		break;
	case TEST_CMD_SUB:
		params[1].value.a = params[0].value.a - params[0].value.b;
		break;
	case TEST_CMD_MUL:
		params[1].value.a = params[0].value.a * params[0].value.b;
		break;
	case TEST_CMD_DIV:
		params[1].value.a = params[0].value.a / params[0].value.b;
		break;
	}
	return TEE_SUCCESS;
}

/*
 * param[0].value.a	- fill pattern, must be same as buffer data. if it's 0,
 *			  then the first data is address, increase 4 for each
 *			  word.
 * param[0].value.b	- return the processed lenggh, must be same as size
 * memref.buffer	- buffer, must be 4bytes aligned. return reversed data
 *			  of input
 * memref.size		- data length
 */
TEE_Result TestMem(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_VALUE_INOUT ||
		TEE_PARAM_TYPE_GET(paramTypes, 1) != TEE_PARAM_TYPE_MEMREF_INOUT)
		return TEE_ERROR_BAD_PARAMETERS;

	uint32_t i;
	uint32_t pattern = params[0].value.a;
	uint32_t count = params[1].memref.size / sizeof(uint32_t);
	uint32_t *word = (uint32_t *)params[1].memref.buffer;

	/* verify inputs */
	for (i = 0; i < count; i++) {
		uint32_t got = word[i];
		uint32_t expect = (pattern == 0) ? TEE_VirtToPhys(&word[i]) : pattern;
		if (got != expect) {
			error("input data error @ %d word, 0x%08x != 0x%08x\n",
					i, got, expect);
			return TEE_ERROR_COMMUNICATION;
		}
	}

	/* reverse the data */
	for (i = 0; i < count; i++)
		word[i] = ~word[i];

	params[0].value.b = count * sizeof(uint32_t);

	return TEE_SUCCESS;
}

TEE_Result TestReverse(
	void*			sessionContext,
	uint32_t		commandID,
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

TEE_Result TestAllocate(
	void*			sessionContext,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{
	void *mem[100] = {NULL};

	int i;
	for (i = 0; i < 100; i++) {
		mem[i] = malloc(512 << 10);
		if (mem[i] == NULL) {
			info("fail to alloc memory %d\n", i);
			break;
		}
	}

	i = 0;
	while (mem[i] != NULL) {
		free(mem[i]);
		i++;
	}

	return TEE_SUCCESS;
}
