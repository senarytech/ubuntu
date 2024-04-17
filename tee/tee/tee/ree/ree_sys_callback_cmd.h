/*
 * Copyright (c) 2013-2014 Synaptics International Ltd. and its affiliates.
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
#ifndef _REE_SYS_CALLBACK_CMD_H_
#define _REE_SYS_CALLBACK_CMD_H_

#include "types.h"
#include "tee_comm.h"
#include "tee_internal_api.h"

enum REE_SysCallbackCmd {
	REE_CMD_TIME_GET,
	REE_CMD_TIME_WAIT,
	REE_CMD_THREAD_CREATE,
	REE_CMD_THREAD_DESTROY,
	REE_CMD_LOG_WRITE,
	REE_CMD_DEPRECATED1,
	REE_CMD_DEPRECATED2,
	REE_CMD_DEPRECATED3,
	REE_CMD_DEPRECATED4,
	REE_CMD_DEPRECATED5,
	REE_CMD_DEPRECATED6,
	REE_CMD_DEPRECATED7,
	REE_CMD_DEPRECATED8,
	REE_CMD_DEPRECATED9,
	REE_CMD_DEPRECATED10,
	REE_CMD_DEPRECATED11,
	REE_CMD_FILE_OPEN,
	REE_CMD_FILE_CLOSE,
	REE_CMD_FILE_READ,
	REE_CMD_FILE_WRITE,
	REE_CMD_FILE_SEEK,
	REE_CMD_FOLDER_MAKE,
	REE_CMD_RPMB_READ_COUNTER,
	REE_CMD_RPMB_WRITE_BLOCK,
	REE_CMD_RPMB_READ_BLOCK,
	REE_CMD_MUTEX_EXT_WAIT,
	REE_CMD_MUTEX_EXT_WAKE,
	REE_CMD_MUTEX_EXT_DEL,
	REE_CMD_RPMB_READ_SIZE,
	REE_CMD_RPMB_WRITE_KEY,
	REE_CMD_FILE_DELETE,
	REE_CMD_FOLDER_OPEN,
	REE_CMD_FOLDER_READ,
	REE_CMD_FOLDER_CLOSE,
	REE_CMD_MAX
};

#ifndef REE_MUTEX_NAME_MAX_LEN
#define REE_MUTEX_NAME_MAX_LEN		32
#endif

#ifndef REE_SEMAPHORE_NAME_MAX_LEN
#define REE_SEMAPHORE_NAME_MAX_LEN	32
#endif

typedef struct REE_MutexCreateParam
{
	void		*lock;
	char		name[REE_MUTEX_NAME_MAX_LEN + 1];
} REE_MutexCreateParam;

typedef struct REE_SemaphoreCreateParam
{
	void		*sem;
	int		value;
	char		name[REE_SEMAPHORE_NAME_MAX_LEN + 1];
} REE_SemaphoreCreateParam;

typedef struct REE_SemaphoreTimedWaitParam
{
	void		*sem;
	uint32_t	timeout;
} REE_SemaphoreTimedWaitParam;

typedef struct REE_FileReadWriteParam
{
	uint64_t	filp;
	int32_t		offset;
	int32_t		ret;
	int32_t		size;
	char		buff[0];
} REE_FileReadWriteParam;

typedef struct REE_FileSeekParam
{
	uint64_t	filp;
	int32_t		offset;
	uint32_t	whence;
	uint32_t	pos;
} REE_FileSeekParam;

#endif /* _REE_SYS_CALLBACK_CMD_H_ */
