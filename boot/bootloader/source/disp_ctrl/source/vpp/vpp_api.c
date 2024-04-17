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

#include "printf.h"
#include "io.h"
#include "soc.h"
#include "vpp_module.h"
#include "vpp_cfg.h"
#include "vpp_api.h"
#include "vpp_mem.h"
#include "api_dhub.h"
#include "avio.h"
#include "Galois_memmap.h"
#include "apbRegBase.h"
#include "disp_ctrl.h"
#include "vpp_cmds.h"

#define HDMITX_ENABLE_TIMEOUT
#define HDMITX_TIMEOUT_COUNT  500

#define avioDhubSemMap_vpp_vppCPCB0_intr avioDhubSemMap_vpp128b_vpp_inr0
#define avioDhubSemMap_vpp_vppOUT4_intr avioDhubSemMap_vpp128b_vpp_inr6
#define bTST(x, b) (((x) >> (b)) & 1)

#define MEMMAP_AVIO_REG_BASE 0xF7400000

VPP_OBJ *pVppobj;
volatile unsigned logo_isr_count;

extern int VPP_ISR_Handler(UINT32 msg_id, UINT32 msg_para, VPP_OBJ *vpp_obj);
void show_logoframe_isr(void);
extern int register_isr(void (*isr)(void), int irq_id);
extern void set_irq_enable(int irq_id);

static void enable_irq()
{
    if(register_isr(show_logoframe_isr, IRQ_dHubIntrAvio0_0))
        printf("vpp isr can't be registered\n");

    set_irq_enable(IRQ_dHubIntrAvio0_0);
}

void show_logoframe_isr()
{
    ++logo_isr_count;
    VPP_ISR_Handler_irq();
}

VOID MV_VPPOBJ_CloseDispWindow()
{
    GA_REG_WORD32_WRITE(0xf74b004c, 0x00000003);
}

INT32  MV_DisplayFrame(VPP_OBJ *pVpp_obj, VBUF_INFO *vbufinfo)
{
    if (frmq_push(&(pVpp_obj->inputq), vbufinfo) == 0)
        return MV_VPP_EFRAMEQFULL;
    return 0;
}

VOID MV_VPP_RecycleFrames(VOID *param)
{
    VOID *frame = NULL;
    VPP_OBJ *pVpp_obj = (VPP_OBJ *)param;
    if (frmq_pop(&(pVpp_obj->outputq), (VOID **)&frame))
        frmq_pop_commit(&(pVpp_obj->outputq));
}

INT32 MV_VPP_CONFIG(INT32 cpcbID, VPP_OBJ **pVpp)
{
    VPP_OBJ *vpp_obj;
    INT32 result = MV_VPP_OK;

    vpp_obj = GaloisMalloc(sizeof(VPP_OBJ));

    if (vpp_obj == NULL) {
        printf("Failed to allocate mem for vpp object\n");
        *pVpp = NULL;
        return MV_VPP_ENOMEM;
    }

    *pVpp = vpp_obj;
    /* create VBI BCM buffer */
    if (VPP_BCMBUF_Create(&(vpp_obj->vbi_bcm_buf[0]),
            BCM_BUFFER_SIZE) != MV_VPP_OK) {
        result = MV_VPP_ENOMEM;
        goto cleanup;
    }

    if (VPP_BCMBUF_Create(&(vpp_obj->vbi_bcm_buf[1]),
            BCM_BUFFER_SIZE) != MV_VPP_OK) {
        result = MV_VPP_ENOMEM;
        goto cleanup;
    }
    if (VPP_BCMBUF_Create(&(vpp_obj->vbi_clear_bcm_buf[0]),
            BCM_BUFFER_SIZE) != MV_VPP_OK) {
        result = MV_VPP_ENOMEM;
        goto cleanup;
    }
    if (VPP_BCMBUF_Create(&(vpp_obj->vbi_clear_bcm_buf[0]),
            BCM_BUFFER_SIZE) != MV_VPP_OK) {
        result = MV_VPP_ENOMEM;
        goto cleanup;
    }
    if (VPP_BCMBUF_Create(&(vpp_obj->vbi_clear_bcm_buf[1]),
            BCM_BUFFER_SIZE) != MV_VPP_OK){
        result = MV_VPP_ENOMEM;
        goto cleanup;
    }
    (vpp_obj->vbi_dma_cfgQ[0]).handle = GaloisMalloc(DMA_CMD_BUFFER_SIZE);
    if((vpp_obj->vbi_dma_cfgQ[0]).handle == NULL) {
        goto cleanup;
    }
    vpp_obj->vbi_dma_cfgQ[0].addr = (vpp_obj->vbi_dma_cfgQ[0]).handle;
    vpp_obj->vbi_dma_cfgQ[0].phy_addr = (vpp_obj->vbi_dma_cfgQ[0]).handle;
    (&vpp_obj->vbi_dma_cfgQ[0])->len = 0;

    (vpp_obj->vbi_dma_cfgQ[1]).handle = GaloisMalloc(DMA_CMD_BUFFER_SIZE);
    if((vpp_obj->vbi_dma_cfgQ[1]).handle == NULL){
        goto cleanup;
    }
    vpp_obj->vbi_dma_cfgQ[1].addr = (vpp_obj->vbi_dma_cfgQ[1]).handle;
    vpp_obj->vbi_dma_cfgQ[1].phy_addr = (vpp_obj->vbi_dma_cfgQ[1]).handle;
    (&vpp_obj->vbi_dma_cfgQ[1])->len = 0;

    vpp_obj->curr_cpcb_vbi_dma_cfgQ = &(vpp_obj->vbi_dma_cfgQ[0]);

    (vpp_obj->vbi_bcm_cfgQ[0]).handle = GaloisMalloc(DMA_CMD_BUFFER_SIZE);
    if((vpp_obj->vbi_bcm_cfgQ[0]).handle == NULL){
        goto cleanup;
    }
    vpp_obj->vbi_bcm_cfgQ[0].addr = vpp_obj->vbi_bcm_cfgQ[0].handle;
    vpp_obj->vbi_bcm_cfgQ[0].phy_addr = vpp_obj->vbi_bcm_cfgQ[0].handle;
    (&vpp_obj->vbi_bcm_cfgQ[0])->len = 0;

    (vpp_obj->vbi_bcm_cfgQ[1]).handle = GaloisMalloc(DMA_CMD_BUFFER_SIZE);
    if((vpp_obj->vbi_bcm_cfgQ[1]).handle == NULL){
        goto cleanup;
    }
    vpp_obj->vbi_bcm_cfgQ[1].addr = vpp_obj->vbi_bcm_cfgQ[1].handle;
    vpp_obj->vbi_bcm_cfgQ[1].phy_addr = vpp_obj->vbi_bcm_cfgQ[1].handle;
    (&vpp_obj->vbi_bcm_cfgQ[1])->len = 0;

    vpp_obj->curr_cpcb_vbi_bcm_cfgQ = &(vpp_obj->vbi_bcm_cfgQ[0]);

    /* reset VBI BCM buffer */
    VPP_BCMBUF_Reset(&vpp_obj->vbi_bcm_buf[0]);
    vpp_obj->pVbiBcmBuf = &vpp_obj->vbi_bcm_buf[0];
    VPP_BCMBUF_Reset(&vpp_obj->vbi_bcm_buf[1]);
    vpp_obj->pVbiBcmBufCpcb[0] = &vpp_obj->vbi_bcm_buf[0];
    vpp_obj->pVbiBcmBufCpcb[1] = &vpp_obj->vbi_bcm_buf[0];
    vpp_obj->pVbiBcmBufCpcb[2] = &vpp_obj->vbi_bcm_buf[0];
    /* reset VBI CLEAR BCM buffer */
    VPP_BCMBUF_Reset(&vpp_obj->vbi_clear_bcm_buf[0]);
    vpp_obj->pVbiClearBcmBuf = &vpp_obj->vbi_clear_bcm_buf[0];
    VPP_BCMBUF_Reset(&vpp_obj->vbi_clear_bcm_buf[1]);
    vpp_obj->pVbiClearBcmBufCpcb[0] = &vpp_obj->vbi_clear_bcm_buf[0];
    vpp_obj->pVbiClearBcmBufCpcb[1] = &vpp_obj->vbi_clear_bcm_buf[0];
    vpp_obj->pVbiClearBcmBufCpcb[2] = &vpp_obj->vbi_clear_bcm_buf[0];

    exec_command(CMD_ID_2);

    vpp_obj->pSemHandle = dhub_semaphore(&VPP_dhubHandle.dhub);

    exec_command(CMD_ID_3);

    VPP_RegisterInterruptService(MV_VPP_RecycleFrames, NULL, NULL);
    BCM_SCHED_SetMux(BCM_SCHED_Q0, 0);
    BCM_SCHED_SetMux(BCM_SCHED_Q1, 11);
    vpp_obj->status = STATUS_ACTIVE;

    exec_command(CMD_ID_4);

    exec_command(CMD_ID_5);
    exec_command(CMD_ID_6);

    VPP_BCMBUF_Select(vpp_obj->pVbiBcmBuf, cpcbID);
    vpp_obj->vbi_num = 0;
    vpp_obj->dvstatus = STATUS_DISP_VIDEO;
    vpp_obj->curr_frame = NULL;
    vpp_obj->prev_curr_frame = NULL;
    vpp_obj->curr_still_picture = NULL;
    vpp_obj->still_picture = NULL;
    vpp_obj->frm_count = 0;
    vpp_obj->skip_vde_int = 0;
    /* reset frame descriptor queues */
    frmq_reset(&(vpp_obj->inputq));
    frmq_reset(&(vpp_obj->outputq));

#ifdef ENABLE_GFX
    vpp_obj->dmaRID = avioDhubChMap_vpp128b_GFX_R1;
#else
    vpp_obj->dmaRID = avioDhubChMap_vpp128b_MV_R0;
#endif

    return MV_VPP_OK;
cleanup:
    printf("Failed to allocate memory for VPP buffers\n");
    /*No need to check validity of pointer here*/
    VPP_BCMBUF_Destroy(&(vpp_obj->vbi_bcm_buf[0]));
    VPP_BCMBUF_Destroy(&(vpp_obj->vbi_bcm_buf[1]));
    VPP_BCMBUF_Destroy(&(vpp_obj->vbi_clear_bcm_buf[0]));
    VPP_BCMBUF_Destroy(&(vpp_obj->vbi_clear_bcm_buf[1]));
    return result;
}

VOID MV_VPP_DisplayPatternFrame(INT32 x, INT32 y, INT32 logo_width,
                INT32 logo_height, INT32 logo_stride, VOID *yuv_logo)
{
    VBUF_INFO *pVbufInfo = NULL;
    unsigned size;
    int i =0;
    char *dest = NULL;
    char *src = NULL;
    int align_x = logo_width % 8;
    int align_w = logo_width + align_x;
#ifdef ENABLE_GFX
    int align_stride = align_w*4;
#else
    int align_stride = align_w*2;
#endif

    if((logo_width <= 0 || logo_height <= 0) ||
        (x < 0 || y < 0))
    {
        printf("Error: Invalid width=%d and height=%d\n",
                logo_width, logo_height);
        return;
    }

    pVbufInfo = MV_VPP_GetFrame(x, y, align_w, logo_height, align_stride);
    if(pVbufInfo == NULL)
    {
        printf("Error: Memory not available for frame\n");
    }
    printf("MV_VPP_DisplayPatternFrame pVbufInfo->m_pbuf_start=0x%x "
            "pVbufInfo->m_buf_stride=%d pVbufInfo->m_active_height=%d\n",
            pVbufInfo->m_pbuf_start, pVbufInfo->m_buf_stride,
            pVbufInfo->m_active_height);

    dest = (char*)pVbufInfo->m_pbuf_start;
    src = (char*)yuv_logo;
    if(src == NULL || dest == NULL)
    {
        printf("Error: Invalid logo params\n");
        return;
    }
    for(i = 0; i < logo_height; i++)
    {
        memcpy(dest + (i * pVbufInfo->m_buf_stride),
                src + (i * logo_stride),
                logo_stride);
    }

    size = pVbufInfo->m_buf_stride * pVbufInfo->m_active_height;
    flush_dcache_range(pVbufInfo->m_pbuf_start,
            (void *)(((char *)pVbufInfo->m_pbuf_start) + size));
    if (pVbufInfo)
    {
        MV_DisplayFrame(pVppobj, pVbufInfo);
    }
}

static VOID setup_gfx()
{
    VPP_OBJ* vpp_obj = pVppobj;
    static int single = 1;

    if(single)
    {
        single = 0;
        MV_VPP_DisableISR(pVppobj);
        vpp_obj->dmaRID = avioDhubChMap_vpp128b_GFX_R1;
        exec_command_gfx(CMD_ID_3);
        exec_command_gfx(CMD_ID_4);

        exec_command_gfx(CMD_ID_5);
        exec_command_gfx(CMD_ID_6);
        MV_VPP_EnableISR(pVppobj);
    }
}
VOID MV_VPP_DisplayGfxFrame(void *buf,int width,int height)
{
    unsigned size;
    VBUF_INFO *vbuf;

    printf("GFX buf=%x width=%d height=%d\n", buf, width, height);
    vbuf = MV_VPP_GetFrame_gfx(0, 0, width, height, width*4);

    //Use the input buffer
    vbuf->m_pbuf_start = buf;

    size = vbuf->m_buf_stride * vbuf->m_active_height;
    flush_dcache_range(vbuf->m_pbuf_start,
            (void *)(((char *)vbuf->m_pbuf_start) + size));
    MV_VPP_RecycleFrames(pVppobj);
    MV_DisplayFrame(pVppobj, vbuf);

    setup_gfx();
}

VOID VPP_ISR_Handler_irq()
{
    VPP_OBJ* vpp_obj = pVppobj;
    int instat = 0;

    vpp_obj->pSemHandle = dhub_semaphore(&VPP_dhubHandle.dhub);
    instat = semaphore_chk_full(vpp_obj->pSemHandle, -1);
    if(!!(bTST(instat, avioDhubSemMap_vpp_vppCPCB0_intr)))
    {
        semaphore_pop(vpp_obj->pSemHandle,
                avioDhubSemMap_vpp_vppCPCB0_intr, 1);
        semaphore_clr_full(vpp_obj->pSemHandle,
                avioDhubSemMap_vpp_vppCPCB0_intr);

        VPP_ISR_Handler(0, instat, vpp_obj);
    }
    if(!!(bTST(instat, avioDhubSemMap_vpp_vppOUT4_intr)))
    {
        semaphore_pop(vpp_obj->pSemHandle,
                avioDhubSemMap_vpp_vppOUT4_intr, 1);
        semaphore_clr_full(vpp_obj->pSemHandle,
                avioDhubSemMap_vpp_vppOUT4_intr);
    }
}

static int dhub_init(void)
{
    GA_REG_WORD32_WRITE(MEMMAP_AVIO_REG_BASE +
        AVIO_MEMMAP_AVIO_BCM_REG_BASE + RA_AVIO_BCM_AUTOPUSH, 0x0);
    DhubInitialization(0, VPP_DHUB_BASE, VPP_HBO_SRAM_BASE, &VPP_dhubHandle,
            VPP_config, VPP_NUM_OF_CHANNELS, DHUB_TYPE_128BIT);
    DhubInitialization(0, AG_DHUB_BASE, AG_HBO_SRAM_BASE, &AG_dhubHandle,
            AG_config, AG_NUM_OF_CHANNELS, DHUB_TYPE_64BIT);
    return 0;
}

INT32 MV_VPP_Init(INT32 resid)
{
    int res = 0;

    dhub_init();
    init_resolution_index(resid);

    exec_command(CMD_ID_1);

    res = MV_VPP_CONFIG(CPCB_1, &pVppobj);
    if (pVppobj == NULL)
        return res;

    res = create_global_desc_array();
    if (res != MV_VPP_OK)
        return res;

    printf("MV_VPP_Init\n");
    return 0;
}

void MV_VPP_Enable_IRQ()
{
    logo_isr_count = 0;
    enable_irq();

    MV_VPP_EnableISR(pVppobj);
}

VOID MV_VPP_Deinit(VOID)
{
    while (logo_isr_count < MAX_ISR_COUNT);

    MV_VPPOBJ_CloseDispWindow();
    MV_VPP_DisableISR(pVppobj);
    destroy_global_desc_array();
    VPP_BCMBUF_Destroy(&(pVppobj->vbi_bcm_buf[0]));
    VPP_BCMBUF_Destroy(&(pVppobj->vbi_bcm_buf[1]));
    VPP_BCMBUF_Destroy(&(pVppobj->vbi_clear_bcm_buf[0]));
    VPP_BCMBUF_Destroy(&(pVppobj->vbi_clear_bcm_buf[1]));
}

int getFrmCount()
{
    return pVppobj->frm_count;
}
