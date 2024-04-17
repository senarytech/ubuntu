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
#ifdef CONFIG_SM

#include "io.h"
#include "debug.h"

#include "SysMgr.h"

#include "system_manager.h"

#include "chip_id.h"
#include "apbRegBase.h"

#include "timer.h"

#include "bootloader.h"
#include "rt_stage.h"
#include "bl_boot_arg.h"
#include "misc.h"
#include "boot_mode.h"

#define	MV88DE3100_CHIPID_REG2	(0xF7CC07CC)

extern INT32 MV_SM_Dev_Init(UCHAR * pucData_Buffer,UINT32 uiData_Size);

extern int MV_SM_Send_Msg(int, unsigned char *, int);

extern unsigned int get_soc_tsen_id(void);

extern unsigned int get_board_ver(void);

#define MV_SM_POWER_SYS_RESET 0xFF
#define MV_SM_POWER_STANDBY_REQUEST 0x1

#define REBOOT_MSG_EXTRA_BUF_SIZE	512
uintptr_t reset_msg_extra_buf_addr = SM_MSG_EXTRA_BUF_ADDR;
int reset_msg_extra_size = 0;
char reset_msg_extra_buf[REBOOT_MSG_EXTRA_BUF_SIZE];

enum sm_ldo_ctrl_vout_sel_e {
	SM_LDO_CTRL_VOUT_1_050V_default = 0,
	SM_LDO_CTRL_VOUT_0_800V,
	SM_LDO_CTRL_VOUT_0_900V,
	SM_LDO_CTRL_VOUT_1_000V,
	SM_LDO_CTRL_VOUT_1_050V,
	SM_LDO_CTRL_VOUT_1_100V,
	SM_LDO_CTRL_VOUT_1_150V,
	SM_LDO_CTRL_VOUT_1_200V,
	SM_LDO_CTRL_VOUT_MAX
};

static void sm_init_parameters(void)
{
	//dbg_printf(PRN_DBG, "SM Init: uiBoot = 0x%x, SM_BOOT_FLAG_REG=0x%X !\n", readl(SM_BOOT_FLAG_REG), SM_BOOT_FLAG_REG);
	init_bootstate();
}

static void sm_pad_init(void)
{
	//FIXME
}

/*
* For Android not need to load SM from bootimgs.
* Currently, we use this solution. Because it can save the space of flash
* memory of bootimgs & bootimgs-B. And currently, there is a bug in recoery
* scripts, it doesn't update bootimgs-B.
*/
static int load_and_start_sm(void)
{
	/****************************************************************
	 * Load and start SM
	 ****************************************************************/
	//if not warm down or warm up, always load system manage image
	//the cases are:	cold boot up, warm up after onlineUpgrade
	if(is_reset() || is_poweron() || is_coldbootup())
	{
		//when box product release is defined, SM will shut down SoC after power on.
		//this can be done by write the cold boot flag to SM's register
		if(is_coldbootup()) {
			dbg_printf(PRN_RES, "Set cold boot flag: %08x\n", MV_SM_RSTFLAG_COLD);
			set_bootstate(MV_SM_RSTFLAG_COLD);
		}
#ifdef CONFIG_GENX_ENABLE
		extern INT32 Genx_MV_SM_Dev_Init(void);
		dbg_printf(PRN_RES, "Load SM Firmware Image and Start to Kick Off SM CPM");
		Genx_MV_SM_Dev_Init();
#else
		extern unsigned int sm_start, sm_end;
		dbg_printf(PRN_RES, "start to kick off SM CPU: start 0x%08x, size %d\n", sm_start, sm_end - sm_start);
		//download SM image from DDR to ITCM and kick off SM
		MV_SM_Dev_Init((unsigned char*)(uintptr_t)sm_start, sm_end - sm_start);
#endif
		dbg_printf(PRN_INFO, "[%6d] SM CPU is kicked off.\n", get_time_ms());
	}

	return 0;
}


