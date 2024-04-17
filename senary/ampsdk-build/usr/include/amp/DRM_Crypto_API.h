/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2017 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated (“Synaptics”). The holder of this file shall treat all
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
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS,” AND
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
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS’ TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */
////////////////////////////////////////////////////////////////////////////////
//! \file DRM_Crypto_API.h
//! \brief MARVELL VIDEO Crypto library APIs
//!
//! Purpose:
//!
//!  Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef __DRM_CRYPTO_API_H__
#define __DRM_CRYPTO_API_H__
#include "com_type.h"
#include "DRM_Common.h"
#include "OSAL_api.h"

/*$cszhu: add for seperating RSA Padding*/
#define     SEPERATE_RSA_PADDING   1




#define CRYPTO_AES128_KEY_LEN  16
#define CRYPTO_AES192_KEY_LEN  24
#define CRYPTO_AES256_KEY_LEN  32

#define CRYPTO_AES128_BLK_LEN 16
#define CRYPTO_AES192_BLK_LEN 24
#define CRYPTO_AES256_BLK_LEN 32

#define CRYPTO_AES128_RN      10
#define CRYPTO_AES192_RN      12
#define CRYPTO_AES256_RN      14

#define MAX_IV_SIZE                16

#define CRYPTO_AES_MAX_BLK_LEN  CRYPTO_AES256_BLK_LEN
#define CRYPTO_AES_MAX_KEY_LEN  CRYPTO_AES256_KEY_LEN
#define CRYPTO_AES_MAX_RN           CRYPTO_AES256_RN

#define CRYPTO_C2_KEY_LEN 7
#define CRYPTO_C2_RN  10


////////////////////////////////////////////////////////////////////////////////
//! \file DRM_Crypto_API.h
//! \brief Head file of the DRM library
//!
//! Purpose:
//!    This file will be delivered to the customer with  the library libDRM.a
//!
//!  Note:
////////////////////////////////////////////////////////////////////////////////

typedef VOID* MV_CRYPTO_KEY_HANDLE;
typedef VOID* MV_CRYPTO_HASH_HANDLE;

typedef UINT32 rsaPublicKeyHandle;  //to be deleted


#define MV_CRYPTO_INVALID_HANDLE NULL

typedef enum tagCRYPTOKeyParameters
{
  MV_KEY_PARM,
  MV_KEY_MODE_ECB,
  MV_KEY_MODE_CBC,
  MV_KEY_MODE_CFB,
  MV_KEY_MODE_CTS,
  MV_EC_GROUP_PARM,
  MV_EC_PUBKEY_PARM,
  MV_EC_PRVKEY_PARM,
  MV_EC_AACS_CERT_PARM,
  MV_RSA_PUBKEY_PARM,
  MV_RSA_PRVKEY_PARM,
  MV_HASH_TYPE_SHA1,
  MV_HASH_TYPE_SHA256,
  MV_KEY_MODE_CTR,
  //To be added...
}MV_CRYPTO_PARM;


typedef enum tagCryptoAlgorithmID
{
  CRYPTO_ALG_AES128,
  CRYPTO_ALG_AES192,
  CRYPTO_ALG_AES256,
  CRYPTO_ALG_DEFAULT,
  CRYPTO_ALG_DES,
  CRYPTO_ALG_CSS,
  CRYPTO_ALG_OMAC,
  CRYPTO_ALG_SHA1,
  CRYPTO_ALG_SHA256,
  CRYPTO_ALG_AESH,
  CRYPTO_ALG_ECDSA_GP,
  CRYPTO_ALG_ECDSA_P256,
  CRYPTO_ALG_AACS_MK,
  CRYPTO_ALG_AACS_PROCK,
  CRYPTO_ALG_AACS_BD_BLK_KEY,
  CRYPTO_ALG_AACS_AESG,
  CRYPTO_ALG_RSA_PKCS1,
  CRYPTO_ALG_RSA_OAEP,
  CRYPTO_ALG_RSA_OAEP_SHA256,
  CRYPTO_ALG_CMAC,
  CRYPTO_ALG_RC4,
  CRYPTO_ALG_CBCMAC_WMDRM,
  CRYPTO_ALG_HMAC,
  CRYPTO_ALG_HMAC_SHA256,
  CRYPTO_ALG_C2,
  CRYPTO_ALG_C2H,
  CRYPTO_ALG_CPRM_C2G,
  CRYPTO_ALG_MD5,
  CRYPTO_ALG_RSA_PSS,
} MV_CRYPTO_ALG_ID;


enum
{
  BN_HEX,
  BN_HEX_CHAR,
  BN_DEC,
  BN_DEC_CHAR,
};
#define MAX_BN_DATA_SIZE 128
typedef struct tagBigNumberData
{
  UINT32  uType;
  UINT8   pbData[MAX_BN_DATA_SIZE];
  UINT32  uSize;
  UINT32  nNeg;
}MV_BN_DATA;

#define DECLARE_MV_BN_STR(_str, _type, _neg)\
{                     \
  _type,                  \
  _str,                 \
  sizeof(_str) - 1,           \
  _neg,                 \
}

#define DECLARE_MV_BN_HEXSTR(_str)      \
{                     \
  BN_HEX_CHAR,              \
  _str,                 \
  sizeof(_str) - 1,           \
  0,                    \
}

#define DECLARE_MV_BN_DECSTR(_str)      \
{                     \
  BN_DEC_CHAR,              \
  _str,                 \
  sizeof(_str) - 1,           \
  0,                    \
}

#define DECLARE_MV_BN_HEX(_arry, _len)    \
{                     \
  BN_HEX,                 \
  _arry,                  \
  _len,                 \
  0,                    \
}

typedef struct tagECDSAGPPublicKeyParameters
{
  MV_BN_DATA bnP;
  MV_BN_DATA bnA;
  MV_BN_DATA bnB;
  MV_BN_DATA bnX;
  MV_BN_DATA bnY;
  MV_BN_DATA bnR;
} MV_EC_GROUP_DATA;

