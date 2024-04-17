#include "config.h"
#include "tee_internal_api.h"
#include "log.h"

TEE_Result AESGenKey(const void *key, size_t keyBits, TEE_ObjectHandle *object)
{
	TEE_Result res;
	TEE_Attribute attr;

	if (!object || !key || ((keyBits != 128) &&
				(keyBits != 192) && (keyBits != 256))) {
		error("bad key, keyBits=%d\n", keyBits);
		return TEE_ERROR_BAD_PARAMETERS;
	}

	res = TEE_AllocateTransientObject(TEE_TYPE_AES, keyBits, object);
	if (res != TEE_SUCCESS) {
		error("fail to allocate transitent object, res = 0x%08x\n", res);
		return res;
	}
	TEE_RestrictObjectUsage(*object, TEE_USAGE_ENCRYPT | TEE_USAGE_DECRYPT);

	TEE_InitRefAttribute(&attr, TEE_ATTR_SECRET_VALUE, key, keyBits/8);
	res = TEE_PopulateTransientObject(*object, &attr, 1);
	if (res != TEE_SUCCESS) {
		error("fail to populate attribute\n");
		TEE_FreeTransientObject(*object);
	}
	return res;
}

TEE_Result AESCipher(void *destData, const void *srcData, size_t len,
		uint32_t mode, TEE_ObjectHandle key,
		const void *IV, size_t IVLen)
{
	TEE_Result res;
	uint32_t alg = TEE_ALG_AES_CBC_NOPAD;
	size_t destLen = len;
	size_t keyBits = 0;

	TEE_OperationHandle operation = NULL;

	if (!destData || !srcData || !len || !IV || !IVLen) {
		error("bad parameters\n");
		return TEE_ERROR_BAD_PARAMETERS;
	}

	res = TEE_GetObjectBufferAttribute(key, TEE_ATTR_SECRET_VALUE,
			NULL, &keyBits);
	keyBits *= 8;
	if (res != TEE_SUCCESS) {
		error("fail to get key attribute, res=0x%08x\n", res);
		return res;
	}

	res = TEE_AllocateOperation(&operation, alg, mode, keyBits);
	if (res != TEE_SUCCESS) {
		error("fail to allocate operation, res=0x%08x\n", res);
		return res;
	}

	res = TEE_SetOperationKey(operation, key);
	if (res != TEE_SUCCESS) {
		error("fail to set operation key, res=0x%08x\n", res);
		goto failed;
	}

	TEE_CipherInit(operation, IV, IVLen);
	res = TEE_CipherUpdate(operation, srcData, len, destData, &destLen);
	if (res != TEE_SUCCESS) {
		error("fail to execute TEE_CipherUpdate, res=0x%08x\n", res);
		goto failed;
	}

	res = TEE_CipherDoFinal(operation, NULL, 0, NULL, NULL);
	if (res != TEE_SUCCESS) {
		error("fail to execute TEE_CipherDoFinal, res=0x%08x\n", res);
		goto failed;
	}

failed:
	TEE_FreeOperation(operation);
	return res;
}

/*
 * Test AES Cipher
 */
static void dumpData(uint8_t *data, size_t len, const char *info)
{
	size_t i;

	printf("---------------------------%s dump start-------------------------\n", info);
	for (i = 0; i < len; i++)
		printf("%02x ", data[i]);
	printf("\n---------------------------%s dump end-------------------------\n", info);
}

TEE_Result AESTest(void)
{
	uint8_t keyData[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	uint8_t IV[]  = {0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad};
	const size_t len = 256;
	uint8_t src[len];
	uint8_t enc[len];
	uint8_t dec[len];
	size_t i;
	TEE_Result res;

	TEE_ObjectHandle key = NULL;

	res = AESGenKey(keyData, sizeof(keyData), &key);
	if (res != TEE_SUCCESS) {
		error("failed to generate the key object\n");
		return res;
	}

	/* prepare test data */
	for (i = 0; i < len; i++)
		src[i] = i;

	dumpData(src, len, "original");

	/* encryption */

	res = AESCipher(enc, src, len, TEE_MODE_ENCRYPT,
			key, IV, sizeof(IV));

	if (res != TEE_SUCCESS) {
		error("fail to encrypt, res=0x%08x\n", res);
		return res;
	}

	dumpData(enc, len, "encrypted");

	/* decryption */

	res = AESCipher(dec, enc, len, TEE_MODE_DECRYPT,
			key, IV, sizeof(IV));

	if (res != TEE_SUCCESS) {
		error("fail to decrypt, res=0x%08x\n", res);
		return res;
	}

	dumpData(dec, len, "decrypted");

	for (i = 0; i < len; i++) {
		if (dec[i] != src[i])
			error("decrypted data is not correct at %d\n", i);
	}

	return res;
}
