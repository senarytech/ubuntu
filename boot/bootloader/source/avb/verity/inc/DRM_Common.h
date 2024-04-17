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
//! \file DRM_Common.h
//! \brief include nessary system headfiles, definition of some micro.
//!
//! Purpose:
//!
//!  Note:
////////////////////////////////////////////////////////////////////////////////


#ifndef __DRM_COMMON_H__
#define __DRM_COMMON_H__

#include "ctypes.h"


#ifdef WIN32
#       include <windows.h>
#       include "DRM_Memory.h"

#       define DRIVE_HANDLE HANDLE
#       define DRM_GaloisMalloc(_size)      _DebugMalloc(_size, THIS_FILE, __LINE__)
#       define DRM_GaloisFree(_pbPtr)       _DebugFree(_pbPtr)
#       define DRM_OrionMalloc(_size)       _DebugMalloc(_size, THIS_FILE, __LINE__)
#       define DRM_OrionFree(_pbPtr)        _DebugFree(_pbPtr)
#       define MemCheck()                   _DebugDumpMemoryLeak()

#else
#       include "com_type.h"
//#       include "ErrorCode.h"

//#       if defined(linux)
//#           include <linux/cdrom.h>
//#           define DRIVE_HANDLE int
//#       endif

#if 0
void*	 DRM_GaloisMalloc(size_t nSize);
void	 DRM_GaloisFree();
#endif

#       define DRM_OrionMalloc(_size)   GaloisMalloc(_size)
#       define DRM_OrionFree(_pbPtr)    GaloisFree(_pbPtr)
#       define MemCheck(...)

#       define INVALID_HANDLE_VALUE E_HANDLE
#endif

//! Declare DRM error code
#define E_DRM_SUC( code ) ( E_SUC | E_DRM_BASE | ( code & 0xFFFF ) )
#define E_DRM_ERR( code ) ( E_ERR | E_DRM_BASE | ( code & 0xFFFF ) )

//! Unknown AACS type : HD-DVD or BD
#define E_INVALIDAACSTYPE	E_GEN_ERR( 0x001 )

//loader type
typedef enum
{
        UNKNOWN_LOADER,
        DVD_LOADER,
        HDDVD_LOADER,
        BD_LOADER,
} LOADER_TYPE;


void MV_DRM_DebugTrace( const char* pszModuleName,
                       const char* pszFuncName,
                       UINT uLevel,
                       const char *szFmt, ...);

void MV_DRM_ErrorTrace( const char* pszModuleName,
                       const char* pszFuncName,
                       UINT uLevel,
                       const char *szFmt, ...);


void MV_DRM_TraceHex(const char* pszModuleName,
                       const char* pszFuncName,
                       UINT uLevel,
                       const unsigned char* pbData, UINT32 uSize );
#ifdef NO_RPC
#   define MV_DRM_SHM_Malloc(_key,_size) DRM_GaloisMalloc(_size)
#   define MV_DRM_SHM_Free(_pbPtr) DRM_GaloisFree(_pbPtr)
#else
#   define MV_DRM_SHM_Malloc(_key,_size) MV_DRM_CC_SHM_Malloc(_key,_size)
#   define MV_DRM_SHM_Free(_pbPtr) MV_DRM_CC_SHM_Free(_pbPtr)
#endif


#define ERROR_LEVEL1	    NULL,           __FUNCTION__,  0,
#define UNIT_TEST_LEVEL1    "AACSUT",       __FUNCTION__,  0,
#define	LD_LEVEL1		    "AACSLOADER",   __FUNCTION__,  0,

#define AACS_AGENT_LEVEL1	"AACSAGENT ",    __FUNCTION__,  1,
#define AACS_SERVER_LEVEL1	"AACSSERVER",    __FUNCTION__,  1,
#define AACS_SERVER_LEVEL2	"AACSSERVER",    __FUNCTION__,  2,
#define AACS_PACK_LEVEL	    "AACSDEC",       __FUNCTION__,  5,

