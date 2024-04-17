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

#ifndef _DISP_CTRL_H_
#define _DISP_CTRL_H_
#include "com_type.h"
#define NUM_BUFFERS	(2)
#define MAX_ISR_COUNT 200

INT32 MV_VPP_Init(INT32 resid);
VOID MV_VPP_Deinit(VOID);
VOID MV_VPP_Enable_IRQ();
VOID MV_VPP_DisplayPatternFrame(INT32 x, INT32 y, INT32 logo_width,
            INT32 logo_height, INT32 logo_stride, VOID *yuv_logo);
VOID VPP_ISR_Handler_irq();
VOID MV_VPP_DisplayGfxFrame(void *pBuf,int width,int height);
void* GaloisMalloc(unsigned int size);
void* GaloisMemSet(void *s, int c, int n);
void* GaloisMemcpy(void *s1, const void *s2, int n);
#endif /* _DISP_CTRL_H_ */
