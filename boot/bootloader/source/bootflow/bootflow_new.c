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
	int ret= 0;
	int boot_mode = get_bootmode();

	init_linux_addr();

	dbg_printf(PRN_NOTICE, "load boot %s image.\n", boot_mode ? "recovery": "normal");

	ret = load_android_image(boot_mode);
	if((ret != 0)&&(BOOTMODE_NORMAL == boot_mode)) {
		dbg_printf(PRN_RES, "Invalid normal boot image! Boot recovery image...\n");
		//write_android_bootloader_message("boot-recovery", "", "recovery\n--show_text\n");
		set_bootmode(BOOTMODE_RECOVERY);
		return BOOTLOADER_NORMAL_RESET_STAGE;
	} else if((ret != 0)&&(BOOTMODE_RECOVERY == boot_mode)) {
		dbg_printf(PRN_RES, "Invalid recovery boot image! Boot normal image...\n");
		/* Note: we don't clear the recovery flags, we just let device
		*  to enter recovery mode if recovery mode is avaliable.
		*  write_android_bootloader_message("", "", "");
		*/
		set_bootmode(BOOTMODE_NORMAL);
		return BOOTLOADER_NORMAL_RESET_STAGE;
	}

	return ret;
}


