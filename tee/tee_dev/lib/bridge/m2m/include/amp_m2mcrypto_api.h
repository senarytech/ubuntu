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
//! \file amp_m2mcrypto_api.h
//!
//! \brief This file define all common data structure, enum and API for
//!        AMP M2M Crypto.
//!
//!
//!     Version     Date                    Author
//!     V 1.00,     Apr. 1 2014,       Weizhao Jiang
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef __AMP_M2M_CRYPTO_H__
#define __AMP_M2M_CRYPTO_H__
///////////////////////////////////////////////////////////////////////////////
//! [Part1] Header file include
///////////////////////////////////////////////////////////////////////////////
//#include "amp_types.h"
//#include "amp_crypto_types.h"
//#include "isl/amp_shm.h"
///////////////////////////////////////////////////////////////////////////////
//! [Part2] Local macros, type definitions
///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

/** \enum AMP_M2MCRYPTO_KEY_OPTION
 *  \brief The key option is indicating what's the key and where it is from.
 *         The option will be used case by case in SetKey().
 */
typedef enum __m2m_key_option__ {
        AMP_M2MCRYPTO_KEY_INVALID       = 0x0,      /**< invalid case*/
        AMP_M2MCRYPTO_KEY_CONTENT       = 0x1,      /**< Content key from Apps*/
        AMP_M2MCRYPTO_KEY_INDEX         = 0x2,      /**< Key table index from Apps*/
        AMP_M2MCRYPTO_KEY_FROM_DRM      = 0x3,      /**< Key from DRM*/
        AMP_M2MCRYPTO_KEY_MAX,
} AMP_M2MCRYPTO_KEY_OPTION;

/** \enum AMP_M2MCRYPTO_CRYPTO_TYPE
 *  \brief To enumerate all the dmx crypto type.
 */
typedef enum __m2m_crypto_type__ {
        AMP_M2MCRYPTO_TYPE_INVALID        = 0x0,      /**< Invalid case*/
        AMP_M2MCRYPTO_TYPE_AES_128_ECB    = 0x1,      /**< for AES_128_ECB case */
        AMP_M2MCRYPTO_TYPE_AES_128_CBC    = 0x2,      /**< for AES_128_CBC case */
        AMP_M2MCRYPTO_TYPE_AES_128_CTR    = 0x3,      /**< for AES_128_CTR case */
        AMP_M2MCRYPTO_TYPE_TDES_128_ECB   = 0x4,      /**< for TDES_128_ECB case */
        AMP_M2MCRYPTO_TYPE_TDES_128_CBC   = 0x5,      /**< for TDES_128_CBC case */
        AMP_M2MCRYPTO_TYPE_TDES_128_CTR   = 0x6,      /**< for TDES_128_CTR case */
        AMP_M2MCRYPTO_TYPE_DVB_CSA_30     = 0x7,      /**< for DVB_CSA_30 case */
        AMP_M2MCRYPTO_TYPE_ASA            = 0x8,      /**< for ASA case */
        AMP_M2MCRYPTO_TYPE_DES_56_ECB     = 0x9,      /**< for DES_56_ECB case */
        AMP_M2MCRYPTO_TYPE_DVB_CSA_20     = 0xa,      /**< for DVB_CSA_20 case */
        AMP_M2MCRYPTO_TYPE_MAX                        /**< Max value */
} AMP_M2MCRYPTO_TYPE;

/** \enum AMP_M2MCRYPTO_SESSION_TYPE
 *  \brief To enumerate all the dmx crypto type.
 */
typedef enum __m2m_sess_type__ {
        AMP_M2MCRYPTO_SESSION_TYPE_INVALID= 0x0,    /**< Invalid case*/
        AMP_M2MCRYPTO_SESSION_TYPE_ENC    = 0x1,    /**< for encryption session */
        AMP_M2MCRYPTO_SESSION_TYPE_DEC    = 0x2,    /**< for decryption session */
        AMP_M2MCRYPTO_SESSION_TYPE_MAX              /**< Max value */
} AMP_M2MCRYPTO_SESSION_TYPE;

#ifdef __cplusplus
}
#endif

#endif  /*__AMP_M2M_CRYPTO_H__*/


