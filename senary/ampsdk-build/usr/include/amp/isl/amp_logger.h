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
//! \file amp_logger.h
//!
//! \brief This file declare the logger core functions.
//!
//! Purpose: Provide log funtions.
//!
//!
//!     Version    Date                     Author
//!     V 1.00,    Dec 13 2012,             Justin Wu. Initial from mv_logger.h.
//!     V 1.10,    Jan 22 2013,             Justin Wu. Prefix with source info.
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef __AMP_LOGGER_H__
#define __AMP_LOGGER_H__

#include "amp_types.h"
#include "isl/amp_logger_isr.h"
#include <inttypes.h>


/** \def AMP_LOG_NAME_LEN_MAX
 *  \brief The log module name string max length.
 */
#define AMP_LOG_NAME_LEN_MAX    32

/** \typedef AMP_LOG_LEVEL
 *  \brief Debug log level definition.
 */
typedef enum {
    AMP_LOG_FATAL = 0,          /**< For errors that system can not continue. */
    AMP_LOG_ERROR,              /**< For errors that system can still work. */
    AMP_LOG_HIGH,               /**< High level debug info. */
    AMP_LOG_MID,                /**< Middle level debug info. */
    AMP_LOG_LOW,                /**< Low level debug info. */
    AMP_LOG_INFO,               /**< Periodical and trivial debug info. */
    AMP_LOG_USER1,              /**< User specific debug info. */
    AMP_LOG_USER2,              /**< User specific debug info. */
    AMP_LOG_LEVEL_MAX = 8,      /**< MAX level should not exceed 7. */
} AMP_LOG_LEVEL;

/** \typedef AMP_LOG_CMD
 *  \brief Debug log control command definition.
 *  Please ref AMP_LOG_Control for the usage.
 */
typedef enum {
    AMP_LOG_CMD_ENDEBUG,        /**< Enable or disable log command. */
    AMP_LOG_CMD_LEVELUP,        /**< Increace log level by 1. */
    AMP_LOG_CMD_LEVELDOWN,      /**< Decreace log level by 1. */
    AMP_LOG_CMD_SETMNAME,       /**< Set module name based on module ID. */
    AMP_LOG_CMD_GETMNAME,       /**< Get module name based on module ID. */
    AMP_LOG_CMD_SETLEVEL,       /**< Set log level based on module ID. */
    AMP_LOG_CMD_SETLEVELBYNAME, /**< Set log level based on module name. */
    AMP_LOG_CMD_GETLEVEL,       /**< Get log level based on module ID. */
    AMP_LOG_CMD_GETLEVELBYNAME, /**< Get log level based on module name. */
    AMP_LOG_CMD_ENLEVEL,        /**< Enable module log level. */
    AMP_LOG_CMD_ENLEVELBYNAME,  /**< Enable log level based on module name. */
    AMP_LOG_CMD_DISLEVEL,       /**< Disable module log level. */
    AMP_LOG_CMD_DISLEVELBYNAME, /**< Disable log level based on module name. */
    AMP_LOG_CMD_GETMNUM,        /**< Get total module number. */
    AMP_LOG_CMD_SETFILENAME,    /**< Set log file name.
                                     Only valid with AMP_LOG_FILE path.
                                     Don't calling this cmd under other path. */
    AMP_LOG_CMD_INITDSPLOG,       /**<  init dsp log. */
} AMP_LOG_CMD;

/** \typedef AMP_LOG_PATH
 *  \brief Debug log output path definition.
 */
typedef enum {
    AMP_LOG_CONSOLE,            /**< Debug log output to console. */
    AMP_LOG_FILE,               /**< Debug log output to file. */
    AMP_LOG_ANDROID_LOG,        /**< Debug log output to Android log. */
    AMP_LOG_SYSLOG,             /**< Debug log output through syslog(3). */
    AMP_LOG_PATH_MAX            /**< MAX path should not execeed 2. */
} AMP_LOG_PATH;


