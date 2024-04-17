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
//! \file		map.h
//! \brief 
//! \author		Junfeng Geng
//! \version	0.1
//! \date		July 2007
///////////////////////////////////////////////////////////////////////////////

#ifndef _MAP_H_
#define _MAP_H_

#include "com_type.h"
#include "ErrorCode.h"

/*
 * Status
 *
 * Feature      Implemented  Functioning  Reviewed  Tested
 *
 * ctor/dtor    Y            Y
 * clear        Y            Y
 * find         Y            Y
 * insert       Y            Y
 * erase        Y            Y
 * size         
 * iter         Y            Y
 *
 * Implemented = code is written
 * Functioning = pass a basic test
 * Reviewed    = carefully examined. should not exist bugs
 * Tested      = passed UT
 */

typedef enum
{
	STD_MAP_NODE_VALUE,
	STD_MAP_NODE_POINTER
} std_map_node_type;

typedef struct _std_map_node
{
	PVOID pData;
	PVOID pKey;
	UINT bRed;
	struct _std_map_node* pLeft;
	struct _std_map_node* pRight;
	struct _std_map_node* pParent;
} std_map_node;

/* Compare(a,b) should return 1 if *a > *b, -1 if *a < *b, and 0 otherwise */
typedef INT (*std_map_compare_func)(PVOID a, PVOID b);
typedef VOID (*std_map_dtor_func)(PVOID obj);

typedef struct _std_map
{
	std_map_compare_func compare;
	std_map_dtor_func key_dtor;
	std_map_dtor_func data_dtor;
	std_map_node* m_pRoot;
	std_map_node* m_pNil;
	std_map_node_type m_eType;
	UINT uiSize;
} std_map;

std_map* std_map_ctor(std_map* self,
		      std_map_compare_func compare,
		      std_map_dtor_func key_dtor,
		      std_map_dtor_func data_dtor,
		      std_map_node_type eType);
VOID std_map_dtor(std_map* self);
HRESULT std_map_clear(std_map* self);
HRESULT std_map_find(std_map* self, PVOID pKey, PVOID* ppData);
HRESULT std_map_insert(std_map* self, PVOID pKey, PVOID pData);
HRESULT std_map_erase(std_map* self, PVOID pKey);
HRESULT std_map_size(std_map* self, UINT* puiSize);
HRESULT std_map_iter(std_map* self, std_map_node** ppNode);

#endif //<! #ifndef _MAP_H_
