/*
 * Copyright (c) 2013-2014 Marvell International Ltd. and its affiliates.
 * All rights reserved.
 *
 * This software file (the "File") is owned and distributed by Marvell
 * International Ltd. and/or its affiliates ("Marvell") under the following
 * licensing terms.
 *
 * If you received this File from Marvell and you have entered into a
 * commercial license agreement (a "Commercial License") with Marvell, the
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
 * @param cmdId		command ID.
 */
int InvokeCommand(int cmdId);

#endif /* _TEE_CA_SAMPLE_H_ */
