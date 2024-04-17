/*******************************************************************************
*                Copyright 2007, MARVELL SEMICONDUCTOR, LTD.                   *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL.                      *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
*                                                                              *
* MARVELL COMPRISES MARVELL TECHNOLOGY GROUP LTD. (MTGL) AND ITS SUBSIDIARIES, *
* MARVELL INTERNATIONAL LTD. (MIL), MARVELL TECHNOLOGY, INC. (MTI), MARVELL    *
* SEMICONDUCTOR, INC. (MSI), MARVELL ASIA PTE LTD. (MAPL), MARVELL JAPAN K.K.  *
* (MJKK), MARVELL ISRAEL LTD. (MSIL).                                          *
*******************************************************************************/

#include "vpp_module.h"
#include "io.h"
#include "vpp_cmds.h"
#include "vpp_api.h"
#include "vpp_cmds_prv.h"

#define VPP_BCMBUF_WRITE(a, d) VPP_BCMBUF_Write(pVppobj->pVbiBcmBuf, a, d)
#define VPP_BCMBUF_WRITE_CLEARBUF(a, d) \
				VPP_BCMBUF_Write(pVppobj->pVbiClearBcmBuf, a, d)

extern VPP_OBJ *pVppobj;

static unsigned int gRes_index = INDEX_MAIN;
static unsigned int gRes_index_gfx = INDEX_GFX;

cmd_config_t cmd_cfgs[CMD_CONFIG_MAX];

static VOID delay_1ms(INT32 DelayTime)
{
    (void)DelayTime;
    volatile unsigned int counter = 200000;
    while(--counter > 0);
}

static void calib_func(UINT32 status_reg, UINT32 status_mask)
{
	UINT32 regData = 0;
	UINT32 counter = 0;

	do {
		GA_REG_WORD32_READ(status_reg, &regData);
		if (regData & status_mask)
			break;
		counter++;
		if (counter >= 2000) {
			printf("calibration failed : status_reg=0x%08x "
					"status_mask=0x%08x regData=0x%08x\n",
					status_reg, status_mask, regData);
			break;
		}
	} while (1);
}

static void calib_phy_func(UINT32 status_reg, UINT32 status_mask)
{
	UINT32 regData = 0;
	UINT32 counter = 0;

	do {
		delay_1ms(1);
		GA_REG_WORD32_READ(status_reg, &regData);
		counter++;
		if (counter >= 2000) {
			printf("calib_phy_func: failed : status_reg=0x%08x "
					"status_mask=0x%08x regData=0x%08x\n",
					status_reg, status_mask, regData);
			break;
		}
	} while (((regData & status_mask) == status_mask));
}

static void calib_phy_chk_func(UINT32 status_reg, UINT32 status_mask)
{
	UINT32 regData = 0;

	GA_REG_WORD32_READ(status_reg, &regData);
	if ((regData & status_mask) == status_mask)
		printf("calib_phy_chk_func: Calibration failed\n");
}

static void calib_phy_lock_func(UINT32 status_reg, UINT32 status_mask)
{
	UINT32 regData = 0;
	UINT32 counter = 0;

	do {
		delay_1ms(1);
		GA_REG_WORD32_READ(status_reg, &regData);
		counter++;
		if (counter >= 2000) {
			printf("calib_phy_lock_func: failed : status_reg=0x%08x "
					"status_mask=0x%08x regData=0x%08x\n",
					status_reg, status_mask, regData);
			break;
		}
	} while (((regData & status_mask) == 0));
}

void init_resolution_index(int resid)
{
    switch (resid)
    {
    case RES_1080P60:
        init_1080p();
        break;
    case RES_720P60:
        init_720p();
        break;
    case RES_525P60:
        init_480p();
        break;
    default:
        printf("Error: Resolution %d is not supported\n");
        break;
    }
}

void exec_common_command(CMD_ID_T cmd_id, UINT32 res_index)
{
	UINT32 i = 0, arr_size = 0, (*arr)[3] = NULL;

	switch (cmd_id) {

	case CMD_ID_1:
		arr_size = cmd_cfgs[res_index].cmd_size[CMD_ID_1];
		arr = cmd_cfgs[res_index].cmd_refs[CMD_ID_1];
		break;
	case CMD_ID_2:
		arr_size = cmd_cfgs[res_index].cmd_size[CMD_ID_2];
		arr = cmd_cfgs[res_index].cmd_refs[CMD_ID_2];
		break;
	case CMD_ID_3:
		arr_size = cmd_cfgs[res_index].cmd_size[CMD_ID_3];
		arr = cmd_cfgs[res_index].cmd_refs[CMD_ID_3];
		break;
	case CMD_ID_4:
		arr_size = cmd_cfgs[res_index].cmd_size[CMD_ID_4];
		arr = cmd_cfgs[res_index].cmd_refs[CMD_ID_4];
		break;
	case CMD_ID_5:
		arr_size = cmd_cfgs[res_index].cmd_size[CMD_ID_5];
		arr = cmd_cfgs[res_index].cmd_refs[CMD_ID_5];
		break;
	case CMD_ID_6:
		arr_size = cmd_cfgs[res_index].cmd_size[CMD_ID_6];
		arr = cmd_cfgs[res_index].cmd_refs[CMD_ID_6];
		break;
	case CMD_ID_7:
		arr_size = cmd_cfgs[res_index].cmd_size[CMD_ID_7];
		arr = cmd_cfgs[res_index].cmd_refs[CMD_ID_7];
		break;
	case CMD_ID_8:
		arr_size = cmd_cfgs[res_index].cmd_size[CMD_ID_8];
		arr = cmd_cfgs[res_index].cmd_refs[CMD_ID_8];
		break;
	default:
		printf("Not a valid command id\n");
		break;
	}

	for (i = 0; i < arr_size; i++) {
		switch (arr[i][0]) {

		case WRITE_FUNC:
			GA_REG_WORD32_WRITE(arr[i][1], arr[i][2]);
			break;
		case SLEEP_FUNC:
			delay_1ms(arr[i][1]);
			break;
		case CALIB_FUNC:
			calib_func(arr[i][1], arr[i][2]);
			break;
		case CALIB_PHY_FUNC:
			calib_phy_func(arr[i][1], arr[i][2]);
			break;
		case CALIB_PHY_CHK_FUNC:
			calib_phy_chk_func(arr[i][1], arr[i][2]);
			break;
		case CALIB_PHY_LOCK_FUNC:
			calib_phy_lock_func(arr[i][1], arr[i][2]);
			break;
		case BCM_WRITE_FUNC:
			VPP_BCMBUF_WRITE(arr[i][1], arr[i][2]);
			break;
		case BCM_CLEAR_WRITE_FUNC:
			VPP_BCMBUF_WRITE_CLEARBUF(arr[i][1], arr[i][2]);
			break;
		default:
			printf("Not a valid function id\n");
			break;
		}
	}
}

void exec_command(CMD_ID_T cmd_id)
{
    exec_common_command(cmd_id, gRes_index);
}
void exec_command_gfx(CMD_ID_T cmd_id)
{
    exec_common_command(cmd_id, gRes_index_gfx);
}
