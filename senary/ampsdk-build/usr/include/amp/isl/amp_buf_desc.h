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

////////////////////////////////////////////////////////////////////////////////
//! \file amp_buf_desc.h
//!
//! \brief This file used to define all buffer, buffer descriptor and buffer
//!        descriptor interface for AMP.
//!
//! Purpose:
//!
//!
//! Version, Date and Author :
//!   V 1.00,    December 7 2012,   Justin Wu
//!   V 1.01,    December 12 2012,  Weizhao Jiang
//!   V 1.02,    December 19 2012,  Weizhao Jiang
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////


#ifndef __AMP_BUF_DESC_H__
#define __AMP_BUF_DESC_H__

/************* Header file include ********************************************/
#include "amp_types.h"
#include "OSAL_api.h"
#include "isl/amp_shm.h"
#include "isl/amp_bd_tag.h"

/************* Macros, type definitions****************************************/

/**\def     AMP_BD_MAX_SIZE
 * \brief   Description for macro AMP_BD_MAX_SIZE
 */
#define AMP_BD_MAX_SIZE         (0x1000)

/**\def     AMP_BD_NAME_LEN
 * \brief   Description for macro AMP_BD_NAME_LEN
 */
#define AMP_BD_NAME_LEN         (8)

/**\def     AMP_BD_SHM_ALIGN
 * \brief   Description for macro AMP_BD_SHM_ALIGN
 */
#define AMP_BD_SHM_ALIGN        (32)

/**\def     AMP_BD_SET_ALLOC_VA(hBD, VA)
 * \brief   Description for macro AMP_BD_SET_ALLOC_VA(hBD, VA)
 */
#define AMP_BD_SET_ALLOC_VA(hBD, VA)    \
        ((AMP_BD_HANDLE)(hBD))->uiAllocVA = (unsigned long)(VA)

/**\def     AMP_BD_GET_ALLOC_VA(hBD)
 * \brief   Description for macro AMP_BD_GET_ALLOC_VA(hBD)
 */
#define AMP_BD_GET_ALLOC_VA(hBD)       (unsigned long) (((AMP_BD_HANDLE)hBD)->uiAllocVA)

/**\def     AMP_BD_SET_BDID(hBD, uiBDId)
 * \brief   Description for macro AMP_BD_SET_BDID(hBD, uiBDId)
 */
#define AMP_BD_SET_BDID(hBD, BDId)    \
        ((AMP_BD_HANDLE)(hBD))->uiBDId = (UINT32)(BDId)

/**\def     AMP_BD_GET_BDID(hBD)
 * \brief   Description for macro AMP_BD_GET_BDID(hBD)
 */
#define AMP_BD_GET_BDID(hBD)            (((AMP_BD_HANDLE)hBD)->uiBDId)

#define AMPCHAIN_MAX_CACHE_NODE_NUM         (256)

#define BITS_OF(t)                  (8*sizeof(t))


/** \typedef AMP_BD_HANDLE;
 *  \brief Define AMP buffer descriptor handle as AMP_BD_ST type.
 */
typedef AMP_BD_ST   *AMP_BD_HANDLE;

/** \typedef AMP_BD_ID;
 *  \brief Define AMP BD unique ID as UINT32(SHM Offset), the ID(SHM Offset) can
 *         be transferred over the processes.
 */
typedef UINT32   AMP_BD_ID;

/** \struct _amp_bd_node_
 *  \brief The BD node structure is used to link all BD as chain for easy
 *         management.
 */
struct _amp_bd_node_ {
    struct _amp_bd_node_  *pPrev;    /**< Point to the previous node. */
    struct _amp_bd_node_  *pNext;    /**< Point to the next node. */
    AMP_BD_HANDLE       hBD;         /**< Current BD node. */
};

/** \typedef AMP_BDNODE
 *  \brief The BD node structure is used to link all BD as chain for easy
 *         management.
 */
typedef struct _amp_bd_node_  AMP_BDNODE;

/** \struct AMP_BDCHAIN
 *  \brief Buffer descriptor chain structure for eay management.
 */
typedef struct {
    MV_OSAL_HANDLE_MUTEX_t  hChainLock; /**< Multi thread protection. */
    UINT32                  uiTotalNum; /**< The BD number in this chain. */
    struct _amp_bd_node_    *pHead;     /**< Point to the head of the chain. */
    struct _amp_bd_node_    *pTail;     /**< Point to the tail of the chain. */
    UINT32  uiCacheTable[((AMPCHAIN_MAX_CACHE_NODE_NUM-1)/BITS_OF(UINT32))+1];
                                        /**< Indicate cache status in chain. */
} AMP_BDCHAIN;


