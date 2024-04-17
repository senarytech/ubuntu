/* tools/mkbootimg/bootimg.h
**
** Copyright 2007, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef _BOOT_IMAGE_H_
#define _BOOT_IMAGE_H_

typedef struct boot_img_hdr boot_img_hdr;
typedef struct boot_img_hdr_v3 boot_img_hdr_v3;
typedef struct vendor_boot_img_hdr_v3 vendor_boot_img_hdr_v3;
typedef struct boot_img_hdr_v4 boot_img_hdr_v4;
typedef struct vendor_boot_img_hdr_v4 vendor_boot_img_hdr_v4;
typedef struct vendor_ramdisk_table_entry_v4 vendor_ramdisk_table_entry_v4;


#define BOOT_MAGIC "ANDROID!"
#define BOOT_MAGIC_SIZE 8
#define BOOT_NAME_SIZE 16
#define BOOT_ARGS_SIZE 512
#define BOOT_EXTRA_ARGS_SIZE 1024

#define VENDOR_BOOT_MAGIC "VNDRBOOT"
#define VENDOR_BOOT_MAGIC_SIZE 8
#define VENDOR_BOOT_ARGS_SIZE 2048
#define VENDOR_BOOT_NAME_SIZE 16

#define VENDOR_RAMDISK_TYPE_NONE 0
#define VENDOR_RAMDISK_TYPE_PLATFORM 1
#define VENDOR_RAMDISK_TYPE_RECOVERY 2
#define VENDOR_RAMDISK_TYPE_DLKM 3
#define VENDOR_RAMDISK_NAME_SIZE 32
#define VENDOR_RAMDISK_TABLE_ENTRY_BOARD_ID_SIZE 16

#define BOOT_ARGS_TOTAL_SIZE (BOOT_ARGS_SIZE + BOOT_EXTRA_ARGS_SIZE + VENDOR_BOOT_ARGS_SIZE)

enum{
	BOOTIMG_HEADER_V0,
	BOOTIMG_HEADER_V1,
	BOOTIMG_HEADER_V2,
	BOOTIMG_HEADER_V3,
	BOOTIMG_HEADER_V4,
	BOOTIMG_HEADER_VERSION_MAX
};

struct boot_img_hdr
{
	unsigned char magic[BOOT_MAGIC_SIZE];

	unsigned kernel_size;  /* size in bytes */
	unsigned kernel_addr;  /* physical load addr */

	unsigned ramdisk_size; /* size in bytes */
	unsigned ramdisk_addr; /* physical load addr */

	unsigned second_size;  /* size in bytes */
	unsigned second_addr;  /* physical load addr */

	unsigned tags_addr;    /* physical addr for kernel tags */
	unsigned page_size;    /* flash page size we assume */
	/*
	 * version for boot image header
	 */
	unsigned header_version;

	/* operating system version and security patch level; for
	 * version "A.B.C" and patch level "Y-M-D":
	 * ver = A << 14 | B << 7 | C         (7 bits for each of A, B, C)
	 * lvl = ((Y - 2000) & 127) << 4 | M  (7 bits for Y, 4 bits for M)
	 * os_version = ver << 11 | lvl */
	uint32_t os_version;

	unsigned char name[BOOT_NAME_SIZE]; /* asciiz product name */

	unsigned char cmdline[BOOT_ARGS_SIZE];

	unsigned id[8]; /* timestamp / checksum / sha1 / etc */

	unsigned char extra_cmdline[BOOT_EXTRA_ARGS_SIZE];
	unsigned recovery_dtbo_size;	/* size of recovery dtbo image */
	unsigned recovery_dtbo_offset[2];	/* physical load addr */
	unsigned header_size;	/* size of boot image header in bytes */
	unsigned dtb_size; /* size in bytes for DTB image */
	unsigned dtb_addr[2]; /* physical load address for DTB image */
};

