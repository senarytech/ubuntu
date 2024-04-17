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
#if defined(CONFIG_ANDROID_BOOTFLOW)

#include "com_type.h"
#include "io.h"
#include "string.h"
#include "debug.h"
#include "image_chunk.h"
#include "load_gpt.h"
#include "load_gih.h"
#include "../android/bootimg.h"
#include "../android/android_utils.h"
#include "codetype.h"
#include "vb_state.h"
#include "lz4dec.h"
#include "bootloader_header.h"
#include "ptinfo.h"
#include "mmgr.h"

#ifdef CONFIG_NR_BOOTFLOW
#include "libfdt_env.h"
#include "dtbo.h"
#endif

#ifdef CONFIG_GENX_ENABLE
#include "image_info.h"
#endif
#include "libxbc.h"

unsigned header_version;

#define MAX_DTBO_SIZE (0x10000)   //64KB

#if defined(CONFIG_PRELOADTA)
extern unsigned gOs_version, gOs_lvl;
extern uint64_t gBoot_lvl, gSystem_lvl, gVendor_lvl;
extern bool allow_verification_error;
extern void set_public_key(const char * buf, int size);
#endif

enum {
	ANDROID_OEM_KEY = 1,
	ANDROID_USR_KEY = 2
};

typedef struct {
	unsigned char * boot_buff;
	unsigned int boot_size;
	unsigned char * androidkey_buff;
	unsigned char * dtbo_buff;
	unsigned int dtbo_size;
	unsigned char * vendor_boot_buff;
	unsigned int vendor_boot_size;
	unsigned char * cmdline;
	unsigned int bootconfig_size;
	unsigned int bootconfig_start_addr;
	unsigned int ramdisk_addr;
	unsigned int ramdisk_size;
	unsigned char * init_buff;
	unsigned int init_size;
}avb_predata;

static avb_predata predata;
static char * avb_kernel_commandline = NULL;
static rpmb_avb_t stored_rpmb_avb_info = { 0 };
static rpmb_avb_t current_rpmb_avb_info = { 0 };

static inline unsigned get_aligned(unsigned address, unsigned page_size) {
	return (address + page_size - 1) / page_size * page_size;
}

extern int VerifyImage(const void *src, unsigned int size, void *dst, unsigned int codetype);
//FIXME: need a header file?

static uint8_t * load_android_keystore(unsigned int id, unsigned char * keystore_buff)
{
	uint8_t * oemkey = (unsigned char *)(uintptr_t)get_aligned((uintptr_t)keystore_buff, 64);
	//load it just after oem cust key and oem ext rsa key
	uint8_t * keystore = (oemkey + CUSTKEY_SIZE);
	//bg5ct and future chip
	struct gpt_ent gpte_key;
	int index = -1;
	long long ret = 0;

	index = get_pt_index(PT_KEY);

	if(index < 0)
	{
		dbg_printf(PRN_ERR,"didn't find key partition %s!!!\n", get_pt_name(PT_KEY));
		return NULL;
	}
	fetch_partition_info(index, &gpte_key);
	ret = read_image(&gpte_key, CUSTKEY_SIZE, oemkey);
	if(ret != 0)
		return NULL;
	ret = read_image_from_offset(&gpte_key, (id * CHUNK_SIZE), CHUNK_SIZE, keystore);
	if(ret != 0)
		return NULL;

	//verify android key store
#ifndef CONFIG_GENX_ENABLE
	ret = VerifyImage((void *)oemkey, (CUSTKEY_SIZE + CHUNK_SIZE), (void *)keystore, CODETYPE_KEYSTORE);
#else
	struct genx_image_info *genx_img_info = (struct genx_image_info *)keystore;
	ret = VerifyImage((void *)keystore, genx_img_info->image_size, (void *)keystore, IMAGE_TYPE_AVB_KEYS);
#endif
	if(ret)
	{
		dbg_printf(PRN_ERR, "keystore verify fail in key partition %s. result = %d\n", get_pt_name(PT_KEY), ret);
		return NULL;
	}
#ifndef CONFIG_GENX_ENABLE
	return keystore;
#else
	//Skip GenX format image header
	return (keystore + GENX_IMAGE_HEADER_AVB_KEYS_SIZE);
#endif
}


static int print_bootimg_image_hdr_v3(struct boot_img_hdr_v3 *hdr)
{
	INFO("   Image magic:   %s\n", hdr->magic);
	INFO("   kernel_size:   0x%x\n", hdr->kernel_size);
	INFO("   ramdisk_size:  0x%x\n", hdr->ramdisk_size);
	INFO("   header_size:   0x%x\n", hdr->header_size);
	INFO("   header_version:   0x%x\n", hdr->header_version);
	INFO("   cmdline:   %s\n", hdr->cmdline);

	return 0;
}

static int print_bootimg_image_hdr_v2 (boot_img_hdr *hdr)
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

static int print_vendor_bootimg_image_hdr (struct vendor_boot_img_hdr_v3 *hdr)
{
	INFO("   Vendor Image magic:    %s\n", hdr->magic);
	INFO("   Vendor header_version: 0x%x\n", hdr->header_version);
	INFO("   Vendor page_size:      0x%x\n", hdr->page_size);
	INFO("   Vendor ramdisk_size:   0x%x\n", hdr->vendor_ramdisk_size);
	INFO("   Vendor header_size:    0x%x\n", hdr->header_size);
	INFO("   Vendor dtb_size:       0x%x\n", hdr->dtb_size);
	INFO("   Vendor name:           %s\n", hdr->name);
	INFO("   Vendor cmdline:        %s\n\n", hdr->cmdline);

	return 0;
}

static int get_vendor_boot_image_size(struct vendor_boot_img_hdr_v3 * hdr)
{
	unsigned int image_size = 0;
	unsigned int page_size;
	long long ret = 0;
	unsigned char * const temp_buff = malloc_ion_cacheable(0x4000);
#ifdef CONFIG_AB_BOOTFLOW
	void *pt_name = (get_abmode() == BOOTSEL_A) ? VENDOR_BOOT_A_NAME : VENDOR_BOOT_B_NAME;
#else
	void *pt_name = VENDOR_BOOT_NAME;
#endif

	/* Read boot image header to determing length of the image */
	ret = read_image_by_ptname(pt_name,	0x4000, temp_buff);
	if(ret < 0){
		dbg_printf(PRN_ERR, "Failed to read boot image header!\n");
		return -1;
	}

	memcpy((unsigned char*)hdr, temp_buff, sizeof(struct vendor_boot_img_hdr_v3));

	if (strncmp((const char *)hdr->magic, VENDOR_BOOT_MAGIC, VENDOR_BOOT_MAGIC_SIZE)) {
		ERR("ERROR: Invalid vendor boot header!\n");
		return -1;
	}

	if (hdr->header_version >= BOOTIMG_HEADER_VERSION_MAX) {
		ERR("ERROR: Invalid vendor boot image header version %d !\n", hdr->header_version);
		return -1;
	}

	page_size = hdr->page_size;

	// align by page_size
	image_size += get_aligned(hdr->header_size, page_size);
	image_size += get_aligned(hdr->vendor_ramdisk_size, page_size);
	image_size += get_aligned(hdr->dtb_size, page_size);
	print_vendor_bootimg_image_hdr(hdr);

	/* Add min size check to be safe */
	if (image_size > MAX_VENDOR_BOOT_IMG_SIZE || image_size < MIN_VENDOR_BOOT_IMG_SIZE) {
		ERR("ERROR: aligned vendor boot img_size %u is invalid\n", image_size);
		//return -1;
	}

	return image_size;
}