#define CSS_AGENT_LEVEL1	"CSSAGENT ",    __FUNCTION__,  1,
#define CSS_SERVER_LEVEL1	"CSSSERVER",    __FUNCTION__,  1,
#define CSS_SERVER_LEVEL2	"CSSSERVER",    __FUNCTION__,  2,
#define CSS_PACK_LEVEL	    "CSSDEC",       __FUNCTION__,  5,


#define WMDRM_AGENT_LEVEL1	"WMDRMAGENT ",    __FUNCTION__,  1,
#define WMDRM_SERVER_LEVEL1	"WMDRMSERVER",    __FUNCTION__,  1,
#define WMDRM_SERVER_LEVEL2	"WMDRMSERVER",    __FUNCTION__,  2,
#define WMDRM_PACK_LEVEL	    "WMDRMDEC",       __FUNCTION__,  5,

#define PLAYREADY_AGENT_LEVEL1      "PLAYREADYAGENT ",    __FUNCTION__,  1,
#define PLAYREADY_SERVER_LEVEL1     "PLAYREADYSERVER",    __FUNCTION__,  1,
#define PLAYREADY_SERVER_LEVEL2     "PLAYREADYSERVER",    __FUNCTION__,  2,
#define PLAYREADY_PACK_LEVEL        "PLAYREADYDEC",       __FUNCTION__,  5,

#define DIVXDRM_AGENT_LEVEL1	"DIVXDRMAGENT ",    __FUNCTION__,  1,
#define DIVXDRM_SERVER_LEVEL1	"DIVXDRMSERVER",    __FUNCTION__,  1,
#define DIVXDRM_SERVER_LEVEL2	"DIVXDRMSERVER",    __FUNCTION__,  2,
#define DIVXDRM_PACK_LEVEL	    "DIVXDRMDEC",       __FUNCTION__,  5,

#define DTCPIP_AGENT_LEVEL1	"DTCPIPAGENT ",    __FUNCTION__,  1,
#define DTCPIP_SERVER_LEVEL1	"DTCPIPSERVER",    __FUNCTION__,  1,
#define DTCPIP_SERVER_LEVEL2	"DTCPIPSERVER",    __FUNCTION__,  2,
#define DTCPIP_PACK_LEVEL	    "DTCPIPDEC",       __FUNCTION__,  5,

#define CUSTOMER_AGENT_LEVEL1	"CUSTOMERAGENT ",    __FUNCTION__,  1,
#define CUSTOMER_SERVER_LEVEL1	"CUSTOMERSERVER",    __FUNCTION__,  1,
#define CUSTOMER_SERVER_LEVEL2	"CUSTOMERSERVER",    __FUNCTION__,  2,
#define CUSTOMER_PACK_LEVEL	    "CUSTOMERDEC",       __FUNCTION__,  5,

#define CPRM_AGENT_LEVEL1	"CPRMAGENT ",    __FUNCTION__,  1,
#define CPRM_SERVER_LEVEL1	"CPRMSERVER",    __FUNCTION__,  1,
#define CPRM_SERVER_LEVEL2	"CPRMSERVER",    __FUNCTION__,  2,
#define CPRM_PACK_LEVEL	    "CPRMDEC",       __FUNCTION__,  5,



#define WIDEVINE_AGENT_LEVEL1	"WIDEVINEAGENT ",    __FUNCTION__,  1,
#define WIDEVINE_SERVER_LEVEL1	"WIDEVINESERVER",    __FUNCTION__,  1,
#define WIDEVINE_SERVER_LEVEL2	"WIDEVINESERVER",    __FUNCTION__,  2,
#define WIDEVINE_PACK_LEVEL	    "WIDEVINEDEC",       __FUNCTION__,  5,

