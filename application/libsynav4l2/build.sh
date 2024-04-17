#!/bin/bash

source build/header.rc
source build/chip.rc
source build/install.rc
source build/module/toolchain/${CONFIG_TOOLCHAIN_APPLICATION}.rc

mod_dir=$(dirname "$0")
sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_SYSYROOT}
rootfs=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_ROOTFS}
opt_intermediate_dir=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/libsynav4l2
mkdir -p ${opt_intermediate_dir}
application_top_dir=${CONFIG_SYNA_SDK_PATH}/application

[ ${clean} -eq 1 ] && make clean -C ${mod_dir} BUILD_DIR="${opt_intermediate_dir}" && exit 0

cflags=${CONFIG_TOOLCHAIN_APPLICATION_CFLAG}
cflags="${cflags} --sysroot=${sysroot}"
if [ "is${CONFIG_ANDROID_OS}" = "isy" ]; then
    cflags="${cflags} -DANDROID"
fi
if [ "is${CONFIG_AMP_RELEASE_BUILD}" = "isy" ]; then
    cflags="${cflags} -DAMP_RELEASE_BUILD"
fi

# chip flags

cflags="${cflags} -DBERLIN_VS680=0x3130 \
	-DBERLIN_VS680_A0=0x3150"

# uncomment below line for videowall support
#cflags="${cflags} -DVIDEO_WALL_MODE"

if [ "${syna_chip_name}${syna_chip_rev}" = "vs680Z1" ]; then
    cflags="${cflags} -DBERLIN_CHIP_VERSION=BERLIN_VS680"
fi
if [ "${syna_chip_name}${syna_chip_rev}" = "vs680A0" ]; then
    cflags="${cflags} -DBERLIN_CHIP_VERSION=BERLIN_VS680_A0"
fi

cflags="${cflags} -Wno-undefined-bool-conversion"
cflags="${cflags} -I${topdir}/amp/inc"
cc_name="${CONFIG_TOOLCHAIN_APPLICATION}gcc"
cxx_name="${CONFIG_TOOLCHAIN_APPLICATION}g++"

ldflags="${CONFIG_TOOLCHAIN_APPLICATION_LDFLAG}"
ldflags="${ldflags} -lampclient"

if [ "is${CONFIG_ANDROID_OS}" = "isy" ]; then
    ldflags="${ldflags} -ltinyxml2"
else
    if [ "is${CONFIG_RUNTIME_OE64}" = "isy" ]; then
        ldflags="${ldflags} -ltinyxml2"
    else
        if [ -z "${CONFIG_RUNTIME_RDK}" ];then
            ldflags="${ldflags} -ltinyxml2"
        else
            ldflags="${ldflags} -ltinyxml2"
        fi
    fi
fi

if [ "${CONFIG_TOOLCHAIN_APPLICATION_CC}" = "clang" ]; then
    clangcflag=${CONFIG_TOOLCHAIN_CLANG_CFLAG}
    clangldflag=${CONFIG_TOOLCHAIN_CLANG_LDFLAG}
    cflags="${clangcflag} ${cflags}"
    ldflags="${clangcflag} ${clangldflag} ${ldflags}"
    cc_name="clang"
    cxx_name="clang++"
fi

make all -C ${mod_dir} CC="${cc_name}" CXX="${cxx_name}" E_CFLAG="${cflags}" E_LDFLAG="${ldflags}" SYSROOT_DIR="${sysroot}/usr/" BUILD_DIR="${opt_intermediate_dir}" APPLICATION_TOP_DIR="${application_top_dir}"
make install -C ${mod_dir} LIB_DIR="${rootfs}/opt/syna/lib64/" SYSROOT_DIR="${sysroot}/usr/" BUILD_DIR="${opt_intermediate_dir}" APPLICATION_TOP_DIR="${application_top_dir}"
