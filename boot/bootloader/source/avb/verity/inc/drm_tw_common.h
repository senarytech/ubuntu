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

///////////////////////////////////////////////////////////////////////////////
//! \file   drm_tw_common.h 
//! \brief  Definition of drm ta common use
//! \author
//! \date
///////////////////////////////////////////////////////////////////////////////


#ifndef __DRM_TW_COMMON__
#define __DRM_TW_COMMON__

#define assert(_x_)							\
	do {								\
		if (!(_x_)) {						\
			printf("assert fail at "#_x_ " %s:%d/%s()\n",	\
					__FILE__, __LINE__, __func__);	\
			while(1);					\
		}							\
	}while(0)
#define GaloisMalloc                 malloc
//#define GaloisMemClear               memset
#define GaloisFree                   free
#define GaloisMemClear(ptr, size)    memset(ptr, 0x00, size)

#define MV_CRYPTO_KEY_HANDLE    void*

//#define E_SUC               ( 0x00000000 )
//#define E_ERR               ( 0x80000000 )

//#define E_GENERIC_BASE      ( 0x0000 << 16 )

// generic error code macro
//#define E_GEN_SUC( code )   ( E_SUC | E_GENERIC_BASE | ( code & 0x0000FFFF ) )
//#define E_GEN_ERR( code )   ( E_ERR | E_GENERIC_BASE | ( code & 0x0000FFFF ) )


//#define S_OK                E_GEN_SUC( 0x0000 ) // Success
//#define S_FALSE             E_GEN_SUC( 0x0001 ) // Success but return false status

#define E_NOTIMPL           E_GEN_ERR( 0x4001 ) // Not implemented
#define E_NOINTERFACE       E_GEN_ERR( 0x4002 ) // No such interface supported
#define E_POINTER           E_GEN_ERR( 0x4003 ) // Pointer that is not valid
#define E_ABORT             E_GEN_ERR( 0x4004 ) // Operation aborted
//#define E_FAIL              E_GEN_ERR( 0x4005 ) // Unspecified failure
//#define E_NOPERMISSION        E_GEN_ERR( 0x4006 ) // No permission to do sth
#define E_UNEXPECTED        E_GEN_ERR( 0xFFFF ) // Unexpected failure
#define E_ACCESSDENIED      E_GEN_ERR( 0x7005 ) // General access denied error
#define E_HANDLE            E_GEN_ERR( 0x7006 ) // Handle that is not valid
#define E_OUTOFMEMORY       E_GEN_ERR( 0x700E ) // Failed to allocate necessary memory
#define E_INVALIDARG        E_GEN_ERR( 0x7057 ) // One or more arguments are not valid

#define E_BADVALUE          E_GEN_ERR( 0x7060 ) // The value is illegal (general)
#define E_OUTOFRANGE        E_GEN_ERR( 0x7061 ) // The value is out of range
#define E_TIMEOUT           E_GEN_ERR( 0x7062 ) // Operation time Out
#define E_FULL              E_GEN_ERR( 0x7063 ) // Item is full (Queue or table etc...)
#define E_EMPTY             E_GEN_ERR( 0x7064 ) // Item is empty (Queue or table etc...)
#define E_NOTREADY          E_GEN_ERR( 0x7065 ) // Operation or item is not ready yet
#define E_ALREADYEXIST      E_GEN_ERR( 0x7066 ) // Tried to create existing item
#define E_NOTEXIST          E_GEN_ERR( 0x7067 ) // Tried to operate not existing item
#define E_FILEOPEN          E_GEN_ERR( 0x7068 ) // open file failed
#define E_FILEWRITE         E_GEN_ERR( 0x7069 ) // write file failed
#define E_FILEREAD          E_GEN_ERR( 0x7070 ) // read file failed
#define E_FILESEEK          E_GEN_ERR( 0x7071 ) // seek file failed
#define E_FILECLOSE         E_GEN_ERR( 0x7072 ) // close file failed

#define E_TASKFAIL          E_GEN_ERR( 0x7080 ) // task error (general)
#define E_TASKCREATE        E_GEN_ERR( 0x7081 ) // task creation failed
#define E_TASKDESTROY       E_GEN_ERR( 0x7082 ) // task destroying failed

//////////////////////////////////////////////////////////////////////////
void*    DRM_GaloisMalloc(unsigned int nSize);
void     DRM_GaloisFree();

//#define DEBUG_DRM
#ifdef DEBUG_DRM
#define TRACE(fmt, args...) printf("%s, %s(), L%d: " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ##args)
#else //! DEBUG_DRM
#define TRACE(fmt, args...) do {} while(0)
#endif // DEBUG_DRM

#define VM_CHIPID_LENGTH    8
#define TPV_CHIPID_SIZE    16

#endif
