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
# $Source: gen_OTA.sh
#==============================================================================================

###############################################################################################
#  Goal:  This script is used to convert pre-production OTA package to production OTA package.
#         In production OTA package rootrsa_signk7.keystore will be relpaced.
###############################################################################################

work_dir=`pwd`

function print_help() {
echo 'Usage: '
echo './gen_OTA_img.sh'
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

OTA_PKG_SOURCE=$(eval printf "%s" "${array_opts[ANDROID_OTA_PKG_SOURCE]}")
ANDROID_PATH=$(eval printf "%s" "${array_opts[ANDROID_PATH]}")
SDK_PATH=$(eval printf "%s" "${array_opts[SDK_PATH]}")
SDK_PRODUCT=$(eval printf "%s" "${array_opts[SDK_PRODUCT_NAME]}")
SIGNK7_FILE_PATH=$(eval printf "%s" "${array_opts[SIGNK7_FILE_PATH]}")

if [ "a$OTA_PKG_SOURCE" = "a" ] || [ "a$ANDROID_PATH" = "a" ] || [ "a$SDK_PATH" = "a" ] || [ "a$SDK_PRODUCT" = "a" ] || [ "a$SIGNK7_FILE_PATH" = "a" ]; then
   print_help;
   exit 1;
fi

TMP=$work_dir/tmp
PROD_ESMT=$SDK_PATH/out/$SDK_PRODUCT/target/preboot/preboot_esmt.bin
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

if [ ! -f $OTA_PKG_SOURCE ];then
  echo "Original OTA package does not exist..."
  exit 1
fi

if [ ! -f $SIGNK7_FILE_PATH ];then
  echo "rootrsa_signk7.keystore does not exist..."
  exit 1
fi

if [ ! -f $TESTKEY_X509 ];then
  echo "testkey.x509.pem does not exist, please check Android package..."
  exit 1
fi

if [ ! -f $TESTKEY_PK8 ];then
  echo "testkey.pk8 does not exist, please check Android package..."
  exit 1
fi

if [ "x$is_nocs" != "xy" ]; then
  if [ ! -f $PROD_ESMT ];then
    echo "preboot_esmt.bin does not exist, please check Synaptics SDK..."
    exit 1
  fi
fi

mkdir $TMP

signk7_size=$(stat -c %s "$SIGNK7_FILE_PATH")
if [ $signk7_size != 2068 ]; then
  echo "$SIGNK7_FILE_PATH size is not 2068"
  exit 1
fi

#unzip source OTA package
unzip $OTA_PKG_SOURCE -d $TMP/source_ota

#replace SIGNK7 store
if [ "x$is_nocs" == "xy" ]; then
  dd if=$TMP/source_ota/preboot.img of=$TMP/preboot.img.head bs=1 count=443040
  dd if=$TMP/source_ota/preboot.img of=$TMP/preboot.img.tail bs=1 skip=445108
  cat $TMP/preboot.img.head $SIGNK7_FILE_PATH $TMP/preboot.img.tail > $TMP/preboot.img
else
  dd if=$PROD_ESMT of=$TMP/tmp.bin bs=1 skip=$signk7_size
  cat $SIGNK7_FILE_PATH $TMP/tmp.bin > $TMP/preboot.img
fi

rm $TMP/source_ota/preboot.img
cp $TMP/preboot.img $TMP/source_ota/preboot.img
cd $TMP/source_ota/
zip -r unsigned_OTA.zip *
cd $work_dir
java -Xmx2048m -Djava.library.path=$ANDROID_PATH/out/host/linux-x86/lib64 -jar $ANDROID_PATH/out/host/linux-x86/framework/signapk.jar -w $ANDROID_PATH/build/target/product/security/testkey.x509.pem $ANDROID_PATH/build/target/product/security/testkey.pk8 $TMP/source_ota/unsigned_OTA.zip $work_dir/OTA_PROD.zip

rm -fr $TMP

echo "Please get the OTA package from $work_dir/OTA_PROD.zip"
