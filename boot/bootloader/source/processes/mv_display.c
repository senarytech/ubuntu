/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (C) 2013-2018 Synaptics Incorporated. All rights reserved.
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
#ifdef CONFIG_FASTLOGO
#include "system_manager.h"
#if (BERLIN_CHIP_VERSION != BERLIN_BG5CT) || defined(CONFIG_VPP_USE_FASTLOGO_TA)
#include "avpll.h"
#include "vdec_com.h"
#endif
#include "vpp_api.h"
#include "memmap.h"
#include "irq.h"
#include "soc.h"
#include "gic.h"
#include "bootloader.h"
#include "flash_ts.h"
#include "avio_memmap.h"
#else
#include "disp_ctrl.h"
#endif
#include "debug.h"
#include "mv_display_api.h"
#if defined(CONFIG_VPP_USE_FASTLOGO_TA)
#include "OSAL_api.h"
#include "vpp_ca_wrap.h"
#endif //CONFIG_VPP_USE_FASTLOGO_TA
#include "cache.h"
#include "vpp_api.h"
#include "vpp_cfg.h"

#include "boot_mode.h"

#ifdef CONFIG_PANEL_FASTLOGO
#include "panelcfg.h"
#endif

#define PRN_DEBUG_LEVEL PRN_DBG
#if defined IRQ_dHubIntrAvio0
#define IRQ_DHUB_INTR_AVIO_0 IRQ_dHubIntrAvio0
#elif defined(IRQ_dHubIntrAvio0_0)
#define IRQ_DHUB_INTR_AVIO_0 IRQ_dHubIntrAvio0_0
#else
#endif

#if defined(CONFIG_VPP_USE_FASTLOGO_TA)
#if  defined(CONFIG_VPP_AUTOPUSH_ENABLED)
//If Autopush is enabled, then no need to wait for reciever stabilization/synchnization to TMDS signal
#define WAIT_LOOP_COUNT 10
#else
#define WAIT_LOOP_COUNT 240
#endif //CONFIG_VPP_AUTOPUSH_ENABLED
#else
#define WAIT_LOOP_COUNT 120
#endif //CONFIG_VPP_USE_FASTLOGO_TA
#define avioDhubSemMap_vpp_vppCPCB0_intr avioDhubSemMap_vpp128b_vpp_inr0
#define avioDhubSemMap_vpp_vppOUT4_intr avioDhubSemMap_vpp128b_vpp_inr6
#ifndef bTST
#define bTST(x, b) (((x) >> (b)) & 1)
#endif

#ifdef CONFIG_PANEL_FASTLOGO
#define VPP_MIPI_INIT_PANEL_PARAM(VAR, VALUE)  pMipiConfigParams->initparams.VAR = VALUE
#define VPP_MIPI_RESINFO_PANEL_PARAM(VAR, VALUE) pMipiConfigParams->infoparams.resInfo.VAR = VALUE
#define VPP_MIPI_TGINFO_PANEL_PARAM(VAR, VALUE)  pMipiConfigParams->infoparams.tgParams.VAR = VALUE

