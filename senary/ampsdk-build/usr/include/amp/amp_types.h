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
//! \file amp_types.h
//!
//! \brief This file defined common types used in AMP.
//!
//! Purpose:
//!
//!
//! Version, Date and Author :
//!   V 1.00,    Septemper 18 2012,    Justin Wu
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef __AMP_TYPES_H__
#define __AMP_TYPES_H__

#include "amp_client_rpc.h"
#include "com_type.h"

///////////////////////////////////////////////////////////////////////////////
//! [Part1] Header file include
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//! [Part2] Declaration of external variables or functions
///////////////////////////////////////////////////////////////////////////////

/** \def AMP_IN
 *
 *  \brief Dummy MACRO for declare function parameter is input direction.
 *
 *         This macro should be used for API declartion. It is helpful to let
 *         user know the parameter direction is input.
 */
#define AMP_IN

/** \def AMP_OUT
 *
 *  \brief Dummy MACRO for declare function parameter is output direction.
 *
 *         This macro should be used for API declartion. It is helpful to let
 *         user know the parameter direction is output.
 */
#define AMP_OUT

/** \def AMP_INOUT
 *
 *  \brief Dummy MACRO for declare function parameter is input/output direction.
 *
 *         This macro should be used for API declartion. It is helpful to let
 *         user know the parameter direction is input then output.
 */
#define AMP_INOUT

/** \def FUNC_POINTER(name, param)
 *
 *  \brief Declaration for API function pointer.
 *
 *         This macro just to make it simpler to declare a function pointer.
 *
 *  \param[in] name The function pointer name.
 *  \param[in] param The funcation parameter list. It should be arround by ().
 */
#define FUNC_POINTER(name, param)   HRESULT (* name) param

///////////////////////////////////////////////////////////////////////////////
//! [Part3] Local macros, type definitions
///////////////////////////////////////////////////////////////////////////////

#ifndef _typedef___HRESULT
#define _typedef___HRESULT
/** \typedef HRESULT
 *
 *  \brief Type defined for function return code (error code).
 */
typedef unsigned long       HRESULT;
#endif /* _typedef___HRESULT */

/** \enum AMP_MODULE
 *
 *  \brief The AMP software module code.
 */
typedef enum {
    MODULE_GENERIC  = 0x0000,               /**< For generic modules. */
    MODULE_DUM      = AMP_COMPONENT_DUM,    /**< For DUM component. */
    MODULE_CLK      = AMP_COMPONENT_CLK ,   /**< For CLK component. */
    MODULE_DMX      = AMP_COMPONENT_DMX ,   /**< For DMX component. */
    MODULE_VDEC     = AMP_COMPONENT_VDEC,   /**< For VDEC component. */
    MODULE_VENC     = AMP_COMPONENT_VENC,   /**< For VENC component. */
    MODULE_VOUT     = AMP_COMPONENT_VOUT,   /**< For VOUT component. */
    MODULE_ADEC     = AMP_COMPONENT_ADEC,   /**< For ADEC component. */
    MODULE_AENC     = AMP_COMPONENT_AENC,   /**< For AENC component. */
    MODULE_AREN     = AMP_COMPONENT_AREN,   /**< For AREN component. */
    MODULE_APP      = AMP_COMPONENT_APP ,   /**< For APP component. */
    MODULE_AOUT     = AMP_COMPONENT_AOUT,   /**< For AOUT component. */
    MODULE_AVIN     = AMP_COMPONENT_AVIN,   /**< For AVIN component. */
    MODULE_IVPP     = AMP_COMPONENT_IVPP,   /**< For IVPP component. */
    MODULE_DEINT    = AMP_COMPONENT_DEINT,  /**< For DEINT component. */
    MODULE_OVPP     = AMP_COMPONENT_DEINT,  /**< For DEINT component. deprecated */
    MODULE_IMGDEC   = AMP_COMPONENT_IMGDEC, /**< For IMGDEC component. */
    MODULE_CI       = AMP_COMPONENT_CI,     /**< For CI component. */
    MODULE_DVS      = AMP_COMPONENT_DVS,     /**< For DVS component. */
    MODULE_DATA_EXPORTER    = AMP_COMPONENT_DATA_EXPORTER,   /**< For Data Exporter component. */
    MODULE_SCALER   = AMP_COMPONENT_SCALER, /**< For Scaler component. */
    MODULE_VCAP     = AMP_COMPONENT_VCAP, /**< For VCAP component. */
    MODULE_ISP      = AMP_COMPONENT_ISP,    /**< For ISP component. */
    MODULE_DEWARP      = AMP_COMPONENT_DEWARP,    /**< For ISP component. */
    MODULE_DNS      = AMP_COMPONENT_DNS,    /**< For ISP DNS component. */
    MODULE_ROTATE   = AMP_COMPONENT_ROTATE,    /**< For ROTATE component. */
    MODULE_SR       = AMP_COMPONENT_SR,     /**< For SR component. */
    MODULE_VA       = AMP_COMPONENT_VA,     /**< For VA component. */

    MODULE_FW       = 0x0020,   /**< For framework module. */
    MODULE_LOG      = 0x0021,   /**< For debug log module. */
    MODULE_BUF      = 0x0022,   /**< For buffer management module. */
    MODULE_MEM      = 0x0023,   /**< For memory management module. */
    MODULE_EVENT    = 0x0024,   /**< For event module. */
    MODULE_EVENTLI  = 0x0025,   /**< For event listener module. */
    MODULE_EVENTBUS = 0x0026,   /**< For event bus module. */

    MODULE_PIPELINE = 0x0030,   /**< For pipeline. */
    MODULE_DISPSRV  = 0x0031,   /**< For display service. */
    MODULE_SNDSRV   = 0x0032,   /**< For sound service. */
    MODULE_DRMSRV   = 0x0033,   /**< For DRM service. */
    MODULE_ZSP      = 0x0034,   /**< For ZSP module */
    MODULE_CPM      = 0x0035,   /**< For CPM module */
    MODULE_QOS      = 0x0036,   /**< For Qos module */

    MODULE_AVS      = 0x0040,   /**< For av sync module. */
    MODULE_VMETA    = 0x0041,   /**< For vmeta scheduler component. */

    MODULE_M2MCRYPTO = 0x0042,     /**< For MEM2MEM crypto module. */
    MODULE_DRMCLT   = 0x0043,     /**< For DRM client. */

    MODULE_SHM      = 0x0044,   /**< For shared memory module. */
    MODULE_DSP      = 0x0045,   /**< For DSP module */
    MODULE_DSP_MAX      = 0x0059,   /**< reserver 20 module interface For DSP module */

    MODULE_MAX,                 /**< The max value of AMP module ID. */
    MODULE_ALL                  /**< For identify all AMP module ID. */
} AMP_MODULE;

