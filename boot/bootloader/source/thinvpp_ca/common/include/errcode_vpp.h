/********************************************************************************
*		Copyright 2007, MARVELL SEMICONDUCTOR, LTD.								*
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL.						*
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT	*
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE			*
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.		*
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,		*
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.	*
*																				*
* MARVELL COMPRISES MARVELL TECHNOLOGY GROUP LTD. (MTGL) AND ITS SUBSIDIARIES,	*
* MARVELL INTERNATIONAL LTD. (MIL), MARVELL TECHNOLOGY, INC. (MTI), MARVELL		*
* SEMICONDUCTOR, INC. (MSI), MARVELL ASIA PTE LTD. (MAPL), MARVELL JAPAN K.K.	*
* (MJKK), MARVELL ISRAEL LTD. (MSIL).											*
*********************************************************************************/

///////////////////////////////////////////////////////////////////////////////
//! \file errcode_vpp.h
//! \brief Header file for VPP error codes
///////////////////////////////////////////////////////////////////////////////

#ifndef	_ERRCODE_VPP_H_
#define	_ERRCODE_VPP_H_

#include "ErrorCode.h"

#define S_VPP( code ) ( E_SUC | E_VPP_BASE | ( (code) & 0xFFFF ) )
#define E_VPP( code ) ( E_ERR | E_VPP_BASE | ( (code) & 0xFFFF ) )

#define S_VPP_OK 						(S_OK)

/* error code */
#define VPP_E_NODEV         E_VPP(1)
#define VPP_E_BADPARAM      E_VPP(2)
#define VPP_E_BADCALL       E_VPP(3)
#define VPP_E_UNSUPPORT     E_VPP(4)
#define VPP_E_IOFAIL        E_VPP(5)
#define VPP_E_UNCONFIG      E_VPP(6)
#define VPP_E_CMDQFULL      E_VPP(7)
#define VPP_E_FRAMEQFULL    E_VPP(8)
#define VPP_E_BCMBUFFULL    E_VPP(9)
#define VPP_E_NOMEM         E_VPP(10)
#define VPP_EVBIBUFFULL  E_VPP(11)
#define VPP_EHARDWAREBUSY  E_VPP(12)
#define VPP_ENOSINKCNCTED  E_VPP(13)
#define VPP_ENOHDCPENABLED  E_VPP(14)
#endif //!< #ifndef _ERRCODE_VPP_H_