typedef struct tagAACSHostCertificateAndPrivateKey
{
  MV_EC_GROUP_DATA ecGroup;
  UCHAR      pbHostCert[AACS_CERT_LEN];
  MV_BN_DATA     bnPrvKey;
} MV_AACS_CERT_KEY;



typedef struct tagDTCPIPECGroupAndPrivateKeys
{
    MV_EC_GROUP_DATA ecGroup;
    MV_BN_DATA     bnPrvKey;
}DTCPIP_EC_PARAM_KEY_CTX;


typedef struct tagAACSMediaKeyData32
{
  UCHAR   pbUMaskNr;
  UINT32  uUVNum;
  UCHAR   pbCVal[16];
} MV_AACS_MK_DATA;

//! \todo add message type
typedef enum tagCryptoMessageType
{
  CRYPTO_MSG_IMAGE,
  CRYPTO_MSG_NORMALMSG,
} MV_CRYPTO_MSG_TYPE;

//! \todo add certificate type
typedef enum tagCryptoCertificateType
{
  CRYPTO_CERT_AACS,
  CRYPTO_CERT_MARVELL,
} MV_CRYPTO_CERT_TYPE;


typedef enum tagCryptoKeyID
{
    CRYPTO_KEY_ID_OTP,
}MV_CRYPTO_KEY_ID;


typedef enum tagCSSAuthType
{
  CRYPTO_CSS_AUTH_KEY1 =0,//the key of host challenge
  CRYPTO_CSS_AUTH_KEY2 =1,//the key of drive challenge
  CRYPTO_CSS_AUTH_BUSKEY =2,// the Bus key
}MV_CRYPTO_CSS_AUTH_KEY;

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_INITIAL
//!
//! Description: MV_CRYPTO_INITIAL function will prepare all the keys Crypto API is needed.
//!
//! \param NO.
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_INITIAL();

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_CreateKey
//!
//! Description: MV_CRYPTO_CreateKey function will create Marvell DRM KEY Handle,
//! which will be used to decrypt cipher block or encrypt plain text.
//!
//!      After creating the handle with a specified encrypt algorithm, you should select a way to
//!       initialize the pbKeyData of it by calling MV_CRYPTO_ImportKey or MV_CRYPTO_GenKey
//!
//!         It should be released by calling  MV_CRYPTO_ReleaseKey function after usage.
//!
//! \param pKeyHandle (OUT): -- return the MV_CRYPTO_KEY_HANDLE
//! \param uAlgID (IN): -- type of cryptography algorithm for the Handle
//!
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_CreateKey(MV_CRYPTO_KEY_HANDLE *pKeyHandle,
                            MV_CRYPTO_ALG_ID uAlgID);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_ReleaseKey
//!
//! Description: MV_CRYPTO_ReleaseKey function will released the resources
//! occupied by the DRM Handle
//!
//!A Marvell DRM KEY Handle should be released by calling  the function
//!    when it wiil be never used again.
//!
//!\param pKeyHandle(IN):-- address of the MV_CRYPTO_KEY_HANDLE which is not going to be used again
//!
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_ReleaseKey(MV_CRYPTO_KEY_HANDLE *pKeyHandle);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_SetKeyParm
//!
//! Description: MV_CRYPTO_SetKeyParm function is used to specify the key of
//! a given Marvell DRM Handle
//!
//!
//! \param hKey (IN):-- Marvell DRM KEY Handle which is used to identify the cryptography algorithm
//!           specified by the user
//! \param pbData (IN): -- the plain text key specified by the user
//! \param uSize (IN): -- length of the plain text key specified by the user (how many bytes)
//! \param uParm (IN): -- specify the key parameter type
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_SetKeyParm(MV_CRYPTO_KEY_HANDLE hKey,
                             const UCHAR *pbData,
                             UINT uSize,
                             MV_CRYPTO_PARM uParm);


////////////////////////////////////////////////////////////////////////////////
//! \brief Retrieve the key type
//! \param hKey(IN): The key handle
//! \param pbType(OUT): Pointer to the key type
//! \return Error Code
//! \retval S_OK    Succeed
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_GetKeyType(MV_CRYPTO_KEY_HANDLE hKey,
                             UINT32 *pbType);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_GetOTPKey
//!
//! Description: MV_CRYPTO_GetOTPKey function is used to get the OTP key to a Marvell
//!                 Crypto Key Handle
//!
//! \param hKey (OUT): -- Marvell DRM Handle which is used to identify the cryptography
//!                 algorithm specified by the user
//!
//! \return Return:      error code : S_OK if ok other: pls see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_GetOTPKey(MV_CRYPTO_KEY_HANDLE hKey);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_GetProductionKey
//!
//! Description: MV_CRYPTO_GetProductionKey function is used to get the production key
//!             to a Marvell Crypto Key Handle
//!
//! \param hKey (OUT): -- Marvell DRM Handle which is used to identify the cryptography
//!                 algorithm specified by the user
//! \param uKeyID (IN): -- Key ID of the production key
//!
//! \return Return:      error code : S_OK if ok other: pls see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_GetProductionKey(MV_CRYPTO_KEY_HANDLE hKey, MV_CRYPTO_KEY_ID uKeyID);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_ImportKey
//!
//! Description: MV_CRYPTO_ImportKey function is used to import key data from
//!                 MV_CRYPTO_DOB structure to a Marvell DRM Key Handle.
//!
//! \param hKey (OUT): -- Marvell DRM Handle which is used to identify the cryptography algorithm
//!           specified by the user
//! \param pszPWD (IN) -- the password that used to decrypt the key. if no password
//!                     has been specified when the DOB is created,set it to NULL.
//! \param uPWDLength (IN) -- the length of the password.
//! \param pbData (IN) -- address of the DOB data.
//! \param uDataLength (IN) -- length of the DOB data.
//!
//! \return Return:      error code : S_OK if ok other: pls see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_ImportKey(MV_CRYPTO_KEY_HANDLE* phKeyRes,
                            UCHAR * pszPWD,
                            UINT32 uPWDLength,
                            VOID * pbData,
                            UINT32 uDataLength);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_ExportKey
