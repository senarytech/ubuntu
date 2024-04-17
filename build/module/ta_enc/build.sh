#!/bin/bash

source build/header.rc
source build/chip.rc
source build/security.rc
source build/install.rc

########
# Build#
########

[ $clean -eq 1 ] && exit 0

source build/module/ta_enc/ta_list.rc
source build/module/ta_enc/common.rc

### Copy signed TA to rootfs ###
if [ "x$CONFIG_RUNTIME_LINUX_BASELINE_BUILDROOT$CONFIG_RUNTIME_RDK$CONFIG_RUNTIME_OE$CONFIG_RUNTIME_OE64$CONFIG_RUNTIME_UBUNTU" != "x" ]; then
  outdir_ta=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system/opt/syna/ta
else
  outdir_ta=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system/home/galois/ta
fi
mkdir -p ${outdir_ta}

if [ "${CONFIG_PREBOOT_TEE_ENABLE}" = "y" ] && [ "is${CONFIG_MINIBUILD}" != "isy" ]; then
  cp -adv ${enc_ta_path}/*/*.ta ${outdir_ta}/. || { echo "WARNING: failed on copying TEE TA files"; }
fi