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

cp -r ${CONFIG_SYNA_SDK_PATH}/senary/common-sysroot/* ${rootfs};
cp -r ${CONFIG_SYNA_SDK_PATH}/senary/ampsdk-sysroot/* ${rootfs};

if [ "is${CONFIG_CUSTOM_PROFILE}" = "isc01" ]; then
	echo "CONFIG_CUSTOM_PROFILE=${CONFIG_CUSTOM_PROFILE}.............................................."
    cp -r ${CONFIG_SYNA_SDK_PATH}/senary/001-sysroot/* ${rootfs}

elif [ "is${CONFIG_CUSTOM_PROFILE}" = "isc02" ]; then
	echo "CONFIG_CUSTOM_PROFILE=${CONFIG_CUSTOM_PROFILE}.............................................."
    cp -r ${CONFIG_SYNA_SDK_PATH}/senary/002-sysroot/* ${rootfs}

else
    echo "Not find CONFIG_CUSTOM_PROFILE=${CONFIG_CUSTOM_PROFILE}."
fi

sdk_sysroot=${CONFIG_SYNA_SDK_PATH}/senary/sdk-sysroot
rm -rf ${sdk_sysroot}
mv ${rootfs} ${sdk_sysroot}
mkdir -p ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system
base_ubuntu=${CONFIG_SYNA_SDK_PATH}/sysroot/ubuntu20

cp -ar ${base_ubuntu}/data/rootfs/* ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system

cp -ar ${sdk_sysroot}/etc/*.xml ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system/etc/
cp -ar ${sdk_sysroot}/lib/* ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system/lib
cp -ar ${sdk_sysroot}/opt ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system/
cp -ar ${sdk_sysroot}/usr/* ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system/opt/syna/

basedir_script_subimg=${moduledir}/lib/subimage

source ${script_dir}/packaging.bashrc
