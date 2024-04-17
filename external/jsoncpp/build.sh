#!/bin/bash
source build/header.rc
source build/install.rc
source build/module/toolchain/${CONFIG_TOOLCHAIN_APPLICATION}.rc

#############
# Functions #
#############

install_json (){

  gotoDir="mkdir -p ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/jsoncpp"
  gotoDir="${gotoDir} && cd ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/jsoncpp"

  args=""
  args="${args} -DCMAKE_BUILD_TYPE=none"
  args="${args} -DJSONCPP_WITH_TESTS=OFF"
  args="${args} -DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF"
  args="${args} -DBUILD_SHARED_LIBS=OFF"
  libjsoncpp_dest=${opt_workdir_build_sysroot}/usr/lib
if [ "is${CONFIG_ANDROID_OS}" = "isy" ]; then
  args="${args} -DCMAKE_TOOLCHAIN_FILE=${opt_src}/Toolchain-syna-android.cmake"
  args="${args} -DSYSROOT=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/build_sysroot"
  args="${args} -DTARGET=${CONFIG_TARGET}"
  libjsoncpp_dest+=/${ANDROID_SYSROOT_TARGET}/${CONFIG_RUNTIME_ANDROID_API_LEVEL}
else
  args="${args} -DCMAKE_TOOLCHAIN_FILE=${opt_src}/Toolchain-syna-linux.cmake"
fi
  args="${args} ../../../../external/jsoncpp"

  eval ${gotoDir} && cmake ${args} && make -j4

  INSTALL_D ${opt_src}/include/json ${opt_workdir_build_sysroot}/usr/include
  INSTALL_F ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/jsoncpp/src/lib_json/libjsoncpp.a ${libjsoncpp_dest}

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
    install_json &
    wait $!
fi
popd
