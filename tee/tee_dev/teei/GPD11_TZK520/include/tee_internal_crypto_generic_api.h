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
#ifndef _TEE_INTERNAL_CRYPTO_GENERIC_API_H_
#define _TEE_INTERNAL_CRYPTO_GENERIC_API_H_

#include "tee_internal_crypto_common.h"

/** Allocate a handle for a new cryptographic operation.
 *
 * @param operation	reference to generated operation handle
 * @param algorithm	one of the cipher algorithms enumerated in
 *			TEE_CRYPTO_ALGORITHM_ID
 * @param mode		mode for the current operation, as specified by the
 * 			enumeration TEE_OperationMode
 * @param maxKeySize	maximum key size that is in use by the algorithm
 *
 * @retval TEE_SUCCESS			on success
 * @retval EE_ERROR_SHORT_BUFFER	if the output buffer is not large enough
 *					to allocate the operation
 * @retval TEE_ERROR_NOT_SUPPORTED	if the mode is not compatible with the
 * 					algorithm or key size or if the algorithm
 *					is not one of the listed algorithms
 */
TEE_Result TEE_AllocateOperation(TEE_OperationHandle *operation,
				uint32_t algorithm, uint32_t mode,
				uint32_t maxKeySize);

/** Deallocate all resources associated with an operation handle.
 *
 * @param operation	pointer to operation handle
 */
void TEE_FreeOperation(TEE_OperationHandle operation);

/** Fill in the operationInfo structure associated with an operation.
 *
 * @param operation	operation handle
 *
 * @param operationInfo	pointer to a structure filled with the operation information
 */
void TEE_GetOperationInfo(TEE_OperationHandle operation,
				TEE_OperationInfo* operationInfo);

/** Associate an operation with a key.
 *
 * @param operation	operation handle
 * @param key		handle on a key object
 *
 * @return	The only possible return value is TEE_SUCCESS
 */
TEE_Result TEE_SetOperationKey(TEE_OperationHandle operation,
				TEE_ObjectHandle key);

#endif /* _TEE_INTERNAL_CRYPTO_GENERIC_API_H_ */
