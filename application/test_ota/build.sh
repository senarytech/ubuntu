#!/bin/bash

source build/header.rc
source build/install.rc
source build/module/toolchain/${CONFIG_TOOLCHAIN_APPLICATION}.rc

mod_dir=$(dirname "$0")
sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_SYSYROOT}
rootfs=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_ROOTFS}
opt_intermediate_dir=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/test_ota
mkdir -p ${opt_intermediate_dir}

[ ${clean} -eq 1 ] && make clean -C ${mod_dir} BUILD_DIR="${opt_intermediate_dir}" && exit 0

cflags=${CONFIG_TOOLCHAIN_APPLICATION_CFLAG}
cflags="${cflags} --sysroot=${sysroot}"
cc_name="${CONFIG_TOOLCHAIN_APPLICATION}gcc"

if [ "${CONFIG_TOOLCHAIN_APPLICATION_CC}" = "clang" ]; then
    clangcflag=${CONFIG_TOOLCHAIN_CLANG_CFLAG}
    clangldflag=${CONFIG_TOOLCHAIN_CLANG_LDFLAG}
    cflags="${clangcflag} ${cflags}"
    ldflags="${clangcflag} ${clangldflag} ${ldflags}"
    cc_name="clang"
fi

cxx_name="${CONFIG_TOOLCHAIN_APPLICATION}g++"
ldflags="${ldflags} -L${sysroot}/usr/lib/ -lota_api"

make all -C ${mod_dir} CC="${cc_name}" CXX="${cxx_name}" E_CFLAG="${cflags}" E_LDFLAG="${ldflags}" BUILD_DIR="${opt_intermediate_dir}"

mkdir -p ${rootfs}/usr/bin
make install -C ${mod_dir} APP_DIR="${rootfs}/usr/bin/" BUILD_DIR="${opt_intermediate_dir}"
