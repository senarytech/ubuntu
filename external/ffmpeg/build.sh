#!/bin/bash
source build/header.rc
source build/install.rc
source build/module/toolchain/${CONFIG_TOOLCHAIN_APPLICATION}.rc

#############
# Functions #
#############

config_ffmpeg() {
  local exec2run
  local cmd_args

  exec2run="${opt_ffmpeg_src}/configure"

  cmd_args=""
  cmd_args="${cmd_args} --enable-cross-compile"
  cmd_args="${cmd_args} --cross-prefix=${CONFIG_TOOLCHAIN_APPLICATION}"
  cmd_args="${cmd_args} --enable-shared"
  cmd_args="${cmd_args} --prefix=${outdir_ffmpeg_intermediate}"
  cmd_args="${cmd_args} --disable-static"
  cmd_args="${cmd_args} --disable-programs"
  cmd_args="${cmd_args} --disable-doc"
  cmd_args="${cmd_args} --disable-avdevice"
  cmd_args="${cmd_args} --disable-swresample"
  cmd_args="${cmd_args} --disable-swscale"
  cmd_args="${cmd_args} --disable-avfilter"
  cmd_args="${cmd_args} --disable-muxers"
  cmd_args="${cmd_args} --disable-bsfs"
  cmd_args="${cmd_args} --cpu=armv8-a"
  if [[ ${CONFIG_TOOLCHAIN_APPLICATION} =~ "aarch64-" ]]; then
      cmd_args="${cmd_args} --arch=aarch64"
  else
      cmd_args="${cmd_args} --arch=arm"
      cmd_args="${cmd_args} --enable-neon"
  fi
  cmd_args="${cmd_args} --disable-ffplay"
  cmd_args="${cmd_args} --disable-decoders"
  cmd_args="${cmd_args} --enable-decoder=vc1"
  cmd_args="${cmd_args} --disable-encoders"
  cmd_args="${cmd_args} --disable-hwaccels"
  cmd_args="${cmd_args} --enable-bsf=h264_mp4toannexb"
  cmd_args="${cmd_args} --enable-bsf=hevc_mp4toannexb"
  cmd_args="${cmd_args} --disable-indevs"
  cmd_args="${cmd_args} --disable-outdevs"
  cmd_args="${cmd_args} --disable-stripping"

  # clang build
  if [ "${CONFIG_TOOLCHAIN_APPLICATION_CC}" = "clang" ]; then
    cmd_args="${cmd_args} --cc=clang"
    cmd_args="${cmd_args} --cxx=clang++"
    cmd_args="${cmd_args} --extra-cflags='${CONFIG_TOOLCHAIN_CLANG_CFLAG} ${CONFIG_TOOLCHAIN_APPLICATION_CFLAG}'"
    cmd_args="${cmd_args} --extra-libs='${CONFIG_TOOLCHAIN_CLANG_CFLAG} ${CONFIG_TOOLCHAIN_CLANG_LDFLAG} ${CONFIG_TOOLCHAIN_APPLICATION_LDFLAG}'"
  else
    cmd_args="${cmd_args} --cc=${CONFIG_TOOLCHAIN_APPLICATION}gcc --sysroot=${opt_workdir_build_sysroot}"
    cmd_args="${cmd_args} --extra-cflags='${CONFIG_TOOLCHAIN_APPLICATION_CFLAG}'"
    cmd_args="${cmd_args} --extra-libs='${CONFIG_TOOLCHAIN_APPLICATION_LDFLAG}'"
  fi
  # androideabi toochain
  if [ "${CONFIG_TOOLCHAIN_APPLICATION}" = "arm-linux-androideabi-" ]; then
    cmd_args="${cmd_args} --target-os=android"
  else
    cmd_args="${cmd_args} --target-os=linux"
  fi

  if [ "is${CONFIG_ANDROID_OS}" = "isy" ]; then
    cmd_args="${cmd_args} --sysroot=${opt_workdir_build_sysroot}"
  fi

  eval $exec2run "$cmd_args"
}

compile_and_install_ffmpeg() {
  # Config, build and install
  if [ -f config.mak ]; then
    echo "config file existed"
  else
    config_ffmpeg
  fi

  make -j ${CONFIG_CPU_NUMBER}
  make install

  # Copy results
  [ "x${opt_workdir_build_sysroot}" != "x" ]

  mkdir -p ${opt_workdir_build_sysroot}/usr/lib
  mkdir -p ${opt_workdir_build_sysroot}/usr/include
  mkdir -p ${opt_workdir_runtime_sysroot}/usr/lib

  if [ "${CONFIG_TOOLCHAIN_APPLICATION}" = "aarch64-linux-gnu-" ];then
      INSTALL_D ${outdir_ffmpeg_intermediate}/lib/  ${opt_workdir_build_sysroot}/usr/lib64/
      INSTALL_D ${outdir_ffmpeg_intermediate}/lib/ ${opt_workdir_runtime_sysroot}/usr/lib64/
      INSTALL_D ${outdir_ffmpeg_intermediate}/include ${opt_workdir_build_sysroot}/usr/
  else
      INSTALL_D ${outdir_ffmpeg_intermediate}/lib ${opt_workdir_build_sysroot}/usr/
      INSTALL_D ${outdir_ffmpeg_intermediate}/lib ${opt_workdir_runtime_sysroot}/usr/
      INSTALL_D ${outdir_ffmpeg_intermediate}/include ${opt_workdir_build_sysroot}/usr/
  fi
}

#############
# Submodule #
#############

### Common settings ###
outdir_ffmpeg_intermediate=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/ffmpeg
mkdir -p ${outdir_ffmpeg_intermediate}
opt_workdir_build_sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/build_sysroot
opt_workdir_runtime_sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system
opt_ffmpeg_src=${topdir}/external/ffmpeg

### Build FFMPEG ###
echo "Build and install FFMPEG clean:${clean}"

pushd ${outdir_ffmpeg_intermediate}

if [ ${clean} -eq 0 ];then
    compile_and_install_ffmpeg &
    wait $!
else
    [ -f config.mak ] && make clean
fi
popd

### Clean up ###
unset -f compile_and_install_ffmpeg
unset -f config_ffmpeg
