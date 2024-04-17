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

#ifndef __DRM_KEY_AGENT_API_H__
#define __DRM_KEY_AGENT_API_H__

#include "OSAL_api.h"

/***************************************************************************
 * Description:
 *     DRM KEY Agent provides APIs for use with external/3rd party software
 *     to create customized DRM Scheme and manage cipher/clear keys based
 *     on Marvell DRM solution.
 *
 * DRM Scheme Big Picture:
 *     DRM Scheme may contain several content key units, and each key unit
 *     has key & IV pair. For cipher key/IV, a HW keyslot needs to be
 *     allocated for use with TZ TA for key/IV loading.
 *
 *                            ----------------
 *                            |  DRM Scheme  |
 *                            ----------------
 *                                   |
 *            ---------------------------------------------------
 *            |                      |                          |
 *    ------------------     ------------------        ------------------
 *    | ContentKeyUnit |     | ContentKeyUnit |        | ContentKeyUnit |
 *    ------------------     ------------------        ------------------
 *            |                      |                          |
 *       ------------                |                   ---------------
 *       |          |                |                   |             |
 *   ------------ -----------        |              ----------- ----------
 *   |HW KeySlot| |HW IVSlot|        |              |Clear key| |Clear IV|
 *   ------------ -----------        |              ----------- ----------
 *                            ---------------
 *                            |             |
 *                       ------------ ------------
 *                       |HW KeySlot| | Clear IV |
 *                       ------------ ------------
 *
 * APIs and usage:
 *
 * 1. Register/Unregister DRM Scheme
 *
 * MV_DRM_KEY_RequestDRMScheme(INOUT UINT32 *puScheme, IN UINT32 uSessionID);
 *     Register a DRM Scheme to DRM KEY server, on success the puScheme
 *     contains the DRM Scheme that Key server allocates
 *
 * MV_DRM_KEY_ReleaseDRMScheme(IN UINT32 uScheme, IN UINT32 uSessionID);
 *     Unregister the DRM Scheme and all its resources will be freed
 *
 * 2. Content Key unit allocation/free
 *
 * MV_DRM_KEY_AllocateContentKeyUnit(IN UINT32 uScheme, IN UINT32 uSessionID,
 *                                   OUT UINT32 *puKeyID);
 *    Allocate a Content Key Unit for DRM scheme specified in uScheme, on
 *    success the puKeyID has the Content Key Unit ID that Key server
 *    allocates
 *
 * MV_DRM_KEY_FreeContentKeyUnit(IN UINT32 uScheme, IN UINT32 uSessionID,
 *                               IN uKeyID);
 *    Free Content Key Unit and all its resources
 *
 * 3. Content Key Unit algorithm configuration
 *
 * MV_DRM_KEY_ConfigAlgorithm(IN UINT32 uScheme, IN UINT32 uSessionID,
 *                            IN UINT32 uKeyID,
 *                            IN UINT32 uAlgType, IN UINT32 uAlgMode);
 *    Configure the algorithm type and mode of a given Content Key Unit
 *
 * 4. Allocate HW key/IV slot
 *
 * MV_DRM_KEY_AllocateKeySlot(IN UINT32 uScheme, IN UINT32 uSessionID,
 *                            IN UINT32 uKeyID, IN UINT32 uKeyLen,
 *                            OUT UINT32 *puKeyIndex);
 * MV_DRM_KEY_AllocateIVSlot(IN UINT32 uScheme, IN UINT32 uSessionID,
 *                           IN UINT32 uKeyID, IN UINT32 uIVLen,
 *                           OUT UINT32 *puIVIndex);
 *
 *    Allocate needed HW key slot for cipher key/IV, on success puKeyIndex
 *    contains the HW index of cipher key slot and puIVIndex contains the
 *    HW index of cipher IV slot
 *
 * 5. Allocate HW key/IV slot pair
 * MV_DRM_KEY_AllocateKeySlotPair(IN UINT32 uScheme, IN UINT32 uSessionID,
 *                                IN UINT32 uOddKeyID, IN UINT32 uEvenKeyID,
 *                                IN UINT32 uKeyLen,
 *                                OUT UINT32 *puOddKeyIndex,
 *                                OUT UINT32 *pEvenKeyIndex);
 * MV_DRM_KEY_AllocateIVSlotPair(IN UINT32 uScheme, IN UINT32 uSessionID,
 *                               IN UINT32 uOddKeyID, IN UINT32 uEvenKeyID,
 *                               IN UINT32 uIVLen,
 *                               OUT UINT32 *puOddIVIndex,
 *                               OUT UINT32 *puEvenIVIndex);
 *
 *    Allocate needed HW key slot pair for cipher key/IV, on success
 *    puOddKeyIndex and puEvenKeyIndex contains the HW index of odd/even
 *    cipher keyslot, puOddIVIndex and puEvenIVIndex contains the odd/even
 *    HW index of cipher IV slot
 *
 * 6. Clear key/IV loading
 *
 * MV_DRM_KEY_LoadClearKey(IN UINT32 uScheme, IN UINT32 uSessionID,
 *                         IN UINT32 uKeyID,
 *                         IN UINT8 *pbKeyData, IN INT32 uKeyDataLen);
 * MV_DRM_KEY_LoadClearIV(IN UINT32 uScheme, IN UINT32 uSessionID,
 *                        IN UINT32 uKeyID,
 *                        IN UINT8 *pbIVData, IN INT32 uIVDataLen);
 *    Loads clear key/IV into Content Key Unit
 *
 * Example:
 *
 * #define CIPLUS_SCHEME 10000
 *
 * UINT32 uCIPLUSSCHEME = CIPLUS_SCHEME;
 * UINT32 uODDKEY, uEVENKEY;
 * HRESULT lRes = S_OK;
 * UINT8 bIVData[16], bEVENData[16], bODDData[16];
 * UINT32 uODDIDX, uEVENIDX, uODDIVIDX, uEVENIVIDX;
 *
 * HRESULT CIPLUS_INIT(void)
 * {
 *   lRes = MV_DRM_KEY_RequestDRMScheme(&uCIPLUSSCHEME, 0);
 *   if (S_OK != lRes)
 *     return lRes;
 *   lRes = MV_DRM_KEY_AllocateContentKeyUnit(uCIPLUSSCHEME, 0,
 *                                            &uODDKEY);
 *   if (S_OK != lRes)
 *     return lRes;
 *   lRes = MV_DRM_KEY_AllocateContentKeyUnit(uCIPLUSSCHEME, 0,
 *                                            &uEVENKEY);
 *   if (S_OK != lRes)
 *     return lRes;
 *   lRes = MV_DRM_KEY_ConfigAlgorithm(uCIPLUSSCHEME, 0,
 *                                     uODDKEY, DRMCKTYPE_AES, DRMCKMODE_CBC);
 *   if (S_OK != lRes)
 *     return lRes;
 *   lRes = MV_DRM_KEY_ConfigAlgorithm(uCIPLUSSCHEME, 0,
 *                                     uEVENKEY, DRMCKTYPE_AES, DRMCKMODE_CBC);
 *   if (S_OK != lRes)
 *     return lRes;
 * #ifdef CONFIG_CLEAR_KEY
 *   lRes = MV_DRM_KEY_LoadClearKey(uCIPLUSSCHEME, 0,
 *                                  uODDKEY, bODDData, 16);
 *   if (S_OK != lRes)
 *     return lRes;
 *   lRes = MV_DRM_KEY_LoadClearIV(uCIPLUSSCHEME, 0,
 *                                 uODDKEY, bIVData, 16);
 *   if (S_OK != lRes)
 *     return lRes;
 *   lRes = MV_DRM_KEY_LoadClearKey(uCIPLUSSCHEME, 0,
 *                                  uEVENKEY, bEVENData, 16);
 *   if (S_OK != lRes)
 *     return lRes;
 *   lRes = MV_DRM_KEY_LoadClearIV(uCIPLUSSCHEME, 0,
 *                                 uEVENKEY, bIVData, 16);
 *   if (S_OK != lRes)
 *     return lRes;
 * #else
 *   lRes = MV_DRM_KEY_AllocateKeySlotPair(uCIPLUSSCHEME, 0,
 *                                         uODDKEY, uEVENKEY, 16,
 *                                         &uODDIDX, &uEVENIDX);
 *   if (S_OK != lRes)
 *     return lRes;
 *   lRes = MV_DRM_KEY_AllocateIVSlotPair(uCIPLUSSCHEME, 0,
 *                                        uODDKEY, uEVENKEY, 16,
 *                                        &uODDIVIDX, &uEVENIVIDX);
 *   if (S_OK != lRes)
 *     return lRes;
 *
 *   // Now uODDIDX, uEVENIDX, uODDIVIDX, uEVENIVIDX can be
 *   // passed to TA for key loading
 * #endif
 * }
 *
 * HRESULT CIPLUS_EXIT(void)
 * {
 *   MV_DRM_KEY_ReleaseDRMScheme(uCIPLUSSCHEME, 0);
 *   return S_OK;
 * }
 *
 **************************************************************************/

