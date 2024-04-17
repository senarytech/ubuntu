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
#include "mem_region.h"
#include "macros.h"

/* A dummy sets of security settings */

/* memory region setting */
const struct mem_region mem_region_param[0];
const uint32_t mem_region_num = ARRAY_SIZE(mem_region_param);

/* memory zone setting */
const struct mem_zone mem_zone_param[0];
const uint32_t mem_zone_num = ARRAY_SIZE(mem_zone_param);

/* register firewall setting */
const struct rf_window rf_window_param[0];
const uint32_t rf_window_num = ARRAY_SIZE(rf_window_param);

/* master setting */
const struct master_setting master_setting_param[0];
const uint32_t master_setting_num = ARRAY_SIZE(master_setting_param);

/* register region settings */
const struct reg_region reg_region_param[0];
const uint32_t reg_region_param_num = ARRAY_SIZE(reg_region_param);
