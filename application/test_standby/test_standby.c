/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2018 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner
 * unless Synaptics has otherwise provided express, written
 * permission.
 *
 * Use of the materials may require a license of intellectual property
 * from a third party or from Synaptics. This file conveys no express
 * or implied licenses to any intellectual property rights belonging
 * to Synaptics.
 *
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
 * SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 * INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
 * BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
 * COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */

////////////////////////////////////////////////////////////////////////////////
//! \file test_standby.c
//!
//! \brief standby mode test code for pure linux SDK.
//!
//!
//! Purpose:
//!
//!
//! Version, Date and Author :
//!   V0.1,    May/15/2019,    Jie Zhang
//! Note:
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

#include "OSAL_api.h"
#include "amp_client.h"
#include "amp_client_support.h"

/* Macro Define */
#define SCALING_AVAILABLE_FREQUENCIES \
	"/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies"
#define MAX_CPU_NUM 10

/* Struct definition */
struct cpu_info {
	unsigned int max_available_freq;
	unsigned int min_available_freq;
	unsigned int available_freqs[MAX_CPU_NUM];
	unsigned int available_freqs_num;
	unsigned int cpu_core_num;
};

typedef struct command_s {
    const char *name;
    const char *alias;
    const char *help;
    int (*cmd)(int, char **);
} command_t;


/* Global variable */
static char *client_argv[] =
      {"client", "iiop:1.0//127.0.0.1:999/AMP::FACTORY/factory"};

static AMP_FACTORY hFactory;
static AMP_LIB hAMPLib;
static AMP_DISP gDisp;
static struct cpu_info syna_cpuf_info;


static int sysfs_read(const char *path, char *s, const int num_bytes)
{
    char buf[80];
    int count;
    int fd = open(path, O_RDONLY);

    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        printf("Error opening %s: %s\n", path, buf);
        return -1;
    }

    if ((count = read(fd, s, (num_bytes - 1))) < 0) {
        strerror_r(errno, buf, sizeof(buf));
        printf("Error reading from  %s: %s\n", path, buf);
    } else {
        if ((count >= 1) && (s[count-1] == '\n')) {
            s[count-1] = '\0';
        } else {
            s[count] = '\0';
        }
    }
    close(fd);

    return count;
}

static int sysfs_write(const char *path, char *s)
{
    char buf[80];
    int count;
    int fd = open(path, O_WRONLY);

    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        printf("Error opening %s: %s\n", path, buf);
        return -1;
    }

    if ((count = write(fd, s, strlen(s))) < 0) {
        strerror_r(errno, buf, sizeof(buf));
        printf("Error writing to  %s: %s\n", path, buf);
    }

    close(fd);

    return count;
}

static int get_cpu_core_num(struct cpu_info *pSynaCpuInfo)
{
	char cpu_core_online[64];
	int num = 0;

	do {
		snprintf(cpu_core_online, 64, "/sys/devices/system/cpu/cpu%u/online", num);
		if (access(cpu_core_online, F_OK) == 0)
			num++;
		else
			break;
	} while (1);

	if (num == 0)
	{
		printf("Didn't enable CPU info in kernel driver!!\n");
		return -1;
	}
	else
	{
		pSynaCpuInfo->cpu_core_num = num;
		printf("This CPU has %d core.\n", pSynaCpuInfo->cpu_core_num);
	}

	return 0;
}

static int get_cpu_freqs(struct cpu_info *pSynaCpuInfo)
{
	unsigned int temp_val;
	unsigned int max_available_freq = 0;
	unsigned int min_available_freq = 0;
	int count = 0;
    char buf[80];

	FILE* pfile = fopen(SCALING_AVAILABLE_FREQUENCIES, "r");
	if (pfile < 0) {
		strerror_r(errno, buf, sizeof(buf));
		return -1;
	}

	while (fscanf(pfile, " %u", &temp_val) > 0)
	{
		/* Update max/min available frequency*/
		if (count == 0)
		{
			max_available_freq = temp_val;
			min_available_freq = temp_val;
		}
		else
		{
			if (temp_val > max_available_freq)
				max_available_freq = temp_val;
			else if (temp_val < min_available_freq)
				min_available_freq = temp_val;
		}

		/* Store available frequency*/
		pSynaCpuInfo->available_freqs[count] = temp_val;
		count++;
	}

	pSynaCpuInfo->available_freqs_num = count;
	pSynaCpuInfo->max_available_freq = max_available_freq;
	pSynaCpuInfo->min_available_freq = min_available_freq;

	if (pSynaCpuInfo->available_freqs_num == 0)
	{
		printf("No available freqs!!!!\n");
		return -1;
	}
	else
		printf("Freqs count: %u, Max_freq: %u, Min_freq: %u\n",
			pSynaCpuInfo->available_freqs_num,
			pSynaCpuInfo->max_available_freq,
			pSynaCpuInfo->min_available_freq);

	return 0;
}

