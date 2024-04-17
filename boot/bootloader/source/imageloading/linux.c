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
#if defined(LINUXSDK_BOOTLOADER)

#include "com_type.h"
#include "io.h"
#include "string.h"
#include "debug.h"

#include "gpt.h"
#include "load_gpt.h"
#include "load_gih.h"

#include "flash_adaptor.h"

#include "image_info.h"

#include "codetype.h"
#include "tz_nw_boot.h"
#include "lz4dec.h"

#include "../android/bootimg.h"

#include "bootloader_header.h"

boot_img_hdr Mkbootimg_hdr;

extern int VerifyImage(const void *src, unsigned int size, void *dst, unsigned int codetype);
extern long long read_image_by_ptname(const char *module_name,unsigned int image_size,unsigned char *image_buff);

static inline unsigned get_aligned(unsigned address, unsigned page_size) {
	return (address + page_size - 1) / page_size * page_size;
}

static unsigned int bootimg_size = 0;
static int get_boot_image_size(const char *ptname, unsigned char * buff)
{
	int ret;
	ret = find_partition(ptname);
	if(ret < 0) {
		dbg_printf(PRN_ERR, "system critical error: can't find kernel image pt info!\n");
		return -1;
	}

	if(bootimg_size == 0) {
		long long ret = 0;
		struct image_info *img_info;

		/* Read boot image header to determing length of the image */
		ret = read_image_by_ptname(ptname, get_page_size(), buff);
		if(ret < 0){
			dbg_printf(PRN_ERR, "Failed to read boot image header!\n");
			return -1;
		}

		/* check image information */
		img_info = (struct image_info *)buff;
		if(img_info->magic != IMAGE_INFO_MAGIC)
		{
			dbg_printf(PRN_ERR, "ERROR: ptname %s: incorrect magic in image info  0x%08x\n", ptname, img_info->magic);
			return -1;
		}
		if(img_info->version != IMAGE_INFO_VERSION_1_0_0_0)
		{
			dbg_printf(PRN_ERR, "ERROR: unsupported version of image info\n");
			return -1;
		}

		/* find out boot image size */
		bootimg_size = img_info->image_size;
		bootimg_size = get_aligned(bootimg_size, 16);
		bootimg_size += img_info->image_offset;
	}

	dbg_printf(PRN_INFO, "bootimg_size = 0x%x \n", bootimg_size);
	return bootimg_size;
}

int get_boot_image_offset(unsigned char * buff)
{
	struct image_info *img_info = (struct image_info *)buff;

	return img_info->image_offset;
}

static int bootimg_print_image_hdr (boot_img_hdr *hdr)
{
	int i;
	UNUSED(hdr);
	dbg_printf(PRN_INFO, "   Image magic:   %s\n", hdr->magic);

	dbg_printf(PRN_INFO, "   kernel_size:   0x%x\n", hdr->kernel_size);
	dbg_printf(PRN_INFO, "   kernel_addr:   0x%x\n", hdr->kernel_addr);

	dbg_printf(PRN_INFO, "   rdisk_size:   0x%x\n", hdr->ramdisk_size);
	dbg_printf(PRN_INFO, "   rdisk_addr:   0x%x\n", hdr->ramdisk_addr);

	dbg_printf(PRN_INFO, "   second_size:   0x%x\n", hdr->second_size);
	dbg_printf(PRN_INFO, "   second_addr:   0x%x\n", hdr->second_addr);

	dbg_printf(PRN_INFO, "   tags_addr:   0x%x\n", hdr->tags_addr);
	dbg_printf(PRN_INFO, "   page_size:   0x%x\n", hdr->page_size);

	dbg_printf(PRN_INFO, "   name:      %s\n", hdr->name);
	dbg_printf(PRN_INFO, "   cmdline:   %s\n", hdr->cmdline);

	for (i=0;i<8;i++)
		dbg_printf(PRN_INFO, "   id[%d]:   0x%x\n", i, hdr->id[i]);

	return 0;
}

unsigned char * get_cmdline(void)
{
	return Mkbootimg_hdr.cmdline;
}

unsigned int get_ramdisk_buff(void)
{
	return Mkbootimg_hdr.ramdisk_addr;
}

unsigned int get_ramdisk_size(void)
{
	return Mkbootimg_hdr.ramdisk_size;
}

long long verify_boot_image(unsigned char * src, unsigned char * dest, unsigned int size)
{
	return VerifyImage(src, size, dest, CODETYPE_KERNEL);
}

