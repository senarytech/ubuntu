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

#ifndef _VBUF_H_
#define _VBUF_H_
#define VDES_INFO_GRP_ALLOC			0x1
#define VDES_DESC_GRP_ALLOC			0x2
typedef struct vbuf_info_t {
	UINT32	m_grp_alloc_flag;
	UINT32	m_buf_size;
	UINT32	m_flags;
	UINT32	m_srcfmt;
	UINT32	m_order;
	UINT32	m_bytes_per_pixel;
	UINT32	m_content_offset;
	UINT32	m_content_width;
	UINT32	m_content_height;
	INT32	m_active_left;
	INT32	m_active_top;
	UINT32	m_active_width;
	UINT32	m_active_height;
	INT32	m_disp_offset;
	UINT32	m_buf_stride;
	VOID   *m_bufferID;
	UINT32  *m_pbuf_start;
	VOID   *hShm;
}  VBUF_INFO;
#endif
