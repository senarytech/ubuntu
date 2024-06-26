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
#ifndef _TEE_INTERNAL_CRYPTO_MAC_API_H_
#define _TEE_INTERNAL_CRYPTO_MAC_API_H_

#include "tee_internal_crypto_common.h"

/** Initialize a MAC(Message Authentication code) operation.
 *
 * @param operation	operation handle
 * @param IV		initialization vector
 * @param IVLen		length of the initialization vector
 */
void TEE_MACInit(TEE_OperationHandle operation,void* IV, size_t IVLen);

/** Accumulates data for a MAC calculation.
 *
 * @param operation	operation handle
 * @param chunk		input message on which MAC calculation needs to be done
 * @param chunkSize	length of the input message
 */
void TEE_MACUpdate(TEE_OperationHandle operation,void* chunk, size_t chunkSize);

/** Finalize the MAC operation with a last chunk of message.
 *
 * @param operation	operation handle
 * @param message	input buffer containing a last message chunk to MAC
 * @param messageLen	length of the input buffer
 * @param mac		output buffer filled with the computed MAC
 * @param macLen	pointer to length of filled with the computed MAC
 * @retval TEE_SUCCESS			on success
 * @retval EE_ERROR_SHORT_BUFFER	if the output buffer is not large enough
 *					to contain the output
 */
TEE_Result TEE_MACComputeFinal(TEE_OperationHandle operation,
		void* message, size_t messageLen,void* mac, size_t *macLen);

/** Finalizes the MAC operation and compare the MAC with the input buffer.
 *
 * @param operation	operation handle
 * @param message	input buffer containing a last message chunk to MAC
 * @param messageLen	length of the input buffer
 * @param mac		input buffer containing the MAC to check
 * @param macLen	pointer to length of the input buffer
 * @retval TEE_SUCCESS			on success
 * @retval TEE_ERROR_MAC_INVALID	if the computed MAC does not correspond
 *					to the value passed in the parameter mac
 */
TEE_Result TEE_MACCompareFinal(TEE_OperationHandle operation,
		void* message, size_t messageLen,void* mac, size_t *macLen);

#endif /* _TEE_INTERNAL_CRYPTO_MAC_API_H_ */
