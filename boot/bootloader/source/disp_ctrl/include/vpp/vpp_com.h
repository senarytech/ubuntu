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

#ifndef _VPP_COMMON_H_
#define _VPP_COMMON_H_

/* maximum number of VPP objects */
#ifdef MAX_NUM_OBJECTS
#error "duplicated MACRO definition of MAX_NUM_OBJECTS."
#else
#define MAX_NUM_OBJECTS  1
#endif

/* maximum number of integer parameters of callback functions */
#ifdef MAX_NUM_PARAMS
#error "duplicated MACRO definition of MAX_NUM_PARAMS."
#else
#define MAX_NUM_PARAMS	  10
#endif

/* maximum number of frames in frame buffer queue */
#ifdef MAX_NUM_FRAMES
#error "duplicated MACRO definition of MAX_NUM_FRAMES"
#else
#define MAX_NUM_FRAMES	  20
#endif

/* maximum number of commands in command queue */
#ifdef MAX_NUM_CMDS
#error "duplicated MACRO definition of MAX_NUM_CMDS"
#else
#define MAX_NUM_CMDS	  500
#endif

/* maximum number of events */
#ifdef MAX_NUM_EVENTS
#error "duplicated MACRO definition of MAX_NUM_EVENTS"
#else
#define MAX_NUM_EVENTS	  32
#endif

/* BCM buffer size */
#ifdef BCM_BUFFER_SIZE
#error "duplicated MACRO definition of BCM_BUFFER_SIZE"
#else
#define BCM_BUFFER_SIZE   0x17700
#define DV1_BCM_BUFFER_SIZE   0x7d00
#define DV2_BCM_BUFFER_SIZE   0x7d00
#define DV3_BCM_BUFFER_SIZE   0x7d00
#define VDE_BCM_BUFFER_SIZE   0x800
#endif

/* DMA command buffer size */
#ifdef DMA_CMD_BUFFER_SIZE
#error "duplicated MACRO definition of DMA_CMD_BUFFER_SIZE"
#else
#define DMA_CMD_BUFFER_SIZE   (100 * 8)
#endif

#endif
