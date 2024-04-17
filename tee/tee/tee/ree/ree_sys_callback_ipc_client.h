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
#ifndef _REE_SYS_CALLBACK_IPC_H_
#define _REE_SYS_CALLBACK_IPC_H_
#include "tee_comm.h"

#define TEE_SOCKS_ADDR "@tee_daemon_socks"

struct ipc_generic_param {
	/*
	 * the first is always the command id
	 */
	unsigned int cmd_id;
	unsigned int param_len;
	char param[0];
};

int REE_CreateIpcServer(const char*);

int REE_ClientProcessIpcRequest(int ipc_cmd, void *in_param, int in_len,
		void *out_param, int out_len,
		int (*client_reqeust_callback) (int, void*, int, void*, int));

#endif /* _REE_SYS_CALLBACK_IPC_H_ */
