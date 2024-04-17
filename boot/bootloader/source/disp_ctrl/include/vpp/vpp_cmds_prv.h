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

#ifndef _VPP_CMDS_PRV_H_
#define _VPP_CMDS_PRV_H_

#define CMD_MAX 8
#define CMD_CONFIG_MAX 2
#define INDEX_MAIN 0
#define INDEX_GFX 1

enum FUNCTION_ID {
    WRITE_FUNC,
    SLEEP_FUNC,
    CALIB_FUNC,
    CALIB_PHY_FUNC,
    CALIB_PHY_CHK_FUNC,
    CALIB_PHY_LOCK_FUNC,
    BCM_WRITE_FUNC,
    BCM_CLEAR_WRITE_FUNC,
};

typedef struct _cmd_conig_t_ {
    unsigned int res_index;
    unsigned int cmd_size[CMD_MAX];
    unsigned int (*cmd_refs[CMD_MAX])[3];
} cmd_config_t;

extern cmd_config_t cmd_cfgs[CMD_CONFIG_MAX];

void init_1080p(void);
void init_720p(void);
void init_480p(void);

#endif