#ifdef __cplusplus
extern "C"
{
#endif
/*
 * \brief This function is used to register dsp log update/init function
 */
void AMP_RegisterDspLog(void (*update)(int, unsigned int),void (*init)(int));

/** \fn HRESULT AMP_LOG_Initialize(
 *          AMP_IN      AMP_LOG_LEVEL       level,
 *          AMP_IN      AMP_LOG_PATH        path);
 *
 *  \brief This function is used to initialize the AMP logger module.
 *
 *         AMP logger provides a set of functions to print debug informations
 *         to console, Android logcat or file (not implemented). Programers
 *         could use it print helpful debug informations, and each of debug
 *         information could be binded to a module and a debug level. It provids
 *         a control function (AMP_LOG_Control) to disable/enable the
 *         information print for each of module and debug level. For module
 *         list, please ref AMP_MODULE. For debug level, please ref
 *         AMP_LOG_LEVEL.
 *
 *         This function always initialize control structure and set default
 *         level and name for each module. It also initialize the ISR logger
 *         module which is used for low level driver debug in interrupt handle
 *         functions.
 *
 *         This function should be called only once for each of process before
 *         it use any other AMP logger functions. If you call several times,
 *         each of call will reset the AMP logger settings.
 *
 *         AMP logger is fully independed between processes. So you could use
 *         it with different settings for each of process.
 *
 *  \param[in] level The default logger level for all modules. It also be set as
 *                   the global debug level and will be used by other funcation.
 *                   Please ref AMP_LOG_LEVEL for detail.
 *
 *  \param[in] path  The default output path of debug message. Please ref
 *                   AMP_LOG_PATH for detail. (AMP_LOG_FILE don't be
 *                   implemented)
 *  \retval SUCCESS Initialize AMP logger successed.
 *
 *  \sa AMP_MODULE
 *  \sa AMP_LOG_LEVEL
 *  \sa AMP_LOG_PATH
 *  \sa AMP_LOG_Control
 *  \sa AMP_LogPro
 *  \sa AMPPRINTF
 *  \sa AMPLOGF
 *  \sa AMPLOGE
 *  \sa AMPLOGD
 *  \sa AMPLOG
 */
HRESULT AMP_LOG_Initialize(
            AMP_IN      AMP_LOG_LEVEL       level,
            AMP_IN      AMP_LOG_PATH        path);

/*
 * \brief This function is used to uninitialize the AMP logger module.
 *         The main work it does is to terminate the internal task and
 *         release some OS resources (semaphore, mutex etc).
 * \retval SUCCESS Uninitialize AMP logger successed
 */
HRESULT AMP_LOG_Uninitialize();

/** \fn HRESULT AMP_LOG_Control(
 *          AMP_IN      AMP_LOG_CMD         cmd,
 *          AMP_IN      UINT32              param1,
 *          AMP_IN      UINT32              param2,
 *          AMP_INOUT   CHAR                *pModule);
 *
 *  \brief This function is used to set logger level and other parameters in
 *         runtime.
 *
 *         Since each of debug message is bind to a specified module ID and
 *         a debug level, we implemented this function to enable/disable debug
 *         messages. This function could also be used to get/set module name,
 *         get total module number and other misc control functions. Please ref
 *         AMP_LOG_CMD for detail commands.
 *
 *         This function could be used only after AMP_LOG_Initialize() returned
 *         success.
 *
 *  \param[in] cmd Debug control command, see AMP_LOG_CMD for detail.
 *
 *             AMP_LOG_CMD_ENDEBUG: enable or disable debug log.
 *               param1: 0 for disable all debug log, else enable debug log.
 *               param2: Not be used.
 *               pModule: Not be used.
 *
 *             AMP_LOG_CMD_LEVELUP: increase the globle debug level, then
 *               set all modules debug level to globle debug level.
 *               param1: It should be a AMP_LOG_LEVEL * type pointer. It is
 *                 used to return the globle debug level to caller. If this
 *                 parameter is 0 or NULL, nothing will return.
 *               param2: Not be used.
 *               pModule: Not be used.
 *
 *             AMP_LOG_CMD_LEVELDOWN: decrease the globle debug level, then
 *               set all modules debug level to globle debug level.
 *               param1: It should be a AMP_LOG_LEVEL * type pointer. It is
 *                 used to return the globle debug level to caller. If this
 *                 parameter is 0 or NULL, nothing will return.
 *               param2: Not be used.
 *               pModule: Not be used.
 *
 *             AMP_LOG_CMD_SETMNAME: set the specified module's name.
 *               param1: The module ID, please ref AMP_MODULE for detail.
 *                 It shouldn't equal to MODULE_MAX.
 *               param2: Not be used.
 *               pModule: The module name string. The name string should
 *                 less than AMP_LOG_NAME_LEN_MAX.
 *
 *             AMP_LOG_CMD_GETMNAME: get the specified module's name.
 *               param1: The module ID, please ref AMP_MODULE for detail.
 *                 It shouldn't equal to MODULE_MAX.
 *               param2: Not be used.
 *               pModule: The module name string buffer start address
 *                 provide by called. The name string should less than
 *                 AMP_LOG_NAME_LEN_MAX.
 *
 *             AMP_LOG_CMD_SETLEVEL: set the specified module's debug level.
 *               param1: The module ID, please ref AMP_MODULE for detail.
 *                 It will set all modules to this level if the value is
 *                 equal to MODULE_MAX.
 *               param2: The desired debug level. See AMP_LOG_LEVEL for
 *                 detail.
 *               pModule: Not be used.
 *
 *             AMP_LOG_CMD_SETLEVELBYNAME: set the specified module's debug
 *               level by module name.
 *               param1: The desired debug level. See AMP_LOG_LEVEL for
 *                 detail.
 *               param2: Not be used.
 *               pModule: The module name string. The name string should
 *                 less than AMP_LOG_NAME_LEN_MAX. It will set all modules
 *                 to this debug level if the string is equal to "ALL".
 *
 *             AMP_LOG_CMD_GETLEVEL: get the specified module's debug level.
 *               param1: The module ID, please ref AMP_MODULE for detail.
 *                 It shouldn't equal to MODULE_MAX.
 *               param2: It should be a AMP_LOG_LEVEL * type pointer. It is
 *                 used to return the specified module's debug level to
 *                 caller. If this parameter is 0 or NULL, nothing will
 *                 return.
 *               pModule: Not be used.
 *
 *             AMP_LOG_CMD_GETLEVELBYNAME: get the specified module's debug
 *               level by module name.
 *               param1: It should be a AMP_LOG_LEVEL * type pointer. It is
 *                 used to return the specified module's debug level to
 *                 caller. If this parameter is 0 or NULL, nothing will
 *                 return.
 *               param2: Not be used.
 *               pModule: The module name string. The name string should
 *                 less than AMP_LOG_NAME_LEN_MAX.
 *
 *             AMP_LOG_CMD_ENLEVEL: enable specified module's specified
 *               debug level.
 *               param1: The module ID, please ref AMP_MODULE for detail.
 *                 It will enable all module's specified debug level if it
 *                 is equal to MODULE_MAX.
 *               param2: The desired debug level. See AMP_LOG_LEVEL for
 *                 detail.
 *               pModule: Not be used.
 *
 *             AMP_LOG_CMD_ENLEVELBYNAME: enable specified module's
 *               specified debug level by module name.
 *               param1: The desired debug level. See AMP_LOG_LEVEL for
 *                 detail.
 *               param2: Not be used.
 *               pModule: The module name string. The name string should
 *                 less than AMP_LOG_NAME_LEN_MAX. It will set all modules
 *                 specified debug level if the string is equal to "ALL".
 *
 *             AMP_LOG_CMD_DISLEVEL: disable specified module's specified
 *               debug level.
 *               param1: The module ID, please ref AMP_MODULE for detail.
 *                 It will disable all module's specified debug level if it
 *                 is equal to MODULE_MAX.
 *               param2: The desired debug level. See AMP_LOG_LEVEL for
 *                 detail.
 *               pModule: Not be used.
 *
 *             AMP_LOG_CMD_DISLEVELBYNAME: disable specified module's
 *               specified debug level by module name.
 *               param1: The desired debug level. See AMP_LOG_LEVEL for
 *                 detail.
 *               param2: Not be used.
 *               pModule: The module name string. The name string should
 *                 less than AMP_LOG_NAME_LEN_MAX. It will clear all modules
 *                 specified debug level if the string is equal to "ALL".
 *
 *             AMP_LOG_CMD_GETMNUM: get the MODULE_MAX value.
 *               param1: It should be a UINT32 * type pointer which is used
 *                 to return the MODULE_MAX value to caller. Nothing will
 *                 return if it is equal to 0 or NULL.
 *               param2: Not be used.
 *               pModule: Not be used.
 *
 *  \param[in] param1 Parameter 1, command related.
 *  \param[in] param2 Parameter 2, command related.
 *  \param[in] pModule Pointer to a string of module name, command related.
 *
 *  \retval SUCCESS Operation is successed.
 *  \retval HRESULT_GEN(LOG, ERR_ERRPARAM) Parameter is wrong.
 *
 *  \sa AMP_LOG_CMD
 *  \sa AMP_LOG_LEVEL
 *  \sa AMP_MODULE
 *  \sa AMP_LOG_Initialize
 */
HRESULT AMP_LOG_Control(
            AMP_IN      AMP_LOG_CMD         cmd,
            AMP_IN      intptr_t            param1,
            AMP_IN      intptr_t            param2,
            AMP_INOUT   CHAR                *pModule);

/** \fn int AMP_LogHead(
 *          AMP_IN      AMP_MODULE          mod,
 *          AMP_IN      AMP_LOG_LEVEL       level,
 *          AMP_IN      char                *head,
 *          AMP_IN      int                 size,
 *          AMP_IN      const char          *pHead,
 *          AMP_IN      const char          *pMiddle,
 *          AMP_IN      int                 uiTail);
 *
 *  \brief This function is used to generate the log message head if corespond
 *         module and debug level has been enabled.
 *
 *         To generate a debug log message head, we need provide module name or
 *         source code file name, function name and source code line number
 *         information. Then this function will check the corespond module and
 *         debug level is enabled or not, and then generate head message if it's
 *         enabled.
 *
 *         Inside of this function, it used snprintf() to generte the message
 *         head.
 *
 *         The log head will be used for most of AMPLOGx functions to provide
 *         useful information just lik the module name, caller function name
 *         and the source code line number.
 *
 *         This function could be used only after AMP_LOG_Initialize() returned
 *         success.
 *
 *  \param[in]  mod The module ID, see AMP_MODULE for detail.
 *  \param[in]  level Debug level of this message, see AMP_LOG_LEVEL for detail.
 *  \param[out] head The head string buffer start address provided by caller
 *                   that used to storage the head message.
 *  \param[in]  size The head string buffer size.
 *  \param[in]  pHead The first string for head message.
 *                    It is often be module name or source file name.
 *  \param[in]  pMiddle The second string for head message.
 *                      It is often be function name.
 *  \param[in]  uiTail Tail number to append to head message.
 *                      It is often be source code line number.
 *
 *  \retval 1: generated the head message successed.
 *  \retval 0: fail to generate the head message since the corespond module or
 *             debug level has been disabled.
 *
 *  \sa AMP_MODULE
 *  \sa AMP_LOG_LEVEL
 *  \sa AMP_LogPro
 *  \sa AMPPRINTF
 *  \sa AMPLOGF
 *  \sa AMPLOGE
 *  \sa AMPLOGD
 *  \sa AMPLOG
 */
int AMP_LogHead(
            AMP_IN      AMP_MODULE          mod,
            AMP_IN      AMP_LOG_LEVEL       level,
            AMP_IN      char                *head,
            AMP_IN      int                 size,
            AMP_IN      const char          *pHead,
            AMP_IN      const char          *pMiddle,
            AMP_IN      int                 uiTail);

/** \fn void AMP_LogPro(
 *          AMP_IN      AMP_MODULE          mod,
 *          AMP_IN      AMP_LOG_LEVEL       level,
 *          AMP_IN      char                *pHead,
 *          AMP_IN      char                *pMsg,
 *          AMP_IN      ...);
 *
 *  \brief This function is used to print a formated character message when
 *         the related level of module is enabled.
 *
 *         This function is the basic function that works for print debug
 *         messages. We also provided some other macro functions just like
 *         AMPLOGF, AMPLOGE, AMPLOGD, AMPLOD and AMPPRINTF which are based
 *         on this function.
 *
 *         Since AMP logger support output to different output path (see
 *         AMP_LOG_PATH for detail), this function will call low level function
 *         callback which has been specified when call AMP_LOG_Initialize()
 *         (with "path" parameter). For AMP_LOG_CONSOLE, it will call vprintf()
 *         to output message to console. For AMP_LOG_ANDROID_LOG, it will call
 *         Android provide __android_log_write() to output the messages. We
 *         don't implement AMP_LOG_FILE at current stage since it's easy to
 *         achieve same function with Linux's shell's output re-direction.
 *
 *         Please note the message output is under the control of module enable
 *         and debug level enable flags. Please ref AMP_LOG_Control() to change
 *         current debug logger attribute.
 *
 *         This function could be used only after AMP_LOG_Initialize() returned
 *         success.
 *
 *  \param[in] mod The module ID of this message belongs to, see AMP_MODULE for
 *                 detail.
 *  \param[in] level Debug level of this message.
 *  \param[in] pHead Character message to print.
 *  \param[in] pMsg Parameter list for formating the message.
 *
 *  \retval No return value.
 *
 *  \sa AMP_MODULE
 *  \sa AMP_LOG_LEVEL
 *  \sa AMP_LOG_PATH
 *  \sa AMP_LOG_Control
 *  \sa AMPPRINTF
 *  \sa AMPLOGF
 *  \sa AMPLOGE
 *  \sa AMPLOGD
 *  \sa AMPLOG
 */
void AMP_LogPro(
            AMP_IN      AMP_MODULE          mod,
            AMP_IN      AMP_LOG_LEVEL       level,
            AMP_IN      const char          *pHead,
            AMP_IN      const char          *pMsg,
            AMP_IN      ...);

#ifdef __cplusplus
}
#endif


