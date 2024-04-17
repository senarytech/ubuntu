/*
 * Copyright (c) 2017-2022 Synaptics International Ltd. and its affiliates.
 * All rights reserved.
 *
 * This software file (the "File") is owned and distributed by Synaptics
 * International Ltd. and/or its affiliates ("Synaptics") under the following
 * licensing terms.
 *
 * If you received this File from Synaptics and you have entered into a
 * commercial license agreement (a "Commercial License") with Synaptics, the
 * File is licensed to you under the terms of the applicable Commercial
 * License.
 */

#ifndef _TEE_CA_SAMPLE_H_
#define _TEE_CA_SAMPLE_H_

#include "tee_sample_cmd.h"

/** initialize TA
 *
 * @param taName	TA name.
 *
 * @retval 0		success
 * @retval <0		fail
 */
int Initialize(const char *taName);

/** finalize the TA.
 */
void Finalize(void);

/** example to invoke a command.
 *
 * @param cmdId		  command ID.
 * @param custblob_file	  CustBlob path.
 */
int InvokeCommand(int cmdId, const char *custblob_file);

#endif /* _TEE_CA_SAMPLE_H_ */
