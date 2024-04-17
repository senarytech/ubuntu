#!/bin/bash -e

# prepare_SDImage.sh <Project root directory>

arg1=$1

if [ "$arg1" == "" ] ; then
	echo "error: can't generate SD.img"
	echo "usage:
	prepare_SDImage.sh <Project root directory>"
	exit
fi

cd ${arg1}

WORK_DIR=`pwd`
cd ${WORK_DIR}

echo "prepare_SDImage worked at ${WORK_DIR}"

PRJ=`grep CONFIG_PRODUCT_NAME build/.config | awk -F \" '{print $2 }'`
echo $PRJ

if [ "$PRJ" == "" ]; then
	echo "can't find the CONFIG_PRODUCT_NAME"
	exit
fi

DST_DIR=${WORK_DIR}/out/${PRJ}/target/release/SDimg/intermediate
RELEASE_DIR=${WORK_DIR}/out/${PRJ}/target/release/SDimg/release

copy_img() {
	local dir
	local prj
	local dts
	dir=$1; shift; [ "x${dir}" != "x" ]
	prj=$1; shift; [ "x${prj}" != "x" ]
	dts=$1; shift; [ "x${dts}" != "x" ]

	mkdir -p ${dts}

	cd ${dts}
	cp -f ${dir}/out/${prj}/target/release/eMMCimg/* .
	rm -f system.subimg*
	find . -name "*.gz" -print | xargs gunzip -f
}

pre_img() {
	local dir
	local prj
	local dts
	dir=$1; shift; [ "x${dir}" != "x" ]
	prj=$1; shift; [ "x${prj}" != "x" ]
	dts=$1; shift; [ "x${dts}" != "x" ]

	cp ${dir}/out/${prj}/target/obj/PACKAGING/subimg_intermediate/system.subimg .
	cp ${dir}/build/scripts/gen_sd .
	./gen_sd
	cd -

	[ -f ${dts}/vs680_SD.img ]

	echo "prepare SDImage success"
}

pre_kernel() {
	local dir
	local prj
	local dts
	dir=$1; shift; [ "x${dir}" != "x" ]
	prj=$1; shift; [ "x${prj}" != "x" ]
	dts=$1; shift; [ "x${dts}" != "x" ]

	mkdir -p ${dts}/kernel
	cp -f ${dir}/out/${prj}/target/linux/vmlinux ${dts}/kernel
	find ${dir}/out/${prj}/target/linux/dtbs/ -name *.dtb -exec cp -r '{}' ${dts}/kernel/kernel.dtb ';'

	cd ${dts}
	dd if=/dev/zero of=boot.subimg bs=1M count=16
	mkfs -t fat boot.subimg
	mkdir -p tmp_mnt
	mount boot.subimg tmp_mnt
	cp ${dts}/kernel/* tmp_mnt/

	cp ${dir}/boot/u-boot_2019_10/arch/arm/mach-synaptics/boot.scr tmp_mnt/
	umount tmp_mnt
	cd -
	cat boot.subimg | gzip -1 > ${dir}/out/${prj}/target/release/eMMCimg/boot.subimg.gz

	echo "prepare kernel success"
}

pre_preboot() {
	local dir
	local prj
	local dts
	dir=$1; shift; [ "x${dir}" != "x" ]
	prj=$1; shift; [ "x${prj}" != "x" ]
	dts=$1; shift; [ "x${dts}" != "x" ]

	cd ${dts}
	echo "preboot.subimg,b1" > emmc_image_list
	echo "preboot.subimg,b2" >> emmc_image_list
	mkdir -p preboot
	cp -f emmc_image_list preboot.subimg emmc_part_list preboot
	cd -
	echo "prepare preboot success"
}

copy_img ${WORK_DIR} ${PRJ} ${DST_DIR}
pre_kernel ${WORK_DIR} ${PRJ} ${DST_DIR}
pre_img ${WORK_DIR} ${PRJ} ${DST_DIR}
pre_preboot ${WORK_DIR} ${PRJ} ${DST_DIR}

mkdir -p ${RELEASE_DIR}/preboot
cp -f ${DST_DIR}/preboot/* ${RELEASE_DIR}/preboot
mv -f ${DST_DIR}/vs680_SD.img ${RELEASE_DIR}

echo "All SUCCESS"