/***************************************************************************
    Description:
        Register a DRM Scheme into DRM Key server

    Input Parameters:
        puScheme:    DRM Scheme number, usage:
                     (0 ~ 99):
                       These number are reserved for DRM internal use. DRM
                       key server will dynamically allocate scheme number
                       if set to any number of these values
                     (100 ~ 65535):
                       Specify the scheme number to register
        uSessionID:  session ID

    Output Parameters:
        puScheme:    DRM Scheme number that successfully allocated by DRM
                     key server
**************************************************************************/
HRESULT MV_DRM_KEY_RequestDRMScheme(UINT32 *puScheme, UINT32 uSessionID);

/***************************************************************************
    Description:
        Unregister a DRM Scheme

    Input Parameters:
        uScheme:     DRM Scheme number
        uSessionID:  session ID

    Output Parameters:
        N/A
**************************************************************************/
HRESULT MV_DRM_KEY_ReleaseDRMScheme(UINT32 uScheme, UINT32 SessionID);

/***************************************************************************
    Description:
        Allocate Content Key Unit in a given DRM scheme

    Input Parameters:
        uScheme:     DRM Scheme number
        uSessionID:  session ID

    Output Parameters:
        puKeyID:     Content Key Unit ID that allocated by DRM key server
**************************************************************************/
HRESULT MV_DRM_KEY_AllocateContentKeyUnit(UINT32 uScheme, UINT32 uSessionID,
                                          UINT32 *puKeyID);

