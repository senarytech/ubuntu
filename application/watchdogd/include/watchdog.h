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

#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include <features.h>

__BEGIN_DECLS

int watchdog_init(const char *prefer);
void watchdog_start(void);
void watchdog_stop(void);
void watchdog_kick(void);
void watchdog_die(void);
unsigned watchdog_interval(void);

__END_DECLS

#endif /* __WATCHDOG_H__ */
