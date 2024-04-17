/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (C) 2013-2017 Synaptics Incorporated. All rights reserved.
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
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS", AND
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

/*	cmd_buf.h

	This file defines two kinds of command buffers and associated cmd types
	1. Stream control info buffer: control info coming from the stream.
	2. Input control tags: tags given by application.
*/	   

#ifndef __CMD_BUF_H__
#define __CMD_BUF_H__
#include "com_type.h"

#if defined (ARM) && defined (__ARMCC_VERSION)
#pragma anon_unions 
#endif

///////////////////// AV ACTION COMMAND BUFFER /////////////////////////////
typedef struct t_audio_cmd_data {
			INT m_Parameter_1;
			INT m_Parameter_2; 
} AUDIO_CMD_DATA;

typedef	union t_audio_cmd_data_union {
//		AUDIO_INFO m_audio_info;
		AUDIO_CMD_DATA m_cmd_data;
} AUDIO_CMD_DATA_UNION;

typedef struct t_audio_cmd_cell {
	INT m_SeqId; // sequence ID coming from the upper level
	INT m_UserParam;
	INT m_CmdType;
	INT m_StreamId; // Input stream
	INT m_PathId; // output path
	INT m_Asap;	// 1: ASAP (will ignore m_Effect_PTS), 0: not ASAP
	UINT m_EffectPts; // EffectPts and EffectCntr will composed the precisely control.
	UINT m_EffectCntr;
	AUDIO_CMD_DATA_UNION m_Union;
	INT m_CmdStatus;
	VOID (*m_CallBackFunction)(VOID *pInfo);
} AUDIO_CMD_CELL;

typedef struct t_vidio_cmd_data {
			INT m_Parameter_1;
			INT m_Parameter_2;
			INT m_Parameter_3;
} VIDEO_CMD_DATA;

typedef	union t_video_cmd_data_union {
		VIDEO_CMD_DATA m_cmd_data;
} VIDEO_CMD_DATA_UNION;

typedef struct t_video_cmd_cell {
	INT m_CmdType;
	VIDEO_CMD_DATA_UNION m_Union;
} VIDEO_CMD_CELL;

typedef struct t_av_cmd_ring_buf {
	union {
		AUDIO_CMD_CELL *m_AudioCmdStart;
		VIDEO_CMD_CELL *m_VideoCmdStart;
	};
	INT m_CmdRdIndex;
	INT m_CmdWrIndex;
	INT m_CmdSize;
} AV_CMD_RING_BUF;

typedef struct t_av_cmd_que_list {

	union {
		AUDIO_CMD_CELL	m_AudioCmdQueCell;
		VIDEO_CMD_CELL	m_VideoCmdQueCell;
	};
	struct t_av_cmd_que_list *m_Next;
	struct t_av_cmd_que_list *m_Prev;
} AV_CMD_QUE_LIST;

typedef struct t_AV_cmd_que_buffer {
	AV_CMD_QUE_LIST	*m_FreeList;
	AV_CMD_QUE_LIST	*m_BusyList;
	AV_CMD_QUE_LIST	*m_CmdListHead; // Assuming that we allocate the sequencial buffer at once.
	UINT			m_QueSize;
	UINT			m_cmdque_format;
} AV_CMD_QUE_BUFFER;

//////////////// Command Que Format ////////////////////////////
enum {
	eCmdQueAudio	= 1,
	eCmdQueVideo,
};

/////////////////////////// AV Command Queue Buffer ////////////////////////////////////
/**************************************************************************************
 * Function:    AVCmdQueCreate
 *
 * Description: Create CmdQue Buffer.
 *
 * Inputs:      cmd_que_size --- item size of the command Queue
 *              cmd_que_format-- specify the format of the command queue. for audio/video
 * Outputs:     none
 *
 * Return:      return the handler of cmmand queue buffer
 **************************************************************************************/
VOID *AVCmdQueCreate(INT cmd_que_size, INT cmd_que_format);

/**************************************************************************************
 * Function:    AVCmdQueFree
 *
 * Description: Free CmdQue Buffer.
 *
 * Inputs:      pHandler --- handler of cmmand queue buffer.
 *
 * Outputs:     pHandler --- handler will be NULL.
 *
 * Return:      0 means success, none 0 means command queue is empty.
 **************************************************************************************/
INT AVCmdQueFree(AV_CMD_QUE_BUFFER **pHandler);

/**************************************************************************************
 * Function:    GetCmdQueFreeCell
 *
 * Description: Get a free CmdQueCell from Free List.
 *
 * Inputs:      pCmdList --- Command queue buffer handler.
 *
 * Outputs:     none
 *
 * Return:      pointer of the free command queue cell.
 **************************************************************************************/
VOID *GetAVCmdQueFreeCell(AV_CMD_QUE_BUFFER *pCmdList);

/**************************************************************************************
 * Function:    AddCmdQueToBusyList
 *
 * Description: Add the cmdQue into Busy List. For audio, the command will be reorganized
 *              In order according to the PTS associated with the command.
 *
 * Inputs:      pCmdList --- Command queue buffer handler.
 *              pList-- command queue cell handler
 * Outputs:     none
 *
 * Return:      none
 **************************************************************************************/
VOID AddCmdQueToBusyList(AV_CMD_QUE_BUFFER *pCmdList, AV_CMD_QUE_LIST *pList);

/**************************************************************************************
 * Function:    PutCmdQueCellIntoFreeList
 *
 * Description: Put the command Que cell into free list
 *
 * Inputs:      pCmdList --- Command queue buffer handler.
 *              pList-- command queue cell handler
 * Outputs:     none
 *
 * Return:      none
 **************************************************************************************/
VOID PutCmdQueCellIntoFreeList(AV_CMD_QUE_BUFFER *pCmdList, AV_CMD_QUE_LIST *pList);

/**************************************************************************************
 * Function:    RemoveCmdQueFromBusyList
 *
 * Description: Search for the pList in CMD_QUE_BUFFER, remove the pList from the 
 *              CMD_QUE_BUFF busy_list.
 *
 * Inputs:      pCmdList --- Command queue buffer handler.
 *              pList-- command queue cell handler
 * Outputs:     none
 *
 * Return:      none
 **************************************************************************************/
VOID RemoveCmdQueFromBusyList(AV_CMD_QUE_BUFFER *pCmdList, AV_CMD_QUE_LIST **pList);

#endif //__CMD_BUF_H__
