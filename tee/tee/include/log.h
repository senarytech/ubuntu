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

#ifndef _LOG_H_
#define _LOG_H_

#if defined(__TRUSTZONE__)
	#include "macros.h"
	#include "printf.h"
	#include "printk.h"
	int get_cpu_id(void);
	#include "cpu_context.h"
	#define HALT()		cpu_halt()
	#define tee_printf	printf
#elif defined(CONFIG_AMP)
	#include "isl/amp_logger.h"
	#define HALT()		while(1)
	#define tee_printf	AMPPRINTF
	#undef CONFIG_LOG_TIME
	#undef CONFIG_LOG_CPUID
#elif defined(__KERNEL__)
	#include <linux/kernel.h>
	#define HALT()		while(1)
	#define tee_printf	printk
	#undef CONFIG_LOG_TIME
	#undef CONFIG_LOG_CPUID
#elif defined(__TRUSTEDAPP__)
	#include "tee_internal_api.h"
	#undef CONFIG_LOG_CPUID
	#define HALT()		while(1)
	#define tee_printf	printf
#else
	#define HALT()		while(1)
	#define tee_printf	printf
#endif

#ifdef CONFIG_LOG_TIME
	#ifdef __TRUSTEDAPP__
		#define log(info, fmt, args...)	do { \
			TEE_Time __t__; \
			TEE_GetSystemTime(&__t__); \
			tee_printf("[%d.%03d000] TA %s " info "[%s:%d] " fmt, __t__.seconds, \
					__t__.millis, __ThisTA.name, __func__, \
					__LINE__, ##args); \
			} while (0)
	#else /* !__TRUSTEDAPP__ */
		#include "clock_source.h"
		#define log(info, fmt, args...)	do { \
			uint32_t __s__, __us__; \
			clocksource_read_timestamp(&__s__, &__us__); \
					tee_printf("[%d.%06d] TZ CPU%d " info "[%s:%d] " fmt, __s__, __us__, \
					get_cpu_id(), __func__, __LINE__, ##args); \
			} while (0)
	#endif /* __TRUSTEDAPP__ */
#elif defined(CONFIG_LOG_CPUID)
	#define log(info, fmt, args...)	do { \
			tee_printf("TZ CPU%d " info "[%s:%d] " fmt, get_cpu_id(), __func__, __LINE__, ##args); \
		} while (0)
#else
	#ifdef __TRUSTEDAPP__
		#define log(info, fmt, args...)	do { \
				tee_printf("TA %s " info "[%s:%d] " fmt, \
						 __ThisTA.name, __func__, __LINE__, ##args); \
			} while (0)
	#else /* !__TRUSTEDAPP__ */
		#define log(info, fmt, args...)	do { \
				tee_printf("TZ " info "[%s:%d] " fmt, __func__, __LINE__, ##args); \
			} while (0)
	#endif /* __TRUSTEDAPP__ */
#endif /* CONFIG_LOG_TIME */

#ifdef CONFIG_TRACE
#	define trace(fmt, args...)	log("", fmt, ##args)
#else /* !CONFIG_TRACE */
#	define trace(fmt, ...)		do { } while (0)
#endif /* CONFIG_TRACE */

#ifdef CONFIG_ERROR
#	define error(fmt, args...)	log("ERROR", fmt, ##args)
#else /* !CONFIG_ERROR */
#	define error(fmt, ...)		do { } while (0)
#endif /* CONFIG_ERROR */

#ifdef CONFIG_WARN
#	define warn(fmt, args...)	log("WARNING", fmt, ##args)
#else /* !CONFIG_WARN */
#	define warn(fmt, ...)		do { } while (0)
#endif /* CONFIG_WARN */

#ifdef CONFIG_INFO
#	define info(fmt, args...)	log("", fmt, ##args)
#else /* !CONFIG_INFO */
#	define info(fmt, ...)		do { } while (0)
#endif /* CONFIG_INFO */

#undef assert
#ifdef CONFIG_ASSERT
	#define assert(_x_)							\
		do {								\
			if (!(_x_)) {						\
				tee_printf("assert fail at "#_x_ " %s:%d/%s()\n",\
						__FILE__, __LINE__, __func__);	\
				HALT();						\
			}							\
		}while(0)
#else
	#define assert(_x_)
#endif /* CONFIG_ASSERT */

#endif /* _LOG_H_ */
