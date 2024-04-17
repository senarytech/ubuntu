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

/* enum & struct */
#define NAME_SIZE 256

typedef enum {
	EMMC_SLOT_A = 0,
	EMMC_SLOT_B = 1,
} emmc_slot_t;

// TBD: we can use standard errno
typedef enum {
	EMMC_FAILED = -1,     // common error code
	EMMC_IO_ERR = -2,     // IO error
	// ...    // please add more definitions
} emmc_error_t;

typedef struct {
    char name[NAME_SIZE];
    unsigned int size;
} partition_info_t;

typedef struct {
    unsigned int count;
    partition_info_t *partitions;
} partition_list_t;



/* AB slot API */

/* Name: cpr_emmc_get_selected_slot
 * Desc: get the A/B slot that is selected to boot
 * Return: emmc_slot_t or emmc_error_t
 */
int cpr_emmc_get_selected_slot(void);

/* Name: cpr_emmc_get_running_slot
 * Desc: get the A/B slot that is running. Note that the return value may be
 * 	different with cpr_emmc_get_selected_slot(), because there will be a way
 * 	to force boot from the slot that is not be selected.
 * Return: emmc_slot_t or emmc_error_t
 */
int cpr_emmc_get_running_slot(void);

/* Name: cpr_emmc_select_slot
 * Desc: change the selected slot
 * Return: 0 for success, emmc_error_t for failure
 */
int cpr_emmc_select_slot(emmc_slot_t part);



/* Flash API */

/* Name: cpr_emmc_get_block_size
 * Desc: get the block size of the flash
 * Return: block size in bytes
 */
unsigned int cpr_emmc_get_block_size(void);

/* Name: cpr_emmc_get_partition_list
 * Desc: Get the information of all partitions
 * Return: a pointer to partition_list_t. The memory should be unique and maintained
 *  by cpr_emmc lib. That means, the upper app don't need to free the memory, and
 *	it will not cause memory leak if this API is repeatedly called.
 *	Note that besides the GPT partitions, "preboot1" and "preboot2" (b1/b2) should
 *	also be included.
 */
partition_list_t* cpr_emmc_get_partition_list(void);

/* Name: cpr_emmc_erase_partition
 * Desc: erase the specified partition
 * Param:
 *    partition: the specified partition name, can be a GPT partition name,
 *				 or "preboot1" "preboot2"
 * Return: 0 for success, emmc_error_t for failure
 */
int cpr_emmc_erase_partition(const char* partition);

/* Name: cpr_emmc_erase_blocks
 * Desc: erase the specified blocks
 * Params:
 *    partition: partition name, can be a GPT partition name, or "preboot1" "preboot2"
 *    start_block: the first block to be erased
 *    block_num: how many blocks to be erased
 * Return: 0 for success, emmc_error_t for failure
 */
int cpr_emmc_erase_blocks(const char* partition, unsigned int start_block, unsigned int block_num);

/* Name: cpr_emmc_read
 * Desc: read data from emmc partition
 * Params:
 *    partition: partition name, can be a GPT partition name, or "preboot1" "preboot2"
 *    offset: the offset to be read
 *    buffer: the buf to store the data
 *    size: how many bytes to be read
 * Return: how many bytes are read successfully. emmc_error_t for failure.
 */
int cpr_emmc_read(const char* partition, unsigned int offset, void *buffer, unsigned int size);

/* Name: cpr_emmc_write
 * Desc: write data to emmc partition
 * Params:
 *    partition: partition name, can be a GPT partition name, or "preboot1" "preboot2"
 *    offset: the offset to be written
 *    buffer: the data to be written
 *    size: how many bytes to be written
 * Return: how many bytes are written successfully. emmc_error_t for failure.
 */
int cpr_emmc_write(const char* partition, unsigned int offset, void *buffer, unsigned int size);