//!
//! Description: MV_CRYPTO_ExportKey function is used to exprot key data from key handle  to a
//!                    MV_CRYPTO_DOB structure.
//!
//! \param hKey (IN): -- Marvell DRM Handle which is used to identify the cryptography algorithm
//!           specified by the user
//! \param ProtectKeyID (IN) -- specify the Key ID that used to encrypt the DOB
//! \param pszPWD (IN) -- optional, specify a password to protect the DOB
//!                     if it is not necessary, set it to NULL
//! \param uPWDLength (IN) -- the length of the password.
//! \param pbData (OUT) -- address of the DOB data.
//! \param puSize (OUT) -- address of the param store the length of the buffer
//!                     pointed by pbData. When pbData is set to NULL, the function
//!                     will return set it to the minimal size of buffer pointed by pbData.
//!
//! \return Return:      error code : S_OK if ok other: pls see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_ExportKey(MV_CRYPTO_KEY_HANDLE hKey ,
                            MV_CRYPTO_KEY_ID ProtectKeyID,
                            UCHAR * pszPWD,
                            UINT32 uPWDLength,
                            VOID * pbData,
                            UINT32* puSize);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_ImportMsg
//!
//! Description: MV_CRYPTO_ImportMsg function is used to import Message data from
//!                 MV_CRYPTO_DOB structure to a specified buffer.
//!
//! \param pbMsg (OUT): -- address of the buffer to contain the decrypted message.
//!                     the buffer has to be allocate by the user. If set pbMsg to NULL,
//!                     function will return the minimal buffer size that is required
//! \param puMsgSize (OUT): -- length of the decrypted message.
//!                     Before calling this function, the caller SHALL set this parameter to
//!                     the max size of the pbMsg buffer.
//!                     Unpon return, this parameter contains the bytes of decrypted data
//!                     When pbMsg is set to NULL, the function will set puMsgSize to
//!                     the minimal buffer size required to contain the imported message.
//! \param uMsgType (IN) -- specify the message type.
//! \param pszPWD (IN) -- the password that used to decrypt the key. if no password
//!                     has been specified when the DOB is created,set it to NULL.
//! \param uPWDLength (IN) -- the length of the password.
//! \param pbData (IN) -- address of the DOB data.
//! \param uDataLength (IN) -- length of the DOB data.
//!
//! \return Return:      error code : S_OK if ok other: pls see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_ImportMsg(UCHAR *pbMsg,
                            UINT32 *puMsgSize,
                            MV_CRYPTO_MSG_TYPE uMsgType,
                            UCHAR * pszPWD,
                            UINT32 uPWDLength,
                            VOID * pbData,
                            UINT32 uDataLength);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_ExportMsg
//!
//! Description: MV_CRYPTO_ExportMsg function is used to exprot Message data from
//!                     a buffer to a MV_CRYPTO_DOB structure.
//!
//! \param pbMsg (IN): --address of the buffer containing the message plaintext.
//! \param uMsgLength (IN) -- length of the input Message
//! \param uMsgType (IN) --  the message type.
//! \param ProtectKeyID (IN) -- specify the Key ID that used to encrypt the DOB
//! \param pszPWD (IN) -- optional, specify a password to protect the DOB
//!                     if it is not necessary, set it to NULL
//! \param uPWDLength (IN) -- the length of the password.
//! \param pbData (OUT) -- address of the DOB data.
//! \param puSize (OUT) -- address of the param store the length of the buffer
//!                     pointed by pbData. When pbData is set to NULL, the function
//!                     will set puSize to the minimal buffer size required to contain
//!                     the exported message.
//!
//! \return Return:      error code : S_OK if ok other: pls see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_ExportMsg(UCHAR *pbMsg,
                            UINT32 uMsgLength,
                            MV_CRYPTO_MSG_TYPE uMsgType,
                            MV_CRYPTO_KEY_ID ProtectKeyID,
                            UCHAR * pszPWD,
                            UINT32 uPWDLength,
                            UCHAR * pbData,
                            UINT32* puSize);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_ImportCert
//!
//! Description: MV_CRYPTO_ImportCert function is used to import Certificate data from
//!                 MV_CRYPTO_DOB structure to a specified buffer.
//!
//! \param pbCert (OUT): -- address of the buffer to contain the decrypted Certificate.
//!                     the buffer has to be allocate by the user. If set pbCert to NULL,
//!                     function will return the minimal buffer size that is required
//! \param puCertSize (OUT): -- length of the decrypted message.
//!                     Before calling this function, the caller SHALL set this parameter to
//!                     the max size of the pbCert buffer
//!                     Unpon return, this parameter contains the bytes of decrypted data
//!                     When pbCert is set to NULL, the function will set puMsgSize to
//!                     the minimal buffer size required to contain the imported message.
//! \param uCertType (IN) -- specify the Certificate type.
//! \param pszPWD (IN) -- the password that used to decrypt the key. if no password
//!                     has been specified when the DOB is created,set it to NULL.
//! \param uPWDLength (IN) -- the length of the password.
//! \param pbData (IN) -- address of the DOB data.
//! \param uDataLength (IN) -- length of the DOB data.
//!
//! \return Return:      error code : S_OK if ok other: pls see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_ImportCert(UCHAR *pbCert,
                            UINT32 *puCertSize,
                            MV_CRYPTO_CERT_TYPE uCertType,
                            UCHAR * pszPWD,
                            UINT32 uPWDLength,
                            VOID * pbData,
                            UINT32 uDataLength);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_ExportCert
//!
//! Description: MV_CRYPTO_ExportCert function is used to exprot Certificate data from
//!                     a buffer to a MV_CRYPTO_DOB structure.
//!
//! \param pbCert (IN): --address of the buffer containing the Certificate plaintext.
//! \param uCertLength (IN) -- length of the input Certificate
//! \param uCertType (IN) -- specify the Certificate type
//! \param ProtectKeyID (IN) -- specify the Key ID that used to encrypt the DOB
//! \param pszPWD (IN) -- optional, specify a password to protect the DOB
//!                     if it is not necessary, set it to NULL
//! \param uPWDLength (IN) -- the length of the password.
//! \param pbData (OUT) -- address of the DOB data.
//!                     the buffer has to be allocate by the user. If set pbData to NULL,
//!                     function will return the minimal buffer size that is required
//! \param puSize (OUT) -- address of the param store the length of the buffer
//!                     pointed by pbData. When pbData is set to NULL, the function
//!                     will set puSize to the minimal buffer size required
//!
//! \return Return:      error code : S_OK if ok other: pls see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_ExportCert(UCHAR *pbCert,
                            UINT32 uCertLength,
                            MV_CRYPTO_CERT_TYPE uCertType,
                            MV_CRYPTO_KEY_ID ProtectKeyID,
                            UCHAR * pszPWD,
                            UINT32 uPWDLength,
                            VOID * pbData,
                            UINT32* puSize);


////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_GenKey
//!
//! Description: MV_CRYPTO_GenKey function is used to generate a random the pbKeyData
//!  for a given Marvell DRM Handle
//!
//!
//! \param hKey (IN):-- Marvell DRM KEY Handle which is used to identify the cryptography algorithm
//!           specified by the user
//!
//!
//! \return Return:      error code : S_OK if ok other: pls see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_GenKey(MV_CRYPTO_KEY_HANDLE hKey );


////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_DeriveKey
//!
//! Description: MV_CRYPTO_DeriveKey function will derive content  Key Data from encrypted data
//!
//!
//! \param hKey (IN): --Marvell DRM KEY Handle which is used to identify the cryptography algorithm
//!           specified by the user
//! \param pbKeyData (IN): -- data that contain the encrypted content Key
//! \param uSize (IN): -- size of the pbKeyData (how many bytes)
//! \param uDeriveAlgID(IN):-- Derive algorithm of the Derive.
//!                            such as CRYPTO_ALG_AACS_MK, CRYPTO_ALG_AACS_AESG
//! \param uKeyType (IN): -- key type of the key that is going to be decrypted.
//! \param phResult (OUT): -- the Marvell DRM KEY Handle is returned as result of
//!                           the DeriveKey, which contains the
//!           key that has been decrypted.
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_DeriveKey(MV_CRYPTO_KEY_HANDLE hKey , const UCHAR *pbKeyData, UINT uSize,
                         MV_CRYPTO_ALG_ID uALGID, MV_CRYPTO_ALG_ID uAlgID,
                         MV_CRYPTO_KEY_HANDLE* phResult);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_Decrypt
//!
//! Description: MV_CRYPTO_Decrypt function will decrypt the data with the pbKeyData carried by the
//!       Marvell DRM Handle and cryptography algorithm specified by it.
//!
//!
//! \param hKey (IN): -- Marvell DRM Handle which is used to identify the cryptography algorithm
//!           specified by the user
//! \param pbDataEnc  (IN): -- address of data that is encrypted and is going to be decrypted
//! \param uEncSize   (IN): -- size of the data that is encrypted
//! \param pbDataDec  (OUT): -- address where the decryped data going to be saved
//! \param puDecSize  (OUT): -- size of the data that is  decrypted
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_Decrypt(MV_CRYPTO_KEY_HANDLE hKey, const UCHAR *pbDataEnc,
                       UINT32 uEncSize,
                       UCHAR *pbDataDec,
                       UINT32* puDecSize);

////////////////////////////////////////////////////////////////////////////////
//! \brief Encrypt data with the specific key handle
//! \param hKey(IN):          The handle of the key to be used to encrypt the data
//! \param pbDataDec(IN):     The pointer of the buffer containing the plain text
//! \param uDecSize(IN):      Number of bytes of the plain text
//! \param pbDataEnc(OUT):    The pointer of the buffer containing the cipher text
//! \param puEncSize(IN/OUT): Before calling this function, the caller SHALL set this parameter to
//!                             the max size of the pbDataEnc
//!             Unpon return, this parameter contains the bytes of the cipher text generated
//! \return                 Error Code
//! \retval S_OK            Succeed
//! \retval E_INVALIDARG    Invalid input argument
//! \retval E_NOTIMPL       The specific algorithm has no be implemented yet
//! \note   Before calling this function, a valid key object SHALL be created
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_Encrypt(MV_CRYPTO_KEY_HANDLE hKey, const UCHAR *pbDataDec,
              UINT32 uDecSize,
              UCHAR *pbDataEnc,
              UINT32* puEncSize);


////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_CreateHash
//!
//! Description: MV_CRYPTO_CreateHash function will create Marvell DRM Hash Handle
//!       which will be used to caculate hash value. when it is created,
//!       a specified hash algorithm is associated to it.
//!       which is specified by the user.
//!
//!
//! \param phHandle (OUT): -- return the address of MV_CRYPTO_HASH_HANDLE
//! \param uAlgID (IN): -- type of hash algorithm specified by the user, for the Handle
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_CreateHash( MV_CRYPTO_ALG_ID algID,
          MV_CRYPTO_KEY_HANDLE hKey,
                               MV_CRYPTO_HASH_HANDLE* phHash);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_HashData
//!
//! Description: MV_CRYPTO_HashData function will caculate the data's hash value
//!              with the hash algorithm
//!specified by the Marvell DRM Handle
//!we can used it when the data going to be hashed is just one whole block of data.
//!And there is no necessary to
//!       call function MV_CRYPTO_InitHashData.
//!
//! \param hHash (IN): -- Marvell DRM Hash Handle which is used to identify the digest algorithm
//!           specified by the user
//! \param pbData (IN): -- address of data that is going to be hashed
//! \param uSize (IN): -- size of the data that is going to be hashed
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_HashData(MV_CRYPTO_HASH_HANDLE hHash, UCHAR *pbData,
                           UINT uSize);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_InitHashData
