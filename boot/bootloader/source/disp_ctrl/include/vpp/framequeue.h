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

#ifndef _FRAMEQUEUE_H_
#define _FRAMEQUEUE_H_

#include "vpp_com.h"
#ifdef __cplusplus
extern "C" {
#endif

/* structure for frame descriptor queue */
typedef struct FRAMEQUEUE_T {
	void *frame_descrs[MAX_NUM_FRAMES];
	int shadow_head;
	int head;
	int tail;
} FRAMEQUEUE;

/* frame queue APIs */
/******************************************************************************
 * FUNCTION: reset frame queue to be empty state
 * PARAMS: *frmq - pionter to a frame queue
 *
 ******************************************************************************/
void frmq_reset(FRAMEQUEUE *frmq);

/******************************************************************************
 * FUNCTION: push a frame descriptor into frame queue
 * PARAMS: *frmq - pointer to frame queue
 *		   *frm_descr - pointer to a frame descriptor
 * RETURN: 1 - succeed
 *		   0 - fail: frame queue is full
 ******************************************************************************/
int frmq_push(FRAMEQUEUE *frmq, void *frm_descr);

/******************************************************************************
 * FUNCTION: pop a frame descriptor out of a frame queue
 *			 but actually without update head pointer.
 * PARAMS: *frmq - pointer to a frame queue
 *		   **frm_descr - pointer to the frame descriptor
 * RETURN: 1 - succeed
 *		   0 - command queue is empty, no command is available
 * NOTE: use pop_commit to actually update head pointer.
 ******************************************************************************/
int frmq_pop(FRAMEQUEUE *frmq, void **frm_descr);

/******************************************************************************
 * FUNCTION: commit previous pop operation
 *			 by actually update head pointer.
 * PARAMS: *frmq - pointer to a frame queue
 * RETURN: 1 - succeed
 *		   0 - command queue is empty, no command is available
 ******************************************************************************/
int frmq_pop_commit(FRAMEQUEUE *frmq);

#ifdef __cplusplus
}
#endif

#endif