/*
** You may obtain the layout of boot.img from
**
**     https://github.com/cfig/Android_boot_image_editor/blob/dev/doc/layout.md#1-header-part
**
*/

/*data part*/
/*
**+-----------------------------------------------------------+    --> pagesize
**|<kernel>                        |   kernel length          |
**|--------------------------------+--------------------------|
**|<padding>                       | min(n * page_size - len) |
**+-----------------------------------------------------------+
**
**+-----------------------------------------------------------+
**|<ramdisk>                       |   ramdisk length         |
**|--------------------------------+--------------------------|
**|<padding>                       | min(n * page_size - len) |
**+-----------------------------------------------------------+
**
**+-----------------------------------------------------------+
**|<second bootloader>             | second bootloader length |
**|--------------------------------+--------------------------|
**|<padding>                       | min(n * page_size - len) |
**+-----------------------------------------------------------+
**
**+-----------------------------------------------------------+
**|<recovery dtbo>          [v1]   | recovery dtbo length     |
**|--------------------------------+--------------------------|
**|<padding>                [v1]   | min(n * page_size - len) |
**+-----------------------------------------------------------+
**
**+-----------------------------------------------------------+
**|<dtb>                    [v2]   | dtb length               |
**|--------------------------------+--------------------------|
**|<padding>                [v2]   | min(n * page_size - len) |
**+-----------------------------------------------------------+    --> end of data part
**

**l, m, n, o and p are calculated as follows:
**l = (size of kernel + page size - 1) / page size
**m = (size of ramdisk + page size - 1) / page size
**n = (size of second stage + page size - 1) / page size
**o = (size of recovery DTBO + page size - 1) / page size
**p = (size of DTB + page size - 1) / page size
**
** 0. all entities are page_size aligned in flash
** 1. kernel and ramdisk are required (size != 0)
** 2. second is optional (second_size == 0 -> no second)
** 3. load each element (kernel, ramdisk, second) at
**    the specified physical address (kernel_addr, etc)
** 4. prepare tags at tag_addr.  kernel_args[] is
**    appended to the kernel commandline in the tags.
** 5. r0 = 0, r1 = MACHINE_TYPE, r2 = tags_addr
** 6. if second_size != 0: jump to second_addr
**    else: jump to kernel_addr
*/

#define BOOT_IMAGE_V3_PAGE_SIZE 4096
#define BOOT_IMAGE_V4_PAGE_SIZE 4096
#define VENDOR_BOOT_IMAGE_V3_PAGE_SIZE 4096
#define VENDOR_BOOT_IMAGE_V4_PAGE_SIZE 4096

/*header part*/
/*
**          item                        size in bytes             position
**+-----------------------------------------------------------+    --> 0
**|<MAGIC HEADER>                  |     8                    |
**|--------------------------------+--------------------------|    --> 8
**|<kernel length>                 |     4                    |
**|--------------------------------+--------------------------|    --> 12
**|<kernel offset>                 |     4                    |
**|--------------------------------+--------------------------|    --> 16 (0x10)
**|<ramdisk length>                |     4                    |
**|--------------------------------+--------------------------|    --> 20
**|<ramdisk offset>                |     4                    |
**|--------------------------------+--------------------------|    --> 24
**|<second bootloader length>      |     4                    |
**|--------------------------------+--------------------------|    --> 28
**|<second bootloader offset>      |     4                    |
**|--------------------------------+--------------------------|    --> 32 (0x20)
**|<tags offset>                   |     4                    |
**|--------------------------------+--------------------------|    --> 36
**|<page size>                     |     4                    |
**|--------------------------------+--------------------------|    --> 40
**|<header version>                |     4 (value in [0,1,2]) |
**|--------------------------------+--------------------------|    --> 44
**|<os version & os patch level>   |     4                    |
**|--------------------------------+--------------------------|    --> 48 (0x30)
**|<board name>                    |     16                   |
**|--------------------------------+--------------------------|    --> 64 (0x40)
**|<cmdline part 1>                |     512                  |
**|--------------------------------+--------------------------|    --> 576 (0x240)
**|<hash digest>                   |     32                   |
**|--------------------------------+--------------------------|    --> 608 (0x260)
**|<cmdline part 2>                |     1024                 |
**|--------------------------------+--------------------------|    --> 1632 (0x660)
**|<dtbo length>            [v1]   |     4                    |
**|--------------------------------+--------------------------|    --> 1636
**|<dtbo offset>            [v1]   |     8                    |
**|--------------------------------+--------------------------|    --> 1644
**|<header size>            [v1]   |     4 (v1: value=1648)   |
**|                                |       (v2: value=1660)   |
**|--------------------------------+--------------------------|    --> 1648 (0x670)
**|<dtb  length>            [v2]   |     4                    |
**|--------------------------------+--------------------------|    --> 1652
**|<dtb  offset>            [v2]   |     8                    |
**|--------------------------------+--------------------------|    --> 1660 (0x67c)
**|<padding>                       | min(n * page_size        |
**|                                |           - header_size) |
**+--------------------------------+--------------------------+    --> pagesize
*/