//!
//! Description: MV_CRYPTO_InitHashData function will initialize the Marvell DRM Hash Handle
//!       we should use function MV_CRYPTO_InitHashData,
//!       MV_CRYPTO_UpdateHashData, MV_CRYPTO_FinalHashData
//!       sequential to do the digest
//!
//! \param hHash (IN): -- Marvell DRM Hash Handle which is used to identify the digest algorithm
//!           specified by the user
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_InitHashData(MV_CRYPTO_HASH_HANDLE hHash);


////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_UpdateHashData
//!
//! Description: MV_CRYPTO_UpdateHashData function will addd new data bolock to caculate
//!              the data's hash value  with the hash algorithm
//!              specified by the Marvell DRM Handle
//!
//! \param hHash (IN): -- Marvell DRM Hash Handle which is used to identify the digest algorithm
//!           specified by the user
//! \param pbData (IN): -- address of data that is going to be hashed
//! \param uSize (IN): -- size of the data that is going to be hashed
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_UpdateHashData(MV_CRYPTO_HASH_HANDLE hHash,
                               UCHAR *pbData,
                               UINT uSize);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_FinalHashData
//!
//! Description: MV_CRYPTO_FinalHashData function will be called after all the blocks of
//!              data has been updated.
//!
//! \param hHash (IN): -- Marvell DRM Hash Handle which is used to identify the digest algorithm
//!           specified by the user
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_FinalHashData(MV_CRYPTO_HASH_HANDLE hHash);


HRESULT MV_CRYPTO_ReleaseHash(MV_CRYPTO_HASH_HANDLE* phHash);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_SignHash
//!
//! Description: MV_CRYPTO_SignHash function will sign the data with the key related with the
//!              hKey specified by the user
//!
//! \param hKey (IN): -- Marvell DRM Handle which is used to identify the cryptography algorithm
//!           specified by the user
//! \param hHash (IN):-- Marvell DRM hash Handle which is used to identify the hash algorithm
//!           specified by the user
//! \param pSignData (OUT): -- address of data that is going to be signed
//! \param puSize (IN): -- size of the data that is going to be signed
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_SignHash(MV_CRYPTO_KEY_HANDLE hKey,
                           MV_CRYPTO_HASH_HANDLE hHash,
                           UCHAR *pbSignData, UINT* puSize );



////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_Sign
//!
//! Description: MV_CRYPTO_SignHash function will sign the data with the key related with the
//!              hKey specified by the user
//!
//! \param hKey (IN): -- Marvell DRM Handle which is used to identify the cryptography algorithm
//!           specified by the user
//! \param pbDigest (IN):-- Digest value
//! \param uDigestLen(IN): -- Digest Len
//! \param pSignData (OUT): -- address of data that is going to be signed
//! \param puSize (IN): -- size of the data that is going to be signed
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_Sign(MV_CRYPTO_KEY_HANDLE hKey,
                           UCHAR* pbDigest,
                           UINT   uDigestLen,
                           UCHAR *pbSignData,
                           UINT* puSize );

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_GetHashValue
//!
//! Description: MV_CRYPTO_GetHashValue will copy the digest to user's buffer
//!
//! \param hHash (IN):-- Marvell DRM hash Handle which is used to identify the hash algorithm
//!                     specified by the user
//! \param pData (OUT): -- address of user's buffer
//! \param puSize (IN/OUT): -- max size of the buffer,if the buffer size is shorter than the digest
//!                         the first *puSize bytes of the digest will be return.
//!             on return, the *puSize contains the length of data returned
//!
//! \return Return:      Error Code
//! \retval S_OK            Succeed
//! \retval E_INVALIDARG    Invalid input argument
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_GetHashValue(MV_CRYPTO_HASH_HANDLE hHash,
                           UCHAR *pbData, UINT* puSize );

HRESULT MV_CRYPTO_SetHashValue(MV_CRYPTO_HASH_HANDLE hHash,
                               UCHAR *pbData, UINT32 uSize );

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_VerifySig
//!
//! Description: MV_CRYPTO_VerifySig function will verify the signature
//!
//! \param hKey (IN): -- Marvell DRM Handle which is used to identify the cryptography algorithm
//!           specified by the user
//! \param hHash (IN):-- Marvell DRM hash Handle which is used to identify the hash algorithm
//!           specified by the user
//! \param pSignData (IN): -- address of data that is going to be verified
//! \param pbResult (OUT): -- result of the verification TRUE if it successes, FALSE if it fails
//! \param uSize (IN): -- size of the data that is going to be verified
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_VerifySig(MV_CRYPTO_KEY_HANDLE hKey,
                            MV_CRYPTO_HASH_HANDLE hHash,
                            const UCHAR *pbSignData,
                            UINT uSize,
                            BOOL *pbResult);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_CMAC
//!
//! Description: MV_CRYPTO_CMAC function will calculate the CMAC of a given data buffer.
//!
//! \param hKey (IN): -- Marvell DRM Handle which is used to identify the cryptography algorithm
//!           specified by the user
//! \param pbData (IN): -- address of data that is going to be verified
//! \param uSize (IN): -- size of the data pointed by pbData
//! \param pCMAC (IN/OUT): -- address of the buffer to contain the calculated CMAC.it should be no
//!                         less than 16bytes(128bit).
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_CMAC(MV_CRYPTO_KEY_HANDLE hKey,
                             const UCHAR *pbData,
                             UINT uSize,
                             UCHAR *pCMAC);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_VerifyCMAC
