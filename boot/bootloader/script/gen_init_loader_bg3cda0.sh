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

KEY_INJECTION_DIR=../../IMAGE/Key_Injection
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
if [ "is$CONFIG_SIGNTYPE" = "is" ]; then
        CONFIG_SIGNTYPE=RSA1024_SHA1
fi

WORKING_DIR=`pwd`
ADDHEADER_TOOL="../../Scripts/addheader.sh"
LOCAL_CUST_SELFKEY_PATH=$WORKING_DIR/../../../Customization_Data/$CUST_SELFKEY_IMAGE
SYSTEM_INIT_FILE_LOCAL=$OUT_DIR/SI_part1.bin
SI_MINILOADER_IMAGE_LOCAL=$OUT_DIR/miniloader_en.bin

SYS_INIT_DIR=../sys_init/out
SYS_INIT_BIN_FILE=sys_init_${BOOT_DEV}_secure_CPU"$CPUPLL"_MEM"$MEMPLL"_SYS"$SYSPLL"_$PLATFORM.bin
if [ $CPUPLL -ge 800 ]; then
	SYS_INIT_BIN_FILE=sys_init_${BOOT_DEV}_secure_CPU800_MEM"$MEMPLL"_SYS"$SYSPLL"_$PLATFORM.bin
fi

