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

 /*FIXME: this file need to be refine continuely*/
#include "com_type.h"
#include "io.h"
#include "string.h"
#include "debug.h"

#include "flash_adaptor.h"
#include "load_gpt.h"
#include "bootloader_header.h"

#include "vb_state.h"

#ifdef CONFIG_ANDROID_BOOTFLOW
#include "android_utils.h"
#endif

#ifdef CONFIG_FASTLOGO
//FIXME: Enable UI for verify boot as vpp driver is ready although not fully ready
#define ENABLE_UI 1
#endif

#if ENABLE_UI
extern int message_box(int type, unsigned char state, char * text);
extern void show_warning_message(int color);
#endif

typedef struct{
	unsigned int magic;
	unsigned int reserve1[6];
	unsigned int lock_flag;
	unsigned int lock_critical_flag;
	unsigned int reserve2[119];  /* round to 512 bytes */
}oem_lock_flag;

typedef struct{
	unsigned char reserve[510];
	unsigned char lock_critical_flag;
	unsigned char lock_flag;
}frp_lock_flag;

#define FASTBOOT_MAGIC		(0xaa5577ff)
#define OEM_UNLOCK_FLAG		(0xa55a)
#define FRP_UNLOCK_FLAG		(0xa5)

static VB_DEVICE_STATE dev_state = DEV_LOCK;
static int verify_state = GREEN;
static int finish_verify = 0x0;

static VB_DEVICE_STATE read_device_state(void)
{
#ifdef CONFIG_ANDROID_BOOTFLOW
    rpmb_avb_t rpmb_info;
    if (!gRpmbAvbOps->read(&rpmb_info)) {
        ERR("RPMB AVB read failed\n");
        return DEV_LOCK;
    }
    if (RPMB_AVB_MAGIC == rpmb_info.magic && FRP_UNLOCK_FLAG == rpmb_info.unlock) {
        return DEV_UNLOCK;
    } else {
        return DEV_LOCK;
    }
#endif
	int index = 0;
	unsigned char * const k_buff = malloc_ion_cacheable(64 << 10); //alloc 64KB
	struct gpt_ent gpt_info;
	long long flag_start = 0x0;

	index = find_partition(DEVINFO_NAME);
	if(index == PARTITION_NOT_EXIST)
		index = find_partition(FRP_NAME);
	if(index != PARTITION_NOT_EXIST)
	{
		frp_lock_flag * buff = (frp_lock_flag *)(k_buff + 0x0);

		fetch_partition_info(index, &gpt_info);

		flag_start = (gpt_info.ent_lba_end) * 0x200;
		read_flash(flag_start, 0x200, (uint8_t *)buff); //SDIO_BLK_SIZE, get_block_size()

		//it's the unlock only when unlock flag is set
		if(FRP_UNLOCK_FLAG == (buff->lock_flag))
			return DEV_UNLOCK;
	}
	return DEV_LOCK;
}

static VB_RET vb_state_init(void)
{
	//1: read device
	dev_state = read_device_state();

	if(dev_state == DEV_UNLOCK) {
		verify_state = ORANGE;
		finish_verify = VB_FINISH;

#if ENABLE_UI
		show_warning_message(verify_state);
		//FIXME: show dialogue "boot anyways?" yes, boot; no, recovery
		if(0x1 == message_box(0x1, verify_state, "Device is unLocked,boot anyways?")) {//ORANGE Prompt !
			//ORANGE STATE: Finish boot
			verify_state = ORANGE;
			finish_verify = VB_FINISH;
			dbg_printf(PRN_RES, "Press YES\n");
			show_warning_message(STOP_DISPLAY);
		} else {
			//Boot State: ORANGE, Boot Flow: Recovery !
			verify_state = ORANGE;
			finish_verify = VB_REBOOT;
			dbg_printf(PRN_RES, "Press NO\n");
		}
#else
		dbg_printf(PRN_RES, "Device is unLocked and Boot state is ORANGE!\n");
#endif
	} else {
		verify_state = GREEN;
		finish_verify = VB_CONTINUE;
		//NOTICE: Device is Locked, Continue the Booting Verify Flow !
		dbg_printf(PRN_RES, "Device is Locked and Boot state is GREEN!\n");
	}
	return finish_verify;
}

