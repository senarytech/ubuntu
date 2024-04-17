#include "config.h"
#include "tee_ta_testapi.h"

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
	uint32_t		flag;
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

	case TEST_CMD_CANCELLATION:

		while (1) {
			flag = TEE_GetCancellationFlag();
			if (flag)
				return TEE_ERROR_CANCEL;
		}
		break;
	}
	return TEE_SUCCESS;
}


static int ZspDmaM2M(const void *src, void *dst, unsigned int size)
{
/*	struct master *zsp = &master_zsp_dma;
	unsigned int len;

	zsp->reset();
	len = zsp->m2m(src, dst, size);
*/
	return size;
}

static void FillMem(uint32_t *src, int count, int pattern)
{
	int i;
	for(i = 0; i < count; i++) {

		src[i] = pattern;
	}


}

/*
 * param[0].value.a	- expect pattern, compare with zsp dst data.
 * param[0].value.b	- return the processed lenggh, must be same as size
 * memref.buffer	- buffer, must be 4bytes aligned, input for zsp src.
 * memref.size		- data length
 * memref.buffer	- buffer, must be 4bytes aligned, input for zsp dst..
 * memref.size		- data length
 */
TEE_Result TestCache(
	void*			sessionContext,
	uint32_t		paramTypes,
	TEE_Param		params[4])
{


	int res = 1;
	int processed = 0;

	uint32_t i;
	uint32_t count;
	uint32_t *cache_src;
	uint32_t *nocache_src;

	int	pattern = 0xa5a5a5a5;

	count = params[0].value.b;
	cache_src = (uint32_t*)params[0].value.a;
	nocache_src = (uint32_t*)params[1].value.a;

	if (TEE_PARAM_TYPE_GET(paramTypes, 0) != TEE_PARAM_TYPE_VALUE_INOUT ||
		TEE_PARAM_TYPE_GET(paramTypes, 1) != TEE_PARAM_TYPE_VALUE_INOUT)
		return TEE_ERROR_BAD_PARAMETERS;

	/* case 1 : don't flush the cache, verify will fail*/


	printf("cache at %x, nocache at %x, count = %x\n", cache_src, nocache_src, count);

	nocache_src[0] = 0x5a5a5a5a;

	printf("2 cache at %x, nocache at %x, count = %x\n", cache_src, nocache_src, count);
	FillMem(nocache_src, count>>2, 0x5a5a5a5a);

	FillMem(cache_src, count>>2, pattern);



	res = 1;
	/* verify with the zsp outputs */
	for (i = 0; i < count>>2; i++) {
		uint32_t got = nocache_src[i];
		uint32_t expect = pattern;
		if (got != expect) {
			printf("zsp output data not match @ %d word, 0x%08x != 0x%08x\n",
					i, got, expect);

			res = TEE_SUCCESS;
			break;
		}
	}

	if(res != TEE_SUCCESS) {
		return TEE_ERROR_COMMUNICATION;

	}else{
		printf(" no flush, not  match, this is ok.\n");
	}

	/*case 2 : flush the cache, verify will success*/

	TEE_FlushCache(cache_src, count);


	res = TEE_SUCCESS;
	/* verify with the zsp outputs */
	for (i = 0; i < count>>2; i++) {
		uint32_t got = nocache_src[i];
		uint32_t expect = pattern;
		if (got != expect) {
			printf("zsp output data error @ %d word, 0x%08x != 0x%08x\n",
					i, got, expect);
			res = TEE_ERROR_COMMUNICATION;
			break;
		}
	}


	return res;
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
		uint32_t expect = (pattern == 0) ? (uint32_t)&word[i] : pattern;
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