/** \enum HRESULT_VALUE
 *
 *  \brief The AMP software module code.
 */
enum HRESULT_VALUE {
    SUCCESS         = 0x0000,   /**< Success. */
    ERR_NOTIMPL     = 0x0001,   /**< Function/feature not implementated. */
    ERR_ERRPARAM    = 0x0002,   /**< Function parameter error. */
    ERR_NOMEM       = 0x0003,   /**< Not enough memory. */
    ERR_NOSHM       = 0x0004,   /**< Not enough share memory. */
    ERR_TIMEOUT     = 0x0005,   /**< Operation timeout. */
    ERR_ERRSYSCALL  = 0x0006,   /**< Syscall error. */
    ERR_IOFAIL      = 0x0007,   /**< Perepheral IO fail. */
    ERR_EVENTFULL   = 0x0008,   /**< Event queue full. */
    ERR_HWBUSY      = 0x0009,   /**< Hardware busy. */
    ERR_HWFAIL      = 0x000A,   /**< Hardware fail. */
    ERR_OSALFAIL    = 0x000B,   /**< OSAL fail. */
    ERR_NOSWRSC     = 0x000C,   /**< Not enough software resource. */
    ERR_NOHWRSC     = 0x000D,   /**< Not enough hardware resource. */
    ERR_SWSTATEWRONG= 0x000E,   /**< Don't permit in current software state. */
    ERR_HWSTATEWRONG= 0x000F,   /**< Don't permit in current hardware state. */
    ERR_RCPERROR    = 0x0010,   /**< RPC error. */
    ERR_SWMODEWRONG = 0x0011,   /**< Software mode wrong. */
    ERR_HWMODEWRONG = 0x0012,   /**< Hardware mode wrong. */
    ERR_CONNCLEARED = 0x0013,   /**< Connection has been cleared error. */
    ERR_RANGEPARAM  = 0x0014,   /**< Parameter is out of range. */
    ERR_CANCELLED   = 0x0015,   /**< Operation is cancelled. */
    ERR_BDERROR     = 0x0016,   /**< BD data error. */
    ERR_NOTEXISTED  = 0x0017,   /**< The input item doesn't exist */

    ERR_PRIVATE_BASE= 0x1000,   /**< The module private error value base. */
};

/** \enum HRESULT_PRIVATE_VALUE
 *
 *  \brief The private error code define.
 */