#define MOBITV_AGENT_LEVEL1	    "MOBITVAGENT ",    __FUNCTION__,  1,
#define MOBITV_AGENT_LEVEL2	    "MOBITVAGENT ",    __FUNCTION__,  2,
#define MOBITV_SERVER_LEVEL1	"MOBITVSERVER",    __FUNCTION__,  1,
#define MOBITV_SERVER_LEVEL2	"MOBITVSERVER",    __FUNCTION__,  2,

#define CRYPTOENGINE_AGENT_LEVEL1	"CRYPTOENGINEAGENT ",    __FUNCTION__,  1,
#define CRYPTOENGINE_SERVER_LEVEL1	"CRYPTOENGINESERVER",    __FUNCTION__,  1,
#define CRYPTOENGINE_SERVER_LEVEL2	"CRYPTOENGINESERVER",    __FUNCTION__,  2,
#define CRYPTOENGINE_PACK_LEVEL	    "CRYPTOENGINEDEC",       __FUNCTION__,  5,

#define VUDU_AGENT_LEVEL1	"VUDUAGENT ",    __FUNCTION__,  1,
#define VUDU_SERVER_LEVEL1	"VUDUSERVER",    __FUNCTION__,  1,
#define VUDU_SERVER_LEVEL2	"VUDUSERVER",    __FUNCTION__,  2,
#define VUDU_PACK_LEVEL	    "VUDUDEC",       __FUNCTION__,  5,

#define STUB_AGENT_LEVEL1	"STUBAGENT ",    __FUNCTION__,  1,
#define STUB_SERVER_LEVEL1	"STUBSERVER",    __FUNCTION__,  1,
#define STUB_SERVER_LEVEL2	"STUBSERVER",    __FUNCTION__,  2,
#define STUB_PACK_LEVEL	    "STUBDEC",       __FUNCTION__,  5,



#define CRYPTO_LEVEL0		"CRYPTOAPI",     __FUNCTION__,  2,
#define CRYPTO_LEVEL1		"CRYPTOAPI",     __FUNCTION__,  3,
#define CRYPTO_LEVEL2		"CRYPTOAPI",     __FUNCTION__,  4,

#define DRMFIGO_LEVEL0       "DRMFIGO",     __FUNCTION__,   0,
#define DRMFIGO_LEVEL1       "DRMFIGO",     __FUNCTION__,   1,
#define DRMFIGO_LEVEL2       "DRMFIGO",     __FUNCTION__,   2,

#if   CONFIG_DEBUG_DRM
#define ENABLE_DEBUG_DRM
#endif


#if defined(ENABLE_DEBUG_DRM)
#define LD_DEBUG            MV_DRM_DebugTrace
#define LD_ERROR            MV_DRM_ErrorTrace

#define MV_CRYPTO_ERROR        MV_DRM_ErrorTrace
#define MV_CRYPTO_DEBUG        MV_DRM_DebugTrace

#define AACS_AGENT_DEBUG    MV_DRM_DebugTrace
#define AACS_AGENT_ERROR    MV_DRM_ErrorTrace

#define WMDRM_AGENT_DEBUG    MV_DRM_DebugTrace
#define WMDRM_AGENT_ERROR    MV_DRM_ErrorTrace

#define PLAYREADY_AGENT_DEBUG MV_DRM_DebugTrace
#define PLAYREADY_AGENT_ERROR MV_DRM_ErrorTrace

#define DIVXDRM_AGENT_DEBUG    MV_DRM_DebugTrace
#define DIVXDRM_AGENT_ERROR    MV_DRM_ErrorTrace

#define DTCPIP_AGENT_DEBUG    MV_DRM_DebugTrace
#define DTCPIP_AGENT_ERROR    MV_DRM_ErrorTrace

#define CUSTOMER_AGENT_DEBUG    MV_DRM_DebugTrace
#define CUSTOMER_AGENT_ERROR    MV_DRM_ErrorTrace

#define CPRM_AGENT_DEBUG    MV_DRM_DebugTrace
#define CPRM_AGENT_ERROR    MV_DRM_ErrorTrace

