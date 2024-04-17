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
	export CONFIG_ANDROID_BOOTFLOW
}
pagesize=`echo $FLASH_TYPE | cut -d _ -f1`
page_per_block=`echo $FLASH_TYPE | cut -d _ -f3`
block_size=`expr $pagesize \* $page_per_block`
block_size_kB=`expr $block_size / 1024`
echo "pagesize=$pagesize , page_per_block=$page_per_block"
echo "block_size_kB=$block_size_kB"

WORKING_DIR=`pwd`
SDK_PATH=${WORKING_DIR%%MV88DE3100_SDK*}
SDK_PATH="$SDK_PATH"MV88DE3100_SDK

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
SI_CUST_FIGO_IMAGE_LOCAL=$OUT_DIR/SI_cust_figo.bin
SI_DDRSCRAM_FIGO_IMAGE_LOCAL=$OUT_DIR/SI_DDRSCRAM_figo.bin
SI_MINILOADER_IMAGE_LOCAL=$OUT_DIR/miniloader_en.bin

SYS_INIT_DIR=../sys_init/out
SYS_INIT_BIN_FILE=sys_init_${BOOT_DEV}_secure_CPU"$CPUPLL"_MEM"$MEMPLL"_SYS"$SYSPLL"_$PLATFORM.bin
if [ $CPUPLL -ge 800 ]; then
	SYS_INIT_BIN_FILE=sys_init_${BOOT_DEV}_secure_CPU800_MEM"$MEMPLL"_SYS"$SYSPLL"_$PLATFORM.bin
fi

