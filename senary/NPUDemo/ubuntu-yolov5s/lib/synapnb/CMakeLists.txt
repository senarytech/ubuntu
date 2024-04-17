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

option(ENABLE_OSAL_ALLOCATOR "Enable synap OSAL allocator" OFF)
option(ENABLE_ION_ALLOCATOR "Enable synap ION allocator" CMAKE_CROSSCOMPILING)
option(ENABLE_SECURE_DEBUG "Enable debugging secure allocator" OFF)

set(name synapnb)

file(GLOB SOURCES src/*.cpp android/*.cpp)
file(GLOB HEADERS inc/synap/*.hpp)
list(FILTER SOURCES EXCLUDE REGEX "debug.cpp")
if(NOT ENABLE_ION_ALLOCATOR)
    list(FILTER SOURCES EXCLUDE REGEX "ion.cpp")
    list(FILTER SOURCES EXCLUDE REGEX "ion_debug.cpp")
endif()
if(NOT ENABLE_OSAL_ALLOCATOR)
    list(FILTER SOURCES EXCLUDE REGEX "osal.cpp")
    list(FILTER SOURCES EXCLUDE REGEX "osal_secure.cpp")
    list(FILTER SOURCES EXCLUDE REGEX "osal_secure_debug.cpp")
endif()

add_library(${name} SHARED ${SOURCES})

#INCLUDE_DIRECTORIES(/home/liudongbo/NPU/project/dnnSynapUbuntu64Test/lib/synap_base/inc)

#INCLUDE_DIRECTORIES(/home/liudongbo/NPU/project/dnnSynapUbuntu64Test/lib/synap_base/inc/synap)


target_link_libraries(${name}
    PUBLIC synap_utils
    PUBLIC synap_base
    )




#find_package(synap_device REQUIRED)
target_link_libraries(${name}
    PUBLIC synap_device)

set(VSI_DIR ${CMAKE_SOURCE_DIR}/external/vsi_acuity/cmdtools)

target_include_directories(${name}
    PUBLIC inc
    PRIVATE android
    )

if(ENABLE_ION_ALLOCATOR)
    target_include_directories(${name}
        PRIVATE ${VSSDK_DIR}/ampsdk/osal/include/
        )
    target_compile_definitions(${name}
        PUBLIC ENABLE_ION_ALLOCATOR
    )
endif()

set_target_properties(${name} PROPERTIES PUBLIC_HEADER "${HEADERS}")

if(ENABLE_OSAL_ALLOCATOR AND BUILD_SYSROOT_AVAILABLE)
    target_link_libraries(${name} PRIVATE ampclient)
    target_compile_definitions(${name}
        PUBLIC ENABLE_OSAL_ALLOCATOR
    )
endif()

if(ENABLE_SECURE_DEBUG AND EXISTS ${CMAKE_CURRENT_LIST_DIR}/src/allocator_secure_debug.cmake)
    # Use debug allocator instead of standard one
    include( ${CMAKE_CURRENT_LIST_DIR}/src/allocator_secure_debug.cmake)
endif()

install(TARGETS ${name}
    LIBRARY DESTINATION ${LIB_INSTALL_SUBDIR}
    PUBLIC_HEADER DESTINATION ${INC_INSTALL_SUBDIR}/synap
    )

# install to the directory that generate IMAGE for VSSDK build
if("${SYNAP_BUILD_TYPE}" STREQUAL "VSSDK")
    install(TARGETS ${name}
        LIBRARY DESTINATION ${LIB_INSTALL_ROOTFS_DIR}
        PUBLIC_HEADER DESTINATION ${INC_INSTALL_SUBDIR}/synap
        )
endif()
