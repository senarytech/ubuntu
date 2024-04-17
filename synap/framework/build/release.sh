#!/bin/bash

source build/header.rc

source ${CONFIG_SYNA_SDK_PATH}/synap/framework/build/release_mod.rc

release_mod "${CONFIG_SYNA_SDK_PATH}/synap/framework"

echo "Release SyNAP framework Done"
