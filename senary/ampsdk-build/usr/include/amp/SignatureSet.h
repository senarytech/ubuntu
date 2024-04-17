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

#ifndef __SIGNATURE_SET__
#define __SIGNATURE_SET__


typedef struct Release_Signature
{
    int bIMGType;
    #define TYPE_IMAGE_SM       1
    #define TYPE_IMAGE_BL       2   // BootLoader
    #define TYPE_IMAGE_2_Part1  3
    #define TYPE_IMAGE_CPU1     4
    #define TYPE_IMAGE_FIGO_CUST    5   // Image used to load customer key in Image2
    #define TYPE_IMAGE_FIGO     6
    #define TYPE_IMAGE_2_Part2  7
    #define TYPE_IMAGE_CPU0     8
    #define TYPE_IMAGE_RAMDISK_NORMAL   9
    #define TYPE_IMAGE_RAMDISK_RECOVERY 10
    #define TYPE_IMAGE_BLS_ARM          11
    #define TYPE_IMAGE_BLS_FIGO         12
    #define TYPE_IMAGE_TZK_NORMAL       13
    #define TYPE_IMAGE_TZK_RECOVERY     14
    #define TYPE_IMAGE_FW               15
    #define TYPE_IMAGE_TA               16
    char bImageName[32]; //bImageName, uImageOffset just for FIGO Section
    int  uImageOffset;
    char bSignature[256];
    char bCheckValue[128];  //Check Value to make sure the Signature is matched with the image
}Release_Signature;

typedef struct Release_Signature_Set
{
    int  uSignatureNum;
    #define MAX_IMG_NUMBER  120
    int  uSetSize;//Include Header length
    char bPublicKeyMod[512];
    Release_Signature pSignatures[0];
}Release_Signature_Set;

typedef struct Release_Signature_Sets_Pack
{
    int uSetsNum;
    int uPackSize;
    int uSetsSize[4];
    char bSetsName[4][32];
    Release_Signature_Set pSignatureSet[0];
}Release_Signature_Sets_Pack;

#endif
