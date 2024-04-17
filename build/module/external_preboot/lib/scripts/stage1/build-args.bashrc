# Bash script: stage 1: handle arguments

opt_chip_name="${syna_chip_name}"
opt_chip_rev="${syna_chip_rev}"
opt_board_name="${CONFIG_BOARD_NAME}"
opt_profile="${CONFIG_PREBOOT_PROFILE}"
opt_platform="${CONFIG_PREBOOT_PLATFORM}"
opt_production_build="${CONFIG_PRODUCTION_BUILD}"

opt_preboot_tzk_mem_layout="${CONFIG_PREBOOT_TZK_MEM_LAYOUT}"
opt_preboot_tzk_mzone_enable="${CONFIG_PREBOOT_TZK_MZONE_ENABLE}"
opt_preboot_tzk_regfw_enable="${CONFIG_PREBOOT_TZK_REGFW_ENABLE}"
if [ "is${CONFIG_PREBOOT_TEE_ENABLE}" = "isy" ]; then
  opt_preboot_tee_enable="${CONFIG_PREBOOT_TEE_ENABLE}"
else
  opt_preboot_tee_enable=n
fi

if [ "x${CONFIG_EXTERNAL_BOOT}" != "x" ]; then
  opt_external_boot="${CONFIG_EXTERNAL_BOOT}"
fi

if [ "is${CONFIG_LINUX_OS}" = "isy" ]; then
  opt_bootflow="LINUX"
fi

if [ "is${CONFIG_ANDROID_OS}" = "isy" ]; then
  opt_bootflow="VERIFIEDBOOT"
fi

if [ "is${CONFIG_EXT_PREBOOT_BOOTFLOW_AB}" = "isy" ]; then
  opt_bootflow_version="AB"
fi

if [ "is${CONFIG_IMAGE_EMMC}" = "isy" ]; then
  opt_flash_type="EMMC"
fi
if [ "is${CONFIG_IMAGE_NAND}" = "isy" ]; then
  opt_flash_type="NAND"
  opt_boot_part_size="${CONFIG_NAND_BOOT_PART_SIZE}"
  opt_nand_randomizer="${CONFIG_NAND_RANDOMIZER}"
fi

if [ "is${CONFIG_IMAGE_USBBOOT}" = "isy" ]; then
  opt_flash_type="USBBOOT"
fi

if [ "is${CONFIG_FASTBOOT_FLOW}" = "isy" ]; then
  opt_config_fastboot="${CONFIG_FASTBOOT_FLOW}"
else
  opt_config_fastboot=n
fi

opt_market_id="${CONFIG_PREBOOT_MARKET_ID}"

opt_basedir_security_images="${CONFIG_SYNA_SDK_PATH}/${CONFIG_PREBOOT_SECURITY_IMAGE_PATH}"
opt_basedir_security_tools="${opt_basedir_security_tools}"
opt_workdir_security_keys="${opt_basedir_security_keys}"
opt_workfile_security_version_config="${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/release/SECURITY/version/codetype_1/config"
opt_basedir_ddr_phy="${CONFIG_SYNA_SDK_PATH}/boot/external/google_cast/preboot/source/sysinit/hwinit/chip/${syna_chip_name}/${syna_chip_rev}/${CONFIG_PREBOOT_DDR_TYPE}"
if [ ! -d $opt_basedir_ddr_phy ]; then
  opt_basedir_ddr_phy="${CONFIG_SYNA_SDK_PATH}/boot/external/google_cast/preboot/source/sysinit/hwinit/chip/${syna_chip_name}/${CONFIG_PREBOOT_PLATFORM}/${syna_chip_rev}"
fi

opt_ddr_type="${CONFIG_PREBOOT_DDR_TYPE}"
opt_board_memory_speed="${CONFIG_PREBOOT_MEMORY_SPEED}"
opt_board_memory_size="${CONFIG_PREBOOT_MEMORY_SIZE}"
if [ "x{CONFIG_PREBOOT_MEMORY_VARIANT}" != "x" ]; then
  opt_board_memory_variant="${CONFIG_PREBOOT_MEMORY_VARIANT}"
fi

# vim: set ai filetype=sh tabstop=2 softtabstop=2 shiftwidth=2 expandtab:
