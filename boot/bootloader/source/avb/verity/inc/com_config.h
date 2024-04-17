/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2018 Synaptics Incorporated. All rights reserved.
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
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
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


////////////////////////////////////////////////////////////////////////////////
//! \file com_config.h
//! \brief 
//!			
//!	
//! Purpose:
//!	   
//!
//!	Version, Date and Author :
//!		V 1.00,	3/14/2008,	Fang Bao	
//!		V 1.01,	7/22/2008,	Fang Bao	add TEST_RPC_XXX for RPC performance and trace test
//!		V 1.02,	8/21/2008,	Fang Bao	add BERLIN_PEK for timer configuration
//!		V 1.03,	10/17/2008,	Fang Bao	modify timer configuration of new BERLIN_PEK
//!		V 1.04,	11/03/2008,	Fang Bao	add SYSTEM_TIMER_CLOCK to replace SYSTEM_TIMER_RATIO
//!		V 1.05,	1/07/2009,	Fang Bao	modify SYSTEM_TIMER_CLOCK, (CYGNUM_HAL_RTC_PERIOD of "hal_arm_arm9_galois.h")
//!		V 1.06,	2/17/2009,	Fang Bao	add macro for (#include "hal_arm_arm9_galois.h"), it is available in Linux User and eCos
//!		V 1.07,	4/08/2009,	Fang Bao	remove dependency of hal_arm_arm9_galois.h, set SYSTEM_TIMER_CLOCK as a fixed value
//!		V 1.08,	6/3/2009,	Fang Bao	add MV_OS_TICKNUM_PER_SECOND to compute CYGNUM_HAL_RTC_PERIOD in driver file "galois_misc.c"
//!		V 1.09,	7/10/2009,	Fang Bao	add MV_ADDR_system_Clock_Timer and MV_ADDR_system_Clock_Second which are in driver file "galois_int.c"
//!		V 1.10,	5/07/2009,	Fang Bao	add MV_LIBRARY_PATH_NAVIGATOR, for dynamically loading library
//!     V 1.11, 5/11/2009,  Fang Bao    add MV_LIBRARY_PATH_PLAYER, for dynamically loading library
//!     V 1.12, 5/28/2009,  Fang Bao    remove selection of MV_LIBRARY_PATH_PLAYER and MV_LIBRARY_PATH_NAVIGATOR
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef _COM_CONFIG_H_
#define _COM_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif	// #ifdef __cplusplus


////////////////////////////////////////////////////////////////////////////////
//! \brief Hardware definition 
//!
////////////////////////////////////////////////////////////////////////////////

#define MV_CC_HAL_MEMBOUNDARY				(64)			// in bytes, Hardware Memroy Boundary Size (DMA operation)

//#include "sys_memmap.h"
//------------------------------------------------------------------------------
//! \brief Memory definition for Shared Memory
#define MV_NONCACHE_Base					(MEMMAP_DRAM_UNCACHE_BASE) // refer to sys_memmap.h
#ifdef __LINUX_KERNEL__
#define MV_NONCACHE_Start					((MV_NONCACHE_Base) | (CONFIG_MV88DE3010_SHMMEM_START))
#define MV_NONCACHE_Size					(CONFIG_MV88DE3010_SHMMEM_SIZE)				// 64M Size for ICC FIFO, ICC EXTRA and Shared Memory
#else
#define MV_NONCACHE_Start					(GALOIS_NON_CACHE_START)  // refer to sys_memmap.h
#define MV_NONCACHE_Size					(GALOIS_NON_CACHE_SIZE)	  // refer to sys_memmap.h, 64M Size for ICC FIFO, ICC EXTRA and Shared Memory
#endif
#define MV_ICC_FIFO_ADDR_def				(MV_NONCACHE_Start)
#define MV_ICC_FIFO_SIZE_def				(0x100000)					// 1M data for ICC FIFO
#define MV_ICC_EXTRA_ADDR_def				(MV_NONCACHE_Start + MV_ICC_FIFO_SIZE_def)
#define MV_ICC_EXTRA_SIZE_def				(0x1000)					// 4K extra data for ICC
#ifdef __LINUX_KERNEL__
#define MV_ICC_FIFO_ADDR_def_KernVirt		(CONFIG_MV88DE3010_SHMMEM_VIRTSTART)						// Linux kernel Virtual Address of 0x5C000000
#define MV_ICC_EXTRA_ADDR_def_KernVirt		(MV_ICC_FIFO_ADDR_def_KernVirt + MV_ICC_FIFO_SIZE_def)			// Linux kernel Virtual Address
#endif



