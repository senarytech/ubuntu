#!/bin/bash
source $PWD/config
function print_usage(){
  printf "\nUsage: $0 <input file> \n"
  printf "\nif <input file> is MDK.bin,  then output file is MDK_TA_ROOT_CERT.bin"
  printf "\nif <input file> is TA_K0_AES_Key.bin,  then output file is TA_K0_AES_Key_TA_CERTbin"
  printf "\nif <input file> is TA_K1_AES_Key.bin,  then output file is TA_K1_AES_Key_MODEL.bin \n\n"
}
function write_l_endian32() {
  echo `printf "%08x"  $1` | grep -o .. | tac | echo "$(tr -d '\n')" | xxd -r -p
}

function write_l_endian16() {
  echo `printf "%04x"  $1` | grep -o .. | tac | echo "$(tr -d '\n')" | xxd -r -p
}

function write_byte() {
  echo `printf "%02x"  $1` | xxd -r -p
}

function calc_hmac_sha256() {
  key_in=$1
  image_name=$2
  image_type=$3
  f_out=$4

  f_value="$PWD/value.bin"
  if [ -f $f_value ]; then
    rm $f_value
  fi
  write_l_endian16 $LEVEL > $f_value
  write_byte 0x0D >> $f_value
  write_l_endian32 $image_type >> $f_value
  write_l_endian32 $HEADER_VERSION >> $f_value
  write_byte $DEV_IMAGE_FLAG >> $f_value
  write_l_endian32 $TA_VENDOR_GID >> $f_value
  write_byte 0x00 >> $f_value
  write_l_endian32 $CONTEXT0_3 >> $f_value
  write_l_endian32 $CONTEXT4_7 >> $f_value
  write_l_endian16 $DESIRED_NUM >> $f_value
  if [ "$PRINT_INFO" -eq "1" ]; then
    printf "%16s | %08x | " $image_name $image_type
    xxd -ps $f_value
  fi

  openssl dgst -sha256 -mac HMAC -macopt "hexkey:$key_in" -binary -out $f_out $f_value
  truncate -s 16 $f_out

  if [ -f $f_value ]; then
    rm $f_value
  fi
}

if [ $# != "1" ];then
  print_usage
  exit 1
fi

F_KEY=`basename $1`
echo "F_KEY is $F_KEY"

if [ ! -f $F_KEY ]; then
  echo "Error! can't find $F_KEY"
  exit 1
fi

if [ x$F_KEY == x"MDK.bin" ]; then
  image_type=IMAGE_TYPE_TA_ROOT_CERT
elif [ x$F_KEY == x"TA_K0_AES_Key.bin" ]; then
  image_type=IMAGE_TYPE_TA_CERT
elif [ x$F_KEY == x"TA_K1_AES_Key.bin" ]; then
  image_type=IMAGE_TYPE_MODEL
else
  print_usage
  exit 1
fi

#if [ -d $DIR_OUT ]; then
#  echo "Error! $DIR_OUT exist!"
#  exit 1
#fi
if [ ! -d $DIR_OUT ]; then
  mkdir $DIR_OUT
fi

if [ "$PRINT_INFO" -eq "1" ]; then
  str_mdk=`xxd -p -l 16 $F_KEY`
  echo "*****************************************************************************"
  echo "Input Key: $str_mdk"
  echo "*****************************************************************************"
  echo "KDF values:"
fi

for ((i=0; i<${#Imgs[@]}; i++)); do
  str_image_type="IMAGE_TYPE_"${Imgs[$i]}
  if [ $str_image_type == $image_type ]; then
    calc_hmac_sha256 $str_mdk ${Imgs[$i]} ${!str_image_type} $DIR_OUT/${F_KEY%.*}_${Imgs[$i]}.bin
    break
  fi
done

echo "*****************************************************************************"
echo "Generate KDF keys in $DIR_OUT"
exit 0

