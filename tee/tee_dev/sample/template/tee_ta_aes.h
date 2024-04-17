#ifndef _TEE_TA_AES_H_
#define _TEE_TA_AES_H_

#include "tee_internal_api.h"

/** Generate a Key object.
 *
 * @param key		key buffer
 * @param keyBytes	length of the key, in bytes, can be 16, 24, 32
 * @param object	buffer to return the key object.
 */
TEE_Result AESGenKey(const void *key, size_t keyBits, TEE_ObjectHandle *object);

/* AES crypto
 *
 * @param destData	destDataination data buffer
 * @param srcData	source data buffer
 * @param len		data length, must pad before encryption/decryption
 * @param mode		TEE_MODE_ENCRYPT or TEE_MODE_DECRYPT
 * @param key		key buffer
 * @param IV		initial vector buffer
 * @param IVLen		length of the initial vector, in bytes
 */
TEE_Result AESCipher(void *destData, const void *srcData, size_t len,
		uint32_t mode, TEE_ObjectHandle key,
		const void *IV, size_t IVLen);

#endif /* _TEE_TA_AES_H_ */
