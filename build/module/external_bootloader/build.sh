#!/bin/bash

source build/header.rc
source build/chip.rc
source build/security.rc

build_external_bootloader() {
  if [ "is${CONFIG_IMAGE_EMMC}" = "isy" ]; then
    BOOT_TYPE=emmc_boot
  fi
  if [ "is${CONFIG_IMAGE_NAND}" = "isy" ]; then
    BOOT_TYPE=nand_boot
  fi

  if [ "is${CONFIG_EXT_PREBOOT_BOOTFLOW_AB}" = "isy" ];then
    BOOTFLOW_VER=AB
  else
    BOOTFLOW_VER=NR
  fi

  if [ "is${CONFIG_IMAGE_EMMC}" = "isy" ] || [ "is${CONFIG_IMAGE_NAND}" = "isy" ]; then
    ### Copy prebuilt images ###
    prebuilt_target_dir=${module_topdir}/berlin_tools/sys_init_${syna_chip_name}_${syna_chip_rev,,}
    if [[ ${board_name} == luther-* ]]; then
      prebuilt_target_dir=${prebuilt_target_dir}/bg4cdp_tyson
    elif [[ ${board_name} == valens-proto ]]; then
      prebuilt_target_dir=${prebuilt_target_dir}/bg4cdp_valens
    elif [[ ${board_name} == vento-proto ]]; then
      prebuilt_target_dir=${prebuilt_target_dir}/bg4cdp_valens
    elif [[ ${board_name} == valens-proto-rdk ]]; then
      prebuilt_target_dir=${prebuilt_target_dir}/bg4cdp_valens_rdk
    elif [[ ${board_name} == valens-* ]]; then
      prebuilt_target_dir=${prebuilt_target_dir}/${board_name}
    elif [[ ${board_name} == vento-* ]]; then
      prebuilt_target_dir=${prebuilt_target_dir}/${board_name}
    elif [[ ${board_name} == prince-* ]]; then
      prebuilt_target_dir=${prebuilt_target_dir}/${board_name}
    elif [[ ${board_name} == vs680-* ]] || [[ ${board_name} == luis-* ]]; then
      prebuilt_target_dir=${prebuilt_target_dir}/${board_name}
     elif [[ ${board_name} == as470-* ]] || [[ ${board_name} == bowie-* ]]; then
      prebuilt_target_dir=${prebuilt_target_dir}/${board_name}
    else
      echo "ERROR: unsupported board ${board_name}"
      /bin/false
    fi

    mkdir -p ${prebuilt_target_dir}/image1/release
    mkdir -p ${prebuilt_target_dir}/image2/release

    preboot_build_basedir=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_PREBOOT_REL_PATH}
    prebuilt_source_dir="${preboot_build_basedir}/intermediate/release"
    if [ -d ${prebuilt_source_dir} ]; then
      cp ${prebuilt_source_dir}/erom.bin ${prebuilt_target_dir}/image1/release/bcm_erom.bin
      cp ${prebuilt_source_dir}/tsm.bin ${prebuilt_target_dir}/image1/release/drm_erom.bin

      if [ "is${CONFIG_PREBOOT_DDR_PHY_FW}" = "isy" ]; then
	[ -f ${prebuilt_source_dir}/ddrphy.bin ]
        cp ${prebuilt_source_dir}/ddrphy.bin ${prebuilt_target_dir}/image1/release/ddrphy.bin
      fi

      cp ${prebuilt_source_dir}/sysinit_en.bin ${prebuilt_target_dir}/image2/release/toshiba_sysinit.bin
      cp ${prebuilt_source_dir}/miniloader_en.bin ${prebuilt_target_dir}/image2/release/tz_loader_en.bin
      cp ${prebuilt_source_dir}/miniloader_en.bin ${prebuilt_target_dir}/image2/release/tz_loader_en_tz.bin
    else
      echo "ERROR: preboot release path is not existed: ${prebuilt_source_dir}"
      /bin/false
    fi

    prebuilt_source_dir=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_TEE_KERNEL_REL_PATH}
    if [ -d ${prebuilt_source_dir} ]; then
      if [ "${BOOTFLOW_VER}" = "AB" ]; then
        ### Not sure if A/B mode has the same image at least we can make difference ###
        mkdir -p ${module_topdir}/berlin_tools/SUB_IMAGE/tz_en_a/${board_name}/
        install --mode=0644 ${prebuilt_source_dir}/tee_en.bin ${module_topdir}/berlin_tools/SUB_IMAGE/tz_en_a/${board_name}/tw_en.bin

        mkdir -p ${module_topdir}/berlin_tools/SUB_IMAGE/tz_en_b/${board_name}/
        install --mode=0644 ${prebuilt_source_dir}/tee_en.bin ${module_topdir}/berlin_tools/SUB_IMAGE/tz_en_b/${board_name}/tw_en.bin
      else
        mkdir -p ${module_topdir}/berlin_tools/SUB_IMAGE/tz_en/${board_name}/
        install --mode=0644 ${prebuilt_source_dir}/tee_en.bin ${module_topdir}/berlin_tools/SUB_IMAGE/tz_en/${board_name}/tw_en.bin
      fi
    else
      echo "ERROR: tee release path is not existed: ${prebuilt_source_dir}"
      /bin/false
    fi

    if [ "is${CONFIG_IMAGE_USBBOOT}" = "isy" ]; then
      BOOT_TYPE=usb_boot
    fi
    unset prebuilt_source_dir
    unset prebuilt_target_dir

    ### Build Google Cast BOOTLOADER ###
    # Launch build
    pushd ${module_topdir}
    CROSS_COMPILE64=aarch64-cros-linux-gnu-
    SM_CROSS_COMPILE=arm-none-eabi-
    export SM_CROSS_COMPILE
    export CROSS_COMPILE64

    # Let bootloader use keys from security folder
    local MARKET_ID=`printf 0x%x ${CONFIG_PREBOOT_MARKET_ID}`
    local hex32_market_id=$(printf "%08x" ${syna_chip_mid})
    local workdir_security_keys=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SECURITY_KEY_PATH}/${syna_chip_name}/${syna_chip_rev}
    local workdir_security_keys=${workdir_security_keys}/mid-${hex32_market_id}/codetype_5
    cp -adf ${workdir_security_keys}/custk.bin ./berlin_tools/store/custk_sdk.bin
    cp -adf ${workdir_security_keys}/custk.keystore ./berlin_tools/store/custk_sdk.store
    cp -adf ${workdir_security_keys}/extrsa.pem ./berlin_tools/store/extrsa_sdk_priv.pem
    cp -adf ${workdir_security_keys}/extrsa.keystore ./berlin_tools/store/extrsa_sdk.store
    cp -adf ${workdir_security_keys}/sign_config ./berlin_tools/store/sign_config

    ./build_bootloader.sh ${board_name} ${BOOT_TYPE} ${BOOTFLOW_VER}
    popd
  fi
}

