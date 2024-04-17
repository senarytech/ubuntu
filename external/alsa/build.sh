#!/bin/bash

echo "alsa build script"
source build/header.rc
source build/install.rc
source build/module/toolchain/${CONFIG_TOOLCHAIN_APPLICATION}.rc

VER_NUM=1.1.6
ALSALIB=alsa-lib-${VER_NUM}
ALSAUTILS=alsa-utils-${VER_NUM}

#############
# Functions #
#############

config_alsa_lib() {
  local exec2run
  local cmd_args

  exec2run="${opt_alsa_src}/${ALSALIB}/configure"
  toolchain=$(basename ${CONFIG_TOOLCHAIN_APPLICATION} -)
  cmd_args=""
  cmd_args="${cmd_args} --host=${toolchain}"
  cmd_args="${cmd_args} --prefix=/usr"
  cmd_args="${cmd_args} --with-sysroot=${opt_workdir_build_sysroot}"
  cmd_args="${cmd_args} --disable-python"

  eval $exec2run "$cmd_args"
}

config_alsa_utils() {
  local exec2run
  local cmd_args

  toolchain=$(basename ${CONFIG_TOOLCHAIN_APPLICATION} -)
  exec2run="${opt_alsa_src}/${ALSAUTILS}/configure"

  cmd_args=""
  cmd_args="${cmd_args} --host=${toolchain}"
  cmd_args="${cmd_args} --prefix=/usr"
  cmd_args="${cmd_args} --with-udev-rules-dir=/lib/udev/rules.d"
  cmd_args="${cmd_args} --disable-alsaconf"
  cmd_args="${cmd_args} --disable-alsamixer"
  cmd_args="${cmd_args} --disable-xmlto"
  cmd_args="${cmd_args} --disable-nls"

  eval $exec2run "$cmd_args"
}

compile_and_install() {
  # build and install
  make  -j ${CONFIG_CPU_NUMBER}
  make DESTDIR=${opt_workdir_runtime_sysroot} install
  make DESTDIR=${opt_workdir_build_sysroot} install
}

#############
# Submodule #
#############

### Common settings ###
outdir_alsa_lib_intermediate=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/alsa/lib
outdir_alsa_utils_intermediate=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/alsa/utils
mkdir -p ${outdir_alsa_lib_intermediate}
mkdir -p ${outdir_alsa_utils_intermediate}
opt_workdir_build_sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/build_sysroot
opt_workdir_runtime_sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system
opt_alsa_src=$(dirname $0)

export PKG_CONFIG_PATH=${opt_workdir_build_sysroot}/usr/lib/pkgconfig:${opt_workdir_build_sysroot}/lib/pkgconfig
E_CFLAGS="${CONFIG_TOOLCHAIN_APPLICATION_CFLAG}"
E_CFLAGS="${E_CFLAGS} -I${opt_workdir_build_sysroot}/usr/include -I${opt_workdir_build_sysroot}/include"
export CFLAGS="${E_CFLAGS}"
E_LDFLAGS=${CONFIG_TOOLCHAIN_APPLICATION_LDFLAG}
E_LDFLAGS="${E_LDFLAGS} -L${opt_workdir_build_sysroot}/lib -L${opt_workdir_build_sysroot}/usr/lib"
export LDFLAGS="${E_LDFLAGS}"

### Build ALSA ###
echo "Build and install ALSA clean:${clean}"

if [ ${clean} -eq 1 ];then
    rm -rf ${outdir_alsa_lib_intermediate}
    rm -rf ${outdir_alsa_utils_intermediate}
    exit 0
fi

# config, compile and install alsa-lib
pushd ${outdir_alsa_lib_intermediate}
    config_alsa_lib
    compile_and_install&
    wait $!
popd

# config, compile and install alsa-utils
pushd ${outdir_alsa_utils_intermediate}
    config_alsa_utils
    compile_and_install&
    wait $!
popd

### Clean up ###
unset -f compile_and_install
unset -f config_alsa_lib
unset -f config_alsa_utils
