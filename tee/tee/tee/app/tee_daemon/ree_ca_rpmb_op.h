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
#ifndef _REE_CA_RPMB_OP_H_
#define _REE_CA_RPMB_OP_H_

#include <tee_client_const.h>
#include <tee_client_type.h>
#include <tee_internal_crypto_rpmb.h>

#define REE_RPMB_KEY_SIZE	32
#define REE_RPMB_DATA_SIZE	256
#define RPMB_FILE_PATH_LEN	63
#define MAC_LEN			32
#define NONCE_LEN		16
/* if RPMB_USE_COMMERCIAL_KEY defined as 1, it will program secure key
   if RPMB_USE_COMMERCIAL_KEY defined as 0, it will program default key*/
#define RPMB_USE_COMMERCIAL_KEY		1
#ifndef __linux__
#define htobe16(x)	((x&0x00ff) << 8 | (x&0xff00) >> 8)
#define htobe32(x)	((x&0x000000ff) << 24 | (x&0x0000ff00) << 8 \
			| (x&0x00ff0000) >> 8 | (x&0xff000000) >> 24)
#endif

TEEC_Result Do_RPMBWriteBlock(void *rpmbParam);
TEEC_Result Do_RPMBReadBlock(void *rpmbParam);
TEEC_Result Do_RPMBReadSize(void *rpmbParam);

#endif