//------------------------------------------------------------------------------
//! \brief Timer

#if defined( __ECOS__ )

// The tick number per second in eCos OS
#define MV_OS_TICKNUM_PER_SECOND			(100)								// please ref the eCos setting
#define MV_OS_MS_PER_TICK					(1000 / MV_OS_TICKNUM_PER_SECOND)	
#define CYG_MS_PER_TICK						(MV_OS_MS_PER_TICK)					// from com_type.h

#endif

#if !defined(WIN32) && !defined(SoC_PCSim)

#define TIMER_SYSTEM						(6)								/* system timer, refer to pic_cpu_intr.h */
#define TIMER_DEBUG							(7)								/* performance timer */

//hal_arm_arm9_galois.h is in TOPDIR "/OS/eCos/ARM/include/pkgconf/hal_arm_arm9_galois.h"

#if defined( __LINUX__ ) || defined( __ECOS__ )
//#include "hal_arm_arm9_galois.h"
//#define SYSTEM_TIMER_CLOCK					((CYGNUM_HAL_RTC_PERIOD + 1) * 100 )
#if defined(FPGA)
#define SYSTEM_TIMER_CLOCK					(5000000)				// 5 MHz
#elif (BERLIN_CHIP_VERSION < BERLIN_B_0)
#define SYSTEM_TIMER_CLOCK					(65625000)				// 65.625 MHz
#elif (BERLIN_CHIP_VERSION >= BERLIN_B_0)
#define SYSTEM_TIMER_CLOCK					(75000000)				// 75 MHz
#else
#define SYSTEM_TIMER_CLOCK					(75000000)				// 75 MHz
#endif
#define SYSTEM_TIMER_RATIO					(1*1000*1000*1000 / SYSTEM_TIMER_CLOCK)
#define SYSTEM_TIMER_DELTA					(0xFFFFFFFF - 100 * 1000 / SYSTEM_TIMER_RATIO) 	
#endif

#define MV_TIME_CLOCK_UPDATE_MS				(200)

#define SYSTEM_TIMER_CLOCK_PROCFILE			"/proc/mv88de3010_timerfreq"

#define SYSTEM_TIMER_VALUE					(0xFFFFFFFF)
#define DEBUG_TIMER_VALUE					(0xFFFFFFFF)

#define PFTIMER_DEVICE 						"/dev/pftimer"

#define MV_SHM_DeviceName_ICCEXTRA			"/dev/MV_CC_SHM2"
#define MV_ADDR_system_timer_jiffies		(0)
#define MV_SIZE_system_timer_jiffies		(4)
#define MV_ADDR_system_Clock_Timer			(4)
#define MV_SIZE_system_Clock_Timer			(4)
#define MV_ADDR_system_Clock_Second			(8)
#define MV_SIZE_system_Clock_Second			(4)

#define MV_REGMAP_DEVICE					"/dev/regmap"
#define MV_ADDR_REG_TIMER_BASE				(0xF7F32000)
#define MV_SIZE_REG_TIMER_BASE				(0x1000)
#define MV_ADDR_REG_TIMER_OFFSET			(0x800)
#define MV_ADDR_REG_TIMER6_VALUE			(MV_ADDR_REG_TIMER_BASE + MV_ADDR_REG_TIMER_OFFSET + 0x14 * 6 + 0x04)
#define MV_SIZE_REG_TIMER6_VALUE			(4)
#define MV_ADDR_REG_TIMER7_VALUE			(MV_ADDR_REG_TIMER_BASE + MV_ADDR_REG_TIMER_OFFSET + 0x14 * 7 + 0x04)
#define MV_SIZE_REG_TIMER7_VALUE			(4)