/*
 * All AMP log helper macros take effects only in a debug build.
 * In a release build, the macro expands to nothing, and thus
 * takes no effects.
 * A release build is defined by following statement in the
 * corresponding profile at the "Options" section.
 * defined AMP_RELEASE_BUILD
 */

/*
 * The option is temporarily disabled since some code using AMPLOG
 * requires modification for unused variables.
 * So use AMP_RELEASE_BUILD_N instead of AMP_RELEASE_BUILD
 * to disable the feature temporarily
 */
#ifndef AMP_RELEASE_BUILD_N

/** \def AMP_LOG_HSZ
 *  \brief Log information head buffer size in byte.
 */
#define AMP_LOG_HSZ                         128

/** \def AMPPRINTF
 *  \brief This macro function is used to print debug message with the setting
 *         of MODULE_GENERIC and AMP_LOG_HIGH level.
 *
 *         This function is fully based on AMP_LogPro. So please ref AMP_LogPro
 *         for more information.
 *
 *         This function could be used only after AMP_LOG_Initialize() returned
 *         success.
 *
 *  \param[in] ... The parameter list for formating the message.
 *
 *  \retval No return value.
 *
 *  \sa AMP_LOG_Control
 *  \sa AMP_LogPro
 *  \sa AMPLOGF
 *  \sa AMPLOGE
 *  \sa AMPLOGD
 *  \sa AMPLOG
 */