static int set_cpu_freq(int min_freq, int max_freq)
{
	char cpu_freq[10];

	snprintf(cpu_freq, 10, "%u", min_freq);
	sysfs_write("/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq", cpu_freq);

	snprintf(cpu_freq, 10, "%u", max_freq);
	sysfs_write("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq", cpu_freq);

	return 0;
}

static int syna_get_cpu_info(struct cpu_info *pSynaCpuInfo)
{
	int ret = 0;

	/* Get CPU Core Number*/
	ret = get_cpu_core_num(pSynaCpuInfo);
	if (ret)
	{
		printf("Get CPU Core Number Fail!\n");
		goto __exit;
	}

	/* Get CPU frequency */
	ret = get_cpu_freqs(pSynaCpuInfo);
	if (ret)
	{
		printf("Get CPU CPU frequency Fail!\n");
		goto __exit;
	}

__exit:
	return ret;
}

static int adjust_cpu_cores(struct cpu_info *pSynaCpuInfo, bool on)
{
	char cpu_core_online[64];
	int ret = 0;

	if (pSynaCpuInfo->cpu_core_num > 0)
	{
		/* Offline/Online CPU1/2/3 */
		int i = 1;
		do {
			snprintf(cpu_core_online, 64, "/sys/devices/system/cpu/cpu%u/online", i);
			if (on)
				sysfs_write(cpu_core_online, "1");
			else
				sysfs_write(cpu_core_online, "0");
			i++;
		} while (i < pSynaCpuInfo->cpu_core_num);
	}
	else
	{
		printf("CPU Core number less than 0!!!!\n");
	}

	return ret;
}

static int syna_amp_init()
{
	int ret = 0;
	CORBA_Environment ev;

	ret = MV_OSAL_Init();
	if (ret){
		printf("MV_OSAL_INIT faild, ret=0x%x\n", ret);
		return ret;
	}

	ret = AMP_Initialize(2, client_argv, &hFactory);
	if (ret){
		printf("AMP_Initialize faild, ret=0x%x\n", ret);
		return ret;
	}

	ret = AMP_GetFactory(&hFactory);
	if (ret){
		printf("AMP_GetFactory faild, ret=0x%x\n", ret);
		return ret;
	}

	/* Create AMP lib */
	AMP_RPC(ret, AMP_FACTORY_CreateLibrary, hFactory, &hAMPLib);
	if (ret != SUCCESS) {
		printf("AMP_FACTORY_CreateLibrary failed, ret=0x%x", ret);
	}

	/* Create disp service */
	AMP_RPC(ret, AMP_FACTORY_CreateDisplayService, hFactory, &gDisp);
	if (ret != SUCCESS) {
		printf("AMP_FACTORY_CreateDisplayService failed, ret=0x%x", ret);
		goto __exit;
	}

	return 0;

__exit:
	MV_OSAL_Exit();

	return 0;
}

static int syna_amp_deinit()
{
	int ret = 0;

	ret = AMP_Deinitialize();
	if (ret){
		printf("AMP_Deinitialize faild, ret=0x%x\n", ret);
		return ret;
	}

	ret = MV_OSAL_Exit();
	if (ret){
		printf("AMP_Deinitialize faild, ret=0x%x\n", ret);
		return ret;
	}

	return 0;
}