__attribute__((unused)) static int get_vendor_boot_img_v4_size(struct vendor_boot_img_hdr_v4 * hdr_v4)
{
	UINT image_size = 0;
	unsigned char * const temp_buff = malloc_ion_cacheable(VENDOR_BOOT_IMAGE_V4_PAGE_SIZE);
#ifdef CONFIG_AB_BOOTFLOW
	void *pt_name = (get_abmode() == BOOTSEL_A) ? VENDOR_BOOT_A_NAME : VENDOR_BOOT_B_NAME;
#else
	void *pt_name = VENDOR_BOOT_NAME;
#endif

	/* Read boot image header to determing length of the image */
	if(read_image_by_ptname(pt_name, VENDOR_BOOT_IMAGE_V4_PAGE_SIZE, temp_buff) < 0){
		ERR("Failed to read boot image header!\n");
		free_ion_cacheable(temp_buff);
		return -1;
	}

	memcpy((unsigned char*)hdr_v4, temp_buff, sizeof(struct vendor_boot_img_hdr_v4));

	if (strncmp((const char *)hdr_v4->magic, VENDOR_BOOT_MAGIC, VENDOR_BOOT_MAGIC_SIZE)) {
		ERR("ERROR: Invalid vendor boot header!\n");
		free_ion_cacheable(temp_buff);
		return -1;
	}

	if (hdr_v4->header_version >= BOOTIMG_HEADER_VERSION_MAX) {
		ERR("ERROR: Invalid vendor boot image header version %d !\n", hdr_v4->header_version);
		return -1;
	}

	// align by page_size

	image_size = get_aligned(hdr_v4->header_size, hdr_v4->page_size) +
                     get_aligned(hdr_v4->vendor_ramdisk_size, hdr_v4->page_size) +
                     get_aligned(hdr_v4->dtb_size, hdr_v4->page_size) +
                     get_aligned(hdr_v4->vendor_ramdisk_table_size, hdr_v4->page_size) +
                     get_aligned(hdr_v4->bootconfig_size, hdr_v4->page_size);

	/* Add min size check to be safe */
	if (image_size > MAX_VENDOR_BOOT_IMG_SIZE || image_size < MIN_VENDOR_BOOT_IMG_SIZE) {
		ERR("ERROR: aligned vendor boot img_size %u is invalid\n", image_size);
		free_ion_cacheable(temp_buff);
		//return -1;
	}

	free_ion_cacheable(temp_buff);

	return image_size;
}

__attribute__((unused)) static int get_boot_image_size_v4(struct boot_img_hdr_v4 *hdr)
{
	UINT32 image_size = 0;
	INT64  ret = 0;
	UCHAR * const temp_buff = malloc_ion_cacheable(BOOT_IMAGE_V4_PAGE_SIZE);
	void *pt_name = NULL;

#ifdef CONFIG_AB_BOOTFLOW
	pt_name = (get_abmode() == BOOTSEL_A) ?  KERNEL_A_NAME : KERNEL_B_NAME;
#else
	pt_name = (get_bootmode() == BOOTMODE_NORMAL) ? KERNEL_NAME : RECOVERY_NAME;
#endif

	/* Read boot image header to determing length of the image */
	ret = read_image_by_ptname(pt_name, BOOT_IMAGE_V4_PAGE_SIZE, temp_buff);
	if(ret < 0){
		ERR("Failed to read boot image header!\n");
		return -1;
	}

	memcpy((UCHAR*)hdr, temp_buff, sizeof(struct boot_img_hdr_v4));

	if (strncmp((const char *)hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)) {
		ERR("ERROR: Invalid boot header!\n");
		return -1;
	}

	// align by page_size and add one page for skipping header.
	image_size = get_aligned(hdr->header_size, BOOT_IMAGE_V4_PAGE_SIZE) +
                     get_aligned(hdr->kernel_size, BOOT_IMAGE_V4_PAGE_SIZE) +
                     get_aligned(hdr->ramdisk_size, BOOT_IMAGE_V4_PAGE_SIZE) +
                     get_aligned(hdr->signature_size, BOOT_IMAGE_V4_PAGE_SIZE);

	/* Add min size check to be safe */
	if (image_size > MAX_ANDROID_IMG_SIZE || image_size < MIN_ANDROID_IMG_SIZE) {
		ERR("ERROR: aligned boot img_size %u is invalid\n", image_size);
		return -1;
	}

	return image_size;
}

static int get_boot_image_size_v3(struct boot_img_hdr_v3 *hdr)
{
	unsigned int image_size = 0;
	unsigned int page_size = BOOT_IMAGE_V3_PAGE_SIZE;
	long long ret = 0;
	unsigned char * const temp_buff = malloc_ion_cacheable(0x4000);
	void *pt_name = NULL;

#ifdef CONFIG_AB_BOOTFLOW
	pt_name = (get_abmode() == BOOTSEL_A) ?  KERNEL_A_NAME : KERNEL_B_NAME;
#else
	pt_name = (get_bootmode() == BOOTMODE_NORMAL) ? KERNEL_NAME : RECOVERY_NAME;
#endif

	/* Read boot image header to determing length of the image */
	ret = read_image_by_ptname(pt_name, 0x4000, temp_buff);
	if(ret < 0){
		ERR("Failed to read boot image header!\n");
		return -1;
	}

	memcpy((unsigned char*)hdr, temp_buff, sizeof(struct boot_img_hdr_v3));

	if (strncmp((const char *)hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)) {
		ERR("ERROR: Invalid boot header!\n");
		return -1;
	}

	// align by page_size and add one page for skipping header.
	image_size += page_size;
	image_size += get_aligned(hdr->kernel_size, page_size);
	image_size += get_aligned(hdr->ramdisk_size, page_size);

	/* Add min size check to be safe */
	if (image_size > MAX_ANDROID_IMG_SIZE || image_size < MIN_ANDROID_IMG_SIZE) {
		ERR("ERROR: aligned boot img_size %u is invalid\n", image_size);
		return -1;
	}

	print_bootimg_image_hdr_v3(hdr);

	return image_size;
}

static int get_boot_image_size_v2(struct gpt_ent * gpte, unsigned char * buff)
{
	unsigned int bootimg_size = 0;
	long long ret = 0;
	struct boot_img_hdr * hdr = NULL;
	unsigned mkbootimg_page;

	/* Read boot image header to determing length of the image */
	ret = read_image(gpte, sizeof(struct boot_img_hdr), buff);
	if(ret < 0){
		dbg_printf(PRN_ERR, "Failed to read boot image header!\n");
		return -1;
	}

	hdr = (struct boot_img_hdr *)buff;
	if (strncmp((const char *)hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)) {
		ERR("ERROR: Invalid boot header!\n");
		return -1;
	}

	if (hdr->header_version > BOOTIMG_HEADER_V2) {
		ERR("ERROR: Invalid boot image header version %d !\n", hdr->header_version);
		return -1;
	} else {
		NOTICE("BOOT IMAGE HEADER VERSION: %d !\n", hdr->header_version);
	}

	mkbootimg_page = hdr->page_size;

	// align by page_size and add one page for skipping header.
	bootimg_size = mkbootimg_page +
		get_aligned(hdr->kernel_size, mkbootimg_page) +
		get_aligned(hdr->ramdisk_size, mkbootimg_page) +
		get_aligned(hdr->second_size, mkbootimg_page);

	if (BOOTIMG_HEADER_V1 == hdr->header_version) {
		bootimg_size += \
			get_aligned(hdr->recovery_dtbo_size, mkbootimg_page);
	}

	if (BOOTIMG_HEADER_V2 == hdr->header_version) {
		bootimg_size += \
			get_aligned(hdr->recovery_dtbo_size, mkbootimg_page) +
			get_aligned(hdr->dtb_size, mkbootimg_page);
	}

	return bootimg_size;
}


