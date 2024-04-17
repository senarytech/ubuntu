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
#ifndef _REE_CA_FILE_OP_H_
#define _REE_CA_FILE_OP_H_

#include "tee_client_const.h"
#include "tee_client_type.h"

TEEC_Result Do_FileSeek(void *param);
TEEC_Result Do_FolderMake(void *param, char *root_dir);
TEEC_Result Do_FolderOpen(void *param, char *root_dir);
TEEC_Result Do_FolderRead(void *param, char *root_dir);
TEEC_Result Do_FolderClose(void *param);
TEEC_Result Do_FileWrite(void *param, uint32_t *outlen);
TEEC_Result Do_FileRead(void *param);
TEEC_Result Do_FileClose(void *param);
TEEC_Result Do_FileOpen(void *param, char *root_dir);
TEEC_Result Do_FileDelete(void *param, char *root_dir);

#endif