static int syna_set_hdmi(bool on)
{
	int ret = 0;

	if (on)
	{
		AMP_RPC(ret, AMP_DISP_SetPortPower, gDisp, AMP_DISP_PORT_HDMI, 1);
		if (ret != SUCCESS) {
			printf("AMP_DISP_SetPortPower on failed, ret=0x%x\n", ret);
			goto __exit;
		}

		AMP_RPC(ret, AMP_DISP_OUT_SetMute, gDisp, AMP_DISP_OUT_HDMI, 0);
		if (ret != SUCCESS) {
			printf("AMP_DISP_OUT_SetMute off failed, ret=0x%x\n", ret);
			goto __exit;
		}
	}
	else
	{
		AMP_RPC(ret, AMP_DISP_OUT_SetMute, gDisp, AMP_DISP_OUT_HDMI, 1);
		if (ret != SUCCESS) {
			printf("AMP_DISP_OUT_SetMute on failed, ret=0x%x\n", ret);
			goto __exit;
		}

		AMP_RPC(ret, AMP_DISP_SetPortPower, gDisp, AMP_DISP_PORT_HDMI, 0);
		if (ret != SUCCESS) {
			printf("AMP_DISP_SetPortPower off failed, ret=0x%x\n", ret);
			goto __exit;
		}
	}

	printf("syna_set_hdmi %s\n", on ? "on" : "off");

__exit:
	return ret;
}

static int syna_amp_clock_gating(bool on)
{
	int LOW_POWER_ENTER = 2;	// low power mode
	int LOW_POWER_RESUME = 3;	// resume from low power mode
	int ret = 0;

	AMP_RPC(ret, AMP_LIB_ActiveStandby, hAMPLib,
			on ? LOW_POWER_RESUME : LOW_POWER_ENTER);
	if (ret != SUCCESS) {
		printf("amp clock gating: %s: fail", on ? "on" : "off");
	} else {
		printf("amp clock gating: %s: success", on ? "on" : "off");
	}

	return ret;
}

static int syna_fast_standby(int argc, char *argv[])
{
	int ret = 0;
	unsigned int max_cpu_freq = syna_cpuf_info.max_available_freq;
	unsigned int min_cpu_freq = syna_cpuf_info.min_available_freq;

	/* */
	ret = syna_get_cpu_info(&syna_cpuf_info);
	if (ret)
	{
		printf("syna_get_cpu_info failed\n");
		goto __exit;
	}

	if (strncmp(argv[1], "enter", 5) == 0)
	{
		/* Mute HDMI output and power off HDMI port */
		ret = syna_set_hdmi(0);
		if (ret != SUCCESS) {
			printf("syna_set_hdmi off failed, ret=0x%x\n", ret);
			goto __exit;
		}

		/* CPM clock gating */
		ret = syna_amp_clock_gating(0);
		if (ret != SUCCESS) {
			printf("syna_amp_clock_gating off failed, ret=0x%x\n", ret);
			goto __exit;
		}

		/* Set CPU clock frequency*/
		ret = set_cpu_freq(min_cpu_freq, min_cpu_freq);
		if (ret != SUCCESS) {
			printf("set_cpu_freq failed, ret=0x%x\n", ret);
			goto __exit;
		}

		/* Offline CPU1/2/3 */
		ret = adjust_cpu_cores(&syna_cpuf_info, 0);
		if (ret != SUCCESS) {
			printf("Offline CPU1/2/3 failed, ret=0x%x\n", ret);
			goto __exit;
		}
	}
	else if (strncmp(argv[1], "exit", 4) == 0)
	{
		/* Online CPU1/2/3 */
		ret = adjust_cpu_cores(&syna_cpuf_info, 1);
		if (ret != SUCCESS) {
			printf("Online CPU1/2/3 failed, ret=0x%x\n", ret);
			goto __exit;
		}

		/* Set CPU clock frequency*/
		ret = set_cpu_freq(min_cpu_freq, max_cpu_freq);
		if (ret != SUCCESS) {
			printf("set_cpu_freq failed, ret=0x%x\n", ret);
			goto __exit;
		}

		/* CPM clock gating */
		ret = syna_amp_clock_gating(1);
		if (ret != SUCCESS) {
			printf("syna_amp_clock_gating on failed, ret=0x%x\n", ret);
			goto __exit;
		}

		/* Unmute HDMI output and power on HDMI port */
		ret = syna_set_hdmi(1);
		if (ret != SUCCESS) {
			printf("syna_set_hdmi on failed, ret=0x%x\n", ret);
			goto __exit;
		}
	}
	else
	{
		printf("Common Error: sample as below:\n"
				"test_standby -f enter\ntest_standby -f exit\n");
	}

__exit:
	return ret;
}

