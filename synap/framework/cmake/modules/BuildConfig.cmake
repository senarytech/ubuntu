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

macro(CacheVar var v description)
    set(${var} ${v} CACHE INTERNAL "${description}" FORCE)
endmacro()

# SYNAP_BUILD_TYPE
# There are 5 different build type for different request currently
# SyNAP, VSSDK, VSSDK_REL, VSSDK_LINUX_HOST, VSSDK_LINUX_HOST_REL
# - SyNAP: build independently from VSSDK
# - VSSDK, VSSDK_REL, VSSDK_LINUX_HOST, VSSDK_LINUX_HOST_REL
#   the maine difference between VSSDK_* is install directory.
#   - VSSDK_*: engineering build, generally install headers/libs/bins
#              to build_sysroot and rootfs
#   - VSSDK_*_REL: release build, install to rel-package, except headers/libs/bins,
#              some source code and resources need to install as well.
#              *_REL instal prefix is `/syna-release/synap` in order to install different resources.
#   - VSSDK_LINUX_HOST: engineering host build, install to /vssdk/synap/release
#   - VSSDK_LINUX_HOST_REL: release bulid, install into /syna-release/synap/release
# - Suggest to add a install path variable in below InstallConfig() in case confusion.

function(InstallConfig buildType targetOs)
    set(CMAKE_INSTALL_MESSAGE LAZY CACHE INTERNAL "..." FORCE)

    if(${buildType} STREQUAL "SyNAP")

        if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
            set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "..." FORCE)
        endif()

    elseif(${buildType} STREQUAL "VSSDK")
        set(CMAKE_INSTALL_PREFIX ${VSSDK_BUILD_TARGET_DIR} CACHE PATH "..." FORCE)

        CacheVar(LIB_INSTALL_SUBDIR ${VSSDK_CONFIG_SYNA_SDK_OUT_SYSYROOT}/usr/lib "")
        CacheVar(LIB_INSTALL_ROOTFS_DIR ${VSSDK_CONFIG_SYNA_SDK_OUT_ROOTFS}/usr/lib "")

        if(${targetOs} MATCHES "android")
            CacheVar(BIN_INSTALL_SUBDIR ${VSSDK_CONFIG_SYNA_SDK_OUT_ROOTFS}/home/galois/bin "")
        elseif(${targetOs} MATCHES "linux")
            CacheVar(BIN_INSTALL_SUBDIR ${VSSDK_CONFIG_SYNA_SDK_OUT_ROOTFS}/opt/syna/bin "")
        elseif(${targetOs} MATCHES "oe")
            CacheVar(BIN_INSTALL_SUBDIR ${VSSDK_CONFIG_SYNA_SDK_OUT_ROOTFS}/opt/syna/bin "")
        else()
            message(FATAL "unknown OS:${TARGET_OS}")
        endif()

        CacheVar(INC_INSTALL_SUBDIR ${VSSDK_CONFIG_SYNA_SDK_OUT_SYSYROOT}/include "")

        return()

    elseif(${buildType} MATCHES "_REL")

        set(CMAKE_INSTALL_PREFIX ${VSSDK_BUILD_REL_DIR}/synap CACHE PATH "..." FORCE)
        CacheVar(LIB_INSTALL_SUBDIR release/lib/${TARGET} "")
        CacheVar(BIN_INSTALL_SUBDIR release/bin/${TARGET} "")
        CacheVar(INC_INSTALL_SUBDIR release/include "")
        CacheVar(VXK_INSTALL_SUBDIR release/vxk "VX Kernel install path")
        return()

    elseif(${buildType} STREQUAL "VSSDK_LINUX_HOST")
        set(CMAKE_INSTALL_PREFIX ${VSSDK_DIR}/synap/release CACHE PATH "..." FORCE)
    else()
        message(FATAL "${SYNAP_BUILD_TYPE} unsupported")
    endif()

    CacheVar(LIB_INSTALL_SUBDIR lib/${TARGET} "")
    CacheVar(BIN_INSTALL_SUBDIR bin/${TARGET} "")
    CacheVar(INC_INSTALL_SUBDIR include "")
    CacheVar(VXK_INSTALL_SUBDIR vxk "VX Kernel install path")
endfunction()

function(BuildConfig buildType curPlatform callInstallConf)

    if(${buildType} STREQUAL "VSSDK_REL")
        set(VSSDK_CONFIG_FILE ${VSSDK_DIR}/build/.config.rel.synap)
    endif()

    find_package(VSSDK REQUIRED)

    if(ENABLE_64)
        set(TARGET_CPU aarch64)
    else()
        set(TARGET_CPU armv7a)
    endif()

    if(${curPlatform} MATCHES "android")
        CacheVar(TARGET_OS android_vssdk "")
        CacheVar(TARGET ${TARGET_CPU}-${TARGET_OS} "")
    elseif(${curPlatform} MATCHES "linux_baseline")
        CacheVar(TARGET_OS linux_baseline "")
        CacheVar(TARGET ${TARGET_CPU}-${TARGET_OS} "")
    elseif(${curPlatform} MATCHES "oe")

        if (NOT DEFINED ENV{OECORE_TARGET_ARCH})
            message(FATAL_ERROR "Missing OE environment variables, did you source the environment file?")
        endif()

        CacheVar(TARGET_OS oe "")
        CacheVar(TARGET ${TARGET_CPU}-${TARGET_OS} "")
    elseif(${curPlatform} MATCHES "linux_host")
        CacheVar(TARGET_OS linux_host "")
        CacheVar(TARGET "x86_64-linux-gcc" "")
    else()
        message(FATAL "unsupported platform")
    endif()

    message(STATUS "Selected TARGET_OS: ${TARGET_OS} TARGET: ${TARGET}")

    if(callInstallConf)
        InstallConfig(${buildType} ${TARGET_OS})
    endif()

    CacheVar(CMAKE_POSITION_INDEPENDENT_CODE ON "")

    # toolchain is passed by the build script for OE builds
    if(${curPlatform} MATCHES "oe")
        message(STATUS "Using external toolchain file provided by OE at ${CMAKE_TOOLCHAIN_FILE}")
        return()
    endif()

    if(${buildType} MATCHES "VSSDK")
        set(TOOLCHAIN_DIR ${VSSDK_DIR}/synap/framework/cmake/toolchain)
    else() #SyNAP
        set(TOOLCHAIN_DIR ${CMAKE_CURRENT_LIST_DIR}/../../cmake/toolchain)
    endif()

    CacheVar(CMAKE_TOOLCHAIN_FILE ${TOOLCHAIN_DIR}/${TARGET_OS}_toolchain.cmake "")
endfunction()