gen2_secure_image() {
  v_image_type=$1; shift
  f_input=$1; shift
  f_output=$1; shift

  ### Check input file ###
  [ -f $f_input ]

  ### Exectuable for generating secure image ###
  if [ "is${CONFIG_RDK_SYS}" != "isy" ]; then
    exec_cmd=${security_tools_path}gen_secure_image
    [ -x $exec_cmd ]
  else
    exec_cmd=gen_secure_image
  fi

  ### Prepare arguments ###
  unset exec_args

  # Codetype
  case "$v_image_type" in
    "bootloader")
      exec_args="${exec_args} --code-type=5"
      ;;
    *) /bin/false ;;
  esac

  local workdir_security_ver=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SECURITY_VERSION_PATH}
  if [ -f ${workdir_security_ver}/codetype_5/config ]; then
      exec_args="${exec_args} --security-config-path=${workdir_security_ver}/codetype_5/config"
  fi

  local MARKET_ID=`printf 0x%x ${CONFIG_PREBOOT_MARKET_ID}`
  # Other parameters
  exec_args="${exec_args} --chip-name=${syna_chip_name}"
  exec_args="${exec_args} --chip-rev=${syna_chip_rev}"
  exec_args="${exec_args} --market-id=${MARKET_ID}"
  exec_args="${exec_args} --workdir-security-tools=${security_tools_path}"
  exec_args="${exec_args} --workdir-security-keys=${security_keys_path}"

  # Input and output
  exec_args="${exec_args} --input-file=${f_input} --output-file=${f_output}"

  # Do not add custk and rsa key stores
  # hardcode for AS390 temperaly
  if [ "x${add_key_store}" != "xy" ]; then
    exec_args="${exec_args} --add-custk-store=0"
    exec_args="${exec_args} --add-ersak-store=0"
  else
    exec_args="${exec_args} --add-custk-store=1"
    exec_args="${exec_args} --add-ersak-store=1"
  fi

  ### Generate secure image ###
  eval ${exec_cmd} "${exec_args}"
}