//!
//! Description: MV_CRYPTO_VerifyCMAC function will verify the CMAC of a given data buffer.
//!
//! \param hKey (IN): -- Marvell DRM Handle which is used to identify the cryptography algorithm
//!           specified by the user
//! \param pbData (IN): -- address of data that is going to be verified
//! \param uSize (IN): -- size of the data pointed by pbData
//! \param pCMAC (IN): -- address of the CMAC data that is going to be verified,
//!                         it should contain 16bytes(128bit) CMAC value.
//! \param pbResult (OUT): -- result of the verification TRUE if it successes, FALSE if it fails
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_VerifyCMAC(MV_CRYPTO_KEY_HANDLE hKey,
                            const UCHAR *pbData,
                            UINT uSize,
                            UCHAR *pCMAC,
                            BOOL *pbResult);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_CRC32Encode
//!
//! Description: MV_CRYPTO_CRC32Encode function will calculate the CRC value of a given data buffer.
//!
//! \param pbData (IN): -- address of data that is going to be verified, it should not be NULL
//! \param uSize (IN): -- size of the data pointed by pbData, it can be 0
//! \param puResult (OUT): -- address of the variable to contain the calculated CRC value
//!
//! \return Return:
//! \retval S_OK        Succeed
//! \retval E_INVALIDARG    Input argument is invalid
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_CRC32(UCHAR* pbData, UINT uSize, UINT * puResult);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_TraceKey
//!
//! Description: MV_CRYPTO_TraceKey function will print out the key value
//!
//! \param hKey (IN): -- Marvell DRM Handle which is used to identify the cryptography algorithm
//!           specified by the user
//! \param pszKeyName (IN): -- name of the key, user can specify it for debug usage.
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_TraceKey(MV_CRYPTO_KEY_HANDLE hKey, const UCHAR* pszKeyName);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_ROM_Decrypt
//!
//! Description: MV_CRYPTO_TraceKey function will print out the key value
//!
//! \param hKey (IN): -- Marvell DRM Handle which is used to identify the cryptography algorithm
//!           specified by the user
//! \param pszKeyName (IN): -- name of the key, user can specify it for debug usage.
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_ROM_Decrypt   ( UINT32 uKeyid,
            const UCHAR *   pbDataEnc,
            UINT32    uEncSize,
            UCHAR *   pbDataDec,
            UINT32 *    puDecSize
          );

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_UpdateFIGOTK
//!
//! Description: the function will update the FIGO title key using the specified one
//!
//! \param hFigoDrv (IN): -- FIGO handle
//! \param hTK (IN): -- handle of the title key
//! \param uSyncPoint(IN): -- the Sync Point where new title key take affect
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT  MV_CRYPTO_UpdateFIGOTK(HANDLE          hFigoDrv,
                                UINT                     uStrmID,
                                MV_CRYPTO_KEY_HANDLE    hTK,
                 const UCHAR*       pbTK,
                 UINT           uTKSize,
                 UINT32         uSyncPointt);

HANDLE MV_CRYPTO_CreateFigoHandle(UINT uTraceLevel,
                                  UCHAR*   pbBIUImageShadow,
                                  UINT     uBIUImageShadowSize);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_InitFIGO
//!
//! Description: the function will initialize the FIGO
//!
//! \param hFigoDrv (IN): -- FIGO handle
//! \param hTK (IN): -- handle of the title key that is used to initialize the FIGO
//! \param pInDataBuf(IN): -- input Circular buffer handle
//! \param pOutDataBuf(IN): -- output Circular buffer handle
//! \param pbEVFIFOBase(IN): -- The output event ddr FIFO base address
//!             it should be allocated and freed by caller.It MUST be non-cacheable memory
//! \param uEVFifoSize(IN): -- The maximum size of output event ddr FIFO
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_InitFIGO(HANDLE hFigoDrv,
               UCHAR*   pbEVFIFOBase,
               UINT     uEVFifoSize);

HRESULT MV_CRYPTO_StopFIGO( HANDLE hFigoDrv );

HRESULT MV_CRYPTO_InitFIGOStream(HANDLE hFigoDrv,
                 UINT uStrmID,
                 MV_CC_HANDLE_CBufCtrl_t* pInDataBuf,
                 MV_CC_HANDLE_CBufCtrl_t* pOutDataBuf);

HRESULT MV_CRYPTO_StartFIGOStream(HANDLE hFigoDrv, UINT uStrmID);

HRESULT MV_CRYPTO_FlushFIGOStream(HANDLE hFigoDrv,
                 UINT uStrmID,
                 MV_CC_HANDLE_CBufCtrl_t* pInDataBuf);

HRESULT MV_CRYPTO_StopFIGOStream(HANDLE hFigoDrv,
               UINT uStrmID,
               MV_CC_HANDLE_CBufCtrl_t* pInDataBuf);


HRESULT MV_CRYPTO_UpdateFIGOStream(HANDLE hFigoDrv,
                   UINT  uStrmID,
                   MV_CC_HANDLE_CBufCtrl_t* pInDataBuf,
                   MV_CC_HANDLE_CBufCtrl_t* pOutDataBuf,
                   UINT* puSizeProc,
                                   UINT* puIsFIGOStopped);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_CSS_DecryptDiscKey
//!
//! Description: the function will decrypt the disc key of a DVD.
//! Algorithm:  Decryption of the disc key with player keys: try to decrypt the disc key
//!       from every position with every player key.
//!       if the function fails to decrypt a right disc key, the reason could be:
//!         1. the device key used is revoked.
//!         2. the Encrypted disc data is not valid
//!
//!
//! \param hKey (IN):-- Marvell DRM KEY Handle which is used to identify the cryptography algorithm
//!           specified by the user
//! \param pbEncDiscKey (IN): -- address of the encrypted disc key data buffer
//! \param uKeySize (IN): -- : -- size of the encrypted disc key data, the size should be 2048
//!
//!
//! \return Error code
//! \retval S_OK        Succeed
//! \retval E_INVALIDARG    Input argument is invalid
//! \retval E_FAIL        Failed to decrypt the right disc key
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_CSS_DecryptDiscKey( MV_CRYPTO_KEY_HANDLE hKey, UCHAR* pbEncDiscKey,
                                      UINT uKeySize, UCHAR* pbPlayerKey, UINT uPlayerKeySize);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_CSS_DecryptTitleKey
