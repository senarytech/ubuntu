#!/bin/bash

set -e

cwd=$(cd $(dirname $0) && pwd)

#FIXME: export linario gcc v5.3 here for bootloader
PREBUILTS_PATH=${cwd}/../../prebuilts
PREBUILTS_PATH2=$PREBUILTS_PATH/gcc/linux-x86/aarch64/gcc-linaro-aarch64-elf-5.3/bin
export PATH=$PREBUILTS_PATH2:$PATH

if [ $# = "1" ];then
	PREBUILT_PATH=$1
else
	PREBUILT_PATH=${cwd}/out/$PLATFORM
fi

CHIP_NAME=`echo $BERLIN_CHIP | tr '[A-Z]' '[a-z]'`

#if there is different config of the project, use the specified one
if [ -e ${cwd}/configs/$CHIP_NAME/$CONFIG_BOOTFLOW/config ]; then
	CONFIG_PATH=${cwd}/configs/$CHIP_NAME/$CONFIG_BOOTFLOW/config
else
#otherwise, use the common one
	CONFIG_PATH=${cwd}/configs/$CHIP_NAME/config
fi

#if there is different config of the platform, use the specified one
if [ -e ${cwd}/configs/$CHIP_NAME/$CONFIG_BOOTFLOW/$PLATFORM/config ]; then
	CONFIG_PATH=${cwd}/configs/$CHIP_NAME/$CONFIG_BOOTFLOW/$PLATFORM/config
fi

echo -------------------------------
echo CONFIG_PATH=$CONFIG_PATH
echo -------------------------------

export CONFIG_PATH
. $CONFIG_PATH

function build_sm() {
echo -------------------------------
echo "   begin to build system manager"
echo -------------------------------

cd ${cwd}/sm_rom

./build.sh n

cd -

echo -------------------------------
echo "   end of building system manager"
echo -------------------------------
}

function build_bootloader() {
echo -------------------------------
echo "   begin to build bootloader"
echo -------------------------------

cd ${cwd}

make clean &&\
make configure CPUPLL=$CPUPLL BOOT_TYPE=$BOOT_TYPE RANDOMIZER=$RANDOMIZER &&\
make || exit 1

cd -

echo -------------------------------
echo "   end of building bootloader"
echo -------------------------------
}

echo -------------------------------
echo "   platform = $PLATFORM "
echo -------------------------------

if [ $CONFIG_SM = "y" ];then
# if sm is existed, build sm
build_sm

#sm ram size is 26K
SM_SIZE=26624

if [ `stat -c %s ${OUTPUT_DIR}/output_sm/bin/sm.bin` -ge $SM_SIZE ]; then
	echo "sm.bin size `stat -c %s ${OUTPUT_DIR}/output_sm/bin/sm.bin` is larger than $SM_SIZE." > /dev/stderr;
	exit 1;
else
	echo "sm.bin size is `stat -c %s ${OUTPUT_DIR}/output_sm/bin/sm.bin`"
fi

#cp sm.bin
cp ${OUTPUT_DIR}/output_sm/bin/sm.bin ${cwd}/include
fi

# build bootloader
build_bootloader
# copy bootloader to target directory
#cp ${cwd}/out/$PLATFORM/bootloader.bin $PREBUILT_PATH
