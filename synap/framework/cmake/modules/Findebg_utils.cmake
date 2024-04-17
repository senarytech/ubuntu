#
# NDA AND NEED-TO-KNOW REQUIRED
#
# Copyright (C) 2013-2022 Synaptics Incorporated. All rights reserved.
#
# This file contains information that is proprietary to Synaptics
# Incorporated ("Synaptics"). The holder of this file shall treat all
# information contained herein as confidential, shall use the
# information only for its intended purpose, and shall not duplicate,
# disclose, or disseminate any of this information in any manner
# unless Synaptics has otherwise provided express, written
# permission.
#
# Use of the materials may require a license of intellectual property
# from a third party or from Synaptics. This file conveys no express
# or implied licenses to any intellectual property rights belonging
# to Synaptics.
#
# INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS", AND
# SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
# INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
# INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
# CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
# OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
# BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
# COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
# DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
# TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
#
set(PKG_NAME "ebg_utils")

if(NOT ${SYNAP_BUILD_TYPE} MATCHES "VSSDK")
    return()
endif()

find_path(EBG_UTILS_INC_DIR synap/ebg_utils.h
    PATHS ${CMAKE_INSTALL_PREFIX}/${INC_INSTALL_SUBDIR} NO_CMAKE_FIND_ROOT_PATH)

find_library(EBG_UTILS_LIB NAMES ebg_utils
    PATHS ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_SUBDIR} NO_CMAKE_FIND_ROOT_PATH)

if(EBG_UTILS_INC_DIR AND EBG_UTILS_LIB)
    set(EBG_UTILS_FOUND TRUE)
endif()

if(NOT EBG_UTILS_FOUND)
    if(EBG_UTILS_REQUIRED)
        message(FATAL_ERROR "Could not find ebg_utils")
    endif()
endif()

add_library(ebg_utils INTERFACE)
target_include_directories(ebg_utils INTERFACE ${EBG_UTILS_INC_DIR})
target_link_libraries(ebg_utils INTERFACE ${EBG_UTILS_LIB})