[ $# = 1 ] && VT_PATH=$1 || VT_PATH=../../.temp/version_table

if [ "$PRODUCTION_CHIP" = "0" ]; then
	echo "Generating bootloader for engineer chip"
	if [ "is$PLATFORM" != "is" -a -d $SYS_INIT_DIR/$PLATFORM ]; then
		SYS_INIT_BIN_DIR=$SYS_INIT_DIR/$PLATFORM/${BOOT_DEV}_secure_romkey
	else
		SYS_INIT_BIN_DIR=$SYS_INIT_DIR/default/${BOOT_DEV}_secure_romkey
	fi
elif [ "$PRODUCTION_CHIP" = "1" ]; then
	echo "Generating bootloader for production chip"
	if [ "is$PLATFORM" != "is" -a -d $SYS_INIT_DIR/$PLATFORM ]; then
		SYS_INIT_BIN_DIR=$SYS_INIT_DIR/$PLATFORM/${BOOT_DEV}_secure_distkey
	else
		SYS_INIT_BIN_DIR=$SYS_INIT_DIR/default/${BOOT_DEV}_secure_distkey
	fi
else
	echo "Generating bootloader for OTP provision"
	if [ "is$PLATFORM" != "is" -a -d $SYS_INIT_DIR/$PLATFORM ]; then
		SYS_INIT_BIN_DIR=$SYS_INIT_DIR/$PLATFORM/${BOOT_DEV}_otp_provision
	else
		SYS_INIT_BIN_DIR=$SYS_INIT_DIR/default/${BOOT_DEV}_otp_provision
	fi
fi

if [ -f $SYS_INIT_BIN_DIR/$SYS_INIT_BIN_FILE ]; then
echo "sys_init binary file: $SYS_INIT_BIN_DIR/$SYS_INIT_BIN_FILE"
else
SYS_INIT_BIN_FILE=sys_init_${BOOT_DEV}_secure_CPU"$CPUPLL"_MEM"$MEMPLL"_SYS"$SYSPLL"_$PLATFORM.bin
echo "sys_init binary file: $SYS_INIT_BIN_DIR/$SYS_INIT_BIN_FILE"
fi
cp $SYS_INIT_BIN_DIR/$SYS_INIT_BIN_FILE $SYSTEM_INIT_FILE_LOCAL && cp $SYS_INIT_BIN_DIR/miniloader_en.bin $OUT_DIR/ || exit 1

if [ ! -f $VT_PATH ];then
	echo "please generate $VT_PATH first."
	exit 1
fi

cp -vf $SYSTEM_INIT_FILE_LOCAL $OUT_DIR/bcm_sysinit_tsm.bin &&\

echo --------------------
echo     begin padding
echo --------------------
# current layout of bootloader.subimg
# _____________________________
# |       |  OEM_ROOT_RSA(4k, p) + MRVL_ROOT_RSA (4k) (not for bg3cd)
# |page   |  BCM_CUSTK0(1k) + layout(512B, bg3cd only) + BCM_EXT_RSA0(1k, 3cd)(4k, 4ct) + header(1k:2k) + EROM
# |aligned|  APU_CUSTK0(1k) + APU_EXT_RSA0(1k:4k) + header(1k:2k) + sys_init
# |       |  page aligned (only for bg3cd?)
# |       |  BCM_CUSTK1(1k) + BCM_EXT_RSA1(1k, 4k) + header(1k:2k) + TSMK
# |       |  page aligned
# |_______|_______________________
# | XXk   |  APU_CUSTK0(1k) + APU_EXT_RSA0(1k:4k) + header(1k:2k) + miniloader(encrypted)
# |       |  page aligned
# |_______|_______________________
# |       |
# | 12k   |  OEM_CUSTK1 (1k) + OEM_EXT_RSA0 (1k:4k) + boot layout info (2k + 2k) + page aligned
# |_______|_______________________
# |       |
# |  4k   |  partition table infor (2k + 2k) + page aligned
# |_______|_______________________
# |          | OEM Key store header (2K)
# |          | OEM Key store (2K, clear)
# | 6k       | User Key store header (2K)           (android only)
# |          | User Key store(2K, clear)
# |__________|_______________________
# |       stuffing
# |_____________________________

CHIP_NAME=`echo $BERLIN_CHIP | tr '[A-Z]' '[a-z]'`
BCM_DIR=../../SecureEngine
ENC_TOOL=$BCM_DIR/encimg
if [ "is$MIDVER" = "is" ]; then
	MIDVER=0x0
fi
MID=$MIDVER
OEM_ROOT_RSA_SIGNK7=$BCM_DIR/BCM/$CHIP_NAME/mid$MID/codetype_05-06-07/OEM_ROOT_RSA_SIGNK7.store

# encrypted partition info table
dd if=$VT_PATH of=$OUT_DIR/version conv=notrunc
if [ "$CONFIG_BOOTFLOW" = "CLEAR" ]; then
	cp $OUT_DIR/version $OUT_DIR/version_en
else
	$ENC_TOOL -c $CHIP_NAME -m $MID -t PT -i $OUT_DIR/version -o $OUT_DIR/version_en
fi

#append to 8k
./script/align.sh $OUT_DIR/version_en 8192
#page aligned
./script/align.sh $OUT_DIR/version_en $pagesize

if [ "$CONFIG_SSTZ" = "y" ]; then
cp ../../../Customization_Data/TZ_Image/$CHIP_NAME/sstz $OUT_DIR/sstz
if [ "$CONFIG_BOOTFLOW" = "CLEAR" ]; then
	cp $OUT_DIR/sstz $OUT_DIR/sstz_en
else
	$ENC_TOOL -c $CHIP_NAME -m $MID -t TZK -i $OUT_DIR/sstz -o $OUT_DIR/sstz_en
fi
fi

if [ "$BCM_ROM_KEY_DISABLE" = "1" ]; then
	if [ -f $OEM_ROOT_RSA_SIGNK7 ]; then
		signk7_len=$(stat -c %s "$OEM_ROOT_RSA_SIGNK7")
		dd if=$OUT_DIR/bcm_sysinit_tsm.bin of=$OUT_DIR/bcm_sysinit_tsm.tail bs=1 skip=$signk7_len
		rm -rf $OUT_DIR/bcm_sysinit_tsm.bin
		cat $OEM_ROOT_RSA_SIGNK7 $OUT_DIR/bcm_sysinit_tsm.tail > $OUT_DIR/bcm_sysinit_tsm.bin
		rm -rf $OUT_DIR/bcm_sysinit_tsm.tail
	else
		echo "can't find $OEM_ROOT_RSA_SINGK7"
		exit 1
	fi
fi
BSP_IMAGE_LIST=($OUT_DIR/bcm_sysinit_tsm.bin $OUT_DIR/miniloader_en.bin $OUT_DIR/image_layout_en.img $OUT_DIR/image_list.img)

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

echo ----------------------------------------------
echo     begin generate final image_list.img
echo ----------------------------------------------

# encrypted partition info
IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i PTIN -d $OUT_DIR/version_en -t attr0 2"

#if [ "$LINUXSDK_BOOTLOADER" != "y" ]; then
# android key (reserve here)
#IMAGE_CMD_PARAM=""
#fi

if [ "$CONFIG_POST_BOOTLOADER" = "y" ]; then
# bootloader is always in user area from now on
cp $OUT_DIR/bootloader_image_list_en.img $OUT_DIR/post_bootloader.img
else
IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i BTLR -d $OUT_DIR/bootloader_image_list_en.img -t attr0 2"
fi

if [ "$CONFIG_SSTZ" = "y" ]; then
IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i SSTZ -d $OUT_DIR/sstz_en -t attr0 2 -a 0"
fi

if [ "$CONFIG_ANDROID_BOOTFLOW" = "y" ]; then
cp $OUT_DIR/../../../../SecureEngine/BCM/bg4ct/oem_verity_key $OUT_DIR/oemk
cp $OUT_DIR/../../../../SecureEngine/BCM/bg4ct/usr_verity_key $OUT_DIR/usrk

if [ "$CONFIG_BOOTFLOW" = "CLEAR" ]; then
cp $OUT_DIR/oemk $OUT_DIR/oemk_en
cp $OUT_DIR/usrk $OUT_DIR/usrk_en
else
./$ENC_TOOL -c $CHIP_NAME -m $MID -t ANDROID_KEY -i $OUT_DIR/oemk -o $OUT_DIR/oemk_en
./$ENC_TOOL -c $CHIP_NAME -m $MID -t ANDROID_KEY -i $OUT_DIR/usrk -o $OUT_DIR/usrk_en
fi

./script/align.sh $OUT_DIR/oemk_en 12288
./script/align.sh $OUT_DIR/usrk_en 12288

IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i OEMK -d $OUT_DIR/oemk_en -t attr0 2"
IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i USRK -d $OUT_DIR/usrk_en -t attr0 2"
fi

echo "image_cmd_param = $IMAGE_CMD_PARAM"
#layout info reserve 12k
$temp_dir/genimg -n imglayout -V 0 -v 1 -A $pagesize $IMAGE_CMD_PARAM -o $OUT_DIR/image_list.img -S $OUT_DIR/image_layout.img 12288
if [ "$?" != "0" ];then
        echo "$0,generate bootloader header error"
        exit 1
fi

#FIXME: ?
# copy the image header image_header.img to key_injection folder
#cp $OUT_DIR/image_header.img $KEY_INJECTION_DIR/

# encrypt the image header
if [ "$CONFIG_BOOTFLOW" = "CLEAR" ]; then
	cp  $OUT_DIR/image_layout.img $OUT_DIR/image_layout_en.img
else
	if [ "$BCM_ROM_KEY_DISABLE" = "1" ]; then
		$ENC_TOOL -c $CHIP_NAME -m $MID -t BLI_OTA -i $OUT_DIR/image_layout.img -o $OUT_DIR/image_layout_en.img
	else
		$ENC_TOOL -c $CHIP_NAME -m $MID -t BLI -i $OUT_DIR/image_layout.img -o $OUT_DIR/image_layout_en.img
	fi
fi

#12k
./script/align.sh $OUT_DIR/image_layout_en.img 12288
#page size aligned
./script/align.sh $OUT_DIR/image_layout_en.img $pagesize

echo ----------------------------------------------
echo     end generate final image_list.img
echo ----------------------------------------------

echo "BSP_IMAGE_LIST = ${BSP_IMAGE_LIST[@]}";
cat ${BSP_IMAGE_LIST[@]} > $OUT_DIR/init_loader.img || exit 1

if [ $BOOT_TYPE = "EMMC_BOOT" ]; then
	INIT_LOADER_FINAL_SIZE=`stat -c %s $OUT_DIR/init_loader.img`
	echo "init_loader size = $INIT_LOADER_FINAL_SIZE"
	if [ $INIT_LOADER_FINAL_SIZE -ge 524288 ]; then
		echo "Error: bootloader partition image size is greater than 512K."
		exit 1;
	fi
	dd if=/dev/zero of=$OUT_DIR/init_loader.img bs=1 count=1 seek=524287 conv=notrunc || exit 1
else
	INIT_LOADER_FINAL_SIZE=`stat -c %s $OUT_DIR/init_loader.img`
	if [ $INIT_LOADER_FINAL_SIZE -ge $block_size ]; then
		echo "Error: bootloader partition image size is greater than block size."
		exit 1;
	fi
	SIZE=`stat -c %s $OUT_DIR/init_loader.img` && \
	dd if=/dev/urandom of=$OUT_DIR/init_loader.img bs=1 seek=$SIZE count=`expr $block_size - $SIZE` conv=notrunc || exit 1

	if [ "$CONFIG_POST_BOOTLOADER" = "y" ]; then
		SIZE=`stat -c %s $OUT_DIR/post_bootloader.img` && \
		dd if=/dev/urandom of=$OUT_DIR/post_bootloader.img bs=1 seek=$SIZE count=`expr $block_size - $SIZE` conv=notrunc || exit 1
	fi
fi

#FIXME: ?
# copy the final image to key_injection folder
cp $OUT_DIR/version $KEY_INJECTION_DIR/
cp $OUT_DIR/init_loader.img $KEY_INJECTION_DIR/bootloader.subimg

echo --------------------
echo     begin end
echo --------------------
