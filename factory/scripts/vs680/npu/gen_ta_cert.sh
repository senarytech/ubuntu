#!/bin/bash

##==============================================================================
## Goal  : Generate Genx TA CERT image
## Input : Genx_TA_ControllingData.csv
## Output: TA_Name.cert
##
## Usage : gen_ta_cert.sh
##==============================================================================

##==============================================================================
## Tool
##==============================================================================
GENX_TOOL=./bin/genx_img
RSA_PAYLOAD_TOOL=./bin/gen_rsa_payload
##==============================================================================
## Functions - Common
##==============================================================================
function print_usage() {
  printf "\nUsage: $0 <CONFIG_CSV>\n"
  printf "\n       <CONFIG_CSV> : Genx TA Config CSV File\n"
}

function l_endian32() {
  echo `printf "%08x"  $1` | grep -o .. | tac | echo "$(tr -d '\n')" | xxd -r -p
}

function check_file_exist() {
  if [ ! -f $1 ]; then
    printf "\nError: Cannot find the File $1\n"
    exit 1
  fi
}

function read_csv_config() {
  while IFS=',' read -r col1 col2
  do
    if [ "x$col2" != "x" ]; then
      line_var=`echo "$col1=${col2}" | echo "$(tr -d '\r')"`
      export $line_var
    fi
  done < $CSV_CONFIG
}

function remove_double_quotes() {
  VARS=`echo $2 | tr -d '"'`
  export $1=$VARS
}

