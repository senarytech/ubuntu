# Bash script: build one feature

### Create subdirs ###
mkdir -p ${preboot_outdir_build_release}
mkdir -p ${preboot_outdir_build_intermediate}

gen_secimg_cfg() {
  f_sec_cfg=$1; shift
  v_codetype=$1;

  ### Check security config file ###
  [ -f $f_sec_cfg ]

  ### parse security_ver.cfg ###
  v_IMGVersion=$(grep "SOCSWVersion" $f_sec_cfg | awk {'print $3'})
  if [ ! -d ${preboot_build_basedir}/product/config/security_version ]; then
    mkdir -p ${preboot_build_basedir}/product/config/security_version/secimg/codetype_$v_codetype
  elif [ ! -d ${preboot_build_basedir}/product/config/security_version/secimg/codetype_$v_codetype ]; then
    mkdir ${preboot_build_basedir}/product/config/security_version/secimg/codetype_$v_codetype
  fi

  ### update security version ###
  echo "[security_version]" > ${preboot_build_basedir}/product/config/security_version/secimg/codetype_$v_codetype/config
  echo "extrsak_store = 0" >> ${preboot_build_basedir}/product/config/security_version/secimg/codetype_$v_codetype/config
  echo "image_signing = $v_IMGVersion" >> ${preboot_build_basedir}/product/config/security_version/secimg/codetype_$v_codetype/config
  echo -e "\n" >> ${preboot_build_basedir}/product/config/security_version/secimg/codetype_$v_codetype/config
  echo "[signing_tool]" >> ${preboot_build_basedir}/product/config/security_version/secimg/codetype_$v_codetype/config
  echo "security_version_style = count" >> ${preboot_build_basedir}/product/config/security_version/secimg/codetype_$v_codetype/config
}

gen2_secure_image() {
  v_image_type=$1; shift
  f_input=$1; shift
  f_output=$1; shift

  ### Check input file ###
  [ -f $f_input ]

  ### Exectuable for generating secure image ###
  exec_cmd=${security_tools_path}gen_secure_image
  [ -x $exec_gen_secure_image ]

  ### Prepare arguments ###
  unset exec_args

  if [ -f ${preboot_build_basedir}/product/config/security_ver.cfg ]; then
    gen_secimg_cfg ${preboot_build_basedir}/product/config/security_ver.cfg 1
    # Security config path
    [ -f ${preboot_build_basedir}/product/config/security_version/secimg/codetype_1/config ]
    exec_args="${exec_args} --security-config-path=${preboot_build_basedir}/product/config/security_version/secimg/codetype_1/config"
  fi

  if [ -f $opt_workfile_security_version_config ]; then
    exec_args="${exec_args} --security-config-path=$opt_workfile_security_version_config"
  fi

  # Codetype
  case "$v_image_type" in
    "sysinit")
      exec_args="${exec_args} --code-type=1"
      ;;
    "miniloader")
      exec_args="${exec_args} --code-type=1"
      exec_args="${exec_args} --add-custk-store=0 --add-ersak-store=0"
      ;;
    *) /bin/false ;;
  esac

  # Other parameters
  exec_args="${exec_args} --chip-name=${opt_chip_name}"
  exec_args="${exec_args} --chip-rev=${opt_chip_rev}"
  exec_args="${exec_args} --market-id=${opt_market_id}"

  exec_args="${exec_args} --custk-type=1"
  exec_args="${exec_args} --rsak-type=1"

  if [ "x${security_keys_path}" != "x" ]; then
    exec_args="${exec_args} --workdir-security-keys=${security_keys_path}"
  fi

  if [ "${opt_flash_type}" = "USBBOOT" ]; then
    exec_args="${exec_args} --usb-boot=1"
  fi

  # Input and output
  exec_args="${exec_args} --input-file=${f_input} --output-file=${f_output}"

  ### Generate secure image ###
  eval ${exec_cmd} "${exec_args}"
}

build_preboot() {
  ### Command line arguments ###
  cmd_args=""

  cmd_args="${cmd_args} --chip-name=${opt_chip_name}"
  cmd_args="${cmd_args} --chip-rev=${opt_chip_rev}"
  cmd_args="${cmd_args} --board-name=${opt_board_name}"

  cmd_args="${cmd_args} --profile=${opt_profile}"
  cmd_args="${cmd_args} --platform=${opt_platform}"

  if [ "x${opt_bootflow}" != "x" ]; then
    cmd_args="${cmd_args} --bootflow=${opt_bootflow}"
  fi

  if [ "x${opt_bootflow_version}" != "x" ]; then
    cmd_args="${cmd_args} --bootflow-version=${opt_bootflow_version}"
  fi

  if [ "x${opt_flash_type}" != "x" ]; then
    cmd_args="${cmd_args} --flash-type=${opt_flash_type}"
  else
    /bin/false
  fi

  cmd_args="${cmd_args} --market-id=$(eval printf 0x%x ${opt_market_id})"

  cmd_args="${cmd_args} --outdir-intermediate=${preboot_outdir_build_intermediate}"
  cmd_args="${cmd_args} --outdir-release=${preboot_outdir_build_release}"
  cmd_args="${cmd_args} --basedir-mboot-common=${topdir}/boot/external/${opt_external_boot}/common"
  cmd_args="${cmd_args} --basedir-security-images=${opt_basedir_security_images}"
  cmd_args="${cmd_args} --basedir-security-tools=${security_tools_path}"
  cmd_args="${cmd_args} --workdir-security-keys=${security_keys_path}"

  cmd_args="${cmd_args} --ddr-type=${opt_ddr_type}"
  cmd_args="${cmd_args} --board-memory-speed=${opt_board_memory_speed}"
  cmd_args="${cmd_args} --board-memory-size=${opt_board_memory_size}"
  if [ "x${opt_board_memory_variant}" != "x" ]; then
    cmd_args="${cmd_args} --board-memory-variant=${opt_board_memory_variant}"
  fi
  cmd_args="${cmd_args} --amp-tee-enable=${opt_preboot_tee_enable}"
  cmd_args="${cmd_args} --tzk-mem-layout=${opt_preboot_tzk_mem_layout}"
  cmd_args="${cmd_args} --tzk-mzone-enable=${opt_preboot_tzk_mzone_enable}"
  cmd_args="${cmd_args} --tzk-regfw-enable=${opt_preboot_tzk_regfw_enable}"

  ### Build PREBOOT ###
  pushd ${preboot_topdir}
    ./build ${cmd_args} ${CONFIG_PRODUCT_NAME}
  popd
}