/* When the boot image header has a version of 3, the structure of the boot
 * image is as follows:
 *
 * +---------------------+
 * | boot header         | 4096 bytes
 * +---------------------+
 * | kernel              | m pages
 * +---------------------+
 * | ramdisk             | n pages
 * +---------------------+
 *
 * m = (kernel_size + 4096 - 1) / 4096
 * n = (ramdisk_size + 4096 - 1) / 4096
 *
 * Note that in version 3 of the boot image header, page size is fixed at 4096 bytes.
 *
 * The structure of the vendor boot image (introduced with version 3 and
 * required to be present when a v3 boot image is used) is as follows:
 *
 * +---------------------+
 * | vendor boot header  | o pages
 * +---------------------+
 * | vendor ramdisk      | p pages
 * +---------------------+
 * | dtb                 | q pages
 * +---------------------+
 * o = (2112 + page_size - 1) / page_size
 * p = (vendor_ramdisk_size + page_size - 1) / page_size
 * q = (dtb_size + page_size - 1) / page_size
 *
 * 0. all entities in the boot image are 4096-byte aligned in flash, all
 *    entities in the vendor boot image are page_size (determined by the vendor
 *    and specified in the vendor boot image header) aligned in flash
 * 1. kernel, ramdisk, vendor ramdisk, and DTB are required (size != 0)
 * 2. load the kernel and DTB at the specified physical address (kernel_addr,
 *    dtb_addr)
 * 3. load the vendor ramdisk at ramdisk_addr
 * 4. load the generic ramdisk immediately following the vendor ramdisk in
 *    memory
 * 5. set up registers for kernel entry as required by your architecture
 * 6. if the platform has a second stage bootloader jump to it (must be
 *    contained outside boot and vendor boot partitions), otherwise
 *    jump to kernel_addr
 */
struct boot_img_hdr_v3 {
    // Must be BOOT_MAGIC.
    uint8_t magic[BOOT_MAGIC_SIZE];
    uint32_t kernel_size; /* size in bytes */
    uint32_t ramdisk_size; /* size in bytes */
    // Operating system version and security patch level.
    // For version "A.B.C" and patch level "Y-M-D":
    //   (7 bits for each of A, B, C; 7 bits for (Y-2000), 4 bits for M)
    //   os_version = A[31:25] B[24:18] C[17:11] (Y-2000)[10:4] M[3:0]
    uint32_t os_version;

    uint32_t header_size;
    uint32_t reserved[4];
    // Version of the boot image header.
    uint32_t header_version;
    uint8_t cmdline[BOOT_ARGS_SIZE + BOOT_EXTRA_ARGS_SIZE];
};

