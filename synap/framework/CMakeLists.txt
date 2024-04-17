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
# or implied licenses to any intellectual property rights belonging`
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

# framework is a standalone project when build inside vssdk
if("${SYNAP_BUILD_TYPE}" MATCHES "VSSDK")
    list(APPEND CMAKE_MODULE_PATH ${VSSDK_DIR}/synap/framework/cmake/modules)
    include(${CMAKE_MODULE_PATH}/BuildConfig.cmake)
    BuildConfig(${SYNAP_BUILD_TYPE} ${CUR_RUNTIME_PLATFORM} ON)
    if("${SYNAP_BUILD_TYPE}" STREQUAL "VSSDK")
        set(CMAKE_CXX_FLAGS "--sysroot=${BUILD_SYSROOT_ABS}")
    endif()
    cmake_minimum_required(VERSION 3.19)
    project(SyNAP_FRAMEWORK)
endif()

if("${SYNAP_BUILD_TYPE}" MATCHES "yocto")
    list(APPEND CMAKE_MODULE_PATH ${VSSDK_DIR}/synap/framework/cmake/modules)

    # for yocto we install the files in the standard cmake locations
    set(LIB_INSTALL_SUBDIR "${CMAKE_INSTALL_LIBDIR}")
    set(BIN_INSTALL_SUBDIR "${CMAKE_INSTALL_BINDIR}")
    set(INC_INSTALL_SUBDIR "${CMAKE_INSTALL_INCLUDEDIR}")

    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    cmake_minimum_required(VERSION 3.19)
    project(SyNAP_FRAMEWORK)
endif()

option(ENABLE_DOC "Enable documents generator" OFF)
message("ENABLE_DOC: ${ENABLE_DOC} (document generation)")

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib)
add_subdirectory(app)

if(ENABLE_DOC)
    add_subdirectory(doc)
endif()

if("${SYNAP_BUILD_TYPE}" STREQUAL "VSSDK_REL")

    install(FILES CMakeLists.txt DESTINATION framework)
    install(DIRECTORY cmake DESTINATION framework)
    install(DIRECTORY release DESTINATION framework)
    install(DIRECTORY java DESTINATION framework)

    file(GLOB BUILDSCRIPTS build/*)
    install(FILES ${BUILDSCRIPTS}
        PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE OWNER_READ GROUP_READ
        DESTINATION framework/build)

endif()
