/*
 * Copyright (c) 2013-2017 Synaptics International Ltd. and its affiliates.
 * All rights reserved.
 *
 * SYNAPTICS CONFIDENTIAL
 *
 * The source code contained or described herein and all documents related to
 * the source code ("Material") are owned by Synaptics International Ltd or its
 * suppliers or licensors. Title to the Material remains with Synaptics
 * International Ltd or its suppliers and licensors. The Material contains
 * trade secrets and proprietary and confidential information of Synaptics or
 * its suppliers and licensors.
 * The Material is protected by worldwide copyright and trade secret laws and
 * treaty provisions. No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Synaptics's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery
 * of the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be
 * express and approved by Synaptics in writing.
 */
#ifndef _TEE_INTERNAL_SECUREKEY_H_
#define _TEE_INTERNAL_SECUREKEY_H_

#include "tee_internal_common.h"

#define APP_UNIQUE_KEY_SIZE		(16)
#define UUID_SIZE			(16)
#define APPID_SIZE			(16)

struct SecureKeyParam {
	uint8_t uuID[UUID_SIZE];
	uint8_t appID[APPID_SIZE];
	uint32_t appVariant;
	uint8_t secureKey[APP_UNIQUE_KEY_SIZE];
};

TEE_Result getSecureKey(struct SecureKeyParam* sk);
#endif /* _TEE_INTERNAL_SA_API_H_ */
