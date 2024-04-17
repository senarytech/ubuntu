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
#ifndef _BOOTLOADER_HEADER_H
#define _BOOTLOADER_HEADER_H

#define DTB_SPACE (0x20000 - 0x100)
#define MAX_REGION_COUNT 16

#define MIN_ANDROID_IMG_SIZE (1 * 1024 * 1024)
#define MAX_ANDROID_IMG_SIZE (32 * 1024 * 1024)
#define MIN_VENDOR_BOOT_IMG_SIZE (10 * 1024)
#define MAX_VENDOR_BOOT_IMG_SIZE (32 * 1024 * 1024)

int init_gpt(void);

///bootflow
const char * get_ptname_bymode(int bootmode);
void get_reset_msg(void);
void set_kernel_entry(unsigned int addr);
void start_android_kernel(void);
int Image_3_Jump(void);

int Image_Load_And_Verify(void);

//image loading
int load_android_image(int bootmode);

//kernel parameters
void pre_init_parameters(void);
void setup_kernel_params(void);

//memory address in bootloader
void get_mem_region_list_from_tz(void);
void init_linux_addr(void);
unsigned int get_kernel_addr(void);
unsigned int get_dtb_addr(void);
unsigned int get_ramdisk_addr(void);
void * get_tz_memregion(void);
int get_tz_memregion_num(void);

unsigned int get_ion_cma_pool_addr(void);
unsigned int get_ion_cma_pool_size(void);
unsigned char * get_cmdline(void);
unsigned int get_ramdisk_buff(void);
unsigned int get_ramdisk_size(void);
unsigned int set_ramdisk_size(unsigned int new_size);
char * get_chip_kernel_param(void);
unsigned int get_bootconfig_start_addr(void);
unsigned int get_bootconfig_size(void);


void * malloc_ion_cacheable(int size);
void free_ion_cacheable(void * m);
void * malloc_ion_noncacheable(int size);
void free_ion_noncacheable(void * m);

#endif
