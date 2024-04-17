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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "log.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)   (sizeof(x)/sizeof(x[0]))
#endif

struct watchdog_impl {
    const char *name;
    const unsigned interval;
    int  (*init)(void);
    void (*clean)(void);
    void (*start)(void);
    void (*stop)(void);
    void (*kick)(void);
    void (*die)(void);
};

#ifdef HAS_SM
#include "com_type.h"
#include "sm_common.h"
#include "sm_agent.h"

static void sm_send_ints(int id, int msg[], int len)
{
    MV_SM_Message sm_msg;
    int *pMsg, i;

    memset(&sm_msg, 0, sizeof (sm_msg));
    sm_msg.m_iModuleID = id;
    pMsg = (int *) sm_msg.m_pucMsgBody;
    for (i = 0; i < len; i++)
        pMsg[i] = msg[i];
    sm_msg.m_iMsgLen = len * sizeof (int);

    MV_SM_Agent_Write_Msg(sm_msg.m_iModuleID,
            sm_msg.m_pucMsgBody, sm_msg.m_iMsgLen);
}

static int sm_msg_watchdog_start[]   = { 1, MV_SM_WD_APP_START };
static int sm_msg_watchdog_exit[]    = { 1, MV_SM_WD_APP_EXIT };
static int sm_msg_watchdog_kickoff[] = { 1, MV_SM_WD_Kickoff };

static int sm_watchdog_init(void)
{
    int has_sm = 1;

    if (!has_sm)
        return -ENODEV;

    MV_SM_Agent_Init();

    return 0;
}

static void sm_watchdog_clean(void)
{
    MV_SM_Agent_Close();
}

static void sm_watchdog_start(void)
{
    sm_send_ints(MV_SM_ID_WDT, sm_msg_watchdog_start,
            ARRAY_SIZE(sm_msg_watchdog_start));
}

static void sm_watchdog_stop(void)
{
    sm_send_ints(MV_SM_ID_WDT, sm_msg_watchdog_exit,
            ARRAY_SIZE(sm_msg_watchdog_exit));
}

static void sm_watchdog_kick(void)
{
    sm_send_ints(MV_SM_ID_WDT, sm_msg_watchdog_kickoff,
            ARRAY_SIZE(sm_msg_watchdog_kickoff));
}

static void sm_watchdog_die(void)
{
    ALERT("pause until sm reset\n");
    for (;;) sleep(1000);
}

struct watchdog_impl sm_watchdog_impl = {
    .name       = "sm",
    .interval   = 1000, /* in ms */
    .init       = sm_watchdog_init,
    .clean      = sm_watchdog_clean,
    .start      = sm_watchdog_start,
    .kick       = sm_watchdog_kick,
    .stop       = sm_watchdog_stop,
    .die        = sm_watchdog_die,
};
#endif /* HAS_SM */

struct watchdog_impl dummy_watchdog_impl = {
    .name       = "dummy",
    .interval   = 1000000, /* in ms */
};

static struct watchdog_impl *__watchdogops[] = {
    &dummy_watchdog_impl,
#ifdef HAS_SM
    &sm_watchdog_impl,
#endif
    //&gpio_watchdog_impl,
    //&monitord_watchdog_impl,
    NULL,
};

static int __watchdog_num = 0;

int watchdog_init(const char *prefer)
{
    const char *meth = (prefer) ? prefer : getenv("watchdogd_meth");

    if (meth) {
        int i;
        for (i = 0; __watchdogops[i]; i++) {
            if (strcmp(__watchdogops[i]->name, meth) == 0) {
                __watchdog_num = i;
                break;
            }
        }
    }

    INFO("select watchdog '%s'\n", __watchdogops[__watchdog_num]->name);

    if (__watchdogops[__watchdog_num]->init)
        return __watchdogops[__watchdog_num]->init();
    return 0;
}

void watchdog_clean(void)
{
    if (__watchdogops[__watchdog_num]->clean)
        __watchdogops[__watchdog_num]->clean();
}

void watchdog_start(void)
{
    if (__watchdogops[__watchdog_num]->start)
        __watchdogops[__watchdog_num]->start();
}

void watchdog_stop(void)
{
    if (__watchdogops[__watchdog_num]->stop)
        __watchdogops[__watchdog_num]->stop();
}

void watchdog_kick(void)
{
    if (__watchdogops[__watchdog_num]->kick)
        __watchdogops[__watchdog_num]->kick();
}

void watchdog_die(void)
{
    if (__watchdogops[__watchdog_num]->die)
        __watchdogops[__watchdog_num]->die();
}

unsigned watchdog_interval(void)
{
    return __watchdogops[__watchdog_num]->interval;
}
