#!/bin/bash

[ "$FLASH_TYPE" = "" -o "$CUST_SELFKEY_IMAGE" = "" ] && {
	source ../../../Customization_Data/config
	if [ -z $CPUPLL ];then
		CPUPLL=1000
	fi
	if [ -z $MEMPLL ];then
		MEMPLL=1600
	fi
	if [ -z $SYSPLL ];then
		SYSPLL=800
	fi
	export PLATFORM
	export CPUPLL
	export PV_COMP
	export NFCECCCLK
	export LINUX_ON_CPU1
	export CPU_TYPE
	export CPU_PLL
	export MEMPLL
	export SYSPLL
	export SYSTEM_CONFIGURATION_PATH
	export PLATFORM_INFO
	export BUILD_GOOGLETV
	export BOOT_TYPE
	export RANDOMIZER
	export SM_IMAGE_ACTIVE
	export SM_IMAGE_PATH
	export SM_IMAGE_FORMAT
	export SM_ROM_ENABLE
	export CPU0_START_ADDR
	export CONFIG_GPT
	export CONFIG_EMMC_WRITE_PROTECT
	export CONFIG_ENCRYPTED_RAMDISK
	export BOOTIMG_ENCRYPT_DISABLE
	export DISABLE_CRASH_COUNTER
	export BERLIN_CHIP
	top_data_dir=../../../Customization_Data/
	export top_data_dir
	export BOOTLOADER_DDR_START
	export BOOTLOADER_DDR_END
	export BOOTLOADER_DDR_BUF
	export CONFIG_DTB
	export CONFIG_NOR_BOOT
	export CONFIG_SIGNTYPE
	export CONFIG_BOOTFLOW
	export CONFIG_APB_TIMER_DISABLE
	export CPU_ARCH
	export CONFIG_EMMC_V5
	export CONFIG_FPGA
	export CONFIG_SSTZ
	export CONFIG_APB_UART_DISABLE
	export CONFIG_ANDROID_BOOTFLOW
	export CONFIG_DDR4
	export CONFIG_FAST_BOOT
	export CHIP_REV
	export CONFIG_MISC
}
echo ------------------------------
echo "   platform == $PLATFORM"
echo ------------------------------
OUT_DIR=out/$PLATFORM
WORKING_DIR=`pwd`

if [ [$CONFIG_NOR_BOOT = "y"] ]; then
	BOOT_DEV=nor
elif [ $BOOT_TYPE = "NAND_BOOT" ]; then
	BOOT_DEV=nand
else
	BOOT_DEV=emmc
fi

rm -rf $WORKING_DIR/include/auto
mkdir $WORKING_DIR/include/auto

# copy the linux parameters (bootargs)
if [ ! -f ../../.temp/linux_param.h ]; then
	echo run \"make linux_param\" under MV88DE3100_Tools/ first
	exit 1
fi
cp -f ../../.temp/linux_param.h ./include/auto

#if [ "$CONFIG_DTB" = "y" ]; then
# Copy dtb.dtb to local
#cp -f $WORKING_DIR/../../../Customization_Data/CPU0_Image/dtb.dtb $WORKING_DIR/include/auto/dtb.dtb
#fi

# check platform info
#if [ \( ! -f $top_data_dir/$PLATFORM_INFO \) -a \( ! -L $top_data_dir/$PLATFORM_INFO \) ]; then
#	echo "Error: Pin setting file $top_data_dir/$PLATFORM_INFO does not exist."
#	exit 1
#fi
#cp -f $top_data_dir/$PLATFORM_INFO include/auto/pin_settings.h ||exit 1
#if test ! -d include/auto/Platform_Info ; then
#    ln -s $top_data_dir/$SYSTEM_CONFIGURATION_PATH/Platform_Info include/auto/Platform_Info || exit 1
#fi


if [ "$SM_IMAGE_ACTIVE" = "1" ]; then
# build and copy SM
echo -------------------------------
echo "   begin to build SM"
echo -------------------------------
if [ "is$SM_ROM_ENABLE" = "is" ]; then
	SM_PATH=$WORKING_DIR/../system_manager
else
	SM_PATH=$WORKING_DIR/../sm_rom
fi

cd $SM_PATH
make clean
./build_all.sh $PLATFORM || exit 1

#use part of itcm as rom
if [ "is$SM_ROM_ENABLE" = "is1" ]; then
	SM_SIZE=26624
	SM_ROM_SIZE=32768
fi

#has sm rom code on chip
if [ "is$SM_ROM_ENABLE" = "is0" ]; then
	SM_SIZE=26624
fi

#no rom code
if [ "is$SM_ROM_ENABLE" = "is" ]; then
	SM_SIZE=65536
fi

if [ `stat -c %s $SM_PATH/out/$PLATFORM/bin/sm.bin` -ge $SM_SIZE ]; then
	echo "sm.bin size `stat -c %s $SM_PATH/out/$PLATFORM/bin/sm.bin` is larger than $SM_SIZE." > /dev/stderr;
	exit 1;
else
	echo "sm.bin size is `stat -c %s $SM_PATH/out/$PLATFORM/bin/sm.bin`"
fi

if [ "is$SM_ROM_ENABLE" = "is1" ]; then
	if [ `stat -c %s $SM_PATH/out/$PLATFORM/bin/sm.rom.bin` -ge $SM_ROM_SIZE ]; then
		echo "sm.rom.bin size `stat -c %s $SM_PATH/out/$PLATFORM/bin/sm.rom.bin` is larger than $SM_ROM_SIZE." > /dev/stderr;
		exit 1;
	else
		echo "sm.rom.bin size is `stat -c %s $SM_PATH/out/$PLATFORM/bin/sm.rom.bin`"
	fi
fi

cp -f $SM_PATH/out/$PLATFORM/bin/sm.bin $WORKING_DIR/sm.bin || exit 1
if [ "is$SM_ROM_ENABLE" = "is1" ]; then
	cp -f $SM_PATH/out/$PLATFORM/bin/sm.rom.bin $WORKING_DIR/sm.rom.bin || exit 1
fi

echo -------------------------------
echo "   end of building SM"
echo -------------------------------

else

echo ----------------------------------------------------------------------------------------------
echo " FPGA Hacking:  Using existed SM Image, Rather than compiling  "
echo ----------------------------------------------------------------------------------------------

fi

cd $WORKING_DIR

echo -------------------------------
echo "   begin to build bootloader"
echo -------------------------------

mkdir out
mkdir $OUT_DIR

make clean &&\
make configure CPU_TYPE=$CPU_TYPE CPUPLL=$CPUPLL BOX_PRODUCT_RELEASE=$BOX_PRODUCT_RELEASE BOOT_TYPE=$BOOT_TYPE RANDOMIZER=$RANDOMIZER &&\
make || exit 1

echo -------------------------------
echo "   end of building bootloader"
echo -------------------------------