/***************************************************************************
    Description:
        Free Content Key Unit in a given DRM scheme

    Input Parameters:
        uScheme:     DRM Scheme number
        uSessionID:  session ID
        uKeyID:      Content Key Unit ID

    Output Parameters:
        N/A
**************************************************************************/
HRESULT MV_DRM_KEY_FreeContentKeyUnit(UINT32 uScheme, UINT32 uSessionID, UINT32 uKeyID);

/***************************************************************************
    Description:
        Configure algorithm of a given Conent Key Unit

    Input Parameters:
        uScheme:     DRM Scheme number
        uSessionID:  session ID
        uKeyID:      Content Key Unit ID
        uAlgType:    Algorithm type
        uAlgMode:    Algorithm mode

    Output Parameters:
        N/A
**************************************************************************/
HRESULT MV_DRM_KEY_ConfigAlgorithm(UINT32 uScheme, UINT32 uSessionID, UINT32 uKeyID,
                                   UINT32 uAlgType, UINT32 uAlgMode);

/***************************************************************************
    Description:
        Allocate HW cipher key slot

    Input Parameters:
        uScheme:     DRM Scheme number
        uSessionID:  session ID
        uKeyID:      Content Key Unit ID
        uKeyLen:     Length of cipher key

    Output Parameters:
        puKeyIndex:  Index of cipher key slot
**************************************************************************/
HRESULT MV_DRM_KEY_AllocateKeySlot(UINT32 uScheme, UINT32 uSessionID, UINT32 uKeyID,
                                   UINT32 uKeyLen, UINT32 *puKeyIndex);

