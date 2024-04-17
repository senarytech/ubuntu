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
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "emmc_api.h"
#include "emmc_boot_ctrl.h"
#include "emmc_partition.h"

extern char partitions[PARTITION_NUM][NAME_LEN];
static partition_list_t list;
static partition_info_t p_info[PARTITION_NUM];

static void error_handle(FILE *fd, char *buf, char *msg) {
	if(msg != NULL) printf("%s\n", msg);
	if(buf != NULL) free(buf);
	if(fd != NULL) fclose(fd);
}

int cpr_emmc_get_selected_slot(void) {
	if(isValidPartition("misc")) {
		printf("misc found \n");
		return getActiveBootSlot();
	}
	if(isValidPartition("fts")) {
		printf("fts found \n");
		return getFtsActiveBootSlot();
	}
	return EMMC_FAILED;
}

int cpr_emmc_get_running_slot(void) {
	if(isValidPartition("misc"))
		return getActiveBootSlot();
	if(isValidPartition("fts"))
		return getFtsActiveBootSlot();

	return EMMC_FAILED;
}

int cpr_emmc_select_slot(emmc_slot_t part) {
	if(isValidPartition("misc"))
		return setActiveBootSlot(part);
	if(isValidPartition("fts"))
		return setFtsActiveBootSlot(part);
	return EMMC_FAILED;
}

unsigned int cpr_emmc_get_block_size(void) {
	struct stat fi;
	char dev[DEV_LEN];
	if(!get_dev("factory_setting", dev))
		return 0;

	stat(dev, &fi);
	return fi.st_blksize;
}

partition_list_t* cpr_emmc_get_partition_list(void) {
	int i;
	int count = getPartitionCount();

	for(i = 0; i < count; i++) {
		strcpy(p_info[i].name, partitions[i]);
		p_info[i].size = getPartitionSize(partitions[i]);
	}

	list.count = count;
	list.partitions = p_info;

	return &list;
}

int cpr_emmc_erase_partition(const char* partition) {
	char dev[DEV_LEN];
	FILE *fd;
	char *buf;
	char msg[BUF_SIZE];
	unsigned int fsize;
	int offset, size;
	int blk_size = cpr_emmc_get_block_size();

	if(blk_size <= 0)
		blk_size = BLOCK_SIZE;

	buf = calloc(blk_size, sizeof(char));
	if(!buf)
		EMMC_FAILED;

	if(!get_dev(partition, dev)) {
		printf("[EMMC_API_LOG] invalid partition\n");
		free(buf);
		return EMMC_FAILED;
	}

	fd = fopen(dev, "w");
	if (fd == NULL) {
		printf("[EMMC_API_LOG] Can not open the device file\n");
		free(buf);
		return EMMC_FAILED;
	}

	if (fseek(fd, 0, SEEK_END) != 0) {
		strcpy(msg,"[EMMC_API_LOG] Failed to fseek to end of partition");
		error_handle(fd, buf, msg);
		return EMMC_FAILED;
	}

	fsize = ftell(fd);
	if(fseek(fd, 0, SEEK_SET) != 0) {
		strcpy(msg,"[EMMC_API_LOG] Failed to fseek to start of partition");
		error_handle(fd, buf, msg);
		return EMMC_FAILED;
	}

	offset = 0;
	while(fsize > 0) {
		if(fsize > blk_size)
			size = blk_size;
		else
			size = fsize;

		if (fseek(fd, offset, SEEK_SET) != 0) {
			strcpy(msg,"[EMMC_API_LOG] Failed to set offset for partition");
			error_handle(fd, buf, msg);
			return EMMC_IO_ERR;
		}

		if (fwrite(buf, sizeof(char), size, fd) != size) {
			strcpy(msg,"[EMMC_API_LOG] Failed to write partition");
			error_handle(fd, buf, msg);
			return EMMC_IO_ERR;
		}

		fsize -= size;
		offset += size;
	}

	error_handle(fd, buf, NULL);
	return 0;
}

