
#base.sh berlin_config_hw_c02.xml  berlin_config_sw_c02.xml panelcfg_c02.h  vs680_a0_ubuntu20_c02_defconfig

hw_config=$1
sw_config=$2
panelcfg=$3
defconfig=$4

#获取 bash.sh的目录，然后，以这个目录为基准，进行下面的操作。
PRG="$0"
while [ -h "$PRG" ] ; do
  ls=`ls -ld "$PRG"`
  link=`expr "$ls" : '.*-> \(.*\)$'`
  if expr "$link" : '/.*' > /dev/null; then
    PRG="$link"
  else
    PRG=`dirname "$PRG"`/"$link"
  fi
done
PRGDIR=$(cd $(dirname $PRG); pwd)

echo ${PRGDIR}

#标定base 目录
cd ${PRGDIR}

cd ../..
#pwd= custbuild,so change to root dir

pwd

#clean ampsdk.
cd ampsdk
source build/envsetup.sh
make clean


#clean all out .
cd ..
pwd
rm out -fr

#for debug and error log check ,plase open this command
#read nm


#clean rootfs_gui. change rm * to rm rootfs_gui/* ,if change dir error ,will del all file .
cd sysroot/ubuntu20/data/
pwd
rm rootfs_gui/* -fr

pwd
tar zxf  minibuild_xfce_gpu.tar.gz -C rootfs_gui --same-owner

cd -
cd ampsdk/products/linux_vs680_a0
#cp -f berlin_config_sw_evk.xml berlin_config_sw.xml
cp -f ${sw_config}  berlin_config_sw.xml
cp -f ${hw_config}  berlin_config_hw.xml
cd -

cd boot/bootloader/include
cp -f ${panelcfg}  panelcfg.h
cd -

make ${defconfig}
#
cd custbuild/base/
. buildfirst.sh 1>/dev/null

cd -

make -j64 1>/dev/null
