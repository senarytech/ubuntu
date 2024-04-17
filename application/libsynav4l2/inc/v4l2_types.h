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
//! \file v4l2_types.h
//! \brief This file defined common types used in V4L2.
//!
//! Purpose:
//!
//! Version, Date and Author :
//!   V 1.00,    May 2018,    Weizhao Jiang
//!  Note:
////////////////////////////////////////////////////////////////////////////////


#ifndef __V4L2_TYPES_H__
#define __V4L2_TYPES_H__

#define E_V4L2_BASE      (0xF0000000)
// generic error code macro
#define E_V4L2_MASK      (0x0000FFFF)
#define E_V4L2_ERR(code)     (E_V4L2_BASE | ( code & E_V4L2_MASK))

#ifndef _typedef___HANDLE
#define _typedef___HANDLE
typedef void *                 V4L2_HANDLE;
#endif /* _typedef___HANDLE */


#ifndef _typedef___HRESULT
#define _typedef___HRESULT
/** \typedef HRESULT
 *
 *  \brief Type defined for function return code (error code).
 */
typedef unsigned int       HRESULT;
#endif /* _typedef___HRESULT */


/** \enum v4l2_result_value
 *
 *  \brief The V4L2 software return code.
 *
 *  Note: If change any in this defination, please update strReturnCode[] as well.
 */
enum v4l2_result_value {
    V4L2_SUCCESS             = 0x0000,               /**< Success. */
    ERROR_NOTIMPL       = E_V4L2_ERR(0x0001),   /**< Function/feature not implementated. */
    ERROR_ERRPARAM      = E_V4L2_ERR(0x0002),   /**< Function parameter error. */
    ERROR_NOMEM         = E_V4L2_ERR(0x0003),   /**< Not enough memory. */
    ERROR_NOSHM         = E_V4L2_ERR(0x0004),   /**< Not enough share memory. */
    ERROR_TIMEOUT       = E_V4L2_ERR(0x0005),   /**< Operation timeout. */
    ERROR_NOTINIT       = E_V4L2_ERR(0x0006),   /**< Driver not initialized */
    ERROR_IOFAIL        = E_V4L2_ERR(0x0007),   /**< Perepheral IO fail. */
    ERROR_IONINIT       = E_V4L2_ERR(0x0008),   /**< Init ION fail */
    ERROR_NOTREADY      = E_V4L2_ERR(0x0009),   /**< Operation Not Ready */
    ERROR_OVERWRITE     = E_V4L2_ERR(0x000A),   /**< Buffer over write. */
    ERROR_OSFAIL        = E_V4L2_ERR(0x000B),   /**< OS fail. */
    ERROR_NOSWRSC       = E_V4L2_ERR(0x000C),   /**< Not enough software resource. */
    ERROR_NOHWRSC       = E_V4L2_ERR(0x000D),   /**< Not enough hardware resource. */
    ERROR_FILEOP        = E_V4L2_ERR(0x000E),   /**< File operation fail */
    ERROR_TYPENOTSUPPORT= E_V4L2_ERR(0x000F),   /**< type not support*/
    ERROR_AMPINIT       = E_V4L2_ERR(0x0010),   /**< Init amp fail. */
    ERROR_INVALIDHANDLE = E_V4L2_ERR(0x0011),   /**< invlid handle */
    ERROR_BADADDR       = E_V4L2_ERR(0x0012),   /**< bad address */
    ERROR_WRONGRESOLUTION= E_V4L2_ERR(0x0013),  /**< wrong resolution */
    ERROR_RANGEPARAM    = E_V4L2_ERR(0x0014),   /**< Parameter is out of range. */
    ERROR_CANCELLED     = E_V4L2_ERR(0x0015),   /**< Operation is cancelled. */
    ERROR_SIGNATURE     = E_V4L2_ERR(0x0016),   /**< Signature error happen. */
    ERROR_TANOTINIT     = E_V4L2_ERR(0x0017),   /**< TA not initialized. */

    //! V4L2 special return code, porting from linux errno-base.h
    ERROR_ENOENT        = -2,       //!#define  ENOENT  2   /* No such file or directory */
    ERROR_EIO           = -5,       //! #define EIO     5   /* I/O error */
    ERROR_EBADR         = -9,       //! #define EBADF   9   /* Bad file number */
    ERROR_EAGAIN        = -11,      //! #define EAGAIN  11  /* Try again */
    ERROR_EBUSY         = -16,      //! #define EBUSY   16  /* Device or resource busy */
    ERROR_EINVAL        = -22,      //! #define EINVAL  22  /* Invalid argument */
    ERROR_EPIPE         = -32,      //! #define EPIPE   32  /* Broken pipe */
    ERROR_END
};


#endif
