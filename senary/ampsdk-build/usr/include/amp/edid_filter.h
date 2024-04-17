/*******************************************************************************
*                Copyright 2007, MARVELL SEMICONDUCTOR, LTD.                   *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL.                      *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
*                                                                              *
* MARVELL COMPRISES MARVELL TECHNOLOGY GROUP LTD. (MTGL) AND ITS SUBSIDIARIES, *
* MARVELL INTERNATIONAL LTD. (MIL), MARVELL TECHNOLOGY, INC. (MTI), MARVELL    *
* SEMICONDUCTOR, INC. (MSI), MARVELL ASIA PTE LTD. (MAPL), MARVELL JAPAN K.K.  *
* (MJKK), MARVELL ISRAEL LTD. (MSIL).                                          *
*******************************************************************************/

/*******************************************************************************
 * \file     edid_filter.h
 * \brief    API for external filterin gusage.
 * \history
 *      Version    Date           Author
 *      1.00       Dec 23, 2011   yinxj@marvell.com
 ******************************************************************************/

#ifndef __EDID_FILTER_H__
#define __EDID_FILTER_H__

#include "com_type.h"

// Disable DTS
#define AUDIO_DTS_DISABLE

/*****************************************************************************
 * EDID filtering functions:To filter Audio/Video/HDMI
 ****************************************************************************/

HRESULT EDID_Filter_Capability(
        UINT8  *ptxEDID,
        UINT8   nBlock,
        BOOLEAN PCMOnly,
        UINT16  PhyAddr,
        UINT8  *prxEDID
);

HRESULT EDID_Get_3D_Capability(UINT8* pEDID, BOOLEAN* pbHdmi3DEn, BOOLEAN* pbAI);
HRESULT Extract_EDID_GammaNColor(UINT8* pEDIDSink, UINT8* pGammaColor);
HRESULT Dup_EDID_GammaNColor(UINT8* pEDID, UINT8* pGammaColor);
HRESULT Change_EDID_PhyAddr(UINT8* pEDID, UINT16 phy_addr, unsigned char ucAI_Sup);
#endif /* __EDID_FILTER_H__ */
