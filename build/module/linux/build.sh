#!/bin/bash

source build/header.rc

# move Linux build tool chain path setting here since it is conflict with BSP build tool chain
if [ "is${CONFIG_LINUX_CROSS_COMPILE_PATH}" != "is" ]; then
    PATH_BAK="${PATH}"
    export PATH="${topdir}/toolchain/${CONFIG_LINUX_CROSS_COMPILE_PATH}/bin:${PATH}"
fi

########
# Main #
########
opt_outdir_release=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_LINUX_REL_PATH}
opt_outdir_intermediate=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_LINUX_REL_PATH}/intermediate
opt_workdir_runtime_sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_ROOTFS}
opt_linux_src=${topdir}/${CONFIG_LINUX_SRC_PATH}
opt_basedir_tools_bin=${CONFIG_SYNA_SDK_PATH}/${CONFIG_TOOLS_BIN_PATH}
opt_linux_dts=${CONFIG_LINUX_DTS//,/ }

mkdir -p ${opt_outdir_release}
mkdir -p ${opt_outdir_intermediate}
mkdir -p ${opt_workdir_runtime_sysroot}

### Check settings ###
[ "x${CONFIG_LINUX_ARCH}" != "x" ]
[ "x${CONFIG_LINUX_DEFCONFIG}" != "x" ]
[ -d "${opt_linux_src}" ]

opt_var_defs=""
if [ "is${CONFIG_LINUX_CROSS_COMPILE_LLVM_UTILS}" = "isy" ]; then
    opt_var_defs="${opt_var_defs} LLVM=1"
fi

pushd ${opt_outdir_intermediate}

if [ $clean -eq 1 ]; then
    make -C ${opt_linux_src} O=${opt_outdir_intermediate} clean
    # Clean up
    rm -fr ${opt_outdir_release}/lib/
    rm -fr ${opt_outdir_release}/dtbs/
    rm -fr ${opt_outdir_intermediate}
    rm -fr ${opt_workdir_runtime_sysroot}/lib/modules/
    rm -fr ${opt_workdir_runtime_sysroot}/lib/firmware/mrvl/
    rm -fr ${opt_workdir_runtime_sysroot}/lib/firmware/bcm/

    popd
    exit 0
fi

### clean configs from source tree ###
make -C ${opt_linux_src} mrproper

### Kernel config ###
ARCH=${CONFIG_LINUX_ARCH} CROSS_COMPILE=${CONFIG_LINUX_CROSS_COMPILE} make CC=${CONFIG_LINUX_CROSS_COMPILE_CC} ${opt_var_defs} -C ${opt_linux_src} O=${opt_outdir_intermediate} ${CONFIG_LINUX_DEFCONFIG}

# Check defconfig consistency
diff .config ${opt_linux_src}/arch/${CONFIG_LINUX_ARCH}/configs/${CONFIG_LINUX_DEFCONFIG} || {
  echo "Warning: inconsistent kernel config file \"arch/${CONFIG_LINUX_ARCH}/configs/${CONFIG_LINUX_DEFCONFIG}\""
  sleep 1
}

### Build Kernel and modules ###
ARCH=${CONFIG_LINUX_ARCH} CROSS_COMPILE=${CONFIG_LINUX_CROSS_COMPILE} make CC=${CONFIG_LINUX_CROSS_COMPILE_CC} ${opt_var_defs} -C ${opt_linux_src} O=${opt_outdir_intermediate} -j ${CONFIG_CPU_NUMBER}

# Only build kernel modules when 'CONFIG_MODULES' is enabled
declare config_modules_enabled="n"
( cat .config  | grep "CONFIG_MODULES=y" ) && config_modules_enabled="y"

if [ "is $config_modules_enabled" = "is y" ]; then
  ARCH=${CONFIG_LINUX_ARCH} CROSS_COMPILE=${CONFIG_LINUX_CROSS_COMPILE} make CC=${CONFIG_LINUX_CROSS_COMPILE_CC} ${opt_var_defs} -C ${opt_linux_src} O=${opt_outdir_intermediate} modules -j ${CONFIG_CPU_NUMBER}
fi

### Build DTS ###
echo $opt_linux_dts
if [ "${CONFIG_LINUX_ARCH}" = "arm64" ]; then
dtbo_name=
for d in ${opt_linux_dts}; do
  DTC=${opt_basedir_tools_bin}/dtc DTC_FLAGS="-@" ARCH=${CONFIG_LINUX_ARCH} CROSS_COMPILE=${CONFIG_LINUX_CROSS_COMPILE} make CC=${CONFIG_LINUX_CROSS_COMPILE_CC} ${opt_var_defs} -C ${opt_linux_src} O=${opt_outdir_intermediate} ${d}.dtb
  dtbo_name=${d}
done

pushd ./arch/${CONFIG_LINUX_ARCH}/boot/dts
${opt_basedir_tools_bin}/mkdtimg create ${dtbo_name}.subimg --version=1 ${dtbo_name}.dtb
cp -ad ${dtbo_name}.subimg ${opt_outdir_release}/
popd
else
for d in ${opt_linux_dts}; do
  ARCH=${CONFIG_LINUX_ARCH} CROSS_COMPILE=${CONFIG_LINUX_CROSS_COMPILE} make CC=${CONFIG_LINUX_CROSS_COMPILE_CC} ${opt_var_defs} -C ${opt_linux_src} O=${opt_outdir_intermediate} ${d}.dtb
done
fi
### Install to destination directory ###

# Kernel image
[ -f ./arch/${CONFIG_LINUX_ARCH}/boot/Image ]
[ -f System.map ]
cp -ad ./arch/${CONFIG_LINUX_ARCH}/boot/Image ${opt_outdir_release}/vmlinux
cp -ad ./System.map ${opt_outdir_release}/.
if [ -f ./arch/${CONFIG_LINUX_ARCH}/boot/zImage ]; then
  cp -ad ./arch/${CONFIG_LINUX_ARCH}/boot/zImage ${opt_outdir_release}/zImage
fi

cp -ad ${opt_linux_src}/arch/${CONFIG_LINUX_ARCH}/configs/${CONFIG_LINUX_DEFCONFIG} ${opt_outdir_release}/.

# Modules
if [ "is $config_modules_enabled" = "is y" ]; then
  ARCH=${CONFIG_LINUX_ARCH} CROSS_COMPILE=${CONFIG_LINUX_CROSS_COMPILE} \
    INSTALL_MOD_PATH=${opt_outdir_release} \
    make -C ${opt_linux_src} O=${opt_outdir_intermediate} modules_install
fi

# Firmware
ARCH=${CONFIG_LINUX_ARCH} CROSS_COMPILE=${CONFIG_LINUX_CROSS_COMPILE} \
    INSTALL_MOD_PATH=${opt_outdir_release} \
    make -C ${opt_linux_src} O=${opt_outdir_intermediate} firmware_install || \
(mkdir -p ${opt_outdir_release}/lib/firmware/mrvl; mkdir -p ${opt_outdir_release}/lib/firmware/bcm; mkdir -p ${opt_outdir_release}/lib/firmware/realtek; \
cp -av ${opt_linux_src}/firmware/realtek/* ${opt_outdir_release}/lib/firmware/realtek/;cp -av ${opt_linux_src}/firmware/mrvl/*.bin ${opt_outdir_release}/lib/firmware/mrvl; cp -av ${opt_linux_src}/firmware/bcm/* ${opt_outdir_release}/lib/firmware/bcm)

# DTB files
mkdir -p ${opt_outdir_release}/dtbs

pushd ./arch/${CONFIG_LINUX_ARCH}/boot/dts
for d in ${opt_linux_dts}; do
    cp -fv --parent ${d}.dtb ${opt_outdir_release}/dtbs/
done
popd

### Check results ###
[ -f ${opt_outdir_release}/vmlinux ]
[ -f ${opt_outdir_release}/System.map ]

popd #linux

# restore build tool chain path
if [ "is${CONFIG_LINUX_CROSS_COMPILE_PATH}" != "is" ]; then
    export PATH="${PATH_BAK}"
fi

copy_subdir() {
  src_basedir=$1; shift
  dst_basedir=$1; shift
  subdir=$1; shift

  if [ -d $src_basedir/$subdir ]; then
    mkdir -p $dst_basedir/$subdir/
    cp -ad $src_basedir/$subdir/. $dst_basedir/$subdir/.
  fi
}

### Copy files to runtime system fs ###
mkdir -p ${opt_workdir_runtime_sysroot}/lib
copy_subdir ${opt_outdir_release}/lib ${opt_workdir_runtime_sysroot}/lib modules
copy_subdir ${opt_outdir_release}/lib ${opt_workdir_runtime_sysroot}/lib firmware/mrvl
copy_subdir ${opt_outdir_release}/lib ${opt_workdir_runtime_sysroot}/lib firmware/bcm
copy_subdir ${opt_outdir_release}/lib ${opt_workdir_runtime_sysroot}/lib firmware/realtek

combine_kernel_dtb_by_gih() {
  local f_kimg=$1; shift
  local f_dtb=$1; shift
  local f_out=$1; shift

  # Compress kernel image
  local f_compressed_kimg

  f_compressed_kimg=${f_kimg}.lz4
  cat $f_kimg | lz4 -f -9 -B4 > ${f_compressed_kimg}

  # Pack kernel and dtb together
  local exec2run
  local cmd_args

  exec2run=${CONFIG_SYNA_SDK_OUT_HOST_REL_PATH}/genimg
    [ -x ${exec2run} ]

  cmd_args=""
  cmd_args="${cmd_args} -n boot"
  cmd_args="${cmd_args} -V 0"
  cmd_args="${cmd_args} -v 0"
  cmd_args="${cmd_args} -i LNXB"
  cmd_args="${cmd_args} -d $f_compressed_kimg"
  cmd_args="${cmd_args} -i LDTB"
  cmd_args="${cmd_args} -d $f_dtb"
  cmd_args="${cmd_args} -o $f_out"

  eval $exec2run "$cmd_args"
}

gen_kernel_dtb_combo_by_gih() {
  local dts
  local dtb

  for dts in ${opt_linux_dts}; do
    local f_kernel
    local f_dtb
    local f_lz4imgdtb

    dtb_name=${dts##*/}

    f_kernel=${opt_outdir_release}/vmlinux
    f_dtb=${opt_outdir_release}/dtbs/${dts}.dtb
    f_lz4imgdtb=${opt_outdir_release}/lz4Image-dtb.${dtb_name}

    [ -f $f_kernel ]
    [ -f $f_dtb ]

    combine_kernel_dtb_by_gih $f_kernel $f_dtb $f_lz4imgdtb

    [ -f $f_lz4imgdtb ]
    break
  done
}

