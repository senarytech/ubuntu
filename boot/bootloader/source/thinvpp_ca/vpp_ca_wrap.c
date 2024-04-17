/*******************************************************************************
*                Copyright 2012, MARVELL SEMICONDUCTOR, LTD.                   *
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

#include "vpp_api.h"
#include "vpp_ca_wrap.h"
#include "vbuf.h"
#include "vdec_com.h"
#include "avpll.h"
#include "tee_ca_vpp.h"
#include "avpll.h"

#if (BERLIN_CHIP_VERSION>=BERLIN_VS680)
#include "diag_pll.h"
#include "vpp_cfg.h"

typedef struct __SetClockFreq_Data_ {
    int pllsrc;
    int isInterlaced;
    int refresh_rate;
    int h_active;
    int v_active;
    int h_total;
    int v_total;
} SetClockFreq_Data;

static void AVPLL_SetClockFreq_Pack(int *p_packed_data, SetClockFreq_Data *p_data) {
    p_packed_data[0] = p_data->pllsrc;
    p_packed_data[1] = p_data->isInterlaced << 8 | p_data->refresh_rate;
    p_packed_data[2] = p_data->h_active << 16 | p_data->v_active;
    p_packed_data[3] = p_data->h_total << 16 | p_data->v_total;
}

int MV_VPP_SetCPCBOutputResolution(int cpcbID, int resID, int bit_depth)
{
    SetClockFreq_Data clock_data;
    int packed_data[4];
    clock_data.pllsrc = 0;
    clock_data.isInterlaced = m_resinfo_table[resID].scan;
    clock_data.refresh_rate = m_resinfo_table[resID].frame_rate;
    clock_data.h_active = m_resinfo_table[resID].active_width;
    clock_data.v_active = m_resinfo_table[resID].active_height;
    clock_data.h_total = m_resinfo_table[resID].width;
    clock_data.v_total = m_resinfo_table[resID].height;

    AVPLL_SetClockFreq_Pack(&packed_data[0], &clock_data);
    AVPLL_SetClockFreq(0, packed_data[1], packed_data[2], packed_data[3]);
#ifdef CONFIG_PANEL_FASTLOGO
    AVPLL_SetClockFreq(1, packed_data[1], packed_data[2], packed_data[3]);
#endif
    MV_VPPOBJ_SetCPCBOutputResolution(0, cpcbID, resID, bit_depth);

    return MV_VPPOBJ_SetHdmiVideoFmt(0, OUTPUT_COLOR_FMT_RGB888, bit_depth, 1);
}

int MV_VPP_Reset(void)
{
    MV_VPPOBJ_Reset(0);
    return 0;
}

#else
#include "pllDiag.h"
#define  VPP_SYS_CLK_SEL    17

AVPLL_RES_DESC res_info[] = {
    {RES_525P60,  27027,  1},
    {RES_720P50,  74250,  0},
    {RES_720P60,  74250,  1},
    {RES_1080P60, 148500, 1},
    {RES_1080P50, 148500, 1},
    {}
};

static int get_pixclk(int resID, int *ppixclk, int *pppm1k)
{
    int res_desc_sz, index, rc = -1;
    res_desc_sz = sizeof(res_info)/sizeof(AVPLL_RES_DESC);
    for(index = 0; index < res_desc_sz; index++)
    {
        if(res_info[index].resID == resID){
            *ppixclk = res_info[index].pixelclk;
            *pppm1k = res_info[index].ppm1ken;
            rc = 0;
        }
    }
    return rc;
}

static void config_avpll_videoclk(int resID)
{
    int pixelclk = 0;
    int ppm1k_en = 0;
    int avpll_freq_index = 0;
    float     ovsmp_index = 0;

    if(get_pixclk(resID, &pixelclk, &ppm1k_en) < 0)
        return;

    if(pixelclk<= 25200)
    {
        avpll_freq_index = 0;
        ovsmp_index = 4.0;
    }
    else if((pixelclk == 27000) || (pixelclk == 27027))
    {
        avpll_freq_index = 1;
        ovsmp_index = 4.0;
    }
    else if((pixelclk == 74250) || (pixelclk == 74176))
    {
        avpll_freq_index = 3;
        ovsmp_index = 2.0;
    }
    else if((pixelclk == 148500) || (pixelclk == 148352))
    {
        avpll_freq_index = 5;
        ovsmp_index = 1.0;
    }
    else if((pixelclk == 297000) || (pixelclk == 296703))
    {
        avpll_freq_index = 5;
        ovsmp_index = 2.0;
    }
    else if((pixelclk == 594000) || (pixelclk == 593406))
    {
        avpll_freq_index = 5;
        ovsmp_index = 4.0;
    }
    AVPLL_SetVideoFreq(AVPLL_A,avpll_freq_index, 0, ppm1k_en, ovsmp_index, 1);
    return;
}

int MV_VPP_SetCPCBOutputResolution(int cpcbID, int resID, int bit_depth)
{
    int reschng_stage1 = 24, reschng_stage2 = 25, reschng_stageID;

    reschng_stageID = resID | (1<<reschng_stage1);
    MV_VPPOBJ_SetCPCBOutputResolution(0, cpcbID, reschng_stageID, bit_depth);

    config_avpll_videoclk(resID);

    reschng_stageID = resID | (1<<reschng_stage2);
    MV_VPPOBJ_SetCPCBOutputResolution(0, cpcbID, reschng_stageID, bit_depth);

    return MV_VPPOBJ_SetHdmiVideoFmt(0, OUTPUT_COLOR_FMT_RGB888, bit_depth, 1);
}

int MV_VPP_Reset(void)
{
    MV_VPPOBJ_Reset(0);
    diag_clock_change_otherClk(VPP_SYS_CLK_SEL, 1, 1, 1);
    diag_clockFreq(AVPLL_A, 7, 660000000, 5);
    return 0;
}
#endif

int VPP_ISR_Handler(UINT32 msg_id, UINT32 intstat)
{
    VppIsrHandler(msg_id, intstat);
    return 0;
}

void MV_VPP_SetDislayMode(int planeID)
{
    MV_VPPOBJ_SetDisplayMode(0, planeID, DISP_FRAME);
    return;
}


int MV_VPP_SetHdmiTxControl(int enable)
{
    return MV_VPPOBJ_SetHdmiTxControl(0, enable);
}

int MV_VPP_Create(unsigned int base, unsigned int hdmi_addr, unsigned options)
{
    MV_VPPOBJ_Create(0,0);//inputs are unused.
    return 0;
}


int MV_VPP_Config(void)
{
    MV_VPPOBJ_Config(0, NULL, NULL, NULL, NULL);
    return 0;
}

int MV_VPP_OpenDispWindow(int planeID, VPP_WIN *win, VPP_WIN_ATTR *attr)
{
    MV_VPPOBJ_OpenDispWindow(0, planeID, win, attr);
    return 0;
}

void MV_VPP_DisplayFrame(int planeID, VBUF_INFO *vbufinfo)
{
    MV_VPPOBJ_DisplayFrame(0, planeID,vbufinfo);
    return;
}

void MV_DisplayFrame(int planeID, VBUF_INFO *vbufinfo, int src_fmt,INT32 x, INT32 y, INT32 width, INT32 height, int bitdepth)
{
    MV_VPP_SetDislayMode(planeID);
    build_frames( vbufinfo, src_fmt, bitdepth,  x,  y,  width,  height,  1, 0,0);
    MV_VPPOBJ_DisplayFrame(0, planeID,vbufinfo);
    return;
}

int MV_VPP_Stop(void)
{
    MV_VPPOBJ_Stop(0);
    return 0;
}

int MV_VPP_Destroy(void)
{
    MV_VPPOBJ_Destroy(0);
    return 0;
}

int MV_VPP_SetFormat(INT handle, INT cpcbID, VPP_DISP_OUT_PARAMS *pDispParams)
{
    int ret = 0;

    SetClockFreq_Data clock_data;
    int packed_data[4];
    UINT32  resID;
    RESOLUTION_INFO*    resinfo;

    resID = pDispParams->uiResId;
    if (pDispParams->uiDispId == VOUT_DSI)
        resinfo = &m_resinfo_table[RES_DSI_CUSTOM];
    else
        resinfo = &m_resinfo_table[resID];

    clock_data.pllsrc = 0;
    clock_data.isInterlaced = resinfo->scan;
    clock_data.refresh_rate = resinfo->frame_rate;
    clock_data.h_active = resinfo->active_width;
    clock_data.v_active = resinfo->active_height;
    clock_data.h_total = resinfo->width;
    clock_data.v_total = resinfo->height;
    AVPLL_SetClockFreq_Pack(&packed_data[0], &clock_data);
    AVPLL_SetClockFreq(0, packed_data[1], packed_data[2], packed_data[3]);
#ifdef CONFIG_PANEL_FASTLOGO
    if (pDispParams->uiDispId == VOUT_DSI)
        AVPLL_SetClockFreq(1, packed_data[1], packed_data[2], packed_data[3]);
#endif
    ret = MV_VPPOBJ_SetFormat(0, cpcbID, pDispParams);

    return ret;
}