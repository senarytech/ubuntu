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

#ifndef RPMB_H
#define RPMB_H

#include <stdint.h>
#include <stddef.h>

#define RPMB_RESULT_OK				0x00
#define RPMB_RESULT_GENERAL_FAILURE		0x01
#define RPMB_RESULT_AUTH_FAILURE		0x02
#define RPMB_RESULT_ADDRESS_FAILURE		0x04
#define RPMB_RESULT_AUTH_KEY_NOT_PROGRAMMED	0x07

#define RPMB_MSG_TYPE_REQ_AUTH_KEY_PROGRAM		0x0001
#define RPMB_MSG_TYPE_REQ_WRITE_COUNTER_VAL_READ	0x0002
#define RPMB_MSG_TYPE_REQ_AUTH_DATA_WRITE		0x0003
#define RPMB_MSG_TYPE_REQ_AUTH_DATA_READ		0x0004
#define RPMB_MSG_TYPE_REQ_RESULT_READ			0x0005
#define RPMB_MSG_TYPE_RESP_AUTH_KEY_PROGRAM		0x0100
#define RPMB_MSG_TYPE_RESP_WRITE_COUNTER_VAL_READ	0x0200
#define RPMB_MSG_TYPE_RESP_AUTH_DATA_WRITE		0x0300
#define RPMB_MSG_TYPE_RESP_AUTH_DATA_READ		0x0400

uint32_t rpmb_process_request(int fd, int cmd, void *req, void *rsp,
			      size_t rsp_size);

#endif /* RPMB_H */
