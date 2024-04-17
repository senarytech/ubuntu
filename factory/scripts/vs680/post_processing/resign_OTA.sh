#!/bin/bash
#==============================================================================================
#
# Copyright © 2011-2020 Synaptics Incorporated. All rights reserved.
#
# This file contains information that is proprietary to Synaptics Incorporated ("Synaptics").
# The holder of this file shall treat all information contained herein as confidential,
# shall use the information only for its intended purpose, and shall not duplicate, disclose,
# or disseminate any of this information in any manner unless Synaptics has otherwise provided
# express, written permission.
#
# $Source: resign_OTA.sh
#==============================================================================================

###############################################################################################
#  Goal: This script is for customer to resign their OTA package with new custk and extrsa
###############################################################################################

work_dir=`pwd`

function print_help() {
echo 'Usage: '
echo './resign_OTA.sh'
echo 'Please fill the config file first'
}

unset -v array_opts
declare -A array_opts

cfg_file=${work_dir}/config
list_cfg=$(cat ${work_dir}/config)

if [ ! -f $cfg_file ]; then
  echo "Can not find the config file"
  exit 1
fi

for c in $list_cfg; do
   declare v=$(expr $c : "^\s*CONFIG_\([A-Z0-9_]\+=[y|n\"].*\)")

   if [ "x$v" != "x" ]; then
     prod_opt=$(echo $v | cut -d = -f 1)
     prod_val=$(echo $v | cut -d = -f 2)

     echo "$prod_opt=$prod_val"
     array_opts[$prod_opt]=$prod_val
   fi
done

RESIGN_OTA_PKG_SOURCE=$(eval printf "%s" "${array_opts[RESIGN_OTA_PKG_SOURCE]}")
ANDROID_PATH=$(eval printf "%s" "${array_opts[ANDROID_PATH]}")
SDK_PATH=$(eval printf "%s" "${array_opts[SDK_PATH]}")
SDK_PRODUCT=$(eval printf "%s" "${array_opts[SDK_PRODUCT_NAME]}")
SIGNK7_FILE_PATH=$(eval printf "%s" "${array_opts[SIGNK7_FILE_PATH]}")
OLD_OEM_CUSTK_FILE_PATH=$(eval printf "%s" "${array_opts[OLD_OEM_CUSTK_FILE_PATH]}")
NEW_OEM_CUSTK_FILE_PATH=$(eval printf "%s" "${array_opts[NEW_OEM_CUSTK_FILE_PATH]}")
NEW_EXTRSA_KEY_FILE_PATH=$(eval printf "%s" "${array_opts[NEW_EXTRSA_KEY_FILE_PATH]}")

if [ "a$RESIGN_OTA_PKG_SOURCE" = "a" ] || [ "a$ANDROID_PATH" = "a" ] || [ "a$SDK_PATH" = "a" ] || [ "a$SDK_PRODUCT" = "a" ] || [ "a$SIGNK7_FILE_PATH" = "a" ] || [ "a$OLD_OEM_CUSTK_FILE_PATH" = "a" ] || [ "a$NEW_OEM_CUSTK_FILE_PATH" = "a" ] || [ "a$NEW_EXTRSA_KEY_FILE_PATH" = "a" ]; then
   print_help;
   exit 1;
fi

TMP=$work_dir/tmp
TOOL_RESIGN=$work_dir/bin/resign_image_v4
TESTKEY_X509=$ANDROID_PATH/build/target/product/security/testkey.x509.pem
TESTKEY_PK8=$ANDROID_PATH/build/target/product/security/testkey.pk8
FACTORY_BK=$SDK_PATH/out/$SDK_PRODUCT/target/preboot/intermediate/release/factory_bk.subimg

if [ -f $FACTORY_BK ]; then
  echo "NOCS type_m chip"
  is_nocs=y
fi

if [ -d $TMP ];then
  echo "tmp folder exist, delete..."
  rm -fr $TMP
fi

