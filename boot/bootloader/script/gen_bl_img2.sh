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
	export JTAG_ENABLE
	export RANDOMIZER
	export SM_IMAGE_ACTIVE
	export SM_IMAGE_PATH
	export SM_IMAGE_FORMAT
	export CPU0_START_ADDR
	export CONFIG_GPT
	export CONFIG_EMMC_WRITE_PROTECT
	export CONFIG_ENCRYPT_RAMDISK
	export DISABLE_CRASH_COUNTER
	export CONFIG_NAND_READ_RETRY
	export BERLIN_CHIP
	top_data_dir=../../../Customization_Data/
	export top_data_dir
	export CONFIG_TRUSTZONE
	export CONFIG_TZ_IMAGE_PATH
	export BOOTLOADER_DDR_START
	export BOOTLOADER_DDR_END
	export BOOTLOADER_DDR_BUF
	export CONFIG_DTB
	export CONFIG_NOR_BOOT
	export CONFIG_SIGNTYPE
	export CONFIG_ANTI_ROLLBACK
	export ANTIROLLBACK_COMVER
	export ANTIROLLBACK_CUSVER
	export CONFIG_BCM
	export CONFIG_POST_BOOTLOADER
	export CONFIG_APB_TIMER_DISABLE
	export CPU_ARCH
	export CONFIG_EMMC_V5
	export CONFIG_FPGA
	export CONFIG_SSTZ
	export LINUXSDK_BOOTLOADER
	export MIDVER
	export CONFIG_ANDROID_BOOTFLOW
	export CONFIG_DDR4
	export BCM_ROM_KEY_DISABLE
	export CONFIG_FAST_BOOT
	export CHIP_REV
	export CONFIG_MISC
}
pagesize=`echo $FLASH_TYPE | cut -d _ -f1`
page_per_block=`echo $FLASH_TYPE | cut -d _ -f3`
block_size=`expr $pagesize \* $page_per_block`
block_size_kB=`expr $block_size / 1024`
echo "pagesize=$pagesize , page_per_block=$page_per_block"
echo "block_size_kB=$block_size_kB"

if [ -z $SDK_CROSS_COMPILE ]; then
CROSS_COMPILE=arm-marvell-eabi-
SDK_CROSS_COMPILE=arm-marvell-eabi-
fi
READELF=${SDK_CROSS_COMPILE}readelf
OBJCOPY=${SDK_CROSS_COMPILE}objcopy

OUT_DIR=out/$PLATFORM
rm -rf $OUT_DIR/*

./script/build.sh || exit 1

echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
echo below steps should be moved to SDK
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
if [ "$CONFIG_NOR_BOOT" = "y" ]; then
	BOOT_DEV=nor
elif [ "$BOOT_TYPE" = "NAND_BOOT" ]; then
	BOOT_DEV=nand
else
	BOOT_DEV=emmc
fi

WORKING_DIR=`pwd`
ADDHEADER_TOOL="../../Scripts/addheader.sh"

CHIP_NAME=`echo $BERLIN_CHIP | tr '[A-Z]' '[a-z]'`
BCM_DIR=../../SecureEngine
ENC_TOOL=$BCM_DIR/encimg
if [ "is$MIDVER" = "is" ]; then
	MIDVER=0x0
fi
MID=$MIDVER
OEM_ROOT_RSA_SIGNK7=$BCM_DIR/BCM/$CHIP_NAME/mid$MID/codetype_05-06-07/OEM_ROOT_RSA_SIGNK7.store

echo ----------------------------------------------
echo     begin generate bootloader_image_list.img
echo ----------------------------------------------

# generate the final image for bootloader
$OBJCOPY -O binary $OUT_DIR/bootloader.elf $OUT_DIR/bootloader.img
# get the exe address
TEMP_FILE=$OUT_DIR/elf_dump
$READELF -S $OUT_DIR/bootloader.elf > $TEMP_FILE
EXE_ADDR=0x`grep '\.text' $TEMP_FILE | sed 's/\[[ ]*[(0-9)]*\]//' | awk {'print $3'}`
rm -f $TEMP_FILE

BL_IMAGE_CMD_PARAM=" -i BTLR -d $OUT_DIR/bootloader.img -a $EXE_ADDR"

if [ "$CONFIG_FAST_BOOT" = "y" ]; then
cp ../../../Customization_Data/Fastboot_Image/u-boot $OUT_DIR/fastboot
cp ../../../Customization_Data/Fastboot_Image/u-boot.bin $OUT_DIR/fastboot.bin

# get the exe address
TEMP_FILE=$OUT_DIR/elf_dump
$READELF -S $OUT_DIR/fastboot > $TEMP_FILE
EXE_ADDR=0x`grep '\.text' $TEMP_FILE | sed 's/\[[ ]*[(0-9)]*\]//' | awk {'print $3'}`
rm -f $TEMP_FILE

BL_IMAGE_CMD_PARAM="$BL_IMAGE_CMD_PARAM -i FSBT -d $OUT_DIR/fastboot.bin -a $EXE_ADDR"
fi


rm -rf $OUT_DIR/bootloader_image_list.img $OUT_DIR/bootloader_image_list_en.img

echo "bl_image_cmd_param = $BL_IMAGE_CMD_PARAM"
$temp_dir/genimg -n bootloader -V 0 -v 1 $BL_IMAGE_CMD_PARAM -o $OUT_DIR/bootloader_image_list.img
if [ "$?" != "0" ];then
	echo "$0,generate bootloader header error"
	exit 1
fi

# if any other bootloader related image, append it here

# encrypt the bootloader image list including header
if [ "$CONFIG_BOOTFLOW" = "CLEAR" ]; then
	cp $OUT_DIR/bootloader_image_list.img $OUT_DIR/bootloader_image_list_en.img
else
	$ENC_TOOL -c $CHIP_NAME -m $MID -t BOOTLOADER -i $OUT_DIR/bootloader_image_list.img -o $OUT_DIR/bootloader_image_list_en.img
	#extra header to store the bootloader size
	mv $OUT_DIR/bootloader_image_list_en.img $OUT_DIR/bootloader_image_list_en.img.temp
	$BCM_DIR/BCM/$CHIP_NAME/gen_enc_img_info.sh $OUT_DIR/bootloader_image_list_en.img.temp $OUT_DIR/bootloader_image_list_en.img
	rm $OUT_DIR/bootloader_image_list_en.img.temp
fi

echo ----------------------------------------------
echo     end generate bootloader_image_list.img
echo ----------------------------------------------

# bootloader is always in user area from now on
cp $OUT_DIR/bootloader_image_list_en.img $OUT_DIR/post_bootloader.img


if [ $BOOT_TYPE = "NAND_BOOT" ]; then
		SIZE=`stat -c %s $OUT_DIR/post_bootloader.img` && \
		dd if=/dev/urandom of=$OUT_DIR/post_bootloader.img bs=1 seek=$SIZE count=`expr $block_size - $SIZE` conv=notrunc || exit 1
fi

echo --------------------
echo     begin end
echo --------------------
