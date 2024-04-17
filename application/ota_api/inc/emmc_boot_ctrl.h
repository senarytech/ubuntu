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

#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>

#define BOOTCTRL_MAGIC             0x42424100
#define BOOTCTRL_SUFFIX_A          "_a"
#define BOOTCTRL_SUFFIX_B          "_b"
#define BOOT_CONTROL_VERSION       1
#define MISC_BOOT_CONTROL_VERSION  2
#define BOOTCTRL_KEY "bootctrl.metadata"


#define CHECK_SLOT(slot)                                             \
  if (slot != 0 && slot != 1) {                                      \
      printf("%s, Wrong slot value %d", slot);                       \
      return false;                                                  \
  }

/**
 *  get the A/B slot that is selected to boot in misc partition
 *
 *  @Retuen -1 : falied to get slot
 *           0 : slot A
 *           1 : slot B
 */
int getActiveBootSlot();

/**
 *  set the A/B slot to boot in misc partition
 *  @param slot 0 : slot A
 *              1 : slot B
 *  @Return:  0 : Successfully to set slot
 *           -1 : falied to set slot
 */
int setActiveBootSlot(int slot);

/**
 *  get the A/B slot that is selected to boot in fts partition
 *
 *  @Retuen -1 : falied to get slot
 *           0 : slot A
 *           1 : slot B
 */
int getFtsActiveBootSlot();

/**
 *  set the A/B slot to boot in fts partition
 *  @param slot 0 : slot A
 *              1 : slot B
 *  @Return:  0 : Successfully to set slot
 *           -1 : falied to set slot
 */
int setFtsActiveBootSlot(int slot);
