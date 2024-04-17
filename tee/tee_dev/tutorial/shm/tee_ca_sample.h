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

/** addition operation (a + b).
 *
 * @param a		addend
 * @param b		addend
 *
 * @return result, 0xdeadbeef for error.
 */
int Add(int a, int b);

/** subtraction operation (a - b).
 *
 * @param a		minuend
 * @param b		subtrahend
 *
 * @return result, 0xdeadbeef for error.
 */
int Sub(int a, int b);

/** allocate shared memory.
 *
 * @param size		size of the shared memory.
 * @param buffer	buffer to return the virtual address of shared memory.
 *
 * @retval NULL		fail to allocate the shared memory.
 * @retval !NULL	shared memory handle.
 */
void *AllocateSharedMemory(unsigned int size, char **buffer);

/** release shared memory.
 *
 * @param shm		shared memory handle.
 */
void ReleaseSharedMemory(void *shm);

/** retrieve shared memory info
 *
 * @param shm		shared memory handle.
 * @param size		buffer to return the size of the shared memory
 *
 * @retval NULL		shm is invalid.
 * @retval !NULL	virtual address of shared memory
 */
char *GetSharedMemoryInfo(void *shm, unsigned int *size);

/** reverse data
 *
 * @param outShm	shm handle to store result
 * @param inShm		shm handle to store source
 * @param len		length of the data
 *
 * @retval 0		success
 * @retval !0		error code
 */
int Reverse(void *outShm, void *inShm, int len);

#endif /* _TEE_CA_SAMPLE_H_ */
