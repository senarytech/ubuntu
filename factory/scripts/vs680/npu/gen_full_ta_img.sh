#!/bin/bash

##==============================================================================
## Goal  : Generate Genx Secure TA image
## Input : Genx_npu_ControllingData.csv
## Output: NPU_Name.bin
##
## Usage : gen_full_npu_img.sh Genx_npu_ControllingData.csv
##==============================================================================

##==============================================================================
## Tool
##==============================================================================
GENX_TOOL=./bin/genx_img
##==============================================================================
## Functions - Common
##==============================================================================
function print_usage() {
  printf "\nUsage: $0 <CONFIG_CSV>\n"
  printf "\n       <CONFIG_CSV> : Genx NPU Config CSV File\n"
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

gen_npu_extras(){
  local f_out

  f_out=$1

 # echo "NPU_ProdImgFlag is $NPU_ProdImgFlag"
  l_endian32 $NPU_ProdImgFlag >$f_out
  l_endian32 $NPU_VendorGID >>$f_out
  l_endian32 $NPU_CertID >>$f_out
  l_endian32 $NPU_BGM_Len >>$f_out
}

gen_npu_payload_extra(){
  local f_extra
  f_extra=$1; shift

  local f_out
  f_out=$1; shift

  l_endian32 $NPU_Input_Mem_Type >$f_extra
  l_endian32 $NPU_Output_Mem_Type >>$f_extra
  
  cat $f_extra $NPU_Graph_MetaData >$f_out
}

gen_secure_image(){
f_in_extras=$1; shift
f_in_payload_extra=$1; shift
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
exec_arg="${exec_arg} -t $NPU_ImageType"

## Encryption Key ##
exec_arg="${exec_arg} -k $PWD/$TA_MDK"

## Signing Key ##
exec_arg="${exec_arg} -n $PWD/$TA_RSA_PRV"

## SegID and SegID Mask ##
exec_arg="${exec_arg} -s $NPU_SegID"
exec_arg="${exec_arg} -S $NPU_SegIDMask"

## Ver and Ver Mask ##
exec_arg="${exec_arg} -r $NPU_Ver"
exec_arg="${exec_arg} -R $NPU_VerMask"

## Extra part ##
exec_arg="${exec_arg} -x $f_in_extras"

## Requested length. 0: use variable payload lenth. fixed lenth: use fixed lenth ##
exec_arg="${exec_arg} -l 0x0"

## In Payload to be encrytped and signed ##
exec_arg="${exec_arg} -i $f_in_payload"

## In Payload extra ##
exec_arg="${exec_arg} -I $f_in_payload_extra"

## ouput file with encyrtion, signature and header info ##
exec_arg="${exec_arg} -o $f_out"

echo "$exec_cmd "$exec_arg""

# Generate secure image ##
eval $exec_cmd "$exec_arg"
}

pack_npu(){
  in_rootcert=$1; shift
  in_tacert=$1; shift
  in_npu=$1; shift
  f_out=$1; shift

  if [ ! -f $in_rootcert ];then
    echo "$in_rootcert doesn't exists, please generate it first"
    exit 1
  else
    echo "$in_rootcert exists"
  fi

  if [ ! -f $in_tacert ];then
    echo "$in_tacert doesn't exists, now generate it"
    $PWD/gen_ta_cert.sh $PWD/Genx_npu_ControllingData.csv
  else
    echo "$in_tacert exists"
  fi

  cat $in_rootcert $in_tacert $in_npu >$f_out

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
else
  rm -rf $WorkDIR/*
fi

##Setup output directory
remove_double_quotes InputData $InputData
InDIR="$PWD/$InputData"
printf "\nInDIR=$InDIR\n"
if [ ! -d $InDIR ];then
  mkdir $InDIR
fi

remove_double_quotes OutputData $OutputData
OutDIR="$PWD/$OutputData"
printf "\nOutDIR=$OutDIR\n"
if [ ! -d $OutDIR ];then
  mkdir $OutDIR
fi

remove_double_quotes TA_RSA_PRV $TA_RSA_PRV
check_file_exist $TA_RSA_PRV

remove_double_quotes TA_MDK $TA_MDK
check_file_exist $TA_MDK

#echo "TA_RSA_PRV is $TA_RSA_PRV"
#echo "TA_MDK is $TA_MDK"

remove_double_quotes PERM_CONFIG_FILE $PERM_CONFIG_FILE
check_file_exist $PERM_CONFIG_FILE


##==============================================================================
## Check TA Root Certificate
##==============================================================================
remove_double_quotes TA_ROOT_CERT_NAME $TA_ROOT_CERT_NAME
in_ROOTCERT_store=$InDIR/$TA_ROOT_CERT_NAME
if [ ! -f $in_ROOTCERT_store ];then
  echo "$in_ROOTCERT_store doesn't exists, please generate it first"
  exit 1
else
  echo "$in_ROOTCERT_store exists"
fi

##==============================================================================
## Generate TA Certificate image
##==============================================================================
$PWD/gen_ta_cert.sh $PWD/Genx_npu_ControllingData.csv

##==============================================================================
## Generate TA Image
##==============================================================================

remove_double_quotes NPU_Input_Mem_Type $NPU_Input_Mem_Type
remove_double_quotes NPU_Output_Mem_Type $NPU_Output_Mem_Type

remove_double_quotes NPU_Graph_MetaData $NPU_Graph_MetaData
check_file_exist $NPU_Graph_MetaData

## Generate NPU extras
in_npu_extras=$WorkDIR/npu_extras
gen_npu_extras $in_npu_extras

#npu_payload_extra_tmp=$WorkDIR/npu_payload_extra_tmp
#in_npu_payload_extra=$WorkDIR/npu_payload_extra
#gen_npu_payload_extra $npu_payload_extra_tmp $in_npu_payload_extra
in_npu_payload_extra=$PWD/$NPU_Graph_MetaData

## Generate NPU Image
echo -e "\nNPU_ImageType is $NPU_ImageType"
remove_double_quotes NPU_RAW_BIN $NPU_RAW_BIN
in_npu_payload=$PWD/$NPU_RAW_BIN

remove_double_quotes NPU_RAW_NAME $NPU_RAW_NAME
out_npu_store=$OutDIR/$NPU_RAW_NAME

gen_secure_image $in_npu_extras $in_npu_payload_extra $in_npu_payload $out_npu_store

##==============================================================================
## Generate NPU_Name.bin
##==============================================================================
remove_double_quotes TA_CERT_NAME $TA_CERT_NAME
remove_double_quotes NPU_IMG_NAME $NPU_IMG_NAME
npu_img_name=$OutDIR/$NPU_IMG_NAME

pack_npu $in_ROOTCERT_store $OutDIR/$TA_CERT_NAME $out_npu_store $npu_img_name

