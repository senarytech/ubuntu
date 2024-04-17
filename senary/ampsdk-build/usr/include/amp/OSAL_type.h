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

#ifndef _OSAL_TYPE_H_
#define _OSAL_TYPE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef INLINE
#define INLINE inline
#endif

#include "com_type.h"
#include "ErrorCode.h"


#ifdef __cplusplus
extern "C"
{
#endif	// #ifdef __cplusplus


////////////////////////////////////////////////////////////////////////////////
//! \brief OSAL Data Structure: MV_OSAL_HANDLE | Handle to common OSAL object
//!
////////////////////////////////////////////////////////////////////////////////
typedef void *MV_OSAL_HANDLE;

////////////////////////////////////////////////////////////////////////////////
//! \brief OSAL Data Structure: MV_CC_HANDLE | Handle to common CC object
//!
////////////////////////////////////////////////////////////////////////////////
typedef void *MV_CC_HANDLE;


////////////////////////////////////////////////////////////////////////////////
//! \brief OSAL CPU constant definition: MV_OSAL_CPUID_U8_t
//!
////////////////////////////////////////////////////////////////////////////////
typedef UINT8 	MV_OSAL_CPUID_U8_t;

////////////////////////////////////////////////////////////////////////////////
//! \brief OSAL Process constant definition: MV_OSAL_ProcessID_U8_t
//!
////////////////////////////////////////////////////////////////////////////////
typedef UINT8 	MV_OSAL_ProcessID_U8_t;

////////////////////////////////////////////////////////////////////////////////
//! \brief definition
//!
////////////////////////////////////////////////////////////////////////////////
#define MV_OSAL_TRUE				(1)
#define MV_OSAL_FALSE				(0)

////////////////////////////////////////////////////////////////////////////////
//! \brief OSAL Macro definition
//!
////////////////////////////////////////////////////////////////////////////////
#define MV_OSAL_Malloc(x)		CommonMemPoolMalloc(x)
#define MV_OSAL_Free(p)			CommonMemPoolFree(p)

#define UINT32_ADDR(a, b)		((UINT32 *)(a) + ((b) >> 2) )
#define UINT16_ADDR(a, b)		((UINT16 *)(a) + ((b) >> 1) )
#define UINT8_ADDR(a, b)		((UINT8 *)(a) + (b) )

#define INT32_ADDR(a, b)		((INT32 *)(a) + ((b) >> 2) )
#define INT16_ADDR(a, b)		((INT16 *)(a) + ((b) >> 1) )
#define INT8_ADDR(a, b)			((INT8 *)(a) + (b) )




#ifdef ENABLE_DEBUG_OSAL
	#define	MV_OSAL_ErrorHandle(s,x)		{		\
				ERRPRINTF(("[ERROR] - [%s] Return MV_OSAL_Err_Code [0x%08x]\n",(s),(x)));	\
				MV_ASSERT(0); \
			}

	#define	MV_OSAL_ErrorPrint(s,x)			{		\
				ERRPRINTF(("[Warning] - [%s] Return MV_OSAL_Err_Code [0x%08x]\n",(s),(x)));	\
			}

	#define	MV_OSAL_TASK_ErrorExit(s,x)		{									\
				ERRPRINTF(("[ERROR] - [%s] Exit MV_OSAL_Err_Code [0x%08x]\n",(s),(x)));	\
				MV_OSAL_Task_Exit(NULL);													\
			}
#else
	#define	MV_OSAL_ErrorHandle(s,x)		return x
	#define	MV_OSAL_ErrorPrint(s,x)
	#define	MV_OSAL_TASK_ErrorExit(s,x)		exit(x)
#endif

#ifdef ENABLE_DEBUG_OSAL

#define	MV_OSAL_DBG_Warning(x, s, ...)		dbg_Out(DBG_OSAL DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
#define	MV_OSAL_DBG_Info(...)				dbg_Out(DBG_OSAL DBG_LV5 " " __VA_ARGS__)
#define	MV_OSAL_DBG_Error(x, s, ...)		{ dbg_Out(DBG_OSAL DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); MV_ASSERT(0); }
#define	MV_OSAL_DBG_Error_Ptr(x, s, ...)	{ dbg_Out(DBG_OSAL DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); MV_ASSERT(0); }
#define	MV_OSAL_DBG_Error_Task(x, s, ...)	{ dbg_Out(DBG_OSAL DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); MV_ASSERT(0); }

