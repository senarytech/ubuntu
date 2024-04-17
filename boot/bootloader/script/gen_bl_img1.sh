#!/bin/bash

#1. pack bcm_erom.bin sys_init.bin and drm_rom.bin(tsm) together, append to fixed size
#2. generate image layout info
#3. encrypt/sign pt info and android key
#4. pack all images to bootloader.subimg

#setup env

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
export MEMPLL
export SYSPLL
export BOOT_TYPE
export FLASH_TYPE
export BERLIN_CHIP
export CONFIG_SSTZ
export CONFIG_TZ_IMAGE_PATH
export CONFIG_BCM
export CONFIG_POST_BOOTLOADER
export CPU_ARCH
export CONFIG_GPT
export CONFIG_FPGA
export MIDVER
export BCM_ROM_KEY_DISABLE
export CONFIG_BOOTFLOW
export CONFIG_ANDROID_BOOTFLOW
export CONFIG_FAST_BOOT
export CHIP_REV
export CONFIG_FIXED_SIZE
export CONFIG_SYSINIT_SIZE
export CONFIG_MINILOADER_SIZE
export PRODUCTION_CHIP

WORKING_DIR=`pwd`
SDK_PATH=${WORKING_DIR%%MV88DE3100_SDK*}
SDK_PATH="$SDK_PATH"MV88DE3100_SDK-shell

pagesize=`echo $FLASH_TYPE | cut -d _ -f1`
page_per_block=`echo $FLASH_TYPE | cut -d _ -f3`
block_size=`expr $pagesize \* $page_per_block`
block_size_kB=`expr $block_size / 1024`
echo "pagesize=$pagesize , page_per_block=$page_per_block"
echo "block_size_kB=$block_size_kB"

KEY_INJECTION_DIR=../../IMAGE/Key_Injection
OUT_DIR=$SDK_PATH/MV88DE3100_Tools/bsp/bootloader/out/$PLATFORM


if [ ! -d "$OUT_DIR" ]; then
	mkdir -p $OUT_DIR