//!
//! Description: the function will decrypt the title key of a DVD.
//!
//!
//! \param hKey (IN): --Marvell DRM KEY Handle which is used to identify the cryptography algorithm
//!           specified by the user
//! \param pbEncTitleKey (IN): -- address of the encrypted title key data buffer
//! \param uKeySize (IN): -- : -- size of the encrypted title key data, the size should be 5
//!
//!
//! \return Error code
//! \retval S_OK        Succeed
//! \retval E_INVALIDARG    Input argument is invalid
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_CSS_DecryptTitleKey( MV_CRYPTO_KEY_HANDLE hKey,
                                       UCHAR* pbEncTitleKey, UINT uKeySize);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_CSS_DecodeDVD
//!
//! Description: the function will decrypt the title key of a DVD.
//!
//!
//! \param hKey (IN): --Marvell DRM KEY Handle which is used to identify the cryptography algorithm
//!           specified by the user
//! \param pbData (IN/OUT): -- address of the encrypted DVD data buffer, the buffer will be
//!               filled with decrypted data after the execution the the function
//! \param uSize (IN): -- : size of the encrypted DVD VOB data in the pbData buffer
//!
//!
//! \return Error code
//! \retval S_OK        Succeed
//! \retval E_INVALIDARG    Input argument is invalid
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_CSS_DecodeDVD(MV_CRYPTO_KEY_HANDLE hKey, UCHAR* pbData, UINT uSize);

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_CSS_DecryptAuthKey
//!
//! Description: the function will decrypt the
//!
//!
//! \param hKey (IN): --Marvell DRM KEY Handle which is used to identify the cryptography algorithm
//!           specified by the user
//! \param Auth_Key (IN): -- the Authentication key type to be decrypted.
//!             CRYPTO_CSS_AUTH_KEY1 :the key of host challenge
//!             CRYPTO_CSS_AUTH_KEY2 :the key of drive challenge
//!             CRYPTO_CSS_AUTH_BUSKEY : the Bus key
//! \param uVariant (IN): -- the authentication variant chosen by drive ,between 0 and 31.
//!
//! \param pbChallenge (IN): -- address of the challenge
//! \param pbkey (IN/OUT): -- : -- size of the buffer to carry the decryption result
//!
//! \return Error code
//! \retval S_OK        Succeed
//! \retval E_INVALIDARG    Input argument is invalid
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_CSS_DecryptAuthKey( MV_CRYPTO_KEY_HANDLE hKey, MV_CRYPTO_CSS_AUTH_KEY Auth_Key,
                   UINT uVariant,UCHAR const* pbChallenge,UCHAR *pbkey);

////////////////////////////////////////////////////////////////////////////////
//! \brief  Encrypt the Title Key data
//!
//! \param  hFigoDrv(IN): Driver handle
//! \param uRKEKID(IN):   Key ID of the KEK to encrypt input title key
//!             \sa MV_DRMLIB_ROMID2KEY
//! \param pbTitleKey(IN):  Pointer the sub key to be used to encrypt the image data directly
//! \param uTitleKeySize(IN): Size of input sub key (For Berlin SOC ,
//!                           this argument should be always 16)
//!
//
//! \return Error code : MV_FIGODRV_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
UINT32 MV_DRMLIB_EncryptTitleKey( HANDLE  hFigoDrv,
                UINT32  uRKEKID,
                 UINT8* pbTitleKey,
                 UINT32 uTitleKeySize );

void MV_CRYPTO_Hex2Bin(const char *pszHex, UCHAR* pbyBin, INT nMaxBinLen);



HRESULT MV_CRYPTO_SetHashParm(MV_CRYPTO_HASH_HANDLE hHash,
                const UCHAR *pbData,
                UINT uSize,
                MV_CRYPTO_PARM uParm);

HRESULT MV_CRYPTO_GetHashParm(MV_CRYPTO_HASH_HANDLE hHash,
                const UCHAR *pbData,
                UINT uSize,
                MV_CRYPTO_PARM uParm);
HRESULT MV_CRYPTO_GetKeyParm(MV_CRYPTO_KEY_HANDLE hKey,
                             MV_CRYPTO_PARM uParm,
                             UCHAR *pbKeyData,
                             UINT32 *uSize
                             );

void XOR(UCHAR* pbDst, const UCHAR* pbSrc, UINT32 uSize);

HRESULT MV_CRYPTO_GetKeyDataAddr(MV_CRYPTO_KEY_HANDLE hKeyHandle, UINT8 **pKeyData);
HRESULT MV_CRYPTO_MakeKey(MV_CRYPTO_KEY_HANDLE hKey);
HRESULT MV_CRYPTO_GetTKData(MV_CRYPTO_KEY_HANDLE hTK, UINT8 **pKeyData);
HRESULT MV_AACS_EC_BASE_POINT_Mul(MV_CRYPTO_KEY_HANDLE hKey, UINT8 * pbOutPoint, UINT32 *puSize,
                                  UINT8 * pbScalar, UINT32 uScalarSize);
HRESULT MV_AACS_EC_POINT_Mul(MV_CRYPTO_KEY_HANDLE hKey, UINT8 * pbInPoint, UINT32 uDataSize,
                         UINT8 * pbOutPoint, UINT32 *puSize, UINT8* pbScalar, UINT32 uScalarSize);

UINT32 DOB_SIZE_OF_DATA(UINT32 uDataSize);
HRESULT MV_CRYPTO_SetHashState(MV_CRYPTO_HASH_HANDLE hHash, UINT8* pbState);
HRESULT MV_CRYPTO_GetHashState(MV_CRYPTO_HASH_HANDLE hHash, UINT8* pbState);
HRESULT MV_CRYPTO_CBCMAC_Invert(MV_CRYPTO_HASH_HANDLE hHash,
                UCHAR *pbData, UINT32* puSize );