enum HRESULT_PRIVATE_VALUE {
    ERR_VOUT_NO_PTS = ERR_PRIVATE_BASE | 0x1,  /**<No PTS, for MODULE_VOUT .*/
    ERR_VOUT_NO_EOS = ERR_PRIVATE_BASE | 0x2,  /**<No EOS, for MODULE_VOUT .*/
    ERR_VOUT_RPU_PRESENT = ERR_PRIVATE_BASE | 0x3, /*<Not BL ONLY for MODULE_VOUT */
};
#include "isl/amp_logger.h"

/** \def HRESULT_GEN(module, value)
 *
 *  \brief This macro function is used to generate HRESULT code based on module
 *         name and error code value.
 *
 *         HRESULT code is combined from module ID (high 16 bits) and error
 *         code value (low 16 bits). This function will get the module macro
 *         by add prefix of "MODULE_" before "module", then combine with error
 *         code value.
 *
 *         We needn't combine the module ID for SUCCESS.
 *
 *  \param[in] module The module ID. Please ref AMP_MODULE for detail.
 *  \param[in] value The error code. Please ref HRESULT_VALUE for detail.
 *                                   Other software modules could define it's
 *                                   private error code who's value should
 *                                   bigger than ERR_PRIVATE_BASE.
 *
 *  \retval Return the generated HRESULT value.
 *
 *  \sa AMP_MODULE
 *  \sa HRESULT_VALUE
 *  \sa HRESULT
 *  \sa HRESULT_CODE
 */
#define HRESULT_GEN(module, value)  (                                       \
        ((((MODULE_##module) & 0xFFFF) << 16) | ((value) & 0xFFFF))         \
    )

/** \def HRESULT_CODE(module, value)
 *
 *  \brief This macro function is to generate HRESULT code based on module ID
 *         value and error code value.
 *
 *         This function is very like HRESULT_GEN(), but the caller just need
 *         provide the module ID value but module name. This is helpful to
 *         to generte HRESULT code when the caller just knows the module ID
 *         value.
 *
 *         Same as HRESULT_GEN(), we needn't combine the module ID for SUCCESS.
 *
 *  \param[in] module The module ID value.
 *  \param[in] value The error code. Please ref HRESULT_VALUE for detail.
 *                                   Other software modules could define it's
 *                                   private error code who's value should
 *                                   bigger than ERR_PRIVATE_BASE.
 *
 *  \retval Return the generated HRESULT value.
 *
 *  \sa AMP_MODULE
 *  \sa HRESULT_VALUE
 *  \sa HRESULT
 *  \sa HRESULT_GEN
 */
#define HRESULT_CODE(module, value)                                         \
    ((((module) & 0xFFFF) << 16) | ((value) & 0xFFFF))

/** \def HRESULT_VALUE(code)
 *
 *  \brief This macro function is used to get error code value from HRESULT.
 *
 *         HRESULT code is combined from module ID (high 16 bits) and error
 *         code value (low 16 bits). To get the error code value, the caller
 *         could just call this function.
 *
 *  \param[in] code The HRESULT code.
 *
 *  \retval Return the error code value (low 16 bits in HRESULT).
 *
 *  \sa AMP_MODULE
 *  \sa HRESULT_VALUE
 *  \sa HRESULT
 *  \sa HRESULT_GEN
 *  \sa HRESULT_CODE
 *  \sa HRESULT_MODULE
 */
#define HRESULT_VALUE(code)     ((code) & 0xFFFF)

/** \def HRESULT_MODULE(code)
 *
 *  \brief This macro function is used to get HRESULT module id.
 *
 *         HRESULT code is combined from module ID (high 16 bits) and error
 *         code value (low 16 bits). To get the module ID, the caller
 *         could just call this function.
 *
 *  \param[in] code The HRESULT code.
 *
 *  \retval Return the module ID (high 16 bits in HRESULT).
 *
 *  \sa AMP_MODULE
 *  \sa HRESULT_VALUE
 *  \sa HRESULT
 *  \sa HRESULT_GEN
 *  \sa HRESULT_CODE
 *  \sa HRESULT_VALUE
 */
#define HRESULT_MODULE(code)    ((code >> 16) & 0xFFFF)

///////////////////////////////////////////////////////////////////////////////
//! [Part4] Declaration of local/global variables
///////////////////////////////////////////////////////////////////////////////
#define VARIABLE_UNUSED(a) (a) = (a)

#ifdef __cplusplus
extern "C"
{
#endif
///////////////////////////////////////////////////////////////////////////////
//! [Part5] Declarations of local static functions
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
//! [Part6] Global functions
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif


#endif  /* __AMP_TYPES_H__ */