// please be note, offset should be 512B aligned for emmc
AvbIOResult avb_read_from_partition(AvbOps* ops, const char* partition,
                                     int64_t offset, size_t num_bytes,
                                     void* buffer, size_t* out_num_read)
{
	AvbIOResult ret = AVB_IO_RESULT_OK;
	int index = 0;
	struct gpt_ent gpte;
	int64_t image_offset = offset;
	size_t image_size = num_bytes;
	unsigned char * image_buffer = NULL;

	unsigned int image_spare_size = 0x0;
	size_t already_read_size = 0x0;


	index = find_partition(partition);
	if(PARTITION_NOT_EXIST == index)
		return AVB_IO_RESULT_ERROR_NO_SUCH_PARTITION;

	fetch_partition_info(index, &gpte);

	if(offset < (int64_t)0x0) {
		image_offset = (int64_t)(gpte.ent_lba_end - gpte.ent_lba_start + 1) * SDIO_BLK_SIZE;
		image_offset += offset;
	}

	if(image_offset < (int64_t)0x0) {
		ERR("ERR: avb_read_from_partition invalid offset 0x%llx\n", image_offset);
		return AVB_IO_RESULT_ERROR_RANGE_OUTSIDE_PARTITION;
	}

	/* 1. handle the first un-aligned part */
	image_spare_size = (unsigned int)(image_offset % SDIO_BLK_SIZE);
	if(image_spare_size) {
		image_offset -= image_spare_size;
		image_buffer = (unsigned char *)malloc(SDIO_BLK_SIZE);
		if(NULL == image_buffer)
			return AVB_IO_RESULT_ERROR_OOM;

		ret = read_image_from_offset(&gpte, (unsigned int)image_offset, SDIO_BLK_SIZE, image_buffer);
		if(ret)
			return AVB_IO_RESULT_ERROR_IO;
		if(num_bytes <= (SDIO_BLK_SIZE - image_spare_size)) {
			memcpy(buffer, image_buffer + image_spare_size, num_bytes);
			already_read_size += num_bytes;
			goto out;
		}
		else {
			image_size = (SDIO_BLK_SIZE - image_spare_size);
			memcpy(buffer, image_buffer + image_spare_size, image_size);
			already_read_size += (SDIO_BLK_SIZE - image_spare_size);

			buffer += image_size;
			image_offset += SDIO_BLK_SIZE;
		}
	}

	/* 2. start to read the second aligned part */
	image_size = num_bytes - already_read_size;
	image_size = (image_size / SDIO_BLK_SIZE) * SDIO_BLK_SIZE;
	if(image_size > 0x0) {
		ret = read_image_from_offset(&gpte, (unsigned int)image_offset, image_size, buffer);
		if(ret)
			return AVB_IO_RESULT_ERROR_IO;

		already_read_size += image_size;
	}
	buffer += image_size;

	/* 3. handle the third un-aligned part */
	image_offset += image_size;
	image_size = num_bytes - already_read_size;
	if(image_size) {
		if(NULL == image_buffer) {
			image_buffer = (unsigned char *)malloc(SDIO_BLK_SIZE);
			if(NULL == image_buffer)
				return AVB_IO_RESULT_ERROR_OOM;
		}
		memset(image_buffer, 0x0, SDIO_BLK_SIZE);

		ret = read_image_from_offset(&gpte, (unsigned int)image_offset, SDIO_BLK_SIZE, image_buffer);
		if(ret)
			return AVB_IO_RESULT_ERROR_IO;

		memcpy(buffer, image_buffer, image_size);
		already_read_size += image_size;
	}

out:
	if(NULL != image_buffer) {
		free(image_buffer);
		image_buffer = NULL;
	}

	*out_num_read = already_read_size;

	return ret;
}

AvbIOResult avb_get_preloaded_partition(AvbOps* ops, const char* partition,
										size_t num_bytes, uint8_t** out_pointer,
										size_t* out_num_bytes_preloaded)
{
	if((0 == strncmp(partition, "boot", 4)) || (0 == strncmp(partition, "recovery", 8)))
	{
		*out_pointer = predata.boot_buff;
		*out_num_bytes_preloaded = predata.boot_size;
	}
#ifdef CONFIG_ENABLE_DTBO
	if(0 == strncmp(partition, "dtbo", 4))
	{
		*out_pointer = predata.dtbo_buff;
		*out_num_bytes_preloaded = predata.dtbo_size;
	}
#endif
	if(0 == strncmp(partition, "vendor_boot", 11))
	{
		*out_pointer = predata.vendor_boot_buff;
		*out_num_bytes_preloaded = predata.vendor_boot_size;
	}
	if (0 == strncmp(partition, "init_boot", 9)) {
		*out_pointer = predata.init_buff;
		*out_num_bytes_preloaded = predata.init_size;
	}

	return AVB_IO_RESULT_OK;
}

AvbIOResult avb_validate_vbmeta_public_key(AvbOps* ops, const uint8_t* public_key_data,
										size_t public_key_length, const uint8_t* public_key_metadata,
										size_t public_key_metadata_length, bool* out_is_trusted)
{
	if(memcmp((const char *)public_key_data, (const char *)predata.androidkey_buff, public_key_length) == 0) {
		*out_is_trusted = true;
#if defined(CONFIG_PRELOADTA)
		set_public_key((const char *)public_key_data, public_key_length);
#endif
	} else {
		*out_is_trusted = false;
	}

	return AVB_IO_RESULT_OK;
}

AvbIOResult avb_validate_public_key_for_partition(AvbOps* ops, const char* partition,
			const uint8_t* public_key_data,
			size_t public_key_length,
			const uint8_t* public_key_metadata,
			size_t public_key_metadata_length,
			bool* out_is_trusted,
			uint32_t* out_rollback_index_location)
{
	if(memcmp((const char *)public_key_data, (const char *)predata.androidkey_buff, public_key_length) == 0) {
		*out_is_trusted = true;
#if defined(CONFIG_PRELOADTA)
		set_public_key((const char *)public_key_data, public_key_length);
#endif
	} else {
		*out_is_trusted = false;
	}
	return AVB_IO_RESULT_OK;
}



AvbIOResult set_rollback_index_for_version_binding(const char* partition, uint64_t rollback_index)
{
#if defined(CONFIG_PRELOADTA)
	/* version binding feature only for Preload KM TA4.0 */
	if((0 == strncmp(partition, "boot", 4)) || (0 == strncmp(partition, "recovery", 8)))
	{
		gBoot_lvl = rollback_index;
		dbg_printf(PRN_INFO, "INFO: boot patch level 0x%llx \n", gBoot_lvl);
	}

	if(0 == strncmp(partition, "system", 6))
	{
		gSystem_lvl = rollback_index;
		dbg_printf(PRN_INFO, "INFO: system patch level 0x%llx \n", gSystem_lvl);
	}

	if(0 == strncmp(partition, "vendor", 6))
	{
		gVendor_lvl = rollback_index;
		dbg_printf(PRN_INFO, "INFO: vendor patch level 0x%llx \n", gVendor_lvl);
	}
#else
	UNUSED(partition);
	UNUSED(rollback_index);
#endif

	return AVB_IO_RESULT_OK;
}

AvbIOResult set_osVersion_for_version_binding(const char* prop_value,
				uint64_t value_bytes)
{
#if defined(CONFIG_PRELOADTA)
	#define MAX_OSVERSION_VALUE_LENGTH	7
	char *value_str = NULL;
	unsigned int os_version;

	/* Current solution: max version: 127.127.127 --> 2097151 */
	if (value_bytes > MAX_OSVERSION_VALUE_LENGTH || !prop_value) {
		dbg_printf(PRN_ERR, "ERR: failed to get property value for osVersion !\n");
		return AVB_IO_RESULT_ERROR_IO;
	}

	value_str = (char *)malloc(value_bytes + 1);
	if (!value_str) {
		dbg_printf(PRN_ERR, "ERR: failed to allocate memory for osVersion !\n");
		return AVB_IO_RESULT_ERROR_OOM;
	}

	memset((void *)value_str, 0x0, sizeof(value_str));
	memcpy((void *)value_str, (const void *)prop_value, (value_bytes + 1));
	os_version = (unsigned int)(strtoul(value_str, NULL, 10) & 0xFFFFFFFF);
	if ((os_version << 14) != gOs_version) {
		NOTICE("boot header os_version(%d) not equal to com.android.build.system.os_version(%d)\n", gOs_version, os_version);
		gOs_version = os_version << 14;
	}

	free(value_str);
	value_str = NULL;
#else
	UNUSED(prop_value);
	UNUSED(value_bytes);
#endif

	return AVB_IO_RESULT_OK;
}

static AvbIOResult avb_read_rollback_index(AvbOps* ops, size_t rollback_index_location,
							uint64_t* out_rollback_index)
{
	if (stored_rpmb_avb_info.magic == RPMB_AVB_MAGIC && rollback_index_location < 16) {
		*out_rollback_index = stored_rpmb_avb_info.values[rollback_index_location];
	} else {
		*out_rollback_index = 0;
	}
	INFO("%s(index=%ld)=%ld\n", __FUNCTION__, rollback_index_location, *out_rollback_index);
	return AVB_IO_RESULT_OK;
}