int cpr_emmc_erase_blocks(const char* partition, unsigned int start_block, unsigned int block_num) {
	char dev[DEV_LEN];
	FILE *fd;
	char *buf;
	char msg[BUF_SIZE];
	unsigned int fsize;
	int offset, size;
	int blk_size = cpr_emmc_get_block_size();

	if(blk_size <= 0)
		blk_size = BLOCK_SIZE;

	buf = calloc(blk_size, sizeof(char));
	if(!buf)
		EMMC_FAILED;

	if(!get_dev(partition, dev)) {
		printf("[EMMC_API_LOG] invalid partition\n");
		free(buf);
		return EMMC_FAILED;
	}

	fd = fopen(dev, "w");
	if (fd == NULL) {
		printf("[EMMC_API_LOG] Can not open the device file\n");
		free(buf);
		return EMMC_FAILED;
	}

	if (fseek(fd, 0, SEEK_END) != 0) {
		strcpy(msg,"[EMMC_API_LOG] Failed to fseek to end of partition");
		error_handle(fd, buf, msg);
		return EMMC_FAILED;
	}

	fsize = ftell(fd);
	if(fseek(fd, 0, SEEK_SET) != 0) {
		strcpy(msg,"[EMMC_API_LOG] Failed to fseek to start of partition");
		error_handle(fd, buf, msg);
		return EMMC_FAILED;
	}

	if((start_block + block_num) > fsize/blk_size) {
		strcpy(msg,"[EMMC_API_LOG] Block number over limit for partition");
		error_handle(fd, buf, msg);
		return EMMC_FAILED;
	}


	offset = start_block*blk_size;
	fsize = block_num*blk_size;

	while(fsize > 0) {
		if(fsize > blk_size)
			size = blk_size;
		else
			size = fsize;

		if (fseek(fd, offset, SEEK_SET) != 0) {
			strcpy(msg,"[EMMC_API_LOG] Failed to set offset for partition");
			error_handle(fd, buf, msg);
			return EMMC_FAILED;
		}

		if (fwrite(buf, sizeof(char), size, fd) != size) {
			strcpy(msg,"[EMMC_API_LOG] Failed to write partition");
			error_handle(fd, buf, msg);
			return EMMC_FAILED;
		}

		fsize -= size;
		offset += size;
	}

	error_handle(fd, buf, NULL);
	return 0;
}

int cpr_emmc_read(const char* partition, unsigned int offset, void *buffer, unsigned int size) {
	long long len;
	char dev[DEV_LEN];
	FILE* fd;

	if(!get_dev(partition, dev)) {
		printf("[EMMC_API_LOG] invalid partition\n");
		return EMMC_FAILED;
	}

	fd = fopen(dev, "r");
	if(fd == NULL) {
		printf("[EMMC_API_LOG] Failed to open partition %s\n", partition);
		return EMMC_FAILED;
	}

	if (fseek(fd, 0, SEEK_END) != 0) {
		printf("[EMMC_API_LOG] Failed to fseek to end of partition %s\n", partition);
		fclose(fd);
		return EMMC_FAILED;
	}
	len = ftell(fd);

	if((offset + size) > len) {
		printf("[EMMC_API_LOG] reading partition over limit\n");
		fclose(fd);
		return EMMC_FAILED;

	}

	if (fseek(fd, offset, SEEK_SET) != 0) {
		printf("[EMMC_API_LOG] Failed to set offset for partition %s\n", partition);
		fclose(fd);
		return EMMC_FAILED;
	}

	if (fread(buffer,sizeof(char), size, fd) != size) {
		printf("[EMMC_API_LOG] Failed to read partition %s\n", partition);
		fclose(fd);
		return EMMC_FAILED;
	}

	printf("[EMMC_API_LOG] read %d bytes from %s partition at offset %d successfully!\n", size, partition, offset);

	fclose(fd);
	return 0;
}

int cpr_emmc_write(const char* partition,  unsigned int offset, void *buffer, unsigned int size) {
	unsigned long long len;
	char dev[DEV_LEN];
	FILE* fd;

	if(!get_dev(partition, dev)) {
		printf("[EMMC_API_LOG] invalid partition\n");
		return EMMC_FAILED;
	}

	fd = fopen(dev, "w");
	if(fd == NULL) {
		printf("[EMMC_API_LOG] Failed to open partition %s\n", partition);
		return EMMC_IO_ERR;
	}

	fseek(fd, 0, SEEK_END);
	len = ftell(fd);

	if((offset + size) > len) {
		printf("[EMMC_API_LOG] writing partition over limit\n");
		fclose(fd);
		return EMMC_IO_ERR;
	}

	if (fseek(fd, offset, SEEK_SET) != 0) {
		printf("[EMMC_API_LOG] Failed to set offset for partition %s\n", partition);
		fclose(fd);
		return EMMC_IO_ERR;
	}

	if (fwrite(buffer, sizeof(char), size, fd) != size) {
		printf("[EMMC_API_LOG] Failed to write partition %s\n", partition);
		fclose(fd);
		return EMMC_IO_ERR;
	}

	printf("[EMMC_API_LOG] write %d bytes to %s partition at offset %d successfully!\n", size, partition, offset);

	fclose(fd);
	return 0;
}