#define	MV_CC_DBG_Error_UINT8(x, s, ...)	{ dbg_Out(DBG_CC DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); MV_ASSERT(0); }
#define	MV_CC_DBG_Error_Ptr(x, s, ...)		{ dbg_Out(DBG_CC DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); MV_ASSERT(0); }
#define	MV_CC_DBG_Error_Task(x, s, ...)		{ dbg_Out(DBG_CC DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); MV_ASSERT(0); }

#define	MV_CC_DBG_Warning(x, s, ...)	dbg_Out(DBG_CC DBG_LV3 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__)
#define	MV_CC_DBG_Info(...)				dbg_Out(DBG_CC DBG_LV5 " " __VA_ARGS__)
#define	MV_CC_DBG_Error(x, s, ...)		{ dbg_Out(DBG_CC DBG_LV1 "(ECode=%08x) | " s "\n", x,  __VA_ARGS__); MV_ASSERT(0); }


#else

#define	MV_OSAL_DBG_Warning(x, s, ...)
#define	MV_OSAL_DBG_Info(...)
#define	MV_OSAL_DBG_Error(x, s, ...)		{return x;}
#define	MV_OSAL_DBG_Error_Ptr(x, s, ...)	{return NULL;}
#define	MV_OSAL_DBG_Error_Task(x, s, ...)	{MV_OSAL_Task_Exit(NULL);}

#define	MV_CC_DBG_Warning(x, s, ...)
#define	MV_CC_DBG_Info(...)
#define	MV_CC_DBG_Error(x, s, ...)			{return x;}
#define	MV_CC_DBG_Error_UINT8(x, s, ...)	{return 0;}
#define	MV_CC_DBG_Error_Ptr(x, s, ...)		{return NULL;}
#define	MV_CC_DBG_Error_Task(x, s, ...)		{MV_OSAL_Task_Exit(NULL);}

#endif

////////////////////////////////////////////////////////////////////////////////
//! \brief CC-Service Macro definition
//!
////////////////////////////////////////////////////////////////////////////////
typedef enum _MV_CC_SrvLevel_U8 {
    MV_CC_SrvLevel_ITC              = 0,    /* Inter-Thread Communication */
    MV_CC_SrvLevel_IPC              = 1,    /* Inter-Process Communication(obsolete) */
    MV_CC_SrvLevel_ICC              = 2,    /* Inter-Process Communication */
    MV_CC_NoCCSrvLevel              = 0xFF  /*  No CC Service */
} MV_CC_SrvLevel_U8_t;

typedef enum _MV_CC_SrvType_U8 {
    MV_CC_SrvType_MsgQ              = 0,    /* Message Queue Service */
    MV_CC_SrvType_RPC               = 1,    /* RPC Service */
    MV_CC_SrvType_CBuf              = 2,    /* Circular Buffer Service */
    MV_CC_SrvType_MsgQEx            = 3,    /* Message Queue Extension Service */
    MV_CC_SrvType_RPCClnt           = 4,    /* RPC Client */
    MV_CC_SrvType_Max               = 5,
    MV_CC_NoCCSrvType               = 0xFF  /*  No CC Service */
} MV_CC_SrvType_U8_t;

////////////////////////////////////////////////////////////////////////////////
//! \brief CC Service definition: MV_OSAL_ServiceID_U32_t
//!
////////////////////////////////////////////////////////////////////////////////
typedef UINT32 	MV_OSAL_ServiceID_U32_t;
typedef UINT32 	MV_CC_ServiceID_U32_t;

////////////////////////////////////////////////////////////////////////////////
//! \brief CC Flag Operation Macro definition
//!
////////////////////////////////////////////////////////////////////////////////
#define MV_CC_FlagSet(x,a,n,m)				((a)?((x)|=(n)):((x)&=(m)))
#define MV_CC_FlagGet(x,n)					(((x)&(n))?(1):(0))
#define MV_CC_FlagCopyXtoY(x,y,n,m)			(((x)&(n))?((y)|=(n)):((y)&=(m)))

#define MV_CC_BitSet(x,n)					((x)|=(n))
#define MV_CC_BitClean(x,n)					((x)&=(~n))
#define MV_CC_BitGet(x,n)					(((x)&(n))?(1):(0))


