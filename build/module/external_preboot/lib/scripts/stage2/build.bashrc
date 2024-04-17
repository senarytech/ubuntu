# Bash script: stage 2 build
###################
# Global settings #
###################
[ -f "${preboot_module_dir}/lib/scripts/stage2/build-args.bashrc" ]
source "${preboot_module_dir}/lib/scripts/stage2/build-args.bashrc"

#############
# Functions #
#############
get_feature_list() {
  f_list=$1; shift

  cat "${preboot_module_dir}/lib/features/$f_list" \
    | grep -v "^#" \
    | grep -v "^\s*$"
}

install_miniloader() {
  ### Install prebuilt binaries ###
  mkdir -p ${preboot_release_topdir}/image1/release
  mkdir -p ${preboot_release_topdir}/image2/release
  if [ "is${CONFIG_PREBOOT_DDR_PHY_FW}" = "isy" ]; then
    if [ -f ${preboot_outdir_build_release}/ddrphy.bin ]; then
      INSTALL_F ${preboot_outdir_build_release}/ddrphy.bin ${preboot_release_topdir}/image1/release/ddrphy.bin
    fi
  fi
  INSTALL_F ${preboot_outdir_build_release}/erom.bin ${preboot_release_topdir}/image1/release/bcm_erom.bin
  INSTALL_F ${preboot_outdir_build_release}/tsm.bin ${preboot_release_topdir}/image1/release/drm_erom.bin
  INSTALL_F ${preboot_outdir_build_release}/miniloader_en.bin ${preboot_release_topdir}/image2/release/tz_loader_en.bin
  INSTALL_F ${preboot_outdir_build_release}/miniloader_en.bin ${preboot_release_topdir}/image2/release/tz_loader_en_tz.bin
}

build_and_install_hwinit_feature() {
  feature_name=$1; shift

  opt_ddr_type=$1; shift
  opt_board_memory_size=$1; shift
  opt_board_memory_speed=$1; shift
  opt_board_memory_variant=$1; shift

  source "${preboot_module_dir}/lib/scripts/stage2/build-one.bashrc" &
  wait $!

  ### Install prebuilt binaries ###
  mkdir -p ${preboot_release_topdir}/image2/release
  INSTALL_F ${preboot_outdir_build_release}/sysinit_en.bin ${preboot_release_topdir}/image2/release/toshiba_sysinit.bin
}

###################################
# Main: build preboot and install sysinit #
###################################
declare hwinit_features
hwinit_features=$(get_feature_list $syna_chip_name/hwinit.list)

while read -r line
do
  build_and_install_hwinit_feature ${line}
done <<< "${hwinit_features}"

###################################
# Main: Install Miniloader        #
###################################
install_miniloader
# vim: set ai filetype=sh tabstop=2 softtabstop=2 shiftwidth=2 expandtab:
