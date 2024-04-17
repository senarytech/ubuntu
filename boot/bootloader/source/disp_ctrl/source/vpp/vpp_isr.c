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

#define _VPP_ISR_C_
#include "vpp_module.h"
#include "vpp_cfg.h"
#include "vpp_bcmbuf.h"
#include "api_avio_dhub.h"
#include "vpp_api.h"
#include "io.h"
#include "disp_ctrl.h"
#include "printf.h"
#include "vpp_cmds.h"

#define avioDhubSemMap_vpp_vppCPCB0_intr avioDhubSemMap_vpp128b_vpp_inr0
#define avioDhubSemMap_vpp_vppOUT4_intr avioDhubSemMap_vpp128b_vpp_inr6
#define bTST(x, b) (((x) >> (b)) & 1)

#define VPP_BCMBUF_WRITE(a, d) VPP_BCMBUF_Write(vpp_obj->pVbiBcmBuf, a, d)
#define VPP_BCMBUF_WRITE_CLEARBUF(a, d) \
                VPP_BCMBUF_Write(vpp_obj->pVbiClearBcmBuf, a, d)

static VOID VPP_CPCB_ISR_service(VPP_OBJ *vpp_obj);
static VOID disp_callback_dummy(VOID *param);

voidfunc_t disp_isr_service = disp_callback_dummy;
voidfunc_t disp_after_isr_service = disp_callback_dummy;
voidfunc_t disp_dsr_service = disp_callback_dummy;

static UINT32 g_vpp_isr_status;
static VOID disp_callback_dummy(VOID *param)
{
    (void)param;
}

VOID VPP_RegisterInterruptService(voidfunc_t f1, voidfunc_t f2, voidfunc_t f3)
{
    if (NULL != f1)
        disp_isr_service = f1;
    if (NULL != f2)
        disp_after_isr_service = f2;
    if (NULL != f3)
        disp_dsr_service = f3;
    return;
}

void VPP_Enable_ISR_Interrupt(VPP_OBJ *vpp_obj, int cpcbID)
{
    vpp_obj->pSemHandle = dhub_semaphore(&VPP_dhubHandle.dhub);
    if (cpcbID == CPCB_1) {
        /* configure and enable CPCB0 interrupt */
        semaphore_cfg(vpp_obj->pSemHandle,
                avioDhubSemMap_vpp_vppCPCB0_intr,
                1, 0);
        semaphore_clr_full(vpp_obj->pSemHandle,
                avioDhubSemMap_vpp_vppCPCB0_intr);
        semaphore_intr_enable(vpp_obj->pSemHandle,
                avioDhubSemMap_vpp_vppCPCB0_intr, 0,
                1, 0, 0, 0);
        semaphore_cfg(vpp_obj->pSemHandle,
                avioDhubSemMap_vpp_vppOUT4_intr,
                1, 0);
        semaphore_clr_full(vpp_obj->pSemHandle,
                avioDhubSemMap_vpp_vppOUT4_intr);
        semaphore_intr_enable(vpp_obj->pSemHandle,
                avioDhubSemMap_vpp_vppOUT4_intr, 0,
                1, 0, 0, 0);
    }
}

void VPP_Disable_ISR_Interrupt(VPP_OBJ *vpp_obj, int cpcbID)
{
    vpp_obj->pSemHandle = dhub_semaphore(&VPP_dhubHandle.dhub);
    if (cpcbID == CPCB_1) {
        semaphore_clr_full(vpp_obj->pSemHandle,
                avioDhubSemMap_vpp_vppCPCB0_intr);
        semaphore_intr_enable(vpp_obj->pSemHandle,
                avioDhubSemMap_vpp_vppCPCB0_intr, 0,
                0, 0, 0, 0);
        semaphore_clr_full(vpp_obj->pSemHandle,
                avioDhubSemMap_vpp_vppOUT4_intr);
        semaphore_intr_enable(vpp_obj->pSemHandle,
                avioDhubSemMap_vpp_vppOUT4_intr, 0,
                0, 0, 0, 0);
    }
    return;
}

static void updateFEChannelCurrentFrame(VPP_OBJ *vpp_obj)
{
    VBUF_INFO *pinfo;

    if (vpp_obj->dvstatus == STATUS_DISP_VIDEO) {
        /* recycle only when new frame is popped */
        if (vpp_obj->prev_curr_frame != vpp_obj->curr_frame &&
                vpp_obj->prev_curr_frame)
            frmq_push(&vpp_obj->outputq, vpp_obj->prev_curr_frame);

        vpp_obj->prev_curr_frame = vpp_obj->curr_frame;
        vpp_obj->curr_frame = NULL;
        /* try to get a new frame descriptor from frame queue */
        while (frmq_pop(&vpp_obj->inputq, (void **)&pinfo)) {
            frmq_pop_commit(&vpp_obj->inputq);
            /* recycle current frame descriptor */
            if (vpp_obj->curr_frame) {
                frmq_push(&vpp_obj->outputq,
                    vpp_obj->curr_frame);
            }
            vpp_obj->curr_frame = pinfo;
        }
        /*repeat the old frame if no one is not popped*/
        if (!vpp_obj->curr_frame)
            vpp_obj->curr_frame = vpp_obj->prev_curr_frame;
        vpp_obj->frm_count++;
    }
}

static int startChannelDataLoader(VPP_OBJ *vpp_obj)
{
    VBUF_INFO *pinfo;
    UINT32 *cfgQ, *cfgQ_shadow;
    UINT32 *cfgQ_len;

    cfgQ_shadow = vpp_obj->curr_cpcb_vbi_dma_cfgQ->addr;
    cfgQ_len = &(vpp_obj->curr_cpcb_vbi_dma_cfgQ->len);

    if (vpp_obj->dvstatus == STATUS_DISP_VIDEO)
        pinfo = vpp_obj->curr_frame;
    else
        return MV_VPP_OK;
    if (pinfo == NULL)
        return MV_VPP_OK;
    cfgQ = cfgQ_shadow + *cfgQ_len * 2;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
    *cfgQ_len += START_2NDDMA(&VPP_dhubHandle, vpp_obj->dmaRID,
            (UINT32)pinfo->m_pbuf_start,
            pinfo->m_buf_stride, pinfo->m_buf_stride, 1,
            pinfo->m_buf_stride, pinfo->m_active_height, (T64b *) cfgQ);
#pragma GCC diagnostic pop
    return MV_VPP_OK;
}

int VPP_ISR_Handler(UINT32 msg_id, UINT32 msg_para, VPP_OBJ *vpp_obj)
{
    int instat;
    HRESULT rc;
    unsigned int EmptySts;

    if (vpp_obj->status != STATUS_ACTIVE)
        return 0;

    if (VPP_CC_MSG_TYPE_VPP == msg_id) {
        instat = msg_para;
        /* CPCB-0 VBI interrupt */
        if (bTST(instat, avioDhubSemMap_vpp_vppCPCB0_intr)) {
            BCM_SCHED_GetEmptySts(0, &EmptySts);

            if (EmptySts == 0) {
                vpp_obj->skip_vde_int = 1;
                return 0;
            } else {
                vpp_obj->skip_vde_int = 0;
            }

            disp_dsr_service((void *)vpp_obj);

            /* current VBI BCM cfgQ */
            vpp_obj->curr_vbi_bcm_cfgQ =
                vpp_obj->curr_cpcb_vbi_bcm_cfgQ;

            /* reset vbi bcm & dma cfgQ length */
            vpp_obj->curr_cpcb_vbi_bcm_cfgQ->len = 0;
            vpp_obj->curr_cpcb_vbi_dma_cfgQ->len = 0;
            /* select BCM buffer for the CPCB */
            vpp_obj->pVbiBcmBuf =
                vpp_obj->pVbiBcmBufCpcb[CPCB_1];

            /* select BCM sub-buffer to dump register settings */
            VPP_BCMBUF_Select(vpp_obj->pVbiBcmBuf, CPCB_1);

            /* select CLEAR BCM buffer for the CPCB */
            vpp_obj->pVbiClearBcmBuf =
                vpp_obj->pVbiClearBcmBufCpcb[CPCB_1];

            VPP_BCMBUF_Select(vpp_obj->pVbiClearBcmBuf, CPCB_1);

            disp_isr_service(vpp_obj);

            VPP_CPCB_ISR_service(vpp_obj);
            /* generate CPCB-0 CLEAR BCM DHUB cmdQ and,*/
            rc = VPP_BCMBUF_To_CFGQ(vpp_obj->pVbiClearBcmBuf,
                    vpp_obj->curr_vbi_bcm_cfgQ);

            if (rc == MV_VPP_OK) {
                if (vpp_obj->pVbiClearBcmBufCpcb[CPCB_1] ==
                        &vpp_obj->vbi_clear_bcm_buf[0])
                    vpp_obj->pVbiClearBcmBufCpcb[CPCB_1] =
                        &vpp_obj->vbi_clear_bcm_buf[1];
                else
                    vpp_obj->pVbiClearBcmBufCpcb[CPCB_1] =
                        &vpp_obj->vbi_clear_bcm_buf[0];
            }

            /* generate CPCB-0 BCM DHUB cmdQ and,*/
            rc = VPP_BCMBUF_To_CFGQ(vpp_obj->pVbiBcmBuf,
                    vpp_obj->curr_vbi_bcm_cfgQ);
            VPP_CFGQ_To_CFGQ(vpp_obj->curr_cpcb_vbi_dma_cfgQ,
                    vpp_obj->curr_vbi_bcm_cfgQ);

            if (rc == MV_VPP_OK) {
                /* switch BCM buffer for the CPCB */
                if (vpp_obj->pVbiBcmBufCpcb[CPCB_1] ==
                        &vpp_obj->vbi_bcm_buf[0])
                    vpp_obj->pVbiBcmBufCpcb[CPCB_1] =
                        &vpp_obj->vbi_bcm_buf[1];
                else
                    vpp_obj->pVbiBcmBufCpcb[CPCB_1] =
                        &vpp_obj->vbi_bcm_buf[0];
            }

            rc = VPP_BCMDHUB_CFGQ_Commit(vpp_obj->curr_vbi_bcm_cfgQ,
                    CPCB_1, 0);

            if (vpp_obj->curr_cpcb_vbi_dma_cfgQ ==
                        &vpp_obj->vbi_dma_cfgQ[0])
                vpp_obj->curr_cpcb_vbi_dma_cfgQ =
                        &(vpp_obj->vbi_dma_cfgQ[1]);
            else
                vpp_obj->curr_cpcb_vbi_dma_cfgQ =
                        &(vpp_obj->vbi_dma_cfgQ[0]);

            if (vpp_obj->curr_cpcb_vbi_bcm_cfgQ ==
                        &vpp_obj->vbi_bcm_cfgQ[0])
                vpp_obj->curr_cpcb_vbi_bcm_cfgQ =
                        &(vpp_obj->vbi_bcm_cfgQ[1]);
            else
                vpp_obj->curr_cpcb_vbi_bcm_cfgQ =
                        &(vpp_obj->vbi_bcm_cfgQ[0]);
        }

    }
    return MV_VPP_OK;
}

void VPP_CPCB_ISR_service(VPP_OBJ *vpp_obj)
{
    static int first_time_init = 1;

    vpp_obj->vbi_num++;
    if ((vpp_obj->dvstatus == STATUS_INACTIVE) ||
                (vpp_obj->vbi_num <= 2))
        return;

    if (vpp_obj->dvstatus != STATUS_INACTIVE) {

        if(first_time_init == 1)
        {
            exec_command(CMD_ID_7);
            first_time_init = 0;
        }
        else
        {
            if(vpp_obj->dmaRID == avioDhubChMap_vpp128b_GFX_R1)
            {
                static int first_time_init_gfx = 1;
                if(first_time_init_gfx == 1)
                {
                    exec_command_gfx(CMD_ID_7);
                    first_time_init_gfx = 0;
                }
                else
                {
                    exec_command_gfx(CMD_ID_8);
                }
            }
            else
            {
                exec_command(CMD_ID_8);
            }
        }
        updateFEChannelCurrentFrame(vpp_obj);
        startChannelDataLoader(vpp_obj);
    }
    return;
}

VOID MV_VPP_EnableISR(VPP_OBJ *pVpp_obj)
{
    if(!g_vpp_isr_status) {
        VPP_Enable_ISR_Interrupt(pVpp_obj, CPCB_1);
        g_vpp_isr_status = 1;
    }
}

VOID MV_VPP_DisableISR(VPP_OBJ *pVpp_obj)
{
    if(g_vpp_isr_status) {
        VPP_Disable_ISR_Interrupt(pVpp_obj, CPCB_1);
        g_vpp_isr_status = 0;
    }
}