gen_tacert_payload() {
  local f_cfg
  local f_out

  f_cfg=$1; shift
  f_out=$1; shift

##Check config file
  #echo "f_cfg is $f_cfg"
  [ -f $f_cfg ]
  source $f_cfg

  tacert_perm=$WorkDIR/tacert_perm

  group_cnt=${#TAPERM_GROUP[@]}
  perm_len=$((20 + 20 * group_cnt))

  l_endian32 $TACERT_PermMagicNum >$tacert_perm
  l_endian32 $TACERT_PermVer  >>$tacert_perm
  l_endian32 $perm_len >>$tacert_perm
  l_endian32 $TACERT_PermVendorGID >>$tacert_perm
  l_endian32 ${#TAPERM_GROUP[@]} >>$tacert_perm

  if [ "$group_cnt" -ge 1 ];then
    i=$group_cnt
    while [ "$i" -gt 0 ];
    do
      i=$((i-1))
      echo -n `printf "%s" ${TAPERM_GROUP[$i]}` >>$tacert_perm
      gname_len=${#TAPERM_GROUP[$i]}
      echo `printf "%0$(((16 - gname_len) * 2))x" 0` | xxd -r -p >>$tacert_perm
      l_endian32 $gname_len >> $tacert_perm
    done
  fi

## Get pub key modules(256 bytes)
  in_rsa_pub_payload=$WorkDIR/ta_rsa_pub_modules.bin
  $RSA_PAYLOAD_TOOL -i $TA_RSA_PUB -o $in_rsa_pub_payload

  cat $in_rsa_pub_payload $TA_AES_key $tacert_perm >$f_out
}

gen_tacert_extras(){
  local f_out

  f_out=$1

  #echo "TACERT_ProdImgFlag is $TACERT_ProdImgFlag"
  l_endian32 $TACERT_ProdImgFlag >$f_out
  l_endian32 $TACERT_VendorGID >>$f_out
  l_endian32 $TACERT_CertID >>$f_out
}

gen_secure_image(){
f_in_extras=$1; shift
f_in_payload=$1; shift
f_out=$1; shift

##Check input files##
[ -f $f_in_payload ]
[ -f $f_in_extras ]

## Execytable for generating secure image ##
exec_cmd=$GENX_TOOL
[ -x $exec_cmd ]

## Prepare arguments ##
unset exec_args

## Image Type ##
exec_arg="${exec_arg} -t $TACERT_ImageType"

## Encryption Key ##
exec_arg="${exec_arg} -k $PWD/$TA_CERT_MDK"

## Signing Key ##
exec_arg="${exec_arg} -n $PWD/$TA_CERT_RSA_PRV"

## SegID and SegID Mask ##
exec_arg="${exec_arg} -s $TACERT_SegID"
exec_arg="${exec_arg} -S $TACERT_SegIDMask"

## Ver and Ver Mask ##
exec_arg="${exec_arg} -r $TACERT_Ver"
exec_arg="${exec_arg} -R $TACERT_VerMask"

## Extra part ##
exec_arg="${exec_arg} -x $f_in_extras"

## Requested length. 0: use variable payload lenth. fixed lenth: use fixed lenth ##
exec_arg="${exec_arg} -l 0x0"

## In Payload to be encrytped and signed ##
exec_arg="${exec_arg} -i $f_in_payload"

## ouput file with encyrtion, signature and header info ##
exec_arg="${exec_arg} -o $f_out"

echo "$exec_cmd "$exec_arg""

# Generate secure image ##
eval $exec_cmd "$exec_arg"
}
##==============================================================================
## Check parameters
##==============================================================================
if [ $# != "1" ];then
  print_usage
  exit 1
fi

CSV_CONFIG=$1
check_file_exist $CSV_CONFIG

read_csv_config

##Setup working directory
remove_double_quotes tmp_dir $tmp_dir
WorkDIR="$PWD/$tmp_dir"
printf "\nWorkDIR=$WorkDIR"

if [ ! -d "$WorkDIR" ];then
  mkdir $WorkDIR
fi

##Setup output directory
remove_double_quotes OutputData $OutputData
OutDIR="$PWD/$OutputData"
printf "\nOutDIR=$OutDIR\n"
if [ ! -d $OutDIR ];then
  mkdir $OutDIR
fi

remove_double_quotes TA_CERT_RSA_PRV $TA_CERT_RSA_PRV
remove_double_quotes TA_CERT_MDK $TA_CERT_MDK
remove_double_quotes TA_RSA_PUB $TA_RSA_PUB
remove_double_quotes TA_AES_key $TA_AES_key

#echo "TA_CERT_RSA_PRV is $TA_CERT_RSA_PRV"
#echo "TA_CERT_MDK is $TA_CERT_MDK"
#echo "TA_RSA_PUB is $TA_RSA_PUB"
#echo "TA_AES_key is $TA_AES_key"

remove_double_quotes PERM_CONFIG_FILE $PERM_CONFIG_FILE
#echo "TA PERM_CONFIG_FILE is $PERM_CONFIG_FILE"

check_file_exist $TA_CERT_RSA_PRV
check_file_exist $TA_CERT_MDK
check_file_exist $TA_RSA_PUB
check_file_exist $TA_AES_key
check_file_exist $PERM_CONFIG_FILE

##==============================================================================
## Generate TA Certificate payload
##==============================================================================
in_tacert_payload=$WorkDIR/tacert_payload
gen_tacert_payload $PWD/$PERM_CONFIG_FILE $in_tacert_payload

##==============================================================================
## Generate extra part of TA Certificate header
##==============================================================================
in_tacert_extras=$WorkDIR/tacert_extras
gen_tacert_extras $in_tacert_extras

##==============================================================================
## Generate TA Certificate Image
##==============================================================================
echo -e "\nTACERT_ImageType is $TACERT_ImageType"
remove_double_quotes TA_CERT_NAME $TA_CERT_NAME
out_TACEERT_store=$OutDIR/$TA_CERT_NAME

#$TOOL_CUSTK_STORE -t $TACERT_ImageType -k $PWD/$TA_CERT_MDK -n $PWD/$TA_CERT_RSA_PRV -s $TACERT_SegID -S $TACERT_SegIDMask -r $TACERT_Ver -R $TACERT_VerMask -x $in_tacert_extras -l 0x0 -i $in_tacert_payload -o $out_TACEERT_store

gen_secure_image $in_tacert_extras $in_tacert_payload $out_TACEERT_store
