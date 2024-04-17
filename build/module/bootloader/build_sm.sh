#!/bin/bash
source build/header.rc
source build/chip.rc

#
# function - generateConfig
# param #1 - quoted "variable list"
# param #2 - output file
#
function generateConfig() {
    local config_str=""
    for item in $1
    do
        config_str="${config_str}${item}=${!item}\n"
    done
    printf "${config_str}" > $2
}

#
# function - build_sm
#
function build_sm() {
    if [ "is${CONFIG_SM_CM3_FW}" != "isy" ]; then
        echo "only SM_CM3 is supported"
        exit 1
    fi
    local varlist=" \
        BERLIN_CHIP \
        CHIP_VER \
        COMM_DIR \
        CONFIG_BL_EXTERNAL_PLATFORM \
        CONFIG_EXTERNAL_PLATFORM \
        CONFIG_FUNCTION_BUTTON_HIGH_LEV_VALID \
        CONFIG_PRODUCTION_BUILD \
        CONFIG_RDK_SYS \
        CONFIG_SM_ACTIVE_STANDBY \
        CONFIG_SM_ADCBTN \
        CONFIG_SM_CEC \
        CONFIG_SM_GIGA_ETHERNET \
        CONFIG_SM_GPIOBTN \
        CONFIG_SM_IRP_NEC \
        CONFIG_SM_IRP_RC5 \
        CONFIG_SM_IR_PROTOCOL \
        CONFIG_SM_IRP_TC9012 \
        CONFIG_SM_IRP_XMP \
        CONFIG_SM_IR_RC_VENDOR \
        CONFIG_SM_LED \
        CONFIG_SM_MENUCONFIG \
        CONFIG_SM_MICMUTE_ADC \
        CONFIG_SM_MICMUTE_ADC_CH \
        CONFIG_SM_MICMUTE_EVENT \
        CONFIG_SM_MICMUTE_HIGH_LEV_VALID \
        CONFIG_SM_OVERHEAT_MONITOR \
        CONFIG_SM_RAM_PARAM_ENABLE \
        CONFIG_SM_RAM_TS_DISABLE \
        CONFIG_SM_RAM_TS_ENABLE \
        CONFIG_SM_TEMPERATURE_MONITOR \
        CONFIG_SM_WAKE_ONLINE \
        CONFIG_SM_WATCHDOG_MONITOR \
        OUTPUT_DIR \
        PLATFORM \
        SM_CROSS_COMPILE \
        SM_PLATFORM_DIR"
    local tc=$(which arm-linux-gnueabihf-gcc)
    if [ "null$tc" != "null" ]; then
        SM_CROSS_COMPILE=${tc::-3}
    else
        SM_CROSS_COMPILE=arm-linux-gnueabihf-
    fi
    CONFIG_FUNCTION_BUTTON_HIGH_LEV_VALID=${CONFIG_BL_HIGH_LEVEL_VALID}

    if [ "is${CONFIG_UBOOT_BOOTUP_SYSTEM}" = "isy" ]; then
        OUTPUT_DIR=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_UBOOT_REL_PATH}/intermediate
    else
        OUTPUT_DIR=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_BL_REL_PATH}/intermediate
    fi

    BERLIN_CHIP=${syna_chip_name}
    CHIP_VER=${syna_chip_rev}
    CONFIG_SM_MENUCONFIG=$CONFIG_BL_SYSTEM_MANAGER
    COMM_DIR=${CONFIG_SYNA_SDK_PATH}/boot/common
    if [ "is${CONFIG_BL_EXTERNAL_PLATFORM}" = "isy" ]; then
        PLATFORM=${CONFIG_SM_CM3_FW_PLATFORM}
        SM_PLATFORM_DIR=${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_SM_EXTERNAL_PLATFORM_PATH}/${CONFIG_BL_EXTERNAL_PLATFORM_NAME}
        BL_PLATFORM_DIR=${CONFIG_EXTERNAL_ROOT_PATH}/${CONFIG_BL_EXTERNAL_PLATFORM_PATH}
        CONFIG_EXTERNAL_PLATFORM=y
    else
        PLATFORM=${CONFIG_SM_CM3_FW_PLATFORM}
    fi

    generateConfig "${varlist}" ${topdir}/boot/bootloader/sm_cm3/syna/.config
    make -C ${topdir}/boot/bootloader/sm_cm3/syna clean
    make -C ${topdir}/boot/bootloader/sm_cm3/syna ci
}

########
# Main #
########
build_sm
