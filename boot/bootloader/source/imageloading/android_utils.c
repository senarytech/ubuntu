/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2022 Synaptics Incorporated. All rights reserved.
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
#include "android_utils.h"
#include "bootloader_header.h"

extern unsigned gOs_version, gOs_lvl;

#define TEST_AND_RETURN(expr)  \
    do {                       \
        int res = (expr);\
        if (res < 0) return res;  \
    } while (0)

static const char *part2gpt(part_type_t part) {
    switch (part) {
        case BOOT:
#ifdef CONFIG_AB_BOOTFLOW
            return (get_abmode() == BOOTSEL_A) ? KERNEL_A_NAME : KERNEL_B_NAME;
#else
            return (get_bootmode() == BOOTMODE_NORMAL) ? KERNEL_NAME : RECOVERY_NAME;
#endif
        case VENDOR_BOOT:
#ifdef CONFIG_AB_BOOTFLOW
            return (get_abmode() == BOOTSEL_A) ? VENDOR_BOOT_A_NAME : VENDOR_BOOT_B_NAME;
#else
            return VENDOR_BOOT_NAME;
#endif
        case INIT_BOOT:
#ifdef CONFIG_AB_BOOTFLOW
            return (get_abmode() == BOOTSEL_A) ? INIT_BOOT_A_NAME : INIT_BOOT_B_NAME;
#else
            return INIT_BOOT_NAME;
#endif
        case DTBO:
#ifdef CONFIG_AB_BOOTFLOW
            return (get_abmode() == BOOTSEL_A) ? DTBO_A_NAME : DTBO_B_NAME;
#else
            return DTBO_NAME;
#endif
        default:
            return NULL;
    }
}

static uint32_t round_to_multiple(uint32_t size, uint32_t page) {
    uint32_t remainder = size % page;
    if (remainder == 0) {
        return size;
    } else {
        return size + page - remainder;
    }
}

void dumpHex(const char *buf, uint32_t sz) {
    INFO("HEX DUMP\n");
    for (int i = 0; i < sz; i++) {
        INFO("%02x ", buf[i]);
        if ((i + 1) % 16 == 0) INFO("\n");
    }
    INFO("\n");
}

static AvbIOResult get_partition_size(const char *partition, uint64_t *out_size_num_bytes) {
    int ret = find_partition(partition);
    if (ret < 0) {
        INFO("%s(): no partition [%s] in gpt\n", __FUNCTION__, partition);
        return AVB_IO_RESULT_ERROR_NO_SUCH_PARTITION;
    }
    struct gpt_ent part;
    fetch_partition_info(ret, &part);
    uint64_t sz = (part.ent_lba_end - part.ent_lba_start + 1) * BLK_LEN;
    *out_size_num_bytes = sz;
    INFO("%s(): %s sz=%ld\n", __FUNCTION__, partition, sz);
    return AVB_IO_RESULT_OK;
}

static int get_boot_image_version(void) {
    int8_t header_page[512] = {0};
    TEST_AND_RETURN(read_image_by_ptname(part2gpt(BOOT),
                                         512, (unsigned char *) header_page));
    struct boot_img_hdr_v4 *header = (boot_img_hdr_v4 *) header_page;
    NOTICE("Boot Image Version is %d\n", header->header_version);
    if (strncmp((const char *) header->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE) != 0) {
        ERR("ERROR: Invalid boot header!\n");
        return -1;
    }
    switch (header->header_version) {
        case BOOTIMG_HEADER_V0:
        case BOOTIMG_HEADER_V1:
        case BOOTIMG_HEADER_V2:
            gOs_version = ((struct boot_img_hdr *) header_page)->os_version >> 11;
        case BOOTIMG_HEADER_V3:
        case BOOTIMG_HEADER_V4:
            gOs_version = header->os_version >> 11;
            break;
        default:
            return -1;
    }
    return (int) header->header_version;
}

static long long read_partition(const char *name, unsigned int offset, unsigned int sz, unsigned char *buf) {
    int pt_index = find_partition(name);
    if (pt_index < 0) {
        return -AVB_IO_RESULT_ERROR_NO_SUCH_PARTITION;
    }
    struct gpt_ent gpte;
    fetch_partition_info(pt_index, &gpte);
    return read_image_from_offset(&gpte, offset, sz, buf);
}

static int get_avb_image_size(const char *gpt_part) {
    uint64_t sz = 0;
    int8_t header_page[512 + 64] = {0};
    if (AVB_IO_RESULT_OK != get_partition_size(gpt_part, &sz)) {
        INFO("can not find sz of %s\n", gpt_part);
        return -AVB_IO_RESULT_ERROR_NO_SUCH_PARTITION;
    }
    long pt = round_to_multiple((long) header_page, 64);
    TEST_AND_RETURN(read_partition(gpt_part, sz - 512, 512, (unsigned char *) pt));
    struct AvbFooter *f = (struct AvbFooter *) (pt + 512 - 64);
    uint8_t magic[] = {0x41, 0x56, 0x42, 0x66};
    if (0 != memcmp(f->magic, magic, 4)) {
        ERR("fail to find avb footer\n");
        return -1;
    }
    return (int) avb_be64toh(f->original_image_size);
}

AvbIOResult parse_boot_header(boot_img_hdr_v4 *boot_header, part_type_t partType) {
    AvbIOResult ret = AVB_IO_RESULT_ERROR_IO;
    uint32_t sz = sizeof(boot_img_hdr_v4);
    unsigned char *const header_page = malloc_ion_cacheable(2048);
    if (read_partition(part2gpt(partType), 0, sz, header_page) < 0) {
        goto end;
    }
    memcpy(boot_header, (uint8_t *) header_page, sizeof(boot_img_hdr_v4));
    if (0 != strncmp((const char *) boot_header->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)) {
        ERR("ERROR: Invalid boot header!\n");
        goto end;
    }
    ret = AVB_IO_RESULT_OK;

    end:
    free_ion_cacheable(header_page);
    return ret;
}

static AvbIOResult parse_vndboot_header(vendor_boot_img_hdr_v4 *header) {
    AvbIOResult ret = AVB_IO_RESULT_ERROR_IO;
    uint32_t sz = sizeof(vendor_boot_img_hdr_v4);
    unsigned char *const header_page = malloc_ion_cacheable(sz);
    long long rc = read_partition(part2gpt(VENDOR_BOOT), 0, sz, header_page);
    if (rc < 0) {
        if (rc == -AVB_IO_RESULT_ERROR_NO_SUCH_PARTITION) {
            ret = AVB_IO_RESULT_ERROR_NO_SUCH_PARTITION;
        }
        goto end;
    }
    memcpy(header, (uint8_t *) header_page, sizeof(vendor_boot_img_hdr_v4));
    if (0 != strncmp((const char *) header->magic, VENDOR_BOOT_MAGIC, VENDOR_BOOT_MAGIC_SIZE)) {
        ERR("ERROR: Invalid vendor boot header!\n");
        return -1;
    }
    ret = AVB_IO_RESULT_OK;

    end:
    free_ion_cacheable(header_page);
    return ret;
}

static bool read_rpmb_avb(rpmb_avb_t* info) {
    int ret = rpmb_read_blocks(get_rpmb_avb_offset(), 1, info, sizeof(rpmb_avb_t));
    if (0 != ret) {
        dbg_printf(PRN_ERR, "failed to read rpmb avb info\n");
        return false;
    }
    return true;
}

static bool write_rpmb_avb(const rpmb_avb_t* info) {
    int ret = rpmb_write_blocks(get_rpmb_avb_offset(), 1, (void*)info, sizeof(rpmb_avb_t));
    if (ret < 0) {
        dbg_printf(PRN_ERR, "INFO: write RPMB data fail!\n");
        return false;
    } else {
        dbg_printf(PRN_INFO, "INFO: write RPMB data OK, counter=0x%x\n", rpmb_get_write_count());
        return true;
    }
}

static bool init_rpmb_avb() {
    if (sizeof(rpmb_avb_t) != 256) {
        dbg_printf(PRN_ERR, "RPMB AVB sanity check failed\n");
        return false;
    }
    if (get_rpmb_avb_offset() == 0) {
        dbg_printf(PRN_ERR, "RPMB AVB offset invalid\n");
        return false;
    }
    rpmb_avb_t info = { 0 };
    if (!read_rpmb_avb(&info)) {
        dbg_printf(PRN_ERR, "RPMB AVB read failed\n");
        return false;
    }
    if (info.magic != RPMB_AVB_MAGIC) {
        rpmb_avb_t init_info = { 0 };
        init_info.magic = RPMB_AVB_MAGIC;
        return write_rpmb_avb(&init_info);
    }
    return true;
}

static rpmb_avb_ops_t _rpmb_avb_ops = {
        init_rpmb_avb,
        read_rpmb_avb,
        write_rpmb_avb,
};

rpmb_avb_ops_t* gRpmbAvbOps = &_rpmb_avb_ops;

static android_utils_t _android_utils = {
        part2gpt,
        round_to_multiple,
        read_partition,
        get_boot_image_version,
        get_avb_image_size,
        get_partition_size,
        parse_boot_header,
        parse_vndboot_header,
};

android_utils_t *au = &_android_utils;