########
# Main #
########
module_topdir=${topdir}/boot/external/${CONFIG_EXTERNAL_BOOT}/bootloader
opt_outdir_release=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_BL_REL_PATH}
opt_outdir_intermediate=${opt_outdir_release}/intermediate
mkdir -p ${opt_outdir_intermediate}

if [ $clean -eq 1 ]; then
    rm -rf ${module_topdir}/out/ ${opt_outdir_intermediate}/bootloader_raw.bin ${opt_outdir_release}/bootloader_en.bin
    exit 0
fi

board_name="${CONFIG_BOARD_NAME}"
if [ "x${board_name}" = "x" ]; then
  # Set 'board_name' to 'vs680-fpga' if not specified
  board_name="vs680-evk"
fi

[ "x${board_name}" != "x" ]
### Build bootloader ###
build_external_bootloader
if [ -f ${module_topdir}/berlin_tools/bootloader/bootloader.bin ]; then
  mkdir -p ${opt_outdir_intermediate}/output_bootloader
  cp -f ${module_topdir}/berlin_tools/bootloader/bootloader.bin ${opt_outdir_intermediate}/output_bootloader/bootloader.bin
else
  echo "ERROR: bootloader.bin is not generated!!"
  /bin/false
fi

### Check results ###
[ -f ${opt_outdir_intermediate}/output_bootloader/bootloader.bin ]
cp -f ${opt_outdir_intermediate}/output_bootloader/bootloader.bin ${opt_outdir_intermediate}/bootloader_raw.bin
sha256sum ${opt_outdir_intermediate}/bootloader_raw.bin

### Check raw binaries ###
[ -f ${opt_outdir_intermediate}/bootloader_raw.bin ]
ls -l ${opt_outdir_intermediate}/*.bin

#use the exiting encrypted binary in external bootloader
#gen2_secure_image "bootloader" ${opt_outdir_intermediate}/bootloader_raw.bin ${opt_outdir_release}/bootloader_en.bin
[ -f ${module_topdir}/berlin_tools/bootloader/bootloader_en.bin ]
cp ${module_topdir}/berlin_tools/bootloader/bootloader_en.bin ${opt_outdir_release}/bootloader_en.bin

## Copy packed image ###
# bootloader has appended the version table into preboot image
# Need to replace the preboot binary in this stage
outdir_preboot_release=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_PREBOOT_REL_PATH}
[ -f ${module_topdir}/bootloader.build.${board_name}/pre-bootloader.subimg ]
cp -f ${module_topdir}/bootloader.build.${board_name}/pre-bootloader.subimg ${outdir_preboot_release}/preboot_esmt.bin