static AvbIOResult avb_write_rollback_index(AvbOps *ops,
						size_t rollback_index_location,
						uint64_t rollback_index) {
	INFO("%s(index=%ld, value=%ld)\n", __FUNCTION__, rollback_index_location, rollback_index);
	if (current_rpmb_avb_info.magic == RPMB_AVB_MAGIC && rollback_index_location < 16) {
		current_rpmb_avb_info.values[rollback_index_location] = rollback_index;
	}
	return AVB_IO_RESULT_OK;
}

static AvbIOResult avb_read_is_device_unlocked(AvbOps* ops, bool* out_is_unlocked)
{
	*out_is_unlocked = is_dev_lock() ? false : true;
	return AVB_IO_RESULT_OK;
}

AvbIOResult avb_get_unique_guid_for_partition(AvbOps* ops, const char* partition,
												char* guid_buf, size_t guid_buf_size)
{
	struct gpt_ent gpte_par;
	int ret = -1;

	ret = find_partition(partition);
	if(ret < 0) {
		ERR("Error: can't find %s image gpt info\n", partition);
		return AVB_IO_RESULT_ERROR_NO_SUCH_PARTITION;
	}
	fetch_partition_info(ret, &gpte_par);
	uuid_to_str(guid_buf, UUID_STR_LEN, &gpte_par.ent_uuid);

	return AVB_IO_RESULT_OK;
}

AvbIOResult avb_get_size_of_partition(AvbOps* ops, const char* partition, uint64_t* out_size_num_bytes)
{
	//FIXME: for images in different partition, maybe the method is not the same
	if((0 == strncmp(partition, "boot", 4)) || (0 == strncmp(partition, "recovery", 8))) {
		*out_size_num_bytes = predata.boot_size;
	}
#ifdef CONFIG_ENABLE_DTBO
	if(0 == strncmp(partition, "dtbo", 4)) {
		*out_size_num_bytes = predata.dtbo_size;
	}
#endif
	return AVB_IO_RESULT_OK;
}

char * get_avb_kernel_commandline(void)
{
	return avb_kernel_commandline;
}

#ifdef CONFIG_ENABLE_DTBO
unsigned char * get_dtbo_image(void)
{
	return predata.dtbo_buff;
}
#endif

unsigned char * get_cmdline(void)
{
	return predata.cmdline;
}

unsigned int get_ramdisk_buff(void)
{
	return predata.ramdisk_addr;
}

unsigned int get_ramdisk_size(void)
{
	return predata.ramdisk_size;
}

unsigned int set_ramdisk_size(unsigned int new_size)
{
	return predata.ramdisk_size = new_size;
}


unsigned int get_bootconfig_start_addr(void)
{
	return predata.bootconfig_start_addr;
}

unsigned int get_bootconfig_size(void)
{
	return predata.bootconfig_size;
}


static int verify_slot(const char** requested_partitions)
{
	bool isDeviceUnlocked = false;
	AvbSlotVerifyResult verify_result;
	AvbOps ops;
	AvbSlotVerifyData* slot_data = NULL;
	bool set_slot_unbootable = false;
	AvbSlotVerifyFlags flags = 0;
#if defined(CONFIG_AVB_1P2_LAUNCHED) && !defined(CONFIG_AB_BOOTFLOW)
	int boot_mode = get_bootmode();
	if(boot_mode == BOOTMODE_NORMAL)
		flags = AVB_SLOT_VERIFY_FLAGS_NONE;
	else if (boot_mode == BOOTMODE_RECOVERY)
		flags = AVB_SLOT_VERIFY_FLAGS_NO_VBMETA_PARTITION;
#else
	flags = AVB_SLOT_VERIFY_FLAGS_NONE;
#endif

	allow_verification_error = false;

	ops.read_from_partition = avb_read_from_partition;
	ops.get_preloaded_partition = avb_get_preloaded_partition;
	ops.write_to_partition = NULL;
	ops.validate_vbmeta_public_key = avb_validate_vbmeta_public_key;
	ops.validate_public_key_for_partition = avb_validate_public_key_for_partition;

	ops.read_rollback_index = avb_read_rollback_index;
	ops.write_rollback_index = avb_write_rollback_index;

	ops.read_is_device_unlocked = avb_read_is_device_unlocked;
	ops.get_unique_guid_for_partition = avb_get_unique_guid_for_partition;
	ops.get_size_of_partition = avb_get_size_of_partition;

	ops.ab_ops = NULL;

	ops.read_is_device_unlocked(NULL, &isDeviceUnlocked);
	if (isDeviceUnlocked) {
		flags |= AVB_SLOT_VERIFY_FLAGS_ALLOW_VERIFICATION_ERROR;
	}

	verify_result = avb_slot_verify(&ops, requested_partitions, get_pt_suffix(), flags, AVB_HASHTREE_ERROR_MODE_RESTART, &slot_data);

	switch (verify_result) {
		case AVB_SLOT_VERIFY_RESULT_ERROR_OOM:
			ERR("out of memory\n");
			set_slot_unbootable = true;
			break;
		case AVB_SLOT_VERIFY_RESULT_ERROR_IO:
			ERR("IO error\n");
			set_slot_unbootable = true;
			break;
		case AVB_SLOT_VERIFY_RESULT_OK:
			avb_kernel_commandline = slot_data->cmdline;
			INFO("command line: %s\n\n",avb_kernel_commandline);
			INFO("\nverify slot done\n");
			break;
		case AVB_SLOT_VERIFY_RESULT_ERROR_INVALID_METADATA:
		case AVB_SLOT_VERIFY_RESULT_ERROR_UNSUPPORTED_VERSION:
			/* Even with AVB_SLOT_VERIFY_FLAGS_ALLOW_VERIFICATION_ERROR
			* these mean game over.
			*/
			set_slot_unbootable = true;
			break;
		/* explicit fallthrough. */
		case AVB_SLOT_VERIFY_RESULT_ERROR_VERIFICATION:
		case AVB_SLOT_VERIFY_RESULT_ERROR_ROLLBACK_INDEX:
		case AVB_SLOT_VERIFY_RESULT_ERROR_PUBLIC_KEY_REJECTED:
			if (flags & AVB_SLOT_VERIFY_FLAGS_ALLOW_VERIFICATION_ERROR) {
			/* Do nothing since we allow this. */
				NOTICE("find allowed error\n");
				avb_kernel_commandline = slot_data->cmdline;
				allow_verification_error = true;
			} else {
				set_slot_unbootable = true;
			}
			break;
		case AVB_SLOT_VERIFY_RESULT_ERROR_INVALID_ARGUMENT:
			NOTICE("\n");
			set_slot_unbootable = true;
			break;
		/* Do not add a 'default:' case here because of -Wswitch. */
	}

	if(set_slot_unbootable == true)
	{
		ERR("Slot verify fail and unbootable!!!!\n");
		return -1;
	}
	return 0;
}

static int load_and_verify_keystore(unsigned char * k_buff )
{
	uint8_t * oem_keystore = NULL;
	uint8_t * user_keystore = NULL;

	oem_keystore = load_android_keystore(ANDROID_OEM_KEY, k_buff);
	if(NULL == oem_keystore) {
		vb_state_enterstep(VB_STATE_OEMK_FAIL);
		user_keystore = load_android_keystore(ANDROID_USR_KEY, k_buff);
		if(NULL == user_keystore) {
			vb_state_enterstep(VB_STATE_USERK_FAIL);
			ERR("failed to load both oem and usr android key store!\n");
			return -1;
		} else {
			if(is_dev_lock())
			{
				if(VB_REBOOT == vb_state_enterstep(VB_STATE_USERK_SUCC)) {
					return -1;
				}
			}
		}
	} else {
		if(is_dev_lock())
			vb_state_enterstep(VB_STATE_OEMK_SUCC);
	}
	predata.androidkey_buff = (oem_keystore != NULL) ? oem_keystore : user_keystore;

	return 0;
}

