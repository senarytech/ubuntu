#!/bin/bash

##==============================================================================
## Goal  : Generate TA Cert: TA_Name.cert
## Input : ./config/config.cfg
##         ./config/signing.cfg
## Output: <TA_CustKey>     $TA_CUSTK  (./keys/TA_CustKey.bin)
##         <TA_ExtRsaKey>   $TA_EXTRSA (./keys/TA_ExtRsaKey.pem)
##         TA_Name.cert
##
## Usage : gen_ta_cert.sh <TACert_Config>
##==============================================================================
TA_CERT_CONFIG=None
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
gen_ta_cert() {
  local f_in
  local f_out

  f_in=$1; shift
  f_out=$1; shift

  ### Check input file ###
  [ -f $f_in ]

  ## Gen TA_Cert Embedded Header
  f_ta_cert_eheader=$f_out.ehdr
  gen_ta_cert_eheader $f_ta_cert_eheader
  ## Gen TA_Cert Image
  f_ta_cert_img=$WorkDIR/$(basename $f_in .raw)
  cat $f_ta_cert_eheader $f_in > $f_ta_cert_img

  ## Encrypt TA_Cert Image
  gen_secure_image "tacert" $f_ta_cert_img $f_out
}

gen_ta_cert_eheader() {
  local f_out
  f_out=$1; shift

  f_perm=$f_out.perm
  echo "TAROOTPERM_UUID = (${TAROOTPERM_UUID[0]} ${TAROOTPERM_UUID[1]} ${TAROOTPERM_UUID[2]} ${TAROOTPERM_UUID[3]})"
  echo "TAPERM_UUID = $TAPERM_UUID"
  echo "TAPERM_GROUP count = ${#TAPERM_GROUP[@]}"

  ## Mask the LSB 2-bytes
  l_endian32 ${TAROOTPERM_UUID[3]}  > $f_out.uuid3
  l_endian16 $TAPERM_UUID           > $f_out.ta_uuid
  dd if=$f_out.ta_uuid of=$f_out.uuid3 bs=1 conv=notrunc seek=2
  UUID3=`xxd -p $f_out.uuid3`
  echo "UUID3 = $UUID3"
  rm $f_out.uuid3 $f_out.ta_uuid

  if [ "$TAPERM_GROUP_CNT" == "0xffff0000" ]; then
    echo "Inherit TA Permission Group from Root Cert"
    eval TAPERM_GROUP=()
  else
    TAPERM_GROUP_CNT=${#TAPERM_GROUP[@]}
  fi
  echo "TAPERM_GROUP_CNT = $TAPERM_GROUP_CNT"

  group_cnt=${#TAPERM_GROUP[@]}
  perm_size=$((32 + 20 * group_cnt))
  l_endian32 $TAPERM_MAGIC          > $f_perm
  l_endian32 $TAPERM_VERSION       >> $f_perm
  l_endian32 $perm_size            >> $f_perm
  l_endian32 ${TAROOTPERM_UUID[0]} >> $f_perm
  l_endian32 ${TAROOTPERM_UUID[1]} >> $f_perm
  l_endian32 ${TAROOTPERM_UUID[2]} >> $f_perm
  echo -ne $UUID3 | xxd -r -p      >> $f_perm

  l_endian32 ${TAPERM_GROUP_CNT}   >> $f_perm
  if [ "$group_cnt" -ge 1 ]; then
    i=$group_cnt
    while [ "$i" -gt 0 ];
    do
      i=$((i - 1))
      echo -n `printf "%s" ${TAPERM_GROUP[$i]}` >> $f_perm
      gname_len=${#TAPERM_GROUP[$i]}
      echo `printf "%0$(((16 - gname_len) * 2))x" 0` | xxd -r -p >> $f_perm
      l_endian32 $gname_len >> $f_perm
    done
  fi

  header_size=44
  header_size=$((header_size + perm_size))
  stuffing_size=$(((header_size + 15) / 16 * 16 - header_size))
  header_size=$((header_size + stuffing_size))
  l_endian32 $VNDR_TACERT_MagicNum > $f_out
  l_endian32 $header_size >> $f_out
  echo `printf "%02x"  $VNDR_TACERT_VendorID` | xxd -r -p >> $f_out
  vname_size=${#VNDR_TACERT_VendorName}
  echo "VendorName length = $vname_size"
  if [ "$vname_size" -gt 15 ]; then
    echo "Warning: Vendor Name length is greater than 15 characters!!"
    VNDR_TACERT_VendorName=${VNDR_TACERT_VendorName:0:15}
  fi
  echo -n `printf "%s" $VNDR_TACERT_VendorName` >> $f_out
  echo `printf "%0$(((15 - vname_size) * 2))x" 0` | xxd -r -p >> $f_out
  l_endian32 $VNDR_TACERT_SegID     >> $f_out
  l_endian32 $VNDR_TACERT_SegIDMask >> $f_out
  l_endian32 $VNDR_TACERT_Ver       >> $f_out
  l_endian32 $VNDR_TACERT_VerMask   >> $f_out
  cat $f_perm >> $f_out
  #rm $f_perm
  echo `printf "%0$((stuffing_size * 2))x" 0` | xxd -r -p >> $f_out
  l_endian32 $VNDR_TACERT_FooterMagicNum >> $f_out
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
  #   exec_args="${exec_args} -T 0x0"
  #else
     exec_args="${exec_args} -T 0x1"
  #fi

  echo; echo "TA_ROOT_CUSTK = $TA_ROOT_CUSTK"
  echo "TA_ROOT_EXTRSA = $TA_ROOT_EXTRSA"
  ## Codetype
  case "$v_image_type" in
    "tacert")
      exec_args="${exec_args} -g -d 0x2"
      f_custk=$TA_ROOT_CUSTK
      f_extrsa=$TA_ROOT_EXTRSA
      ;;
    *) /bin/false ;;
  esac

  [ -f $f_custk ]
  [ -f $f_extrsa ]

  ## security_level
  exec_args="${exec_args} -l 0x0 "

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

##==============================================================================
## main()
##==============================================================================
## Setup log directory
LogDIR=./log
check_directory_exist $LogDIR
LOG_FILE=$LogDIR/ta_cert_log.txt
exec > $LOG_FILE 2>&1

check_file_exist $TA_ROOT_CUSTK
check_file_exist $TA_ROOT_EXTRSA

check_directory_exist $KeyDIR
check_directory_exist $WorkDIR

##==============================================================================
## TA_CustKey & TA_ExtRsaKey
##==============================================================================
echo "Generate TA_CustKey & TA_ExtRsaKey ..."

dd if=/dev/urandom of=$TA_CUSTK bs=1 count=16
openssl genrsa -out $TA_EXTRSA -f4 2048
openssl rsa -in $TA_EXTRSA -pubout > $TA_EXTRSA_PUBLIC

##==============================================================================
## TA_Vendor[x]_CustKey keystore & TA_Vendor[x]_ExtRsaKey keystore
##==============================================================================
printf "\nConcatenate TA_Vendor[x]_CustKey and TA_Vendor[x]_ExtRsaKey keystore ...\n"

## Gen TA_Vendor[x]_CustKey keystore
$TOOL_CUSTK_STORE -H 0x01 -a 0x7 -t 0x1 -C $TA_CUSTK -R $TA_ROOT_CUSTK -o $KeyDIR/TA_CustKey.keystore
store_size=`stat -c "%s" $KeyDIR/TA_CustKey.keystore`
dd if=/dev/urandom of=$WorkDIR/custk.pad bs=1 count=$((CUST_KEY_SIZE - $store_size))

## Gen TA_Vendor[x]_ExtRsaKey keystore
$TOOL_EXTRSA_STORE -g -H 0x20000000 -S $TA_ROOT_EXTRSA -r 0x1 -l 0x1 -P $TA_EXTRSA_PUBLIC -o $KeyDIR/TA_ExtRsaKey.keystore -m 0x7 -M 0xffffffff -e 0x0 -E 0xff -C $TA_CUSTK -s 0xfffb -f -t 0x01 -a 32
store_size=`stat -c "%s" $KeyDIR/TA_ExtRsaKey.keystore`
dd if=/dev/urandom of=$WorkDIR/extrsa.pad bs=1 count=$((EXTRSA_KEY_SIZE - $store_size))

f_ta_cert_raw=$WorkDIR/TA_Cert.img.raw
cat $KeyDIR/TA_CustKey.keystore $WorkDIR/custk.pad $KeyDIR/TA_ExtRsaKey.keystore $WorkDIR/extrsa.pad > $f_ta_cert_raw
rm $WorkDIR/custk.pad $WorkDIR/extrsa.pad

##==============================================================================
## TA_Name.cert
##==============================================================================
check_directory_exist $VENDOR_Output
gen_ta_cert $f_ta_cert_raw $TA_CERT
printf "\nDone.\n"
