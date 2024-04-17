#!/bin/bash

source build/header.rc
source build/install.rc

mod_dir=${CONFIG_SYNA_SDK_PATH}/synap

[ $clean -eq 1 ] && exit 0


## build_sysroot  global search path for build
build_sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_SYSYROOT}

##rootfs  files will copy to sdk lib and finally in image
rootfs=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_ROOTFS}
rootfs_install=${rootfs}/usr

mod_dir=$(dirname $0)

if [ "is$CONFIG_RUNTIME_LINUX_BASELINE_BUILDROOT" -o "is$CONFIG_RUNTIME_UBUNTU" = "isy" ]; then
    models_install_dir=${rootfs}/opt/syna/fw/models
    vxk_install_dir=${rootfs}/opt/syna/fw/vxk
    bin_install_dir=${rootfs}/opt/syna/bin
    target_os=linux_baseline
elif [ "is$CONFIG_RUNTIME_OE" = "isy" -o "is$CONFIG_RUNTIME_OE64" = "isy" ]; then
    models_install_dir=${rootfs}/opt/syna/fw/models
    vxk_install_dir=${rootfs}/opt/syna/fw/vxk
    bin_install_dir=${rootfs}/opt/syna/bin
    target_os=oe
else
    models_install_dir=${rootfs}/home/galois/firmware/models
    vxk_install_dir=${rootfs}/home/galois/firmware/vxk
    bin_install_dir=${rootfs}/home/galois/bin
    target_os=android_vssdk
fi

if [[ ${CONFIG_TOOLCHAIN_APPLICATION} == *"aarch64"* ]];then
    aarch="aarch64"
else
    aarch="armv7a"
fi

synap_inc=${mod_dir}/include
synap_lib=${mod_dir}/lib/${aarch}-${target_os}
synap_bin=${mod_dir}/bin/${aarch}-${target_os}
synap_3rd_bin=${mod_dir}/bin/${aarch}-android

if [ "is${CONFIG_BERLIN_VS640_A0}" = "isy" ]; then
    chip_c="VS640A0"
elif [ "is${CONFIG_BERLIN_VS680_A0}" = "isy" ]; then
    chip_c="VS680A0"
else
    echo "error! synap not supported!"
    exit 1
fi

# install path
build_sysroot_usr_lib=${build_sysroot}/usr/lib
build_sysroot_inc=${build_sysroot}/include
rootfs_lib=${rootfs_install}/lib

mkdir -p ${build_sysroot_usr_lib}
mkdir -p ${build_sysroot_inc}
mkdir -p ${rootfs_lib}
mkdir -p ${bin_install_dir}

#install synap if needed
if [[ -d ${synap_inc} ]];then
  cp -rf ${synap_inc}/* ${build_sysroot_inc}
fi

if [[ -d ${synap_bin} ]]; then
  cp ${synap_bin}/* ${bin_install_dir}
fi

if [[ -d ${synap_3rd_bin} ]] && [[ "is${CONFIG_RUNTIME_ANDROID}" = "isy" ]]; then
  cp ${synap_3rd_bin}/* ${bin_install_dir}
fi

if [[ -d ${synap_lib} ]];then
  rsync -az ${synap_lib}/* ${build_sysroot_usr_lib}
  rsync -az ${synap_lib}/* ${rootfs_lib}
fi

#install models
if [ -d "${mod_dir}/models/${chip_c}" ]; then
    mkdir -p ${models_install_dir}
    cp -adprf ${mod_dir}/models/${chip_c}/* ${models_install_dir}/.
fi

#install vxk
if [ -d "${mod_dir}/vxk/${chip_c}" ]; then
    mkdir -p ${vxk_install_dir}
    cp -adprf ${mod_dir}/vxk/${chip_c}/* ${vxk_install_dir}/.
fi


echo "install synap done"
