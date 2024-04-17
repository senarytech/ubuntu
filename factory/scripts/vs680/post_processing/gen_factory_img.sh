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
# $Source: gen_factory_img.sh
#==============================================================================================

###############################################################################################
#  Goal:  This script is used to generate factory eMMC image that from pre-production eMMC image.
#         In factory eMMC image rootrsa_signk7.keystore will be replaced and production.subimg
#         will be injected in /vendor/factory folder.
###############################################################################################

work_dir=`pwd`

function print_help() {
echo 'Usage: '
echo './gen_factory_img.sh'
echo 'Please fill the config file first'
}

function print_MDK_warning() {
echo "!!!Not MDK images!!!"
echo "Please make sure to use Synaptics MDK SDK to build the Factory images"
exit 1
}

cfg_file=${work_dir}/config
list_cfg=$(cat ${work_dir}/config)

if [ ! -f $cfg_file ]; then
  echo "Can not find the config file"
  exit 1
fi

unset -v array_opts
declare -A array_opts

for c in $list_cfg; do
   declare v=$(expr $c : "^\s*CONFIG_\([A-Z0-9_]\+=[y|n\"].*\)")

   if [ "x$v" != "x" ]; then
     prod_opt=$(echo $v | cut -d = -f 1)
     prod_val=$(echo $v | cut -d = -f 2)

     echo "$prod_opt=$prod_val"
     array_opts[$prod_opt]=$prod_val
   fi
 done

ANDROID_PATH=$(eval printf "%s" "${array_opts[ANDROID_PATH]}")
SDK_PATH=$(eval printf "%s" "${array_opts[SDK_PATH]}")
PRODUCT_NAME=$(eval printf "%s" "${array_opts[ANDROID_PRODUCT_NAME]}")
SDK_PRODUCT=$(eval printf "%s" "${array_opts[SDK_PRODUCT_NAME]}")

if [ "a$ANDROID_PATH" = "a" ] || [ "a$SDK_PATH" = "a" ] || [ "a$PRODUCT_NAME" = "a" ] || [ "a$SDK_PRODUCT" = "a" ]; then
  print_help;
  exit 1;
fi

TMP=$work_dir/tmp
EMMC_ORG=$ANDROID_PATH/out/target/product/$PRODUCT_NAME/eMMCimg
EMMC_FACTORY=$work_dir/eMMCimg_Factory
FILE_CONTEXTS=$ANDROID_PATH/out/target/product/$PRODUCT_NAME/obj/ETC/file_contexts.bin_intermediates/file_contexts.bin
CMD_MAKE2FS=$ANDROID_PATH/out/host/linux-x86/bin/mke2fs
CMD_E2FSDROID=$ANDROID_PATH/out/host/linux-x86/bin/e2fsdroid
FACTORY_BK=$SDK_PATH/out/$SDK_PRODUCT/target/preboot/intermediate/release/factory_bk.subimg

if [ -f $FACTORY_BK ]; then
  echo "NOCS type_m chip"
  is_nocs=y
fi

if [ "x$is_nocs" != "xy" ]; then
  PROD_ESMT=$SDK_PATH/out/$SDK_PRODUCT/target/preboot/preboot_esmt.bin
  FACTORY_ESMT=$SDK_PATH/out/$SDK_PRODUCT/target/preboot/preboot_esmt_factory.bin
fi

if [ -d $TMP ];then
  echo "tmp folder exist, delete..."
  rm -fr $TMP
fi

if [ -d $EMMC_FACTORY ];then
  echo "eMMCimg_Factory folder exist, delete..."
  rm -fr $EMMC_FACTORY
fi

if [ ! -d $EMMC_ORG ];then
  echo "eMMCimg folder does not exist, please build Android first..."
  exit 1
fi

if [ ! -f $FILE_CONTEXTS ];then
  echo "Android file contexts does not exist, please build Android first..."
  exit 1
fi

if [ ! -f $CMD_MAKE2FS ] || [ ! -f $CMD_E2FSDROID ]; then
  echo "mke2fs or e2fsdroid command not found"
  echo "Please run below commands in Android first"
  echo ". build/envsetup.sh"
  echo "lunch [product]"
  exit 1
fi

if [ "x$is_nocs" != "xy" ]; then
  if [ ! -f $PROD_ESMT ];then
    echo "production preboot_esmt.bin does not exist, please check Synaptics SDK..."
    exit 1
  fi

  if [ ! -f $FACTORY_ESMT ];then
    echo "production preboot_esmt_factory.bin does not exist, please check Synaptics SDK..."
    exit 1
  fi
fi

mkdir $TMP
cp -fr $EMMC_ORG $EMMC_FACTORY


if [ "x$is_nocs" == "xy" ]; then
  #Check MDK image
  mdk_check=$(xxd -p -s 445108 $EMMC_FACTORY/preboot.subimg | head -c 4)
  if [ $mdk_check == 5001 ]; then
    dd if=$EMMC_FACTORY/preboot.subimg of=$TMP/preboot.subimg.head bs=1 count=344736
    #344736 + 98304 = 443040
    dd if=$EMMC_FACTORY/preboot.subimg of=$TMP/preboot.subimg.tail bs=1 skip=443040
    dd if=$EMMC_FACTORY/preboot.subimg of=$TMP/production.bk bs=1 skip=344736 count=98304
    mkdir $TMP/factory_setting
    cp $TMP/production.bk $TMP/factory_setting/production.subimg
  else
    print_MDK_warning;
    exit 1
  fi
else
  mdk_check=$(xxd -p -s 8196 $PROD_ESMT | head -c 8)
  if [ $mdk_check == 37c20001 ]; then
    cp -f $PROD_ESMT $TMP/production.subimg
    mkdir $TMP/factory_setting
    cp $TMP/production.subimg $TMP/factory_setting/
  else
    print_MDK_warning;
    exit 1
  fi
fi

#re-packing factory_setting subimg
$CMD_MAKE2FS -L /vendor/factory -t ext4 -b 4096 $TMP/factory_setting.img 4096
$CMD_E2FSDROID -e -p $TMP/factory_setting -S $FILE_CONTEXTS -f $TMP/factory_setting -a /vendor/factory $TMP/factory_setting.img

mv $TMP/factory_setting.img $TMP/factory_setting.subimg
if [ -f $EMMC_FACTORY/factory_setting.subimg.gz ]; then
  gzip $TMP/factory_setting.subimg
  cp -f $TMP/factory_setting.subimg.gz $EMMC_FACTORY
else
  cp -f $TMP/factory_setting.subimg $EMMC_FACTORY
fi

#re-packing preboot.subimg
if [ "x$is_nocs" == "xy" ]; then
  cat $TMP/preboot.subimg.head $FACTORY_BK $TMP/preboot.subimg.tail > $TMP/preboot.subimg
else
  cp -f $FACTORY_ESMT $TMP/preboot.subimg
fi

if [ -f $EMMC_FACTORY/preboot.subimg ]; then
  cp -f $TMP/preboot.subimg $EMMC_FACTORY
elif [ -f $EMMC_FACTORY/preboot.subimg.gz ]; then
  gzip $TMP/preboot.subimg
  cp -f $TMP/preboot.subimg.gz $EMMC_FACTORY
else
  echo "Error: preboot.subimg not found!!"
  exit 1
fi

if [ -f $EMMC_FACTORY/emmc_image_list_full ]; then
  cp $EMMC_FACTORY/emmc_image_list $EMMC_FACTORY/emmc_image_list_org
  cp $EMMC_FACTORY/emmc_image_list_full $EMMC_FACTORY/emmc_image_list
fi

rm -fr $TMP

echo "Please get the Factory eMMCimg from $EMMC_FACTORY folder"