[ $# = 1 ] && VT_PATH=$1 || VT_PATH=../../.temp/version_table

#if [ "$ROM_KEY_DISABLE" = "0" ]; then
#	cp diag_jtag_switch_figo_romkey.bin figo_image.bin
#else
#	cp diag_jtag_switch_figo_distkey.bin figo_image.bin
#fi

if [ "$PRODUCTION_CHIP" = "0" ]; then
	echo "Generating bootloader for engineer chip"
	if [ ! -f ../../DRM_Image/$CONFIG_SIGNTYPE/customer_keystore_figo.bin_ROM_Key ]; then
		echo "../../DRM_Image/$CONFIG_SIGNTYPE/customer_keystore_figo.bin_ROM_Key not found"
		exit 1
	fi
	DRM_IMAGE_FILE=../../DRM_Image/$CONFIG_SIGNTYPE/customer_keystore_figo.bin_ROM_Key
	if [ "is$PLATFORM" != "is" -a -d $SYS_INIT_DIR/$PLATFORM ]; then
		SYS_INIT_BIN_DIR=$SYS_INIT_DIR/$PLATFORM/${BOOT_DEV}_secure_romkey
	else
		SYS_INIT_BIN_DIR=$SYS_INIT_DIR/default/${BOOT_DEV}_secure_romkey
	fi
	if [ "$CONFIG_SIGNTYPE" = "RSA2048_SHA256" ]; then
		if [ ! -f ../../DRM_Image/$CONFIG_SIGNTYPE/ddr_scrambler_figo.bin_ROM_Key ]; then
			echo "../../DRM_Image/$CONFIG_SIGNTYPE/ddr_scrambler_figo.bin_ROM_Key not found"
			exit 1
		fi
	fi
	DRM_DDRSCRAM_IMAGE_FILE=../../DRM_Image/$CONFIG_SIGNTYPE/ddr_scrambler_figo.bin_ROM_Key
else
	echo "Generating bootloader for production chip"
	if [ ! -f ../../DRM_Image/$CONFIG_SIGNTYPE/customer_keystore_figo.bin_Dist_Key ]; then
		echo "../../DRM_Image/$CONFIG_SIGNTYPE/customer_keystore_figo.bin_Dist_Key not found"
		exit 1
	fi
	DRM_IMAGE_FILE=../../DRM_Image/$CONFIG_SIGNTYPE/customer_keystore_figo.bin_Dist_Key
	if [ "is$PLATFORM" != "is" -a -d $SYS_INIT_DIR/$PLATFORM ]; then
		SYS_INIT_BIN_DIR=$SYS_INIT_DIR/$PLATFORM/${BOOT_DEV}_secure_distkey
	else
		SYS_INIT_BIN_DIR=$SYS_INIT_DIR/default/${BOOT_DEV}_secure_distkey
	fi
	if [ "$CONFIG_SIGNTYPE" = "RSA2048_RSA256" ]; then
		if [ ! -f ../../DRM_Image/$CONFIG_SIGNTYPE/ddr_scrambler_figo.bin_Dist_Key ]; then
			echo "../../DRM_Image/$CONFIG_SIGNTYPE/ddr_scrambler_figo.bin_Dist_Key not found"
			exit 1
		fi
	fi
	DRM_DDRSCRAM_IMAGE_FILE=../../DRM_Image/$CONFIG_SIGNTYPE/ddr_scrambler_figo.bin_Dist_Key
fi

#	Copy system init part1 image and cust figo image to local
cp $DRM_IMAGE_FILE $SI_CUST_FIGO_IMAGE_LOCAL

if [ -f $SYS_INIT_BIN_DIR/$SYS_INIT_BIN_FILE ]; then
echo "sys_init binary file: $SYS_INIT_BIN_DIR/$SYS_INIT_BIN_FILE"
else
SYS_INIT_BIN_FILE=sys_init_${BOOT_DEV}_secure_CPU"$CPUPLL"_MEM"$MEMPLL"_SYS"$SYSPLL"_$PLATFORM.bin
echo "sys_init binary file: $SYS_INIT_BIN_DIR/$SYS_INIT_BIN_FILE"
fi
cp $SYS_INIT_BIN_DIR/$SYS_INIT_BIN_FILE $SYSTEM_INIT_FILE_LOCAL && cp $SYS_INIT_BIN_DIR/miniloader_en.bin $OUT_DIR/ || exit 1

#	Copy DDR Scrambler figo image to local if any
if [ ! -f $DRM_DDRSCRAM_IMAGE_FILE ] ; then
	echo "$DRM_DDRSCRAM_IMAGE_FILE not found, DDR Scrambler will be disabled"
else
	cp $DRM_DDRSCRAM_IMAGE_FILE $SI_DDRSCRAM_FIGO_IMAGE_LOCAL || exit 1
fi

if [ ! -f $VT_PATH ];then
	echo "please generate $VT_PATH first."
	exit 1
fi

echo cust_self_key=$CUST_SELFKEY_IMAGE
ls -l $WORKING_DIR/../../../Customization_Data/$CUST_SELFKEY_IMAGE

#$ADDHEADER_TOOL $OUT_DIR/bootloader.elf $OUT_DIR/bootloader.img && \
#cp -f ../../enc_tool $OUT_DIR && \
#cp -f ../../Common/tools/encryption.sh $OUT_DIR
#cp -f ../../Common/tools/encrypt $OUT_DIR


if [ "$CPU_IMAGE_ENCRYPT" = "y" ];then
	cp -f $LOCAL_CUST_SELFKEY_PATH $OUT_DIR
fi

if [ "$ROM_KEY_DISABLE" = "" ];then
    echo "ROM_KEY_DISABLE is not set in config file, please update your config file"
    exit 1
fi

if [ "$ROM_KEY_DISABLE" = "1" ]; then
	echo " ****************** ROM key has been disabled ************"

	if [ "$OTP_RSA_KEY_FILE_NAME" = "" ]; then
		echo "ROM KEY has been disabled, but OTP_RSA_KEY_FILE_NAME is not defined"
		exit 1
	fi

	if [ ! -f ../../../Customization_Data/${OTP_RSA_KEY_FILE_NAME} ];then
		echo " Can not find User RSA key file: ../../Customization_Data/${OTP_RSA_KEY_FILE_NAME} "
		exit 1
	fi
	cp ../../../Customization_Data/${OTP_RSA_KEY_FILE_NAME} ./
	#cp ../SPI_Secure_Uboot_Releases_Prod_Chip/ChangeSign ./
	if [ "is$CONFIG_SIGNTYPE" = "isRSA2048_SHA256" ]; then
		cp ../../kms_release_bg2q/seclab/bin/ChangeSign ./
	else
		cp ../../kms_release_bg2_32/seclab/bin/ChangeSign ./
	fi
	if [ ! -f ./ChangeSign ];then
		echo "ChangeSign doesn't exist"
		exit 1
	fi
	if [ "is$CONFIG_SIGNTYPE" = "isRSA2048_SHA256" ]; then
		echo "	resign system init..."
		./ChangeSign $SYSTEM_INIT_FILE_LOCAL 0 1
		echo "  resign cust figo image..."
		./ChangeSign $SI_CUST_FIGO_IMAGE_LOCAL 0 2
		echo "  resign DDRS figo image..."
		./ChangeSign $SI_DDRSCRAM_FIGO_IMAGE_LOCAL 0 2
		echo "  resign miniloader..."
		./ChangeSign $SI_MINILOADER_IMAGE_LOCAL 0 0
	else
		echo "	resign system init..."
		./ChangeSign $SYSTEM_INIT_FILE_LOCAL 0
		echo "  resign cust figo image..."
		./ChangeSign $SI_CUST_FIGO_IMAGE_LOCAL 0
	fi

fi

cp -vf $SYSTEM_INIT_FILE_LOCAL $OUT_DIR/image2_init.bin &&\
$ADDHEADER_TOOL $SI_CUST_FIGO_IMAGE_LOCAL $OUT_DIR/figo.img &&\
$ADDHEADER_TOOL $WORKING_DIR/../../../Customization_Data/${CUST_KEY_IMAGE} $OUT_DIR/key.img || exit 1

if [ -f $SI_DDRSCRAM_FIGO_IMAGE_LOCAL ] ; then
	$ADDHEADER_TOOL $SI_DDRSCRAM_FIGO_IMAGE_LOCAL $OUT_DIR/ddr_scrambler.img || exit 1
else
	rm $OUT_DIR/ddr_scrambler.img &> /dev/null
fi

# copy android key if configed
if [ "$CONFIG_ANDROID_BOOTFLOW" = "y" ]; then
OEM_KEY_FILE=$OUT_DIR/oemk
USR_KEY_FILE=$OUT_DIR/usrk

cp $SDK_PATH/Customization_Data/Android_Key/oem_verity_key $OEM_KEY_FILE
cp $SDK_PATH/Customization_Data/Android_Key/usr_verity_key $USR_KEY_FILE
fi

echo --------------------
echo     begin padding
echo --------------------
# current layout of bootloader.subimg
#---------------------
# 52K  |  sys_init binary
#---------------------
# 4K   |  encrypted image layout info
#---------------------
# 4K   |  Custkey
#---------------------
# 20K  |  DRM Custkey loading FW
#---------------------
# 20K  |  DRM DDR scrambler FW
#---------------------
# 36K  |  miniloader
#---------------------
# 4K   |  Partition version table(encypted)
#---------------------
#---------------------
#      |  others like bootloader, jtag_debug
#      |  can be find by parsing image lst header
#---------------------

# customer key
SIZE=`stat -c %s $OUT_DIR/key.img`
	dd if=/dev/urandom of=$OUT_DIR/key.img bs=1 seek=$SIZE count=`expr 4096 - $SIZE` conv=notrunc || exit 1

# figo img, 20K
SIZE=`stat -c %s $OUT_DIR/figo.img`
dd if=/dev/urandom of=$OUT_DIR/figo.img bs=1 seek=$SIZE count=`expr 20480 - $SIZE` conv=notrunc || exit 1

# ddr scrambler fw, append to 20K
# now we have no ddr scrambler fw image. so we just generate an image filled with zero data
if [ -f $OUT_DIR/ddr_scrambler.img ] ; then
SIZE=`stat -c %s $OUT_DIR/ddr_scrambler.img`
test $SIZE -le 20480 || (echo $OUT_DIR/ddr_scrambler.img exceeds 20K && exit 1)
dd if=/dev/urandom of=$OUT_DIR/ddr_scrambler.img bs=1 seek=$SIZE count=`expr 20480 - $SIZE` conv=notrunc || exit 1
else
dd if=/dev/zero of=$OUT_DIR/ddr_scrambler.img bs=1 count=`expr 20480` conv=notrunc || exit 1
fi

# encrypted version table
dd if=$VT_PATH of=$OUT_DIR/version conv=notrunc
./script/encryption.sh $OUT_DIR/version $OUT_DIR/version_en || {
	echo fail to encrypt version table
	exit 1
}
SIZE=`stat -c %s $OUT_DIR/version_en`
echo "version_en size = $SIZE"
dd if=/dev/urandom of=$OUT_DIR/version_en bs=1 seek=$SIZE count=`expr 4096 - $SIZE` conv=notrunc || exit 1

#BOOTLOADER_IMAGE_LIST=($OUT_DIR/image2_init.bin $OUT_DIR/key.img $OUT_DIR/figo.img $OUT_DIR/ddr_scrambler.img $OUT_DIR/version_en $OUT_DIR/generic_image_list_en.img)
BOOTLOADER_IMAGE_LIST=($OUT_DIR/image2_init.bin $OUT_DIR/image_header_en.img $OUT_DIR/image_list.img)

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

rm -rf $OUT_DIR/bootloader_image_list.img $OUT_DIR/bootloader_image_list_en.img

echo "bl_image_cmd_param = $BL_IMAGE_CMD_PARAM"
$temp_dir/genimg -n bootloader -V 0 -v 1 $BL_IMAGE_CMD_PARAM -o $OUT_DIR/bootloader_image_list.img
if [ "$?" != "0" ];then
	echo "$0,generate bootloader header error"
	exit 1
fi

# if any other bootloader related image, append it here

# encrypt the bootloader image list including header
./script/encryption.sh $OUT_DIR/bootloader_image_list.img $OUT_DIR/bootloader_image_list_en.img || {
	echo fail to generate generic_image_list_en.img
	exit 1
}

echo ----------------------------------------------
echo     end generate bootloader_image_list.img
echo ----------------------------------------------

echo ----------------------------------------------
echo     begin generate final image_list.img
echo ----------------------------------------------

#if bcm, only version table is needed
if [ "$CONFIG_BCM" != "y" ]; then
# cust key must be the first one
IMAGE_CMD_PARAM=" -i CUTK -d $OUT_DIR/key.img -t attr0 1"
# figo cust key loading fw
IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i CTFW -d $OUT_DIR/figo.img -t attr0 3"
# ddr scrambler
IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i DDRS -d $OUT_DIR/ddr_scrambler.img -t attr0 3"
fi
# miniloader
IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i MMLD -d $OUT_DIR/miniloader_en.bin -t attr0 2"
# encrypted partition info
IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i PTIN -d $OUT_DIR/version_en -t attr0 2"

if [ "$CONFIG_ANDROID_BOOTFLOW" = "y" ]; then
	OEM_KEY_EN_FILE=$OUT_DIR/oemk_en
	USR_KEY_EN_FILE=$OUT_DIR/usrk_en
	if [ "$CONFIG_BOOTFLOW" = "CLEAR" ]; then
		cp $OEM_KEY_FILE $OEM_KEY_EN_FILE
		cp $USR_KEY_FILE $USR_KEY_EN_FILE
	else
		./script/encryption.sh $OEM_KEY_FILE $OEM_KEY_EN_FILE || {
			echo fail to encrypt android oem key
			exit 1
		}
		./script/encryption.sh $USR_KEY_FILE $USR_KEY_EN_FILE || {
			echo fail to encrypt android user key
			exit 1
		}
	fi

	./script/align.sh $OEM_KEY_EN_FILE 12288
	./script/align.sh $USR_KEY_EN_FILE 12288

	IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i OEMK -d $OEM_KEY_EN_FILE -t attr0 2"
	IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i USRK -d $USR_KEY_EN_FILE -t attr0 2"
fi

if [ "$CONFIG_POST_BOOTLOADER" = "y" ]; then
cp $OUT_DIR/bootloader_image_list_en.img $OUT_DIR/post_bootloader.img
else
# bootloader image list
IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i BTLR -d $OUT_DIR/bootloader_image_list_en.img -t attr0 2"
fi

if [ "$JTAG_ENABLE" = "y" ]; then
	if [ "$PRODUCTION_CHIP" = "0" ]; then
		cp $SYS_INIT_DIR/../source/jtag_debug/$CONFIG_SIGNTYPE/diag_jtag_switch_figo_romkey.bin $OUT_DIR/jtag_enable.bin
	else
		cp $SYS_INIT_DIR/../source/jtag_debug/$CONFIG_SIGNTYPE/diag_jtag_switch_figo_distkey.bin $OUT_DIR/jtag_enable.bin
	fi
	IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i JTAG -d $OUT_DIR/jtag_enable.bin -t attr0 3"
fi

if [ "$GETUNITID_ENABLE" = "y" ]; then
	if [ "$PRODUCTION_CHIP" = "0" ]; then
		cp $SYS_INIT_DIR/../source/get_device_id/$CONFIG_SIGNTYPE/diag_get_unitid_figo_romkey.bin $OUT_DIR/getunitid_enable.bin
	else
		cp $SYS_INIT_DIR/../source/get_device_id/$CONFIG_SIGNTYPE/diag_get_unitid_figo_distkey.bin $OUT_DIR/getunitid_enable.bin
	fi
	IMAGE_CMD_PARAM="$IMAGE_CMD_PARAM -i UNIT -d $OUT_DIR/getunitid_enable.bin -t attr0 3"
fi


rm -rf $OUT_DIR/image_list.img $OUT_DIR/image_header.img $OUT_DIR/image_header_en.img

echo "image_cmd_param = $IMAGE_CMD_PARAM"
$temp_dir/genimg -n imglayout -V 0 -v 1 -A 4096 $IMAGE_CMD_PARAM -o $OUT_DIR/image_list.img -S $OUT_DIR/image_header.img 4096
if [ "$?" != "0" ];then
        echo "$0,generate bootloader header error"
        exit 1
fi

# copy the image header image_header.img to key_injection folder
cp $OUT_DIR/image_header.img $KEY_INJECTION_DIR/

# encrypt the image header
./script/encryption.sh $OUT_DIR/image_header.img $OUT_DIR/image_header_en.img || {
	echo fail to generate generic_image_list_en.img
	exit 1
}

SIZE=`stat -c %s $OUT_DIR/image_header_en.img`
	dd if=/dev/urandom of=$OUT_DIR/image_header_en.img bs=1 seek=$SIZE count=`expr 4096 - $SIZE` conv=notrunc || exit 1

echo ----------------------------------------------
echo     end generate final image_list.img
echo ----------------------------------------------

echo "BOOTLOADER_IMAGE_LIST = ${BOOTLOADER_IMAGE_LIST[@]}";
cat ${BOOTLOADER_IMAGE_LIST[@]} > $OUT_DIR/init_loader.img || exit 1

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
# copy the final image to key_injection folder
cp $OUT_DIR/version $KEY_INJECTION_DIR/
cp $OUT_DIR/init_loader.img $KEY_INJECTION_DIR/bootloader.subimg

rm CUSTOMER_SELF_KEYS.bin
echo --------------------
echo     begin end
echo --------------------
