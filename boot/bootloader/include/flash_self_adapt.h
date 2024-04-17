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
#ifndef __FLASH_SELF_ADAPT__
#define __FLASH_SELF_ADAPT__

#include "version_table.h"
#include "bootloader_header.h"

#define FTS_NAME        "fts"
#define KERNEL_NAME     "boot"
#define RECOVERY_NAME   "recovery"

void init_flash(int block_size, int page_size, int addr_cycle);
int get_version_table(int part);
void set_flash_ts_param(char *param_buf);
long long read_image_header(ver_table_entry_t *vt,unsigned int header_size,char *header_buff,unsigned char* const aligned_buff);
long long read_image(ver_table_entry_t *vt,unsigned int image_size,unsigned char *image_buff);
long long read_image_from_offset(ver_table_entry_t *vt, unsigned int pt_offset, unsigned int size, unsigned char *image_buf);
int read_uboot_param(char *env_buff);

#if !ANDROID_BOOT
long long get_next_img_addr(unsigned int start, unsigned int end, unsigned int data_size);
#endif

int find_version_table(const void *name);
unsigned int fetch_version_table(int num, ver_table_entry_t *vt);
unsigned int get_version_table_entry_number();

#endif
