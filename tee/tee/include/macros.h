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

#ifndef __MACROS_H__
#define __MACROS_H__

#include "align.h"

#ifndef MAX
# define MAX(a, b)	(((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
# define MIN(a, b)	(((a) < (b)) ? (a) : (b))
#endif

#ifndef UNUSED
# define UNUSED(x)	((void)(x))
#endif

#ifndef N_ELEMENTS
# define N_ELEMENTS(x)	(sizeof(x) / sizeof((x)[0]))
#endif

#ifndef ARRAY_SIZE
# define ARRAY_SIZE	N_ELEMENTS
#endif

#ifndef ALWAYS_INLINE
# define ALWAYS_INLINE	inline __attribute__((always_inline))
#endif

#ifndef STRINGIFY
# define __STRINGIFY(x)	#x
# define STRINGIFY(x)	__STRINGIFY(x)
#endif

#if defined(__GNUC__) && (__GNUC__ > 2)
# define LIKELY(expr)	(__builtin_expect(!!(expr), 1))
# define UNLIKELY(expr)	(__builtin_expect(!!(expr), 0))
#else
# define LIKELY(expr)	(expr)
# define UNLIKELY(expr)	(expr)
#endif

#ifndef offsetof
# if defined(__GNUC__)  && __GNUC__ >= 4
#  define offsetof(TYPE, MEMBER)		__builtin_offsetof(TYPE, MEMBER)
# else
#  define offsetof(TYPE, MEMBER)		((size_t)&(((TYPE *)0)->MEMBER))
# endif
#endif

#ifndef container_of
# define container_of(PTR, TYPE, MEMBER)	((TYPE *)((char *)(PTR) - offsetof(TYPE, MEMBER)))
#endif

#ifndef WEAK_ALIAS
# define WEAK_ALIAS(name, aliasname) \
	extern typeof(name) aliasname __attribute__ ((weak, alias (#name)));
#endif

#ifndef STRONG_ALIAS
# define STRONG_ALIAS(name, aliasname) \
	extern typeof(name) aliasname __attribute__ ((alias (#name)));
#endif

#ifndef BUG
# define BUG(str) do { \
		printk("BUG: " str " failure at %s:%d/%s()!\n", \
			__FILE__, __LINE__, __func__); \
		cpu_halt(); \
	} while (0)
#endif

#ifndef bug
# define bug()	BUG("")
#endif

#ifndef BUG_ON
# define BUG_ON(cond)	\
	do { if (UNLIKELY(cond)) BUG(STRINGIFY(cond)); } while (0)
#endif

#ifndef bug_on
# define bug_on(cond)	BUG_ON(cond)
#endif

/*
 * Compile time assert, works by defining an array type that has -1
 * elements when expr is false
 */
#ifndef COMPILE_ASSERT
# define COMPILE_ASSERT(expr, msg)	typedef char msg[(expr) ? 1 : -1]
#endif

/* Round up the even multiple of size, size has to be a multiple of 2 */
#ifndef ROUNDUP
#define ROUNDUP(v, size) (((v) + ((size) - 1)) & ~((size) - 1))
#endif

/* Round down the even multiple of size, size has to be a multiple of 2 */
#ifndef ROUNDDOWN
#define ROUNDDOWN(v, size) ((v) & ~((size) - 1))
#endif

/* x has to be of an unsigned type */
#ifndef IS_POWER_OF_TWO
#define IS_POWER_OF_TWO(x) (((x) != 0) && (((x) & (~(x) + 1)) == (x)))
#endif

/* convert to string */
#ifndef TO_STR
#define TO_STR(x) STRINGIFY(x)
#endif

#endif
