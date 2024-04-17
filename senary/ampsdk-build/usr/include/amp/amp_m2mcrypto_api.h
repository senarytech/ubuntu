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
#include "amp_types.h"
#include "isl/amp_shm.h"
///////////////////////////////////////////////////////////////////////////////
//! [Part2] Local macros, type definitions
///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

/** \typedef AMP_DRM_CRYPTO_ST
 *  \brief In case app using M2M still use the AMP_CRYPTO_ST
 *         Please use AMP_DRM_CRYPTO_ST in the future development
 */
typedef AMP_DRM_CRYPTO_ST AMP_CRYPTO_ST;

/** \typedef AMP_M2MCRYPTO_CB
 *  \brief AMP M2M user callback handle. it will be used to pass callback func.
 *         user is trying to do encryption/decryption with asynchronous mode.
 *         The callback is used to notify the operation is done.
 *         The parameter -- pUserdata is passed in AsyncUpdate() called by user.
 *         It will be passed back to user when callback is called.
 */
typedef HRESULT (*AMP_M2MCRYPTO_CB)(VOID *pUserdata);


/** \struct AMP_M2MCRYPTO_MEMOBJ
 *  \typedef AMP_M2MCRYPTO_MEM_H
 *  \brief The structure of memory handle to indicate both AMP SHM handle/offset
 *         and size. It will be used in API SyncUpdate()/AsyncUpdate() to
 *         indicate the IO data block in SHM.
 */
typedef struct __m2m_mem_handle__ {
    AMP_SHM_HANDLE      hSHM;
    UINT32              uOffset;
    UINT32              uSize;
} AMP_M2MCRYPTO_MEMOBJ, *AMP_M2MCRYPTO_MEM_H;


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
        AMP_M2MCRYPTO_TYPE_AES_128_CTR64  = 0xb,      /**< for AES_128_CTR64 case */
        AMP_M2MCRYPTO_TYPE_AES_128_CTR128 = 0xc,      /**< for AES_128_CTR128 case */
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

/** \typedef AMP_M2MCRYPTO_PATTERN_MODE_CFG
 *  \brief in library.idl had define AMP_LIB_M2M_PATTERN_MODE_CFG_ST
 *         So don't need to define new structure, just typedef
 */
typedef AMP_LIB_M2M_PATTERN_MODE_CFG_ST AMP_M2MCRYPTO_PATTERN_MODE_CFG;

/** \enum AMP_M2MCRYPTO_RESIDUE_TYPE
*  \brief all the residue types which M2M can support.
*/
typedef enum M2M_RESIDUE_TYPE {
        AMP_M2MCRYPTO_RESIDUE_TYPE_DEFAULT   =0x0,        /**< for CBC the default type is clear , for CTR it is encrypted */
        AMP_M2MCRYPTO_RESIDUE_TYPE_PATTERN   =0x1,        /**<  for pattern mode */
        AMP_M2MCRYPTO_RESIDUE_TYPE_EXOR      =0x2,        /**< do exor operation, vudu/ATIS use this */
        AMP_M2MCRYPTO_RESIDUE_TYPE_RP        =0x3,        /**< for marlin case */
        AMP_M2MCRYPTO_RESIDUE_TYPE_CTS       =0x4,        /**< widevine legacy use cts */
        AMP_M2MCRYPTO_RESIDUE_TYPE_MAX                    /**< Number of total types. */
}AMP_M2MCRYPTO_RESIDUE_TYPE;


/** \fn HRESULT  AMP_M2MCrypto_Open(AMP_M2MCRYPTO_SESSION_TYPE eType,
 *                               UINT32 *pSessionID);
 *
 *  \brief This function is called to create a crypto connection session from
 *          client to server. The session structure will be allocated and
 *          initialized. The session ID will be returned to user after
 *          successfully open.
 *
 *         pSessionID is the Output parameter, it will be set after successfully
 *         open. It will be used to operate this session such as SetScheme()/
 *         Close()/SetKey()/SyncUpdate()/AsyncUpdate().
 *
 *         AMP_M2MCrypto_Close() is paired with AMP_M2MCrypto_Open().
 *
 *  \param eType        IN      The type of session, enc or dec.
 *  \param pSessionID   OUT     Return Session ID.
 *
 *  \retval M2M_OK       Open succeeded
 *  \retval M2M_FAIL     Open fail
 *
 *  \sa AMP_M2MCrypto_Close
 *  \sa AMP_M2MCrypto_SetScheme
 *  \sa AMP_M2MCrypto_SetKey
 *  \sa AMP_M2MCrypto_SyncUpdate
 *  \sa AMP_M2MCrypto_AsyncUpdate
 */
HRESULT AMP_M2MCrypto_Open(AMP_M2MCRYPTO_SESSION_TYPE eType, UINT32 *pSessionID);


/** \fn HRESULT  AMP_M2MCrypto_Close(UINT32 uSessionID);
 *
 *  \brief This function is implemented to close a previously opened crypto
 *          session. The resource will be freed and all allocated software
 *          structures will be deleted. The session will then be available to
 *          be opened again with a call to AMP_M2MCrypto_Open.
 *
 *         AMP_M2MCrypto_Open() is paired with AMP_M2MCrypto_Close().
 *
 *  \param uSessionID   IN      The ID of session.
 *
 *  \retval M2M_OK       Close succeeded
 *  \retval M2M_FAIL     Close fail
 *
 *  \sa AMP_M2MCrypto_Open
 *  \sa AMP_M2MCrypto_SetScheme
 *  \sa AMP_M2MCrypto_SetKey
 *  \sa AMP_M2MCrypto_SyncUpdate
 *  \sa AMP_M2MCrypto_AsyncUpdate
 */
HRESULT AMP_M2MCrypto_Close(UINT32 uSessionID);

/** \fn HRESULT  AMP_M2MCrypto_RegisterNotify(UINT32 uSessionID,
 *                                                          UINT32 uServiceId,
 *                                                          UINT32 uEventType);
 *
 *  \brief This function is called to register an event notify to a m2m session
    *
    *         The event listener is a standalone software module. Application
    *         shall call AMP_Event_CreateListener() to create a listener at
    *         client, then call AMP_Event_RegisterCallback() to register event
    *         to the listener. To make M2M session get the which event shall
    *         be send out, the application shall call this function to let
    *         m2m session  knows which event listener it shall send to,
    *         and what event shall send out.
 *
 *  \param uSessionID   IN      The ID of session.
 *  \param uServiceId   IN     The event listener service id.
 *  \param uServiceId   IN     The event type.
 *
 *  \retval M2M_OK       Succeeded
 *  \retval M2M_FAIL     Fail
 *
 *  \sa AMP_M2MCrypto_Open
 *  \sa AMP_M2MCrypto_close
 */
HRESULT AMP_M2MCrypto_RegisterNotify(UINT32 uSessionID,
                                        UINT32 uServiceId,
                                        UINT32 uEventType);

/** \fn HRESULT  AMP_M2MCrypto_UnregisterNotify(UINT32 uSessionID,
 *                                                          UINT32 uServiceId,
 *                                                          UINT32 uEventType);
 *
 *  \brief This function is called to unregister an event notify from a m2m session
 *
 *         The application shall also call AMP_Event_UnregisterCallback()
 *         to let the event listener stop the specified event passing. And
 *         call AMP_Event_DestroyListener() to destroy the event listener.
 *
 *  \param uSessionID   IN      The ID of session.
 *  \param uServiceId   IN     The event listener service id.
 *  \param uServiceId   IN     The event type.
 *
 *  \retval M2M_OK       Succeeded
 *  \retval M2M_FAIL     Fail
 *
 *  \sa AMP_M2MCrypto_Open
 *  \sa AMP_M2MCrypto_close
 */
HRESULT AMP_M2MCrypto_UnregisterNotify(UINT32 uSessionID,
                                        UINT32 uServiceId,
                                        UINT32 uEventType);

/** \fn HRESULT AMP_M2MCrypto_SetScheme(UINT32 uSessionID,
 *                           M2M_CRYPTO_TYPE eCryptoType);
 *
 *  \brief This function is called to set the type of crypto algorithm, detailed
 *          in AMP_M2MCRYPTO_TYPE.
 *
 *  \param uSessionID   IN      The ID of session.
 *  \param eCryptoType  IN      The type of crypto algorithm.
 *
 *  \retval M2M_OK       Succeeded
 *  \retval M2M_FAIL     Fail
 *
 *  \sa AMP_M2MCrypto_Open
 *  \sa AMP_M2MCrypto_close
 *  \sa AMP_M2MCrypto_Update
 */
HRESULT AMP_M2MCrypto_SetScheme(UINT32 uSessionID,
                            AMP_M2MCRYPTO_TYPE eCryptoType);

/** \fn HRESULT AMP_M2MCrypto_SetKey(UINT32   uSessionID,
 *                           AMP_M2MCRYPTO_KEY_OPTION  KeyOption,
 *                           UINT32          KeyTblIndex,
 *                           UINT8           *pKeyData,
 *                           UINT32          KeyLength,
 *                           AMP_M2MCRYPTO_KEY_OPTION  IVOption,
 *                           UINT32          IVTblIndex,
 *                           UINT8           *pIVData,
 *                           UINT32          IVLength,
 *                           AMP_DRMSCHEME_TYPE  *pCryptInfo);
 *
 *  \brief This function is called to set Key/IVKey, user need to indicate Key
 *          option as below:
 *          1. KEY_INVALID, the key won't be applied.
 *          2. KEY_INDEX, the uKeyIndex will be applied.
 *          3. KEY_CONTENT, the pKeyData and uKeyLen will be applied.
 *          4. KEY_DROM_DRM, DMX will do interaction with DRM, get the Key
 *             context with CryptInfo from DRM.
 *
 *
 *  \param uSessionID   IN      The ID of session.
 *  \param eCryptoType  IN      The type of crypto algorithm.
 *
 *  \retval M2M_OK       Succeeded
 *  \retval M2M_FAIL     Fail
 *
 *  \sa AMP_M2MCrypto_Open
 *  \sa AMP_M2MCrypto_close
 *  \sa AMP_M2MCrypto_Update
 */