struct vendor_boot_img_hdr_v3 {
    // Must be VENDOR_BOOT_MAGIC.
    uint8_t magic[VENDOR_BOOT_MAGIC_SIZE];
    // Version of the vendor boot image header.
    uint32_t header_version;
    uint32_t page_size; /* flash page size we assume */
    uint32_t kernel_addr; /* physical load addr */
    uint32_t ramdisk_addr; /* physical load addr */
    uint32_t vendor_ramdisk_size; /* size in bytes */
    uint8_t cmdline[VENDOR_BOOT_ARGS_SIZE];
    uint32_t tags_addr; /* physical addr for kernel tags (if required) */
    uint8_t name[VENDOR_BOOT_NAME_SIZE]; /* asciiz product name */
    uint32_t header_size;
    uint32_t dtb_size; /* size in bytes for DTB image */
    uint64_t dtb_addr; /* physical load address for DTB image */
};

/* When the boot image header has a version of 4, the structure of the boot
 * image is as follows:
 *
 * +---------------------+
 * | boot header         | 4096 bytes
 * +---------------------+
 * | kernel              | m pages
 * +---------------------+
 * | ramdisk             | n pages
 * +---------------------+
 * | boot signature      | g pages
 * +---------------------+
 *
 * m = (kernel_size + 4096 - 1) / 4096
 * n = (ramdisk_size + 4096 - 1) / 4096
 * g = (signature_size + 4096 - 1) / 4096
 *
 * Note that in version 4 of the boot image header, page size is fixed at 4096
 * bytes.
 *
 * The structure of the vendor boot image version 4, which is required to be
 * present when a version 4 boot image is used, is as follows:
 *
 * +------------------------+
 * | vendor boot header     | o pages
 * +------------------------+
 * | vendor ramdisk section | p pages
 * +------------------------+
 * | dtb                    | q pages
 * +------------------------+
 * | vendor ramdisk table   | r pages
 * +------------------------+
 * | bootconfig             | s pages
 * +------------------------+
 *
 * o = (2128 + page_size - 1) / page_size
 * p = (vendor_ramdisk_size + page_size - 1) / page_size
 * q = (dtb_size + page_size - 1) / page_size
 * r = (vendor_ramdisk_table_size + page_size - 1) / page_size
 * s = (vendor_bootconfig_size + page_size - 1) / page_size
 *
 * Note that in version 4 of the vendor boot image, multiple vendor ramdisks can
 * be included in the vendor boot image. The bootloader can select a subset of
 * ramdisks to load at runtime. To help the bootloader select the ramdisks, each
 * ramdisk is tagged with a type tag and a set of hardware identifiers
 * describing the board, soc or platform that this ramdisk is intended for.
 *
 * The vendor ramdisk section is consist of multiple ramdisk images concatenated
 * one after another, and vendor_ramdisk_size is the size of the section, which
 * is the total size of all the ramdisks included in the vendor boot image.
 *
 * The vendor ramdisk table holds the size, offset, type, name and hardware
 * identifiers of each ramdisk. The type field denotes the type of its content.
 * The vendor ramdisk names are unique. The hardware identifiers are specified
 * in the board_id field in each table entry. The board_id field is consist of a
 * vector of unsigned integer words, and the encoding scheme is defined by the
 * hardware vendor.
 *
 * For the different type of ramdisks, there are:
 *    - VENDOR_RAMDISK_TYPE_NONE indicates the value is unspecified.
 *    - VENDOR_RAMDISK_TYPE_PLATFORM ramdisks contain platform specific bits, so
 *      the bootloader should always load these into memory.
 *    - VENDOR_RAMDISK_TYPE_RECOVERY ramdisks contain recovery resources, so
 *      the bootloader should load these when booting into recovery.
 *    - VENDOR_RAMDISK_TYPE_DLKM ramdisks contain dynamic loadable kernel
 *      modules.
 *
 * Version 4 of the vendor boot image also adds a bootconfig section to the end
 * of the image. This section contains Boot Configuration parameters known at
 * build time. The bootloader is responsible for placing this section directly
 * after the generic ramdisk, followed by the bootconfig trailer, before
 * entering the kernel.
 *
 * 0. all entities in the boot image are 4096-byte aligned in flash, all
 *    entities in the vendor boot image are page_size (determined by the vendor
 *    and specified in the vendor boot image header) aligned in flash
 * 1. kernel, ramdisk, and DTB are required (size != 0)
 * 2. load the kernel and DTB at the specified physical address (kernel_addr,
 *    dtb_addr)
 * 3. load the vendor ramdisks at ramdisk_addr
 * 4. load the generic ramdisk immediately following the vendor ramdisk in
 *    memory
 * 5. load the bootconfig immediately following the generic ramdisk. Add
 *    additional bootconfig parameters followed by the bootconfig trailer.
 * 6. set up registers for kernel entry as required by your architecture
 * 7. if the platform has a second stage bootloader jump to it (must be
 *    contained outside boot and vendor boot partitions), otherwise
 *    jump to kernel_addr
 */
