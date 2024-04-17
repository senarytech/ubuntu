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

#ifndef _VPP_CA_WRAP_H_
#define _VPP_CA_WRAP_H_

#include "vpp_ca_wrap.h"
#include "vdec_com.h"
#include "com_type.h"

typedef struct _AVPLL_RES_DESC_
{
    int resID;
    int pixelclk;
    int ppm1ken; //0 for 50/59.94Hz SD & 59.94/29.97/23.98 HD resolutions
}AVPLL_RES_DESC;

int VPP_ISR_Handler(UINT32 msg_id, UINT32 intstat);
void MV_VPP_SetDislayMode(int planeID);
int MV_VPP_SetCPCBOutputResolution(int cpcbID, int resID, int bit_depth);
int MV_VPP_SetHdmiTxControl(int enable);
int MV_VPP_Create(unsigned int base, unsigned int hdmi_addr, unsigned options);
int MV_VPP_Reset(void);
int MV_VPP_Config(void);
int MV_VPP_OpenDispWindow(int planeID, VPP_WIN *win, VPP_WIN_ATTR *attr);
void MV_VPP_DisplayFrame(int planeID, VBUF_INFO *vbufinfo);
void MV_DisplayFrame(int planeID, VBUF_INFO *vbufinfo, int src_fmt,INT32 x, INT32 y, INT32 width, INT32 height, int bitdepth);
int MV_VPP_Stop(void);
int MV_VPP_Destroy(void);
int MV_VPP_SetFormat(INT handle, INT cpcbID,
                        VPP_DISP_OUT_PARAMS *pDispParams);

HRESULT create_global_desc_array(int src_fmt, int bitdepth,int width, int height);
HRESULT get_vbuf_info(VBUF_INFO **pVBInfo);
void build_frames( VBUF_INFO *vbufinfo, INT32 srcfmt,
        INT32 bit_depth, INT32 x, INT32 y, INT32 width, INT32 height, INT32 progressive, INT32 pattern_type,bool IsPatt);
#endif
