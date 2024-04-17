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
#ifndef _TEE_INTERNAL_CRYPTO_RANDOM_API_H_
#define _TEE_INTERNAL_CRYPTO_RANDOM_API_H_

#include "tee_internal_crypto_common.h"

void TEE_GenerateRandom(void* randomBuffer, size_t randomBufferLen);

#endif /* _TEE_INTERNAL_CRYPTO_API_H_ */