static void send_reset_cmd(void)
{
	unsigned int msgSend = MV_SM_POWER_SYS_RESET;
	dbg_printf(PRN_NOTICE, "send reset command to sm\n");
	MV_SM_Send_Msg(MV_SM_ID_POWER, (unsigned char *)(&msgSend), sizeof(msgSend));
}

static void send_powerdown_cmd(void)
{
	unsigned int msgSend = MV_SM_POWER_STANDBY_REQUEST;
	dbg_printf(PRN_NOTICE, "send powerdown command to sm\n");
	MV_SM_Send_Msg(MV_SM_ID_POWER, (unsigned char *)(&msgSend), sizeof(msgSend));
}


// send special bootmode message. We might not otherwise send a message to SM, depending
// on how we decided to choose recovery/normal, so make sure he knows. (This is so that Beetle
// devices can handle the FUNCTION button correctly for Recovery.)
// all warmup states should pass this way.
static void notify_SM_of_Bootmode(int boot_mode)
{
	//FIXME
}

#ifdef ARMV8_64BIT
static void DisableInterrupt(void)
{
	__asm__ __volatile__(
			"MSR     DAIFSET, #3"	"\n\t"
			"isb"
		);
}
#else
static void DisableInterrupt(void)
{
	__asm__ __volatile__(
			"MRS			r0, CPSR"	"\n\t"		//get CRPR
			"BIC			r0, #0xC0"	"\n\t"		// disable IRQ and FIQ
			"MSR			CPSR, r0"  "\n\t"
			:
			:
			:"r0"
		);
}
#endif

static void response_to_sm_pre()
{
	//add by hjcai,System reset,SM codes will be loaded by CPU1 image, so should suspend SM
	if(is_reset())
	{
		dbg_printf(PRN_INFO, "Boot from system reset.\n");
		//		reg(SM_CPU_CTRL)=0x4;
	}
	else if(is_standby_2_active())
	{
		//	first send WARMUP_RESP_0 to SM
		dbg_printf(PRN_INFO, "Send WARMUP_0_RESP.\n");

		unsigned int uiMsg;
		uiMsg = MV_SoC_STATE_WARMUP_0_RESP ;
		MV_SM_Send_Msg(MV_SM_ID_POWER, (unsigned char *)(&uiMsg), sizeof(uiMsg)) ;
	}
	else if(is_resume() || is_lowpowerstandby_2_active())
	{
		//	first send WARMUP_RESP_1 to SM
		unsigned int uiMsg;

		dbg_printf(PRN_INFO, "Send WARMUP_1_RESP.\n");
		if (is_lowpowerstandby_2_active())
			dbg_printf(PRN_INFO, "Answering WU_FROM_DDR_OFF1.\n");
		uiMsg = MV_SoC_STATE_WARMUP_1_RESP ;
		MV_SM_Send_Msg(MV_SM_ID_POWER, (unsigned char *)(&uiMsg), sizeof(uiMsg)) ;
	}
}

static void notify_SM_of_SocTsenId(void)
{
	unsigned int soc_tsen_id;
	unsigned int pMsgSend[2] ;

	soc_tsen_id = get_soc_tsen_id();
	pMsgSend[0] = MV_SM_SOC_TSEN_ID ;
	pMsgSend[1] = soc_tsen_id ;

	MV_SM_Send_Msg(MV_SM_ID_TEMP, (unsigned char *)pMsgSend, sizeof(pMsgSend)) ;

	return;
}

static void notify_SM_of_BoardVer(void)
{
	unsigned int board_version;
	unsigned int pMsgSend[2] ;

	board_version = get_board_ver();
	pMsgSend[0] = MV_SM_BOARDVER_GET ;
	pMsgSend[1] = board_version ;

	MV_SM_Send_Msg(MV_SM_ID_GPIO, (unsigned char *)pMsgSend, sizeof(pMsgSend)) ;

	return;
}

