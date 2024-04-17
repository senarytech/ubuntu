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

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "emmc_boot_ctrl.h"
#include <boot_devinfo.h>
#include "emmc_partition.h"

/* misc partition */
static unsigned int SYNA_SPACE_OFFSET_IN_MISC = 4 * 1024;
static misc_boot_ctrl_t boot;
static bool boot_ctrl_ready = false;
static const char* miscPartition = "misc";

/* fts partition */
#define FTS_DEVICE "/dev/fts"
#define FLASH_TS_MAX_KEY_SIZE 64
#define FLASH_TS_MAX_VAL_SIZE 2048
#define FLASH_TS_IO_MAGIC 0xFE
struct flash_ts_io_req {
  char key[FLASH_TS_MAX_KEY_SIZE];
  char val[FLASH_TS_MAX_VAL_SIZE];
};
#define FLASH_TS_IO_SET _IOW(FLASH_TS_IO_MAGIC, 0, struct flash_ts_io_req)
#define FLASH_TS_IO_GET _IOWR(FLASH_TS_IO_MAGIC, 1, struct flash_ts_io_req)
static const int kFtsActiveSlotPriority = 8;
static const int kFtsOtherSlotPriority = 3;

static void swap(uint8_t *a, uint8_t *b) {
   uint8_t t;

   t  = *b;
   *b = *a;
   *a = t;
}

bool readMiscPartition(void* p, size_t size, size_t offset) {
  char miscDevice[256];
  FILE* fd;

  if(!get_dev(miscPartition, miscDevice)) {
    printf("Failed to found device for partition %s\n", miscPartition);
    return false;
  }

  fd = fopen(miscDevice, "r");
  if (fd == NULL) {
      printf("failed to open %s: %s\n", miscDevice, strerror(errno));
      return false;
  }

  if (fseek(fd, offset, SEEK_SET) != 0) {
      printf("failed to lseek %s: %s\n", miscDevice,strerror(errno));
      fclose(fd);
      return false;
  }

  if (fread(p, sizeof(char), size, fd) != size) {
      printf("failed to read %s: %s\n", miscDevice,strerror(errno));
      fclose(fd);
      return false;
  }

  fclose(fd);
  return true;
}

bool writeMiscPartition(void* p, size_t size, size_t offset) {
  char miscDevice[256];
  FILE* fd;

  if(!get_dev(miscPartition, miscDevice)) {
    printf("Failed to found device for partition %s\n", miscPartition);
    return false;
  }

  fd = fopen(miscDevice, "w");
  if (fd == NULL) {
      printf("failed to open %s: %s\n", miscDevice, strerror(errno));
      return false;
  }
  if (fseek(fd, offset, SEEK_SET) != 0) {
      printf("failed to fseek %s: %s\n", miscDevice,strerror(errno));
      fclose(fd);
      return false;
  }
  if (fwrite(p, sizeof(char), size, fd) != size) {
      printf("failed to read %s: %s\n", miscDevice,strerror(errno));
      fclose(fd);
      return false;
  }

  fclose(fd);
  return true;
}

bool readBootInfo(void) {
  bool ret = readMiscPartition(&boot, sizeof(misc_boot_ctrl_t), SYNA_SPACE_OFFSET_IN_MISC);

  if (!ret) {
      printf("readMiscPartition() failed\n");
      return false;
  }

  if (boot.magic != BOOTCTRL_MAGIC || boot.version != MISC_BOOT_CONTROL_VERSION) {
      printf("misc boot info corrupted\n");
      return false;
  }

  return true;
}


bool updatBootInfo(void) {
  bool ret = writeMiscPartition(&boot, sizeof(misc_boot_ctrl_t), SYNA_SPACE_OFFSET_IN_MISC);
  return ret;
}

int getActiveBootSlot() {
  printf("%s\n", __func__);
  if(readBootInfo() == false)
      return -1;

  return (boot.slot_info[0].priority >= boot.slot_info[1].priority) ? 0 : 1;
}

