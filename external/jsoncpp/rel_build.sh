#!/bin/bash

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
  args="${args} ../../../../external/jsoncpp"

  libjsoncpp_dest=${opt_workdir_build_sysroot}/usr/lib
if [ $cur_runtime = "ANDROID" ]; then
  args="${args} -DCMAKE_TOOLCHAIN_FILE=${opt_src}/Toolchain-syna-android.cmake"
  args="${args} -DSYSROOT=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_SYSYROOT}"
  args="${args} -DTARGET=${CONFIG_TARGET}"
  libjsoncpp_dest+=/${ANDROID_SYSROOT_TARGET}/${CONFIG_RUNTIME_ANDROID_API_LEVEL}
fi

if [ ${cur_runtime} = "LINUX" ]; then
  args="${args} -DCMAKE_TOOLCHAIN_FILE=${opt_src}/Toolchain-syna-linux.cmake"
fi

  eval ${gotoDir} && cmake ${args} && make -j4
  INSTALL_D ${opt_src}/include/json ${opt_workdir_build_sysroot}/usr/include
  INSTALL_F ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/jsoncpp/src/lib_json/libjsoncpp.a ${libjsoncpp_dest}
  eval ${gotoDir} &&  find . -maxdepth 1 ! -name lib -type d -not -path '.' -exec rm -rf {} +
}

#############
# Submodule #
#############

### Common settings ###
opt_src=$(dirname $0)
module_name=$(basename ${opt_src})
outdir_intermediate=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${module_name}
mkdir -p ${outdir_intermediate}
opt_workdir_build_sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_SYSYROOT}
opt_workdir_runtime_sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system


### Build ###
echo "Build and install ${module_name} clean:${clean}"

pushd ${outdir_intermediate}
if [ ${clean} -eq 0 ];then
    install_json &
    wait $!
fi
popd
