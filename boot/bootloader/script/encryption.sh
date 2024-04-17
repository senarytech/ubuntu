#!/bin/bash
if [ "$CUST_SELFKEY_IMAGE" = "" ];then
	source ../../../Customization_Data/config
fi
if [ "$CPU_IMAGE_ENCRYPT" = "y" -o "$CPU_IMAGE_ENCRYPT" = "" ];then
	cp -f ../../../Customization_Data/${CUST_SELFKEY_IMAGE} . || exit 1
	cp -f ../../../Customization_Data/${OTP_RSA_KEY_FILE_NAME} . || exit 1
fi
ENC_TOOL_FAKE=./encrypt

if [ "$ROM_KEY_DISABLE" = "1" ] ;then

	if [ "$OTP_RSA_KEY_FILE_NAME" = "" ]; then
		echo "ROM KEY has been disabled, but OTP_RSA_KEY_FILE_NAME is not defined"
		exit 1
	fi

	if [ ! -f ../../../Customization_Data/${OTP_RSA_KEY_FILE_NAME} ];then
		echo " Can not find User RSA key file: ../../Customization_Data/${OTP_RSA_KEY_FILE_NAME} "
		exit 1
	fi

	cp ../../../Customization_Data/${OTP_RSA_KEY_FILE_NAME} ./
fi

if [ -z $ENC_TOOL ]; then
	ENC_TOOL=../enc_tool
fi

if [ "$CONFIG_BOOTFLOW" != "CLEAR" ]; then
if [ "$CONFIG_BCM" = "y" ]; then
CHIP_NAME=`echo $BERLIN_CHIP | tr '[A-Z]' '[a-z]'`
BCM_DIR=../../SecureEngine
ENC_TOOL=$BCM_DIR/encimg
if [ "is$MIDVER" = "is" ]; then
	MIDVER=0x0
fi
MID=$MIDVER
fi
fi
[ ! -f $ENC_TOOL ] && echo "encryption tool $ENC_TOOL does not exist." && exit 1

echo "encryption tool: $ENC_TOOL"

if [ "$ROM_KEY_DISABLE" = "0" ] ;then
	if [ "$CPU_IMAGE_ENCRYPT" = "y" ];then
		echo "ROM key is enabled, CPU_IMAGE_ENCRYPT = y "
		if [ "$CONFIG_BCM" = "y" ]; then
		if [ "$CONFIG_BOOTFLOW" = "CLEAR" ]; then
			cp $1 $2
		else
			$ENC_TOOL -c $CHIP_NAME -m $MID -t BOOTLOADER -i $1 -o $2
		fi
		else
			if [ $CPU_TYPE = "Z1" ];then
				$ENC_TOOL $1 $2 yes rsap 16 96
			else
				#$ENC_TOOL $1 $2 yes rsap 16 80
				$ENC_TOOL $1 $2 yes rsap 128 80
			fi
		fi
	else
		echo "ROM key is enabled, CPU_IMAGE_ENCRYPT = n "
		if [ ! -f $ENC_TOOL_FAKE ];then
			echo "no $ENC_TOOL_FAKE file"
			exit 1
		fi
		$ENC_TOOL_FAKE $1 $2
	fi
else
	echo "ROM key is disabled, use cust key to encypt and sign images"
	$ENC_TOOL $1 $2 yes rsap 128 2
fi