#define WIDEVINE_AGENT_DEBUG    MV_DRM_DebugTrace
#define WIDEVINE_AGENT_ERROR    MV_DRM_ErrorTrace

#define CRYPTOENGINE_AGENT_DEBUG    MV_DRM_DebugTrace
#define CRYPTOENGINE_AGENT_ERROR    MV_DRM_ErrorTrace

#define VUDU_AGENT_DEBUG    MV_DRM_DebugTrace
#define VUDU_AGENT_ERROR    MV_DRM_ErrorTrace

#define STUB_AGENT_DEBUG    MV_DRM_DebugTrace
#define STUB_AGENT_ERROR    MV_DRM_ErrorTrace

#define DUMMY_AGENT_DEBUG    MV_DRM_DebugTrace
#define DUMMY_AGENT_ERROR    MV_DRM_ErrorTrace

#define VMX_AGENT_DEBUG    MV_DRM_DebugTrace
#define VMX_AGENT_ERROR    MV_DRM_ErrorTrace

#define AACS_SERVER_DEBUG	MV_DRM_DebugTrace
#define AACS_SERVER_ERROR	MV_DRM_ErrorTrace
#define AACS_SERVER_TRACEHEX	MV_DRM_TraceHex

#define AACS_PACK_DEBUG     MV_DRM_DebugTrace
#define AACS_AGENT_TRACEHEX	MV_DRM_TraceHex


#define CSS_AGENT_DEBUG    MV_DRM_DebugTrace
#define CSS_AGENT_ERROR    MV_DRM_ErrorTrace



#define CSS_SERVER_DEBUG	MV_DRM_DebugTrace
#define CSS_SERVER_ERROR	MV_DRM_ErrorTrace
#define CSS_SERVER_TRACEHEX	MV_DRM_TraceHex

#define CSS_PACK_DEBUG     MV_DRM_DebugTrace
#define CSS_AGENT_TRACEHEX	MV_DRM_TraceHex

#define WMDRM_SERVER_DEBUG	MV_DRM_DebugTrace
#define WMDRM_SERVER_ERROR	MV_DRM_ErrorTrace
#define WMDRM_SERVER_TRACEHEX	MV_DRM_TraceHex

#define PLAYREADY_SERVER_DEBUG      MV_DRM_DebugTrace
#define PLAYREADY_SERVER_ERROR      MV_DRM_ErrorTrace
#define PLAYREADY_SERVER_TRACEHEX   MV_DRM_TraceHex

#define DIVXDRM_SERVER_DEBUG	MV_DRM_DebugTrace
#define DIVXDRM_SERVER_ERROR	MV_DRM_ErrorTrace
#define DIVXDRM_SERVER_TRACEHEX	MV_DRM_TraceHex

#define DTCPIP_SERVER_DEBUG	MV_DRM_DebugTrace
#define DTCPIP_SERVER_ERROR	MV_DRM_ErrorTrace
#define DTCPIP_SERVER_TRACEHEX	MV_DRM_TraceHex

#define CUSTOMER_SERVER_DEBUG	MV_DRM_DebugTrace
#define CUSTOMER_SERVER_ERROR	MV_DRM_ErrorTrace
#define CUSTOMER_SERVER_TRACEHEX	MV_DRM_TraceHex

#define CPRM_SERVER_DEBUG	MV_DRM_DebugTrace
#define CPRM_SERVER_ERROR	MV_DRM_ErrorTrace
#define CPRM_SERVER_TRACEHEX	MV_DRM_TraceHex


#define WIDEVINE_SERVER_DEBUG	MV_DRM_DebugTrace
#define WIDEVINE_SERVER_ERROR	MV_DRM_ErrorTrace
#define WIDEVINE_SERVER_TRACEHEX	MV_DRM_TraceHex