#define AMPPRINTF(...)                                                      \
    do {                                                                    \
        char head[AMP_LOG_HSZ];                                             \
        if (AMP_LogHead(MODULE_GENERIC, AMP_LOG_HIGH, head, AMP_LOG_HSZ,    \
                    "MODULE_GENERIC",__FUNCTION__, __LINE__))               \
        {AMP_LogPro(MODULE_GENERIC, AMP_LOG_HIGH, head, __VA_ARGS__);}      \
    } while (0)

/** \def AMPLOGF
 *  \brief This macro function is used to print debug message with the setting
 *         of MODULE_GENERIC and AMP_LOG_FATAL level.
 *
 *         This function is fully based on AMP_LogPro. So please ref AMP_LogPro
 *         for more information.
 *
 *         This function could be used only after AMP_LOG_Initialize() returned
 *         success.
 *
 *  \param[in] ... The parameter list for formating the message.
 *
 *  \retval No return value.
 *
 *  \sa AMP_LOG_Control
 *  \sa AMP_LogPro
 *  \sa AMPPRINTF
 *  \sa AMPLOGE
 *  \sa AMPLOGH
 *  \sa AMPLOGD
 *  \sa AMPLOG
 */
#define AMPLOGF(...)                                                        \
    do {                                                                    \
        char head[AMP_LOG_HSZ];                                             \
        if (AMP_LogHead(MODULE_GENERIC, AMP_LOG_FATAL, head, AMP_LOG_HSZ,   \
                    "MODULE_GENERIC", __FUNCTION__, __LINE__))              \
        {AMP_LogPro(MODULE_GENERIC, AMP_LOG_FATAL, head, __VA_ARGS__);}     \
    } while (0)

/** \def AMPLOGE
 *  \brief This macro function is used to print debug message with the setting
 *         of MODULE_GENERIC and AMP_LOG_ERROR level.
 *
 *         This function is fully based on AMP_LogPro. So please ref AMP_LogPro
 *         for more information.
 *
 *         This function could be used only after AMP_LOG_Initialize() returned
 *         success.
 *
 *  \param[in] ... The parameter list for formating the message.
 *
 *  \retval No return value.
 *
 *  \sa AMP_LOG_Control
 *  \sa AMP_LogPro
 *  \sa AMPPRINTF
 *  \sa AMPLOGF
 *  \sa AMPLOGH
 *  \sa AMPLOGD
 *  \sa AMPLOG
 */
#define AMPLOGE(...)                                                        \
    do {                                                                    \
        char head[AMP_LOG_HSZ];                                             \
        if (AMP_LogHead(MODULE_GENERIC, AMP_LOG_ERROR, head, AMP_LOG_HSZ,   \
                    "MODULE_GENERIC", __FUNCTION__, __LINE__))              \
        {AMP_LogPro(MODULE_GENERIC, AMP_LOG_ERROR, head, __VA_ARGS__);}     \
    } while (0)

/** \def AMPLOGH
 *  \brief This macro function is used to print debug message with the setting
 *         of MODULE_GENERIC and AMP_LOG_HIGH level.
 *
 *         This function is fully based on AMP_LogPro. So please ref AMP_LogPro
 *         for more information.
 *
 *         This function could be used only after AMP_LOG_Initialize() returned
 *         success.
 *
 *  \param[in] ... The parameter list for formating the message.
 *
 *  \retval No return value.
 *
 *  \sa AMP_LOG_Control
 *  \sa AMP_LogPro
 *  \sa AMPPRINTF
 *  \sa AMPLOGF
 *  \sa AMPLOGE
 *  \sa AMPLOGD
 *  \sa AMPLOG
 */
