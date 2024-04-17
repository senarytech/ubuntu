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

#ifndef _ASM_H_
#define _ASM_H_

#include <config.h>

/* Some toolchains use other characters (e.g. '`') to mark new line in macro */
#ifndef ASM_NL
#define ASM_NL		;
#endif

#define LENTRY(name) \
	name:

#define ENTRY(name) \
	.globl name ASM_NL \
	LENTRY(name)

#define END(name) \
	.size name, .-name

#define ENDPROC(name) \
	.type name %function ASM_NL \
	END(name)

#define ENDDATA(name) \
	.type name %object

#define FUNC(name) \
	.globl name ASM_NL \
	.type name %function ASM_NL \
	LENTRY(name)

#define LOCAL_FUNC(name) \
	.type name %function ASM_NL \
	LENTRY(name)

#define END_FUNC(name) \
	END(name)

#define DATA(name) \
	.globl name ASM_NL \
	.type name %object ASM_NL \
	LENTRY(name) \

#define LOCAL_DATA(name) \
	.tpe name %object ASM_NL \
	LENTRY(name)

#define END_DATA(name) \
	END(name)

#ifdef CONFIG_SMP
# define ALT_SMP(instr...)	instr
# define ALT_UP(instr...)
#else /* !CONFIG_SMP */
# define ALT_SMP(instr...)
# define ALT_UP(instr...)	instr
#endif /* CONFIG_SMP */

#endif /* _ASM_H_ */
