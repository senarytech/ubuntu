#!/bin/bash

source build/header.rc
source build/chip.rc
source build/security.rc

##rootfs  files will copy to sdk lib and finally in image
rootfs=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_ROOTFS}

moduledir=${topdir}/build/module/image
[ $clean -eq 1 ] && exit 0
if [ "is${CONFIG_IMAGE_EMMC}" = "isy" ]; then
    script_dir=${moduledir}/lib/boot_type/emmc
fi
if [ "is${CONFIG_IMAGE_NAND}" = "isy" ]; then
    script_dir=${moduledir}/lib/boot_type/nand
fi

basedir_script_subimg=${moduledir}/lib/subimage

sdk_sysroot=${CONFIG_SYNA_SDK_PATH}/senary/sdk-sysroot
if [ "is${CONFIG_MINIBUILD}" == "isy" ]; then
rm -rf ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system
else
rm -rf ${sdk_sysroot}
mv ${rootfs} ${sdk_sysroot}
cp -ar ${sdk_sysroot}/usr/* ${sdk_sysroot}/opt/syna/
rm -rf ${sdk_sysroot}/usr
fi

mkdir -p ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system
base_ubuntu=${CONFIG_SYNA_SDK_PATH}/sysroot/ubuntu20
cp -ar ${base_ubuntu}/data/rootfs/* ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system

cp -r ${sdk_sysroot}/etc/* ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system/etc/
cp -r ${sdk_sysroot}/lib/modules ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system/lib/
cp -r ${sdk_sysroot}/lib/firmware ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system/lib/
cp -r ${sdk_sysroot}/opt/* ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system/opt/
#cp -r ${sdk_sysroot}/home/* ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system/home/

cp -r ${CONFIG_SYNA_SDK_PATH}/senary/common-sysroot/* ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system/

if [ "is${CONFIG_CUSTOM_PROFILE}" = "isc01" ]; then
	echo "CONFIG_CUSTOM_PROFILE=${CONFIG_CUSTOM_PROFILE}.............................................."
    cp -r ${CONFIG_SYNA_SDK_PATH}/senary/001-sysroot/* ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system/

elif [ "is${CONFIG_CUSTOM_PROFILE}" = "isc02" ]; then
	echo "CONFIG_CUSTOM_PROFILE=${CONFIG_CUSTOM_PROFILE}.............................................."
    cp -r ${CONFIG_SYNA_SDK_PATH}/senary/002-sysroot/* ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system/

elif [ "is${CONFIG_CUSTOM_PROFILE}" = "isc05" ]; then
    echo "CONFIG_CUSTOM_PROFILE=${CONFIG_CUSTOM_PROFILE}.............................................."
    cp -r ${CONFIG_SYNA_SDK_PATH}/senary/005-sysroot/* ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system/ || { echo "WARNING: failed on copy sysroot"; }

elif [ "is${CONFIG_CUSTOM_PROFILE}" = "isc06" ]; then
    echo "CONFIG_CUSTOM_PROFILE=${CONFIG_CUSTOM_PROFILE}.............................................."
    cp -r ${CONFIG_SYNA_SDK_PATH}/senary/006-sysroot/* ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system/ || { echo "WARNING: failed on copy sysroot"; }

else
    echo "Not find CONFIG_CUSTOM_PROFILE=${CONFIG_CUSTOM_PROFILE}."
fi

source ${script_dir}/packaging.bashrc