#define AMPLOGH(...)                                                        \
    do {                                                                    \
        char head[AMP_LOG_HSZ];                                             \
        if (AMP_LogHead(MODULE_GENERIC, AMP_LOG_HIGH, head, AMP_LOG_HSZ,    \
                    "MODULE_GENERIC", __FUNCTION__, __LINE__))              \
        {AMP_LogPro(MODULE_GENERIC, AMP_LOG_HIGH, head, __VA_ARGS__);}      \
    } while (0)

/** \def AMPLOGD
 *  \brief This macro function is used to print debug message with the setting
 *         of MODULE_GENERIC and AMP_LOG_INFO level.
 *
 *         This function is fully based on AMP_LogPro. So please ref AMP_LogPro
 *         for more information.
 *
 *         This function could be used only after AMP_LOG_Initialize() returned
 *         success.
 *
 *  \param[in] ... The parameter list for formating the message.
 *
 *  \retval No return value.
 *
 *  \sa AMP_LOG_Control
 *  \sa AMP_LogPro
 *  \sa AMPPRINTF
 *  \sa AMPLOGF
 *  \sa AMPLOGE
 *  \sa AMPLOGH
 *  \sa AMPLOG
 */
#define AMPLOGD(...)                                                        \
    do {                                                                    \
        char head[AMP_LOG_HSZ];                                             \
        if (AMP_LogHead(MODULE_GENERIC, AMP_LOG_INFO, head, AMP_LOG_HSZ,    \
                    "MODULE_GENERIC", __FUNCTION__, __LINE__))              \
        {AMP_LogPro(MODULE_GENERIC, AMP_LOG_INFO, head, __VA_ARGS__);}      \
    } while (0)

