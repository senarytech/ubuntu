/*******************************************************************************
 * Copyright (C) 2016, Marvell International Ltd. and its affiliates
 *
 * This software file (the "File") is owned and distributed by Marvell
 * International Ltd. and/or its affiliates ("Marvell") under the following
 * alternative licensing terms.
 *
 *******************************************************************************
 * Marvell Commercial License Option
 *
 * If you received this File from Marvell and you have entered into a commercial
 * license agreement (a "Commercial License") with Marvell, the File is licensed
 * to you under the terms of the applicable Commercial License.
 *
 *******************************************************************************
 */

#ifndef __LOG_H__
#define __LOG_H__

#include <features.h>
#include <syslog.h>

__BEGIN_DECLS

void log_init(const char *prefer);
void log_clean(void);
void log_vprint(int level, const char *fmt, va_list ap);

void debug(const char *fmt, ...);
void info(const char *fmt, ...);
void error(const char *fmt, ...);
void alert(const char *fmt, ...);

#define DEBUG(x...)  debug("watchdogd: " x)
#define INFO(x...)   info("watchdogd: " x)
#define ERROR(x...)  error("watchdogd: " x)
#define ALERT(x...)  alert("watchdogd: " x)

__END_DECLS

#endif
