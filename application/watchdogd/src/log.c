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
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

#if defined(ANDROID) || defined(LOGCAT)
#include <cutils/log.h>
#endif

struct log_impl {
	const char *name;
	void (*vprint)(int level, const char *fmt, va_list ap);
	void (*init)(void);
	void (*clean)(void);
};

static int klog_fd = -1;
static int klog_level = LOG_ERR;
static const char *klog_devname = "/dev/__kmsg__";

static void klog_loginit(void)
{
    char *env;

    if (mknod(klog_devname, S_IFCHR | 0600, (1 << 8) | 11) == 0) {
        klog_fd = open(klog_devname, O_WRONLY);
        fcntl(klog_fd, F_SETFD, FD_CLOEXEC);
        unlink(klog_devname);
    }

    if ((env = getenv("watchdogd_loglevel")))
        klog_level = atoi(env) & 0x7;
}

#define LOG_BUF_MAX 512

static void klog_logvprint(int level, const char *fmt, va_list ap)
{
    static const char *prio[] = {
        "<0>", "<1>", "<2>", "<3>",
        "<4>", "<5>", "<6>", "<7>",
    };
    char buf[LOG_BUF_MAX];
    int len;

    if (level > klog_level) return;
    if (klog_fd < 0) return;

    buf[0] = 0;
    strcat(buf, prio[level]);
    len = vsnprintf(buf+3, LOG_BUF_MAX-3, fmt, ap);
    buf[LOG_BUF_MAX-1] = 0;
    write(klog_fd, buf, len+3);
}

static void klog_logclean(void)
{
    if (klog_fd > -1)
        close(klog_fd);
    klog_fd = -1;
}

#ifdef ANDROID
static const char *android_logtag = "watchdogd";

static void android_loginit(void)
{
	char buf[128];

	snprintf(buf, sizeof(buf), "watchdog daemon %s", VERSION);

	__android_log_print(ANDROID_LOG_INFO, android_logtag, "%s", buf);
}

static void android_logvprint(int level, const char *fmt, va_list ap)
{
	static android_LogPriority androidlevel[] = {
		ANDROID_LOG_FATAL,
		ANDROID_LOG_ERROR,
		ANDROID_LOG_WARN,
		ANDROID_LOG_INFO,
		ANDROID_LOG_DEBUG,
		ANDROID_LOG_VERBOSE,
		ANDROID_LOG_VERBOSE,
		ANDROID_LOG_VERBOSE
	};

	__android_log_vprint(androidlevel[level&0x07], android_logtag, fmt, ap);
}
#endif /* ANDROID */

static void syslog_loginit(void)
{
	int option = LOG_NDELAY | LOG_PID;

	openlog("watchdogd", option, LOG_DAEMON);

	syslog(LOG_INFO, "watchdog deamon %s", VERSION);
}

static void syslog_logvprint(int level, const char *fmt, va_list ap)
{
	vsyslog(level, fmt, ap);
}

static void syslog_logclean(void)
{
	closelog();
}

#define ARRAY_SIZE(x)	((sizeof(x)/sizeof(x[0])))

static struct log_impl __logops[] = {
    { "klog", klog_logvprint, klog_loginit, klog_logclean },
#ifdef ANDROID
	{ "android", android_logvprint, android_loginit, NULL },
#endif
	{ "syslog", syslog_logvprint, syslog_loginit, syslog_logclean },
};

static int __log_num = 0;

void log_init(const char *prefer)
{
	const char *logname = (prefer) ? prefer : getenv("watchdogd_log");
	int i;

	if (logname) {
		for (i = 0; i < (int)ARRAY_SIZE(__logops); i++) {
			if (strcmp(logname, __logops[i].name) == 0) {
				__log_num = i;
                break;
            }
        }
	}

	__logops[__log_num].init();
}

void log_clean(void)
{
	__logops[__log_num].clean();
}

void log_vprint(int level, const char *fmt, va_list ap)
{
	__logops[__log_num].vprint(level&0x7, fmt, ap);
}

void info(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_vprint(LOG_INFO, fmt, ap);
	va_end(ap);
}

void error(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_vprint(LOG_ERR, fmt, ap);
	va_end(ap);
}

void debug(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_vprint(LOG_DEBUG, fmt, ap);
	va_end(ap);
}

void alert(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_vprint(LOG_ALERT, fmt, ap);
	va_end(ap);
}
