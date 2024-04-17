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
#ifndef _TEE_INTERNAL_CRYPTO_CIPHER_API_H_
#define _TEE_INTERNAL_CRYPTO_CIPHER_API_H_

#include "tee_internal_crypto_common.h"

/** Initialize a symmetric cipher operation.
 *
 * @param operation	operation handle
 * @param IV		initialization vector
 * @param IVLen		length of the initialization vector
 */
void TEE_CipherInit(TEE_OperationHandle operation,void* IV, size_t IVLen);

/** Encrypt or decrypt the input data.
 *
 * @param operation	operation handle
 * @param srcData	data that is to be encrypted or decrypted
 * @param srcLen	length of the input data encrypted or decrypted
 * @param destData	output data into which the data needs to be stored
 * @param destLen	pointer to the length of output data
 * @retval TEE_SUCCESS			on success
 * @retval EE_ERROR_SHORT_BUFFER	if the output buffer is not large enough
 *					to contain the output
 */
TEE_Result TEE_CipherUpdate(TEE_OperationHandle operation,
				void* srcData, size_t srcLen,
				void* destData, size_t *destLen);

/** Finalize the cipher operation and encrypt or decrypt any remaining data.
 *
 * @param operation	operation handle
 * @param srcData	data that is to be encrypted or decrypted
 * @param srcLen	length of the input data encrypted or decrypted
 * @param destData	output data into which the data needs to be stored
 * @param destLen	pointer to the length of output data
 * @retval TEE_SUCCESS			on success
 * @retval EE_ERROR_SHORT_BUFFER	if the output buffer is not large enough
 *					to contain the output
 */
TEE_Result TEE_CipherDoFinal(TEE_OperationHandle operation,
				void* srcData, size_t srcLen,
				void* destData, size_t *destLen);

#endif /* _TEE_INTERNAL_CRYPTO_CIPHER_API_H_ */
