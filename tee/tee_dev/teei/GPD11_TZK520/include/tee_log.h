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

/** Definition of logging API for TEE.
 *
 * Purpose: This file defines a uniform macro for AMP TAs to output log in
 *          the same way as AMP does.
 *
 *     Version    Date                     Author
 *     V 1.01,    Feb 27 2014,             Yongsen Chen
 *        - change the copyright info to Marvell commercial notice.
 *        - use tab rather than space as the indent to make it consitent with
 *          other TEE code.
 *        - use hex for bit fields
 *     V 1.00,    Feb 12 2014,             Jun Ma
 */

#ifndef __TEE_LOG_H__
#define __TEE_LOG_H__

/** Definition of TEE debug log level
 */
typedef enum {
	TEE_LOG_FATAL = 0,	/**< For errors that system can't continue. */
	TEE_LOG_ERROR,		/**< For errors that system can still work. */
	TEE_LOG_HIGH,		/**< High level debug info.                 */
	TEE_LOG_MED,		/**< Medium level debug info.               */
	TEE_LOG_LOW,		/**< Low level debug info.                  */
	TEE_LOG_INFO,		/**< Periodical and trivial debug info.     */
	TEE_LOG_USER1,		/**< User specific debug info.              */
	TEE_LOG_USER2,		/**< User specific debug info.              */
	TEE_LOG_LEVEL_MAX	/**< MAX level should not exceed 7.         */
} TEE_LOG_LEVEL;


/*
 * Bellow bits are used to control which log to display.
 *
 * it's not to use simple log level is because sometimes the user may need log
 * FATAL and USER2 together.
 */
#ifdef CONFIG_LOG_FATAL
#define LOG_FATAL   0x01
#else
#define LOG_FATAL   0
#endif

#ifdef CONFIG_LOG_ERROR
#define LOG_ERROR   0x02
#else
#define LOG_ERROR   0
#endif

#ifdef CONFIG_LOG_HIGH
#define LOG_HIGH    0x04
#else
#define LOG_HIGH    0
#endif

#ifdef CONFIG_LOG_MED
#define LOG_MED     0x08
#else
#define LOG_MED     0
#endif

#ifdef CONFIG_LOG_LOW
#define LOG_LOW     0x10
#else
#define LOG_LOW     0
#endif

#ifdef CONFIG_LOG_INFO
#define LOG_INFO    0x20
#else
#define LOG_INFO    0
#endif

#ifdef CONFIG_LOG_USER1
#define LOG_USER1   0x40
#else
#define LOG_USER1   0
#endif

#ifdef CONFIG_LOG_USER2
#define LOG_USER2   0x80
#else
#define LOG_USER2   0
#endif

#define TEE_LOG_MASK (LOG_FATAL | LOG_ERROR | LOG_HIGH | LOG_MED |	\
		LOG_LOW | LOG_INFO | LOG_USER1 | LOG_USER2)


#ifdef CONFIG_TEE_LOG_ENABLE

#ifdef CONFIG_ANDROID_LOG

#ifndef LOG_TAG
#define LOG_TAG "AMP_TA"
#endif /* LOG_TAG */

#include "cutils/log.h"


/*
 *  Table for TEE log levels to ANDROID log levels conversion.
 */
static const unsigned char _tee_log_lev_map_tbl[] = {
	ANDROID_LOG_FATAL,		/**< TEE_LOG_FATAL  */
	ANDROID_LOG_ERROR,		/**< TEE_LOG_ERROR  */
	ANDROID_LOG_WARN,		/**< TEE_LOG_HIGH   */
	ANDROID_LOG_INFO,		/**< TEE_LOG_MED    */
	ANDROID_LOG_DEBUG,		/**< TEE_LOG_LOW    */
	ANDROID_LOG_VERBOSE,		/**< TEE_LOG_INFO   */
	ANDROID_LOG_VERBOSE,		/**< TEE_LOG_USER1  */
	ANDROID_LOG_VERBOSE,		/**< TEE_LOG_USER2  */
};

#define TEE_LOG(_lvl, ...)						\
	do {								\
		if ((1 << _lvl) & TEE_LOG_MASK) {			\
			__android_log_buf_print(LOG_ID_SYSTEM,		\
					_tee_log_lev_map_tbl[_lvl],	\
					LOG_TAG,			\
					__VA_ARGS__);			\
		}							\
	} while (0)


#else /* CONFIG_ANDROID_LOG */

#define TEE_LOG(_lvl, ...)						\
	do {								\
		if ((1 << _lvl) & TEE_LOG_MASK) {			\
			printf(__VA_ARGS__);				\
		}							\
	} while (0)

#endif /* CONFIG_ANDROID_LOG */

#else /* CONFIG_TEE_LOG_ENABLE */

#define TEE_LOG(_lvl, ...)   do { } while (0)

#endif

#define TEE_LOGF(...)        TEE_LOG(TEE_LOG_FATAL, __VA_ARGS__)
#define TEE_LOGE(...)        TEE_LOG(TEE_LOG_ERROR, __VA_ARGS__)
#define TEE_LOGH(...)        TEE_LOG(TEE_LOG_HIGH, __VA_ARGS__)
#define TEE_LOGM(...)        TEE_LOG(TEE_LOG_MED, __VA_ARGS__)
#define TEE_LOGL(...)        TEE_LOG(TEE_LOG_LOW, __VA_ARGS__)
#define TEE_LOGI(...)        TEE_LOG(TEE_LOG_INFO, __VA_ARGS__)
#define TEE_LOGU1(...)       TEE_LOG(TEE_LOG_USER1, __VA_ARGS__)
#define TEE_LOGU2(...)       TEE_LOG(TEE_LOG_USER2, __VA_ARGS__)


#endif /* __TEE_LOG_H__ */
