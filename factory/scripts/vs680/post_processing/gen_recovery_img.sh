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
# $Source: gen_recovery_img.sh
#==============================================================================================

###############################################################################################
#  Goal:  This script is used to generate recovery eMMC image that from pre-production eMMC image.
#         In recovery eMMC image rootrsa_signk7.keystore, custk_aesk0.keystore and
#         extrsa_signk7.keystore will be replaced and you can decide whether or not to erase
#         Application key stores in /vendor/factory
###############################################################################################

work_dir=`pwd`

function print_help() {
echo 'Usage: '
echo './gen_recovery_img.sh'
echo 'Please fill the config file first'
}

function get_image_aligned() {
f_input=$1;
align_size=$2;

### Check input file ###
if [ ! -f $f_input ];then
   echo "$f_input file not exist..."
   exit 1
fi

f_size=`stat -c %s ${f_input}`

append_size=`expr $align_size - $f_size % $align_size`
if [ $append_size -lt $align_size ]; then
   dd if=/dev/zero of=$f_input bs=1 seek=$f_size count=$append_size conv=notrunc
fi
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
SIGNK7_FILE_PATH=$(eval printf "%s" "${array_opts[SIGNK7_FILE_PATH]}")
CUSTK_FILE_PATH=$(eval printf "%s" "${array_opts[CUSTK_FILE_PATH]}")
EXTRSA_KEY_FILE_PATH=$(eval printf "%s" "${array_opts[EXTRSA_KEY_FILE_PATH]}")
SKIP_FACTORY_SETTING=$(eval printf "%s" "${array_opts[SKIP_FACTORY_SETTING]}")

if [ "a$ANDROID_PATH" = "a" ] || [ "a$SDK_PATH" = "a" ] || [ "a$PRODUCT_NAME" = "a" ] || [ "a$SDK_PRODUCT" = "a" ] || [ "a$SIGNK7_FILE_PATH" = "a" ] || [ "a$CUSTK_FILE_PATH" = "a" ] || [ "a$EXTRSA_KEY_FILE_PATH" = "a" ]; then
  print_help;
  exit 1;
fi

if [ $# -gt 0 ] && [ -d $1 ]; then
  echo "$1 does exist."
  EMMC_ORG=$1
else
  echo "Use org eMMCimg from Android out directory"
  EMMC_ORG=$ANDROID_PATH/out/target/product/$PRODUCT_NAME/eMMCimg
fi

TMP=$work_dir/tmp
EMMC_RECOVERY=$work_dir/eMMCimg_Recovery
PROD_ESMT=$SDK_PATH/out/$SDK_PRODUCT/target/preboot/preboot_esmt.bin
CUSTK_NAME=$(basename $CUSTK_FILE_PATH)
EXTRSA_KEY_FILE_NAME=$(basename $EXTRSA_KEY_FILE_PATH)
FACTORY_BK=$SDK_PATH/out/$SDK_PRODUCT/target/preboot/intermediate/release/factory_bk.subimg

if [ -f $FACTORY_BK ]; then
  echo "NOCS type_m chip"
  is_nocs=y
fi

if [ -d $TMP ];then
  echo "tmp folder exist, delete..."
  rm -fr $TMP
fi

if [ -d $EMMC_RECOVERY ];then
  echo "eMMCimg_Recovery folder exist, delete..."
  rm -fr $EMMC_RECOVERY
fi

if [ ! -d $EMMC_ORG ];then
  echo "eMMCimg folder does not exist, please build Android first..."
  exit 1
fi

if [ ! -f $SIGNK7_FILE_PATH ];then
  echo "rootrsa_signk7.keystore does not exist..."
  exit 1
fi

if [ ! -f $CUSTK_FILE_PATH ];then
  echo "custk_aesk0.keystore does not exist..."
  exit 1
fi

if [ ! -f $EXTRSA_KEY_FILE_PATH ];then
  echo "extrsa_signk7.keystore does not exist..."
  exit 1
fi

if [ "x$is_nocs" != "xy" ]; then
  if [ ! -f $PROD_ESMT ];then
    echo "preboot_esmt.bin does not exist, please check Synaptics SDK..."
    exit 1
  fi
fi

mkdir $TMP
cp -fr $EMMC_ORG $EMMC_RECOVERY

signk7_size=$(stat -c %s "$SIGNK7_FILE_PATH")
if [ $signk7_size != 2068 ]; then
  echo "$SIGNK7_FILE_PATH size is not 2068"
  exit 1
fi

#replace SIGNK7 store
if [ "x$is_nocs" == "xy" ]; then
  if [ -f $EMMC_RECOVERY/preboot.subimg ]; then
    dd if=$EMMC_RECOVERY/preboot.subimg of=$TMP/preboot.img.head bs=1 count=443040
    dd if=$EMMC_RECOVERY/preboot.subimg of=$TMP/preboot.img.tail bs=1 skip=445108
    cat $TMP/preboot.img.head $SIGNK7_FILE_PATH $TMP/preboot.img.tail > $TMP/preboot.subimg
  elif [ -f $EMMC_RECOVERY/preboot.subimg.gz ]; then
    gunzip $TMP/preboot.subimg.gz
    dd if=$EMMC_RECOVERY/preboot.subimg of=$TMP/preboot.img.head bs=1 count=443040
    dd if=$EMMC_RECOVERY/preboot.subimg of=$TMP/preboot.img.tail bs=1 skip=445108
    cat $TMP/preboot.img.head $SIGNK7_FILE_PATH $TMP/preboot.img.tail > $TMP/preboot.subimg
  fi
else
  dd if=$PROD_ESMT of=$TMP/tmp.bin bs=1 skip=$signk7_size
  cat $SIGNK7_FILE_PATH $TMP/tmp.bin > $TMP/preboot.subimg
fi

if [ -f $EMMC_RECOVERY/preboot.subimg ]; then
  cp -f $TMP/preboot.subimg $EMMC_RECOVERY
elif [ -f $EMMC_RECOVERY/preboot.subimg.gz ]; then
  gzip $TMP/preboot.subimg
  cp -f $TMP/preboot.subimg.gz $EMMC_RECOVERY
else
  echo "Error: preboot.subimg not found!!"
  exit 1
fi

if [ "a$SKIP_FACTORY_SETTING" = "aY" ] || [ "a$SKIP_FACTORY_SETTING" = "ay" ]; then
  echo "Delete factory_setting from eMMCimg_Recovery"
  sed -i '/factory_setting/d' $EMMC_RECOVERY/emmc_image_list
  rm -f $EMMC_RECOVERY/factory_setting.subimg $EMMC_RECOVERY/factory_setting.subimg.gz
fi

#replace CUSTK/EXT_RSA store
cp $CUSTK_FILE_PATH $TMP
cp $EXTRSA_KEY_FILE_PATH $TMP

export KEY_SUBIMG=NONE
export KEY_TYPE=NONE
if [ -f $EMMC_RECOVERY/key_1st.subimg.gz ]; then
  KEY_SUBIMG="GZIPPED"
  KEY_TYPE="NON-AB"
elif [ -f $EMMC_RECOVERY/key.subimg.gz ]; then
  KEY_SUBIMG="GZIPPED"
  KEY_TYPE="AB"
elif [ -f $EMMC_RECOVERY/key_1st.subimg ]; then
  KEY_SUBIMG="RAW"
  KEY_TYPE="NON-AB"
elif [ -f $EMMC_RECOVERY/key.subimg ]; then
  KEY_SUBIMG="RAW"
  KEY_TYPE="AB"
else
  echo "Error: key subimg not found!!"
  exit 1
fi

if [ "a$KEY_SUBIMG" = "aGZIPPED" ] && [ "a$KEY_TYPE" = "aNON-AB" ]; then
  cp $EMMC_RECOVERY/key_1st.subimg.gz $TMP
  cp $EMMC_RECOVERY/key_2nd.subimg.gz $TMP
  gunzip $TMP/key_1st.subimg.gz
  gunzip $TMP/key_2nd.subimg.gz
elif [ "a$KEY_SUBIMG" = "aRAW" ] && [ "a$KEY_TYPE" = "aNON-AB" ]; then
  cp $EMMC_RECOVERY/key_1st.subimg $TMP
  cp $EMMC_RECOVERY/key_2nd.subimg $TMP
elif [ "a$KEY_SUBIMG" = "aGZIPPED" ] && [ "a$KEY_TYPE" = "aAB" ]; then
  cp $EMMC_RECOVERY/key.subimg.gz $TMP
  gunzip $TMP/key.subimg.gz
elif [ "a$KEY_SUBIMG" = "aRAW" ] && [ "a$KEY_TYPE" = "aAB" ]; then
  cp $EMMC_RECOVERY/key.subimg $TMP
fi

get_image_aligned $TMP/$CUSTK_NAME 1024
cat $TMP/$CUSTK_NAME $TMP/$EXTRSA_KEY_FILE_NAME > $TMP/key.bin
get_image_aligned $TMP/key.bin 16384

if [ "a$KEY_TYPE" = "aNON-AB" ]; then
  dd if=$TMP/key_1st.subimg of=$TMP/tmp1.bin bs=1 skip=16384
  cat $TMP/key.bin $TMP/tmp1.bin > $TMP/key_1st.subimg

  dd if=$TMP/key_2nd.subimg of=$TMP/tmp2.bin bs=1 skip=16384
  cat $TMP/key.bin $TMP/tmp2.bin > $TMP/key_2nd.subimg
elif [ "a$KEY_TYPE" = "aAB" ]; then
  dd if=$TMP/key.subimg of=$TMP/tmp1.bin bs=1 skip=16384
  cat $TMP/key.bin $TMP/tmp1.bin > $TMP/key.subimg
fi

if [ "a$KEY_SUBIMG" = "aGZIPPED" ] && [ "a$KEY_TYPE" = "aNON-AB" ]; then
  gzip $TMP/key_1st.subimg
  gzip $TMP/key_2nd.subimg
  cp $TMP/key_1st.subimg.gz $EMMC_RECOVERY/key_1st.subimg.gz
  cp $TMP/key_2nd.subimg.gz $EMMC_RECOVERY/key_2nd.subimg.gz
elif [ "a$KEY_SUBIMG" = "aRAW" ] && [ "a$KEY_TYPE" = "aNON-AB" ]; then
  cp $TMP/key_1st.subimg $EMMC_RECOVERY/key_1st.subimg
  cp $TMP/key_2nd.subimg $EMMC_RECOVERY/key_2nd.subimg
elif [ "a$KEY_SUBIMG" = "aGZIPPED" ] && [ "a$KEY_TYPE" = "aAB" ]; then
  gzip $TMP/key.subimg
  cp $TMP/key.subimg.gz $EMMC_RECOVERY/key.subimg.gz
elif [ "a$KEY_SUBIMG" = "aRAW" ] && [ "a$KEY_TYPE" = "aAB" ]; then
  cp $TMP/key.subimg $EMMC_RECOVERY/key.subimg
fi

rm -fr $TMP

echo "Please get the Recovery eMMCimg from $EMMC_RECOVERY folder"
