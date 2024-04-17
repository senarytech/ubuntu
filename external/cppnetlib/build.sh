#!/bin/bash
source build/header.rc
source build/install.rc
source build/module/toolchain/${CONFIG_TOOLCHAIN_APPLICATION}.rc

#############
# Functions #
#############

install_cpp-netlib (){

  gotoDir="mkdir -p ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/cppnetlib"
  gotoDir="${gotoDir} && cd ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/cppnetlib"

  args=""
  args="${args} -DCMAKE_BUILD_TYPE=-Wno-dev"
  args="${args} -DBOOST_ROOT=${opt_src}/../boost"
  args="${args} -DBOOST_LIBRARYDIR=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/boost/lib"
  args="${args} -DCPP-NETLIB_BUILD_TESTS=OFF"
  args="${args} -DCPP-NETLIB_BUILD_EXAMPLES=OFF"
  args="${args} -DCPP-NETLIB_ENABLE_HTTPS=OFF"
  args="${args} -DCPPNETLIB_INCLUDE_DIRS=${opt_src}/../boost/boost"
  args="${args} -DBUILD_SHARED_LIBS=ON"

if [ "is${CONFIG_ANDROID_OS}" = "isy" ]; then
  args="${args} -DCMAKE_TOOLCHAIN_FILE=${opt_src}/Toolchain-syna-android.cmake"
  args="${args} -DTARGET=${CONFIG_TARGET}"
else
  args="${args} -DCMAKE_TOOLCHAIN_FILE=${opt_src}/Toolchain-syna-linux.cmake"
fi

  args="${args} ${opt_src}"
  args="${args} -DSYSROOT=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/build_sysroot"
  eval ${gotoDir} && cmake ${args} && make -j4

  INSTALL_F ${outdir_intermediate}/libs/network/src/libcppnetlib-client-connections.so ${opt_workdir_runtime_sysroot}/usr/lib
  INSTALL_F ${outdir_intermediate}/libs/network/src/libcppnetlib-server-parsers.so ${opt_workdir_runtime_sysroot}/usr/lib
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
    install_cpp-netlib &
    wait $!
fi
popd