////////////////////////////////////////////////////////////////////////////////
//! \brief CC Data Structure: MV_CC_CBufCtrlType_t | CBuf Control Object type
//!
////////////////////////////////////////////////////////////////////////////////
typedef enum _MV_CC_CBufCtrlType {
//	MV_CC_CBufCtrlType_None		= 0,	/* Cbuf NULL Object */
	MV_CC_CBufCtrlType_Consumer	= 1,	/* Cbuf Consumer(Server) Object */
	MV_CC_CBufCtrlType_Producer	= 2,	/* Cbuf Producer(client) Object */
} MV_CC_CBufCtrlType_t;



////////////////////////////////////////////////////////////////////////////////
//! \brief CC Data Structure: MV_CC_CBufCtrlType_t | CBuf Control Object type
//!
////////////////////////////////////////////////////////////////////////////////
typedef enum _MV_CC_MsgQType {
	MV_CC_MsgQType_None		= 0,	/* Message Queue (None) */
	MV_CC_MsgQType_ICC		= 1,	/* Message Queue (Inter-CPU) */
	MV_CC_MsgQType_ITC		= 2,	/* Message Queue (Inter-Task) */
	MV_CC_MsgQType_ExICC 	= 3,	/* Message Queue (Inter-CPU) Extension */
	MV_CC_MsgQType_ExITC 	= 4,	/* Message Queue (Inter-Task)Extension */
} MV_CC_MsgQType_t;


////////////////////////////////////////////////////////////////////////////////
//! \brief RPC Message Data Structure: MV_CC_RPC_CmdID_U16_t, MV_CC_RPC_RtnSt_U16_t
//!
////////////////////////////////////////////////////////////////////////////////
typedef UINT16	MV_CC_RPC_CmdID_U16_t;
typedef UINT16	MV_CC_RPC_RtnSt_U16_t;

#define MV_CC_RPC_SUCCESS					(MV_CC_RPC_RtnSt_U16_t)(0x0000)  /* RPC success */
#define MV_CC_RPC_FAILURE					(MV_CC_RPC_RtnSt_U16_t)(0x0001)  /* RPC failure (general)*/
#define MV_CC_RPC_ERRRTN_CMD				(MV_CC_RPC_RtnSt_U16_t)(0x0002)  /* RPC error  - command id*/
#define MV_CC_RPC_ERRRTN_PARAM				(MV_CC_RPC_RtnSt_U16_t)(0x0003)  /* RPC error  - parameter */
#define MV_CC_RPC_ERRRTN_NOSRVFUNC			(MV_CC_RPC_RtnSt_U16_t)(0x0004)  /* RPC error  - no rpc server function on dst address */
#define MV_CC_RPC_ERRRTN_SRVBUSY			(MV_CC_RPC_RtnSt_U16_t)(0x0005)  /* RPC error  - rpc server busy */



////////////////////////////////////////////////////////////////////////////////
//! \brief Data Structure: MV_CC_RPC_RegCmdItem_t, *pMV_CC_RPC_RegCmdItem_t
//!
//! 	The m_pList should be a list sorted by m_Cmd key. And a[n] < a[n+1]
//!		So it is an ascending order list.
////////////////////////////////////////////////////////////////////////////////
typedef struct _MV_CC_RPC_RegCmdItem {

	MV_CC_RPC_CmdID_U16_t m_Cmd;

	/* Warning - If the OSAL Status code CmdFunc_RPCSrv return is not S_OK,
	RPC Server will auto-return the RtnSt error code (MV_CC_RPC_FAILURE) only. */
	INT32 (*pCmdFunc_RPCSrv)(MV_CC_RPC_RtnSt_U16_t 	*pRPC_Rtn,
							UCHAR 					*pParamBuf,
							UINT8 					*pParamSize,
							PVOID		 			*pShmSlot);

} MV_CC_RPC_RegCmdItem_t, *pMV_CC_RPC_RegCmdItem_t;