#define PANEL_INIT_PARAM(VAR)  \
            {                                                                                 \
                VPP_MIPI_INIT_PANEL_PARAM(resId, RES##VAR##_RESID);                               \
                VPP_MIPI_INIT_PANEL_PARAM(no_of_lanes, RES##VAR##_NO_OF_LANES);                   \
                VPP_MIPI_INIT_PANEL_PARAM(data_lane_polarity, RES##VAR##_DATA_LANE_POLARITY);     \
                VPP_MIPI_INIT_PANEL_PARAM(clk_lane_polarity, RES##VAR##_CLOCK_LANE_POLARITY);     \
                VPP_MIPI_INIT_PANEL_PARAM(virtual_channel, RES##VAR##_VIRTUAL_CHANNEL);           \
                VPP_MIPI_INIT_PANEL_PARAM(video_mode, RES##VAR##_VIDEO_MODE);                     \
                VPP_MIPI_INIT_PANEL_PARAM(non_continuous_clock, RES##VAR##_NON_CONTINUOUS_CLOCK); \
                VPP_MIPI_INIT_PANEL_PARAM(receive_ack_packets, RES##VAR##_RECEIVE_ACK);           \
                VPP_MIPI_INIT_PANEL_PARAM(byte_clock, RES##VAR##_BYTE_CLOCK);                     \
                VPP_MIPI_INIT_PANEL_PARAM(color_coding_format, RES##VAR##_COLOR_FORMAT);          \
                VPP_MIPI_INIT_PANEL_PARAM(is_18_loosely, RES##VAR##_IS_LOOSELY_18);               \
                VPP_MIPI_INIT_PANEL_PARAM(data_en_polarity, RES##VAR##_DATA_POLARITY);            \
                VPP_MIPI_INIT_PANEL_PARAM(h_polarity, RES##VAR##_HSYNC_POLARITY);                 \
                VPP_MIPI_INIT_PANEL_PARAM(v_polarity, RES##VAR##_VSYNC_POLARITY);                 \
                VPP_MIPI_INIT_PANEL_PARAM(eotp_rx_en, RES##VAR##_EOTP_RX);                        \
                VPP_MIPI_INIT_PANEL_PARAM(eotp_tx_en, RES##VAR##_EOTP_TX);                        \
                VPP_MIPI_INIT_PANEL_PARAM(no_of_chunks, RES##VAR##_NO_OF_CHUNKS);                 \
                VPP_MIPI_INIT_PANEL_PARAM(null_packet_size, RES##VAR##_NULL_PACKET_SIZE);         \
                VPP_MIPI_INIT_PANEL_PARAM(dpi_lp_cmd_en, RES##VAR##_LP_CMD_MODE);                 \
            }

#define PANEL_RESINFO_PARAM(VAR)   \
           {                                                                         \
                VPP_MIPI_RESINFO_PANEL_PARAM(active_width, RES##VAR##_ACTIVE_WIDTH);    \
                VPP_MIPI_RESINFO_PANEL_PARAM(active_height, RES##VAR##_ACTIVE_HEIGHT);  \
                VPP_MIPI_RESINFO_PANEL_PARAM(width, RES##VAR##_HTOTAL);                 \
                VPP_MIPI_RESINFO_PANEL_PARAM(hfrontporch, RES##VAR##_HFP);              \
                VPP_MIPI_RESINFO_PANEL_PARAM(hsyncwidth, RES##VAR##_HSYNCWIDTH);        \
                VPP_MIPI_RESINFO_PANEL_PARAM(hbackporch, RES##VAR##_HBP);               \
                VPP_MIPI_RESINFO_PANEL_PARAM(vfrontporch, RES##VAR##_VFP);              \
                VPP_MIPI_RESINFO_PANEL_PARAM(vsyncwidth, RES##VAR##_VSYNCWIDTH);        \
                VPP_MIPI_RESINFO_PANEL_PARAM(vbackporch, RES##VAR##_VBP);               \
                VPP_MIPI_RESINFO_PANEL_PARAM(type, RES##VAR##_TYPE);                    \
                VPP_MIPI_RESINFO_PANEL_PARAM(scan, RES##VAR##_SCAN_MODE);               \
                VPP_MIPI_RESINFO_PANEL_PARAM(frame_rate, RES##VAR##_FRAME_RATE);         \
                VPP_MIPI_RESINFO_PANEL_PARAM(flag_3d, RES##VAR##_IS_3D);                \
                VPP_MIPI_RESINFO_PANEL_PARAM(freq, RES##VAR##_PIXEL_CLOCK);             \
                VPP_MIPI_RESINFO_PANEL_PARAM(pts_per_cnt_4, RES##VAR##_PTS_PER_4);      \
            }

#define PANEL_TGINFO_PARAM(VAR)   \
            {                                                                      \
                VPP_MIPI_TGINFO_PANEL_PARAM(tg_vb_min, RES##VAR##_VB_MIN);          \
                VPP_MIPI_TGINFO_PANEL_PARAM(tg_hb_min, RES##VAR##_HB_MIN);          \
                VPP_MIPI_TGINFO_PANEL_PARAM(tg_size_v_off_p, RES##VAR##_V_OFF);     \
                VPP_MIPI_TGINFO_PANEL_PARAM(tg_size_h_off_p, RES##VAR##_H_OFF);     \
                VPP_MIPI_TGINFO_PANEL_PARAM(tg_hb_vop_off, RES##VAR##_HB_VOP_OFF);  \
                VPP_MIPI_TGINFO_PANEL_PARAM(tg_vb0_vop_off, RES##VAR##_VB_VOP_OFF); \
                VPP_MIPI_TGINFO_PANEL_PARAM(tg_hb_be, RES##VAR##_HB_BE);            \
                VPP_MIPI_TGINFO_PANEL_PARAM(tg_vb0_be, RES##VAR##_VB_BE);           \
                VPP_MIPI_TGINFO_PANEL_PARAM(tg_vb_fp, RES##VAR##_VB_FP);            \
                VPP_MIPI_TGINFO_PANEL_PARAM(tg_hb_fp, RES##VAR##_HB_FP);            \
            }

#define PANEL_CONFIG_PARAM(VAR)     acmdbuf[VAR-1].pcmd = aucommand##VAR;\
                                    acmdbuf[VAR-1].size = sizeof(aucommand##VAR);\
                                    acmdbuf[VAR-1].pstbycmd = austbycommand##VAR;\
                                    acmdbuf[VAR-1].stbycmdsize = sizeof(austbycommand##VAR);\
                                    pMipiConfigParams = (((VPP_MIPI_CONFIG_PARAMS*)ppMipiConfigParams) + (VAR-1));\
                                    PANEL_INIT_PARAM(VAR);      \
                                    PANEL_RESINFO_PARAM(VAR);   \
                                    PANEL_TGINFO_PARAM(VAR);

#define     RESINFO_PARAM_UPDATE(VAR)           m_resinfo_table[RES_DSI_CUSTOM].VAR = pMipiConfigParams->infoparams.resInfo.VAR

typedef struct CMDBUFINFO_T {
    UINT8*  pcmd;
    size_t  size;
    UINT8*  pstbycmd;
    size_t  stbycmdsize;
}CMDBUFINFO;

VPP_MIPI_CONFIG_PARAMS (*ppMipiConfigParams)[NO_OF_PANELCONFIG];

int MIPI_Config (void);
#endif

extern uint32_t getMPid(void);
extern int diag_GICSetInt(int MPid, int intId, int enable);
extern int VPP_ISR_Handler(UINT32 msg_id, UINT32 intstat);

#if (BERLIN_CHIP_VERSION != BERLIN_BG5CT) || defined(CONFIG_VPP_USE_FASTLOGO_TA)
#if (BERLIN_CHIP_VERSION >= BERLIN_BG5CT)
static VPP_WIN_ATTR showlogo_attr = {0x00801080, 0xFFF, 0};
#else
static VPP_WIN_ATTR showlogo_attr = {0x00801080, 0xFF, 1};
#endif
#endif

static volatile int loop_isr_count=0;
static int DISP_SetFormat (int cpcbId, int bitdepth);

void MV_DISP_VPP_ISR(void);

#if defined(CONFIG_VPP_USE_FASTLOGO_TA)
void MV_DISP_DisableIRQ(void) {
    diag_GICSetInt(getMPid(), MP_BERLIN_INTR_ID(IRQ_DHUB_INTR_AVIO_0), INT_DISABLE);
}
#endif

void MV_DISP_EnableIRQ(void)
{
#if (BERLIN_CHIP_VERSION != BERLIN_BG5CT) || defined(CONFIG_VPP_USE_FASTLOGO_TA)
    loop_isr_count=0;
    if(register_isr(MV_DISP_VPP_ISR, IRQ_DHUB_INTR_AVIO_0))
        dbg_printf(PRN_ERR, "mmc4 isr can't be registered\n");

    set_irq_enable(IRQ_DHUB_INTR_AVIO_0);
#else
    MV_VPP_Enable_IRQ();
#endif
}

void MV_DISP_VPP_ISR(void)
{
#if !defined(CONFIG_VPP_USE_FASTLOGO_TA)
    int instat;
    HDL_semaphore *pSemHandle;
    /* VPP interrupt handling  */
    pSemHandle = dhub_semaphore(&VPP_dhubHandle.dhub);
    instat = semaphore_chk_full(pSemHandle, -1);
    if (bTST(instat, avioDhubSemMap_vpp_vppCPCB0_intr))
    {
       loop_isr_count++;
       semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppCPCB0_intr, 1);
       semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppCPCB0_intr);
       VPP_ISR_Handler(VPP_CC_MSG_TYPE_VPP,instat);
    }
    if (bTST(instat, avioDhubSemMap_vpp_vppOUT4_intr))
    {
       semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppOUT4_intr, 1);
       semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppOUT4_intr);
    }
#else
    int instat = 0;

    loop_isr_count++;
    VPP_ISR_Handler(VPP_CC_MSG_TYPE_VPP,instat);
#endif
}

static int DISP_SetFormat (int cpcbId, int bitdepth) {
    VPP_DISP_OUT_PARAMS dispParams;

    dispParams.uiBitDepth = bitdepth;
    dispParams.uiColorFmt = OUTPUT_COLOR_FMT_RGB888;
    dispParams.iPixelRepeat = 1;

    if (cpcbId == CPCB_1) {
        dispParams.uiDispId         = VPP_DISP_PARAM_PRIMARY_DISPID;
        dispParams.uiDisplayMode    = VPP_PRIMARY_DISP_MODE;
        dispParams.uiResId          = PRIMARY_DISPLAY_RES;
    }
#ifdef BL_DISPLAY_DUAL_PIP
    else {
        dispParams.uiDispId         = VPP_DISP_PARAM_SECONDARY_DISPID;
        dispParams.uiDisplayMode    = VPP_SECONDARY_DISP_MODE;
        dispParams.uiResId          = SECONDARY_DISPLAY_RES;
    }
#endif

    return  MV_VPP_SetFormat(0, cpcbId, &dispParams);
}

int MV_DISP_InitDisplay(int resid, int hdmi_bitdepth)
{
    int ret = 0;

#if (BERLIN_CHIP_VERSION != BERLIN_BG5CT) || defined(CONFIG_VPP_USE_FASTLOGO_TA)
    int vpp_ret = 0;

    //AVPLL_InitClock();
    dbg_printf(PRN_DEBUG_LEVEL,"[ConfigDisplay] AVPLL enabled\n");
#if !defined(CONFIG_VPP_USE_FASTLOGO_TA)
    DhubInitialization(0, VPP_DHUB_BASE, VPP_HBO_SRAM_BASE, &VPP_dhubHandle, VPP_config, VPP_NUM_OF_CHANNELS,DHUB_TYPE_128BIT );
    DhubInitialization(0, AG_DHUB_BASE, AG_HBO_SRAM_BASE, &AG_dhubHandle, AG_config, AG_NUM_OF_CHANNELS,DHUB_TYPE_64BIT );
#else
    VPP_INIT_PARM vpp_init_parm;
    vpp_ret = MV_VPP_Init(&vpp_init_parm);
    if(vpp_ret != MV_VPP_OK)
    {
        ret= MV_DISP_E_CREATE;
        goto EXIT;
    }
#endif

    vpp_ret = MV_VPP_Create(0,0, 1);
    if(vpp_ret != MV_VPP_OK)
    {
        ret= MV_DISP_E_CREATE;
        goto EXIT;
    }

    vpp_ret = MV_VPP_Reset();
    if(vpp_ret != MV_VPP_OK)
    {
        ret= MV_DISP_E_RST;
        goto EXIT;
    }

    vpp_ret = MV_VPP_Config();
    if(vpp_ret != MV_VPP_OK)
    {
        ret= MV_DISP_E_CFG;
        goto EXIT;
    }

#ifdef CONFIG_PANEL_FASTLOGO
    vpp_ret = MIPI_Config ();
    if(vpp_ret != MV_VPP_OK)
    {
        ret= MV_DISP_E_CFG;
        goto EXIT;
    }
#endif

    vpp_ret = DISP_SetFormat(CPCB_1, hdmi_bitdepth);
    if(vpp_ret != MV_VPP_OK)
    {
        ret= MV_DISP_E_RES;
        goto EXIT;
    }

#if defined (BL_DISPLAY_DUAL_PIP)
    vpp_ret = DISP_SetFormat(CPCB_2, hdmi_bitdepth);

    if(vpp_ret != MV_VPP_OK)
    {
        ret= MV_DISP_E_RES;
        goto EXIT;
    }
#endif

    vpp_ret = MV_VPP_SetHdmiTxControl(1);
    if(vpp_ret != MV_VPP_OK)
    {
        ret= MV_DISP_E_TX_CTRL;
        goto EXIT;
    }

EXIT:
    if(ret != 0)
    {
        dbg_printf(PRN_ERR, "%s - failed : %d,%d\n", __func__, ret, vpp_ret);
    }

    return  ret;
#else
    ret = MV_VPP_Init(resid);
    if(ret != 0)
    {
        dbg_printf(PRN_ERR, "MV_VPP_Init failed\n");
        return ret;
    }
    return 0;
#endif
}

int MV_DISP_LogoStart(int planeID, DISP_LOGO_CONFIG* logocfg, int win_width, int win_height)
{
#if (BERLIN_CHIP_VERSION != BERLIN_BG5CT) || defined(CONFIG_VPP_USE_FASTLOGO_TA)
    VPP_WIN showlogo_win;
    unsigned size;
    VBUF_INFO *p_vbuf;
    int ret = 0;
    int vpp_ret = 0;

#if defined(CONFIG_VPP_USE_FASTLOGO_TA)
    p_vbuf = (VBUF_INFO*)(uintptr_t)VPP_TZ_ALLOC(sizeof(VBUF_INFO));
#else
    static VBUF_INFO vbuf;
    p_vbuf = &vbuf;
#endif

    if (!logocfg)
        return MV_VPP_EBADPARAM;

    //dbg_printf(PRN_DEBUG_LEVEL,"[showlogo] start: buf=0x0%8x, width=%d, heigth=%d, stride=%d %x\n", buf, width, height, stride);
    //showlogo_init_irq();

    memset(p_vbuf,0,sizeof(VBUF_INFO));
    p_vbuf->m_pbuf_start    = (UINT32)(uintptr_t)logocfg->buffer; // base address of the frame buffer;
    p_vbuf->m_disp_offset   = 0;
    p_vbuf->m_active_left   = 0;
    p_vbuf->m_active_top    = 0;

    // adjust the following three fields to reflect the actual logo dimensions

    p_vbuf->m_buf_stride = logocfg->stride;
    p_vbuf->m_active_width = logocfg->width;
    p_vbuf->m_active_height = logocfg->height;

    size = p_vbuf->m_buf_stride * p_vbuf->m_active_height;

    showlogo_win.x = 0;
    showlogo_win.y = 0;
    showlogo_win.width  = win_width;
    showlogo_win.height = win_height;

    vpp_ret = MV_VPP_OpenDispWindow(planeID, &showlogo_win, &showlogo_attr);
    if(vpp_ret != MV_VPP_OK)
    {
       ret= MV_DISP_E_OPEN;
       goto EXIT;
    }

/* TBD : Discuss/Finalize HDMI/DSI(only correct size logo is
 * available) o/p handling for mismatch of FB size & RES size */
#ifdef CONFIG_PANEL_FASTLOGO
    showlogo_win.width  = logocfg->width;
    showlogo_win.height = logocfg->height;
#endif

#if defined(CONFIG_VPP_USE_FASTLOGO_TA)
    ret = MV_VPPOBJ_SetRefWindow(0, planeID, &showlogo_win);
    if(vpp_ret != MV_VPP_OK)
    {
       vpp_ret= MV_DISP_E_WIN;
       goto EXIT;
    }
#endif

    flush_dcache_range(logocfg->buffer, (void *)(((char *)logocfg->buffer) + size));
    dbg_printf(PRN_NOTICE, "[fastlogo]:Show Logo Start !\n");

    //this call will enable isr
    MV_DisplayFrame(planeID, p_vbuf, SRCFMT_YUV422, p_vbuf->m_active_left,\
                    p_vbuf->m_active_top, logocfg->width, logocfg->height, INPUT_BIT_DEPTH_8BIT);

EXIT:
    if(ret != 0)
    {
        dbg_printf(PRN_ERR, "%s - failed : %d, %d\n", __func__, ret, vpp_ret);
    }

    return  ret;

#else
    MV_VPP_DisplayPatternFrame(0, 0, logocfg->width, logocfg->height, logocfg->stride, logocfg->buffer);
    return 0;
#endif
}


int MV_DISP_ConfigDisplay(int res_width, int res_height, int gfb_width, int gfb_height)
{
#if (BERLIN_CHIP_VERSION != BERLIN_BG5CT) ||  defined(CONFIG_VPP_USE_FASTLOGO_TA)
    VPP_WIN showlogo_win;
    int ret = 0;
    int vpp_ret = 0;

    showlogo_win.x = 0;
    showlogo_win.y = 0;
    showlogo_win.width  = res_width;
    showlogo_win.height = res_height;

    //graphics
    vpp_ret = MV_VPP_OpenDispWindow(PLANE_GFX1, &showlogo_win, &showlogo_attr);
    if(vpp_ret != MV_VPP_OK)
    {
       ret = MV_DISP_E_OPEN;
       goto EXIT;
    }

#if defined(CONFIG_VPP_USE_FASTLOGO_TA)
    showlogo_win.width  = gfb_width;
    showlogo_win.height = gfb_height;

    vpp_ret = MV_VPPOBJ_SetRefWindow(0, PLANE_GFX1, &showlogo_win);
    if(vpp_ret != MV_VPP_OK)
    {
       ret = MV_DISP_E_WIN;
       goto EXIT;
    }
#endif
    MV_VPP_SetDislayMode(PLANE_GFX1);

    ret = create_global_desc_array(SRCFMT_ARGB32, INPUT_BIT_DEPTH_8BIT, gfb_width, gfb_height);

EXIT:
    if(ret != 0)
    {
        dbg_printf(PRN_ERR, "%s - failed : %d, %d\n", __func__, ret, vpp_ret);
    }

    return  ret;
#else
    return 0;
#endif
}

void MV_DISP_Display_Frame(void *buf,int width,int height)
{
#if (BERLIN_CHIP_VERSION != BERLIN_BG5CT) ||  defined(CONFIG_VPP_USE_FASTLOGO_TA)
    unsigned size;
    VBUF_INFO *vbuf;
    get_vbuf_info(&vbuf);
    vbuf->m_pbuf_start   = (long)buf;
    vbuf->m_disp_offset  = 0;
    vbuf->m_active_left  = 0;
    vbuf->m_active_top   = 0;
    vbuf->m_buf_stride = width*4;
    vbuf->m_active_width = width;
    vbuf->m_active_height = height;
    size = vbuf->m_buf_stride * vbuf->m_active_height;

    if(buf) {
        flush_dcache_range(buf, (void *)(((char *)buf) + size));
    }
    MV_VPPOBJ_RecycleFrames(0,PLANE_GFX1);
    MV_VPP_DisplayFrame(PLANE_GFX1,vbuf);
#else
    MV_VPP_DisplayGfxFrame(buf, width, height);
#endif
}

void MV_DISP_MuteDisplay(int mute)
{
#if (BERLIN_CHIP_VERSION != BERLIN_BG5CT) ||  defined(CONFIG_VPP_USE_FASTLOGO_TA)
    MV_VPPOBJ_SetPlaneMute(0,PLANE_GFX1,mute);
#endif
}
void MV_DISP_StopDisplay(void)
{
#if (BERLIN_CHIP_VERSION != BERLIN_BG5CT) ||  defined(CONFIG_VPP_USE_FASTLOGO_TA)
   loop_isr_count=0;
   while(loop_isr_count<WAIT_LOOP_COUNT);
   MV_DISP_MuteDisplay(1);
#endif
}

void MV_DISP_DestroyDisplay(void)
{
#if (BERLIN_CHIP_VERSION != BERLIN_BG5CT) ||  defined(CONFIG_VPP_USE_FASTLOGO_TA)
    MV_DISP_StopDisplay();
#if !defined(CONFIG_VPP_USE_FASTLOGO_TA)
    MV_VPP_Stop();
#else
    MV_DISP_DisableIRQ();
#endif

    if(BOOTMODE_RECOVERY == get_bootmode())
        MV_VPP_Stop();
    else
        MV_VPP_Destroy();
#else
    MV_VPP_Deinit();
#endif
    dbg_printf(PRN_DEBUG_LEVEL,"[Display] stopped\n");
    return;
}

#ifdef CONFIG_PANEL_FASTLOGO
void pwm_backlight_enable (int ch, int on)
{
    /*PWM backlight settings*/
    unsigned int reg;
    unsigned int scale = 0x0; /*4096*/
    unsigned int pol = 0;
    unsigned int tcnt = 25000;
    unsigned int duty = 20000;
    unsigned int reg_base = MEMMAP_PWM_REG_BASE;

    reg = (scale & 0x7) | ((pol & 0x1) << 3);
    writel(reg, (reg_base + RA_PWM_pwmCh0Ctrl +(0x10*ch)));

    reg = (duty & 0xFFFF);
    writel(reg, (reg_base + RA_PWM_pwmCh0Duty +(0x10*ch)));

    reg = (tcnt & 0xFFFF);
    writel(reg, (reg_base + RA_PWM_pwmCh0TCnt +(0x10*ch)));

    //enable/disable
    reg = on?0x1:0x0;
    writel(reg, (reg_base + RA_PWM_pwmCh0En+(0x10*ch)));
    return;
}

void configure_panel_backlight(void)
{
    unsigned char buff[2], rbuff[2];

    i2c_master_init(0, 100, 0);

    /*LP8556 initialization*/
    buff[0] = 0x00;
    buff[1] = 0xFF;//brightness
    i2c_master_write_and_read(0, 0x2c, buff, 2, rbuff, 0);
    buff[0] = 0x01;
    buff[1] = 0x00; /*control*/
    i2c_master_write_and_read(0, 0x2c, buff, 2, rbuff, 0);
    buff[0] = 0xA2;
    buff[1] = 0x28;
    i2c_master_write_and_read(0, 0x2c, buff, 2, rbuff, 0);
    i2c_master_write_and_read(0, 0x2c, buff, 1, rbuff, 1);
    dbg_printf(PRN_ERR, "configure_panel_backlight:..: 0x%x\r\n", rbuff[0]);

    return;
}

void MV_DISP_Backlight_Enable(int resid)
{
    pwm_backlight_enable(1,1);
    if(resid == RES_DSI_800x1280P60)
        configure_panel_backlight();
}

int MIPI_Config (void)
{
    VPP_MIPI_LOAD_CONFIG    MipiCfg;
    int ret;

    MipiCfg.noOfresID = NO_OF_PANELCONFIG;
    MipiCfg.vppMipiCfgPA  = (UINT32)(uintptr_t)ppMipiConfigParams;

    /* load the resinfo table and init CMD of each resolution to TA.
       Selection of panel will be handled during Setformat */
    ret = MV_VPPOBJ_LoadMipiConfig (0, &MipiCfg);
    if(ret)
    {
        dbg_printf(PRN_ERR, "[fastlogo]: MIPI Load info params failed[%d]!\n",ret);
    }

    return ret;
}

static int Mipi_UpdateCommandBuffer (VPP_MIPI_CMD_TYPE enCmdType, UINT8* pbuf,
                                     CMDBUFINFO *pcmdbufinfo)
{
    VPP_MIPI_CMD_HEADER *pcmdHeader;
    void*  pbufferStart;
    UINT16 cmdstartOffset, maxcmdsize, size;

    if (!pbuf || !pcmdbufinfo)
        return VPP_E_BADPARAM;

    cmdstartOffset = (enCmdType == VPP_CMD_TYPE_INIT) ? MIPI_INIT_CMD_START :
                                                        MIPI_STANDBY_CMD_START;
    maxcmdsize = (enCmdType == VPP_CMD_TYPE_INIT) ? MAX_MIPI_INIT_CMD_SIZE :
                                                 MAX_MIPI_STANDBY_CMD_SIZE;
    if (enCmdType == VPP_CMD_TYPE_INIT)
    {
        pbufferStart = pcmdbufinfo->pcmd;
        size = pcmdbufinfo->size;
    } else {
        pbufferStart = pcmdbufinfo->pstbycmd;
        size = pcmdbufinfo->stbycmdsize;
    }

    if(size > maxcmdsize)
    {
        dbg_printf(PRN_ERR, "[fastlogo]: MIPI Command exceed size!\n");
        return VPP_E_BADPARAM;
    }

    pcmdHeader = (VPP_MIPI_CMD_HEADER*)&pbuf[cmdstartOffset];
    pcmdHeader->cmd_type = enCmdType;
    pcmdHeader->cmd_size = size;
    memcpy((pbuf + cmdstartOffset + MIPI_CMD_HEADER_SIZE),
            pbufferStart, size);

    return MV_VPP_OK;
}

int Mipi_LoadInfoTable(void)
{
    VPP_MIPI_CONFIG_PARAMS* pMipiConfigParams;
    UINT32 buffer;
    void *pbuf;
    int i;
    VPLL_CONFIG pllcfg;
    RESOLUTION_INFO*  rescfg;
    CMDBUFINFO  acmdbuf[NO_OF_PANELCONFIG];
    int resid = PANEL_RESID;

    ppMipiConfigParams = VPP_TZ_ALLOC(sizeof(VPP_MIPI_CONFIG_PARAMS) * NO_OF_PANELCONFIG);
    if (!ppMipiConfigParams)
    {
        dbg_printf(PRN_ERR, "[fastlogo]: MIPI Config buffer alloc fail!\n");
        return VPP_E_NOMEM;
    }

    memset (ppMipiConfigParams, 0, (sizeof(VPP_MIPI_CONFIG_PARAMS) * NO_OF_PANELCONFIG));

    PANEL_CONFIG_PARAM (1);

#if (NO_OF_PANELCONFIG > 1)
    PANEL_CONFIG_PARAM (2);
#endif

#if (NO_OF_PANELCONFIG > 2)
#error "Configure the resolution table for additional resid"
#endif

    for (i=0; i<NO_OF_PANELCONFIG; i++)
    {
        pMipiConfigParams =  (((VPP_MIPI_CONFIG_PARAMS*)ppMipiConfigParams) + i);
        if(pMipiConfigParams->initparams.resId == resid)
        {
            rescfg = &pMipiConfigParams->infoparams.resInfo;
            RESINFO_PARAM_UPDATE(active_width);
            RESINFO_PARAM_UPDATE(active_height);
            RESINFO_PARAM_UPDATE(hfrontporch);
            RESINFO_PARAM_UPDATE(hsyncwidth);
            RESINFO_PARAM_UPDATE(hbackporch);
            RESINFO_PARAM_UPDATE(vfrontporch);
            RESINFO_PARAM_UPDATE(vsyncwidth);
            RESINFO_PARAM_UPDATE(vbackporch);
            RESINFO_PARAM_UPDATE(type);
            RESINFO_PARAM_UPDATE(scan);
            RESINFO_PARAM_UPDATE(frame_rate);
            RESINFO_PARAM_UPDATE(freq);
            RESINFO_PARAM_UPDATE(flag_3d);
            RESINFO_PARAM_UPDATE(pts_per_cnt_4);

            m_resinfo_table[RES_DSI_CUSTOM].width = rescfg->active_width+\
                                           rescfg->hfrontporch+\
                                           rescfg->hsyncwidth+\
                                           rescfg->hbackporch;

            m_resinfo_table[RES_DSI_CUSTOM].height = rescfg->active_height+\
                                           rescfg->vfrontporch+\
                                           rescfg->vsyncwidth+\
                                           rescfg->vbackporch;

            memset(&pllcfg, 0, sizeof(VPLL_CONFIG));
            /* Fill the PLL and config the clock */
            pllcfg.h_active = rescfg->active_width;
            pllcfg.v_active = rescfg->active_height;
            pllcfg.isInterlaced = rescfg->scan;
            pllcfg.frame_rate_enum = rescfg->frame_rate;
            pllcfg.h_total = m_resinfo_table[RES_DSI_CUSTOM].width;
            pllcfg.v_total = m_resinfo_table[RES_DSI_CUSTOM].height;

            if ( MV_VPP_OK != AVPLL_GetClkgenparams  (m_resinfo_table[RES_DSI_CUSTOM].freq,
                                                  &pllcfg.Dm,
                                                  &pllcfg.Dn,
                                                  &pllcfg.frac,
                                                  &pllcfg.Dp))
                return MV_VPP_EUNSUPPORT;

            AVPLL_Load_ResConfig(&pllcfg);
            break;
        }
    }

    if (i>=NO_OF_PANELCONFIG)
    {
        dbg_printf(PRN_ERR, "[fastlogo]: RESID[%d] not configuration\n", resid);
        return MV_VPP_EBADPARAM;
    }

    /* Allocate buffers for the commands and copy the commands */
    for (i=0; i<NO_OF_PANELCONFIG; i++)
    {
        pMipiConfigParams = (((VPP_MIPI_CONFIG_PARAMS*)ppMipiConfigParams) + i);

        VPP_ALLOC (0, (MAX_CMD_BUF_SIZE), 32, &buffer);
        if(!buffer)
        {
            dbg_printf(PRN_ERR, "[fastlogo]: Cmd Buffer Alloc fail!\n");
            return VPP_E_NOMEM;
        }
        pbuf = (void*)(uintptr_t)buffer;
        pMipiConfigParams->vppMipiCmd.pcmd = buffer;
        Mipi_UpdateCommandBuffer(VPP_CMD_TYPE_INIT, pbuf, &acmdbuf[i]);
        Mipi_UpdateCommandBuffer(VPP_CMD_TYPE_STANDBY, pbuf, &acmdbuf[i]);
        pMipiConfigParams->vppMipiCmd.bufsize = acmdbuf[i].size + acmdbuf[i].stbycmdsize;
        flush_dcache_range(pbuf, (void *)(((char *)pbuf) + MAX_CMD_BUF_SIZE));
    }

    return 0;
}
#endif
