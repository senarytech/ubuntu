# Bash script: stage 3 build

### Variables  ###
preboot_outdir_release=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_PREBOOT_REL_PATH}
preboot_outdir_build_release="${preboot_outdir_release}/intermediate/release"
preboot_topdir=${topdir}/boot/external/${CONFIG_EXTERNAL_BOOT}/preboot
bootloader_topdir=${topdir}/boot/external/${CONFIG_EXTERNAL_BOOT}/bootloader
preboot_release_topdir=${bootloader_topdir}/berlin_tools/sys_init_${syna_chip_name}_${syna_chip_rev,,}

case ${CONFIG_BOARD_NAME} in
  luther-*)
    preboot_release_topdir=${preboot_release_topdir}/bg4cdp_tyson
    ;;
  valens-proto | vento-proto)
    preboot_release_topdir=${preboot_release_topdir}/bg4cdp_valens
    ;;
  valens-proto-rdk)
    preboot_release_topdir=${preboot_release_topdir}/bg4cdp_valens_rdk
    ;;
  valens-* | vento-* | prince-* | vs680-*)
    preboot_release_topdir=${preboot_release_topdir}/${CONFIG_BOARD_NAME}
    ;;
  *)
    echo "ERROR: unsupported board ${CONFIG_BOARD_NAME}"
    ;;
esac

if [ ! -d ${preboot_release_topdir} ]; then
  echo "ERROR: PREBOOT prebuilt path is not existed: ${preboot_release_topdir}"
  exit 1
fi

### Copy prebuilt binaries  ###
mkdir -p ${preboot_outdir_release}
mkdir -p ${preboot_outdir_build_release}

# DDRPHY, EROM, TSM, Copy SYSINIT and MINILOADER
f_ddrphy=${preboot_release_topdir}/image1/release/ddrphy.bin
f_erom=${preboot_release_topdir}/image1/release/bcm_erom.bin
f_tsm=${preboot_release_topdir}/image1/release/drm_erom.bin
f_sysinit=${preboot_release_topdir}/image2/release/toshiba_sysinit.bin
f_miniloader=${preboot_release_topdir}/image2/release/tz_loader_en.bin

if [ "is${CONFIG_PREBOOT_DDR_PHY_FW}" = "isy" ]; then
  if [ -f $f_ddrphy ]; then
    INSTALL_F $f_ddrphy ${preboot_outdir_build_release}/ddrphy.bin
  fi
fi
INSTALL_F $f_erom ${preboot_outdir_build_release}/erom.bin
INSTALL_F $f_tsm ${preboot_outdir_build_release}/tsm.bin
INSTALL_F $f_sysinit ${preboot_outdir_build_release}/sysinit_en.bin
INSTALL_F $f_miniloader ${preboot_outdir_build_release}/miniloader_en.bin

### Pack EROM, SYSINIT, MINILOADER and TSM together

# Pack binaries
cat ${preboot_outdir_build_release}/erom.bin > ${preboot_outdir_release}/preboot_esmt.bin
cat ${preboot_outdir_build_release}/sysinit_en.bin >> ${preboot_outdir_release}/preboot_esmt.bin
cat ${preboot_outdir_build_release}/miniloader_en.bin >> ${preboot_outdir_release}/preboot_esmt.bin
cat ${preboot_outdir_build_release}/tsm.bin >> ${preboot_outdir_release}/preboot_esmt.bin

# Generate information of packed binaries
echo -n "erom," > ${preboot_outdir_release}/preboot_esmt.info
stat -c %s ${preboot_outdir_build_release}/erom.bin >> ${preboot_outdir_release}/preboot_esmt.info

echo -n "sysinit," >> ${preboot_outdir_release}/preboot_esmt.info
stat -c %s ${preboot_outdir_build_release}/sysinit_en.bin >> ${preboot_outdir_release}/preboot_esmt.info

echo -n "miniloader," >> ${preboot_outdir_release}/preboot_esmt.info
stat -c %s ${preboot_outdir_build_release}/miniloader_en.bin >> ${preboot_outdir_release}/preboot_esmt.info

echo -n "tsm," >> ${preboot_outdir_release}/preboot_esmt.info
stat -c %s ${preboot_outdir_build_release}/tsm.bin >> ${preboot_outdir_release}/preboot_esmt.info

# vim: set ai filetype=sh tabstop=2 softtabstop=2 shiftwidth=2 expandtab:
