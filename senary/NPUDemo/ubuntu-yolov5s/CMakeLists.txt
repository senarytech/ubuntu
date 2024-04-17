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
cmake_minimum_required(VERSION 3.10 )


SET(CMAKE_SYSTEM_NAME Linux)

SET(CMAKE_C_COMPILER    /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-gcc)

SET(CMAKE_CXX_COMPILER    /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++)




# Configure toolchain and build options
if("${NDK_DIR}" STREQUAL "")
    set(NDK_DIR "$ENV{NDK_DIR}")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
#include(/home/liudongbo/Midea/android-ndk-r21d/build/cmake/android.toolchain.cmake)

project(synap_samples)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)
include(BuildType)
#find_package(VSSDK REQUIRED)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "Install directory" FORCE)
endif()

message("CMAKE BUILD TYPE:\t${CMAKE_BUILD_TYPE}")
message("CMAKE INSTALL DIR:\t${CMAKE_INSTALL_PREFIX}")

#include_directories(/home/liudongbo/bsp_release/synap_linux/Synaptics_Git_Ext_VSSDK-1.10.1-lts/syna-release/toolchain/aarch64/gcc-arm-aarch64-linux-gnu-8.3/lib/gcc/aarch64-linux-gnu/8.3.0/include/)

# Use stb header library for image processing
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_LIST_DIR}/stb)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -O3")

# Sample SyNAP libraries
add_subdirectory(lib/synap_device)
add_subdirectory(lib/synap_utils)
add_subdirectory(lib/nlohmann)
add_subdirectory(lib/synap_base)
add_subdirectory(lib/synapnb)
add_subdirectory(lib/synap_app)
add_subdirectory(lib/synap_img)
add_subdirectory(lib/synap_preprocess)
add_subdirectory(lib/synap_postprocess)

# Sample SyNAP applications
add_subdirectory(app/synap_cli)
add_subdirectory(app/synap_cli_ic)
add_subdirectory(app/synap_cli_od)
add_subdirectory(app/synap_cli_yolov5)