static VB_RET vb_state_oemk_fail(void)
{
	verify_state = YELLOW;
	finish_verify = VB_CONTINUE;
	dbg_printf(PRN_RES,"OEM key verify failed and Boot state is YELLOW!\n");

	return finish_verify;
}

static VB_RET vb_state_oemk_succ(void)
{
	verify_state = GREEN;
	finish_verify = VB_FINISH;

	return finish_verify;
}

static VB_RET vb_state_userk_fail(void)
{
	//Boot State: RED, Boot Flow: Recovery!
	verify_state = RED;
	finish_verify = VB_REBOOT;

#if ENABLE_UI
	show_warning_message(verify_state);
#endif

	dbg_printf(PRN_RES, "Device failed verification,RED state will enter recovery mode!\n");

	return finish_verify;
}

static VB_RET vb_state_userk_succ(void)
{
	verify_state = YELLOW;
	finish_verify = VB_FINISH;

#if ENABLE_UI
	show_warning_message(verify_state);
	if(0x1 == message_box(0x1, verify_state, "Device loaded different system,boot anyways?")) {
		//YELLOW STATE: Finish boot
		verify_state = YELLOW;
		finish_verify = VB_FINISH;
		dbg_printf(PRN_RES, "Press YES\n");
		show_warning_message(STOP_DISPLAY);
	} else {
		//return value: 0x2
		//YELLOW STATE: Recovery
		verify_state = YELLOW;
		finish_verify = VB_REBOOT;
		dbg_printf(PRN_RES, "Press NO\n");
	}
#else
	dbg_printf(PRN_RES,"Boot image user key verify pass and Boot state is YELLOW!\n");
#endif

	return finish_verify;
}

VB_RET vb_state_enterstep(VB_STATE state)
{
	VB_RET ret = VB_CONTINUE;
	switch(state) {
		case VB_STATE_INIT:
			ret = vb_state_init();
			break;
		case VB_STATE_OEMK_FAIL:
			ret = vb_state_oemk_fail();
			break;
		case VB_STATE_OEMK_SUCC:
			ret = vb_state_oemk_succ();
			break;
		case VB_STATE_USERK_FAIL:
			ret = vb_state_userk_fail();
			break;
		case VB_STATE_USERK_SUCC:
			ret = vb_state_userk_succ();
			break;
		default:
			dbg_printf(PRN_ERR, "no such state\n");
			break;
	};
	return ret;
}

int is_vb_finish(void)
{
	return (0x1 == finish_verify);
}

int is_dev_lock(void)
{
	return (DEV_LOCK == dev_state);
}

int get_verify_state(void)
{
	return verify_state;
}

void vb_setup_linux_parameter(char * kernel_param)
{
	char verify_boot_state[16] = "\0";
#define VERIFY_BOOT_COMMAND_LINE	"androidboot.verifiedbootstate="

	switch(verify_state){
		case GREEN:
			strcpy(verify_boot_state, "green");
			break;
		case ORANGE:
			strcpy(verify_boot_state, "orange");
			break;
		case YELLOW:
			strcpy(verify_boot_state, "yellow");
			break;
		case RED:
			strcpy(verify_boot_state, "red");
			break;
		default:
			strcpy(verify_boot_state,"invalid");
			break;
	}
	if(strlen(verify_boot_state) > 0){
		strcat(kernel_param, " ");
		strcat(kernel_param, VERIFY_BOOT_COMMAND_LINE);
		if(is_vb_finish()){
			strcat(kernel_param, verify_boot_state);//the max len is 16
		}
		else{
			strcat(kernel_param, "invalid");
		}
	}
}

