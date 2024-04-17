#include "config.h"
#include "tee_internal_api.h"
#include "log.h"
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/modes.h>
#include <openssl/modes_lcl.h>
#include <openssl/rsa.h>
#include <openssl/dsa.h>


bool AESEncrypt(void *destData, int *destLen,
		const void *srcData, int srcLen,
		const void *keyValue, const void *IV)
{
	EVP_CIPHER_CTX ctx;
	const EVP_CIPHER *cipher = EVP_aes_128_cbc();

	EVP_CIPHER_CTX_init(&ctx);
	EVP_EncryptInit_ex(&ctx, cipher, NULL, keyValue, IV);
	EVP_CIPHER_CTX_set_padding(&ctx, 0);
	if (!EVP_EncryptUpdate(&ctx, destData, destLen, srcData, srcLen))
		goto failed;

	if (!EVP_EncryptFinal_ex(&ctx, destData, destLen))
		goto failed;

	EVP_CIPHER_CTX_cleanup(&ctx);
	return true;
failed:
	EVP_CIPHER_CTX_cleanup(&ctx);
	return false;
}

bool AESDecrypt(void *destData, int *destLen,
		const void *srcData, int srcLen,
		const void *keyValue, const void *IV)
{
	EVP_CIPHER_CTX ctx;
	const EVP_CIPHER *cipher = EVP_aes_128_cbc();

	EVP_CIPHER_CTX_init(&ctx);

	EVP_DecryptInit_ex(&ctx, cipher, NULL, keyValue, IV);
	EVP_CIPHER_CTX_set_padding(&ctx, 0);
	if (!EVP_DecryptUpdate(&ctx, destData, destLen, srcData, srcLen))
		goto failed;
	if (!EVP_DecryptFinal_ex(&ctx, destData, destLen))
		goto failed;

	EVP_CIPHER_CTX_cleanup(&ctx);
	return true;
failed:
	EVP_CIPHER_CTX_cleanup(&ctx);
	return false;
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

bool OpenSSL_AESTest(void)
{
	const uint8_t keyData[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const uint8_t IV[]  = {0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad};
	const int len = 256;
	int destLen = 0;
	uint8_t src[len];
	uint8_t enc[len];
	uint8_t dec[len];
	size_t i;
	bool res;

	/* prepare test data */
	for (i = 0; i < len; i++)
		src[i] = i;

	dumpData(src, len, "original");

	/* encryption */

	res = AESEncrypt(enc, &destLen, src, len, keyData, IV);

	if (!res) {
		error("fail to encrypt, res=0x%08x\n", res);
		return res;
	}

	dumpData(enc, len, "encrypted");

	/* decryption */

	res = AESDecrypt(dec, &destLen, enc, len, keyData, IV);

	if (!res) {
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
