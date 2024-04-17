#
# NDA AND NEED-TO-KNOW REQUIRED
#
# Copyright (C) 2013-2020 Synaptics Incorporated. All rights reserved.
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
# ////////////////////////////////////////////////////////////////////////////////
# //! \file CMakeLists.txt
# //!
# //! \brief 
# //!
# //!              Version   Date                                 Author
# //!              V 0.01,   Feb 3 2020                           alek
# //!
# //! Note:
# ////////////////////////////////////////////////////////////////////////////////
#
set(PKG_NAME "Udev")

set(INCLUDES libudev.h libevdev/libevdev.h)
list(LENGTH INCLUDES count)
math(EXPR count "${count} - 1")
foreach(i RANGE ${count})
  list(GET INCLUDES ${i} inc)
  find_path(${PKG_NAME}_FOUND_PATH_${i}
    NAMES ${inc}
    PATHS ${CMAKE_FIND_ROOT_PATH}
    PATH_SUFFIXES eudev/usr/include libevdev/usr/include/libevdev-1.0
  )
  list(APPEND ${PKG_NAME}_INCLUDE_DIRS ${${PKG_NAME}_FOUND_PATH_${i}})
endforeach()

set(LIBS udev evdev)
list(LENGTH LIBS count)
math(EXPR count "${count} - 1")
foreach(i RANGE ${count})
  list(GET LIBS ${i} lib)
  find_library(${PKG_NAME}_FOUND_LIB_${i}
    NAMES ${lib}
    PATHS ${CMAKE_FIND_ROOT_PATH}
    PATH_SUFFIXES eudev/usr/lib libevdev/usr/lib
  )
  list(APPEND ${PKG_NAME}_LIBRARIES ${${PKG_NAME}_FOUND_LIB_${i}})
endforeach()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(${PKG_NAME} DEFAULT_MSG
  ${PKG_NAME}_LIBRARIES ${PKG_NAME}_INCLUDE_DIRS)

mark_as_advanced(${PKG_NAME}_INCLUDE_DIRS ${PKG_NAME}_LIBRARIES)


