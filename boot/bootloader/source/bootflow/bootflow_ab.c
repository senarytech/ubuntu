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
#include "com_type.h"
#include "io.h"
#include "string.h"
#include "debug.h"

#include "boot_mode.h"
#include "bootloader.h"
#include "bootloader_header.h"

#include "bootflow_common.c"

void get_reset_msg(void)
{
}

int Image_Load_And_Verify(void)
{
	int ret= -1;
	int boot_mode = get_bootmode();
	int ab_mode = get_abmode();

	init_linux_addr();

	if((ab_mode != BOOTSEL_A) && (ab_mode != BOOTSEL_B)) {
		dbg_printf(PRN_ERR, "No Bootable slot found for image loading, spinning...!!\n");
		return BOOTLOADER_NORMAL_RESET_STAGE;
	}

retry_image_load:
	ret = load_android_image(boot_mode);
	if(ret != 0) {
		if(try_abmode(ab_mode)) {
			dbg_printf(PRN_ERR, "Invalid slot_%s boot image! reset to backup image...\n", slot_name(ab_mode));
			//set_bootmode(BOOTMODE_RECOVERY);
			return BOOTLOADER_NORMAL_RESET_STAGE;
		}
		else {
			dbg_printf(PRN_ERR, "Failed to load slot_%s boot image! Retry again...\n", slot_name(ab_mode));
			goto retry_image_load;
		}
	}

	return ret;
}


