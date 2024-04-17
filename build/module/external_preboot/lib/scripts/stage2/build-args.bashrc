# configure the setting

preboot_topdir=${topdir}/boot/external/${CONFIG_EXTERNAL_BOOT}/preboot
preboot_build_basedir=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_PREBOOT_REL_PATH}
preboot_outdir_release=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_PREBOOT_REL_PATH}

preboot_outdir_build_release="${preboot_build_basedir}/intermediate/release"
preboot_outdir_build_intermediate="${preboot_build_basedir}/intermediate/obj"

opt_board_name="${CONFIG_BOARD_NAME}"
opt_chip_name=$syna_chip_name
opt_chip_rev=$syna_chip_rev
opt_market_id=$syna_chip_mid
opt_flash_type=$syna_flash_type
opt_preboot_tzk_mem_layout=$syna_tzk_memlayout
opt_bootflow="LINUX"
opt_profile="normal"

opt_platform="${CONFIG_PREBOOT_PLATFORM}";
if [ "is${CONFIG_EXT_PREBOOT_BOOTFLOW_AB}" = "isy" ]; then
  opt_bootflow_version="AB"
fi

if [ "x${CONFIG_EXTERNAL_BOOT}" != "x" ]; then
  opt_external_boot="${CONFIG_EXTERNAL_BOOT}"
fi

opt_preboot_tzk_mzone_enable="${CONFIG_PREBOOT_TZK_MZONE_ENABLE}"
opt_preboot_tzk_regfw_enable="${CONFIG_PREBOOT_TZK_REGFW_ENABLE}"
if [ "is${CONFIG_PREBOOT_TEE_ENABLE}" = "isy" ]; then
  opt_preboot_tee_enable="${CONFIG_PREBOOT_TEE_ENABLE}"
else
  opt_preboot_tee_enable=n
fi

preboot_feature_variant="${opt_chip_name}/${opt_chip_rev}/${opt_market_id}"
preboot_feature_variant="${preboot_feature_variant}/${feature_name}/${opt_bootflow_version}/${opt_flash_type}"

preboot_build_sysinit=y
preboot_build_miniloader=y

opt_basedir_security_images="${CONFIG_SYNA_SDK_PATH}/${CONFIG_PREBOOT_SECURITY_IMAGE_PATH}"
opt_basedir_security_tools="${opt_basedir_security_tools}"
opt_workdir_security_keys="${opt_basedir_security_keys}"
opt_workfile_security_version_config="${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/release/SECURITY/version/codetype_1/config"
opt_basedir_ddr_phy="${CONFIG_SYNA_SDK_PATH}/boot/external/google_cast/preboot/source/sysinit/hwinit/chip/${syna_chip_name}/${syna_chip_rev}/${CONFIG_PREBOOT_DDR_TYPE}"
if [ ! -d $opt_basedir_ddr_phy ]; then
 opt_basedir_ddr_phy="${CONFIG_SYNA_SDK_PATH}/boot/external/google_cast/preboot/source/sysinit/hwinit/chip/${syna_chip_name}/${CONFIG_PREBOOT_PLATFORM}/${syna_chip_rev}"
fi
