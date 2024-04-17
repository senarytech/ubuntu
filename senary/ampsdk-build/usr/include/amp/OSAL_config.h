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
#ifndef _OSAL_CONFIG_H_
#define _OSAL_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif

////////////////////////////////////////////////////////////////////////////////
//! \brief Definition: Semphore definition
//!
////////////////////////////////////////////////////////////////////////////////
//Initial count for the semaphore object. It must be greater than or equal to zero and less than or equal to MV_OSAL_SEM_VALUE_MAX.For 32-bit compilers, INT_MAX equals 0x7FFFFFFF (2,147,483,647)
#define MV_OSAL_SEM_VALUE_MAX				((UINT)INT_MAX)

//////////////////////////////////////////////////////////////////////////////////
////! \brief Hardware definition
////!
//////////////////////////////////////////////////////////////////////////////////
#define MV_CC_HAL_MEMBOUNDARY				(64)  // Hardware Memroy Boundary Size (DMA operation)

////////////////////////////////////////////////////////////////////////////////
//! \brief ICCFIFO and ICCFIFOArray definition
//!
////////////////////////////////////////////////////////////////////////////////
#define MV_CC_ICCFIFO_FRAME_SIZE			(128)				// Fixed Size(Byte) for the every Frame in the ICCFIFO
#define MV_CC_ICCFIFO_NMAX					(63)				// Max number Frame in the ICCFIFO buffer except FIFO head
#define MV_CC_ICCFIFO_FullSize(x,y)			(MV_CC_HAL_MEMBOUNDARY + (x) * (y) )
#define MV_CC_ICCFIFO_FULL_SIZE				(MV_CC_ICCFIFO_FullSize(MV_CC_ICCFIFO_NMAX, MV_CC_ICCFIFO_FRAME_SIZE))  	// Byte Size of the ICCFIFO include the FIFO head
#define MV_CC_ICCFIFO_ARRAY_SIZE			(MV_CC_ICCFIFO_FULL_SIZE * MV_OSAL_CPU_NUM * MV_OSAL_CPU_NUM)				// ICCFIFO Array Memory Size
#define MV_CC_ICCFIFO_FrameNMin				(2)


////////////////////////////////////////////////////////////////////////////////
//! \brief CC - ICCP Message definition
//!
////////////////////////////////////////////////////////////////////////////////
#define MV_CC_ICCP_HEAD_BYTEOFFSET			(0)
#define MV_CC_ICCP_HEAD_BYTESIZE			(12)
#define MV_CC_ICCP_DATA_BYTEOFFSET			(MV_CC_ICCP_HEAD_BYTEOFFSET + MV_CC_ICCP_HEAD_BYTESIZE)

////////////////////////////////////////////////////////////////////////////////
//! \brief CC - RPC Message definition
//!
////////////////////////////////////////////////////////////////////////////////
#define MV_CC_RPC_HEAD_BYTEOFFSET			(0)
#define MV_CC_RPC_HEAD_BYTESIZE				(8)
#define MV_CC_RPC_DATA_BYTEOFFSET			(MV_CC_RPC_HEAD_BYTEOFFSET + MV_CC_RPC_HEAD_BYTESIZE)
#define MV_CC_RPC_DATA_BYTESIZE				(MV_CC_ICCFIFO_FRAME_SIZE - MV_CC_RPC_DATA_BYTEOFFSET)
#define MV_CC_RPC_SHMPARAMETER_BYTESIZE		(8)
#define MV_CC_RPC_PARAM_BYTESIZE			(252)

//------------------------------------------------------------------------------
//! \brief TaskPool of PRC
#define MV_CC_RPC_TaskPool_TaskNMAX			(16)

////////////////////////////////////////////////////////////////////////////////
//! \brief CC - Circular Buffer definition
//!
////////////////////////////////////////////////////////////////////////////////
#define MV_CC_CBUF_FLAGS_DEFAULT_SETTING	(MV_CC_CBUF_FLAGS_ALIGNMENT)
#define MV_CC_CBuf_NoticeMsgQ_NMax			(20)


////////////////////////////////////////////////////////////////////////////////
//! \brief CC - Message Queue definition
//!
////////////////////////////////////////////////////////////////////////////////
#define MV_CC_ICC_NMsgsMin					(MV_CC_ICCFIFO_FrameNMin)
#define MV_CC_ITC_NMsgsMin					(MV_CC_ICCFIFO_FrameNMin)

#define MV_CC_MSGQ_HEAD_BYTEOFFSET			(0)
#define MV_CC_MSGQ_HEAD_BYTESIZE			(2)
#define MV_CC_MSGQ_DATA_BYTEOFFSET			(MV_CC_MSGQ_HEAD_BYTEOFFSET + MV_CC_MSGQ_HEAD_BYTESIZE)
#define MV_CC_MSGQ_DATA_BYTESIZE			(MV_CC_ICCFIFO_FRAME_SIZE - MV_CC_MSGQ_DATA_BYTEOFFSET)
#define MV_CC_MSGQ_DATA_BYTESIZE_MAX		(MV_CC_ICCFIFO_FRAME_SIZE - MV_CC_MSGQ_DATA_BYTEOFFSET)
#define MV_CC_MSGQ_DATA_MSGEXLEN_MAX		(MV_CC_MSGQ_DATA_BYTESIZE_MAX - 4)

#define MsgQ_Msg_Flags(x)					(((UINT8 *)(x))[(MV_CC_MSGQ_HEAD_BYTEOFFSET) + 0])
#define MsgQ_Msg_DataLen(x)					(((UINT8 *)(x))[(MV_CC_MSGQ_HEAD_BYTEOFFSET) + 1])

////////////////////////////////////////////////////////////////////////////////
//! \brief OSAL - Task definition
//!
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//! \brief Task - priority
#ifndef __ANDROID__
        #define MV_CC_MIDSRV_TASK_PRIORITY			(MV_OSAL_TASK_PRIORITY_15)
#else
        #define MV_CC_MIDSRV_TASK_PRIORITY			(MV_OSAL_TASK_PRIORITY_DEFAULT)
#endif

#if defined(__ANDROID__)
	#define MV_CC_RPC_SERVER_PRIORITY			(MV_OSAL_TASK_PRIORITY_DEFAULT)
#else
	#define MV_CC_RPC_SERVER_PRIORITY			(MV_OSAL_TASK_PRIORITY_30)
#endif

#define MV_TIMER_TASK_PRIORITY_LINUX		(MV_OSAL_TASK_PRIORITY_29)			// It is higher than the default task priority
#define MV_TIMER_TASK_PRIORITY_ANDROID		(MV_OSAL_TASK_PRIORITY_DEFAULT)			// It is a common task in Android

//------------------------------------------------------------------------------
//! \brief Task - message queue
#define MV_OSAL_Task_NMsgsMin				(1)

#ifndef AMP_RELEASE_BUILD
// FLIXCK_MAX_FD depends on SCM_MAX_FD
#define FLICK_MAX_FD 253
#else
#define FLICK_MAX_FD 10
#endif

#ifdef __cplusplus
}
#endif

#endif
