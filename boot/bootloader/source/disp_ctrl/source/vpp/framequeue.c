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

#define _FRAMEQUEUE_C_

#include "printf.h"
#include "framequeue.h"

#define WRAPAROUND(a) (((a) >= MAX_NUM_FRAMES) ? 0:(a))

/******************************************************************************
 * FUNCTION: reset frame queue to be empty state
 * PARAMS: *frmq - pionter to a frame queue
 *
 ******************************************************************************/
void frmq_reset(FRAMEQUEUE *frmq)
{
    /* reset read/write pointer of frame queue */
    /* the frame queue is empty when read pointer equals write pointer */
    frmq->shadow_head = 0;
    frmq->head = 0;
    frmq->tail = 0;

    /* reset frame pointers to NULL in frame queue */
    GaloisMemSet(frmq->frame_descrs, 0, sizeof(void *)*MAX_NUM_FRAMES);

    return;
}


/******************************************************************************
 * FUNCTION: push a frame descriptor into frame queue
 * PARAMS: *frmq - pointer to frame queue
 *         *frm_descr - pointer to a frame descriptor
 * RETURN: 1 - succeed
 *         0 - fail: frame queue is full
 ******************************************************************************/
int frmq_push(FRAMEQUEUE *frmq, void *frm_descr)
{
    /* first check whether frame queue is full */
    if (WRAPAROUND(frmq->tail+1) == frmq->head)
        return 0;

    /* push frame descriptor into frame queue if not full */
    frmq->frame_descrs[frmq->tail] = frm_descr;

    /* update shadow write pointer */
    frmq->tail = WRAPAROUND(frmq->tail+1);

    return 1;
}

/******************************************************************************
 * FUNCTION: pop a frame descriptor out of a frame queue
 *           but actually without update head pointer.
 * PARAMS: *frmq - pointer to a frame queue
 *         **frm_descr - pointer to the frame descriptor
 * RETURN: 1 - succeed
 *         0 - command queue is empty, no command is available
 * NOTE: use pop_commit to actually update head pointer.
 ******************************************************************************/
int frmq_pop(FRAMEQUEUE *frmq, void **frm_descr)
{
    /* first check whether frame queue is empty */
    if (frmq->head == frmq->tail)
        return 0;

    /* pop a frame descriptor from frame queue if not empty */
    *frm_descr = frmq->frame_descrs[frmq->head];

    /* update shadow read pointer */
    frmq->shadow_head = WRAPAROUND(frmq->head+1);

    return 1;
}

/******************************************************************************
 * FUNCTION: commit previous pop operation
 *           by actually update head pointer.
 * PARAMS: *frmq - pointer to a frame queue
 * RETURN: 1 - succeed
 *         0 - command queue is empty, no command is available
 ******************************************************************************/
int frmq_pop_commit(FRAMEQUEUE *frmq)
{
    /* update shadow read pointer */
    frmq->head = frmq->shadow_head;

    return 1;
}


