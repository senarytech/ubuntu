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
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/poll.h>
#include "log.h"
#include "watchdog.h"

#ifndef TEMP_FAILURE_RETRY
# define TEMP_FAILURE_RETRY(expr) ({ \
        typeof(expr) __result; \
        do __result = (expr); \
        while (__result < 0 && errno == EINTR); \
        __result; })
#endif

static int run_process(const char *path, uid_t uid, gid_t gid, char *const argv[])
{
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        ERROR("fork failed: %s (%d)\n", strerror(errno), errno);
        return -1;
    }

    if (pid == 0) {
        if (uid) {
            if (setuid(uid) < 0) {
                ERROR("setuid failed: %s (%d)\n", strerror(errno), errno);
                _exit(127);
            }
        }

        if (gid) {
            if (setgid(gid) < 0) {
                ERROR("setgid failed: %s (%d)\n", strerror(errno), errno);
                _exit(127);
            }
        }

        INFO("exec %s\n", path);
        execv(path, argv);

        _exit(127);
    }

    return pid;
}

static int service_init(void)
{
    const char oom_adj_str[] = "-17";
    int fd = open("/proc/self/oom_adj", O_WRONLY);
    if (fd > -1) {
        write(fd, oom_adj_str, sizeof(oom_adj_str)-1);
        close(fd);
    }
    return 0;
}

static int signal_fd, signal_recv_fd;

static void signal_handler(int signum)
{
    TEMP_FAILURE_RETRY(write(signal_fd, &signum, sizeof(signum)));
}

static int signal_init(void)
{
    int fds[2];
    pipe(fds);
    signal_recv_fd = fds[0];
    signal_fd = fds[1];
    signal(SIGCHLD, signal_handler);
    return 0;
}

static int watchdogd_main(uid_t uid, gid_t gid, int argc, char *const argv[])
{
    int pid = -1;

    service_init();

    signal_init();

    if (argc > 0) {
        pid = run_process(argv[0], uid, gid, argv);
        if (pid < 0) {
            ERROR("failed to run %s : %s (%d)\n",
                    argv[0], strerror (errno), errno);
            return -errno;
        }
    }

    watchdog_start();

    for (;;) {
        struct pollfd pfd;
        int res;

        pfd.fd = signal_recv_fd;
        pfd.events = POLLIN;
        pfd.revents = 0;

        res = TEMP_FAILURE_RETRY(poll(&pfd, 1, watchdog_interval()));
        if (res == 0) {
            watchdog_kick();
            continue;
        } else if (res == 1) {
            int signum, status;

            TEMP_FAILURE_RETRY(read(signal_recv_fd, &signum, sizeof(signum)));

            res = TEMP_FAILURE_RETRY(waitpid(-1, &status, 0));
            if (res <= 0) {
                if (res == 0)
                    continue;
                ERROR("waitpid failed : %s (%d)\n", strerror(errno), errno);
            } else {
                ALERT("waitpid returnned pid %d, status = %d\n", res, WEXITSTATUS(status));
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
                    watchdog_stop();
                else
                    watchdog_die();
            }
        } else {
            ERROR("poll failed: %s (%d)\n", strerror(errno), errno);
        }
        break;
    }

    watchdog_clean();

    return 0;
}

static void usage(int error)
{
    FILE *fp = (error) ? stdout : stderr;
    fprintf(fp, "Usage: watchdogd -d -l [klog|android|syslog] -m [sm|gpio|monitord]\n");
    fprintf(fp, "                 -u uid -g gid [executable [arg ...]]\n");
    exit(error);
}

#define usage_ifnot(expr) ({ if (!(expr)) usage(1); })

static void fatal(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(1);
}

int main(int argc, char *argv[])
{
    int i, background = 0;
    const char *logname = NULL, *meth = "sm";
    uid_t uid = 0;
    gid_t gid = 0;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] != '-')
            break;
        switch (argv[i][1]) {
        case 'd':
            background = 1;
            break;
        case 'l':
            usage_ifnot(++i < argc);
            logname = argv[i];
            break;
        case 'm':
            usage_ifnot(++i < argc);
            meth = argv[i];
            break;
        case 'u':
            usage_ifnot(++i < argc);
            uid = atoi(argv[i]);
            break;
        case 'g':
            usage_ifnot(++i < argc);
            gid = atoi(argv[i]);
            break;
        default:
            usage(1);
        }
    }

    if (i < argc && (access(argv[i], R_OK|X_OK) != 0))
        fatal("%s is either not readable or not executable\n", argv[i]);

    if (background)
        daemon(0, 0);

    log_init(logname);

    watchdog_init(meth);

    return watchdogd_main(uid, gid, argc-i, argv+i);
}
