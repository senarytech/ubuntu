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

#ifndef _VPP_MODULE_H_
#define _VPP_MODULE_H_

#include "api_avio_dhub.h"
#include "com_type.h"
#include "vpp_com.h"
#include "vpp_api.h"
#include "vpp_bcmbuf.h"
#include "framequeue.h"
#include "vbuf.h"

#define START_1DDMA(dhubID, dmaID, start_addr, size, cfgQ, ptr) \
	dhub_channel_big_write_cmd(&(((HDL_dhub2d *)dhubID)->dhub), \
	dmaID, (INT)start_addr, size, 0,\
	0, 0, 1, cfgQ, ptr)

#define CLEAR_1DDMA(dhubID, dmaID, bcmbuf) \
	dhub_channel_clear_seq(dhubID, dmaID, bcmbuf); \

#define START_2DDMA(dhubID, dmaID, start_addr, stride, width, height, cfgQ) \
	dhub2d_channel_cfg((HDL_dhub2d *)dhubID, dmaID, start_addr, \
	stride, width, height, 1, 0, 0, 0,\
	0, 1, cfgQ)

#define CLEAR_2DDMA(dhubID, dmaID, bcmbuf) \
	do { \
		dhub2d_channel_clear_seq(dhubID, dmaID); \
		dhub2d_channel_start_seq(dhubID, dmaID); \
		dhub2d_channel_clear_seq_bcm(dhubID, dmaID, bcmbuf); \
	} while (0)

#define START_2NDDMA(dhubID, dmaID, start_addr, \
            burst, step1, size1, step2, size2, cfgQ) \
            dhub2nd_channel_cfg((HDL_dhub2d *)dhubID, \
            dmaID, start_addr, burst, step1, size1, \
            step2, size2, 0, 0, 0, 1, cfgQ)

#define CLEAR_2NDDMA(dhubID, dmaID, bcmbuf) \
    do{ \
    }while(0)

/* default window attributes */
#define DEFAULT_BGCOLOR		0
#define DEFAULT_ALPHA		0xff

/* definition of video frame-rate */
enum {
	TYPE_SD = 0,
	TYPE_HD = 1,
	TYPE_UHD = 2,
};

/* definition of video scan mode */
enum {
	SCAN_PROGRESSIVE = 0,
	SCAN_INTERLACED  = 1,
};

enum {
	FRAME_RATE_23P98 = 0,
	FRAME_RATE_24	 = 1,
	FRAME_RATE_25	 = 2,
	FRAME_RATE_29P97 = 3,
	FRAME_RATE_30	 = 4,
	FRAME_RATE_47P96 = 5,
	FRAME_RATE_48	 = 6,
	FRAME_RATE_50	 = 7,
	FRAME_RATE_59P94 = 8,
	FRAME_RATE_60	 = 9,
	FRAME_RATE_100	 = 10,
	FRAME_RATE_119P88 = 11,
	FRAME_RATE_120	 = 12,
};


typedef struct VPP_OBJ_T {
	UINT8 is_uf_triggered;
	INT vbi_num;
	UINT8 dvstatus;
	UINT8 status;
	HDL_semaphore *pSemHandle;
	VBUF_INFO *curr_frame;
	VBUF_INFO *prev_curr_frame;
	VBUF_INFO *curr_still_picture;
	VBUF_INFO *still_picture;
	FRAMEQUEUE inputq;
	FRAMEQUEUE outputq;
	INT frm_count;
	DHUB_CFGQ vbi_dma_cfgQ[2];
	DHUB_CFGQ *curr_cpcb_vbi_dma_cfgQ;
	DHUB_CFGQ vbi_bcm_cfgQ[2];
	DHUB_CFGQ *curr_cpcb_vbi_bcm_cfgQ;
	DHUB_CFGQ vde_bcm_cfgQ[2];
	DHUB_CFGQ *curr_cpcb_vde_bcm_cfgQ;
	DHUB_CFGQ	*curr_vbi_bcm_cfgQ;
	INT skip_vde_int;
	INT dmaRID;
	INT dmaRdhubID;
	BCMBUF  vbi_bcm_buf[2];
	BCMBUF *pVbiBcmBufCpcb[3];
	BCMBUF *pVbiBcmBuf;
	BCMBUF  vbi_clear_bcm_buf[2];
	BCMBUF *pVbiClearBcmBufCpcb[3];
	BCMBUF *pVbiClearBcmBuf;
} VPP_OBJ;

VOID VPP_ISR_Unregistration(INT32 handle);
VOID VPP_RegisterInterruptService(voidfunc_t f1, voidfunc_t f2, voidfunc_t f3);
VOID VPP_Enable_ISR_Interrupt(VPP_OBJ *vpp_obj, INT32 cpcbID);
VOID MV_VPP_EnableISR(VPP_OBJ *pVpp_obj);
VOID MV_VPP_DisableISR(VPP_OBJ *pVpp_obj);
#endif

