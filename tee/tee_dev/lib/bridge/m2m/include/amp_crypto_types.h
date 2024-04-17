/*******************************************************************************
*                Copyright 2012, MARVELL SEMICONDUCTOR, LTD.                   *
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

////////////////////////////////////////////////////////////////////////////////
//! \file amp_crypto_types.h
//!
//! \brief This file define all common data structure, enum and API for AMP
//!        crypto.
//!
//! Purpose:
//!
//!
//! Version, Date and Author :
//!   V 1.00,    March 22 2013,    Yongquan Jia
//!
//! Note: This file owner is Yongquan.
//!       It should be sync with drm/server/keymgr/inc/drm_scheme.h
////////////////////////////////////////////////////////////////////////////////
#ifndef __AMP_CRYPTO_TYPES_H__
#define __AMP_CRYPTO_TYPES_H__

#include "types.h"

/** \enum AMP_DRMSCHEME_TYPE
 *  \brief AMP drm scheme type definition.
 */
typedef enum {
    AMP_DRMSCHEME_INVALID,      /**< Invalid type */
    AMP_DRMSCHEME_HDCP2X,       /**< HDCP 2.0 */
    AMP_DRMSCHEME_DTCPIP,       /**< DTCP on IP */
    AMP_DRMSCHEME_CIPLUS,       /**< CI+ */
    AMP_DRMSCHEME_WIDEVINE,     /**< WideVine */
    AMP_DRMSCHEME_PLAYREADY,    /**< Playready */
    AMP_DRMSCHEME_VUDU,         /**< VUDU */
    AMP_DRMSCHEME_VRMTX,        /**< VRMTX */
    AMP_DRMSCHEME_NDS,          /**< NDS */
    AMP_DRMSCHEME_MLA,          /**< MARLIN Audio */
    AMP_DRMSCHEME_MOBITV,        /**< MOBITV */
    AMP_DRMSCHEME_PVR_C0,        /**< PVR_C0 */
    AMP_DRMSCHEME_DUMMY,        /**< DUMMY */
    AMP_DRMSCHEME_STUB,         /**< STUB */
    AMP_DRMSCHEME_PVR_GENERAL,  /**< PVR_GENERAL */
    AMP_DRMSCHEME_MARLIN,       /**< MARLIN */
    AMP_DRMSCHEME_RAW,          /**< RAW */
    AMP_DRMSCHEME_NAGRA,         /**< NAGRA */

    AMP_DRMSCHEME_CUSTOMER_BASE = 100, /**< customer define DRM scheme, >100 */
} AMP_DRMSCHEME_TYPE;

/** \def KEY_ID_MAX_LEN
    \brief This macro function used to define key ID max length.
 */
#define KEY_ID_MAX_LEN              (48)

/** \typedef AMP_CRYPTO_ST
    \brief Type definition for AMP CRYPTO structure
 */
typedef struct {
    uint32_t  uiSchemeType;           /**< The Scheme type of DRM */
    uint32_t  uiSessionID;            /**< The identify of Session */
    uint8_t   uiKeyID[KEY_ID_MAX_LEN];/**< The identify Key */
    uint32_t  uiKeyIdLen;             /**< The length of key id */
} AMP_CRYPTO_ST;
#endif  /* __AMP_CRYPTO_TYPES_H__ */
