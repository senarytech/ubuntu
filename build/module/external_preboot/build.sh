#!/bin/bash

# set -x

source build/header.rc
source build/chip.rc
source build/install.rc

rebuild_nocs_preboot=${2}

if [ "x${BASH_VERSION}" != "x" ]; then
  set -o errtrace
fi

########
# Main #
########

### Uncomment follow line to show environment settins ###
# set

### Variables ###
preboot_module_dir=$(readlink -f $(dirname $0))

if [ $clean -eq 1 ]; then
  exit 0
fi

if [ "is${rebuild_nocs_preboot}" = "is" ]; then
	echo "no param for rebuild-nocs-reboot"
else
	echo "#################################NOCS####################################"
	echo "${rebuild_nocs_preboot}"
	opt_rebuild_nocs_preboot=${rebuild_nocs_preboot}
	echo "${opt_rebuild_nocs_preboot}"
fi

script_run_stages=${preboot_module_dir}/lib/scripts/run-stages.bashrc
[ -f ${script_run_stages} ]
source ${script_run_stages}

### Run the first existed stage script  ###
if [ -d ${CONFIG_SYNA_SDK_PATH}/boot/external/${CONFIG_EXTERNAL_BOOT}/preboot ]; then
  try_stages 1
else
  # in the released build, preboot is in binary form
  try_stages 3
fi

# vim: set ai filetype=sh tabstop=2 softtabstop=2 shiftwidth=2 expandtab:
