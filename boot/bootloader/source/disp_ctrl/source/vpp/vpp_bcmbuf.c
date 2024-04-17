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

#include "vpp_bcmbuf.h"
#include "vpp_module.h"
#include "vpp_api.h"
#include "avio.h"
#include "printf.h"
#include "io.h"

static inline void FLUSH_DCACHE_RANGE(void* start, unsigned int size)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
    flush_dcache_range(start, (void*)(((int)(start))+(size)));
#pragma GCC diagnostic pop
}

/******************************************************************************
 * FUNCTION: allocate register programming buffer
 * PARAMS: *buf - pointer to a register programming buffer
 *       : size - size of the buffer to allocate
 *       :      - (should be a multiple of 4)
 * RETURN:  1 - succeed
 *          0 - failed to initialize a BCM buffer
 ******************************************************************************/
INT VPP_BCMBUF_Create(BCMBUF *pbcmbuf, int size)
{
    pbcmbuf->handle = NULL;
    pbcmbuf->handle = GaloisMalloc(size);
    if(pbcmbuf->handle == NULL) {
        printf("alloc failure\n");
        return -1;
    }
    pbcmbuf->size = size;
    pbcmbuf->head = pbcmbuf->handle;
    pbcmbuf->phy_addr = pbcmbuf->handle;
    return 0;
}

/******************************************************************************
 * FUNCTION: free register programming buffer
 * PARAMS: *buf - pointer to a register programming buffer
 * RETURN:  1 - succeed
 *          0 - failed to initialize a BCM buffer
 ******************************************************************************/
INT VPP_BCMBUF_Destroy(BCMBUF *pbcmbuf)
{
    /* allocate memory for the buffer */
    if (!pbcmbuf->head)
        return MV_VPP_EBADCALL;

    pbcmbuf->head = NULL;
    return MV_VPP_OK;
}

/******************************************************************************
 * FUNCTION: reset a register programming buffer
 * PARAMS: *buf - pointer to a register programming buffer
 * RETURN:  1 - succeed
 *          0 - failed to initialize a BCM buffer
 ******************************************************************************/
INT VPP_BCMBUF_Reset(BCMBUF *pbcmbuf)
{
    pbcmbuf->tail = pbcmbuf->head + (pbcmbuf->size/4);
    /*set pointers to the head*/
    pbcmbuf->writer = pbcmbuf->head;
    pbcmbuf->dv1_head = pbcmbuf->head;
    pbcmbuf->dv2_head = pbcmbuf->dv1_head + (pbcmbuf->size/12);
    pbcmbuf->dv3_head = pbcmbuf->dv2_head + (pbcmbuf->size/12);
    pbcmbuf->subID = -1; /* total */
    return MV_VPP_OK;
}

/******************************************************************************
 * FUNCTION: selest BCM sub-buffer to use
 * PARAMS: *buf - pointer to the buffer descriptor
 *         subID - DV_1, DV_2, DV_3
 ******************************************************************************/
void VPP_BCMBUF_Select(BCMBUF *pbcmbuf, INT subID)
{
    /* reset read/write pointer of the buffer */
    if (subID == CPCB_1)
        pbcmbuf->writer = pbcmbuf->dv1_head;
    else
        pbcmbuf->writer = pbcmbuf->head;

    pbcmbuf->subID = subID;
    return;
}

/******************************************************************************
 * FUNCTION: write register address (4bytes) and value
 *          (4bytes) to the buffer
 * PARAMS: *buf - pointer to the buffer descriptor
 *          address - address of the register to be set
 *          value - the value to be written into the
 *                  register
 * RETURN: 1 - succeed
 *         0 - register programming buffer is full
 ******************************************************************************/
INT VPP_BCMBUF_Write(BCMBUF *pbcmbuf, UINT32 address, UINT32 value)
{
    UINT32 *end;
    /*if not enough space for storing another 8 bytes, wrap around happens*/
    if (pbcmbuf->subID == CPCB_1)
        end = pbcmbuf->dv2_head;
    else
        end = pbcmbuf->tail;

    if (pbcmbuf->writer == end) {
        /*the buffer is full, no space for wrap around*/
        return MV_VPP_EBCMBUFFULL;
    }

    *pbcmbuf->writer = value;
    pbcmbuf->writer++;
    *pbcmbuf->writer = address;
    pbcmbuf->writer++;
    return MV_VPP_OK;
}

/******************************************************************************
 * FUNCTION: write a block of data to BCM buffer
 * PARAMS: *buf - pointer to the buffer descriptor
 *          *pdata - pointer to the data
 *          length - the length of the data to be written
 *                  to BCM buffer
 * RETURN: 1 - succeed
 *         0 - register programming buffer is full
 ******************************************************************************/
INT VPP_BCMBUF_WriteBlock(BCMBUF *pbcmbuf, UINT32 *pdata, UINT32 length)
{
    UINT32 *end;

    if (pbcmbuf->subID == CPCB_1)
        end = pbcmbuf->dv2_head;
    else
        end = pbcmbuf->tail;

    if (pbcmbuf->writer > end-(length >> 2)) {
        /*the buffer is full*/
        return MV_VPP_EBCMBUFFULL;
    }

    /*save the data to BCM buffer*/
    GaloisMemcpy(pbcmbuf->writer, pdata, length);
    pbcmbuf->writer += (length >> 2);

    return MV_VPP_OK;
}

/******************************************************************************
 * FUNCTION: do the hardware transmission
 * PARAMS: block - 0:return without waiting for
 *                   transactionfinishing
 *                 1:return after waiting for
 *                   transaction finishing
 ******************************************************************************/
void VPP_BCMBUF_HardwareTrans(BCMBUF *pbcmbuf, INT block)
{
    HDL_semaphore *pSemHandle;
    HDL_dhub2d *pDhubHandle;
    UINT32 *start;
    int status;
    int dhubID, size;
    INT32 shm_offset;
    unsigned int bcm_sched_cmd[2];

    if (pbcmbuf->subID == CPCB_1) {
        start = pbcmbuf->dv1_head;
        shm_offset = pbcmbuf->dv1_head - pbcmbuf->head;
    } else {
        start = pbcmbuf->head;
        shm_offset = 0;
    }

    size = pbcmbuf->writer-start;
    if (size <= 0)
        return;
    size = size<<2;
    /* flush data in D$ */
    FLUSH_DCACHE_RANGE(pbcmbuf->phy_addr+shm_offset, size);
    /* get non-cache physical address for DMA */
    //VPP_SHM_GetPhysicalAddress(pbcmbuf->handle, shm_offset, &start);
    start = pbcmbuf->handle + shm_offset;

    /* start BCM engine */
    dhubID = avioDhubChMap_aio64b_BCM_R;
    pDhubHandle = &AG_dhubHandle;

    if (block) {
        pSemHandle = dhub_semaphore(&(pDhubHandle->dhub));
        /* clear possible BCM previous interrupt */
        status = semaphore_chk_full(pSemHandle, dhubID);
        if (status) {
            semaphore_pop(pSemHandle, dhubID, 1);
            semaphore_clr_full(pSemHandle, dhubID);
        }
    }
    /* clear BCM interrupt */
    pSemHandle = dhub_semaphore(&(pDhubHandle->dhub));
    status = semaphore_chk_full(pSemHandle, dhubID);
    while (status) {
        semaphore_pop(pSemHandle, dhubID, 1);
        semaphore_clr_full(pSemHandle, dhubID);
        status = semaphore_chk_full(pSemHandle, dhubID);
    }

    dhub_channel_generate_cmd(&(pDhubHandle->dhub), dhubID, start,
        size, 0, 0, 0, 1, (int*)bcm_sched_cmd);

    while (!BCM_SCHED_PushCmd(BCM_SCHED_Q12, bcm_sched_cmd, NULL)) {
        /* Wait till HW transaction
           compeltes
        */
    }

    if (block) {
        /* check BCM interrupt */
        pSemHandle = dhub_semaphore(&(pDhubHandle->dhub));
        status = semaphore_chk_full(pSemHandle, dhubID);
        while (!status)
            status = semaphore_chk_full(pSemHandle, dhubID);
        /* clear BCM interrupt */
        semaphore_pop(pSemHandle, dhubID, 1);
        semaphore_clr_full(pSemHandle, dhubID);
    }
}

void VPP_BCMBUF_HardwareTrans_Direct(INT block, UINT32 *start, int size)
{
    HDL_semaphore *pSemHandle;
    HDL_dhub2d *pDhubHandle;
    int status;
    int dhubID;
    unsigned int bcm_sched_cmd[2];

    if (size <= 0)
        return;
    size = size<<2;
    /* flush data in D$ */
    FLUSH_DCACHE_RANGE(start, size);

    /* start BCM engine */
    dhubID = avioDhubChMap_aio64b_BCM_R;
    pDhubHandle = &AG_dhubHandle;

    if (block) {
        pSemHandle = dhub_semaphore(&(pDhubHandle->dhub));
        /* clear possible BCM previous interrupt */
        status = semaphore_chk_full(pSemHandle, dhubID);
        if (status) {
            semaphore_pop(pSemHandle, dhubID, 1);
            semaphore_clr_full(pSemHandle, dhubID);
        }
    }
    /* clear BCM interrupt */
    pSemHandle = dhub_semaphore(&(pDhubHandle->dhub));
    status = semaphore_chk_full(pSemHandle, dhubID);
    while (status) {
        semaphore_pop(pSemHandle, dhubID, 1);
        semaphore_clr_full(pSemHandle, dhubID);
        status = semaphore_chk_full(pSemHandle, dhubID);
    }

    dhub_channel_generate_cmd(&(pDhubHandle->dhub), dhubID, start,
        size, 0, 0, 0, 1, (int*)bcm_sched_cmd);

    while (!BCM_SCHED_PushCmd(BCM_SCHED_Q12, bcm_sched_cmd, NULL)) {
        /* Wait till HW transaction
           compeltes
        */
    }

    if (block) {
        /* check BCM interrupt */
        pSemHandle = dhub_semaphore(&(pDhubHandle->dhub));
        status = semaphore_chk_full(pSemHandle, dhubID);
        while (!status)
            status = semaphore_chk_full(pSemHandle, dhubID);
        /* clear BCM interrupt */
        semaphore_pop(pSemHandle, dhubID, 1);
        semaphore_clr_full(pSemHandle, dhubID);
    }
}

/******************************************************************************
 * FUNCTION: send a BCM BUF info info to a BCM cfgQ
 * PARAMS: *pbcmbuf - pointer to the BCMBUF
 *         *cfgQ - target BCM cfgQ
 * NOTE: this API is only called from VBI/VDE ISR.
 *****************************************************************************/
int VPP_BCMBUF_To_CFGQ(BCMBUF *pbcmbuf, DHUB_CFGQ *cfgQ)
{
    UINT32 *start, *phy_start;
    INT32 size, shm_offset;
    unsigned int bcm_sched_cmd[2];

    if (pbcmbuf->subID == CPCB_1) {
        start = pbcmbuf->dv1_head;
        shm_offset = pbcmbuf->dv1_head - pbcmbuf->head;
        phy_start = pbcmbuf->phy_addr;
    } else {
        start = pbcmbuf->head;
        shm_offset = 0;
        phy_start = pbcmbuf->phy_addr;
    }

    size = pbcmbuf->writer-start;

    if (size <= 0)
        return MV_VPP_EBADPARAM;

    size = size << 2;
    FLUSH_DCACHE_RANGE(pbcmbuf->phy_addr+shm_offset, size);
    dhub_channel_generate_cmd(&(AG_dhubHandle.dhub),
            avioDhubChMap_aio64b_BCM_R,
            phy_start, size, 0, 0, 0, 1, (int*)bcm_sched_cmd);
    while (!BCM_SCHED_PushCmd(BCM_SCHED_Q13, bcm_sched_cmd,
            cfgQ->addr + cfgQ->len*2)) {
            /* Wait till transaction */
    }

    cfgQ->len += 2;

    return MV_VPP_OK;
}

/******************************************************************************
 * FUNCTION: send a raw BCM BUF info to a BCM cfgQ
 * PARAMS: pdata - pointer to the data block
 *         length - data length for transaction
 *         *cfgQ - target BCM cfgQ
 * NOTE: this API is only called from VBI/VDE ISR.
 *****************************************************************************/
void VPP_BCMBUF_Raw_To_CFGQ(UINT32 *pdata, UINT32 length,
        DHUB_CFGQ *cfgQ)
{
    unsigned int bcm_sched_cmd[2];

    dhub_channel_generate_cmd(&(AG_dhubHandle.dhub),
            avioDhubChMap_aio64b_BCM_R, pdata,
            length, 0, 0, 0, 1, (int*)bcm_sched_cmd);
    while (!BCM_SCHED_PushCmd(BCM_SCHED_Q13, bcm_sched_cmd,
            cfgQ->addr + cfgQ->len*2)) {
        /* Wait till transaction */
    }
    cfgQ->len += 2;
}

/******************************************************************************
 * FUNCTION: send a BCM cfgQ info to a BCM cfgQ
 * PARAMS: src_cfgQ - pointer to the source BCM cfgQ
 *         *cfgQ - target BCM cfgQ
 * NOTE: this API is only called from VBI/VDE ISR.
 *****************************************************************************/
void VPP_CFGQ_To_CFGQ(DHUB_CFGQ *src_cfgQ, DHUB_CFGQ *cfgQ)
{
    unsigned int bcm_sched_cmd[2];

    if (src_cfgQ->len <= 0)
        return;

    FLUSH_DCACHE_RANGE(src_cfgQ->phy_addr, src_cfgQ->len*8);
    dhub_channel_generate_cmd(&(AG_dhubHandle.dhub),
                avioDhubChMap_aio64b_BCM_R,
                src_cfgQ->phy_addr, (INT)src_cfgQ->len*8,
                0, 0, 0, 1, (int*)bcm_sched_cmd);
    while (!BCM_SCHED_PushCmd(BCM_SCHED_Q13, bcm_sched_cmd,
                cfgQ->addr + cfgQ->len*2)) {
        /* Wait till transaction */
    }
    cfgQ->len += 2;
}

/******************************************************************************
 * FUNCTION: commit cfgQ which contains BCM DHUB programming info to interrupt
 *           service routine
 * PARAMS: *cfgQ - cfgQ
 *         cpcbID - cpcb ID which this cmdQ belongs to
 *         intrType - interrupt type which this cmdQ belongs to:
 *                    0-VBI, 1-VDE
 * NOTE: this API is only called from VBI/VDE ISR.
 *****************************************************************************/
int VPP_BCMDHUB_CFGQ_Commit(DHUB_CFGQ *cfgQ, int cpcbID, int intrType)
{
    unsigned int sched_qid = BCM_SCHED_Q1;
    unsigned int bcm_sched_cmd[2];

    if (cfgQ->len <= 0)
        return MV_VPP_EBADPARAM;

    if (cpcbID == CPCB_1) {
        if (intrType == 0)
            sched_qid = BCM_SCHED_Q0;
        else
            sched_qid = BCM_SCHED_Q1;
    }

    FLUSH_DCACHE_RANGE(cfgQ->phy_addr, cfgQ->len*8);
    dhub_channel_generate_cmd(&(AG_dhubHandle.dhub),
            avioDhubChMap_aio64b_BCM_R,
            cfgQ->phy_addr, cfgQ->len*8, 0, 0,
            0, 1, (int*)bcm_sched_cmd);
    while (!BCM_SCHED_PushCmd(sched_qid, bcm_sched_cmd, NULL)) {
        /* Wait till BCM DHUB transaction
           compeltes
        */
    }
    return MV_VPP_OK;
}

void VPP_REG_Block_Write(unsigned int *start, unsigned int size, int use_dhub)
{
    if(use_dhub)
    {
        VPP_BCMBUF_HardwareTrans_Direct(1, start, size);
    }
    else
    {
        unsigned int i = 0;

        for(i = 0; i < size/(sizeof(unsigned)); i+=2)
            GA_REG_WORD32_WRITE(start[i+1], start[i]);
    }
}