/***************************************************************************
    Description:
        Allocate HW cipher key slot pair, this function is mainly used for
        ensuring the HW cipher key of odd and even are physically adjacent
        as a requirement from DMX

    Input Parameters:
        uScheme:     DRM Scheme number
        uSessionID:  session ID
        uOddKeyID:   Odd Content Key Unit ID
        uEvenKeyID:  Even Content Key Unit ID
        uKeyLen:     Length of cipher key

    Output Parameters:
        puOddKeyIndex:   Index of odd cipher key slot
        puEvenKeyIndex:  Index of even cipher key slot
**************************************************************************/
HRESULT MV_DRM_KEY_AllocateKeySlotPair(UINT32 uScheme, UINT32 uSessionID,
                                       UINT32 uOddKeyID, UINT32 uEvenKeyID, UINT32 uKeyLen,
                                       UINT32 *puOddKeyIndex, UINT32 *puEvenKeyIndex);

/***************************************************************************
    Description:
        Allocate HW cipher IV slot

    Input Parameters:
        uScheme:     DRM Scheme number
        uSessionID:  session ID
        uKeyID:      Content Key Unit ID
        uIVLen:      Length of cipher IV

    Output Parameters:
        puIVIndex:  Index of cipher IV slot
**************************************************************************/
HRESULT MV_DRM_KEY_AllocateIVSlot(UINT32 uScheme, UINT32 uSessionID, UINT32 uKeyID,
                                  UINT32 uIVLen, UINT32 *puIVIndex);

/***************************************************************************
    Description:
        Allocate HW IV keyslot pair, this function is mainly used for
        ensuring the HW IV keyslot of odd and even are physically adjacent
        as a requirement from DMX

    Input Parameters:
        uScheme:     DRM Scheme number
        uSessionID:  session ID
        uOddKeyID:   Odd Content Key Unit ID
        uEvenKeyID:  Even Content Key Unit ID
        uIVLen:      Length of cipher IV key

    Output Parameters:
        puOddKeyIndex:   Index of odd cipher IV key slot
        puEvenKeyIndex:  Index of even cipher IV key slot
**************************************************************************/
HRESULT MV_DRM_KEY_AllocateIVSlotPair(UINT32 uScheme, UINT32 uSessionID,
                                      UINT32 uOddKeyID, UINT32 uEvenKeyID, UINT32 uIVLen,
                                      UINT32 *puOddKeyIndex, UINT32 *puEvenKeyIndex);

/***************************************************************************
    Description:
        Clear key loading into Content Key Unit

    Input Parameters:
        uScheme:     DRM Scheme number
        uSessionID:  session ID
        uKeyID:      Content Key Unit ID
        pbKeyData:   Clear key data
        uKeyDataLen: Length of clear key data

    Output Parameters:
         N/A
**************************************************************************/
HRESULT MV_DRM_KEY_LoadClearKey(UINT32 uScheme, UINT32 uSessionID, UINT32 uKeyID,
                                UINT8 *pbKeyData, UINT32 uKeyDataLen);

/***************************************************************************
    Description:
        Allocate HW cipher IV slot

    Input Parameters:
        uScheme:     DRM Scheme number
        uSessionID:  session ID
        uKeyID:      Content Key Unit ID
        pbIVData:    Clear IV data
        uIVDataLen:  Length of clear IV data

    Output Parameters:
        N/A
**************************************************************************/
HRESULT MV_DRM_KEY_LoadClearIV(UINT32 uScheme, UINT32 uSessionID, UINT32 uKeyID,
                               UINT8 *pbIVData, UINT32 uIVDataLen);
#endif /* __DRM_KEY_AGENT_API_H__ */