struct boot_img_hdr_v4  {
    // Must be BOOT_MAGIC.
    uint8_t magic[BOOT_MAGIC_SIZE];
    uint32_t kernel_size; /* size in bytes */
    uint32_t ramdisk_size; /* size in bytes */
    // Operating system version and security patch level.
    // For version "A.B.C" and patch level "Y-M-D":
    //   (7 bits for each of A, B, C; 7 bits for (Y-2000), 4 bits for M)
    //   os_version = A[31:25] B[24:18] C[17:11] (Y-2000)[10:4] M[3:0]
    uint32_t os_version;

    uint32_t header_size;
    uint32_t reserved[4];
    // Version of the boot image header.
    uint32_t header_version;
    uint8_t cmdline[BOOT_ARGS_SIZE + BOOT_EXTRA_ARGS_SIZE];
    uint32_t signature_size; /* size in bytes */
};

struct vendor_boot_img_hdr_v4 {
    // Must be VENDOR_BOOT_MAGIC.
    uint8_t magic[VENDOR_BOOT_MAGIC_SIZE];
    // Version of the vendor boot image header.
    uint32_t header_version;
    uint32_t page_size; /* flash page size we assume */
    uint32_t kernel_addr; /* physical load addr */
    uint32_t ramdisk_addr; /* physical load addr */
    uint32_t vendor_ramdisk_size; /* size in bytes */
    uint8_t cmdline[VENDOR_BOOT_ARGS_SIZE];
    uint32_t tags_addr; /* physical addr for kernel tags (if required) */
    uint8_t name[VENDOR_BOOT_NAME_SIZE]; /* asciiz product name */
    uint32_t header_size;
    uint32_t dtb_size; /* size in bytes for DTB image */
    uint64_t dtb_addr; /* physical load address for DTB image */

    uint32_t vendor_ramdisk_table_size; /* size in bytes for the vendor ramdisk table */
    uint32_t vendor_ramdisk_table_entry_num; /* number of entries in the vendor ramdisk table */
    uint32_t vendor_ramdisk_table_entry_size; /* size in bytes for a vendor ramdisk table entry */
    uint32_t bootconfig_size; /* size in bytes for the bootconfig section */
};

struct vendor_ramdisk_table_entry_v4 {
    uint32_t ramdisk_size; /* size in bytes for the ramdisk image */
    uint32_t ramdisk_offset; /* offset to the ramdisk image in vendor ramdisk section */
    uint32_t ramdisk_type; /* type of the ramdisk */
    uint8_t ramdisk_name[VENDOR_RAMDISK_NAME_SIZE]; /* asciiz ramdisk name */

    // Hardware identifiers describing the board, soc or platform which this
    // ramdisk is intended to be loaded on.
    uint32_t board_id[VENDOR_RAMDISK_TABLE_ENTRY_BOARD_ID_SIZE];
};

#endif
