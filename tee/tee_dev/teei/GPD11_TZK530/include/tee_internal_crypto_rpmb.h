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
#ifndef _REE_SYS_CALLBACK_RPMB_H_
#define _REE_SYS_CALLBACK_RPMB_H_

#include "tee_comm.h"

#define REE_RPMB_KEY_SIZE	32
#define REE_RPMB_DATA_SIZE	256
#define RPMB_FILE_PATH_LEN	63
#define MAC_LEN			32
#define NONCE_LEN		16
/* if RPMB_USE_COMMERCIAL_KEY defined as 1, it will program secure key
   if RPMB_USE_COMMERCIAL_KEY defined as 0, it will program default key*/
#define RPMB_USE_COMMERCIAL_KEY		1
#define htobe16(x)	((x&0x00ff) << 8 | (x&0xff00) >> 8)
#define htobe32(x)	((x&0x000000ff) << 24 | (x&0x0000ff00) << 8 \
			| (x&0x00ff0000) >> 8 | (x&0xff000000) >> 24)

typedef struct {
	uint8_t  stuff[196];
	uint8_t  keyMAC[32];
	uint8_t  data[256];
	uint8_t  nonce[16];
	uint32_t writeCounter;
	uint16_t addr;
	uint16_t blockCount;
	uint16_t result;
	uint16_t reqResp;
} rpmbFrame;

typedef struct {
    uint8_t key[32];
} REE_RPMBKeyParam;

typedef struct {
    char rpmbDevice[RPMB_FILE_PATH_LEN + 1];
    unsigned int cnt;
} REE_RPMBReadCounterParam;

typedef struct {
    char rpmbDevice[RPMB_FILE_PATH_LEN + 1];
    rpmbFrame frameInOut[1];
} REE_RPMBReadBlockParam;

typedef struct {
    char rpmbDevice[RPMB_FILE_PATH_LEN + 1];
    rpmbFrame frameIn[1];
} REE_RPMBWriteBlockParam;

typedef struct {
    char rpmbDevice[RPMB_FILE_PATH_LEN + 1];
    uint8_t cid[32];
} REE_EMMCGetCidParam;

enum rpmb_op_type {
	MMC_RPMB_WRITE_KEY = 0x01,
	MMC_RPMB_READ_CNT  = 0x02,
	MMC_RPMB_WRITE     = 0x03,
	MMC_RPMB_READ      = 0x04,

	/* For internal usage only, do not use it directly */
	MMC_RPMB_READ_RESP = 0x05
};

#endif
