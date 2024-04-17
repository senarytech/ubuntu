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

///////////////////////////////////////////////////////////////////////////////
//! \file		std_list.h
//! \brief 
//! \author		Cheng Du
//! \version	0.1
//! \date		July 2007
//! \Histroy	4/23/2008	Fang Bao	Add std_list_search()
///////////////////////////////////////////////////////////////////////////////

#ifndef _STD_LIST_H_
#define _STD_LIST_H_

#include "com_type.h"
#include "ErrorCode.h"

#ifdef __cplusplus
extern "C" {
#endif
//! \typedef std_list_node
typedef struct _std_list_node
{
	PVOID pData;
	struct _std_list_node* pPrev;
	struct _std_list_node* pNext;
} std_list_node;

typedef PVOID (*std_list_alloc_func)(UINT uiSize);
typedef HRESULT (*std_list_data_dtor_func)(PVOID pData);

//! \typedef std_list
typedef struct _std_list
{
	std_list_node* m_pHead;
	std_list_node* m_pTail;
	UINT m_uiSize;
	std_list_data_dtor_func m_data_dtor;
} std_list;

///////////////////////////////////////////////////////////////////////////////
//! \brief		Constructs a list. If the list is used to store data within
//!				the size of PVOID, set data_dtor to NULL, otherwise set a function
//!				pointer to data_dtor which will be used to destruct the data later.
//!
//! \param[in]	self		The pointer to the list.
//!							If it is NULL, construct a new list,
//!							otherwise initialize and return it.
//!	\param[in]	data_dtor	A function pointer to destructor the data
//!
//!	\return		The pointer to the list
///////////////////////////////////////////////////////////////////////////////

std_list* std_list_ctor(std_list* self, std_list_data_dtor_func data_dtor);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Destructs a list. This calls m_data_dtor to destruct 
//!				data in all nodes. It deallocates the storage allocated
//!				for the nodes in the list and the list itself.
//!
//! \param[in]	self	The pointer to the list.
///////////////////////////////////////////////////////////////////////////////

VOID std_list_dtor(std_list* self);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Erases all the nodes of a list. This calls m_data_dtor to destruct 
//!				data in all nodes. It deallocates the storage allocated for 
//!				the nodes in the list.
//!
//! \param[in]	self	The pointer to the list
//!
//!	\return		S_OK	All the nodes are erased.
//!	\return		E_FAIL	Failed otherwise
//!
//!	\see		std_list_clear
///////////////////////////////////////////////////////////////////////////////

HRESULT std_list_clear(std_list* self);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Deletes the node at the end of a list and return the data
//!				This calls the deleted node's destructor and effectively 
//!				reduces the list size by one.
//!
//! \param[in]	self	The pointer to the list
//! \param[out] ppData	The pointer to the data
//!
//!	\return		S_OK if the data is successfully retrieved
//!	\return		E_FAIL if he list does not contain any node
//!	\return		E_POINTER if self is NULL
///////////////////////////////////////////////////////////////////////////////

HRESULT std_list_pop_back(std_list* self, PVOID* ppData);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Deletes the node at the beginning of a list and return the data
//!				This calls the deleted node's destructor and effectively 
//!				reduces the list size by one.
//!
//! \param[in]	self	The pointer to the list
//! \param[out] ppData	The pointer to the data
//!
//!	\return		S_OK if successful
//!	\return		E_FAIL if the list does not contain any node
//!	\return		E_POINTER if self is NULL
///////////////////////////////////////////////////////////////////////////////

HRESULT std_list_pop_front(std_list* self, PVOID* ppData);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Adds a new node at the end of a list and store the data
//!				This effectively increases the list size by one.
//!
//! \param[in]	self	The pointer to the list
//! \param[in]	pData	The data
//!
//!	\return		S_OK if successful
//! \return		E_POINTER if self is NULL
//!	\return		E_OUTOFMEMORY if not enough memory to create the node
///////////////////////////////////////////////////////////////////////////////

HRESULT std_list_push_back(std_list* self, PVOID pData);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Adds a new node at the beginning of a list and store the data
//!				This effectively increases the list size by one.
//!
//! \param[in]	self	The pointer to the list
//! \param[in]	pData	The data
//!
//!	\return		S_OK if successful
//! \return		E_POINTER if self is NULL
//!	\return		E_OUTOFMEMORY if not enough memory to create the node
///////////////////////////////////////////////////////////////////////////////

HRESULT std_list_push_front(std_list* self, PVOID pData);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Inserts a node after a specific node.
//!				This effectively increases the list size by one.
//!
//! \param[in]	self	The pointer to the list
//! \param[in]	pNode	The node
//! \param[in]	pData	The data
//!
//!	\return		S_OK if successful
//! \return		E_POINTER if self is NULL
//! \return		E_FAIL if the list has no node
//!	\return		E_OUTOFMEMORY if not enough memory to create the node
///////////////////////////////////////////////////////////////////////////////

HRESULT std_list_insert(std_list* self, std_list_node* pNode, PVOID pData);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Erases a node in a list. This calls m_data_dtor to destruct 
//!				data in all nodes. This effectively decreases the list size by one.
//!
//! \param[in]	self	The pointer to the list
//! \param[in]	pNode	The node
//!
//!	\return		S_OK if successful
//! \return		E_POINTER if self is NULL
//! \return		E_FAIL if the list has no node
///////////////////////////////////////////////////////////////////////////////

HRESULT std_list_remove(std_list* self, std_list_node* pNode);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Returns the number of nodes in a list.
//!
//! \param[in]	self	The pointer to the list
//! \param[out] puiSize	The pointer to the size
//!
//!	\return		S_OK if successful.
//!	\return		E_POINTER if self or puiSize is NULL.
//! \return		E_FAIL if the list has no node
///////////////////////////////////////////////////////////////////////////////

HRESULT std_list_size(std_list* self, UINT* puiSize);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Gets the pointer to the first or next node.
//!				If *ppNode is NULL, set the first node in it,
//!				otherwise set the next node of *ppNode.
//!
//! \param[in]		self	The pointer to the list
//! \param[in/out]	ppNode	The pointer to the node
//!
//! \return		S_OK if sucessful
//!	\return		E_POINTER if self or ppNode is NULL.
///////////////////////////////////////////////////////////////////////////////

HRESULT std_list_iter(std_list* self, std_list_node** ppNode);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Gets the first node of the list.
//!
//! \param[in]	self	The pointer to the list
//! \param[out]	ppNode	The pointer to the first node
//!
//! \return		S_OK if sucessful
//!	\return		E_POINTER if self or ppNode is NULL.
///////////////////////////////////////////////////////////////////////////////

HRESULT std_list_first(std_list* self, std_list_node** ppNode);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Gets the last node of the list.
//!
//! \param[in]	self	The pointer to the list
//! \param[out]	ppNode	The pointer to the last node
//!
//! \return		S_OK if sucessful
//!	\return		E_POINTER if self or ppNode is NULL.
///////////////////////////////////////////////////////////////////////////////

HRESULT std_list_last(std_list* self, std_list_node** ppNode);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Gets the data of the first node in the list.
//!
//! \param[in]	self	The pointer to the list
//! \param[out]	ppNode	The pointer to the data in the first node
//!
//! \return		S_OK if sucessful
//! \return		E_FAIL if the list is empty
//!	\return		E_POINTER if self or ppNode is NULL.
///////////////////////////////////////////////////////////////////////////////

HRESULT std_list_front(std_list* self, PVOID* ppData);

///////////////////////////////////////////////////////////////////////////////
//! \brief		Gets the data of the last node in the list.
//!
//! \param[in]	self	The pointer to the list
//! \param[out]	ppData	The pointer to the data in the last node
//!
//! \return		S_OK if sucessful
//! \return		E_FAIL if the list is empty
//!	\return		E_POINTER if self or ppNode is NULL.
///////////////////////////////////////////////////////////////////////////////

HRESULT std_list_back(std_list* self, PVOID* ppData);

///////////////////////////////////////////////////////////////////////////////
//! \brief		search the data of node in the list
//!
//! \param[in]	self	The pointer to the list
//! \param[in]	pData	The data in the node
//!
//! \return		Not NULL if sucessful
//! \return		NULL if not found
///////////////////////////////////////////////////////////////////////////////

std_list_node* std_list_search(std_list* self, PVOID pData);

#ifdef __cplusplus

}

#endif

#endif //<! #ifndef _STD_LIST_H_

