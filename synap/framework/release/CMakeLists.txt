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
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(FRAMEWORK_DIR ${VSSDK_DIR}/synap/framework)
set(SYNAP_DEVICE_DIR ${VSSDK_DIR}/synap/vsi_npu_driver/synap/user)
set(SYNAP_BUILD_TYPE SyNAP CACHE INTERNAL "" FORCE)

list(APPEND CMAKE_MODULE_PATH ${FRAMEWORK_DIR}/cmake/modules)
include(BuildType)
find_package(VSSDK REQUIRED)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_TOOLCHAIN_FILE ${FRAMEWORK_DIR}/cmake/toolchain/android_vssdk_toolchain.cmake)

project(synap_samples)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "Install directory" FORCE)
endif()

set(LIB_INSTALL_SUBDIR lib/${TARGET} CACHE INTERNAL "lib installation sub-directory" FORCE)
set(BIN_INSTALL_SUBDIR bin/${TARGET} CACHE INTERNAL "bin installation sub-directory" FORCE)
set(INC_INSTALL_SUBDIR include CACHE INTERNAL "inc installation sub-directory" FORCE)

message("CMAKE BUILD TYPE:\t${CMAKE_BUILD_TYPE}")
message("CMAKE INSTALL DIR:\t${CMAKE_INSTALL_PREFIX}")

# Sample SyNAP libraries
add_subdirectory(${SYNAP_DEVICE_DIR} ${CMAKE_BINARY_DIR}/synap_device)
add_subdirectory(${FRAMEWORK_DIR}/lib/stb ${CMAKE_BINARY_DIR}/lib/stb)
add_subdirectory(${FRAMEWORK_DIR}/lib/synap_utils ${CMAKE_BINARY_DIR}/lib/synap_utils)
add_subdirectory(${FRAMEWORK_DIR}/lib/synapnb ${CMAKE_BINARY_DIR}/lib/synapnb)
add_subdirectory(${FRAMEWORK_DIR}/lib/synap_app ${CMAKE_BINARY_DIR}/lib/synap_app)
add_subdirectory(${FRAMEWORK_DIR}/lib/synap_img ${CMAKE_BINARY_DIR}/lib/synap_img)
add_subdirectory(${FRAMEWORK_DIR}/lib/synap_preprocess ${CMAKE_BINARY_DIR}/lib/synap_preprocess)
add_subdirectory(${FRAMEWORK_DIR}/lib/synap_postprocess ${CMAKE_BINARY_DIR}/lib/synap_postprocess)

# Sample SyNAP applications
add_subdirectory(${FRAMEWORK_DIR}/app/synap_cli ${CMAKE_BINARY_DIR}/app/synap_cli)
add_subdirectory(${FRAMEWORK_DIR}/app/synap_cli_ic ${CMAKE_BINARY_DIR}/app/synap_cli_ic)
add_subdirectory(${FRAMEWORK_DIR}/app/synap_cli_od ${CMAKE_BINARY_DIR}/app/synap_cli_od)
