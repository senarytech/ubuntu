#!/bin/bash

source build/header.rc

source ${CONFIG_SYNA_SDK_PATH}/synap/framework/build/build_mod.rc

build_mod "VSSDK_LINUX_HOST_REL" "${CONFIG_SYNA_SDK_PATH}/synap/framework"
