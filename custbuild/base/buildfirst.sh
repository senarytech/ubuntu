#!/bin/bash

sed -i 's/CONFIG_DRM_COMMON=y/CONFIG_DRM_COMMON=n/g' ../../build/.config
sed -i 's/CONFIG_KEYMASTER=y/CONFIG_KEYMASTER=n/g' ../../build/.config
sed -i 's/CONFIG_AMPSDK=n/CONFIG_AMPSDK=y/g' ../../build/.config
sed -i 's/CONFIG_SYNAP=n/CONFIG_SYNAP=y/g' ../../build/.config
sed -i 's/CONFIG_APP_SMBOX=y/CONFIG_APP_SMBOX=n/g' ../../build/.config
sed -i 's/CONFIG_TEST_STANDBY=y/CONFIG_TEST_STANDBY=n/g' ../../build/.config
sed -i 's/CONFIG_TEST_SPI=y/CONFIG_TEST_SPI=n/g' ../../build/.config
sed -i 's/CONFIG_FFMPEG=n/CONFIG_FFMPEG=y/g' ../../build/.config
sed -i 's/CONFIG_CURL=n/CONFIG_CURL=y/g' ../../build/.config
sed -i 's/CONFIG_ALSA=n/CONFIG_ALSA=y/g' ../../build/.config
sed -i 's/CONFIG_BOOST=n/CONFIG_BOOST=y/g' ../../build/.config
sed -i 's/CONFIG_JSONCPP=n/CONFIG_JSONCPP=y/g' ../../build/.config
sed -i 's/CONFIG_CPPNETLIB=n/CONFIG_CPPNETLIB=y/g' ../../build/.config
sed -i 's/CONFIG_READ_RKEKID=n/CONFIG_READ_RKEKID=y/g' ../../build/.config
sed -i 's/CONFIG_TEE_DEV=n/CONFIG_TEE_DEV=y/g' ../../build/.config
sed -i 's/CONFIG_LIBSYNAV4L2=n/CONFIG_LIBSYNAV4L2=y/g' ../../build/.config

build=`grep -nr 'CONFIG_MINIBUILD' ../../build/.config`
echo "+++++++++++++++++++++++"
echo ${build}as
echo "-----------------------"
if [ ${build}as == "as" ]; then
    echo "CONFIG_MINIBUILD=n" >> ../../build/.config
else
    sed -i 's/CONFIG_MINIBUILD=y/CONFIG_MINIBUILD=n/g' ../../build/.config
fi