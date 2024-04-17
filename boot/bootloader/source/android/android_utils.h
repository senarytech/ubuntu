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
#ifndef ANDROID_UTILS_H
#define ANDROID_UTILS_H

#include <avb_ops.h>
#include <boot_mode.h>
#include <com_type.h>
#include <debug.h>
#include <libavb.h>
#include <load_gpt.h>
#include <string.h>
#include <rpmb.h>
#include <../android/bootimg.h>

#define BLK_LEN 512
#define RPMB_AVB_MAGIC 0x55595a59

typedef enum part_type {
    BOOT,
    VENDOR_BOOT,
    INIT_BOOT,
    DTBO,
} part_type_t;

typedef struct {
    const char * (*part2gpt)(enum part_type part);
    uint32_t (*round_to_multiple)(uint32_t size, uint32_t page);
    long long (*read_partition)(const char *name, unsigned int offset, unsigned int sz, unsigned char *buf);
    int (*get_boot_version)(void);
    int (*get_image_size)(const char *gpt_part);
    AvbIOResult (*get_part_size)(const char *partition, uint64_t *out_size_num_bytes);
    AvbIOResult (*parse_boot_header)(boot_img_hdr_v4 *boot_header, part_type_t partType);
    AvbIOResult (*parse_vndboot_header)(vendor_boot_img_hdr_v4 *header);
} android_utils_t;

typedef struct {
    uint32_t magic;
    uint8_t version;
    uint8_t unlock;
    uint64_t values[16];
    uint8_t reserved[122];
} __attribute__((packed)) rpmb_avb_t;

typedef struct {
    bool (*init)();
    bool (*read)(rpmb_avb_t* info);
    bool (*write)(const rpmb_avb_t* info);
} rpmb_avb_ops_t;

extern android_utils_t *au;
extern rpmb_avb_ops_t* gRpmbAvbOps;
extern uint32_t get_rpmb_avb_offset(void);

#endif /* !ANDROID_UTILS_H */
