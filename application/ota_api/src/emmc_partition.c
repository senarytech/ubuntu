/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright Â© 2022-2026 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated (â€œSynapticsâ€). The holder of this file shall treat all
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
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS,â€ AND
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
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICSâ€™ TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "emmc_partition.h"

static char* device_path = "/dev/";
static bool partition_map_ready = false;
static char* cmd = "cat ";
static char* dev_path = "/sys/block/mmcblk0/";
static char* uevent = "/uevent";
static char mmcblk0px[PARTITION_NUM][NAME_LEN] = {
	"mmcblk0p1", "mmcblk0p2", "mmcblk0p3", "mmcblk0p4",
	"mmcblk0p5", "mmcblk0p6", "mmcblk0p7", "mmcblk0p8",
	"mmcblk0p9", "mmcblk0p10", "mmcblk0p11", "mmcblk0p12",
	"mmcblk0p13", "mmcblk0p14", "mmcblk0p15","mmcblk0p16",
	"mmcblk0p17", "mmcblk0p18", "mmcblk0p19","mmcblk0p20",
	"mmcblk0p21", "mmcblk0p22", "mmcblk0p23","mmcblk0p24",
	"mmcblk0p25", "mmcblk0p26", "mmcblk0p27","mmcblk0p28",
	"mmcblk0p29", "mmcblk0p30", "mmcblk0p31","mmcblk0p32",
};

static char *PARTNAME = "PARTNAME=";
char partitions[PARTITION_NUM][NAME_LEN];
static int partitions_num = 0;

static bool get_partition_mmcblk() {
	FILE *fp;
	int i;
	char dev_name[DEV_LEN];
	char command[CMD_LEN];
	char buf[BUF_SIZE];

	if(partition_map_ready)
		return true;

	for(int i = 0; i < PARTITION_NUM; i++) {
		memset(command,0,sizeof(command));
		memset(dev_name,0,sizeof(dev_name));

		strcpy(command, cmd);
		strcpy(dev_name, dev_path);
		strcat(dev_name, mmcblk0px[i]);
		strcat(dev_name, uevent);
		strcat(command, dev_name);

		fp = popen(command, "r");
		if (fp == NULL) {
			printf("Failed to run command\n" );
			return false;
		}

		while (fgets(buf, sizeof(buf), fp) != NULL) {
			char *p = strstr(buf, PARTNAME);
			if(p != NULL) {
				p += strlen( PARTNAME);
				strncpy(partitions[i], p, strlen(p)-1);
				partitions_num++;

				if(!strcmp(partitions[i], "fts")) {
					char cmd[CMD_LEN];
					system("mkdir /dev/block");
					system("mkdir /dev/block/by-name");
					system("rm /dev/block/by-name/fts");
					strcpy(cmd, "ln -s ");
					strcat(cmd, device_path);
					strcat(cmd, mmcblk0px[i]);
					strcat(cmd, " /dev/block/by-name/fts");
					system(cmd);
				}
				break;
			}
		}
		pclose(fp);
	}

	partition_map_ready = true;
	return true;
}

static bool getPartitionIndex(const char* partition, int *index) {
	for(*index = 0; *index < PARTITION_NUM; ++(*index)) {
		if(strcmp(partitions[*index], partition) == 0)
			return true;
	}
	return false;
}

bool get_mmcblk0px(const char* partition, char *mmcblk, int *size) {
	int i;

	if(partition_map_ready == false) {
		if(!get_partition_mmcblk())
			return false;
		partition_map_ready = true;
	}
	if(!getPartitionIndex(partition, &i)) {
		return false;
	}

	strcpy(mmcblk, mmcblk0px[i]);
	*size = strlen(mmcblk0px[i]);

	return true;
}

static bool getDeviceIndex(const char* mmcblk, int *index) {
	for(*index = 0; *index < PARTITION_NUM; ++(*index)) {
		if(strcmp(mmcblk0px[*index], mmcblk) == 0)
			return true;
	}
	return false;
}

bool get_partition(const char* mmcblk, char *partition, int *size) {
	int i;

	if(partition_map_ready == false) {
		if(!get_partition_mmcblk())
			return false;
		partition_map_ready = true;
	}
	if(!getDeviceIndex(mmcblk, &i)) {
		return false;
	}

	strcpy(partition, partitions[i]);
	*size = strlen(partitions[i]);

	return true;
}

bool isValidPartition(const char* partition) {
	if(partition_map_ready == false) {
		if(!get_partition_mmcblk())
			return false;
		partition_map_ready = true;
	}

	for(int i = 0; i < PARTITION_NUM; i++)	{
		if(strcmp(partitions[i], partition) == 0)
			return true;
	}

	return false;
}

bool get_dev(const char* partition, char* dev) {
	int len;
	char mmcblk[DEV_LEN];

	if(!get_mmcblk0px(partition, mmcblk, &len))
		return false;

	strcpy(dev, device_path);
	strncat(dev, mmcblk, len);

	return true;
}

int getPartitionCount() {
	if(partition_map_ready == false) {
		if(!get_partition_mmcblk())
			return false;
		partition_map_ready = true;
	}

	return partitions_num;
}

int getPartitionSize(const char* partition) {
	FILE *fd;
	char dev[DEV_LEN];
	uint32_t size;

	if(!get_dev(partition, dev)) {
		return 0;
	}

	fd = fopen(dev, "r");
	if(fd == NULL) {
		return 0;
	}

	if (fseek(fd, 0, SEEK_END) != 0) {
		fclose(fd);
		return 0;
	}

	size = ftell(fd);

	fclose(fd);
	return size;
}
