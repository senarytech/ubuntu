# Bash script: subimage: preloadta: common

# Output file for preloadta with bootloader
outdir_preload_ta=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/preload_ta
preload_ta_subimg=${outdir_preload_ta}/preload_ta.subimg
if [ "is${CONFIG_UBOOT_BOOTUP_SYSTEM}" = "isy" ]; then
    outfile_bootloader_subimg=${outdir_subimg_intermediate}/fastboot.subimg
    echo "fastboot.subimg act as bootloader.subimg"
else
    outfile_bootloader_subimg=${outdir_subimg_intermediate}/bootloader.subimg
fi

input_ta_path=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_TA_IMAGE_PATH}/${syna_chip_name}/${syna_chip_rev}

