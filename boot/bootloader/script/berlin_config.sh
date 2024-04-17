#!/bin/bash

echo "Do configuration:"

#
#	BOX_PRODUCT_RELEASE	: this option indicates whether you want to generated 
#	if BOX_PRODUCT_RELEASE=y : Image-2 loads SM image and SM shut down SoC
#	if BOX_PRODUCT_RELEASE=n : Image-2 loads SM image and continue to load Linux and eCos,
#							   SM will not shut down SoC
#
echo "BOX_PRODUCT_RELEASE = $BOX_PRODUCT_RELEASE"
if [ "$BOX_PRODUCT_RELEASE" = "" ];then
BOX_PRODUCT_RELEASE=y
fi

#
#	DDR_CHANNEL	: this option indicates whether DDR is single channel or dual channel
#	if DDR_CHANNEL=DDR_DUAL_CHANNEL:  	dual channel ddr
#	if DDR_CHANNEL=DDR_SINGLE_CHANNEL:  	single channel ddr
#
echo "DDR_CHANNEL = $DDR_CHANNEL"
if [ "$DDR_CHANNEL" = "" ];then
DDR_CHANNEL=DDR_DUAL_CHANNEL
fi

sed -ie 's/CPU_TYPE=.*/CPU_TYPE='$CPU_TYPE'/' configure.am && \
sed -ie 's/BOX_PRODUCT_RELEASE=[yn]/BOX_PRODUCT_RELEASE='$BOX_PRODUCT_RELEASE'/' configure.am && \
sed -ie 's/DDR_CHANNEL=.*$/DDR_CHANNEL='$DDR_CHANNEL'/' configure.am
sed -ie 's/CLOCK_SETTING=.*$/CLOCK_SETTING='$CLOCK_SETTING'/' configure.am
sed -ie 's/PV_COMP_METHOD_PWM=.*$/PV_COMP_METHOD_PWM='$PV_COMP_METHOD_PWM'/' configure.am
sed -ie 's/PV_COMP_METHOD_PG8x7=.*$/PV_COMP_METHOD_PG8x7='$PV_COMP_METHOD_PG8x7'/' configure.am


[ $? -ne 0 ] && echo "can't change configure.am" && exit 1
exit 0

