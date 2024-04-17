/*
 * Copyright (c) 2013-2016 Marvell International Ltd. and its affiliates.
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

#ifndef _TA_INFO_H_
#define _TA_INFO_H_

#include "types.h"

#define MAX_VENDOR_NUMS			6

typedef enum {
	TAMgrCmd_FLAG_REGISTER = 0,
	TAMgrCmd_FLAG_GETINFO = 1,
	TAMgrCmd_FLAG_MAX
} TAMgrCmd_FLAG;

typedef struct ta_cert_info {
	uint8_t cert_vendor_id;
	uint8_t cert_vendor_name[15];
	uint32_t cert_segid;
	uint32_t cert_segid_mask;
	uint32_t cert_version;
	uint32_t cert_version_mask;
	uint8_t vendor_uuid[16];
} ta_cert_info_t;

typedef struct ta_img_info {
	uint8_t image_vendor_id;
	uint8_t image_vendor_name[15];
	uint32_t image_segid;
	uint32_t image_segid_mask;
	uint32_t image_version;
	uint32_t image_version_mask;
	uint8_t vendor_uuid[16];
} ta_img_info_t;

typedef struct ta_root_cert_info {
	uint8_t vendor_name[15]; 		// like "synaptics"
	uint8_t vendor_uuid[16];		// only highest 14 bytes are used
	uint32_t cert_version; 		// only lowest 4 bits are effective due to
						// size limitation of OTP field
	uint32_t cert_version_mask;
} ta_root_cert_info_t;

typedef struct ta_export_info {
	ta_root_cert_info_t ta_root_cert_info;
	ta_cert_info_t ta_cert_info;
	ta_img_info_t ta_img_info;
	int ta_getinfo_error;
} ta_export_info_t;


struct ta_vendor_info {
	uint8_t name[15];
	uint32_t reserved;
};

struct ta_info {
	uint32_t version;
	uint32_t size;
	struct ta_vendor_info vendor_info[MAX_VENDOR_NUMS];
};

#endif /* _TA_INFO_H_ */
