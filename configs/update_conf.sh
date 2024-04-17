#!/bin/sh

update_profile()
{
  folder_path=$1
  profile="$(basename $folder_path)_defconfig"

  printf "\n\n--- Handling ${profile} ---\n"
  rm -f build/.config
  if [[ "${folder_path}" == *"configs/release"* ]]; then
    make RELEASE=y ${profile}
  else
    make ${profile}
  fi
  file="${folder_path}/${folder}_defconfig"
  cp build/.config ${file}
}

update_soc()
{
  folder_path=$1
  folder="$(basename $folder_path)"
  profile="$(basename $folder_path)_defconfig"

  topdir=$(pwd)
  shopt -s expand_aliases
  alias TOPDIR="echo $(pwd)"
  source "${folder_path}/${profile}"
  source "build/chipset.rc"

  for syna_chip_name in ${syna_chip_name_set[*]}
  do
    ./build/scripts/override_config -a update -s y -c "${folder_path}/${folder}_${syna_chip_name}_defconfig" -r "${profile}"
  done
}

for folder in `ls configs/release --hide=common`
do
  if [ "${folder}" != "README" ]; then
    update_profile "configs/release/${folder}"
    update_soc "configs/release/${folder}"
  fi
done

for folder in `ls configs/product --hide=common`
do
  if [ "${folder}" != "README" ]; then
    update_profile "configs/product/${folder}"
  fi
done