#define MV_REG_TIMER_6						(6)
#define MV_REG_TIMER_7						(7)

#endif	// #if !defined(WIN32) && !defined(SoC_PCSim)


//------------------------------------------------------------------------------
//! \brief Log Type (used in mon_logtype_list and other Log user)

#define LOGTYPE_DEBUG_ALL	   				(0x00000001)
#define LOGTYPE_DEBUG_PE	   				(0x00010001)
#define LOGTYPE_TEST_RPC_PF   				(0x00010002)
#define LOGTYPE_TEST_RPC_TRACE				(0x00010003)
#define LOGTYPE_CPULOAD		   				(0x00010004)
#define LOGTYPE_CPUSWITCH	   				(0x00010005)
#define LOGTYPE_BUFFER						(0x00010006)
#define LOGTYPE_MEMORY						(0x00010007)   //for memory usage

#define LOGTYPE_TEST_RPC_TRACE_Num			(0x0c)			// number of the point
#define LOGTYPE_TEST_RPC_TRACE_PointSize	(0x04)			// 
#define LOGTYPE_TEST_RPC_TRACE_AllSize		(LOGTYPE_TEST_RPC_TRACE_Num * LOGTYPE_TEST_RPC_TRACE_PointSize)			// 

#define LOGTYPE_TEST_RPC_TRACE_0			(0x00)			// (CPU0 Linux User Space) User Task -> RPC Start 
#define LOGTYPE_TEST_RPC_TRACE_1			(0x01)			// (CPU0 Linux Kernel) MV_ICCNetWrite_process_message_thread 
#define LOGTYPE_TEST_RPC_TRACE_2			(0x02)			// (CPU1 eCos) MV_CC_MidSrv_ServerTask()->MV_CC_HAL_SemWait()
#define LOGTYPE_TEST_RPC_TRACE_3			(0x03)			// (CPU1 eCos) MV_CC_RPCSrv_Task()->MV_CC_RPC_TaskPool_WaitSignalAndGet()
#define LOGTYPE_TEST_RPC_TRACE_4			(0x04)			// (CPU1 eCos) MV_CC_RPCSrv_Task()->MV_CC_RPCSrv_Return()
#define LOGTYPE_TEST_RPC_TRACE_5			(0x05)			// (CPU0 Linux Kernel) (Linux kernel)->MV_ICCNetRead_process_message_thread()->MV_CC_HAL_SemWait()
#define LOGTYPE_TEST_RPC_TRACE_6			(0x06)			// (CPU0 Linux User Space) MV_CC_MidSrv_ServerTask()->recvmsg()
#define LOGTYPE_TEST_RPC_TRACE_7			(0x07)			// (CPU0 Linux User Space) MV_CC_MidSrv_ServerTask()->MV_CC_RPCClnt_OnUDPRecv()
#define LOGTYPE_TEST_RPC_TRACE_8			(0x08)			// (CPU0 Linux User Space) User Task -> MV_OSAL_Sem_TimedWait() RPC End

#define LOGTYPE_TEST_RPC_TRACE_CPU1INT		(0x09)			// (CPU1 INT)
#define LOGTYPE_TEST_RPC_TRACE_CPU1ISR		(0x0a)			// (CPU1 ISR)
#define LOGTYPE_TEST_RPC_TRACE_CPU1DSR		(0x0b)			// (CPU1 DSR)

#define TEST_RPC_TRACE_TIMER				(MV_REG_TIMER_6)
#define TEST_CPU_SWITCH_TIMER				(MV_REG_TIMER_6)

//#define ENABLE_TEST_RPC_TRACE
//#define ENABLE_TEST_RPC

////////////////////////////////////////////////////////////////////////////////
//! \brief Library definition 
//!
////////////////////////////////////////////////////////////////////////////////

#define MV_LIBRARY_PATH_NAVMGR            "libNavMgr.so"
#define MV_LIBRARY_PATH_NAVIGATOR         "libNavi.so"
#define MV_LIBRARY_PATH_PLAYER            "libPlayer.so"

#ifdef __cplusplus
}		// extern "C"
#endif	// #ifdef __cplusplus

#endif
