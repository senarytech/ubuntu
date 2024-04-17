/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <fcntl.h>
#include <linux/types.h>
#include <linux/mmc/ioctl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include <tee_client_api.h>
#include <tee_internal_crypto_rpmb.h>
#include "rpmb_ioctl.h"
#include "rpmb.h"

/*
 * Request and response definitions must be in sync with the secure side
 */

static pthread_mutex_t rpmb_mutex = PTHREAD_MUTEX_INITIALIZER;

#define IOCTL(fd, request, ...)					   \
	({							   \
		int ret;					   \
		ret = ioctl((fd), (request), ##__VA_ARGS__);	   \
		if (ret < 0)					   \
			printf("ioctl ret=%d errno=%d", ret, errno); \
		ret;						   \
	})

static void tee_mutex_lock(pthread_mutex_t *mu)
{
	int e = pthread_mutex_lock(mu);

	if (e) {
		printf("pthread_mutex_lock: %s", strerror(e));
		printf("terminating...");
		exit(EXIT_FAILURE);
	}
}

static void tee_mutex_unlock(pthread_mutex_t *mu)
{
	int e = pthread_mutex_unlock(mu);

	if (e) {
		printf("pthread_mutex_unlock: %s", strerror(e));
		printf("terminating...");
		exit(EXIT_FAILURE);
	}
}

/*
 * Extended CSD Register is 512 bytes and defines device properties
 * and selected modes.
 */
static uint32_t read_ext_csd(int fd, uint8_t *ext_csd)
{
	int st = 0;
	struct mmc_ioc_cmd cmd = {
		.blksz = 512,
		.blocks = 1,
		.flags = MMC_RSP_R1 | MMC_CMD_ADTC,
		.opcode = MMC_SEND_EXT_CSD,
	};

	mmc_ioc_cmd_set_data(cmd, ext_csd);

	st = IOCTL(fd, MMC_IOC_CMD, &cmd);
	if (st < 0)
		return TEEC_ERROR_GENERIC;

	return TEEC_SUCCESS;
}

static uint32_t rpmb_data_req(int fd, struct rpmb_data_frame *req_frm,
			      size_t req_nfrm, struct rpmb_data_frame *rsp_frm,
			      size_t rsp_nfrm)
{
	int st = 0;
	size_t i = 0;
	uint16_t msg_type = ntohs(req_frm->msg_type);
	struct {
		struct mmc_ioc_multi_cmd multi;
		struct mmc_ioc_cmd cmd_buf[3];
	} mmc;
	struct mmc_ioc_cmd *cmd = mmc.multi.cmds;

	memset(&mmc, 0, sizeof(mmc));

	for (i = 1; i < req_nfrm; i++) {
		if (req_frm[i].msg_type != msg_type) {
			printf("All request frames shall be of the same type");
			return TEEC_ERROR_BAD_PARAMETERS;
		}
	}

	switch (msg_type) {
	case RPMB_MSG_TYPE_REQ_AUTH_KEY_PROGRAM:
	case RPMB_MSG_TYPE_REQ_AUTH_DATA_WRITE:
		if (rsp_nfrm != 1) {
			printf("Expected only one response frame");
			return TEEC_ERROR_BAD_PARAMETERS;
		}

		mmc.multi.num_of_cmds = 3;

		/* Request cmd */
		cmd->arg		= 0;
		cmd->blksz 		= 512;
		cmd->blocks 		= 1;
		cmd->write_flag 	= 1 | (1<<31);
		cmd->opcode 		= MMC_WRITE_MULTIPLE_BLOCK;
		cmd->flags 		= MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
		cmd->data_ptr 		= (uintptr_t)req_frm;
		cmd++;

		/* request result */
		memset(rsp_frm, 0, sizeof(*rsp_frm));
		rsp_frm->msg_type= htobe16(RPMB_MSG_TYPE_REQ_RESULT_READ);

		cmd->arg		= 0;
		cmd->blksz 		= 512;
		cmd->blocks 		= 1;
		cmd->write_flag 	= 1;
		cmd->opcode 		= MMC_WRITE_MULTIPLE_BLOCK;
		cmd->flags 		= MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
		cmd->data_ptr 		= (uintptr_t)rsp_frm;
		cmd++;

		/* get response */
		cmd->arg		= 0;
		cmd->blksz 		= 512;
		cmd->blocks 		= 1;
		cmd->write_flag 	= 0;
		cmd->opcode 		= MMC_READ_MULTIPLE_BLOCK;
		cmd->flags 		= MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
		cmd->data_ptr 		= (uintptr_t)rsp_frm;

		st = ioctl(fd, MMC_IOC_MULTI_CMD, &mmc.multi);
		if (st < 0)
			return TEEC_ERROR_GENERIC;

		break;

	case RPMB_MSG_TYPE_REQ_WRITE_COUNTER_VAL_READ:
		if (rsp_nfrm != 1) {
			printf("Expected only one response frame");
			return TEEC_ERROR_BAD_PARAMETERS;
		}
#if __GNUC__ > 6
		__attribute__((fallthrough));
#endif

	case RPMB_MSG_TYPE_REQ_AUTH_DATA_READ:
		if (req_nfrm != 1) {
			printf("Expected only one request frame");
			return TEEC_ERROR_BAD_PARAMETERS;
		}
		mmc.multi.num_of_cmds = 2;

		/* Request cmd */
		cmd->arg		= 0;
		cmd->blksz 		= 512;
		cmd->blocks 		= 1;
		cmd->write_flag 	= 1;
		cmd->opcode 		= MMC_WRITE_MULTIPLE_BLOCK;
		cmd->flags 		= MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
		cmd->data_ptr 		= (uintptr_t)req_frm;
		cmd++;

		/* Read cmd */
		cmd->arg		= 0;
		cmd->blksz 		= 512;
		cmd->blocks 		= rsp_nfrm;
		cmd->write_flag 	= 0;
		cmd->opcode 		= MMC_READ_MULTIPLE_BLOCK;
		cmd->flags 		= MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
		cmd->data_ptr 		= (uintptr_t)rsp_frm;

		st = ioctl(fd, MMC_IOC_MULTI_CMD, &mmc.multi);
		if (st < 0)
			return TEEC_ERROR_GENERIC;

		break;

	default:
		printf("Unsupported message type: %d", msg_type);
		return TEEC_ERROR_GENERIC;
	}

	return TEEC_SUCCESS;
}

static uint32_t rpmb_get_rpmb_size_mult(int fd, uint8_t * rpmb_size_mult)
{
	uint32_t res = 0;
	uint8_t ext_csd[512] = { 0 };

	res = read_ext_csd(fd, ext_csd);
	if (res != TEEC_SUCCESS)
		goto err;

	*rpmb_size_mult = ext_csd[168];

err:
	return res;
}

/*
 * req is one struct rpmb_req followed by one or more struct rpmb_data_frame
 * rsp is either one struct rpmb_dev_info or one or more struct rpmb_data_frame
 */
static uint32_t rpmb_process_request_unlocked(int fd, int cmd, void *req, void *rsp,
			size_t rsp_size)
{
	size_t req_nfrm = 1;
	size_t rsp_nfrm = 0;
	uint32_t res = 0;

	switch (cmd) {
	case CMD_RPMB_WRITE_KEY:
	case CMD_RPMB_WRITE:
	case CMD_RPMB_READ:
		if (fd < 0)
			return TEEC_ERROR_BAD_PARAMETERS;
		rsp_nfrm = rsp_size / 512;
		res = rpmb_data_req(fd, req, req_nfrm, rsp,
				    rsp_nfrm);
		break;

	case CMD_RPMB_READ_SIZE:
		res = rpmb_get_rpmb_size_mult(fd, (uint8_t *)rsp);
		break;

	default:
		printf("Unsupported RPMB command: %d", cmd);
		res = TEEC_ERROR_BAD_PARAMETERS;
		break;
	}

	return res;
}

uint32_t rpmb_process_request(int fd, int cmd, void *req, void *rsp,
			      size_t rsp_size)
{
	uint32_t res = 0;

	tee_mutex_lock(&rpmb_mutex);
	res = rpmb_process_request_unlocked(fd, cmd, req, rsp, rsp_size);
	tee_mutex_unlock(&rpmb_mutex);

	return res;
}
