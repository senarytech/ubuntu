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

#define PARTITION_NUM 32
#define NAME_LEN 256
#define CMD_LEN 1024
#define DEV_LEN 1024
#define BUF_SIZE 1024
#define BLOCK_SIZE 4096

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

/**
 *  Get the mmcblk0px device which is mapped to partition
 * 	@param partition, input partition
 * 	@param mmcblk, the pointer to string for found mmcblk device
 *  @param size, thw pinter to save string size of mmcblk device
 *
 *  @Retuen true, successfuly found mmcblk device,
 *          false,  for error or not found
 *
 */
bool get_mmcblk0px(const char* partition, char *mmcblk, int *size);

/**
 *  Get the partition which is mapped to mmcblk0px device
 * 	@param mmcblk, input mmcblkopx device
 * 	@param partition, the pointer to string for found partition
 *  @param size, thw pinter to save string size of partition
 *
 *  @Retuen true, successfuly found partition,
 *          false,  for error or not found
 *
 */
bool get_partition(const char* mmcblk, char *partition, int *size);

/**
 *  Check if partion is valid
 * 	@param partition, the pointer to partition string
 *
 *  @Retuen true, the partition is found successfuly,
 *          false,  for error or not found
 *
 */
bool isValidPartition(const char* partition);

/**
 *  Get mmcblk0px device for partition
 *  @param partition, the pointer to partition string
 *  @param dev, the pointer to device name string
 *
 *  @Retuen true, the device is found successfuly,
 *          false,  for error or not found
 *
 */
bool get_dev(const char* partition, char* dev);


/**
 *  Get the size of one partition
 *  @param partition, the pointer to partition string
 *
 *  @Retuen the size of partition size
 *
 */
int getPartitionSize(const char* partition);

/**
 *  Get the count of partitions on device
 *
 *  @Retuen the size of partition size
 *
 */
int getPartitionCount();
