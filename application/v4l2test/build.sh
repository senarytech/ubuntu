#!/bin/bash

source build/header.rc
source build/install.rc
source build/module/toolchain/${CONFIG_TOOLCHAIN_APPLICATION}.rc

mod_dir=$(dirname "$0")
sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_SYSYROOT}
rootfs=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_ROOTFS}
opt_intermediate_dir=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/v4l2test
mkdir -p ${opt_intermediate_dir}
application_top_dir=${CONFIG_SYNA_SDK_PATH}/application

[ ${clean} -eq 1 ] && make clean -C ${mod_dir} BUILD_DIR="${opt_intermediate_dir}" && exit 0

cflags=${CONFIG_TOOLCHAIN_APPLICATION_CFLAG}
cflags="${cflags} --sysroot=${sysroot}"
cflags="${cflags} -D__LINUX__"
cflags="${cflags} -I${CONFIG_SYNA_SDK_PATH}/ampsdk/osal/include"
cflags="${cflags} -I${CONFIG_SYNA_SDK_PATH}/ampsdk/amp/src/hal/common/include"
cflags="${cflags} -I${CONFIG_SYNA_SDK_PATH}/ampsdk/amp/src/hal/common/include/Firmware_Berlin_VS680_Z1"

ldflags="${CONFIG_TOOLCHAIN_APPLICATION_LDFLAG}"
cc_name="${CONFIG_TOOLCHAIN_APPLICATION}gcc"
cxx_name="${CONFIG_TOOLCHAIN_APPLICATION}g++"

if [ "${CONFIG_TOOLCHAIN_APPLICATION_CC}" = "clang" ]; then
    clangcflag=${CONFIG_TOOLCHAIN_CLANG_CFLAG}
    clangldflag=${CONFIG_TOOLCHAIN_CLANG_LDFLAG}
    cflags="${clangcflag} ${cflags}"
    ldflags="${clangldflag} ${ldflags}"
    cc_name="clang"
    cxx_name="clang++"
fi

make all -C ${mod_dir} CC="${cc_name}" CXX="${cxx_name}" E_CFLAG="${cflags}" E_LDFLAG="${ldflags}" BUILD_DIR="${opt_intermediate_dir}" APPLICATION_TOP_DIR="${application_top_dir}"
make install -C ${mod_dir} APP_DIR="${rootfs}/usr/bin/" SYSROOT_DIR="${sysroot}/usr/" BUILD_DIR="${opt_intermediate_dir}" APPLICATION_TOP_DIR="${application_top_dir}"