fi
rm -rf $OUT_DIR/*

echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
echo below steps should be moved to SDK
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"

CHIP_NAME=`echo $BERLIN_CHIP | tr '[A-Z]' '[a-z]'`
BCM_DIR=$SDK_PATH/MV88DE3100_Tools/SecureEngine
ENC_TOOL=$BCM_DIR/encimg

if [ "is$MIDVER" = "is" ]; then
	MIDVER=0x0
fi
MID=$MIDVER


if [ "$BOOT_TYPE" = "NAND_BOOT" ]; then
	BOOT_DEV=nand
fi
if [ "$BOOT_TYPE" = "EMMC_BOOT" ]; then
	BOOT_DEV=emmc
fi

SYS_INIT_DIR=$SDK_PATH/MV88DE3100_Tools/bsp/sys_init/out
echo ${SYS_INIT_DIR}

SYS_INIT_BIN_FILE=sys_init_${BOOT_DEV}_secure_CPU"$CPUPLL"_MEM"$MEMPLL"_SYS"$SYSPLL"_$PLATFORM.bin
if [ $CPUPLL -ge 800 ]; then
	SYS_INIT_BIN_FILE=sys_init_${BOOT_DEV}_secure_CPU800_MEM"$MEMPLL"_SYS"$SYSPLL"_$PLATFORM.bin
fi

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

SYS_INIT_FILE=$OUT_DIR/sys_init.bin
MINILOADER_FILE=$OUT_DIR/miniloader_en.bin

# copy sys_init and miniloader
if [ -f $SYS_INIT_BIN_DIR/$SYS_INIT_BIN_FILE ]; then
echo "sys_init binary file: $SYS_INIT_BIN_DIR/$SYS_INIT_BIN_FILE"
else
SYS_INIT_BIN_FILE=sys_init_${BOOT_DEV}_secure_CPU"$CPUPLL"_MEM"$MEMPLL"_SYS"$SYSPLL"_$PLATFORM.bin
echo "sys_init binary file: $SYS_INIT_BIN_DIR/$SYS_INIT_BIN_FILE"
fi
cp $SYS_INIT_BIN_DIR/$SYS_INIT_BIN_FILE $SYS_INIT_FILE && cp $SYS_INIT_BIN_DIR/miniloader_en.bin $MINILOADER_FILE || exit 1

# copy partition table
[ $# = 1 ] && VT_PATH=$1 || VT_PATH=$SDK_PATH/MV88DE3100_Tools/.temp/version_table
if [ ! -f $VT_PATH ];then
	echo "please generate $VT_PATH first."
	exit 1
fi
PT_FILE=$OUT_DIR/partition_table
cp $VT_PATH $PT_FILE

# check bcm
if [ "$PRODUCTION_CHIP" = "1" ]; then
	BCM_FILE_NAME=bcm_erom_mdk.bin
	DRM_FILE_NAME=drm_erom_mdk.bin
elif [ "$PRODUCTION_CHIP" = "2" ]; then
	BCM_FILE_NAME=bcm_erom_otp.bin
	DRM_FILE_NAME=drm_erom_otp.bin
else
	BCM_FILE_NAME=bcm_erom.bin
	DRM_FILE_NAME=drm_erom.bin
fi
if [ ! -f $BCM_DIR/BCM/$CHIP_NAME/mid$MID/$BCM_FILE_NAME ]; then
	echo "Can not find $BCM_DIR/BCM/$CHIP_NAME/mid$MID/$BCM_FILE_NAME"
	exit 1
fi
BCM_FILE=$BCM_DIR/BCM/$CHIP_NAME/mid$MID/$BCM_FILE_NAME

# check tsm if exist
TSM_FILE=
if [ -f $BCM_DIR/BCM/$CHIP_NAME/mid$MID/$DRM_FILE_NAME ]; then
	TSM_FILE=$BCM_DIR/BCM/$CHIP_NAME/mid$MID/$DRM_FILE_NAME
fi

# copy android key if configed
if [ "$CONFIG_ANDROID_BOOTFLOW" = "y" ]; then
OEM_KEY_FILE=$OUT_DIR/oemk
USR_KEY_FILE=$OUT_DIR/usrk

cp $SDK_PATH/MV88DE3100_Tools/SecureEngine/BCM/$CHIP_NAME/oem_verity_key $OEM_KEY_FILE
cp $SDK_PATH/MV88DE3100_Tools/SecureEngine/BCM/$CHIP_NAME/usr_verity_key $USR_KEY_FILE

fi

# copy sstz if configed
if [ "$CONFIG_SSTZ" = "y" ]; then
	SSTZ_FILE=$OUT_DIR/sstz
	cp $SDK_PATH/Customization_Data/TZ_Image/$CHIP_NAME/sstz $SSTZ_FILE
fi

#pack bcm + sys_init + tsm
BST_FILE=$OUT_DIR/bst.bin
cat $BCM_FILE $SYS_INIT_FILE $TSM_FILE > $BST_FILE

if [ "$CONFIG_FIXED_SIZE" = "y" ]; then
	$OUT_DIR/../../script/align.sh  $BST_FILE $CONFIG_SYSINIT_SIZE
	$OUT_DIR/../../script/align.sh  $MINILOADER_FILE $CONFIG_MINILOADER_SIZE
else
	$OUT_DIR/../../script/align.sh  $BST_FILE $FLASH_PAGE_SIZE
	$OUT_DIR/../../script/align.sh  $MINILOADER_FILE $FLASH_PAGE_SIZE
fi

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

OEM_ROOT_RSA_SIGNK7=$BCM_DIR/BCM/$CHIP_NAME/mid$MID/codetype_05-06-07/OEM_ROOT_RSA_SIGNK7.store

# encrypted partition info table
#dd if=$VT_PATH of=$OUT_DIR/version conv=notrunc
PT_EN_FILE=$OUT_DIR/partition_table_en
if [ "$CONFIG_BOOTFLOW" = "CLEAR" ]; then
	cp $PT_FILE $PT_EN_FILE
else
	$ENC_TOOL -c $CHIP_NAME -m $MID -t PT -i $PT_FILE -o $PT_EN_FILE
fi

#append to 8k
./script/align.sh $PT_EN_FILE 8192
#page aligned
./script/align.sh $PT_EN_FILE $pagesize

if [ "$CONFIG_SSTZ" = "y" ]; then
SSTZ_EN_FILE=$OUT_DIR/sstz_en
if [ "$CONFIG_BOOTFLOW" = "CLEAR" ]; then
	cp $SSTZ_FILE $SSTZ_EN_FILE
else
	$ENC_TOOL -c $CHIP_NAME -m $MID -t TZK -i $SSTZ_FILE -o $SSTZ_EN_FILE
fi
fi

if [ "$BCM_ROM_KEY_DISABLE" = "1" ] || [ "$BCM_ROM_KEY_DISABLE" = "2" ]; then
	if [ -f $OEM_ROOT_RSA_SIGNK7 ]; then
		signk7_len=$(stat -c %s "$OEM_ROOT_RSA_SIGNK7")
		dd if=$BST_FILE of=$OUT_DIR/bcm_sysinit_tsm.tail bs=1 skip=$signk7_len
		rm -rf $BST_FILE
		cat $OEM_ROOT_RSA_SIGNK7 $OUT_DIR/bcm_sysinit_tsm.tail > $BST_FILE
		rm -rf $OUT_DIR/bcm_sysinit_tsm.tail
	else
		echo "can't find $OEM_ROOT_RSA_SINGK7"
		exit 1
	fi
fi

BSP_IMAGE_LIST=($BST_FILE $MINILOADER_FILE $OUT_DIR/image_layout_en.img $OUT_DIR/image_list.img)


# encrypted partition info
IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i PTIN -d $PT_EN_FILE -t attr0 2"

if [ "$CONFIG_SSTZ" = "y" ]; then
	IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i SSTZ -d $SSTZ_EN_FILE -t attr0 2 -a 0"
fi

if [ "$CONFIG_ANDROID_BOOTFLOW" = "y" ]; then
OEM_KEY_EN_FILE=$OUT_DIR/oemk_en
USR_KEY_EN_FILE=$OUT_DIR/usrk_en
if [ "$CONFIG_BOOTFLOW" = "CLEAR" ]; then
cp $OEM_KEY_FILE $OEM_KEY_EN_FILE
cp $USR_KEY_FILE $USR_KEY_EN_FILE
else
$ENC_TOOL -c $CHIP_NAME -m $MID -t ANDROID_KEY -i $OEM_KEY_FILE -o $OEM_KEY_EN_FILE
$ENC_TOOL -c $CHIP_NAME -m $MID -t ANDROID_KEY -i $USR_KEY_FILE -o $USR_KEY_EN_FILE
fi

./script/align.sh $OEM_KEY_EN_FILE 12288
./script/align.sh $USR_KEY_EN_FILE 12288

IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i OEMK -d $OEM_KEY_EN_FILE -t attr0 2"
IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i USRK -d $USR_KEY_EN_FILE -t attr0 2"
fi

echo "image_cmd_param = $IMAGE_CMD_PARAM"
#layout info reserve 12k
$SDK_PATH/MV88DE3100_Tools/.temp/genimg -n imglayout -V 0 -v 1 -A $pagesize $IMAGE_CMD_PARAM -o out/$PLATFORM/image_list.img -S out/$PLATFORM/image_layout.img 12288
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
	elif [ "$BCM_ROM_KEY_DISABLE" = "2" ]; then
		$ENC_TOOL -c $CHIP_NAME -m $MID -t BLI_DEBUG -i $OUT_DIR/image_layout.img -o $OUT_DIR/image_layout_en.img
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

fi

#FIXME: ?
# copy the final image to key_injection folder
cp $PT_FILE $KEY_INJECTION_DIR/
cp $OUT_DIR/init_loader.img $KEY_INJECTION_DIR/bootloader.subimg

# for anti-rollback
cp $BCM_DIR/BCM/$CHIP_NAME/anti_rollback_ver.h $SDK_PATH/MV88DE3100_Tools/.temp

echo --------------------
echo     begin end
echo --------------------