#define MOBITV_SERVER_DEBUG	    MV_DRM_DebugTrace
#define MOBITV_SERVER_ERROR	    MV_DRM_ErrorTrace
#define MOBITV_SERVER_TRACEHEX	MV_DRM_TraceHex

#define MOBITV_AGENT_DEBUG	MV_DRM_DebugTrace
#define MOBITV_AGENT_ERROR	MV_DRM_ErrorTrace
#define MOBITV_AGENT_TRACEHEX	MV_DRM_TraceHex


#define VUDU_SERVER_DEBUG	MV_DRM_DebugTrace
#define VUDU_SERVER_ERROR	MV_DRM_ErrorTrace
#define VUDU_SERVER_TRACEHEX	MV_DRM_TraceHex

#define STUB_SERVER_DEBUG	MV_DRM_DebugTrace
#define STUB_SERVER_ERROR	MV_DRM_ErrorTrace
#define STUB_SERVER_TRACEHEX	MV_DRM_TraceHex

#define VMX_SERVER_DEBUG	MV_DRM_DebugTrace
#define VMX_SERVER_ERROR	MV_DRM_ErrorTrace
#define VMX_SERVER_TRACEHEX	MV_DRM_TraceHex

#define DUMMY_SERVER_DEBUG	MV_DRM_DebugTrace
#define DUMMY_SERVER_ERROR	MV_DRM_ErrorTrace
#define DUMMY_SERVER_TRACEHEX	MV_DRM_TraceHex

#define CRYPTOENGINE_SERVER_DEBUG	MV_DRM_DebugTrace
#define CRYPTOENGINE_SERVER_ERROR	MV_DRM_ErrorTrace
#define CRYPTOENGINE_SERVER_TRACEHEX	MV_DRM_TraceHex


#else
#define LD_DEBUG(...)
#define LD_ERROR(...)


#define CSS_AGENT_DEBUG(...)
#define CSS_AGENT_ERROR(...)



#define CSS_SERVER_DEBUG(...)
#define CSS_SERVER_ERROR(...)
#define CSS_SERVER_TRACEHEX(...)

#define CSS_PACK_DEBUG(...)
#define CSS_AGENT_TRACEHEX(...)

#define MV_CRYPTO_ERROR(...)
#define MV_CRYPTO_DEBUG(...)

#define AACS_AGENT_DEBUG(...)
#define AACS_AGENT_ERROR(...)

#define CUSTOMER_AGENT_DEBUG(...)
#define CUSTOMER_AGENT_ERROR(...)


#define AACS_SERVER_DEBUG(...)
#define AACS_SERVER_ERROR(...)
#define AACS_SERVER_TRACEHEX(...)


#define AACS_PACK_DEBUG(...)
#define AACS_AGENT_TRACEHEX(...)

#define WMDRM_AGENT_DEBUG(...)
#define WMDRM_AGENT_ERROR(...)

#define WMDRM_SERVER_DEBUG(...)
#define WMDRM_SERVER_ERROR(...)
#define WMDRM_SERVER_TRACEHEX(...)

#define PLAYREADY_AGENT_DEBUG(...)
#define PLAYREADY_AGENT_ERROR(...)

#define PLAYREADY_SERVER_DEBUG(...)
#define PLAYREADY_SERVER_ERROR(...)
#define PLAYREADY_SERVER_TRACEHEX(...)


#define DIVXDRM_AGENT_DEBUG(...)
#define DIVXDRM_AGENT_ERROR(...)

#define DTCPIP_AGENT_DEBUG(...)
#define DTCPIP_AGENT_ERROR(...)


#define CPRM_AGENT_DEBUG(...)
#define CPRM_AGENT_ERROR(...)

#define WIDEVINE_AGENT_DEBUG(...)
#define WIDEVINE_AGENT_ERROR(...)


#define CRYPTOENGINE_AGENT_DEBUG(...)
#define CRYPTOENGINE_AGENT_ERROR(...)