static int load_android_image_v2(int bootmode)
{
	boot_img_hdr Mkbootimg_hdr;
	struct gpt_ent gpte_kernel;
	int result = 0;
	long long ret = -1;
	unsigned int mkbootimg_page = 0;
	unsigned int k_buff_offset = 0;
	int cpu_img_size = 0;
	struct image_header * gih = NULL;
	unsigned char * const k_buff = malloc_ion_cacheable(MAX_ANDROID_IMG_SIZE + 0x200000); //alloc 32M+2M mmeory. free or not free doesn't matter
	unsigned char * k_buff_img   = k_buff + 0x200000;	    //Start from 2M offset for kernel image buffer
	char * cmdline = malloc_ion_cacheable(BOOT_ARGS_TOTAL_SIZE);

	memset(k_buff, 0, MAX_ANDROID_IMG_SIZE);

	ret = get_pt_index(PT_BOOT);
	if(ret < 0) {
		dbg_printf(PRN_ERR, "system critical error: can't find kernel image gpt info(bootmode = %d)!\n", bootmode);
		return -1;
	}
	dbg_printf(PRN_INFO, "entry num = %d !\n", ret);
	fetch_partition_info(ret, &gpte_kernel);

	dbg_printf(PRN_INFO, "DEBUG: Start to get Boot size !\n");
	cpu_img_size = get_boot_image_size_v2(&gpte_kernel, k_buff);
	if(cpu_img_size == -1) {
		ERR("ERROR: wrong boot image header");
		return -1;
	}

	dbg_printf(PRN_INFO, "DEBUG: Kernel Original Boot size 0x%x !\n", cpu_img_size);

	/* Add min size check to be safe */
	if (cpu_img_size > MAX_ANDROID_IMG_SIZE || cpu_img_size < MIN_ANDROID_IMG_SIZE) {
		dbg_printf(PRN_ERR, "ERROR: aligned CPU img_size %u is invalid\n", cpu_img_size);
		return -1;
	}

	ret = read_image(&gpte_kernel, cpu_img_size, k_buff_img);
	if (ret < 0){
		dbg_printf(PRN_ERR, "ERROR: Failed to read CPU image ret %d\n", ret);
		return -1;
	}

	dbg_printf(PRN_INFO, "DEBUG: Kernel Original Image Offset 0x%x !\n", k_buff);


#ifdef CONFIG_ENABLE_DTBO
#ifdef CONFIG_NR_BOOTFLOW
	if(BOOTMODE_RECOVERY != bootmode)
#endif
	{	//DTBO Partition
		extern int load_dtbo(void *buff, unsigned int *size);
		predata.dtbo_buff = malloc(MAX_DTBO_SIZE);
		if(load_dtbo(predata.dtbo_buff, &predata.dtbo_size)) {
			ERR("load dtbo image error\n");
			return -1;
		}
	}
#endif
	//load and verify keystore
	if(load_and_verify_keystore(k_buff))
	{
		ERR("load_and_verify_keystore fail!!!!\n");
		return -1;
	}

	memcpy(&Mkbootimg_hdr, k_buff_img, sizeof(Mkbootimg_hdr));
	if (strncmp((const char *)Mkbootimg_hdr.magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)) {
		dbg_printf(PRN_ERR, "ERROR: Invalid mkbootimg header!\n");
		return -1;
	}

	predata.boot_buff = k_buff_img;
	predata.boot_size = cpu_img_size;
	predata.cmdline = (unsigned char*)cmdline;
	predata.ramdisk_addr = get_ramdisk_addr();
	predata.ramdisk_size = Mkbootimg_hdr.ramdisk_size;

	//avb slot verify
	const char* requested_partitions[] = {
		get_boot_prefix(),
		get_dtbo_prefix(),
		NULL
	};

	if(verify_slot(requested_partitions) == -1)
	{
		ERR("verify_slot fail!!!!\n");
		return -1;
	}

	if(cmdline)
	{
		memset(cmdline, 0x0, BOOT_ARGS_TOTAL_SIZE);
		strcpy(cmdline, (const char *)Mkbootimg_hdr.cmdline);
	}

	/* force to use predefined kernel addr (CPU0_START_ADDR) in config.
	 * We don't use the kernel_addr from kernel partition for scurity
	 * consideration.
	 */
	Mkbootimg_hdr.kernel_addr = get_kernel_addr();

	mkbootimg_page = Mkbootimg_hdr.page_size;
	if (BOOTIMG_HEADER_V2 != header_version) {
		dbg_printf(PRN_INFO, "DEBUG: mkbootimg_page = %d !\n", mkbootimg_page);

		gih = (struct image_header *)(k_buff_img + mkbootimg_page);

		result = find_chunk(IMAGE_CHUNK_ID_LINUX_BOOTIMG, gih);

		if(result < 0) {
			dbg_printf(PRN_ERR, "didn't find kernel image!!!\n");
			return -1;
		}
	}

	//kernel
#ifdef CONFIG_LZ4_BOOT
	unsigned int decompressed_size = MAX_ANDROID_IMG_SIZE; //set default decompressed_size to maxium buffer size.
	if (BOOTIMG_HEADER_V2 == header_version) {
		dbg_printf(PRN_INFO, "decompressing kernel image size %d...\n", Mkbootimg_hdr.kernel_size);
		ret = lz4_decompress((const unsigned char *)(k_buff_img + mkbootimg_page),
				(const unsigned int)Mkbootimg_hdr.kernel_size,
				(unsigned char *)((uintptr_t)Mkbootimg_hdr.kernel_addr),
				&decompressed_size);
		if (ret != 0){
			dbg_printf(PRN_ERR, "kernel image decompress failed !\n");
			return -1;
		}
		dbg_printf(PRN_INFO, "kernel img addr 0x%x, img len %d.\n", Mkbootimg_hdr.kernel_addr, decompressed_size);
	} else {
		dbg_printf(PRN_INFO, "decompressing kernel image size %d...\n", gih->chunk[result].size);
		ret = lz4_decompress((const unsigned char *)(k_buff_img + mkbootimg_page + gih->chunk[result].offset),
				(const unsigned int)( gih->chunk[result].size),
				(unsigned char *)((uintptr_t)Mkbootimg_hdr.kernel_addr),
				&decompressed_size);
		if (ret != 0){
			dbg_printf(PRN_ERR, "kernel image decompress failed !\n");
			return -1;
		}
		dbg_printf(PRN_INFO, "kernel img addr 0x%x, img len %d.\n", Mkbootimg_hdr.kernel_addr, decompressed_size);
	}
#else
	if (BOOTIMG_HEADER_V2 == header_version) {
		memcpy((void *)((uintptr_t)(Mkbootimg_hdr.kernel_addr)), k_buff_img + mkbootimg_page, \
							Mkbootimg_hdr.kernel_size);
	} else {
		memcpy((void *)((uintptr_t)(Mkbootimg_hdr.kernel_addr)), k_buff_img + mkbootimg_page + gih->chunk[result].offset, \
							(gih->chunk[result].size));
	}

#endif

	set_kernel_entry(Mkbootimg_hdr.kernel_addr);

	if (BOOTIMG_HEADER_V2 == header_version) {
		k_buff_offset = get_aligned(Mkbootimg_hdr.header_size, mkbootimg_page) +
				get_aligned(Mkbootimg_hdr.kernel_size, mkbootimg_page) +
				get_aligned(Mkbootimg_hdr.ramdisk_size, mkbootimg_page) +
				get_aligned(Mkbootimg_hdr.second_size, mkbootimg_page) +
				get_aligned(Mkbootimg_hdr.recovery_dtbo_size, mkbootimg_page);

		memcpy(((void *)(uintptr_t)get_dtb_addr()), \
			(k_buff_img + k_buff_offset), Mkbootimg_hdr.dtb_size);
	} else {
		result = find_chunk(IMAGE_CHUNK_ID_LINUX_DTB, gih);
		if(result < 0) {
			dbg_printf(PRN_ERR, "didn't find dtb!!!\n");
			return -1;
		}
		memcpy(((void *)(uintptr_t)get_dtb_addr()), \
			(k_buff_img + mkbootimg_page + gih->chunk[result].offset), gih->chunk[result].size);
	}

#ifdef CONFIG_NR_BOOTFLOW
	if(BOOTMODE_RECOVERY == bootmode) {
		if(Mkbootimg_hdr.recovery_dtbo_size && (Mkbootimg_hdr.recovery_dtbo_size < MAX_DTBO_SIZE)) {
			struct dt_table_header *dtbo_header = NULL;

			predata.dtbo_buff = (unsigned char *)malloc(Mkbootimg_hdr.recovery_dtbo_size);
			if(!predata.dtbo_buff) {
				dbg_printf(PRN_ERR, "ERR: fail to alloc common memory for recovery_dtbo!\n");
				return -1;
			}

			dtbo_header = (struct dt_table_header *)predata.dtbo_buff;
			k_buff_offset = get_aligned(Mkbootimg_hdr.header_size, mkbootimg_page) +
					get_aligned(Mkbootimg_hdr.kernel_size, mkbootimg_page) +
					get_aligned(Mkbootimg_hdr.ramdisk_size, mkbootimg_page) +
					get_aligned(Mkbootimg_hdr.second_size, mkbootimg_page);
			Mkbootimg_hdr.recovery_dtbo_offset[0] = (unsigned int)((uintptr_t)predata.dtbo_buff & 0xffffffff);
			Mkbootimg_hdr.recovery_dtbo_offset[1] = (unsigned int)(((uintptr_t)predata.dtbo_buff >> 32) & 0xffffffff);
			dbg_printf(PRN_RES, "###find recovery dtbo size:0x%x, offset:0x%08x, load_addr:0x%x%08x ###\n",
					Mkbootimg_hdr.recovery_dtbo_size, k_buff_offset,
					Mkbootimg_hdr.recovery_dtbo_offset[1], Mkbootimg_hdr.recovery_dtbo_offset[0]);

			memcpy(predata.dtbo_buff, k_buff_img + k_buff_offset, Mkbootimg_hdr.recovery_dtbo_size);
			if((DT_TABLE_MAGIC != cpu_to_fdt32(dtbo_header->magic)) || \
					(cpu_to_fdt32(dtbo_header->dt_entry_count) < 1) || \
					(Mkbootimg_hdr.recovery_dtbo_size != cpu_to_fdt32(dtbo_header->total_size))) {
				predata.dtbo_size = 0x0;
				dbg_printf(PRN_ERR, "ERR: invalid dtbo image format!\n");
				return -1;
			}
			else
				predata.dtbo_size = Mkbootimg_hdr.recovery_dtbo_size;
		}
		else {
			dbg_printf(PRN_ERR, "ERR: invalid recovery dtbo image size !\n");
			return -1;
		}
	}
#endif

#ifdef CONFIG_SYSTEM_AS_ROOT
	//ramdisk, only for recovery mode !!
	if(BOOTMODE_NORMAL == bootmode) {
		// if size set to 0, don't use ramdisk
		Mkbootimg_hdr.ramdisk_size = 0x0;
	}
	else {
		if(!Mkbootimg_hdr.ramdisk_size) {
			dbg_printf(PRN_ERR, "didn't find ramdisk for Recovery mode !!!\n");
			return -1;
		}
	}
#endif

	Mkbootimg_hdr.ramdisk_addr = get_ramdisk_addr();
	dbg_printf(PRN_INFO, "INFO: ramdisk_addr 0x%08x, size 0x%08x; while DTB address = 0x%08x!\n", \
			Mkbootimg_hdr.ramdisk_addr, Mkbootimg_hdr.ramdisk_size, get_dtb_addr());

	if (Mkbootimg_hdr.ramdisk_size) {
		/* we also don't use the ramdisk_addr read from flash for security reason */
		//Mkbootimg_hdr.ramdisk_addr = (Mkbootimg_hdr.kernel_addr - LINUX_OFFSET + RAMDISK_OFFSET);
		dbg_printf(PRN_RES,"######find ramdisk and copy to 0x%08x\n", Mkbootimg_hdr.ramdisk_addr);
		k_buff_offset = get_aligned(Mkbootimg_hdr.kernel_size + mkbootimg_page, mkbootimg_page);
		memcpy((void *)((uintptr_t)(Mkbootimg_hdr.ramdisk_addr)), (k_buff_img + k_buff_offset), Mkbootimg_hdr.ramdisk_size);
	}
	print_bootimg_image_hdr_v2(&Mkbootimg_hdr);

	return 0;
}