if [ ! -f $TOOL_RESIGN ];then
  echo "Resign tool  does not exist..."
  exit 1
fi

if [ ! -f $RESIGN_OTA_PKG_SOURCE ];then
  echo "Resign OTA package does not exist..."
  exit 1
fi

if [ ! -f $SIGNK7_FILE_PATH ];then
  echo "rootrsa_signk7.keystore does not exist..."
  exit 1
fi

        print 'Victor'
if [ ! -f $TESTKEY_X509 ];then
  echo "testkey.x509.pem does not exist, please check Android package..."
  exit 1
fi

if [ ! -f $TESTKEY_PK8 ];then
  echo "testkey.pk8 does not exist, please check Android package..."
  exit 1
fi

if [ ! -f $OLD_OEM_CUSTK_FILE_PATH ];then
  echo "old oem custk does not exist..."
  exit 1
fi

if [ ! -f $NEW_OEM_CUSTK_FILE_PATH ];then
  echo "new oem custk does not exist..."
  exit 1
fi

if [ ! -f $NEW_EXTRSA_KEY_FILE_PATH ];then
  echo "new oem extrsa key does not exist..."
  exit 1
fi

mkdir $TMP

signk7_size=$(stat -c %s "$SIGNK7_FILE_PATH")
if [ $signk7_size != 2068 ]; then
  echo "$SIGNK7_FILE_PATH size is not 2068"
  exit 1
fi

#unzip source OTA package
unzip $RESIGN_OTA_PKG_SOURCE -d $TMP/source_ota

#get preboot.img and bl_normal from OTA package
cp $TMP/source_ota/preboot.img $TMP/preboot.img
cp $TMP/source_ota/bl_normal.img $TMP/bl_normal.img

#replace SIGNK7 store
if [ "x$is_nocs" == "xy" ]; then
  dd if=$TMP/source_ota/preboot.img of=$TMP/preboot.img.head bs=1 count=443040
  dd if=$TMP/source_ota/preboot.img of=$TMP/preboot.img.tail bs=1 skip=445108
  cat $TMP/preboot.img.head $SIGNK7_FILE_PATH $TMP/preboot.img.tail > $TMP/preboot.img
else
  dd if=$TMP/preboot.img of=$TMP/tmp.bin bs=1 skip=$signk7_size
  cat $SIGNK7_FILE_PATH $TMP/tmp.bin > $TMP/preboot.img
fi

#put preboot img back to OTA package
cp $TMP/preboot.img $TMP/source_ota/preboot.img

#slice bl_normal
dd if=$TMP/bl_normal.img of=$TMP/bl_normal_header bs=1 count=64
dd if=$TMP/bl_normal.img of=$TMP/bl_normal_payload bs=1 skip=64

#resign bl_normal
$TOOL_RESIGN -t 2 -T 0 -C $OLD_OEM_CUSTK_FILE_PATH -N $NEW_OEM_CUSTK_FILE_PATH -S $NEW_EXTRSA_KEY_FILE_PATH -i $TMP/bl_normal_payload -o $TMP/bl_normal_payload_resigned

#reassemble bl_normal
cat $TMP/bl_normal_header $TMP/bl_normal_payload_resigned > $TMP/bl_normal.img

#put oem bootloader img back to OTA package
cp $TMP/bl_normal.img $TMP/source_ota/bl_normal.img

#packing OTA
cd $TMP/source_ota/
zip -r unsigned_OTA.zip *
cd $work_dir
java -Xmx2048m -Djava.library.path=$ANDROID_PATH/out/host/linux-x86/lib64 -jar $ANDROID_PATH/out/host/linux-x86/framework/signapk.jar -w $ANDROID_PATH/build/target/product/security/testkey.x509.pem $ANDROID_PATH/build/target/product/security/testkey.pk8 $TMP/source_ota/unsigned_OTA.zip $work_dir/OTA_RESIGNED.zip

rm -fr $TMP

echo "Please get OTA package from $work_dir/OTA_RESIGNED.zip"
