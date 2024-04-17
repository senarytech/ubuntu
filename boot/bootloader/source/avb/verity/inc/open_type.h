/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2018 Synaptics Incorporated. All rights reserved.
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
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
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
#ifndef __OPEN_TYPE_H__
#define __OPEN_TYPE_H__


//typedef void* MV_CRYPTO_KEY_HANDLE;
#define MV_CRYPTO_KEY_HANDLE void* 
//typedef unsigned short UINT16; //move to ctypes.h
//typedef void*                  HANDLE;
//typedef int INT32;
//typedef unsigned char UCHAR;

//#define S_OK                E_GEN_SUC( 0x0000 ) // Success
//#define S_FALSE             E_GEN_SUC( 0x0001 ) // Success but return false status

//#define S_OK    0
//#define S_FALSE 1

#define E_FAIL 0x4035

#define OPENCRYPTO_CTX_AGENT_MAGIC_ID   0x42434445
#define OPENCRYPTO_CTX_SERVER_MAGIC_ID      0x45444342


#define IMGSHADOWMALLOC    0x00000040
#define KEYCTXMMALLOC    0x00000041
#define INPUTMALLOC    0x00000042
#define OUTPUTMALLOC    0x00000043
#define IVMALLOC    0x00000044




typedef enum
{
    //! Do nothing 
    ROMINIT_NONE  = 0x0,

    //! Backdoor loading FIGO ROM code
    ROMINIT_LOAD  = 0x1,

    //! Backdoor Check FIGO ROM Code
    ROMINIT_CHECK = 0x2,

} ROMINIT_MODE;

//Add for CIPLUS
typedef void*  AMP_DRM_t;
typedef UINT32 AMP_DMX_CRYPTO_SCHEME_TYPE;

#endif