static int load_android_image_v3(int bootmode)
{
	int boot_img_size = 0, vendor_boot_img_size = 0;
	unsigned int dtbo_offset, ramdisk_offset, vendor_ramdisk_offset;
	long long ret = -1;
	void *pt_name;
	boot_img_hdr_v3 boot_hdr;
	vendor_boot_img_hdr_v3 vendor_hdr;
	unsigned char * boot_buff = NULL;
	unsigned char * vendor_buff = NULL;
	unsigned char * k_buff_img = NULL;
	char * cmdline = malloc_ion_cacheable(BOOT_ARGS_TOTAL_SIZE);

	boot_img_size = get_boot_image_size_v3((void *)&boot_hdr);
	if(boot_img_size == -1) {
		ERR("ERROR: wrong boot image header");
		return -1;
	}

	vendor_boot_img_size = get_vendor_boot_image_size(&vendor_hdr);
	if(vendor_boot_img_size == -1) {
		ERR("ERROR: wrong vendor boot image header");
		return -1;
	}

	boot_buff = malloc_ion_cacheable(boot_img_size + 0x200000);
	vendor_buff = malloc_ion_cacheable(vendor_boot_img_size);
	k_buff_img   = boot_buff + 0x200000; //Start from 2M offset for kernel image buffer

	if(cmdline)
	{
		memset(cmdline, 0x0, BOOT_ARGS_TOTAL_SIZE);
		strcpy(cmdline, (const char *)vendor_hdr.cmdline);
		strcat(cmdline, " ");
		strcat(cmdline, (const char *)boot_hdr.cmdline);
	}
#ifdef CONFIG_AB_BOOTFLOW
	pt_name = (get_abmode() == BOOTSEL_A) ?  KERNEL_A_NAME : KERNEL_B_NAME;
#else
	pt_name = (get_bootmode() == BOOTMODE_NORMAL) ? KERNEL_NAME : RECOVERY_NAME;
#endif
	ret = read_image_by_ptname(pt_name, boot_img_size, k_buff_img);
	if (ret < 0){
		ERR("ERROR: Failed to read boot image ret %d\n", ret);
		return -1;
	}

#ifdef CONFIG_AB_BOOTFLOW
	pt_name = (get_abmode() == BOOTSEL_A) ? VENDOR_BOOT_A_NAME : VENDOR_BOOT_B_NAME;
#else
	pt_name = VENDOR_BOOT_NAME;
#endif
	ret = read_image_by_ptname(pt_name, vendor_boot_img_size, vendor_buff);
	if (ret < 0){
		ERR("ERROR: Failed to read vendor_boot image ret %d\n", ret);
		return -1;
	}

	predata.boot_buff = k_buff_img;
	predata.boot_size = boot_img_size;
	predata.vendor_boot_buff = vendor_buff;
	predata.vendor_boot_size = vendor_boot_img_size;
	predata.cmdline = (unsigned char*)cmdline;
	predata.ramdisk_addr = get_ramdisk_addr();
	predata.ramdisk_size = vendor_hdr.vendor_ramdisk_size + boot_hdr.ramdisk_size;

#ifdef CONFIG_ENABLE_DTBO
#ifdef CONFIG_NR_BOOTFLOW
	if(BOOTMODE_RECOVERY != bootmode)
#endif
	{	//DTBO Partition
		extern int load_dtbo(void *buff, unsigned int *size);
		predata.dtbo_buff = malloc(MAX_DTBO_SIZE);
		if(load_dtbo(predata.dtbo_buff, &predata.dtbo_size)) {
			ERR("load dtbo image error\n");
			return -1;
		}
	}
#endif

	//load and verify keystore
	if(load_and_verify_keystore(boot_buff))
	{
		ERR("load_and_verify_keystore fail!!!!\n");
		return -1;
	}

	//avb slot verify
	const char* requested_partitions[] = {
		get_boot_prefix(),
		get_dtbo_prefix(),
		get_vendor_boot_prefix(),
		NULL
	};

	if(verify_slot(requested_partitions) == -1)
	{
		ERR("verify_slot fail!!!!\n");
		return -1;
	}

	/* We don't use the kernel_addr/ramdisk_addr/dtb_addr from vendor boot header for scurity consideration. */
	vendor_hdr.kernel_addr = get_kernel_addr();
	vendor_hdr.ramdisk_addr = get_ramdisk_addr();
	vendor_hdr.dtb_addr = get_dtb_addr();

	//Move kernel image to destination
#ifdef CONFIG_LZ4_BOOT
	unsigned int decompressed_size = MAX_ANDROID_IMG_SIZE; //set default decompressed_size to maxium buffer size.
	NOTICE("decompressing kernel image size %d...\n", boot_hdr.kernel_size);
	ret = lz4_decompress((const unsigned char *)(k_buff_img + BOOT_IMAGE_V3_PAGE_SIZE),
			(const unsigned int)boot_hdr.kernel_size,
			(unsigned char*)((uintptr_t)vendor_hdr.kernel_addr),
			&decompressed_size);
	if (ret != 0){
		ERR("kernel image decompress failed !\n");
		return -1;
	}
	NOTICE("kernel img addr 0x%x, img len %d.\n", vendor_hdr.kernel_addr, decompressed_size);

#else
	memcpy((void *)((uintptr_t)(vendor_hdr.kernel_addr)), k_buff_img + BOOT_IMAGE_V3_PAGE_SIZE, \
							boot_hdr.kernel_size);
#endif

	set_kernel_entry(vendor_hdr.kernel_addr);

	//Move dtb image to destination
	dtbo_offset = get_aligned(vendor_hdr.header_size, vendor_hdr.page_size) +
					get_aligned(vendor_hdr.vendor_ramdisk_size,  vendor_hdr.page_size);
	memcpy((void *)vendor_hdr.dtb_addr, vendor_buff + dtbo_offset, vendor_hdr.dtb_size);

	//load the vendor ramdisk to destination
	if (vendor_hdr.vendor_ramdisk_size) {
		vendor_ramdisk_offset = get_aligned(vendor_hdr.header_size, vendor_hdr.page_size);
		memcpy((void *)((uintptr_t)(vendor_hdr.ramdisk_addr)), \
			(void *)(vendor_buff + vendor_ramdisk_offset), vendor_hdr.vendor_ramdisk_size);
	}

	//load the generic ramdisk immediately following the vendor ramdisk in memory
	if (boot_hdr.ramdisk_size) {
		unsigned int boot_ramdisk_addr = vendor_hdr.ramdisk_addr + vendor_hdr.vendor_ramdisk_size;
		ramdisk_offset = BOOT_IMAGE_V3_PAGE_SIZE;
		ramdisk_offset += get_aligned(boot_hdr.kernel_size, BOOT_IMAGE_V3_PAGE_SIZE);
		memcpy((void *)(uintptr_t)boot_ramdisk_addr, (void*)(k_buff_img + ramdisk_offset), boot_hdr.ramdisk_size);
	}

	return 0;
}