////////////////////////////////////////////////////////////////////////////////
//! \brief OSAL Macro: MV_OSAL_PRIORITY_t |  The different priority levels for OSAL tasks. Low numbers indiciate higher priority.
//!
////////////////////////////////////////////////////////////////////////////////
typedef enum _MV_OSAL_PRIORITY
{
	MV_OSAL_TASK_PRIORITY_DEFAULT = 0,		/* The default task schedpolicy, no task priority when schedpolicy = SCHED_OTHER in Linux */
    MV_OSAL_TASK_PRIORITY_30 = 1,        	/* The lowest task priority, schedpolicy = SCHED_RR in Linux ( 0 is unavailable ) */
    MV_OSAL_TASK_PRIORITY_29,
    MV_OSAL_TASK_PRIORITY_28,
    MV_OSAL_TASK_PRIORITY_27,
    MV_OSAL_TASK_PRIORITY_26,
    MV_OSAL_TASK_PRIORITY_25,
    MV_OSAL_TASK_PRIORITY_24,
    MV_OSAL_TASK_PRIORITY_23,
    MV_OSAL_TASK_PRIORITY_22,
    MV_OSAL_TASK_PRIORITY_21,
    MV_OSAL_TASK_PRIORITY_20,
    MV_OSAL_TASK_PRIORITY_19,
    MV_OSAL_TASK_PRIORITY_18,
    MV_OSAL_TASK_PRIORITY_17,
    MV_OSAL_TASK_PRIORITY_16,
    MV_OSAL_TASK_PRIORITY_15,
    MV_OSAL_TASK_PRIORITY_14,
    MV_OSAL_TASK_PRIORITY_13,
    MV_OSAL_TASK_PRIORITY_12,
    MV_OSAL_TASK_PRIORITY_11,
    MV_OSAL_TASK_PRIORITY_10,
    MV_OSAL_TASK_PRIORITY_9,
    MV_OSAL_TASK_PRIORITY_8,
    MV_OSAL_TASK_PRIORITY_7,
    MV_OSAL_TASK_PRIORITY_6,
    MV_OSAL_TASK_PRIORITY_5,
    MV_OSAL_TASK_PRIORITY_4,
    MV_OSAL_TASK_PRIORITY_3,
    MV_OSAL_TASK_PRIORITY_2,
    MV_OSAL_TASK_PRIORITY_1 = 30            /* The highest task priority, schedpolicy = SCHED_RR in Linux ( 99 is the best value in Linux ) */
} MV_OSAL_PRIORITY_t;

#define MV_OSAL_TASK_PRIORITY_HIGHEST			99
#define MV_OSAL_TASK_PRIORITY_LOWEST			MV_OSAL_TASK_PRIORITY_30
#define MV_OSAL_TASK_PRIORITY_MIN				MV_OSAL_TASK_PRIORITY_DEFAULT
#define MV_OSAL_TASK_PRIORITY_MAX				99

////////////////////////////////////////////////////////////////////////////////
//! \brief CBuf Flags Definition (m_Flags in MV_CC_CBufBody_t, MV_CC_CBuf_Producer_t, MV_CC_CBuf_Consumer_t)
//!
////////////////////////////////////////////////////////////////////////////////
// Flag->MV_CC_CBUF_FLAGS_ALIGNMENT :	memory boundary block should be added to the Circular Buffer Length.
// If MV_CC_CBUF_FLAGS_ALIGNMENT_Yes exist, memory boundary block should be added to the Circular Buffer Length.
#define MV_CC_CBUF_FLAGS_ALIGNMENT				(UINT32)(0x00000001)		/* n */
#define MV_CC_CBUF_FLAGS_ALIGNMENT_MASK			(UINT32)(0xFFFFFFFE)		/* m */
#define MV_CC_CBUF_FLAGS_ALIGNMENT_No			(UINT32)(0)					/* a */
#define MV_CC_CBUF_FLAGS_ALIGNMENT_Yes			(UINT32)(1)					/* a */

// Flag->MV_CC_CBUF_FLAGS_NOTICE :   create a message queue for producer to post a message when writing data to inform comsuer to read cbuf.
// The notice status is used for MV_CC_CBuf_Create to create a message queue for producer to post a message when writing data to inform comsuer to read cbuf.
#define MV_CC_CBUF_FLAGS_NOTICESTAT				(UINT32)(0x00000002)		/* n */
#define MV_CC_CBUF_FLAGS_NOTICESTAT_MASK		(UINT32)(0xFFFFFFFD)		/* m */
#define MV_CC_CBUF_FLAGS_NOTICESTAT_No			(UINT32)(0)					/* a */
#define MV_CC_CBUF_FLAGS_NOTICESTAT_Yes			(UINT32)(1)					/* a */

// Flag->MV_CC_CBUF_FLAGS_LOCALCPU :   create a message queue used only on local CPU (allowed by Consumer)
// The MV_CC_CBUF_FLAGS_LOCALCPU_Yes status is used for MV_CC_CBuf_Create to create a message queue whihc used only on local CPU, so the buffer is in cache memory for faster access
#define MV_CC_CBUF_FLAGS_LOCALCPU				(UINT32)(0x00000004)		/* n */
#define MV_CC_CBUF_FLAGS_LOCALCPU_MASK			(UINT32)(0xFFFFFFFB)		/* m */
#define MV_CC_CBUF_FLAGS_LOCALCPU_No			(UINT32)(0)					/* a */
#define MV_CC_CBUF_FLAGS_LOCALCPU_Yes			(UINT32)(1)					/* a */