/** \def AMPLOG
 *  \brief This function is used to print debug message based on the module and
 *         debug level settings.
 *
 *         This function is fully based on AMP_LogPro. So please ref AMP_LogPro
 *         for more information.
 *
 *         This function could be used only after AMP_LOG_Initialize() returned
 *         success.
 *
 *  \param[in] _mod The module ID. Please ref AMP_MODULE for detail.
 *  \param[in] _lvl The debug level. Please ref AMP_LOG_LEVEL for detail.
 *  \param[in] ... The parameter list for formating the message.
 *
 *  \retval No return value.
 *
 *  \sa AMP_LOG_Control
 *  \sa AMP_LogPro
 *  \sa AMPPRINTF
 *  \sa AMPLOGF
 *  \sa AMPLOGE
 *  \sa AMPLOGD
 */
#define AMPLOG(_mod,_lvl,...)                                               \
    do {                                                                    \
        char head[AMP_LOG_HSZ];                                             \
        if (AMP_LogHead(_mod, _lvl, head, AMP_LOG_HSZ,                      \
                    #_mod, __FUNCTION__, __LINE__))                         \
        {AMP_LogPro(_mod, _lvl, head, __VA_ARGS__);}                        \
    } while (0)

#else /* AMP_RELEASE_BUILD */

#define AMPPRINTF(...)          do {} while (0)

#define AMPLOGF(...)            do {} while (0)

#define AMPLOGE(...)            do {} while (0)

#define AMPLOGH(...)            do {} while (0)

#define AMPLOGD(...)            do {} while (0)

#define AMPLOG(_mod,_lvl,...)   do {} while (0)

#endif /* AMP_RELEASE_BUILD */

/** \def AMP_Log
 *  \brief This function is fully same as AMPLOG. Please try to avoid use this
 *         function since it maybe removed later.
 */
#define AMP_Log             AMPLOG

#endif /* __AMP_LOGGER_H__ */