# Append $1 which is an integer value to the file at path $2. $1 is appended as
# a binary little endian integer.
append_uint32_le() {
  local val=$1
  local file=$2
  printf "0: %.8x" ${val} | sed -E 's/0: (..)(..)(..)(..)/0: \4\3\2\1/' \
    | xxd -r -g0 >> ${file}
}

# Pack the kernel along with its dtb file
# The format is [header][xz compressed kernel][dtb file]
#
# header is little endian and consists of
# struct {
#   char magic[KDTB_MAGIC_SZ];
#   uint32_t kernel_size;
#   uint32_t dtb_size;
# };
combine_kernel_dtb_by_kdtb() {
  local kernel_file=$1; shift
  local dtb_file=$1; shift
  local packed_kernel=$1; shift

  local compressed_kernel
  local cmd2run
  compressed_kernel=$(dirname $kernel_file)"/"$(basename -s .lz4 ${kernel_file})".xz"
  cmd2run="xz -C crc32 -9 -c ${kernel_file} > ${compressed_kernel}"
  echo $cmd2run
  eval $cmd2run

  local magic="KDTB"
  echo -n ${magic} > ${packed_kernel}
  append_uint32_le $(stat -c %s ${compressed_kernel}) ${packed_kernel}
  append_uint32_le $(stat -c %s ${dtb_file}) ${packed_kernel}

  cat ${compressed_kernel} ${dtb_file} >> ${packed_kernel}
}

gen_kernel_dtb_combo_by_kdtb() {
  local dts
  local dtb

  for dts in ${opt_linux_dts}; do
    local f_kernel
    local f_dtb
    local f_xzimgdtb

    dtb_name=${dts##*/}

    f_kernel=${opt_outdir_release}/vmlinux
    f_dtb=${opt_outdir_release}/dtbs/${dts}.dtb
    f_xzimgdtb=${opt_outdir_release}/xzImage-dtb.${dtb_name}

    [ -f $f_kernel ]
    [ -f $f_dtb ]

    combine_kernel_dtb_by_kdtb $f_kernel $f_dtb $f_xzimgdtb

    [ -f $f_xzimgdtb ]
  done
}

### Generate kernel/dtb combo ###
if [ "is${CONFIG_HEADER_KDTB}" = "isy" ]; then
  gen_kernel_dtb_combo_by_kdtb
else
  gen_kernel_dtb_combo_by_gih
fi
