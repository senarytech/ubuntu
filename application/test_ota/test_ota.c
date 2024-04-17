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
#include "emmc_api.h"
#include "emmc_partition.h"

#define FILENAME_LEN 64
#define BLK_SIZE 1024*8

static char *usb = "/mnt/usb/sda1/OTA-Images/eMMCimg/";

static char images[PARTITION_NUM][FILENAME_LEN] = {
	"preboot.subimg.gz",
	"key_a.subimg.gz",
	"tzk_a.subimg.gz",
	"bl_a.subimg.gz",
	"boot_a.subimg.gz",
	"firmware_a.subimg.gz",
	"key_b.subimg.gz",
	"tzk_b.subimg.gz",
	"bl_b.subimg.gz",
	"boot_b.subimg.gz",
	"firmware_b.subimg.gz",
	"rootfs_a.subimg.gz",
	"rootfs_b.subimg.gz"
	"fastlogo_a.subimg.gz",
	"fastlogo_b.subimg.gz"
};

static void test_menu() {
	printf("\nTest menu\n");
	printf("-------------------------------------------\n");
	printf("(1) - Get seleted slot.\n");
	printf("(2) - Set seleted slot.\n");
	printf("(3) - Read a pritition.\n");
	printf("(4) - Write a pritition.\n");
	printf("(5) - Get block size.\n");
	printf("(6) - Reboot device.\n");
	printf("(7) - Erase partition.\n");
	printf("(8) - List all partitions.\n");
	printf("(0) - Exit test.\n");

}

int main (int argc, char *argv[]) {
	int i;
	char command[CMD_LEN];
	char buf[BLK_SIZE];
	char partition[NAME_LEN];
	char filename[FILENAME_LEN];


	if((argc >= 2 ) && (strcmp(argv[1],"--help") == 0 || strcmp(argv[1],"-?") == 0)) {
		printf("Test condition :\n\n");
		printf("1. Make test USB disk to include the eMMCimg package (USB/OTA-Images/eMMCimg)\n");
		printf("2. Insert USB disk to board, run ./test_ota, and follow up test menu to test\n");
		printf("3. Make sure OTA API library libota_api.so is included in LD_LIBRARY_PATH path\n\n");
		return 0;
	}

	while(1) {
		test_menu();
		printf( "Enter test option:");
		scanf("%d", &i);
		if(i == 0) {
			break;
		}

		switch(i) {
			case 1: {
				int slot;
				printf("Get selected slot.\n");
				slot = cpr_emmc_get_selected_slot();
				if(slot < 0) {
					printf("Failed to get selected slot.\n");
				} else {
					if(slot == 0) {
						printf("The selected slot is A.\n");
					}
					else {
						printf("The selected slot is B.\n");
					}
				}
			}
			break;
			case 2: {
				int slot;
				bool do_set = false;
				while(1) {
					printf("Input selected slot, 0 : Slot A, 1 : Slot B\n");
					scanf("%d", &slot);
					if(slot < 0) break;
					if(slot == 0 || slot == 1) {
						do_set = true;
						break;
					}
				}
				if(do_set == false) break;

				if(cpr_emmc_select_slot(slot) < 0) {
					printf("Failed to set selected slot.\n");
				} else {
					printf("Set slot %c successfully.\n", (slot ? 0 : 'A','B'));
				}

			}
			break;
			case 3: {
				char c;
				while(1) {
					printf("Please input partition name\n");
					scanf("%s", partition);
					printf("You input partition is %s\n", partition);
					if(isValidPartition(partition))
						break;
					printf("Invalid partition name\n");
				}

				printf("Reading partion now ...\n");

				if(cpr_emmc_read(partition, 0, buf, sizeof(buf)) != 0) {
					printf("Failed to read partition\n");
				} else {
					printf("Read %s partition successfully\n", partition);
				}

				printf("Press ENTER key to Continue\n");
				scanf("%c%*c", &c);

			}
			break;
			case 4: {
				int i;
				char c;
				char *p;
				FILE *fp;
				long long fsize;
				int blk_num;
				int size;
				int read_size;
				long long offset;

				while(1) {
					printf("Please input partition name\n");
					scanf("%s", partition);
					printf("You input partition is %s\n", partition);
					if(isValidPartition(partition))
						break;
					printf("Invalid partition name\n");
				}

				for(i = 0; i < PARTITION_NUM; i++) {
					p = strstr(images[i], partition);
					if(p != NULL) {
						break;
					}
				}

				if(!p) {
					printf("No image file for partition %s\n", partition);
					break;
				}

				strcpy(filename, usb);
				strncat(filename, images[i], strlen(images[i]));

				printf("File name %s\n", filename);

				if(strstr(filename,".gz") == NULL) {

				// Raw file format

					fp = fopen(filename, "r");
					if(fp == NULL) {
						printf("Failed to open file %s\n", filename);
						break;
					}

					fseek(fp, 0, SEEK_END);
					fsize = ftell(fp);
					fseek(fp, 0, SEEK_SET);

					blk_num = fsize/BLK_SIZE;
					if(fsize%BLK_SIZE) blk_num++;

					printf("File size is %lld, block number is %d\n", fsize, blk_num);

					for(i = 0; i < blk_num; i++) {
						size = BLK_SIZE;
						offset = i * BLK_SIZE;
						if(i == (blk_num-1)) {
							size = fsize - i*BLK_SIZE;
						}

						if(fseek(fp, offset, SEEK_SET) != 0) {
							printf("Failed to set offset to %ls\n", offset);
						}

						read_size = fread(buf, 1, size, fp);
						if(read_size != size) {
							printf("Reading file failure at offset %lld\n", offset);
							break;
						}
						if(cpr_emmc_write(partition, offset, buf, size) != 0) {
							printf("Failed to write block at offset %lld to partition %s\n", offset, partition);
							break;
						}

					}

					fclose(fp);

				} else {

				// compressed format

					strcpy(command, "gunzip -c ");
					strncat(command, filename, strlen(filename));

					fp = popen(command, "r");
					if (fp == NULL) {
						printf("Failed to run command %s\n", command);
						return -1;
					}

					offset = 0;
					while (1) {
						fseek(fp, offset, SEEK_SET);
						size = fread(buf, 1, BLK_SIZE, fp);
						if(size > 0) {
							if(cpr_emmc_write(partition, offset, buf, size) != 0) {
								printf("Failed to write block at offset %lld to partition %s\n", offset, partition);
								break;
							}
						}
						if(size != BLK_SIZE)
							break;
						offset += BLK_SIZE;
					}

					pclose(fp);
				}

				printf("Press ENTER key to Continue\n");
				scanf("%c%*c", &c);
			}
			break;
			case 5: {
				int blk_size = cpr_emmc_get_block_size();
				printf("The block size = %d\n", blk_size);
			}
			break;
			case 6: {
				system("reboot");
			}
			break;
			case 7: {
				while(1) {
					printf("Please input partition name\n");
					scanf("%s", partition);
					printf("You input partition is %s\n", partition);
					if(isValidPartition(partition))
						break;
					printf("Invalid partition name\n");
				}

				cpr_emmc_erase_partition(partition);
			}
			break;
			case 8: {
				printf("List all partitions\n");
				partition_list_t *list = cpr_emmc_get_partition_list();
				if(!list) break;
				for(int i = 0; i < list->count; i++) {
					printf("name = %s\n", list->partitions[i].name);
					printf("size = %d\n", list->partitions[i].size);
				}
			}
			break;
		}
	}

	return 0;
}