#define VUDU_AGENT_DEBUG(...)
#define VUDU_AGENT_ERROR(...)

#define STUB_AGENT_DEBUG(...)
#define STUB_AGENT_ERROR(...)

#define VMX_AGENT_DEBUG(...)
#define VMX_AGENT_ERROR(...)

#define DUMMY_AGENT_DEBUG(...)
#define DUMMY_AGENT_ERROR(...)

#define DIVXDRM_SERVER_DEBUG(...)
#define DIVXDRM_SERVER_ERROR(...)
#define DIVXDRM_SERVER_TRACEHEX(...)

#define DTCPIP_SERVER_DEBUG(...)
#define DTCPIP_SERVER_ERROR(...)
#define DTCPIP_SERVER_TRACEHEX(...)

#define CUSTOMER_SERVER_DEBUG(...)
#define CUSTOMER_SERVER_ERROR(...)
#define CUSTOMER_SERVER_TRACEHEX(...)

#define CPRM_SERVER_DEBUG(...)
#define CPRM_SERVER_ERROR(...)
#define CPRM_SERVER_TRACEHEX(...)


#define WIDEVINE_SERVER_DEBUG(...)
#define WIDEVINE_SERVER_ERROR(...)
#define WIDEVINE_SERVER_TRACEHEX(...)

#define MOBITV_AGENT_DEBUG(...)
#define MOBITV_AGENT_ERROR(...)
#define MOBITV_AGENT_TRACEHEX(...)

#define MOBITV_SERVER_DEBUG(...)
#define MOBITV_SERVER_ERROR(...)
#define MOBITV_SERVER_TRACEHEX(...)


#define VUDU_SERVER_DEBUG(...)
#define VUDU_SERVER_ERROR(...)
#define VUDU_SERVER_TRACEHEX(...)

#define STUB_SERVER_DEBUG(...)
#define STUB_SERVER_ERROR(...)
#define STUB_SERVER_TRACEHEX(...)


#define VMX_SERVER_DEBUG(...)
#define VMX_SERVER_ERROR(...)
#define VMX_SERVER_TRACEHEX(...)

#define DUMMY_SERVER_DEBUG(...)
#define DUMMY_SERVER_ERROR(...)
#define DUMMY_SERVER_TRACEHEX(...)

#define  CRYPTOENGINE_SERVER_DEBUG(...)
#define  CRYPTOENGINE_SERVER_ERROR(...)
#define  CRYPTOENGINE_SERVER_TRACEHEX(...)


#endif


#define AACS_CTX_MAGIC_ID           0x9018361
#define AACS_DECODER_CTX_MAGIC_ID   0x8859204
#define CSS_CTX_MAGIC_ID			0x7423803
#define CRYPTO_KEY_CTX_MAGIC_ID     0x1645902
#define CRYPTO_HASH_CTX_MAGIC_ID    0x1845902
#define WMDRM_CTX_MAGIC_ID           0x8017438
#define PLAYREADY_CTX_MAGIC_ID	     0x80768269
#define WMDRM_DECODER_CTX_MAGIC_ID   0x5725903

#define CPRM_CTX_MAGIC_ID           0x4350524D

#define DIVXDRM_CTX_MAGIC_ID           0x7017418
#define DIVXDRM_DECODER_CTX_MAGIC_ID   0x8835903

#define DTCPIP_CTX_MAGIC_ID           0x7911418

#define WIDEVINEDRM_CTX_MAGIC_ID      0x57564E45
#define VUDUDRM_CTX_MAGIC_ID      0x56554455

#define MOBITVDRM_CTX_MAGIC_ID		0x11112222


#define CRYPTOENGINE_CTX_MAGIC_ID      0x88de3010

#define OPENCRYPTO_CTX_AGENT_MAGIC_ID  	0x42434445
#define OPENCRYPTO_CTX_SERVER_MAGIC_ID  	0x45444342

#define VMXDRM_CTX_MAGIC_ID         0x564D580A

