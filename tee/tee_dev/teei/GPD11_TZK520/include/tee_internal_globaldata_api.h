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

#ifndef _TEE_INTERNAL_GLOBALDATA_API_H_
#define _TEE_INTERNAL_GLOBALDATA_API_H_

/** Set the TA's global data.
 *
 * @param key		the key of the global data.
 * @param value		the value of the global data.
 *
 * @retval TEE_SUCCESS			set success.
 * @retval TEE_ERROR_OUT_OF_MEMORY	out of memory.
 */
TEE_Result set_globaldata(uint32_t key, uint32_t value);

/** Get the TA's global data.
 *
 * @param key		the key of the global data.
 * @param value		address of the value.
 *
 * @retval TEE_SUCCESS			get success.
 * @retval TEE_ERROR_BAD_PARAMETERS	the address of value is error.
 * @retval TEE_ERROR_ITEM_NOT_FOUND	the key is not found.
 */
TEE_Result get_globaldata(uint32_t key, uint32_t *value);

#endif /* _TEE_INTERNAL_GLOBALDATA_API_H_ */
