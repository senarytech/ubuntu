#!/bin/bash

source build/header.rc

source ${CONFIG_SYNA_SDK_PATH}/synap/framework/build/release_mod.rc

release_mod "${CONFIG_SYNA_SDK_PATH}/synap/vsi_npu_driver"

echo "Release SyNAP NPU driver Done"