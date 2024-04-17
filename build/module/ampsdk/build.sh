#!/bin/bash
echo "ampsdk build script"
source build/header.rc
source build/install.rc
source build/chip.rc
source build/module/toolchain/${CONFIG_TOOLCHAIN_APPLICATION}.rc
source build/module/toolchain/${CONFIG_TOOLCHAIN_TA}.rc

source build/module/ampsdk/common.rc

install_amp() {

  if [ "$1" != "" ]; then
    local opt_workdir_build_sysroot=$1;
  fi

  if [ "$2" != "" ]; then
    local opt_workdir_runtime_sysroot=$2;
  fi

  # Copy files
  cp -ad ${basedir_amp}/amp/inc/. ${outdir_amp}/include/.
  cp -ad ${basedir_amp}/amp/tools/flick/flick-2.1/runtime/headers/. ${outdir_amp}/include/.

  # Copy results to runtime filesystem
  if [ "is$CONFIG_RUNTIME_LINUX_BASELINE_BUILDROOT" = "isy" ] || [ "is$CONFIG_RUNTIME_UBUNTU" = "isy" ]; then
    local outdir_dst_app=${opt_workdir_runtime_sysroot}/opt/syna
  else
    local outdir_dst_app=${opt_workdir_runtime_sysroot}/home/galois
  fi
  local outdir_dst_root=${opt_workdir_runtime_sysroot}
  local workdir_src=${outdir_amp}

  mkdir -p ${outdir_dst_app}/lib
  mkdir -p ${outdir_dst_app}/bin
  mkdir -p ${outdir_dst_root}/etc
  mkdir -p ${opt_workdir_build_sysroot}/usr/include/amp

  # Install to rootfs
  INSTALL_D ${workdir_src}/bin ${outdir_dst_app}/

  # Install to sysroot
  cp -adv ${workdir_src}/include/* ${opt_workdir_build_sysroot}/usr/include/amp
  if [ ${CONFIG_TOOLCHAIN_APPLICATION} = "aarch64-linux-gnu-" ]; then
    mkdir -p ${opt_workdir_build_sysroot}/usr/lib64
    cp -fr ${workdir_src}/lib/* ${opt_workdir_build_sysroot}/usr/lib64/
    INSTALL_F ${outdir_amp}/intermediate/libsm/libsm.so ${opt_workdir_build_sysroot}/usr/lib64
    mkdir -p ${outdir_dst_app}/lib64
    cp -fr ${workdir_src}/lib/* ${outdir_dst_app}/lib64
  else
    INSTALL_D ${workdir_src}/lib ${opt_workdir_build_sysroot}/usr
    INSTALL_D ${workdir_src}/lib ${outdir_dst_app}/
  fi

  if [ "is${CONFIG_AMP_IP_AUDIO_MS12_ARM_V2_4}" = "isy" ]; then
    local ms12_dap_file=ms12v2_4/dap_complexity_4ch.dat
  fi

  if [ "is${CONFIG_AMP_EXTERNAL_PRODUCT}" = "isy" ]; then
    cp -advL ${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_AMP_XML_EXTERNAL_PATH}/${opt_amp_profile}/berlin_config_sw.xml ${outdir_dst_root}/etc/.
    cp -advL ${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_AMP_XML_EXTERNAL_PATH}/${opt_amp_profile}/berlin_config_hw.xml ${outdir_dst_root}/etc/.
    if [ "is${CONFIG_AMP_IP_AUDIO_MS12_ARM_V2_4}" = "isy" ]; then
        if [ -f "${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_AMP_XML_EXTERNAL_PATH}/${opt_amp_profile}/${ms12_dap_file}" ]; then
            cp -advL ${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_AMP_XML_EXTERNAL_PATH}/${opt_amp_profile}/${ms12_dap_file} ${outdir_dst_root}/etc/.
        fi
    fi

    if [ "is${CONFIG_AMP_MIPI_ENABLE}" = "isy" ]; then
        cp -advL ${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_AMP_XML_EXTERNAL_PATH}/${opt_amp_profile}/${CONFIG_AMP_MIPI_PANEL}.xml ${outdir_dst_root}/etc/.
    fi

    if [ "is${CONFIG_AMP_COMPONENT_ISP_ENABLE}" = "isy" ]; then
      cp -advL ${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_AMP_XML_EXTERNAL_PATH}/${opt_amp_profile}/IMX227.xml ${outdir_dst_root}/etc/.
      cp -advL ${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_AMP_XML_EXTERNAL_PATH}/${opt_amp_profile}/IMX258.xml ${outdir_dst_root}/etc/.
	  cp -advL ${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_AMP_XML_EXTERNAL_PATH}/${opt_amp_profile}/IMX415.xml ${outdir_dst_root}/etc/.
	  cp -advL ${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_AMP_XML_EXTERNAL_PATH}/${opt_amp_profile}/OV5647.xml ${outdir_dst_root}/etc/.
      cp -advL ${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_AMP_XML_EXTERNAL_PATH}/${opt_amp_profile}/OV2775.xml ${outdir_dst_root}/etc/.
      cp -advL ${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_AMP_XML_EXTERNAL_PATH}/${opt_amp_profile}/IMX219.xml ${outdir_dst_root}/etc/.
      cp -advL ${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_AMP_XML_EXTERNAL_PATH}/${opt_amp_profile}/3aconfig.json ${outdir_dst_root}/etc/.
      cp -advL ${basedir_amp}/amp/src/hal/isp/core/lib_rel/VS680A0/libtinyxml/*  ${outdir_dst_root}/opt/syna/lib/.
    fi

    if [ "is${CONFIG_AMP_HDMI_HDCP_2X}" = "isy" ]; then
      if [ "is${CONFIG_BERLIN_VS640_A0}" = "isy" ]; then
        mkdir -p ${outdir_dst_app}/fw
        cp -advL ${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_AMP_FW_EXTERNAL_PATH}/${opt_amp_profile}/firmware/esm_firmware.fw ${outdir_dst_app}/fw/.
      elif [[ ( "is${CONFIG_BERLIN_VS680_Z1}" = "isy" ) || ( "is${CONFIG_BERLIN_VS680_A0}" = "isy" )]]; then
        mkdir -p ${outdir_dst_app}/fw
        cp -advL ${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_AMP_FW_EXTERNAL_PATH}/${opt_amp_profile}/firmware/esm_firmware_standalone.fw ${outdir_dst_app}/fw/.
      fi
    fi

    if [ "is${CONFIG_AMP_COMPONENT_DEWARP_ENABLE}" = "isy" ]; then
        mkdir -p ${outdir_dst_app}/firmware/camera
        cp -advL ${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_AMP_XML_EXTERNAL_PATH}/${opt_amp_profile}/camera/DEWARP_CFG.isp ${outdir_dst_app}/firmware/camera/.
        cp -advL ${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_AMP_XML_EXTERNAL_PATH}/${opt_amp_profile}/camera/DEWARP_LUT.isp ${outdir_dst_app}/firmware/camera/.
    fi
  else
    cp -advL ${basedir_amp}/products/${opt_amp_profile}/berlin_config_sw.xml ${outdir_dst_root}/etc/.
    cp -advL ${basedir_amp}/products/${opt_amp_profile}/berlin_config_hw.xml ${outdir_dst_root}/etc/.
    if [ "is${CONFIG_AMP_IP_AUDIO_MS12_ARM_V2_4}" = "isy" ]; then
        if [ -f "${basedir_amp}/products/${opt_amp_profile}/${ms12_dap_file}" ]; then
            cp -advL ${basedir_amp}/products/${opt_amp_profile}/${ms12_dap_file} ${outdir_dst_root}/etc/.
        fi
    fi

    if [ "is${CONFIG_AMP_MIPI_ENABLE}" = "isy" ]; then
        cp -advL ${basedir_amp}/products/${opt_amp_profile}/mipi/${CONFIG_AMP_MIPI_PANEL}.xml ${outdir_dst_root}/etc/.
    fi

    if [ "is${CONFIG_AMP_COMPONENT_ISP_ENABLE}" = "isy" ]; then
      cp -advL ${basedir_amp}/products/${opt_amp_profile}/IMX227.xml ${outdir_dst_root}/etc/.
      cp -advL ${basedir_amp}/products/${opt_amp_profile}/IMX258.xml ${outdir_dst_root}/etc/.
	  cp -advL ${basedir_amp}/products/${opt_amp_profile}/IMX415.xml ${outdir_dst_root}/etc/.
	  cp -advL ${basedir_amp}/products/${opt_amp_profile}/OV5647.xml ${outdir_dst_root}/etc/.
      cp -advL ${basedir_amp}/products/${opt_amp_profile}/OV2775.xml ${outdir_dst_root}/etc/.
      cp -advL ${basedir_amp}/products/${opt_amp_profile}/IMX219.xml ${outdir_dst_root}/etc/.
      cp -advL ${basedir_amp}/products/${opt_amp_profile}/3aconfig.json ${outdir_dst_root}/etc/
      cp -advL ${basedir_amp}/amp/src/hal/isp/core/lib_rel/VS680A0/libtinyxml/*  ${outdir_dst_root}/opt/syna/lib/
    fi

    if [ -f ${basedir_amp}/products/${opt_amp_profile}/model_config.xml ]; then
      cp -adv --dereference ${basedir_amp}/products/${opt_amp_profile}/model_config.xml ${outdir_dst_root}/etc/.
    fi

    if [ -d ${basedir_amp}/products/${opt_amp_profile}/panel_config ]; then
      cp -adv --dereference ${basedir_amp}/products/${opt_amp_profile}/panel_config ${outdir_dst_root}/etc/.
    fi

    if [ "is${CONFIG_AMP_HDMI_HDCP_2X}" = "isy" ]; then
      if [ "is${CONFIG_BERLIN_VS640_A0}" = "isy" ]; then
        mkdir -p ${outdir_dst_app}/fw
        cp -advL ${basedir_amp}/products/${opt_amp_profile}/firmware/esm_firmware.fw ${outdir_dst_app}/fw/.
      elif [[ ( "is${CONFIG_BERLIN_VS680_Z1}" = "isy" ) || ( "is${CONFIG_BERLIN_VS680_A0}" = "isy" )]]; then
        mkdir -p ${outdir_dst_app}/fw
        cp -advL ${basedir_amp}/products/${opt_amp_profile}/firmware/esm_firmware_standalone.fw ${outdir_dst_app}/fw/.
      fi
    fi

    if [ "is${CONFIG_AMP_COMPONENT_DEWARP_ENABLE}" = "isy" ]; then
        mkdir -p ${outdir_dst_app}/firmware/camera
        cp -advL ${basedir_amp}/products/${opt_amp_profile}/camera/DEWARP_CFG.isp ${outdir_dst_app}/firmware/camera/.
        cp -advL ${basedir_amp}/products/${opt_amp_profile}/camera/DEWARP_LUT.isp ${outdir_dst_app}/firmware/camera/.
    fi

  fi
  echo AMP SUCCESS
}

install_prebuild_amp() {
  cp -advL ${prebuild_build}/* ${opt_workdir_build_sysroot}
  cp -advL ${prebuild_runtime}/*  ${opt_workdir_runtime_sysroot}
}

#############
# Main      #
#############
basedir_amp=${topdir}/ampsdk
opt_workdir_build_sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_SYSYROOT}
opt_workdir_runtime_sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_ROOTFS}
opt_amp_profile=${CONFIG_AMP_PROFILE}
outdir_ta_amp=${CONFIG_SYNA_SDK_PATH}/${CONFIG_TA_REL_PATH}/TA_MODULE_PATH/${syna_chip_name}/${syna_chip_rev}
outdir_fw_amp=${CONFIG_SYNA_SDK_PATH}/${CONFIG_FW_REL_PATH}/FW_MODULE_PATH/${syna_chip_name}/${syna_chip_rev}
outdir_amp=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_AMP_OUTPUT_PATH}
prebuild_build=${topdir}/senary/ampsdk-build
prebuild_runtime=${topdir}/senary/ampsdk-sysroot

if [ "is${CONFIG_PRE_BUILD_AMPSDK}" = "isy" ]; then
  if [ $clean -eq 0 ] && [ "is${CONFIG_PRE_BUILD_AMPSDK}" = "isy" ]; then
    #for 64bit ampsdk only

    # compile amp_client(64bit)
    pushd ${basedir_amp}
    compile_amp_client_dependence_only ${TARGET_OS} ${CONFIG_TOOLCHAIN_APPLICATION} &
    wait $!
    popd

    ### Clean up ###
    pushd ${basedir_amp}
    export GALOIS_TOOLS_PREFIX=${CONFIG_CONFIG_AMP_COMPILER_PREFIX}
    compile_amp_client_dependence_only clean &
    wait $!
    popd
    unset -f compile_amp_client_dependence_only

    # compile amp_server(32bit)
    CONFIG_TOOLCHAIN_APPLICATION=arm-linux-gnueabihf-
    source build/module/toolchain/${CONFIG_TOOLCHAIN_APPLICATION}.rc
    pushd ${basedir_amp}
    compile_amp_server_dependence_only ${TARGET_OS} "arm-linux-gnueabihf-" &
    wait $!
    popd

    ### Clean up ###
    unset -f compile_amp_server_dependence_only

    install_prebuild_amp
  else
    if [ ${CONFIG_TOOLCHAIN_APPLICATION} = "aarch64-linux-gnu-" ]; then
      pushd ${basedir_amp}
      export GALOIS_TOOLS_PREFIX=${CONFIG_CONFIG_AMP_COMPILER_PREFIX}
      compile_amp_client_dependence_only clean &
      wait $!
      compile_amp_server_dependence_only clean &
      wait $!
      popd

      unset -f compile_amp_client_dependence_only
      unset -f compile_amp_server_dependence_only
    fi
  fi
else
  if [ $clean -eq 0 ]; then
    echo "Build and install AMP"

    if [ "is${CONFIG_ANDROID_OS}" = "isy" ]; then
      TARGET_OS=ANDROID
    fi

    if [ "is${CONFIG_LINUX_OS}" = "isy" ]; then
      TARGET_OS=LINUX
    fi

    if [ ${CONFIG_TOOLCHAIN_APPLICATION} = "aarch64-linux-gnu-" ]; then
      #for 64bit ampsdk only
      create_autoconfig ${outdir_amp} ${config_file} n
      pushd ${basedir_amp}
      # create autoconf.h
      compile_amp_client ${TARGET_OS} ${CONFIG_TOOLCHAIN_APPLICATION} &
      wait $!
      install_amp ${prebuild_build} ${prebuild_runtime}
      install_amp &
      wait $!
      popd
      ### Clean up ###
      pushd ${basedir_amp}
      export GALOIS_TOOLS_PREFIX=${CONFIG_CONFIG_AMP_COMPILER_PREFIX}
      compile_amp_client clean &
      wait $!
      popd
      unset -f compile_amp_client
      #unset -f install_amp

      # compile amp_server
      CONFIG_TOOLCHAIN_APPLICATION=arm-linux-gnueabihf-
      source build/module/toolchain/${CONFIG_TOOLCHAIN_APPLICATION}.rc
      create_autoconfig ${outdir_amp} ${config_file} n
      pushd ${basedir_amp}
      # create autoconf.h
      compile_amp_server ${TARGET_OS} "arm-linux-gnueabihf-" &
      wait $!
      install_amp ${prebuild_build} ${prebuild_runtime}
      install_amp &
      wait $!
      popd

      ### Clean up ###
      unset -f create_autoconfig
      unset -f compile_amp_server
      unset -f install_amp
      unset -f create_autoconfig
    else
      echo "CONFIG_TOOLCHAIN_APPLICATION is ${CONFIG_TOOLCHAIN_APPLICATION}"
      error not 64bit
      create_autoconfig ${outdir_amp} ${config_file} n
      pushd ${basedir_amp}
      # create autoconf.h
      compile_amp ${TARGET_OS} ${CONFIG_TOOLCHAIN_APPLICATION} &
      wait $!
      install_amp &
      wait $!
      popd

      ### Clean up ###
      unset -f compile_amp
      unset -f install_amp
      unset -f create_autoconfig
    fi
  else
    if [ ${CONFIG_TOOLCHAIN_APPLICATION} = "aarch64-linux-gnu-" ]; then
      pushd ${basedir_amp}
      export GALOIS_TOOLS_PREFIX=${CONFIG_CONFIG_AMP_COMPILER_PREFIX}
      compile_amp_client clean &
      wait $!
      compile_amp_server clean &
      wait $!
      popd
      unset -f compile_amp_server
    else
      pushd ${basedir_amp}
      export GALOIS_TOOLS_PREFIX=${CONFIG_CONFIG_AMP_COMPILER_PREFIX}
      compile_amp clean &
      wait $!
      popd
      unset -f compile_amp
    fi
  fi
fi