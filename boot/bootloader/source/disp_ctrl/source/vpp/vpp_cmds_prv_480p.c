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

#include "vpp_cmds_prv.h"

/* This file include is restricted to this file alone */
#include "vpp_cmds_prv_480p.h"
#include "vpp_cmds_prv_480p_gfx.h"

void init_480p_gfx(void)
{
    int i;
    cmd_config_t *cfgs = &cmd_cfgs[INDEX_GFX];
    cfgs->res_index = INDEX_GFX;

    for(i = 0; i < CMD_MAX; i++)
    {
        switch (i)
        {
        case 0:
            cfgs->cmd_size[i] = sizeof(cmd_11) / (sizeof(unsigned int) * 3);
            cfgs->cmd_refs[i] = cmd_11;
            break;
        case 1:
            cfgs->cmd_size[i] = sizeof(cmd_12) / (sizeof(unsigned int) * 3);
            cfgs->cmd_refs[i] = cmd_12;
            break;
        case 2:
            cfgs->cmd_size[i] = sizeof(cmd_13) / (sizeof(unsigned int) * 3);
            cfgs->cmd_refs[i] = cmd_13;
            break;
        case 3:
            cfgs->cmd_size[i] = sizeof(cmd_14) / (sizeof(unsigned int) * 3);
            cfgs->cmd_refs[i] = cmd_14;
            break;
        case 4:
            cfgs->cmd_size[i] = sizeof(cmd_15) / (sizeof(unsigned int) * 3);
            cfgs->cmd_refs[i] = cmd_15;
            break;
        case 5:
            cfgs->cmd_size[i] = sizeof(cmd_16) / (sizeof(unsigned int) * 3);
            cfgs->cmd_refs[i] = cmd_16;
            break;
        case 6:
            cfgs->cmd_size[i] = sizeof(cmd_17) / (sizeof(unsigned int) * 3);
            cfgs->cmd_refs[i] = cmd_17;
            break;
        case 7:
            cfgs->cmd_size[i] = sizeof(cmd_18) / (sizeof(unsigned int) * 3);
            cfgs->cmd_refs[i] = cmd_18;
            break;
        default:
            printf("Error cmd config 720p\n");
            break;
        }
    }
}
void init_480p(void)
{
    int i;
    cmd_config_t *cfgs = &cmd_cfgs[INDEX_MAIN];
    cfgs->res_index = INDEX_MAIN;

    for(i = 0; i < CMD_MAX; i++)
    {
        switch (i)
        {
        case 0:
            cfgs->cmd_size[i] = sizeof(cmd_1) / (sizeof(unsigned int) * 3);
            cfgs->cmd_refs[i] = cmd_1;
            break;
        case 1:
            cfgs->cmd_size[i] = sizeof(cmd_2) / (sizeof(unsigned int) * 3);
            cfgs->cmd_refs[i] = cmd_2;
            break;
        case 2:
            cfgs->cmd_size[i] = sizeof(cmd_3) / (sizeof(unsigned int) * 3);
            cfgs->cmd_refs[i] = cmd_3;
            break;
        case 3:
            cfgs->cmd_size[i] = sizeof(cmd_4) / (sizeof(unsigned int) * 3);
            cfgs->cmd_refs[i] = cmd_4;
            break;
        case 4:
            cfgs->cmd_size[i] = sizeof(cmd_5) / (sizeof(unsigned int) * 3);
            cfgs->cmd_refs[i] = cmd_5;
            break;
        case 5:
            cfgs->cmd_size[i] = sizeof(cmd_6) / (sizeof(unsigned int) * 3);
            cfgs->cmd_refs[i] = cmd_6;
            break;
        case 6:
            cfgs->cmd_size[i] = sizeof(cmd_7) / (sizeof(unsigned int) * 3);
            cfgs->cmd_refs[i] = cmd_7;
            break;
        case 7:
            cfgs->cmd_size[i] = sizeof(cmd_8) / (sizeof(unsigned int) * 3);
            cfgs->cmd_refs[i] = cmd_8;
            break;
        default:
            printf("Error cmd config 480p\n");
            break;
        }
    }
    init_480p_gfx();
}

