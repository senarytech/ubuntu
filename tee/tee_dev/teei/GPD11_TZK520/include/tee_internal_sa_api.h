/*
 * Copyright (c) 2013-2014 Marvell International Ltd. and its affiliates.
 * All rights reserved.
 *
 * MARVELL CONFIDENTIAL
 *
 * The source code contained or described herein and all documents related to
 * the source code ("Material") are owned by Marvell International Ltd or its
 * suppliers or licensors. Title to the Material remains with Marvell
 * International Ltd or its suppliers and licensors. The Material contains
 * trade secrets and proprietary and confidential information of Marvell or
 * its suppliers and licensors.
 * The Material is protected by worldwide copyright and trade secret laws and
 * treaty provisions. No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Marvell's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery
 * of the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be
 * express and approved by Marvell in writing.
 */
#ifndef _TEE_INTERNAL_SA_API_H_
#define _TEE_INTERNAL_SA_API_H_

#include "tee_internal_common.h"

#define MAX_ENTRY_NUM		2

struct SecurityAttribute {
	uint32_t	index;
	int32_t		sessionId;
	TEE_UUID	uuid;
	int8_t		receivedUsageRules;
	int8_t		hdcpVersion;
	uint16_t	hdcpCappingResolutionWidth;
	uint16_t	hdcpCappingResolutionHeight;
	uint16_t	deviceCappingResolutionWidth;
	uint16_t	deviceCappingResolutionHeight;
	int8_t		resolutionCappingRotationAllowed:1,
			resolutionCappingAspectAllowed:1,
			deviceCappingRotationAllowed:1,
			deviceCappingAspectAllowed:1,
			reserved:4;
};

TEE_Result getSecurityAttributeEntry(struct SecurityAttribute* sessionEntry,
				uint32_t keySessionId);

TEE_Result setSecurityAttritbuteEntry(struct SecurityAttribute* sessionEntry);

uint32_t getActiveEntries(void);

TEE_Result getActiveSecurityAttributeEntry(struct SecurityAttribute*
				sessionEntry);

#endif /* _TEE_INTERNAL_SA_API_H_ */