#ifdef __cplusplus
extern "C" {
#endif

/************* Declaration of Transport Buffer Descriptor Function ************/

/** \fn HRESULT AMP_BD_Allocate( AMP_OUT AMP_BD_HANDLE   *phBD)
 *
 *  \brief This function used to create a new buffer descriptor.
 *
 *         The AMP_BD_MAX_SIZE size share memory will be allocated for exchange
 *         data. Please ref to AMP_SHM_Allocate() for detail.
 *
 *         Ref Cnt is introduced to maintian BD being accessed cross process.
 *
 *  \param phBD Return with buffer descriptor handle.
 *
 *  \retval SUCCESS Allocate succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    Allocate failed
 *
 *  \sa AMP_BD_HANDLE
 *  \sa AMP_BD_Free
 *  \sa AMP_BD_Map
 *  \sa AMP_BD_Unmap
 *  \sa AMP_BD_Ref
 *  \sa AMP_BD_Unref
 *  \sa AMP_BD_GetStatus
 */
HRESULT AMP_BD_Allocate(
        AMP_OUT      AMP_BD_HANDLE   *phBD);

/** \fn HRESULT AMP_BD_Free(AMP_IN AMP_BD_HANDLE   hBD);
 *
 *  \brief This function used to free a buffer descriptor.
 *
 *         AMP_BD_Free() is paired with AMP_BD_Allocate().
 *
 *         The parameter \c hBD will be strictly checked before the buffer
 *         descriptor is freed. ERR_NOSHM will be returned to indicate that
 *         the BD doesn't exist any more, it may be freed before.
 *
 *         A ref count is introduced to maintian BD alloc/free. The count will
 *         be decreased if API i called. Only when the count is 0, the BD will
 *         be freed.
 *
 *  \param hBD the share memory handle to be freed.
 *
 *  \retval SUCCESS Free succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    the free failed
 *
 *  \sa AMP_BD_HANDLE
 *  \sa AMP_BD_Allocate
 *  \sa AMP_BD_Map
 *  \sa AMP_BD_Unmap
 *  \sa AMP_BD_Ref
 *  \sa AMP_BD_Unref
 *  \sa AMP_BD_GetStatus
 */
HRESULT AMP_BD_Free(
        AMP_IN       AMP_BD_HANDLE   hBD);

/** \fn HRESULT AMP_BD_Map(
 *                  AMP_IN  AMP_BD_ID       uiBDId,
 *                  AMP_OUT  AMP_BD_HANDLE   *phBD)
 *
 *  \brief This function used to map a BD with BD ID(the SHM offset).
 *
 *         BDId is required to map out the local hBD. It usually will be called
 *         when BD is transfered from one process to another.
 *
 *  \param uiBDId Identifying the BD with the SHM offset.
 *  \param phBD Return the handle of the buffer descriptor
 *
 *  \retval SUCCESS Map succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    the map failed
 *
 *  \sa AMP_BD_HANDLE
 *  \sa AMP_BD_Allocate
 *  \sa AMP_BD_Map
 *  \sa AMP_BD_Unmap
 *  \sa AMP_BD_Ref
 *  \sa AMP_BD_Unref
 *  \sa AMP_BD_GetStatus
 */
HRESULT AMP_BD_Map(
        AMP_IN       AMP_BD_ID       uiBDId,
        AMP_OUT      AMP_BD_HANDLE   *phBD);

/** \fn HRESULT AMP_BD_Unmap(
 *                  AMP_IN  AMP_BD_HANDLE   hBD);
 *
 *  \brief This function used to unmap BD.
 *
 *         AMP_BD_Unmap() is paired with AMP_BD_Map().
 *
 *         The parameter \c hBD will be strictly checked before the buffer
 *         descriptor is freed. ERR_NOSHM will be returned to indicate that
 *         the BD doesn't exist any more, it may be freed before.
 *
 *  \param hBD Return the handle of the buffer descriptor
 *
 *  \retval SUCCESS Unmap succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM uiBDId is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    Unmap failed
 *
 *  \sa AMP_BD_HANDLE
 *  \sa AMP_BD_Allocate
 *  \sa AMP_BD_Map
 *  \sa AMP_BD_Unmap
 *  \sa AMP_BD_Ref
 *  \sa AMP_BD_Unref
 *  \sa AMP_BD_GetStatus
 */
HRESULT AMP_BD_Unmap(AMP_IN AMP_BD_HANDLE   hBD);

/** \fn HRESULT AMP_BD_Ref(
 *                  AMP_IN  AMP_BD_HANDLE   hBD);
 *
 *  \brief This function used to add a reference of buffer descriptor.
 *
 *         The parameter \c hBD will be strictly checked before the buffer
 *         descriptor is freed. ERR_NOSHM will be returned to indicate that
 *         the BD doesn't exist any more, it may be freed before.
 *
 *         When one moudle recieve one BD from other moudle, Ref it before use
 *         it. The ref count will be increased once the API called.
 *
 *  \param hBD Indicating the handle of the buffer descriptor
 *
 *  \retval SUCCESS Ref succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    Ref failed
 *
 *  \sa AMP_BD_HANDLE
 *  \sa AMP_BD_Allocate
 *  \sa AMP_BD_Map
 *  \sa AMP_BD_Unmap
 *  \sa AMP_BD_Ref
 *  \sa AMP_BD_Unref
 *  \sa AMP_BD_GetStatus
 */
HRESULT AMP_BD_Ref(
               AMP_IN AMP_BD_HANDLE   hBD);

/** \fn HRESULT AMP_BD_Unref(AMP_IN AMP_BD_HANDLE   hBD);
 *
 *  \brief This function used to remove a reference of buffer descriptor.
 *
 *         AMP_BD_Unref() is paired with AMP_BD_Ref().
 *
 *         The parameter \c hBD will be strictly checked before the buffer
 *         descriptor is freed. ERR_NOSHM will be returned to indicate that
 *         the BD doesn't exist any more, it may be freed before.
 *
 *         When one moudle should Unfef the BD that won't be used any more.
 *         The ref count will be increased once the API called.
 *
 *  \param hBD Indicating the handle of the buffer descriptor.
 *
 *  \retval SUCCESS Unref succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    Unref failed
 *
 *  \sa AMP_BD_HANDLE
 *  \sa AMP_BD_Allocate
 *  \sa AMP_BD_Map
 *  \sa AMP_BD_Unmap
 *  \sa AMP_BD_Ref
 *  \sa AMP_BD_Unref
 *  \sa AMP_BD_GetStatus
 */
HRESULT AMP_BD_Unref(
        AMP_IN       AMP_BD_HANDLE   hBD);

/** \fn HRESULT AMP_BD_GetStatus(
 *       AMP_IN      AMP_BD_HANDLE   hBD,
 *       AMP_OUT     BOOL *pValid,
 *       AMP_OUT     UINT32 *pRefCnt);
 *
 *  \brief This function used to get the status by SHM handle.
 *
 *         The parameter \c hBD will be strictly checked before the buffer
 *         descriptor is freed. ERR_NOSHM will be returned to indicate that
 *         the BD doesn't exist any more, it may be freed before.
 *
 *         It's a help function for user to get the BD status.
 *
 *  \param hBD the BD handle.
 *  \param pValid Return TRUE if hBD handle is valid one, otherwise FALSE.
 *  \param pRefCnt Return the ref count of BD.
 *
 *  \retval SUCCESS Unref succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    Unref failed
 *
 *  \sa AMP_BD_HANDLE
 *  \sa AMP_BD_Allocate
 *  \sa AMP_BD_Map
 *  \sa AMP_BD_Unmap
 *  \sa AMP_BD_Ref
 *  \sa AMP_BD_Unref
 *  \sa AMP_BD_GetStatus
 */
HRESULT AMP_BD_GetStatus(
        AMP_IN      AMP_BD_HANDLE   hBD,
        AMP_OUT     BOOL *pValid,
        AMP_OUT     UINT32 *pRefCnt);


/** \fn HRESULT AMP_BD_SetName(
 *              AMP_IN      AMP_BD_HANDLE   hBD,
 *              AMP_IN      CHAR *strName);
 *
 *  \brief This function used to associate a name to BD.
 *
 *         The parameter \c hBD will be strictly checked before the buffer
 *         descriptor is freed. ERR_NOSHM will be returned to indicate that
 *         the BD doesn't exist any more, it may be freed before.
 *
 *         if user sets a name with more than 7 characters, only 7 characters will be saved.
 *         if user doesn't set a name to a BD, a defaut name "DEF_BD" will be assign.
 *
 *         The name associated to BD will easier indicate who own the BD.
 *         It will be more convenient for BD debug.
 *
 *  \param hBD the BD handle.
 *  \param strName the string of name, note that if the string is longger than 7 characters,
 *                            we can get the first 7 characters ad its name.
 *
 *  \retval SUCCESS Unref succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    Unref failed
 *
 *  \sa AMP_BD_HANDLE
 *  \sa AMP_BD_Allocate
 *  \sa AMP_BD_Map
 *  \sa AMP_BD_Unmap
 *  \sa AMP_BD_Ref
 *  \sa AMP_BD_Unref
 *  \sa AMP_BD_GetStatus
 */
HRESULT AMP_BD_SetName(
        AMP_IN      AMP_BD_HANDLE   hBD,
        AMP_IN      CHAR *strName);

/** \fn HRESULT AMP_BD_GetName(
 *              AMP_IN      AMP_BD_HANDLE   hBD,
 *              AMP_OUT   CHAR *strName);
 *
 *  \brief This function used to associate a name to BD.
 *
 *         The parameter \c hBD will be strictly checked before the buffer
 *         descriptor is freed. ERR_NOSHM will be returned to indicate that
 *         the BD doesn't exist any more, it may be freed before.
 *
 *         if user doesn't set a name to a BD, a defaut name "DEF_BD" will be assign.
 *
 *         The name associated to BD will easier indicate who own the BD.
 *         It will be more convenient for BD debug.
 *
 *  \param hBD the BD handle.
 *  \param strName return the string of name.
 *
 *  \retval SUCCESS Unref succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    Unref failed
 *
 *  \sa AMP_BD_HANDLE
 *  \sa AMP_BD_Allocate
 *  \sa AMP_BD_Map
 *  \sa AMP_BD_Unmap
 *  \sa AMP_BD_Ref
 *  \sa AMP_BD_Unref
 *  \sa AMP_BD_GetStatus
 */
HRESULT AMP_BD_GetName(
        AMP_IN      AMP_BD_HANDLE   hBD,
        AMP_OUT     CHAR *strName);

/** \fn HRESULT AMP_BD_Dump(
 *              AMP_IN   AMP_BD_HANDLE   hBD);
 *
 *  \brief This function used to dump BD and BDTag detail information.
 *  only for temp debug, DONOT keep permanent code to calling this function.
 *
 *  \param hBD the BD handle.
 *
 *  \retval SUCCESS
 */
HRESULT AMP_BD_Dump(
        AMP_IN      AMP_BD_HANDLE   hBD);

/** \fn HRESULT AMP_BD_Share(
 *                  AMP_IN   AMP_BD_HANDLE   hBD,
 *                  AMP_OUT  int             *fd)
 *
 *  \brief This function used to share a BD with fd
 *
 *  \param hBD The BD handle.
 *  \param fd  The file descriptor that can be used with open/mmap etc
 *             call close(fd) when it won't be used anymore.
 *
 *  \retval SUCCESS Map succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD or fd is an invalid one.
 *
 *  \sa AMP_BD_HANDLE
 */
HRESULT AMP_BD_Share(
        AMP_IN       AMP_BD_HANDLE   hBD,
        AMP_OUT      int             *fd);


/************* Declaration of Buffer Tag Function ****************************/

/** \fn HRESULT AMP_BDTAG_Append(
 *       AMP_IN      AMP_BD_HANDLE   hBD,
 *       AMP_IN      UINT8           *pData,
 *       AMP_OUT     UINT32          *pIndex,
 *       AMP_OUT     UINT32          *pLeftBytes);
 *
 *  \brief This function used to insert buffer descriptor tag.
 *
 *         The parameter \c hBD will be strictly checked before the buffer
 *         descriptor is freed. ERR_NOSHM will be returned to indicate that
 *         the BD doesn't exist any more, it may be freed before.
 *
 *         Please ref to amp_bd_tag.h for all BD tag definition.
 *         \c pIndex and \c pLeftBytes can be NULL, if user don't care
 *         the return value.
 *
 *  \param hBD The handle of buffer descriptor.
 *  \param pData The data will be copyed to descriptor space.
 *  \param pIndex Return the index of descriptor buffer in this descriptor.
 *  \param pLeftBytes Return the left bytes of descriptor buffer.
 *
 *  \retval SUCCESS Append succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    Append failed
 *  \retval MODULE_MEM|ERR_TAG  Error Tag
 *
 *  \sa AMP_BD_HANDLE
 *  \sa AMP_BDTAG_Append
 *  \sa AMP_BDTAG_Update
 *  \sa AMP_BDTAG_DelTail
 *  \sa AMP_BDTAG_GetNum
 *  \sa AMP_BDTAG_Clear
 *  \sa AMP_BDTAG_GetWithIndex
 *  \sa AMP_BDTAG_GetSpareBytes
 */
HRESULT AMP_BDTAG_Append(
        AMP_IN      AMP_BD_HANDLE   hBD,
        AMP_IN      UINT8           *pData,
        AMP_OUT     UINT32          *pIndex,
        AMP_OUT     UINT32          *pLeftBytes);

/** \fn AMP_BDTAG_Update(
 *       AMP_IN      AMP_BD_HANDLE   hBD,
 *       AMP_IN      UINT32          Index,
 *       AMP_IN      UINT8           *pData);
 *
 *  \brief This function used to update the content of tag with index that have
 *         been appended before.
 *
 *         The parameter \c hBD will be strictly checked before the buffer
 *         descriptor is freed. ERR_NOSHM will be returned to indicate that
 *         the BD doesn't exist any more, it may be freed before.
 *
 *  \param hBD The handle of buffer descriptor.
 *  \param Index The place that data will be updated in descriptor space.
 *  \param pData The data point of data.
 *
 *  \retval SUCCESS Update succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD is an invalid one.
 *  \retval MODULE_MEM|ERR_TAG    Unref failed
 *
 *  \sa AMP_BD_HANDLE
 *  \sa AMP_BDTAG_Append
 *  \sa AMP_BDTAG_Update
 *  \sa AMP_BDTAG_DelTail
 *  \sa AMP_BDTAG_GetNum
 *  \sa AMP_BDTAG_Clear
 *  \sa AMP_BDTAG_GetWithIndex
 *  \sa AMP_BDTAG_GetSpareBytes
 */
HRESULT AMP_BDTAG_Update(
        AMP_IN      AMP_BD_HANDLE   hBD,
        AMP_IN      UINT32          Index,
        AMP_IN      UINT8           *pData);

/** \fn HRESULT AMP_BDTAG_DelTail(
 *       AMP_IN      AMP_BD_HANDLE   hBD);
 *
 *  \brief This function used to delete the tail tag of BD.
 *
 *         The parameter \c hBD will be strictly checked before the buffer
 *         descriptor is freed. ERR_NOSHM will be returned to indicate that
 *         the BD doesn't exist any more, it may be freed before.
 *
 *  \param hBD The handle of buffer descriptor.
 *
 *  \retval SUCCESS Update succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD is an invalid one.
 *  \retval MODULE_MEM|ERR_TAG    Unref failed
 *
 *  \sa AMP_BD_HANDLE
 *  \sa AMP_BDTAG_Append
 *  \sa AMP_BDTAG_Update
 *  \sa AMP_BDTAG_DelTail
 *  \sa AMP_BDTAG_GetNum
 *  \sa AMP_BDTAG_Clear
 *  \sa AMP_BDTAG_GetWithIndex
 *  \sa AMP_BDTAG_GetSpareBytes
 */
HRESULT AMP_BDTAG_DelTail(
        AMP_IN      AMP_BD_HANDLE   hBD);

/** \fn HRESULT AMP_BDTAG_GetNum(
 *       AMP_IN      AMP_BD_HANDLE   hBD,
 *       AMP_OUT     UINT32          *pNum);
 *
 *  \brief This function used to get the tag number of BD.
 *
 *         The parameter \c hBD will be strictly checked before the buffer
 *         descriptor is freed. ERR_NOSHM will be returned to indicate that
 *         the BD doesn't exist any more, it may be freed before.
 *
 *  \param hBD The handle of buffer descriptor.
 *  \param pNum The tag num of BD.
 *
 *  \retval SUCCESS Update succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD is an invalid one.
 *  \retval MODULE_MEM|ERR_TAG    Unref failed
 *
 *  \sa AMP_BD_HANDLE
 *  \sa AMP_BDTAG_Append
 *  \sa AMP_BDTAG_Update
 *  \sa AMP_BDTAG_DelTail
 *  \sa AMP_BDTAG_GetNum
 *  \sa AMP_BDTAG_Clear
 *  \sa AMP_BDTAG_GetWithIndex
 *  \sa AMP_BDTAG_GetSpareBytes
 */
HRESULT AMP_BDTAG_GetNum(
        AMP_IN      AMP_BD_HANDLE   hBD,
        AMP_OUT     UINT32          *pNum);


/** \fn AMP_BDTAG_Clear(
 *       AMP_IN      AMP_BD_HANDLE   hBD);
 *
 *  \brief This function used to clear the buffer of descriptor tag to let it
 *         can be reused.
 *
 *         The parameter \c hBD will be strictly checked before the buffer
 *         descriptor is freed. ERR_NOSHM will be returned to indicate that
 *         the BD doesn't exist any more, it may be freed before.
 *
 *  \param hBD The handle of buffer descriptor.
 *
 *  \retval SUCCESS Clear succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD is an invalid one.
 *  \retval MODULE_MEM|ERR_TAG    Clear failed
 *
 *  \sa AMP_BD_HANDLE
 *  \sa AMP_BDTAG_Append
 *  \sa AMP_BDTAG_Update
 *  \sa AMP_BDTAG_DelTail
 *  \sa AMP_BDTAG_GetNum
 *  \sa AMP_BDTAG_Clear
 *  \sa AMP_BDTAG_GetWithIndex
 *  \sa AMP_BDTAG_GetSpareBytes
 */
HRESULT AMP_BDTAG_Clear(
        AMP_IN      AMP_BD_HANDLE   hBD);

/** \fn HRESULT AMP_BDTAG_GetWithIndex(
 *       AMP_IN      AMP_BD_HANDLE   hBD,
 *       AMP_IN      UINT32          Index,
 *       AMP_OUT     void            **pData);
 *
 *  \brief This function used to get the data pointer with tag index.
 *
 *         The parameter \c hBD will be strictly checked before the buffer
 *         descriptor is freed. ERR_NOSHM will be returned to indicate that
 *         the BD doesn't exist any more, it may be freed before.
 *
 *  \param hBD The handle of buffer descriptor.
 *  \param Index The index in descriptor buffer.
 *  \param pData Return the data pointer of the BD tag.
 *
 *  \retval SUCCESS GetWithIndex succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD is an invalid one.
 *  \retval MODULE_MEM|ERR_TAG    GetWithIndex failed
 *
 *  \sa AMP_BD_HANDLE
 *  \sa AMP_BDTAG_Append
 *  \sa AMP_BDTAG_Update
 *  \sa AMP_BDTAG_DelTail
 *  \sa AMP_BDTAG_GetNum
 *  \sa AMP_BDTAG_Clear
 *  \sa AMP_BDTAG_GetWithIndex
 *  \sa AMP_BDTAG_GetSpareBytes
 */
HRESULT AMP_BDTAG_GetWithIndex(
        AMP_IN      AMP_BD_HANDLE   hBD,
        AMP_IN      UINT32          Index,
        AMP_OUT     void            **pData);

/** \fn HRESULT AMP_BDTAG_GetWithType(
 *       AMP_IN      AMP_BD_HANDLE   hBD,
 *       AMP_IN      AMP_BDTAG_T     Type,
 *       AMP_INOUT   UINT32           *pIndex,
 *       AMP_OUT     void            **pData);
 *
 *  \brief This function used to get the pointer to the tag with specified type.
 *
 *         The parameter \c hBD will be strictly checked before the buffer
 *         descriptor is freed. ERR_NOSHM will be returned to indicate that
 *         the BD doesn't exist any more, it may be freed before.
 *
 *  \param hBD The handle of buffer descriptor.
 *  \param Type The type of the desired BD tag.
 *  \param pIndex Specify the index to start the search
 *                and return the index of the tag if found.
 *  \param pData Return the data pointer of the BD tag.
 *
 *  \retval SUCCESS GetWithType succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD is an invalid one.
 *  \retval MODULE_MEM|ERR_TAG    GetWithType failed
 *
 *  \sa AMP_BD_HANDLE
 *  \sa AMP_BDTAG_Append
 *  \sa AMP_BDTAG_Update
 *  \sa AMP_BDTAG_DelTail
 *  \sa AMP_BDTAG_GetNum
 *  \sa AMP_BDTAG_Clear
 *  \sa AMP_BDTAG_GetWithIndex
 *  \sa AMP_BDTAG_GetSpareBytes
 */
HRESULT AMP_BDTAG_GetWithType(
        AMP_IN      AMP_BD_HANDLE   hBD,
        AMP_IN      AMP_BDTAG_T     Type,
        AMP_INOUT   UINT32          *pIndex,
        AMP_OUT     void            **pData);

/** \fn HRESULT AMP_BDTAG_GetSpareBytes(
 *       AMP_IN      AMP_BD_HANDLE   hBD,
 *       AMP_OUT     UINT32          *pSpareBytes);
 *
 *  \brief This function used to get the spare bytes of BD tag space.
 *
 *         The parameter \c hBD will be strictly checked before the buffer
 *         descriptor is freed. ERR_NOSHM will be returned to indicate that
 *         the BD doesn't exist any more, it may be freed before.
 *
 *         If /c Index is out of the range, ERR_TAG wil be return. Only when
 *         return SUCCESS, caller can access pData, otherwise should .
 *
 *  \param hBD The handle of buffer descriptor.
 *  \param pSpareBytes Return the spare bytes of descriptor buffer.
 *
 *  \retval SUCCESS GetWithIndex succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD is an invalid one.
 *  \retval MODULE_MEM|ERR_TAG    GetWithIndex failed
 *
 *  \sa AMP_BD_HANDLE
 *  \sa AMP_BDTAG_Append
 *  \sa AMP_BDTAG_Update
 *  \sa AMP_BDTAG_DelTail
 *  \sa AMP_BDTAG_GetNum
 *  \sa AMP_BDTAG_Clear
 *  \sa AMP_BDTAG_GetWithIndex
 *  \sa AMP_BDTAG_GetSpareBytes
 */
HRESULT AMP_BDTAG_GetSpareBytes(
        AMP_IN      AMP_BD_HANDLE   hBD,
        AMP_OUT     UINT32          *pSpareBytes);


/************* Declaration of Buffer Descriptor Chain ****************************/

/** \fn HRESULT AMP_BDCHAIN_Create(
 *       AMP_IN      BOOL            bWithLock,
 *       AMP_OUT     AMP_BDCHAIN     **ppChain);
 *
 *  \brief This function used to create a BD chain for easy management.
 *
 *  \param bWithLock Indicates that whether the chain need a lock to do
 *         protection cross threads.
 *  \param ppChain Return the handle of BD chain.
 *
 *  \retval SUCCESS BDCHAIN_Create succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM hBD is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    BDCHAIN_Create failed
 *
 *  \sa AMP_BDCHAIN
 *  \sa AMP_BDCHAIN_PushItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_DeleteItem
 *  \sa AMP_BDCHAIN_NextNode
 *  \sa AMP_BDCHAIN_PrevNode
 *  \sa AMP_BDCHAIN_PeekItem
 *  \sa AMP_BDCHAIN_Destroy
 */
HRESULT AMP_BDCHAIN_Create(
        AMP_IN      BOOL            bWithLock,
        AMP_OUT     AMP_BDCHAIN     **ppChain);

/** \fn HRESULT AMP_BDCHAIN_PushItem(
 *      AMP_IN      AMP_BDCHAIN     *pChain,
 *      AMP_IN      AMP_BD_HANDLE   hBD);
 *
 *  \brief This function is used to push a BD node to the bottom of the chain.
 *
 *  \param pChain The descriptor chain.
 *  \param hBD The BD node that is pushed to chain.
 *
 *  \retval SUCCESS PushItem succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM pChain is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    PushItem failed
 *
 *  \sa AMP_BDCHAIN
 *  \sa AMP_BDCHAIN_PushItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_DeleteItem
 *  \sa AMP_BDCHAIN_NextNode
 *  \sa AMP_BDCHAIN_PrevNode
 *  \sa AMP_BDCHAIN_PeekItem
 *  \sa AMP_BDCHAIN_Destroy
 */
HRESULT AMP_BDCHAIN_PushItem(
        AMP_IN      AMP_BDCHAIN     *pChain,
        AMP_IN      AMP_BD_HANDLE   hBD);

/** \fn HRESULT AMP_BDCHAIN_PopItem(
 *      AMP_IN      AMP_BDCHAIN     *pChain,
 *      AMP_OUT     AMP_BD_HANDLE   *phBD);
 *
 *  \brief This function is used to pop the BD node from the top of chain,
 *         and move pHead to next node.
 *
 *  \param pChain The descriptor chain.
 *  \param phBD Return the pointer of the BD item.
 *
 *  \retval SUCCESS PopItem succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM pChain is an invalid one.
 *  \retval MODULE_MEM|ERR_NODE    PopItem failed
 *
 *  \sa AMP_BDCHAIN
 *  \sa AMP_BDCHAIN_PushItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_DeleteItem
 *  \sa AMP_BDCHAIN_NextNode
 *  \sa AMP_BDCHAIN_PrevNode
 *  \sa AMP_BDCHAIN_PeekItem
 *  \sa AMP_BDCHAIN_Destroy
 */
HRESULT AMP_BDCHAIN_PopItem(
        AMP_IN      AMP_BDCHAIN     *pChain,
        AMP_OUT     AMP_BD_HANDLE   *phBD);

/** \fn HRESULT AMP_BDCHAIN_DeleteItem(
 *      AMP_IN      AMP_BDCHAIN     *pChain,
 *      AMP_IN      AMP_BD_HANDLE   hBD);
 *
 *  \brief This function is used to delete the BD node.
 *
 *  \param pChain The descriptor chain.
 *  \param hBD Return the pointer of the BD item.
 *
 *  \retval SUCCESS DeleteItem succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM pChain is an invalid one.
 *  \retval MODULE_MEM|ERR_NODE    DeleteItem failed
 *
 *  \sa AMP_BDCHAIN
 *  \sa AMP_BDCHAIN_PushItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_DeleteItem
 *  \sa AMP_BDCHAIN_NextNode
 *  \sa AMP_BDCHAIN_PrevNode
 *  \sa AMP_BDCHAIN_PeekItem
 *  \sa AMP_BDCHAIN_Destroy
 */
HRESULT AMP_BDCHAIN_DeleteItem(
        AMP_IN      AMP_BDCHAIN     *pChain,
        AMP_IN      AMP_BD_HANDLE   hBD);

/** \fn HRESULT AMP_BDCHAIN_NextNode(
 *       AMP_IN      AMP_BDCHAIN     *pChain,
 *       AMP_INOUT   AMP_BDNODE      **ppNextBDNode);
 *
 *  \brief This function is used to get the next BD node of the input node.
 *         If the input node is null, return the first node.
 *
 *  \param pChain The descriptor chain.
 *  \param ppNextBDNode Return the pointer of the next BD node.
 *
 *  \retval SUCCESS NextNode succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM pChain is an invalid one.
 *  \retval MODULE_MEM|ERR_NODE    NextNode failed
 *
 *  \sa AMP_BDCHAIN
 *  \sa AMP_BDCHAIN_PushItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_DeleteItem
 *  \sa AMP_BDCHAIN_NextNode
 *  \sa AMP_BDCHAIN_PrevNode
 *  \sa AMP_BDCHAIN_PeekItem
 *  \sa AMP_BDCHAIN_Destroy
 */
HRESULT AMP_BDCHAIN_NextNode(
        AMP_IN      AMP_BDCHAIN     *pChain,
        AMP_INOUT   AMP_BDNODE    **ppNextBDNode);

/** \fn HRESULT AMP_BDCHAIN_PrevNode(
 *       AMP_IN      AMP_BDCHAIN     *pChain,
 *       AMP_INOUT   AMP_BDNODE      **ppPrevBDNode);
 *
 *  \brief This function is used to get the previous BD node of the input node.
 *         If the input node is null, return the last node.
 *
 *  \param pChain The descriptor chain.
 *  \param ppPrevBDNode Return the pointer of the previous BD node.
 *
 *  \retval SUCCESS PrevNode succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM pChain is an invalid one.
 *  \retval MODULE_MEM|ERR_NODE    PrevNode failed
 *
 *  \sa AMP_BDCHAIN
 *  \sa AMP_BDCHAIN_PushItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_DeleteItem
 *  \sa AMP_BDCHAIN_NextNode
 *  \sa AMP_BDCHAIN_PrevNode
 *  \sa AMP_BDCHAIN_PeekItem
 *  \sa AMP_BDCHAIN_Destroy
 */
HRESULT AMP_BDCHAIN_PrevNode(
        AMP_IN      AMP_BDCHAIN     *pChain,
        AMP_INOUT   AMP_BDNODE      **ppPrevBDNode);

/** \fn HRESULT AMP_BDCHAIN_PeekItem(
 *      AMP_IN      AMP_BDCHAIN     *pChain,
 *      AMP_IN      UINT32          uiIndex,
 *      AMP_OUT     AMP_BD_HANDLE   *phBD);
 *
 *  \brief This function is used to get the BD node from the chain with index,
 *         but not move pHead to next node.
 *
 *  \param pChain The descriptor chain.
 *  \param uiIndex Indicate the index of BD Node.
 *  \param phBD Return the pointer of the BD item.
 *
 *  \retval SUCCESS PeekItem succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM pChain is an invalid one.
 *  \retval MODULE_MEM|ERR_NODE    PeekItem failed
 *
 *  \sa AMP_BDCHAIN
 *  \sa AMP_BDCHAIN_PushItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_DeleteItem
 *  \sa AMP_BDCHAIN_NextNode
 *  \sa AMP_BDCHAIN_PrevNode
 *  \sa AMP_BDCHAIN_PeekItem
 *  \sa AMP_BDCHAIN_Destroy
 */
HRESULT AMP_BDCHAIN_PeekItem(
        AMP_IN      AMP_BDCHAIN     *pChain,
        AMP_IN      UINT32          uiIndex,
        AMP_OUT     AMP_BD_HANDLE   *phBD);

/** \fn HRESULT AMP_BDCHAIN_GetItemNum(
 *      AMP_IN      AMP_BDCHAIN     *pChain,
 *      AMP_OUT     UINT32          *pNum);
 *
 *  \brief This function is used to get the item number of the chain,
 *         but not move pHead to next node.
 *
 *  \param pChain The descriptor chain.
 *  \param pNum Return the pointer of the BD item.
 *
 *  \retval SUCCESS GetItemNum succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM pChain is an invalid one.
 *  \retval MODULE_MEM|ERR_NODE    GetItemNum failed
 *
 *  \sa AMP_BDCHAIN
 *  \sa AMP_BDCHAIN_PushItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_DeleteItem
 *  \sa AMP_BDCHAIN_NextNode
 *  \sa AMP_BDCHAIN_PrevNode
 *  \sa AMP_BDCHAIN_PeekItem
 *  \sa AMP_BDCHAIN_Destroy
 */
HRESULT AMP_BDCHAIN_GetItemNum(
        AMP_IN      AMP_BDCHAIN     *pChain,
        AMP_OUT     UINT32          *pNum);

/** \fn AMP_BDCHAIN_Destroy(
 *       AMP_IN      AMP_BDCHAIN   *pChain);
 *
 *  \brief This function used to remove a buffer descriptor chain.
 *
 *  \param pChain The descriptor chain to be removed.
 *
 *  \retval SUCCESS Destroy succeeded
 *  \retval MODULE_MEM|ERR_ERRPARAM pChain is an invalid one.
 *  \retval MODULE_MEM|ERR_NOSHM    Destroy failed
 *
 *  \sa AMP_BDCHAIN
 *  \sa AMP_BDCHAIN_PushItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_PopItem
 *  \sa AMP_BDCHAIN_DeleteItem
 *  \sa AMP_BDCHAIN_NextNode
 *  \sa AMP_BDCHAIN_PrevNode
 *  \sa AMP_BDCHAIN_PeekItem
 *  \sa AMP_BDCHAIN_Destroy
 */
HRESULT AMP_BDCHAIN_Destroy(
        AMP_IN      AMP_BDCHAIN   *pChain);

#ifdef __cplusplus
}
#endif //__cplusplus
#endif
