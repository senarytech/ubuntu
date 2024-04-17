/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2023 Synaptics Incorporated. All rights reserved.
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

#ifndef SRC_CAM_CAM_CONFIG_PARSER_H_
#define SRC_CAM_CAM_CONFIG_PARSER_H_

#include <tinyxml2.h>
#include <amp_types.h>

#ifdef __cplusplus
extern "C" {
#endif

using namespace tinyxml2;

HRESULT cam_configparser_initialize(XMLDocument   *pDoc, const char *fileName);
HRESULT cam_getelementbypath(XMLDocument   *pDoc,
                             const char      **sElePath,
                             int             iListNum,
                             XMLElement    **pEleHandle);
HRESULT cam_getelementattribstring(XMLElement    *pEleHandle,
                                   const char      *sName,
                                   const char      **sValue);
HRESULT cam_getelementattribinteger(XMLElement    *pEleHandle,
                                    const char      *sName,
                                    int             *iValue);
HRESULT cam_getelementattribuinteger(XMLElement    *pEleHandle,
                                     const char      *sName,
                                     UINT32          *uiValue);

#ifdef __cplusplus
}
#endif

#endif
