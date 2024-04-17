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

#ifndef _VPP_CFG_H_
#define _VPP_CFG_H_

typedef struct RESOLUTION_INFO_T {
	int active_width;
	int active_height;
	int width;
	int height;
	int hfrontporch;
	int hsyncwidth;
	int hbackporch;
	int vfrontporch;
	int vsyncwidth;
	int vbackporch;
	int type;
	int scan;
	int frame_rate;
	int flag_3d;
	int freq;
	int pts_per_cnt_4;
} RESOLUTION_INFO;

#ifndef _VPP_CFG_C_
/* VPP module constant tables */
extern const RESOLUTION_INFO m_resinfo_table[MAX_NUM_RESS];

#endif

#endif
