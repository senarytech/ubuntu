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
set(PKG_NAME "z")

if(NOT "${TARGET}" MATCHES "linux_baseline")
    return()
endif()

find_path(Z_INC_DIR zlib.h
    PATHS "${VSSDK_DIR}/sysroot/linux-baseline/data/sysroot/usr/include"
    NO_DEFAULT_PATH
    NO_CMAKE_SYSTEM_PATH)


find_library(Z_LIB NAMES z
    PATHS "${VSSDK_DIR}/sysroot/linux-baseline/data/sysroot/usr/lib"
    NO_DEFAULT_PATH
    NO_CMAKE_SYSTEM_PATH)

if(Z_INC_DIR AND Z_LIB)
    set(Z_FOUND TRUE)
endif()

if(NOT Z_FOUND)
    if(z_REQUIRED)
        message(FATAL_ERROR "Could not find zlib")
    endif()
endif()

add_library(z INTERFACE)
target_include_directories(z INTERFACE ${Z_INC_DIR})
target_link_libraries(z INTERFACE ${Z_LIB})
