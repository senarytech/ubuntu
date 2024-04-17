#!/bin/bash

##==============================================================================
## Goal  : Generate TA_Name.ta
## Input : ./config/config.cfg
##         ./config/signing.cfg
## Output: TA_Name.ta
##
## Usage : gen_full_ta_img.sh <TA_File>
##==============================================================================
CONTROL_CONFIG=./config/config.cfg
SIGNING_CONFIG=./config/signing.cfg
if [ ! -f $CONTROL_CONFIG ] || [ ! -f $SIGNING_CONFIG ]; then
    echo "$CONTROL_CONFIG or "
    echo "$SIGNING_CONFIG is missing."
    exit 1
fi
source $CONTROL_CONFIG
source $SIGNING_CONFIG
check_tools_exist

##==============================================================================
## Functions
##==============================================================================
function print_usage() {
    printf "\nUsage: $0 <TA_File>\n"
    printf "\n       <TA_File>       : clear TA file (ELF)\n"
}

pack_ta() {
  local f_in_rootcert
  local f_in_others
  local f_out

  ## Process arguments
  f_in_rootcert=$1; shift
  f_in_others=$1; shift
  f_out=$1; shift

  ## Gen TA Header (32 bytes)
  f_ta_header="$f_out.ta_header"
  l_endian32 $TA_HEADER_MagicNum  > $f_ta_header
  l_endian32 $TA_HEADER_Size     >> $f_ta_header
  ## ta_root_cert_start
  l_endian32 $TA_HEADER_Size     >> $f_ta_header
  ImageSize=`stat -c "%s"  $f_in_rootcert`
  l_endian32 $ImageSize          >> $f_ta_header
  NextImageOffset=$((TA_HEADER_Size + ImageSize))
  l_endian32 $NextImageOffset    >> $f_ta_header
  ImageSize=`stat -c "%s"  $f_in_others`
  l_endian32 $ImageSize          >> $f_ta_header
  echo `printf "%016x" 0` | xxd -r -p >> $f_ta_header

  cat $f_ta_header $f_in_rootcert $f_in_others > $f_out
}

enc_ta() {
  local f_in
  local f_out

  ## Process arguments
  f_in=$1; shift
  f_out=$1; shift

  [ "x${f_in}" != "x" ]
  [ "x${f_out}" != "x" ]

  [ -f $f_in ]

  ## Compress f_in to f_compress
  local f_compress=${f_in}.zz
  ${TOOL_COMPRESS} $f_in $f_compress
  [ -f $f_compress ]

  ## Add common header to f_compress
  local f_compress_whdr=${f_compress}.whdr
  local in_len=`stat -c "%s" $f_in`
  $TOOL_GENIMG -n tzta -i TZTA -v 0x0 -d $f_compress -t attr0 0x01000000 -t attr1 $in_len -o $f_compress_whdr
  [ -f $f_compress_whdr ]
  rm $f_compress

  ## Generate TA Image Embedded Header from cofig file
  local f_ehdr=${f_in}.ehdr
  gen_ta_eheader $f_ehdr
  [ -f $f_ehdr ]

  ## Encrypt and sign TA
  local f_clear_ta=${f_in}.clr
  cat $f_ehdr $f_compress_whdr > $f_clear_ta
  gen_secure_image "ta" $f_clear_ta $f_out
  [ -f $f_out ]
  rm $f_ehdr
  rm $f_compress_whdr
  rm $f_clear_ta
}

gen_ta_eheader() {
  local f_ta_eheader

  ## Process arguments
  f_ta_eheader=$1; shift

  [ "x${f_ta_eheader}" != "x" ]

  l_endian32 $VNDR_TA_MagicNum > $f_ta_eheader
  l_endian32 64 >> $f_ta_eheader
  echo `printf "%02x"  $VNDR_TA_VendorID` | xxd -r -p >> $f_ta_eheader
  vname_size=${#VNDR_TA_VendorName}
  echo "VendorName length = $vname_size"
  if [ "$vname_size" -gt 15 ]; then
    echo "Warning: Vendor Name length is greater than 15 characters!!"
    VNDR_TA_VendorName=${VNDR_TA_VendorName:0:15}
  fi
  echo -n `printf "%s" $VNDR_TA_VendorName` >> $f_ta_eheader
  echo `printf "%0$(((15 - vname_size) * 2))x" 0` | xxd -r -p >> $f_ta_eheader
  l_endian32 $VNDR_TA_SegID     >> $f_ta_eheader
  l_endian32 $VNDR_TA_SegIDMask >> $f_ta_eheader
  l_endian32 $VNDR_TA_Ver       >> $f_ta_eheader
  l_endian32 $VNDR_TA_VerMask   >> $f_ta_eheader
  echo `printf "%040x" 0` | xxd -r -p >> $f_ta_eheader
  l_endian32 $VNDR_TA_FooterMagicNum >> $f_ta_eheader
}

gen_secure_image() {
  v_image_type=$1; shift
  f_input=$1; shift
  f_output=$1; shift

  local f_custk
  local f_extrsa

  ### Check input file ###
  [ -f $f_input ]

  ### Executable for generating secure image ###
  exec_cmd=$TOOL_CIPHER
  [ -x $exec_cmd ]

  ### Prepare arguments ###
  unset exec_args

  ## head_version
  exec_args="${exec_args} -H 0x20000000"

  ## image_hash_size and header_hash_size
  exec_args="${exec_args} -s 32 -z 32"

  ## ext_rsa_type
  #if [ "$MARKET_ID" == "0x1" ]; then
     #exec_args="${exec_args} -T 0x0"
  #else
     exec_args="${exec_args} -T 0x1"
  #fi

  ## Codetype
  case "$v_image_type" in
    "ta")
      exec_args="${exec_args} -g -d 0x2"
      f_custk=$TA_CUSTK
      f_extrsa=$TA_EXTRSA
      ;;
    "ta_cert_image_combo")
      exec_args="${exec_args} -g -d 0x2"
      f_custk=$TA_ROOT_CUSTK
      f_extrsa=$TA_ROOT_EXTRSA
      ;;
    *) /bin/false ;;
  esac

  echo; echo "f_custk = $f_custk"
  echo "f_extrsa = $f_extrsa"
  [ -f $f_custk ]
  [ -f $f_extrsa ]

  ## security_level
  exec_args="${exec_args} -l 0x0 -f "

  ## cust_key_type
  exec_args="${exec_args} -t 0x1"

  ## cust_keyfile
  exec_args="${exec_args} -C $f_custk"

  ## market_id and mask
  exec_args="${exec_args} -m $MARKET_ID -M 0xffffffff"

  ## version and version_mask
  exec_args="${exec_args} -e 0x0 -E 0x0"

  ## extrsa_keyfile
  exec_args="${exec_args} -S $f_extrsa"

  ## image_type
  exec_args="${exec_args} -y 0"

  ## input and output
  exec_args="${exec_args} -i ${f_input} -o ${f_output}"

  echo "${exec_cmd} "${exec_args}""

  ### Generate secure image ###
  eval ${exec_cmd} "${exec_args}"
}

