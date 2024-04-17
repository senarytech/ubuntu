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
//! \file    IUImageLayout.h
//! \brief   InitImage file structures
//! \author
//! \date
////////////////////////////////////////////////////////////////////////////////

#ifndef __IUIMAGE_LAYOUT_H__
#define __IUIMAGE_LAYOUT_H__

#include "SignatureSet.h"

typedef struct IUImage_Section_Common_t {
    unsigned int    uSectionType;
    #define IUIMAGE_HEADER      0
    #define IUIMAGE_OTPKEYS     1
    #define IUIMAGE_APPKEYS     2
    #define IUIMAGE_IMAGES      3
    #define IUIMAGE_SIGNATURES  4
    unsigned int    uSectionLength; //Contains length of IUImage_Section_Common
}IUImage_Section_Common;


typedef struct IUImage_Section_Header_t {
    IUImage_Section_Common  tCommon;
    unsigned int    uTotalLen; //don't contain the  length of IUImage_Section_Header
    unsigned int    uUniqueID;
    unsigned char   bHashValue[20];
}IUImage_Section_Header;


typedef struct IUImage_Section_OTPKeys_t {
    IUImage_Section_Common  tCommon;
    unsigned char   bOTPKeyData[0];
}IUImage_Section_OTPKeys;

typedef struct Application_Key_t {
    unsigned int    uAppKeyType;
    #define APPKEY_HDCP             10
    #define APPKEY_AACS_KEY         1
    #define APPKEY_AACS_CERT        2
    #define APPKEY_BDPLUS_DEVKEY    3
    #define APPKEY_BDPLUS_DEVCERT   4
    #define APPKEY_BDPLUS_DEVPUB    5
    #define APPKEY_BDPLUS_MVKEY     6
    #define APPKEY_BDPLUS_MVCERT    7
    #define APPKEY_BDPLUS_MVPUB     8
    #define APPKEY_CUSTKEY          9
    #define APPKEY_MAC_ADDR         11
    #define APPKEY_NETFLIX_ESN      12
    #define APPKEY_NETFLIX_MODEL    13
    #define APPKEY_WMDRM_JANUS      14
    #define APPKEY_WMDRM_JANUS_CERT_TEMPLATE  15
    #define APPKEY_CSS          16
    #define APPKEY_CPRM         17
    #define APPKEY_PANDORA      18
    #define APPKEY_SUNEXT_DRIVE 19
    #define APPKEY_ULI          20
    #define APPKEY_WV           21
    #define APPKEY_CLIENTCA0    22
    #define APPKEY_CLIENTCA1    23
    #define APPKEY_CLIENTCA2    24
    #define APPKEY_CLIENTCER    25
    #define APPKEY_CLIENTKEY    26
    #define APPKEY_PRKEY        27
    #define APPKEY_BGROUPCER    28
    #define APPKEY_HDCPPRIV     29
    #define APPKEY_HDCPLC       30
    #define APPKEY_HDCPDN       31
    #define APPKEY_HDCPCER      32
    #define APPKEY_CUSTBLOB     33
    #define APPKEY_UEIKEY       34
    #define APPKEY_HDCP2X_WIDI  35
    #define APPKEY_MOBITV_KEY   36
    #define APPKEY_MOBITV_CERT  37
    #define APPKEY_DTCP_PVK     38
    #define APPKEY_DTCP_CRT     39
    #define APPKEY_DTCP_RNG     40
    #define APPKEY_VUDU_KEY     41
    #define APPKEY_HDCPRX_KEY   42
    #define APPKEY_MARLIN_KEYS  43
    #define APPKEY_MARLIN_CERTS 44
    #define APPKEY_CIPLUS_KEYS  45
    #define APPKEY_CIPLUS_CERTS 46
    #define APPKEY_ICP_PKG      47
    #define APPKEY_DTCP_PKG     48
    #define APPKEY_HDCPPRIV_R   49
    #define APPKEY_HDCPLC_R     50
    #define APPKEY_HDCPDN_R     51
    #define APPKEY_HDCPCER_R    52
    #define APPKEY_GKEY         53
    #define APPKEY_CUSTTA       54
    #define APPKEY_HDCPPRIV_PADDED 55
    #define APPKEY_OEM_EXT_RSA  56
    #define APPKEY_OEM_ROOT_RSA 57
    #define APPKEY_RPMB         58
    #define APPKEY_RPMB_CLEAR   59
    #define APPKEY_WMDRM_JANUS30    60
    #define APPKEY_WMDRM_JANUS_CERT_TEMPLATE30  61
    #define APPKEY_PR30KEY      62
    #define APPKEY_BGROUPCER30  63
    #define APPKEY_SERIAL       64
    #define APPKEY_CARRIER_BILLING  65
    #define APPKEY_CISCO_VSSS_KEY   66
    #define APPKEY_CISCO_VSSS_CERT  67
    #define APPKEY_EXTENDED_KEY0    68  //Used for DCB public key
    #define APPKEY_EXTENDED_KEY1    69  //Used for KM2 key
    #define APPKEY_EXTENDED_KEY2    70  //Used for KM2 certificate
    #define APPKEY_EXTENDED_KEY3    71  //Used for PR4 key
    #define APPKEY_EXTENDED_KEY4    72  //Used for PR4 certificate
    #define APPKEY_EXTENDED_KEY5    73  //Used for CustBlob1
    #define APPKEY_EXTENDED_KEY6    74  //Used for CustBlob2
    #define APPKEY_EXTENDED_KEY7    75  //Used for CustX509 key
    #define APPKEY_EXTENDED_KEY8    76  //Used for CustX509 cert
    #define APPKEY_EXTENDED_KEY9    77  //Used for ESM HDCP2X store
    #define APPKEY_EXTENDED_KEY10   78  //Used for ESM HDCP2X Rx overlay
    #define APPKEY_LAST_KEYTYPE     79


    unsigned int    uLen;   //Only Key Data Length
    unsigned char   bHashValue[20]; //The hash value of the whoel AppKeyData
    unsigned char   bAppKeyData[0];
}Application_Key;


typedef struct IUImage_Section_ApplicationKeys_t {
    IUImage_Section_Common tCommon;
    unsigned int uKeyNum;
    Application_Key bAppKey[0];
}IUImage_Section_ApplicationKeys;


typedef struct Release_Image_t
{
    unsigned int uIMGType;
    unsigned int uImageLength;
    unsigned char bImageData[0];
}Release_Image;

typedef struct IUImage_Section_Images_t {
    IUImage_Section_Common tCommon;
    unsigned int    uImageCount;
    Release_Image   pImage[0];
}IUImage_Section_Images;

typedef struct IUImage_Section_Signatures_t {
    IUImage_Section_Common tCommon;
    Release_Signature_Sets_Pack pSSPack[0];
}IUImage_Section_Signatures;

#endif