#ifndef CONFIG_GENX_ENABLE
#ifndef ADD_CUSTK_STORE
static int load_oem_keystore(const unsigned long keystore_addr)
{
	int i = 0, num = 0;
	//uint8_t * oemkey = (unsigned char *)(uintptr_t)get_aligned((uintptr_t)keystore_buff, 64);
	unsigned char *oemkey = (unsigned char *)(uintptr_t)keystore_addr;
#ifndef CONFIG_AB_BOOTFLOW
	#define KEY_1_NAME KEY1_NAME
	#define KEY_2_NAME KEY2_NAME
#else
	#define KEY_1_NAME KEY_A_NAME
	#define KEY_2_NAME KEY_B_NAME
#endif

	long long ret = 0;
	num = 2;
	for(i = 1; i <= num; i++) {
		if(i == 1)
			ret = read_image_by_ptname(KEY_1_NAME, CUSTKEY_SIZE, oemkey);
		else
			ret = read_image_by_ptname(KEY_2_NAME, CUSTKEY_SIZE, oemkey);

		if(ret != 0)
			continue;

		return 0;
	}

	return -1;
}
#endif
#endif

int load_android_image(int bootmode)
{
	int result = 0;
	long long ret;
	unsigned mkbootimg_page;
	//alloc 32M + 1M memory for key,  free or not free doesn't matter
	unsigned char * k_buff = malloc_ion_cacheable(MAX_ANDROID_IMG_SIZE + 0x100000);
	unsigned char * k_buff_img = NULL;
	unsigned int k_buff_size = 0;
	unsigned int k_buff_offset = 0;
	const char* ptname;

	int cpu_img_size = 0;

	memset(k_buff, 0, MAX_ANDROID_IMG_SIZE);
	k_buff_img = k_buff + CUSTKEY_SIZE;

	ptname = get_ptname_bymode(bootmode);
	dbg_printf(PRN_INFO, "DEBUG: Start to get Boot size !\n");
	cpu_img_size = get_boot_image_size(ptname, k_buff);
	if(cpu_img_size == -1) {
		ERR("ERROR: ptname %s, wrong boot image header\n", ptname);
		return -1;
	}
	dbg_printf(PRN_INFO, "DEBUG: Kernel Original Boot size 0x%x !\n", cpu_img_size);

	/* Add min size check to be safe */
	if (cpu_img_size > MAX_ANDROID_IMG_SIZE || cpu_img_size < MIN_ANDROID_IMG_SIZE) {
		dbg_printf(PRN_ERR, "ERROR: aligned CPU img_size %u is invalid\n", cpu_img_size);
		return -1;
	}

	ret = read_image_by_ptname(ptname, cpu_img_size, k_buff_img);
	if (ret < 0){
		dbg_printf(PRN_ERR, "ERROR: Failed to read CPU image ret %d\n", ret);
		return -1;
	}

	/* adjust point to image payload (skip prepended image info portion) */
	k_buff_offset = get_boot_image_offset(k_buff_img); // offset
	k_buff_img += k_buff_offset;
#ifndef CONFIG_GENX_ENABLE
	unsigned char * k_buff_oemkey = NULL;
#ifndef ADD_CUSTK_STORE
	// load oem custkey just before 5KB of the signged image
	k_buff_oemkey = k_buff_img - CUSTKEY_SIZE;
	ret = load_oem_keystore((uintptr_t)k_buff_oemkey);
	if(ret) {
		dbg_printf(PRN_ERR, "system critical error: can't load oem custkey");
		return -1;

	}
	dbg_printf(PRN_INFO, "DEBUG: Kernel Original Image Offset 0x%x !\n", k_buff_size);

	//FIXME: change to common function because of verified boot
	k_buff_size = cpu_img_size + CUSTKEY_SIZE - k_buff_offset;
#else
	k_buff_oemkey = k_buff_img;
	k_buff_size = cpu_img_size - k_buff_offset;
	dbg_printf(PRN_INFO, "DEBUG: Kernel Image Real Size 0x%x !\n", k_buff_size);
#endif

#ifndef CONFIG_GEN_SKIP
 	ret = verify_boot_image(k_buff_oemkey, k_buff_oemkey, k_buff_size);
	if (ret) {
		dbg_printf(PRN_ERR, "ERROR: Verify k_buff image failed!ret=0x%x\n", ret);
		return -1;
	}

	k_buff_img = k_buff_oemkey;
#endif

#else //CONFIG_GENX_ENABLE
	k_buff_size = cpu_img_size - k_buff_offset;
	ret = VerifyImage((void *)k_buff_img, k_buff_size, (void *)k_buff_img, IMAGE_TYPE_LINUX_KERNEL);
	if (ret) {
		dbg_printf(PRN_ERR, "ERROR: Verify Linux Kernel image failed! ret=0x%x\n", ret);
		return -1;
	}
	k_buff_img += GENX_IMAGE_HEADER_LINUX_SIZE;
#endif
	memcpy(&Mkbootimg_hdr, k_buff_img, sizeof(Mkbootimg_hdr));

	if (strncmp((const char *)Mkbootimg_hdr.magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)) {
		dbg_printf(PRN_ERR, "ERROR: Invalid mkbootimg header!\n");
		return -1;
	}

	/* force to use predefined kernel addr (CPU0_START_ADDR) in config.
	 * We don't use the kernel_addr from kernel partition for scurity
	 * consideration.
	 */
	Mkbootimg_hdr.kernel_addr = get_kernel_addr();

	mkbootimg_page = Mkbootimg_hdr.page_size;
	dbg_printf(PRN_INFO, "DEBUG: mkbootimg_page = %d !\n", mkbootimg_page);

	// align by page_size and add one page for skipping header.
	k_buff_size = mkbootimg_page +
		get_aligned(Mkbootimg_hdr.kernel_size, mkbootimg_page) +
		get_aligned(Mkbootimg_hdr.ramdisk_size, mkbootimg_page) +
		get_aligned(Mkbootimg_hdr.second_size, mkbootimg_page);

	/* Check bootloader has enough memory to be able to
	 * read the firmware image for verification.
	 */
	if (k_buff_size > MAX_ANDROID_IMG_SIZE){
		dbg_printf(PRN_ERR, "BOOTLOADER: Firmware image is too big %d for "
					 "verification\n", k_buff_size);
		return -1;
	}

	struct image_header * gih = (struct image_header *)(k_buff_img + mkbootimg_page);

	result = find_chunk(IMAGE_CHUNK_ID_LINUX_BOOTIMG, gih);

	if(result < 0) {
		dbg_printf(PRN_ERR, "didn't find kernel image!!!\n");
		return -1;
	}

	//kernel
	//FIXME: if it's flate image...?
#ifdef CONFIG_LZ4_BOOT
	unsigned int decompressed_size = MAX_ANDROID_IMG_SIZE; //set default decompressed_size to maxium buffer size.
	dbg_printf(PRN_INFO, "decompressing kernel image size %d...\n", gih->chunk[result].size);
	ret = lz4_decompress((const unsigned char *)(k_buff_img + mkbootimg_page + gih->chunk[result].offset),
			(const unsigned int)( gih->chunk[result].size),
			(unsigned char *)((uintmax_t)Mkbootimg_hdr.kernel_addr),
			&decompressed_size);
	if (ret != 0){
		dbg_printf(PRN_ERR, "kernel image decompress failed !\n");
		return -1;
	}
	dbg_printf(PRN_INFO, "kernel img addr 0x%x, img len %d.\n", Mkbootimg_hdr.kernel_addr, decompressed_size);
#else
	//memcpy((void *)((uintptr_t)(Mkbootimg_hdr.kernel_addr)), k_buff_img + mkbootimg_page + gih->chunk[result].offset, Mkbootimg_hdr.kernel_size);
	memcpy((void *)((uintptr_t)(Mkbootimg_hdr.kernel_addr)), k_buff_img + mkbootimg_page + gih->chunk[result].offset, (gih->chunk[result].size));
#endif

	/* force to use predefined kernel addr (CPU0_START_ADDR) in config.
	 * We don't use the kernel_addr from kernel partition for scurity
	 * consideration.
	 */
	//strcpy((char *)Mkbootimg_hdr.cmdline, (bootmode == BOOTMODE_RECOVERY) ?
	//		LINUX_PARAM_RECOVERY_BOOT_PART : LINUX_PARAM_NORMAL_BOOT_PART);
	set_kernel_entry(Mkbootimg_hdr.kernel_addr);

	dbg_printf(PRN_INFO, "DEBUG: LOAD DTB parameter !\n");
	//dtb
	result = find_chunk(IMAGE_CHUNK_ID_LINUX_DTB, gih);
	if(result < 0) {
		dbg_printf(PRN_ERR, "didn't find dtb!!!\n");
		return -1;
	}
	memcpy(((void *)(uintptr_t)get_dtb_addr()), \
		(k_buff_img + mkbootimg_page + gih->chunk[result].offset), gih->chunk[result].size);

	//ramdisk
	Mkbootimg_hdr.ramdisk_addr = get_ramdisk_addr();
	dbg_printf(PRN_INFO, "DEBUG: LOAD Ramdisk to address 0x%08x, with size 0x%08x; while DTB address = 0x%08x!\n", \
		Mkbootimg_hdr.ramdisk_addr, Mkbootimg_hdr.ramdisk_size, get_dtb_addr());

	if (Mkbootimg_hdr.ramdisk_size) {
		/* we also don't use the ramdisk_addr read from flash for security reason */
		//Mkbootimg_hdr.ramdisk_addr = (Mkbootimg_hdr.kernel_addr - LINUX_OFFSET + RAMDISK_OFFSET);
		k_buff_offset = get_aligned(Mkbootimg_hdr.kernel_size + mkbootimg_page, mkbootimg_page);
		memcpy((void *)((uintptr_t)(Mkbootimg_hdr.ramdisk_addr)), (k_buff_img + k_buff_offset), Mkbootimg_hdr.ramdisk_size);
	}
	bootimg_print_image_hdr(&Mkbootimg_hdr);

	return 0;
}

#endif
