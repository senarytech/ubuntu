#!/bin/bash

source build/header.rc
source build/install.rc
source build/module/toolchain/${CONFIG_TOOLCHAIN_APPLICATION}.rc

mod_dir=$(dirname "$0")
sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_SYSYROOT}
rootfs=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_ROOTFS}
opt_intermediate_dir=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/app_smbox
mkdir -p ${opt_intermediate_dir}

[ ${clean} -eq 1 ] && make clean -C ${mod_dir} BUILD_DIR="${opt_intermediate_dir}" && exit 0

cflags=${CONFIG_TOOLCHAIN_APPLICATION_CFLAG}
cflags="${cflags} -I${sysroot}/usr/include/amp/"
cflags="${cflags} -I${sysroot}/usr/include/osal/"
ldflags=${CONFIG_TOOLCHAIN_APPLICATION_LDFLAG}
cc_name="${CONFIG_TOOLCHAIN_APPLICATION}gcc"

if [ "${CONFIG_TOOLCHAIN_APPLICATION_CC}" = "clang" ]; then
    clangcflag=${CONFIG_TOOLCHAIN_CLANG_CFLAG}
    clangldflag=${CONFIG_TOOLCHAIN_CLANG_LDFLAG}
    cflags="${clangcflag} ${cflags}"
    ldflags="${clangcflag} ${clangldflag} ${ldflags}"
    cc_name="clang"
fi

if [ "is${CONFIG_ANDROID_OS}" = "isy" ]; then
  cflags="${cflags} --sysroot=${sysroot}"
  ldflags="${ldflags} -lcutils -Wl,-pie"
else
  if [ "is${CONFIG_TOOLCHAIN_APPLICATION}" = "isaarch64-linux-gnu-" ]; then
     ldflags="${ldflags} -L${sysroot}/lib -L${sysroot}/usr/lib64"
  else
     ldflags="${ldflags} -L${sysroot}/lib -L${sysroot}/usr/lib"
  fi
fi

make all -C ${mod_dir} CC="${cc_name}" E_CFLAG="${cflags}" E_LDFLAG="${ldflags}" BUILD_DIR="${opt_intermediate_dir}"
mkdir -p ${rootfs}/usr/bin
make install -C ${mod_dir} APP_DIR="${rootfs}/usr/bin/" BUILD_DIR="${opt_intermediate_dir}"
