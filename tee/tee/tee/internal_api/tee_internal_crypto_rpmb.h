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
#ifndef __linux__
#define htobe16(x)	((x&0x00ff) << 8 | (x&0xff00) >> 8)
#define htobe32(x)	((x&0x000000ff) << 24 | (x&0x0000ff00) << 8 \
			| (x&0x00ff0000) >> 8 | (x&0xff000000) >> 24)
#endif

/*
 * the file looks like "/data/tee/uuid2str(32bytes)/obj_id(64bytes)"
 * so the 192 is enough even if extend in future, and it save the
 * channel buffer space
 */
#ifndef REE_FILE_PATH_MAX_LEN
#define REE_FILE_PATH_MAX_LEN		(192 - 1)
#endif

#ifndef REE_FOLDER_PATH_MAX_LEN
#define REE_FOLDER_PATH_MAX_LEN		(192 - 1)
#endif

enum rpmb_op_command {
	CMD_RPMB_WRITE_KEY = 0x01,
	CMD_RPMB_WRITE     = 0x02,
	/* both read data and read write cnt */
	CMD_RPMB_READ      = 0x03,
	CMD_RPMB_READ_SIZE = 0x04,
};

/*
 * This structure is shared with OP-TEE and the MMC ioctl layer.
 * It is the "data frame for RPMB access" defined by JEDEC, minus the
 * start and stop bits.
 */
struct rpmb_data_frame {
	uint8_t stuff_bytes[196];
	uint8_t key_mac[32];
	uint8_t data[256];
	uint8_t nonce[16];
	uint32_t write_counter;
	uint16_t address;
	uint16_t block_count;
	uint16_t op_result;
	uint16_t msg_type;
};

typedef struct {
    uint8_t key[32];
} REE_RPMBKeyParam;

typedef struct {
    char rpmbDevice[RPMB_FILE_PATH_LEN + 1];
    unsigned int cnt;
} REE_RPMBReadCounterParam;

typedef struct {
    char rpmbDevice[RPMB_FILE_PATH_LEN + 1];
    struct rpmb_data_frame frameInOut[1];
} REE_RPMBReadBlockParam;

typedef struct {
    char rpmbDevice[RPMB_FILE_PATH_LEN + 1];
    struct rpmb_data_frame frameInOut[1];
} REE_RPMBWriteBlockParam;

typedef struct {
    char rpmbDevice[RPMB_FILE_PATH_LEN + 1];
    uint8_t cid[32];
} REE_EMMCGetCidParam;

typedef struct {
	char folderPath[REE_FILE_PATH_MAX_LEN + 1];
	uint32_t flags;
	uintptr_t dir;
} IPC_FolderMakeParam;

typedef struct
{
	bool		isdir;
	char		name[REE_FOLDER_PATH_MAX_LEN + 1];
	uintptr_t dir;
} IPC_FolderReadParam;

typedef struct {
	char fileName[REE_FILE_PATH_MAX_LEN + 1];
	uint32_t flags;
	int32_t fd;
	uint32_t size;
} IPC_fileOpenParam;

#endif