static INT32 load_android_image_v4(INT32 bootmode)
{
	INT32 boot_img_size = 0, vendor_boot_img_size = 0, init_boot_size = 0;
	UINT32 dtbo_offset, vendor_ramdisk_table_offset;
	UINT32 vendor_ramdisk_size = 0;
	UINT32 vendor_ramdisk_offset =0;
	INT64 ret = -1;
	boot_img_hdr_v4 boot_hdr;
	boot_img_hdr_v4 init_boot_header = { 0 };
	vendor_boot_img_hdr_v4 vendor_hdr;
	vendor_ramdisk_table_entry_v4 *vendor_ramdisk_table_entrys;
	UCHAR * boot_buff = NULL;
	UCHAR * init_buff = NULL;
	UCHAR * vendor_buff = NULL;
	UCHAR * k_buff_img = NULL;
	CHAR * cmdline = malloc_ion_cacheable(BOOT_ARGS_TOTAL_SIZE);

	boot_img_size = au->get_image_size(au->part2gpt(BOOT));
	if (AVB_IO_RESULT_OK != au->parse_boot_header(&boot_hdr, BOOT) || boot_img_size == -1) {
		ERR("ERROR: wrong boot image header");
		return -1;
	}

	vendor_boot_img_size = au->get_image_size(au->part2gpt(VENDOR_BOOT));
	if (AVB_IO_RESULT_OK != au->parse_vndboot_header(&vendor_hdr) || vendor_boot_img_size == -1) {
		ERR("ERROR: wrong vendor boot image header");
		return -1;
	}

	init_boot_size = au->get_image_size(au->part2gpt(INIT_BOOT));
	if (init_boot_size > 0) {
		au->parse_boot_header(&init_boot_header, INIT_BOOT);
		predata.init_size = init_boot_size;
		init_buff = malloc_ion_cacheable(predata.init_size);
		predata.init_buff = init_buff;
		if (read_image_by_ptname(au->part2gpt(INIT_BOOT), predata.init_size, init_buff) < 0){
			ERR("ERROR: Failed to read init_boot image\n");
			return -1;
		}
	}

	boot_buff   = malloc_ion_cacheable(boot_img_size + 0x200000);
	vendor_buff = malloc_ion_cacheable(vendor_boot_img_size);
	k_buff_img  = boot_buff + 0x200000; //Start from 2M offset for kernel image buffer

	if(cmdline)
	{
		memset(cmdline, 0x0, BOOT_ARGS_TOTAL_SIZE);
		strcpy(cmdline, (const char *)vendor_hdr.cmdline);
		strcat(cmdline, " ");
		strcat(cmdline, (const char *)boot_hdr.cmdline);
	}

	if (read_image_by_ptname(au->part2gpt(BOOT), boot_img_size, k_buff_img) < 0){
		ERR("ERROR: Failed to read boot image ret %d\n", ret);
		return -1;
	}

	if (read_image_by_ptname(au->part2gpt(VENDOR_BOOT), vendor_boot_img_size, vendor_buff) < 0){
		ERR("ERROR: Failed to read vendor_boot image ret %d\n", ret);
		return -1;
	}

	predata.boot_buff = k_buff_img;
	predata.boot_size = boot_img_size;
	predata.vendor_boot_buff = vendor_buff;
	predata.vendor_boot_size = vendor_boot_img_size;
	predata.cmdline = (unsigned char*)cmdline;
	predata.ramdisk_addr = get_ramdisk_addr();
	predata.ramdisk_size = vendor_hdr.vendor_ramdisk_size + boot_hdr.ramdisk_size;

#ifdef CONFIG_ENABLE_DTBO
#ifdef CONFIG_NR_BOOTFLOW
	if(BOOTMODE_RECOVERY != bootmode)
#endif
	{	//DTBO Partition
		extern int load_dtbo(void *buff, unsigned int *size);
		predata.dtbo_buff = malloc(MAX_DTBO_SIZE);
		if(load_dtbo(predata.dtbo_buff, &predata.dtbo_size)) {
			ERR("load dtbo image error\n");
			return -1;
		}
	}
#endif

	if(load_and_verify_keystore(boot_buff))
	{
		ERR("load_and_verify_keystore fail!!!!\n");
		return -1;
	}

	if (!gRpmbAvbOps->init()) {
		ERR("RPMB AVB init failed\n");
		return -1;
	}
	if (!gRpmbAvbOps->read(&stored_rpmb_avb_info)) {
		ERR("RPMB AVB read failed\n");
		memset(&stored_rpmb_avb_info, 0, sizeof(rpmb_avb_t));
		return -1;
	}
	if (is_dev_lock()) {
		avb_memcpy(&current_rpmb_avb_info, &stored_rpmb_avb_info, sizeof(rpmb_avb_t));
	} else {
		rpmb_avb_t empty_info = { 0 };
		empty_info.magic = RPMB_AVB_MAGIC;
		empty_info.version = 0;
		empty_info.unlock = 0xa5; // FRP_UNLOCK_FLAG
		if (0 != avb_memcmp(&stored_rpmb_avb_info, &empty_info, sizeof(rpmb_avb_t))) {
			gRpmbAvbOps->write(&empty_info);
			avb_memcpy(&stored_rpmb_avb_info, &empty_info, sizeof(rpmb_avb_t));
			INFO("RPMB AVB erased\n");
		} else {
			INFO("RPMB AVB already erased\n");
		}
	}

	//avb slot verify
	const char* requested_partitions[] = {
		get_boot_prefix(),
		get_dtbo_prefix(),
		get_vendor_boot_prefix(),
		init_boot_size > 0 ? get_init_boot_prefix() : NULL,
		NULL
	};

	if(verify_slot(requested_partitions) == -1)
	{
		ERR("verify_slot fail!!!!\n");
		return -1;
	}

	if (check_boot_success() && (GREEN == get_verify_state())) {
		if (0 == avb_memcmp(&current_rpmb_avb_info, &stored_rpmb_avb_info, sizeof(rpmb_avb_t))) {
			INFO("RPMB AVB up-to-date\n");
		} else {
			if (gRpmbAvbOps->write(&current_rpmb_avb_info)) {
				INFO("RPMB AVB update done\n");
			} else {
				ERR("RPMB AVB update fail\n");
				return  -1;
			}
		}
	}

	/* We don't use the kernel_addr/ramdisk_addr/dtb_addr from vendor
	 * boot header for scurity consideration. */
	vendor_hdr.kernel_addr  = get_kernel_addr();
	vendor_hdr.ramdisk_addr = get_ramdisk_addr();
	vendor_hdr.dtb_addr     = get_dtb_addr();

	//Move kernel image to destination
#ifdef CONFIG_LZ4_BOOT
	UINT32 decompressed_size = MAX_ANDROID_IMG_SIZE; //set default decompressed_size to maxium buffer size.
	INFO("decompressing kernel image size %d...\n", boot_hdr.kernel_size);
	ret = lz4_decompress((const UCHAR *)(k_buff_img + BOOT_IMAGE_V4_PAGE_SIZE),
			(const UINT32)boot_hdr.kernel_size,
			(UCHAR*)((uintptr_t)vendor_hdr.kernel_addr),
			&decompressed_size);
	if (ret != 0){
		ERR("kernel image decompress failed !\n");
		return -1;
	}
	INFO("kernel img addr 0x%x, img len %d.\n", vendor_hdr.kernel_addr, decompressed_size);
#else
	memcpy((void *)((uintptr_t)(vendor_hdr.kernel_addr)), k_buff_img + BOOT_IMAGE_V4_PAGE_SIZE, \
							boot_hdr.kernel_size);
#endif

	set_kernel_entry(vendor_hdr.kernel_addr);

	//Move dtb image to destination
	if (vendor_hdr.dtb_size) {
		dtbo_offset = get_aligned(vendor_hdr.header_size, vendor_hdr.page_size) +
                              get_aligned(vendor_hdr.vendor_ramdisk_size, vendor_hdr.page_size);
		memcpy((void *)vendor_hdr.dtb_addr, (void*)(vendor_buff + dtbo_offset), vendor_hdr.dtb_size);
	}

	//load the vendor ramdisk to destination
	if (vendor_hdr.vendor_ramdisk_size) {
		UINT32 vendor_ramdisk_size_p = 0, vendor_ramdisk_size_r = 0;
		void *vendor_ramdisk_addr = (void*)(uintptr_t)vendor_hdr.ramdisk_addr;
		vendor_ramdisk_table_offset = get_aligned(vendor_hdr.header_size, vendor_hdr.page_size) +
                                              get_aligned(vendor_hdr.vendor_ramdisk_size, vendor_hdr.page_size) +
                                              get_aligned(vendor_hdr.dtb_size, vendor_hdr.page_size);
		vendor_ramdisk_table_entrys = (vendor_ramdisk_table_entry_v4 *)(vendor_buff + vendor_ramdisk_table_offset);

		//Load platform ramdisk by default
		for(int i = 0; i < vendor_hdr.vendor_ramdisk_table_entry_num; i++)
		{
			if (vendor_ramdisk_table_entrys[i].ramdisk_type == VENDOR_RAMDISK_TYPE_PLATFORM)
			{
				vendor_ramdisk_offset = vendor_ramdisk_table_entrys[i].ramdisk_offset;
				vendor_ramdisk_size_p = vendor_ramdisk_table_entrys[i].ramdisk_size;
			}
		}

		if (vendor_ramdisk_size_p) {
			vendor_ramdisk_offset += get_aligned(vendor_hdr.header_size, vendor_hdr.page_size);
			memcpy(vendor_ramdisk_addr, (void *)(vendor_buff + vendor_ramdisk_offset), vendor_ramdisk_size_p);
			vendor_ramdisk_size += vendor_ramdisk_size_p;
			vendor_ramdisk_addr += vendor_ramdisk_size_p;
		}

		//Load recovery ramdisk following the platform ramdisk in memory if recovery mode
		if (BOOTMODE_RECOVERY == bootmode)
		{
			for(int i = 0; i < vendor_hdr.vendor_ramdisk_table_entry_num; i++)
			{
				if (vendor_ramdisk_table_entrys[i].ramdisk_type == VENDOR_RAMDISK_TYPE_RECOVERY)
				{
					vendor_ramdisk_offset = vendor_ramdisk_table_entrys[i].ramdisk_offset;
					vendor_ramdisk_size_r   = vendor_ramdisk_table_entrys[i].ramdisk_size;
				}
			}

			if (vendor_ramdisk_size_r) {
				vendor_ramdisk_offset += get_aligned(vendor_hdr.header_size, vendor_hdr.page_size);
				memcpy(vendor_ramdisk_addr, (void *)(vendor_buff + vendor_ramdisk_offset), vendor_ramdisk_size_r);
				vendor_ramdisk_size += vendor_ramdisk_size_r;
			}
		}
		else if (BOOTMODE_NORMAL != bootmode )
			ERR("Boot Mode(%d) isn't Correct!!!\n", bootmode);
	}

	//load the generic ramdisk immediately following the vendor ramdisk in memory
	if (boot_hdr.ramdisk_size) {
		void  *boot_ramdisk_addr = (void*)(uintptr_t)(vendor_hdr.ramdisk_addr + vendor_ramdisk_size);
		UINT32 boot_ramdisk_offset = get_aligned(boot_hdr.header_size, BOOT_IMAGE_V4_PAGE_SIZE) +
                                             get_aligned(boot_hdr.kernel_size, BOOT_IMAGE_V4_PAGE_SIZE);
		memcpy(boot_ramdisk_addr, (void*)(k_buff_img + boot_ramdisk_offset), boot_hdr.ramdisk_size);
		predata.ramdisk_size = vendor_ramdisk_size + boot_hdr.ramdisk_size;
	}

	if (init_boot_header.ramdisk_size > 0) {
		void  *addr = (void*)(uintptr_t)vendor_hdr.ramdisk_addr + predata.ramdisk_size;
		uint32_t offset = get_aligned(init_boot_header.header_size, BOOT_IMAGE_V4_PAGE_SIZE) +
			get_aligned(init_boot_header.kernel_size, BOOT_IMAGE_V4_PAGE_SIZE);
		memcpy(addr, (void*)(init_buff + offset), init_boot_header.ramdisk_size);
		predata.ramdisk_size += init_boot_header.ramdisk_size;
	}

	//load the bootconfig immediately following the generic ramdisk
	if (vendor_hdr.bootconfig_size) {
		UINT32 bootconfig_addr = vendor_hdr.ramdisk_addr + predata.ramdisk_size;
		UINT32 bootconfig_size = vendor_hdr.bootconfig_size;
		UINT32 bootconfig_offset = get_aligned(vendor_hdr.header_size, vendor_hdr.page_size) +
                                           get_aligned(vendor_hdr.vendor_ramdisk_size, vendor_hdr.page_size) +
                                           get_aligned(vendor_hdr.dtb_size, vendor_hdr.page_size) +
                                           get_aligned(vendor_hdr.vendor_ramdisk_table_size, vendor_hdr.page_size);
		memcpy((void*)(uintptr_t)bootconfig_addr, (void*)(vendor_buff + bootconfig_offset), vendor_hdr.bootconfig_size);

		//Add additional bootconfig parameters followed by the bootconfig trailer
		bootconfig_size += addBootConfigTrailer((UINT64)bootconfig_addr, vendor_hdr.bootconfig_size);
		predata.bootconfig_size = bootconfig_size;
		predata.bootconfig_start_addr = bootconfig_addr;
		predata.ramdisk_size += bootconfig_size;
	}

	return 0;
}

int load_android_image(int bootmode)
{
	int ret = -1;

	header_version = au->get_boot_version();

	if(header_version < BOOTIMG_HEADER_V3)
		ret = load_android_image_v2(bootmode);
	else if(header_version == BOOTIMG_HEADER_V3)
		ret = load_android_image_v3(bootmode);
	else if(header_version == BOOTIMG_HEADER_V4)
		ret = load_android_image_v4(bootmode);

	return ret;
}

#endif
// vim: noexpandtab