gen_ta_cert_image_combo() {
  local f_ta
  local f_ta_cert
  local f_out

  # Process arguments
  f_ta=$1; shift
  f_ta_cert=$1; shift
  f_out=$1; shift

  ## Gen TA_Combo_Header (32 bytes)
  f_ta_combo_hdr="$WorkDIR/$(basename $f_ta .img).ta_combo_hdr"
  l_endian32 $TA_COMBO_HEADER_MagicNum   > $f_ta_combo_hdr
  l_endian32 $TA_COMBO_HEADER_Size      >> $f_ta_combo_hdr
  ## ta_cert_start
  l_endian32 $TA_COMBO_HEADER_Size      >> $f_ta_combo_hdr
  TacertSize=`stat -c "%s"  $f_ta_cert`
  l_endian32 $TacertSize                >> $f_ta_combo_hdr
  TAImageOffset=$((TA_COMBO_HEADER_Size + TacertSize))
  l_endian32 $TAImageOffset             >> $f_ta_combo_hdr
  TAImageSize=`stat -c "%s"  $f_ta`
  l_endian32 $TAImageSize               >> $f_ta_combo_hdr
  echo `printf "%016x" 0` | xxd -r -p   >> $f_ta_combo_hdr

  cat $f_ta_combo_hdr $f_ta_cert $f_ta > $f_out
}

##==============================================================================
## main()
##==============================================================================
## Setup log directory
LogDIR=./log
check_directory_exist $LogDIR
LOG_FILE=$LogDIR/full_ta_img_log.txt
exec > $LOG_FILE 2>&1

if [ $# != "1" ]; then
    print_usage
    exit 1
else
    CLEAR_TA_FILE=$1
    check_file_exist $CLEAR_TA_FILE
    printf "Got TA file: $CLEAR_TA_FILE\n"
fi

check_file_exist $TA_ROOT_CERT
check_file_exist $TA_CERT

check_file_exist $TA_ROOT_CUSTK
check_file_exist $TA_ROOT_EXTRSA
check_file_exist $TA_CUSTK
check_file_exist $TA_EXTRSA

check_directory_exist $WorkDIR

##==============================================================================
## Encryption TA (TA_Name.img)
##==============================================================================
f_ta=$WorkDIR/$(basename $CLEAR_TA_FILE)
cp $CLEAR_TA_FILE $f_ta
f_raw_ta="$(basename $CLEAR_TA_FILE .clear)"
f_encrypted_ta="$WorkDIR/${f_raw_ta}.img"
enc_ta $f_ta $f_encrypted_ta

##==============================================================================
## TA_Cert_Image_Combo
##==============================================================================
f_ta_cert_image_combo="$WorkDIR/${f_raw_ta}.ta_cert_img_combo"
gen_ta_cert_image_combo $f_encrypted_ta $TA_CERT $f_ta_cert_image_combo

##==============================================================================
## TA_Cert_Image_Combo_Header = Image Header of 'TA_Cert_Image_Combo'
##==============================================================================
f_ta_cert_image_combo_signed="$f_ta_cert_image_combo.signed"
gen_secure_image "ta_cert_image_combo" $f_ta_cert_image_combo $f_ta_cert_image_combo_signed

##==============================================================================
## TA_Name.ta
##==============================================================================
check_directory_exist $VENDOR_Output
f_ta_image="$VENDOR_Output/${f_raw_ta}"
pack_ta $TA_ROOT_CERT $f_ta_cert_image_combo_signed $f_ta_image
printf "\nThe encrypted TA is $f_ta_image\nDone.\n"
