#!/bin/bash

InputDIR=./input
TACertConfigDIR=./config_tacert

OutTACertDIR=./out_tacert
OutTADIR=./out_ta

function Read_Config() {
    CONTROL_CONFIG=./config/config.cfg
    if [ ! -f $CONTROL_CONFIG ]; then
        echo "$CONTROL_CONFIG not exist!!"
        exit 1
    fi
    source $CONTROL_CONFIG
}

function print_usage() {
    printf "\nUsage: $0 <TA_File>\n"
    printf "\n       <TA_File>       : clear TA file (ELF)\n"
}

echo "Encrypt TA"

Read_Config

if [ $# != "1" ]; then
    print_usage
    exit 1
else
    CLEAR_TA_FILE=$1
    check_file_exist $CLEAR_TA_FILE
    printf "Got TA file: $CLEAR_TA_FILE\n"
fi

renew_directory $OutTADIR
renew_directory $OutTACertDIR

ta_name="$(basename $CLEAR_TA_FILE .clear)"

## Encrypt TACert
./gen_ta_cert.sh
if [ -f $VENDOR_Output/TA_Name.cert ]; then
   cp $VENDOR_Output/TA_Name.cert $OutTACertDIR/${ta_name%%.*}.cert
fi

## Encrypt TA
TA_FILE=$CLEAR_TA_FILE
check_file_exist $TA_FILE
./gen_full_ta_img.sh $TA_FILE
if [ -f $VENDOR_Output/$ta_name ]; then
   cp $VENDOR_Output/$ta_name $OutTADIR
fi

rm -fr $WorkDIR 

echo "Done."
echo "Please get encrypted TA from: $OutTADIR/$ta_name"
