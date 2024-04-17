#
# NDA AND NEED-TO-KNOW REQUIRED
#
# Copyright (C) 2013-2021 Synaptics Incorporated. All rights reserved.
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

# CMake invokes the toolchain file twice during the first build, but only once
# during subsequent rebuilds. This trick avoids tp add compile or link option
# twice.
if(ANDROID_VSSDK_TOOLCHAIN_INCLUDED)
  return()
endif(ANDROID_VSSDK_TOOLCHAIN_INCLUDED)
set(ANDROID_VSSDK_TOOLCHAIN_INCLUDED true)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(ENABLE_64)
    set(CLANG_ARCH aarch64)
    set(CLANG_ABI android)
else()
    set(CLANG_ARCH armv7)
    set(CLANG_ABI androideabi)
endif()

# Clang target triple
set(CLANG_TARGET ${CLANG_ARCH}-none-linux-${CLANG_ABI}${VSSDK_CONFIG_RUNTIME_ANDROID_API_LEVEL})
message("CLANG_TARGET: ${CLANG_TARGET}")

set(CMAKE_C_COMPILER_TARGET ${CLANG_TARGET})
set(CMAKE_CXX_COMPILER_TARGET ${CLANG_TARGET})
set(CMAKE_ASM_COMPILER_TARGET ${CLANG_TARGET})

set(CMAKE_C_COMPILER ${VSSDK_CLANG_TOOLCHAIN_DIR}/bin/clang)
set(CMAKE_CXX_COMPILER ${VSSDK_CLANG_TOOLCHAIN_DIR}/bin/clang++)
set(CMAKE_SYSROOT ${ANDROID_TOOLCHAIN_SYSROOT})


# Align compile option with cmake-NDK options
add_compile_options(-g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security)

add_link_options(-fuse-ld=lld)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie")

# Some CMakeLists relies on that.
set(ANDROID ON)

# We are not using NDK toolchain so this is not set automatically.
# This is needed by some of the VSI libraries, for Android properties and
# things like that.
set(ANDROID_PLATFORM_LEVEL ${VSSDK_CONFIG_RUNTIME_ANDROID_API_LEVEL})

# Export configurable variables for the try_compile() command.
# toolchain file is used standalone by cmake when setting the C or C++
# compiler. This saves needed path variables across runs.
set(CMAKE_TRY_COMPILE_PLATFORM_VARIABLES
    VSSDK_CLANG_TOOLCHAIN_DIR
    ANDROID_TOOLCHAIN_SYSROOT
    GCC_TOOLCHAIN_DIR
    VSSDK_CONFIG_RUNTIME_ANDROID_API_LEVEL
    ENABLE_64
    )
