#!/bin/bash

source build/header.rc
source build/install.rc
source build/chip.rc

[ $clean -eq 1 ] && exit 0
mod_dir=${topdir}/sysroot
echo "${mod_dir} build script called"

if [ "is$CONFIG_RUNTIME_ANDROID" = "isy" ]; then
    runtime_dir=android
fi

if [ "is$CONFIG_RUNTIME_RDK" = "isy" ]; then
    runtime_dir=rdk-lib32
fi

if [ "is$CONFIG_RUNTIME_OE" = "isy" ]; then
    runtime_dir=poky-lib32
fi

if [ "is$CONFIG_RUNTIME_OE64" = "isy" ]; then
    runtime_dir=poky
fi

if [ "is$CONFIG_RUNTIME_LINUX_BASELINE_BUILDROOT" = "isy" ]; then
    runtime_dir=linux-baseline
    runtime_para=ramdisc
fi

if [ "is$CONFIG_RUNTIME_LINUX_BASELINE_BUILDROOT64" = "isy" ]; then
    runtime_dir=linux-rootfs64
    runtime_para=ramdisc
fi

if [ "is$CONFIG_RUNTIME_UBUNTU" = "isy" ]; then
    runtime_dir=ubuntu20
    runtime_para=ramdisc
    if [ -L ${mod_dir}/${runtime_dir}/data/rootfs ];then
        rm -fr ${mod_dir}/${runtime_dir}/data/rootfs
    fi
    pushd ${mod_dir}/${runtime_dir}/data/
    if [ "is$CONFIG_UBUNTU_GUI" = "isy" ]; then
        ln -sf  ${mod_dir}/${runtime_dir}/data/rootfs_gui  rootfs
    else
        ln -sf  ${mod_dir}/${runtime_dir}/data/rootfs_nongui  rootfs
    fi
    popd
fi

runtime_sysroot=build_sysroot

source build/module/toolchain/${CONFIG_TOOLCHAIN_APPLICATION}.rc
source ${mod_dir}/${runtime_dir}/build.rc
source ${mod_dir}/${runtime_dir}/pack.rc