// MV_CC_StartStatus :
#define MV_CC_STATUS_BIT_START					(UINT32)(0x00000001)		/* n */
#define MV_CC_STATUS_OK							(UINT32)(0x00000001)


////////////////////////////////////////////////////////////////////////////////
//! \brief Global Service (GS)Definition : MV_CC_DSS_ServiceInfo_t
//!
////////////////////////////////////////////////////////////////////////////////
typedef struct _MV_CC_DSS_ServiceInfo {

	// public info
	MV_CC_ServiceID_U32_t 	m_ServiceID;

	UINT8 					m_SrvLevel;
	UINT8 					m_SrvType;
	UINT8					m_Pad1;
	UINT8					m_Pad2;

	// private info
	UINT8					m_Data[24];

} MV_CC_DSS_ServiceInfo_t, *pMV_CC_DSS_ServiceInfo_t;

typedef struct _MV_CC_DSS_ServiceInfo_MsgQ {

	// public info
	MV_CC_ServiceID_U32_t 	m_ServiceID;

	UINT8 					m_SrvLevel;
	UINT8 					m_SrvType;
	UINT8					m_Pad1;
	UINT8					m_Pad2;

	// private info
	UINT32 		 			m_NMsgsMax;
	UINT8					m_Blank[20];

} MV_CC_DSS_ServiceInfo_MsgQ_t, *pMV_CC_DSS_ServiceInfo_MsgQ_t;

typedef struct _MV_CC_DSS_ServiceInfo_MsgQEx {

	// public info
	MV_CC_ServiceID_U32_t 	m_ServiceID;

	UINT8 					m_SrvLevel;
	UINT8 					m_SrvType;
	UINT8					m_Pad1;
	UINT8					m_Pad2;

	// private info
	UINT32 		 			m_NMsgsMax;
	UINT32					m_MsgLenMax;
	CHAR					m_Blank[16];

} MV_CC_DSS_ServiceInfo_MsgQEx_t, *pMV_CC_DSS_ServiceInfo_MsgQEx_t;

typedef struct _MV_CC_DSS_ServiceInfo_RPC {

	// public info
	MV_CC_ServiceID_U32_t 	m_ServiceID;

	UINT8 					m_SrvLevel;
	UINT8 					m_SrvType;
	UINT8					m_Pad1;
	UINT8					m_Pad2;

	// private info
	UINT32 					m_RegCmdListNum;
	CHAR					m_Blank[20];

} MV_CC_DSS_ServiceInfo_RPC_t, *pMV_CC_DSS_ServiceInfo_RPC_t;

typedef struct _MV_CC_DSS_ServiceInfo_CBuf {

	// public info
	MV_CC_ServiceID_U32_t 	m_ServiceID;

	UINT8 					m_SrvLevel;
	UINT8 					m_SrvType;
	UINT8					m_Pad1;
	UINT8					m_Pad2;

	// private info
	UINT32                  m_seqid;                //! the number of CBuf client
        UINT32           		m_CBufBody_SHMOffset;   //! pointer to the CBuf Body
	UINT32 					m_BufSize;			    //! Circular Buffer data buffer byte size exclude the CBuf head and pending
	UINT32 					m_EndBlockSize;		    //! Circular Buffer Max Block byte size for reading or writing at a time, and also as the size of addition block after buffer end for circular operation
	UINT32 					m_Flags;			    //! flags of the Circular Buffer Client, initialize from CBuf settings flags when init;
	MV_CC_ServiceID_U32_t 	m_NoticeMsgQSID;	    //! It is only available on m_Flags = MV_CC_CBUF_FLAGS_NOTICESTAT_Yes

} MV_CC_DSS_ServiceInfo_CBuf_t, *pMV_CC_DSS_ServiceInfo_CBuf_t;

typedef struct _MV_CC_DSS_ServiceInfo_DataList {

	UINT32					m_ListNum;
	UINT32					m_DataNum;
	UINT32					m_MaxNum;

	MV_CC_DSS_ServiceInfo_t	m_SrvInfo[];

} MV_CC_DSS_ServiceInfo_DataList_t, *pMV_CC_DSS_ServiceInfo_DataList_t;

#ifdef __cplusplus
}		// extern "C"
#endif	// #ifdef __cplusplus

#endif
