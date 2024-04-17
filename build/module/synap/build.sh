#!/bin/bash

source build/header.rc

mod_dir=${CONFIG_SYNA_SDK_PATH}/synap

cur_script=""
function run_script()
{
   if [[ -f "$cur_script" ]] ; then
       ${cur_script} "$@"
   fi
}

script_list=(
   "${mod_dir}/vsi_npu_driver/synap/ta/build/build.sh"
#   "${mod_dir}/vsi_npu_driver/synap/kernel/build.sh"
   "${mod_dir}/vsi_npu_driver/build/build.sh"
   "${mod_dir}/vsi_npu_driver/build/build_host.sh"
   "${mod_dir}/vsi_npu_sw_stack/build/build.sh"
   "${mod_dir}/vsi_npu_sw_stack/build/build_host.sh"
   "${mod_dir}/release/build.sh"
   "${mod_dir}/framework/build/build.sh"
   "${mod_dir}/framework/build/build_host.sh"
)

for s in "${script_list[@]}";do
    cur_script=${s}
    run_script "$@"
done