### Build PREBOOT ###
build_preboot

### Clean up output ###
rm -f ${preboot_outdir_build_release}/preboot_esmt.bin

### Check raw binaries ###
[ -f ${preboot_outdir_build_intermediate}/sysinit_raw.bin ]
### Sign SYSINIT and MINILOADER ###
gen2_secure_image "sysinit" ${preboot_outdir_build_intermediate}/sysinit_raw.bin ${preboot_outdir_build_release}/sysinit_en.bin

if [ "${opt_flash_type}" = "USBBOOT" ]; then
  echo "USB boot flow does not need pack EROM, exit"
  exit 0
fi

[ -f ${preboot_outdir_build_intermediate}/miniloader_raw.bin ]
### Sign MINILOADER ###
gen2_secure_image "miniloader" ${preboot_outdir_build_intermediate}/miniloader_raw.bin ${preboot_outdir_build_release}/miniloader_en.bin

### Copy EROM and TSM ###
f_erom=${opt_basedir_security_images}/chip/${opt_chip_name}/${opt_chip_rev}/mid-$(printf %08x ${opt_market_id})/bcm_erom.bin
f_tsm=${opt_basedir_security_images}/chip/${opt_chip_name}/${opt_chip_rev}/mid-$(printf %08x ${opt_market_id})/drm_erom.bin
f_ddrphy=${opt_basedir_ddr_phy}/ddrphy_fw_images.bin

[ -f $f_erom ]
[ -f $f_tsm ]

/bin/cp $f_erom ${preboot_outdir_build_release}/erom.bin
/bin/cp $f_tsm ${preboot_outdir_build_release}/tsm.bin


if [ "is${CONFIG_PREBOOT_DDR_PHY_FW}" = "isy" ]; then
  [ -f $f_ddrphy ]
  /bin/cp $f_ddrphy ${preboot_outdir_build_release}/ddrphy.bin
fi

### Pack EROM, SYSINIT, MINILOADER and TSM together

# Pack binaries
cat ${preboot_outdir_build_release}/erom.bin > ${preboot_outdir_build_release}/preboot_esmt.bin
cat ${preboot_outdir_build_release}/sysinit_en.bin >> ${preboot_outdir_build_release}/preboot_esmt.bin
cat ${preboot_outdir_build_release}/miniloader_en.bin >> ${preboot_outdir_build_release}/preboot_esmt.bin
cat ${preboot_outdir_build_release}/tsm.bin >> ${preboot_outdir_build_release}/preboot_esmt.bin

# Generate information of packed binaries
echo -n "erom," > ${preboot_outdir_build_release}/preboot_esmt.info
stat -c %s ${preboot_outdir_build_release}/erom.bin >> ${preboot_outdir_build_release}/preboot_esmt.info

echo -n "sysinit," >> ${preboot_outdir_build_release}/preboot_esmt.info
stat -c %s ${preboot_outdir_build_release}/sysinit_en.bin >> ${preboot_outdir_build_release}/preboot_esmt.info

echo -n "miniloader," >> ${preboot_outdir_build_release}/preboot_esmt.info
stat -c %s ${preboot_outdir_build_release}/miniloader_en.bin >> ${preboot_outdir_build_release}/preboot_esmt.info

echo -n "tsm," >> ${preboot_outdir_build_release}/preboot_esmt.info
stat -c %s ${preboot_outdir_build_release}/tsm.bin >> ${preboot_outdir_build_release}/preboot_esmt.info

# if nocs (only miniloader is built) or both sysinit and miniloader are built for normal
if [ "is${opt_profile}" = "isnocs" ] || [[ "is${preboot_build_sysinit}" = "isy" && "is${preboot_build_miniloader}" = "isy" ]]; then
  ### Copy PREBOOT ###
  [ -f ${preboot_outdir_build_release}/preboot_esmt.bin ]
  [ -f ${preboot_outdir_build_release}/preboot_esmt.info ]

  cp -ad ${preboot_outdir_build_release}/preboot_esmt.bin ${preboot_outdir_release}/.
  cp -ad ${preboot_outdir_build_release}/preboot_esmt.info  ${preboot_outdir_release}/.
fi
