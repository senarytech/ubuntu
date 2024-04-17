#!/bin/bash

echo "curl build script"
source build/header.rc
source build/install.rc
source build/module/toolchain/${CONFIG_TOOLCHAIN_APPLICATION}.rc

[ "is${CONFIG_CURL}" = "isy" ] || (echo "curl disabled" && exit 0)

#############
# Functions #
#############

config_curl() {
  local exec2run
  local cmd_args

  pushd ${opt_curl_src}
  aclocal
  automake --add-missing
  autoreconf
  popd

  exec2run="${opt_curl_src}/configure"

  toolchain=$(basename ${CONFIG_TOOLCHAIN_APPLICATION} -)
  cmd_args=""
  cmd_args="${cmd_args} --host=${toolchain}"
  cmd_args="${cmd_args} --enable-shared"
  cmd_args="${cmd_args} --prefix=${opt_workdir_build_sysroot}/usr"
  cmd_args="${cmd_args} --enable-optimize"
  cmd_args="${cmd_args} --enable-http"
  cmd_args="${cmd_args} --disable-verbose"
  cmd_args="${cmd_args} --disable-dict"
  cmd_args="${cmd_args} --disable-crypto-auth"
  cmd_args="${cmd_args} --disable-file"
  cmd_args="${cmd_args} --disable-ftp"
  cmd_args="${cmd_args} --disable-imap"
  cmd_args="${cmd_args} --disable-ldap"
  cmd_args="${cmd_args} --disable-ldaps"
  cmd_args="${cmd_args} --disable-pop3"
  cmd_args="${cmd_args} --disable-proxy"
  cmd_args="${cmd_args} --disable-rtsp"
  cmd_args="${cmd_args} --disable-smtp"
  cmd_args="${cmd_args} --disable-telnet"
  cmd_args="${cmd_args} --disable-tftp"
  cmd_args="${cmd_args} --disable-gopher"
  cmd_args="${cmd_args} --disable-smb"
  cmd_args="${cmd_args} --disable-ipv6"
  cmd_args="${cmd_args} --with-ca-bundle=/etc/ssl/certs/ca-certificates.crt"

  if [ "is${CONFIG_LINUX_OS}" = "isy" ]; then
    cmd_args="${cmd_args} CFLAGS='${CONFIG_TOOLCHAIN_APPLICATION_CFLAG}'"
  else
    cmd_args="${cmd_args} LDFLAGS='--sysroot=${opt_workdir_build_sysroot}'"
    cmd_args="${cmd_args} CFLAGS='${CONFIG_TOOLCHAIN_APPLICATION_CFLAG} --sysroot=${opt_workdir_build_sysroot}'"
  fi

  eval $exec2run "$cmd_args"
}

compile_and_install_curl() {
  # Config, build and install
  config_curl

  make -j ${CONFIG_CPU_NUMBER}
  make install

  # Copy results
  [ "x${opt_workdir_build_sysroot}" != "x" ]

  mkdir -p ${opt_workdir_build_sysroot}/usr/lib
  mkdir -p ${opt_workdir_build_sysroot}/usr/include
  mkdir -p ${opt_workdir_runtime_sysroot}/usr/lib

  INSTALL_F ${opt_workdir_build_sysroot}/usr/lib/libcurl.so ${opt_workdir_runtime_sysroot}/usr/lib
  if [ -f ${opt_workdir_build_sysroot}/usr/lib/libcurl.so.4.5.0 ]; then
    INSTALL_F ${opt_workdir_build_sysroot}/usr/lib/libcurl.so.4.5.0 ${opt_workdir_runtime_sysroot}/usr/lib
  fi
}

#############
# Submodule #
#############

### Common settings ###
outdir_curl_intermediate=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/curl
mkdir -p ${outdir_curl_intermediate}
opt_workdir_build_sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/build_sysroot
opt_workdir_runtime_sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system
opt_curl_src=${topdir}/external/curl


### Build CURL ###
echo "Build and install CURL clean:${clean}"

if [ ${clean} -eq 1 ];then
    rm -rf ${outdir_curl_intermediate}
    rm -f ${opt_curl_src}/Makefile
    rm -f ${opt_curl_src}/Makefile.in
    rm -f ${opt_curl_src}/config.cache
    rm -f ${opt_curl_src}/config.guess

    exit 0
fi

pushd ${outdir_curl_intermediate}
    compile_and_install_curl&
    wait $!
popd

### Clean up ###
unset -f compile_and_install_curl
unset -f config_curl