static void response_to_sm()
{
	/*
	 * Moved from bootloader to here
	 * cold_up warm_up ... response
	 */
	/* send response if system is going to suspend(warmdown2) */
	if(is_coldbootup())    //Remove WARM_DOWN_1/2 state
	{

		dbg_printf(PRN_NOTICE, "Prepare to enter power saving mode.\n");

		// Save DDR parameters to SM
		//		Save_Warm_Boot_Contex() ;

		// Disable interrupt & Make sure code running in I-$
		DisableInterrupt();

		if (is_poweron())
		{
			unsigned int uiMsg;
			dbg_printf(PRN_RES, "Send colddown response.\n");
			uiMsg = MV_SoC_STATE_COLDBOOT;
			MV_SM_Send_Msg(MV_SM_ID_POWER, (unsigned char *)(&uiMsg), sizeof(uiMsg));
		}

		/* wait for sm to power down */
		while(1);
	}
}

static void set_sm_ldo_ctrl_vout(int vout_sel)
{
	//FIXME
}

/******************************************************************
* SM process fuctions of bootloader stage.
* Function used in stage should be declared by convinence macro
******************************************************************/
static int __attribute__((used)) sm_bootloader_initchip_prestage(void)
{
	sm_init_parameters();
	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) sm_bootloader_initchip_stage(void)
{
	//if defined sm, run pad init related with sm
	sm_pad_init();
	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) sm_bootloader_initchip_poststage(void)
{
	//FIXME
	set_sm_ldo_ctrl_vout(SM_LDO_CTRL_VOUT_1_150V);

	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) sm_bootloader_initfts_poststage(void)
{
	// load sm after version table is got
	// maybe we will store sm to another partition
	load_and_start_sm();

	dbg_printf(PRN_RES, "Pre-responding to SM...\n");
	// put partial response here to sync with SM for early stage sm caller usage
	response_to_sm_pre();
	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) sm_bootloader_loadkernel_poststage(void)
{
	//FIXME
	//after kernel image is load and boot mode is determined
	notify_SM_of_Bootmode(get_bootmode());

	//Send SOC TSEN ID to sm
	notify_SM_of_SocTsenId();

	//Send Board version to sm
	notify_SM_of_BoardVer();

	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) sm_bootloader_startkernel_prestage(void)
{
	// consider there maybe have something to do before response to sm
	// i add it in this stage

	dbg_printf(PRN_RES, "Responding to SM...\n");
	response_to_sm();
	//dbg_printf(PRN_RES,"After Responded to SM...%d\n", get_bootstate());
	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) sm_bootloader_normal_reset_stage(void)
{
	//define a specify stage to handle the reset
	//and make the reset flow clear
	send_reset_cmd();
	while(1);
	return BOOTLOADER_NULL_STAGE;
}

static int __attribute__((used)) sm_bootloader_normal_powerdown_stage(void)
{
	send_powerdown_cmd();
	while(1);
	return BOOTLOADER_NULL_STAGE;
}
#endif



DECLARE_RT_STAGE(BOOTLOADER_INITCHIP_PRESTAGE, sm_initchip_prestage, 0, sm_bootloader_initchip_prestage);
DECLARE_RT_STAGE(BOOTLOADER_INITCHIP_STAGE, sm_initchip_stage, 0, sm_bootloader_initchip_stage);
DECLARE_RT_STAGE(BOOTLOADER_INITCHIP_POSTSTAGE, sm_initchip_poststage, 0, sm_bootloader_initchip_poststage);
DECLARE_RT_STAGE(BOOTLOADER_INITFTS_POSTSTAGE, main_initfts_poststage, 0, sm_bootloader_initfts_poststage);
DECLARE_RT_STAGE(BOOTLOADER_LOADKERNEL_POSTSTAGE, sm_loadkernel_poststage, 0, sm_bootloader_loadkernel_poststage);
DECLARE_RT_STAGE(BOOTLOADER_STARTKERNEL_PRESTAGE, sm_startkernel_prestage, 0, sm_bootloader_startkernel_prestage);
DECLARE_RT_STAGE(BOOTLOADER_NORMAL_RESET_STAGE, sm_normal_reset_stage, 0, sm_bootloader_normal_reset_stage);
DECLARE_RT_STAGE(BOOTLOADER_NORMAL_POWERDOWN_STAGE, sm_normal_powerdown_stage, 0, sm_bootloader_normal_powerdown_stage);