HRESULT AMP_M2MCrypto_SetKey(UINT32   uSessionID,
                            AMP_M2MCRYPTO_KEY_OPTION  KeyOption,
                            UINT32          KeyTblIndex,
                            UINT8           *pKeyData,
                            UINT32          KeyLength,
                            AMP_M2MCRYPTO_KEY_OPTION  IVOption,
                            UINT32          IVTblIndex,
                            UINT8           *pIVData,
                            UINT32          IVLength,
                            AMP_DRM_CRYPTO_ST   *pCryptInfo);

/** \fn HRESULT  AMP_M2MCrypto_SetPatternMode(UINT32 uSessionID,
 *                                AMP_M2MCRYPTO_PATTERN_MODE_CFG *patternModeCfg);
 *
 *  \brief This function is called to set the  cfg of pattern mode
 *
 *  \param uSessionID   IN      The ID of session.
 *  \param patternModeCfg  IN     the infomation of pattern mode.
 *
 *  \retval M2M_OK       succeeded
 *  \retval M2M_FAIL     fail
 *
 *  \sa AMP_M2MCrypto_Open
 *  \sa AMP_M2MCrypto_close
 *  \sa AMP_M2MCrypto_Update
 */
HRESULT AMP_M2MCrypto_SetPatternMode(UINT32   uSessionID,
                                     AMP_M2MCRYPTO_PATTERN_MODE_CFG *patternModeCfg);

/** \fn HRESULT  AMP_M2MCrypto_SetResidueMode(UINT32 uSessionID,
 *                                AMP_M2MCRYPTO_RESIDUE_TYPE residueType);
 *
 *  \brief This function is called to set the  mode of residue,default is clear mode
 *
 *  \param uSessionID   IN      The ID of session.
 *  \param residueType  IN     the type of residue.
 *
 *  \retval M2M_OK       succeeded
 *  \retval M2M_FAIL     fail
 *
 *  \sa AMP_M2MCrypto_Open
 *  \sa AMP_M2MCrypto_close
 *  \sa AMP_M2MCrypto_Update
 */
HRESULT AMP_M2MCrypto_SetResidueMode(UINT32   uSessionID,
                                    AMP_M2MCRYPTO_RESIDUE_TYPE residueType);


/** \fn HRESULT  AMP_M2MCrypto_SyncUpdate(UINT32 uSessionID,
 *                           AMP_M2MCRYPTO_MEM_H hInput,
 *                           AMP_M2MCRYPTO_MEM_H hOutput);
 *
 *  \brief This is memory to memory function call for encryption and decryption
 *          in synchronous mode.
 *
 *  \param uSessionID   IN      The ID of session.
 *  \param hInput       IN      The handle of input memory.
 *  \param hOutput      IN      The handle of output memory.
 *
 *  \retval M2M_OK       succeeded
 *  \retval M2M_FAIL     fail
 *
 *  \sa AMP_M2MCrypto_Open
 *  \sa AMP_M2MCrypto_Close
 *  \sa AMP_M2MCrypto_Config
 */
HRESULT AMP_M2MCrypto_SyncUpdate(UINT32 uSessionID,
                            AMP_M2MCRYPTO_MEM_H hInput,
                            AMP_M2MCRYPTO_MEM_H hOutput);

/** \fn HRESULT  AMP_M2MCrypto_AsyncUpdate(UINT32 uSessionID,
 *                           AMP_M2MCRYPTO_MEM_H hInput,
 *                           UINT32 uInputSize,
 *                           AMP_M2MCRYPTO_MEM_H hOutput,
 *                           UINT32 *pOutputSize,
 *                           AMP_M2MCRYPTO_CB *pCallback,
 *                           VOID          *pData);
 *
 *
 *  \brief This is memory to memory function call for encryption and decryption
 *          in asynchronous mode.
 *
 *  \param uSessionID   IN      The ID of session.
 *  \param hInput       IN      The handle of input memory.
 *  \param hOutput      IN      The handle of output memory.
 *
 *  \retval M2M_OK       succeeded
 *  \retval M2M_FAIL     fail
 *
 *  \sa AMP_M2MCrypto_Open
 *  \sa AMP_M2MCrypto_Close
 *  \sa AMP_M2MCrypto_Config
 */
HRESULT AMP_M2MCrypto_AsyncUpdate(UINT32 uSessionID,
                            AMP_M2MCRYPTO_MEM_H hInput,
                            AMP_M2MCRYPTO_MEM_H hOutput);


#ifdef __cplusplus
}
#endif

#endif  /*__AMP_M2M_CRYPTO_H__*/