#define STUBDRM_CTX_MAGIC_ID		0x53545542
#define HDCP2DECODE_CTX_MAGIC_ID	0x88de3310
#define DUMMYDRM_CTX_MAGIC_ID		0x44554D59

#define INVALID_CTX_ID              NULL

#define MV_N2LS(_sVal)              ((((_sVal) >> 8) & 0xff) | (((_sVal) & 0xff) << 8))

#define MV_L2NS(_sVal)              MV_N2LS(_sVal)

#define MV_N2LL(_lVal)              ((((_lVal) >> 24) & 0xff) | (((_lVal) >> 8) & 0xff00) | \
                                    (((_lVal) << 8) & 0xff0000) | (((_lVal) << 24) & 0xff000000))
#define MV_L2NL(_lVal)              MV_N2LL(_sVal)

#define UINT128_FMTSTR    "0x%02X%02X%02X%02X%02X%02X%02X%02X %02X%02X%02X%02X%02X%02X%02X%02X"

#define UINT128_FMTDATA(_pbData)                          \
        ((UINT8*)(_pbData))[0], ((UINT8*)(_pbData))[1],   \
        ((UINT8*)(_pbData))[2], ((UINT8*)(_pbData))[3],   \
        ((UINT8*)(_pbData))[4], ((UINT8*)(_pbData))[5],   \
        ((UINT8*)(_pbData))[6], ((UINT8*)(_pbData))[7],   \
        ((UINT8*)(_pbData))[8], ((UINT8*)(_pbData))[9],   \
        ((UINT8*)(_pbData))[10], ((UINT8*)(_pbData))[11], \
        ((UINT8*)(_pbData))[12], ((UINT8*)(_pbData))[13], \
        ((UINT8*)(_pbData))[14], ((UINT8*)(_pbData))[15]

void  MV_DRM_SetTraceLevel( UINT nNewLevel );

void* MV_DRM_UnCacheMallocAlign32(UINT uSize);
void  MV_DRM_UnCacheFreeAlign32(void* pbDat);
void  MV_DRM_TraceGaloisMallocStatus();

#define AACS_NONCE_LEN      20
#define AACS_CERT_LEN       92
#define AACS_ECP_LEN        40
#define AACS_VID_LEN        16
#define AACS_KCD_LEN        16
#define AACS_MEDIAID_LEN    16
#define AACS_BN_LEN        16
#define AACS_PMSN_LEN       16
#define AACS_PATH_LEN       128
#define AACS_MAC_LEN        16
#define AACS_SIGN_LEN       40
#define AACS_PRVKEY_LEN     20
#define AACS_HASH_LEN		20

#ifdef WIN32
#define DISC_HARDDRIVE_PATH "X:\\server06\\The_Descent_Original"
#else
#define DISC_HARDDRIVE_PATH "./disc/bdj-demo"
#endif

//#define AACS_HW_KEY_LADDERING_ENABLED 1
#define FAIL_REGION_CHECK 0x6B
#define AACS_Debug_Printf(format,...)  \
            printf("File: "__FILE__", Line: %05d: "format"", __LINE__, ##__VA_ARGS__)
//#define AACS_Debug_Printf(format,...)


#ifdef CONFIG_NEW_SECSTORE
#define NEW_SECURE_STORE
#endif

#ifdef CONFIG_ENABLE_PLAYREADY
#define enable_playready    1
#endif

#ifdef CONFIG_ENABLE_PRFIGOFW
#define __DRM_SECURE_AV_PATH__
#endif

#ifdef CONFIG_DMX_FIGO
#define USE_DMX_FIGO
#endif

#if (CONFIG_ENABLE_MDK == 1)
//#define __DRM_SECURE_FIGOIMG_MDK__
#endif

//This macro is to enable drmfwmgr
#define CONFIG_ENABLE_DRMFWMGR


//define macro to enable TZ function
#define CONFIG_ENABLE_TZ

#endif