int setActiveBootSlot(int slot) {
  if(boot_ctrl_ready == false) {
    if(readBootInfo() == false)
        return 0;
  }
  printf("%s, slot = %d\n", __func__, slot);

  CHECK_SLOT(slot);

  // curret slot Slot-A
  if((boot.slot_info[0].priority >= boot.slot_info[1].priority) && slot == 0)
    return 0;

  // curret slot Slot-B
  if((boot.slot_info[0].priority <= boot.slot_info[1].priority) && slot == 1)
    return 0;

  swap(&(boot.slot_info[0].priority), &(boot.slot_info[1].priority));

  if(updatBootInfo() == true)
    return 0;
  else return -1;

}

/* fts partition functions */

static int fts_get(const char* k, char* v, size_t size) {
  int fts_dev = open(FTS_DEVICE, O_RDWR);
  if (-1 == fts_dev) {
    printf("Failed to open %s\n", FTS_DEVICE);
    return EXIT_FAILURE;
  }
  struct flash_ts_io_req cmd;
  strncpy(cmd.key, k, FLASH_TS_MAX_KEY_SIZE - 1);
  if (ioctl(fts_dev, FLASH_TS_IO_GET, &cmd) == -1) {
    close(fts_dev);
    printf("Failed to read fts entry\n");
    return EXIT_FAILURE;
  }
  strncpy(v, cmd.val, size);
  close(fts_dev);
  return EXIT_SUCCESS;
}

static int fts_set(const char* k, const char* v) {
  int fts_dev = open(FTS_DEVICE, O_RDONLY);
  if (-1 == fts_dev) {
    printf("Failed to open %s\n", FTS_DEVICE);
    return EXIT_FAILURE;
  }
  struct flash_ts_io_req cmd;
  strncpy(cmd.key, k, FLASH_TS_MAX_KEY_SIZE - 1);
  strncpy(cmd.val, v, FLASH_TS_MAX_VAL_SIZE - 1);
  if (ioctl(fts_dev, FLASH_TS_IO_SET, &cmd) == -1) {
    close(fts_dev);
    printf("Failed to write fts entry\n");
    return EXIT_FAILURE;
  }
  close(fts_dev);
  return EXIT_SUCCESS;
}

int bootctrl_read_metadata(boot_ctrl_t* bctrl) {
  memset(bctrl, 0, sizeof(boot_ctrl_t));
  char meta_str[32] = {0};

  if(fts_get(BOOTCTRL_KEY, meta_str, 32) != EXIT_SUCCESS) {
    printf("Failed to get boot control metadata\n");
    return EXIT_FAILURE;
  }
  sscanf(meta_str, "%llx" , (uint64_t*)bctrl);
  if (bctrl->magic != BOOTCTRL_MAGIC) {
    printf("(ERROR) boot_ctrl metadata [%s]\n", meta_str);
    printf("(ERROR) boot_ctrl metadata corrupted\n");
    return -EIO;
  }
  return 0;
}

int bootctrl_write_metadata(boot_ctrl_t* bctrl) {
  char metadata_str[32] = {0};
  sprintf(metadata_str, "%016llx", *((uint64_t*)bctrl));
  return fts_set(BOOTCTRL_KEY, metadata_str);
}

int setFtsActiveBootSlot(int slot) {
  int ret;
  boot_ctrl_t metadata;
  slot_metadata_t* slotp;

  CHECK_SLOT(slot);
  ret = bootctrl_read_metadata(&metadata);
  if (ret < 0) {
      return ret;
  }
  slotp = &metadata.slot_info[slot];
  slotp->successful_boot = 0;
  slotp->priority = kFtsActiveSlotPriority;
  slotp->tries_remaining = 2;
  slotp = &metadata.slot_info[1 - slot];
  slotp->priority = kFtsOtherSlotPriority;
  ret = bootctrl_write_metadata(&metadata);
  if (ret < 0) {
      return ret;
  }
  return 0;
}

int getFtsActiveBootSlot() {
  int ret;
  boot_ctrl_t metadata;

  ret = bootctrl_read_metadata(&metadata);
  if (ret < 0) {
      return 0;
  }

  return (metadata.slot_info[0].priority >= metadata.slot_info[1].priority) ? 0 : 1;
}