static int syna_active_standby(int argc, char *argv[])
{
	int ret = 0;

	/* Mute HDMI output and power off HDMI port */
	ret = syna_set_hdmi(0);
	if (ret != SUCCESS) {
		printf("syna_set_hdmi off failed, ret=0x%x\n", ret);
		goto __exit;
	}

	/* Active mode, VCPU is powered off, VCore still power on. */
	system("echo mem > /sys/power/state");

	/* Unmute HDMI output and power on HDMI port */
	ret = syna_set_hdmi(1);
	if (ret != SUCCESS) {
		printf("syna_set_hdmi on failed, ret=0x%x\n", ret);
		goto __exit;
	}
	printf("Welcome back from active standby!!!\n");

__exit:
	return ret;
}

static int syna_suspend_standby(int argc, char *argv[])
{
	int ret = 0;

	/* Mute HDMI output and power off HDMI port */
	ret = syna_set_hdmi(0);
	if (ret != SUCCESS) {
		printf("syna_set_hdmi off failed, ret=0x%x\n", ret);
		goto __exit;
	}

	/* Suspend mode, VCPU and VCore all are powered off. */
	system("echo standby > /sys/power/state");

	/* Unmute HDMI output and power on HDMI port */
	ret = syna_set_hdmi(1);
	if (ret != SUCCESS) {
		printf("syna_set_hdmi on failed, ret=0x%x\n", ret);
		goto __exit;
	}
	printf("Welcome back from suspend standby!!!\n");

__exit:
	return ret;
}

static int syna_normal_standby(int argc, char *argv[])
{
	int ret = 0;

	/* Mute HDMI output and power off HDMI port */
	ret = syna_set_hdmi(0);
	if (ret != SUCCESS) {
		printf("syna_set_hdmi off failed, ret=0x%x\n", ret);
		goto __exit;
	}

	/* Normal standby, system reboot: Power down */
	system("poweroff");

__exit:
	return ret;
}


static command_t commands[] = {
    { "fast", "-f", "-f enter | enter fast standby", syna_fast_standby },
    { "fast", "-f", "-f exit  | exit fast standby", syna_fast_standby },
    { "active", "-a", "-a | active standby", syna_active_standby },
    { "suspend", "-s", "-s | suspend standby", syna_suspend_standby},
    { "normal", "-n", "-n | normal standby", syna_normal_standby},
    { 0, 0, 0, 0 },
};

static const char *prog_name;

static void usage(int error)
{
    int i;
    fprintf(stdout, "Usage:\n");
    for (i = 0; commands[i].name != NULL; i++) {
        if (commands[i].help != NULL)
            fprintf(stdout, "  %s %s\n", prog_name, commands[i].help);
    }
    exit(error);
}

static int do_cmd(const char *cmd, int argc, char *argv[])
{
    int i;
    for (i = 0; commands[i].name != NULL; i++) {
        if (strcmp(cmd, commands[i].name) == 0 ||
            ((commands[i].alias != NULL) &&
             (strcmp(cmd, commands[i].alias) == 0)))
            return commands[i].cmd(argc, argv);
    }
    return -1;
}

int main(int argc, char *argv[])
{
    int ret;

    prog_name = strrchr(argv[0], '/');
    if (prog_name) prog_name++;
    else prog_name = argv[0];

	if ((argc == 1) || (strncmp(argv[1], "help", 4) == 0))
	{
		printf("Standby test command format.\n");
		usage(0);
		return 0;
	}

	/* Init AMP Resource */
	syna_amp_init();

	/* Run function */
	if (do_cmd(argv[1], argc-1, argv+1))
	{
		printf("Test command not correct!!!\n");
		usage(1);
	}

	/* Deinit AMP Resource */
	syna_amp_deinit();

    return 0;
}