HRESULT MV_EC_Decrypt_WMDRM(UINT8 * pbDataEnc, UINT8 * pbDataDec, MV_CRYPTO_KEY_HANDLE pKeyHandle);
HRESULT MV_EC_Encrypt_WMDRM(UINT8 * pbDataDec, UINT8 * pbDataEnc, MV_CRYPTO_KEY_HANDLE pKeyHandle);
HRESULT MV_CRYPTO_WMDRM_InitFIGO( HANDLE hFigoDrv,
                            UCHAR*   pbEVFIFOBase,
                            UINT     uEVFifoSize);

HRESULT MV_CRYPTO_WMDRM_StopFIGO( HANDLE hFigoDrv);
////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    MV_CRYPTO_InitFIGOStream
//!
//! Description: the function will initialize the FIGO
//!
//! \param hFigoDrv (IN): -- FIGO handle
//! \param hTK (IN): -- handle of the title key that is used to initialize the FIGO
//! \param pInDataBuf(IN): -- input Circular buffer handle
//! \param pOutDataBuf(IN): -- output Circular buffer handle
//! \param pbEVFIFOBase(IN): -- The output event ddr FIFO base address
//!                      it should be allocated and freed by caller.It MUST be non-cacheable memory
//! \param uEVFifoSize(IN): -- The maximum size of output event ddr FIFO
//!
//! \return Return:      error code : S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
HRESULT MV_CRYPTO_WMDRM_InitFIGOStream(HANDLE hFigoDrv,
                                 UINT uStrmID,
                                 MV_CC_HANDLE_CBufCtrl_t* pInDataBuf,
                                 MV_CC_HANDLE_CBufCtrl_t* pOutDataBuf);

HRESULT MV_CRYPTO_WMDRM_StartFIGOStream(HANDLE hFigoDrv, UINT uStrmID);
HRESULT MV_CRYPTO_WMDRM_FlushFIGOStream(HANDLE hFigoDrv,
                                  UINT uStrmID,
                                  MV_CC_HANDLE_CBufCtrl_t* pInDataBuf);
HRESULT MV_CRYPTO_WMDRM_UpdateFIGOStream(HANDLE hFigoDrv ,
                                   UINT  uStrmID,
                                   MV_CC_HANDLE_CBufCtrl_t* pInDataBuf,
                                   MV_CC_HANDLE_CBufCtrl_t* pOutDataBuf,
                                   UINT* puSizeProc,
                                   UINT* puIsFIGOStopped);
HRESULT MV_CRYPTO_WMDRM_StopFIGOStream(HANDLE hFigoDrv,
                                 UINT uStrmID,
                                 MV_CC_HANDLE_CBufCtrl_t* pInDataBuf);
HRESULT MV_CRYPTO_WMDRM_PushKeySeed( HANDLE hFigoDrv,    UINT uStrmID, UCHAR* pKeySeed,
                                     UINT uKeySeedSize, UINT uSyncPoint);
HRESULT MV_CRYPTO_WMDRM_UpdateFIGOTK(HANDLE                   hFigoDrv,
                               UINT                     uStrmID,
                               MV_CRYPTO_KEY_HANDLE     hTK,
                               const UCHAR*             pbCKHash,
                               UINT                     uCKHashSize,
                               UINT32                   uSyncPointt);

HRESULT MV_CRYPTO_SPHAL_InitFIGO( HANDLE hFigoDrv);
HRESULT MV_CRYPTO_SPHAL_DestroyFIGO( HANDLE hFigoDrv);
HRESULT MV_CPRM_C2_G(UINT8* pbData1, UINT8* pbData2, UINT8* pbResult);



/*$cszhu: Export Generic RSA Padding function to outside*/

#if SEPERATE_RSA_PADDING
/*RSA Padding Functions*/
UINT32 MV_emsa_pkcs1_cipher_Encode( UINT8 *M,  UINT32 mLen, UINT8 *EM,  UINT32 emLen);


UINT32 MV_emsa_pkcs1_cipher_Verify( UINT8 *EM, UINT32 emLen, UINT8 *M,  UINT32* mLen);


UINT32 MV_emsa_pkcs1_md5sha1_Encode( UINT8 *M,  UINT32 mLen, UINT8 *EM,  UINT32 emLen);


UINT32 MV_emsa_pkcs1_md5sha1_Verify( UINT8 *EM, UINT32 emLen, UINT8 *M,UINT32 mLen, BOOL *pbResult);


HRESULT MV_emsa_pkcs1_Encode(const UINT8 *M, const INT32 mLen, UINT8 *EM, const INT32 emLen);


UINT32 MV_emsa_oaep_Encode( UINT8 *M,  UINT32 mLen, UINT8 *EM,  UINT32 emLen);

UINT32 MV_emsa_oaep_Verify(UINT8 *EM, UINT32 emLen, UINT8 *M,  UINT32* mLen);

UINT32 MV_emsa_pss_Encode( UINT8 *M,  UINT32 mLen, UINT8 *EM,  UINT32 emBits);

HRESULT MV_Crypto_RSAPublicBlock (UINT8 *output, UINT32 *outputLen, UINT8 *input, UINT32 inputLen,
                                  UINT8*u_e, UINT8* u_n );

HRESULT MV_Crypto_RSAPrivateBlock (UINT8 *output, UINT32 *outputLen, UINT8 *input, UINT32 inputLen,
                                   UINT8*u_d, UINT8* u_n );

#endif


HRESULT MV_CRYPTO_Verify(MV_CRYPTO_KEY_HANDLE hKey,
                         UCHAR* pbDigest,
                         UINT  uDigestLen,
                         const UCHAR *pbSignData,
                         UINT uSize,
                         BOOL *pbResult);
#endif




