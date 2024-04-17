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

#ifndef _TEE_CA_UTIL_H_
#define _TEE_CA_UTIL_H_

/** Initialize Util.
 *
 * @note must call TEEU_Finalize() to free resource once no more operations on
 *       Util.
 *
 * @param taFile	file path of Util TA.
 * @retval 0		Success.
 * @retval !0		Fail, error code.
 */
int TEEU_Initialize(const char *taFile);

/** Finalize Util after use it.
 */
void TEEU_Finalize(void);

/** Do memory copy in TEE.
 *
 * @note: the cost is much higher than local memcpy, so, please only use it for
 *        secure memory copy.
 *
 * @dstPhyAddr		Destination physical address.
 * @srcPhyAddr		Source physical address.
 * @len			Bytes to copy.
 *
 * @retval 0		Success to copy
 * @retval !0		Fail, error code.
 */
int TEEU_MemCopy(void *dstPhyAddr, void *srcPhyAddr, unsigned long len);

#endif /* _TEE_CA_UTIL_H_ */
