#!/bin/bash

source build/header.rc
source build/install.rc

mod_dir=${CONFIG_SYNA_SDK_PATH}/synap/release
dst_dir=${CONFIG_SYNA_SDK_REL_PATH}/synap

mkdir -p ${dst_dir}
INSTALL_D ${mod_dir} ${dst_dir}
