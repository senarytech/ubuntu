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

#ifndef _TEE_TA_PERM_RULE_DEF_H_
#define _TEE_TA_PERM_RULE_DEF_H_

#define TEE_PC_MAX_NAME_LENGTH    (16)
#define TEE_PC_MAX_DATA_WORDS     (2)

/* Definations for name item */
#define TEE_PC_ITEM_NAME_MAGIC     (0x656d616e) //ASCII("name")

typedef struct _name_t {
	uint32_t magic;
	char     name[TEE_PC_MAX_NAME_LENGTH];
	uint32_t length;
} name_t;

/* Definations for data item */
#define TEE_PC_ITEM_DATA_MAGIC     (0x61746164) //ASCII("data")

typedef struct _data_t {
	uint32_t magic;
	uint32_t buf[TEE_PC_MAX_DATA_WORDS];
	uint32_t wsize;
} data_t;

/* Definations for access permissions */
#define TEE_TA_PERM_EXEC            (0x1)
#define TEE_TA_PERM_WRITE           (0x2)
#define TEE_TA_PERM_READ            (0x4)

typedef uint32_t perm_t;

/*
 * Definations for object item.
 * embeded in boot parameter
 */
typedef enum {
	TEE_OBJ_REG_RANGE = 0x72676572,       //ASCII(“regr")
	TEE_OBJ_BCM_PI_CATEGORY = 0x636d6362, //ASCII(“bcmc")
} object_type_t;

typedef struct _tz_object_t {
	name_t        name;
	object_type_t type;
	uint32_t      rsv;
	data_t        data;
} tz_object_t;

/*
 * Definations for permission rules.
 * embeded in boot parameter
 */
typedef struct _tz_perm_rule_t {
	name_t obj_name;
	name_t group_name;
	perm_t perm;
} tz_perm_rule_t;

#define _PAGE_ALIGNED_VALUE(x) ((x)|sizeof(int[-!!((x)&0xFFF)]))
#define _NAME_LENGTH(n) ((sizeof(#n)-1)|sizeof(int[-!!(sizeof(#n)>TEE_PC_MAX_NAME_LENGTH)]))

#define _NEW_NAME(n)                  \
{                                     \
	.magic = TEE_PC_ITEM_NAME_MAGIC, \
	.name = #n,                      \
	.length = _NAME_LENGTH(n),       \
}

#define START_OBJ_DEFINE                        \
tz_object_t g_tz_object_list[] = {

/* start and size of register should be aligned by 4k */
#define OBJ_REG(n, s, z)                        \
	{                                          \
		.name = _NEW_NAME(n),                 \
		.type = TEE_OBJ_REG_RANGE,            \
		.data = {                             \
			.magic = TEE_PC_ITEM_DATA_MAGIC, \
			.buf = {                         \
				_PAGE_ALIGNED_VALUE(s),     \
				_PAGE_ALIGNED_VALUE(z),     \
			},                               \
			.wsize = 2,                      \
		},                                    \
	},

#define OBJ_BCM_PI(n, p, s)                     \
	{                                          \
		.name = _NEW_NAME(n),                 \
		.type = TEE_OBJ_BCM_PI_CATEGORY,      \
		.data = {                             \
			.magic = TEE_PC_ITEM_DATA_MAGIC, \
			.buf = {(p), (s)},               \
			.wsize = 2,                      \
		},                                    \
	},

#define FINISH_OBJ_DEFINE                       \
};                                              \
const uint32_t g_tz_object_num = ARRAY_SIZE(g_tz_object_list);

#define START_RULE_DEFINE                       \
tz_perm_rule_t g_tz_rule_list[] = {

#define RULE(o, g, p)                           \
	{                                          \
		.obj_name = _NEW_NAME(o),             \
		.group_name = _NEW_NAME(g),           \
		.perm = p,                            \
	},

#define FINISH_RULE_DEFINE                      \
};                                              \
const uint32_t g_tz_rule_num = ARRAY_SIZE(g_tz_rule_list);

#endif //_TEE_TA_PERM_RULE_DEF_H_
