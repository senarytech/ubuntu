#!/bin/bash

source build/header.rc
source build/install.rc
source build/module/toolchain/${CONFIG_TOOLCHAIN_APPLICATION}.rc

#############
# Functions #
#############

install_boost (){
  local runb2
  local gotoDir
  local bootstrap
  local b2args

  if [ "is${CONFIG_TOOLCHAIN_APPLICATION_CC}" = "isclang" ]; then
    b2args="${b2args} -d+2 -j 2 --reconfigure target-os=android"
    b2args="${b2args} toolset=clang-arm link=shared variant=release"
    b2args="${b2args} threading=multi --with-system"
    b2args="${b2args} --user-config=syna-config-android.jam"
    b2args="${b2args} -s --sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/build_sysroot/"
    b2args="${b2args} -s --target=${CONFIG_TARGET}"
  else
    b2args="${b2args} toolset=gcc-arm"
    b2args="${b2args} --user-config=syna-config-linux.jam"
  fi
  b2args="${b2args} --build-dir=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/boost"
  b2args="${b2args} --stagedir=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/boost"

  export BOOST_ROOT=${opt_src}/

  gotoDir="cd ${opt_src}"
  bootstrap="./bootstrap.sh --with-libraries=system"
  runb2="./b2"

  eval ${gotoDir} && ${bootstrap} && ${runb2} ${b2args}

  if [ "is${CONFIG_ANDROID_OS}" = "is" ]; then
    INSTALL_F ${outdir_intermediate}/lib/libboost_system.so ${opt_workdir_runtime_sysroot}/usr/lib
    INSTALL_F ${outdir_intermediate}/lib/libboost_system.so.1 ${opt_workdir_runtime_sysroot}/usr/lib
    INSTALL_F ${outdir_intermediate}/lib/libboost_system.so.1.71 ${opt_workdir_runtime_sysroot}/usr/lib
    INSTALL_F ${outdir_intermediate}/lib/libboost_system.so.1.71.0 ${opt_workdir_runtime_sysroot}/usr/lib
  else
    # copy library without softlinks
    cp -f ${outdir_intermediate}/lib/libboost_system.so ${opt_workdir_runtime_sysroot}/usr/lib
  fi
  INSTALL_D ${opt_src}/boost ${opt_workdir_build_sysroot}/usr/include

}

#############
# Submodule #
#############

### Common settings ###
opt_src=$(dirname $0)
module_name=$(basename ${opt_src})
outdir_intermediate=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${module_name}
mkdir -p ${outdir_intermediate}
opt_workdir_build_sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/build_sysroot
opt_workdir_runtime_sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system


### Build ###
echo "Build and install ${module_name} clean:${clean}"

pushd ${outdir_intermediate}

if [ ${clean} -eq 0 ];then
    install_boost &
    wait $!
else
    rm -f ${opt_src}/bootstrap.log
    rm -f ${opt_src}/project-config.jam
    rm -f ${opt_src}/tools/build/src/engine/b2
    rm -f ${opt_src}/tools/build/src/engine/bjam
fi
popd

### Clean up ###
unset -f install_boost
