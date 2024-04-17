#
#
# Makefile
#
# GNU makefile for BootLoader object
##

-include $(CONFIG_PATH)

ifeq ($(strip $(SDK_CROSS_COMPILE)),)
CROSS_COMPILE=arm-marvell-eabi-
SDK_CROSS_COMPILE=arm-marvell-eabi-
endif

#
# Source tree roots
#
#
PROJ_DIR = .
COMMON_DIR = ../common
COMMON_INC = $(COMMON_DIR)/inc
OUT_DIR = $(OUTPUT_DIR)/output_bootloader
OBJ_DIR = $(OUT_DIR)/obj/bootloader
CHIP_DIR = $(COMMON_DIR)/chip/$(CHIP_NAME)
VPP_DIR = source/vpp_g4/source/

CFLAGS = -Wall -Werror -Os -nostdlib -fno-builtin -ffunction-sections -fdata-sections -I$(PROJ_DIR) -I$(COMMON_INC) -I../../Common/include -I$(COMMON_INC)/register -DBOOTLOADER -DDEBUG -DCONFIG_DCACHE
SFLAGS = -Os -nostdlib -fno-builtin -I$(PROJ_DIR) -I$(COMMON_INC)/register -DBOOTLOADER -DCONFIG_DCACHE
HOST_CFLAGS = -Werror -Wall -Os

ifeq ($(CHIP_VER), Z1)
CFLAGS += -DCHIP_VER_Z1
else ifeq ($(CHIP_VER), A0)
CFLAGS += -DCHIP_VER_A0
endif

#use debug option if needed
#CFLAGS += -g -gdwarf-2
#SFLAGS += -g -gdwarf-2
#HOST_CFLAGS += -g -gdwarf-2

include $(COMMON_DIR)/chipversions
CHIPVERSIONS = -DBERLIN $(CHIP_VER_DEF)

CFLAGS += $(CHIPVERSIONS) -DCONFIG_PLATFORM=\"$(PLATFORM)\"
SFLAGS += $(CHIPVERSIONS)
HOST_CFLAGS += $(CHIPVERSIONS)

LDFLAGS		= -T script/bootloader.lds -Map $(OUT_DIR)/bootloader.map -Bstatic -Os --gc-sections

CHIP_PATH = $(PROJ_DIR)/customization/$(BERLIN_CHIP)
CHIP_PATH_LOWCASE = $(shell echo $(CHIP_PATH) | tr '[A-Z]' '[a-z]')

CHIP_MKFILE = $(CHIP_PATH_LOWCASE)/chip.mk

BOOTFLOW_VER ?= NR
VERIFIEDBOOT_VER ?= V20

include $(CHIP_MKFILE)
include $(COMMON_DIR)/chip/$(shell echo $(BERLIN_CHIP) | tr '[A-Z]' '[a-z]')/chip.mk

CFLAGS += -DBERLIN_C_SERIES -I$(INCLUDE_DIR) -I../../Common/randomizer/ -DCPU0_START_ADDR=$(CPU0_START_ADDR)
SFLAGS += -DBERLIN_C_SERIES -I$(INCLUDE_DIR)
HOST_CFLAGS += -I$(INCLUDE_DIR)

sSRCS += \
	$(PROJ_DIR)/source/init_64.S \
	$(PROJ_DIR)/source/uish_64.S \
	$(PROJ_DIR)/$(COMMON_DIR)/driver/cpu/armv8/cache/cache_helpers_a64.S \
	$(PROJ_DIR)/$(COMMON_DIR)/driver/cpu/armv8/cache/initMultiLevelMMU_64.S \
	$(COMMON_DIR)/driver/cpu/armv8/irq/exception.S \
	$(PROJ_DIR)/source/code_data.S
cSRCS += \
	$(PROJ_DIR)/$(COMMON_DIR)/driver/cpu/armv8/cache/cache_drv.c \
	$(COMMON_DIR)/driver/cpu/armv8/armv8_fun.c
CFLAGS += -DARMV8_64BIT -I$(PROJ_DIR)/$(COMMON_DIR)/driver/cpu/armv8/cache
SFLAGS += -DARMV8_64BIT -I$(PROJ_DIR)/$(COMMON_DIR)/driver/cpu/armv8/cache
CFLAGS += -I$(COMMON_DIR)/driver/cpu/armv8/irq/
SFLAGS += -I$(COMMON_DIR)/driver/cpu/armv8/irq/

cSRCS += \
	$(PROJ_DIR)/source/processes/main_process.c \
	$(PROJ_DIR)/source/bl_mem.c

ifeq ($(CONFIG_GENX_ENABLE),y)
CFLAGS += -DCONFIG_GENX_ENABLE
SFLAGS += -DCONFIG_GENX_ENABLE
endif

ifeq ($(BOOTFLOW_VER), AB)
CFLAGS += -DCONFIG_AB_BOOTFLOW
cSRCS += \
	$(PROJ_DIR)/source/bootflow/bootflow_ab.c

  ifeq ($(CONFIG_BL_SYSTEM_AS_ROOT), y)
    CFLAGS += -DCONFIG_SYSTEM_AS_ROOT
  endif
else ifeq ($(BOOTFLOW_VER), NR)
cSRCS += \
	$(PROJ_DIR)/source/bootflow/bootflow_new.c

  ifeq ($(CONFIG_BL_SYSTEM_AS_ROOT), y)
    CFLAGS += -DCONFIG_SYSTEM_AS_ROOT
  endif
  ifeq ($(CONFIG_BL_RECOVERY_DTBO), y)
    CFLAGS += -DCONFIG_ENABLE_DTBO
  endif
  CFLAGS += -DCONFIG_NR_BOOTFLOW
endif

ifeq ($(CONFIG_BOOTFLOW), LINUX)
CFLAGS += -DLINUXSDK_BOOTLOADER
cSRCS += \
	 $(PROJ_DIR)/source/imageloading/linux.c
endif

ifeq ($(CONFIG_BOOTFLOW), VERIFIEDBOOT)
CFLAGS += -DCONFIG_VERIFYBOOT
CFLAGS += -DCONFIG_ENABLE_DTBO
#FIXME: this flag should be removed
CFLAGS += -DCONFIG_ANDROID_BOOTFLOW
ifeq ($(VERIFIEDBOOT_VER), V20)
CFLAGS += -DCONFIG_VB20
ifeq ($(AVB_1P2_LAUNCHED), y)
CFLAGS += -DCONFIG_AVB_1P2_LAUNCHED
endif
cSRCS += \
	 $(PROJ_DIR)/source/imageloading/android_20.c \
	 $(PROJ_DIR)/source/imageloading/android_utils.c
endif

ifeq ($(VERIFIEDBOOT_VER), V20)
CFLAGS += -I$(PROJ_DIR)/source/avb/libavb/ -DAVB_COMPILATION
cSRCS += \
	$(PROJ_DIR)/source/avb/libavb/avb_chain_partition_descriptor.c \
	$(PROJ_DIR)/source/avb/libavb/avb_crc32.c \
	$(PROJ_DIR)/source/avb/libavb/avb_crypto.c \
	$(PROJ_DIR)/source/avb/libavb/avb_descriptor.c \
	$(PROJ_DIR)/source/avb/libavb/avb_footer.c \
	$(PROJ_DIR)/source/avb/libavb/avb_hash_descriptor.c \
	$(PROJ_DIR)/source/avb/libavb/avb_hashtree_descriptor.c \
	$(PROJ_DIR)/source/avb/libavb/avb_kernel_cmdline_descriptor.c \
	$(PROJ_DIR)/source/avb/libavb/avb_property_descriptor.c \
	$(PROJ_DIR)/source/avb/libavb/avb_rsa.c \
	$(PROJ_DIR)/source/avb/libavb/avb_sha256.c \
	$(PROJ_DIR)/source/avb/libavb/avb_sha512.c \
	$(PROJ_DIR)/source/avb/libavb/avb_slot_verify.c \
	$(PROJ_DIR)/source/avb/libavb/avb_sysdeps_posix.c \
	$(PROJ_DIR)/source/avb/libavb/avb_util.c \
	$(PROJ_DIR)/source/avb/libavb/avb_vbmeta_image.c \
	$(PROJ_DIR)/source/avb/libavb/avb_version.c \
	$(PROJ_DIR)/source/avb/libavb/avb_cmdline.c
endif

cSRCS += $(PROJ_DIR)/source/avb/vb_state.c
cSRCS += $(PROJ_DIR)/source/libxbc/libxbc.c

#for verified boot GUI
cSRCS += \
	$(PROJ_DIR)/source/processes/graphics.c \
	$(PROJ_DIR)/source/vb_ui.c
endif

ifeq ($(CONFIG_PRODUCTION_BUILD), y)
CFLAGS += -DCONFIG_PRODUCTION_BUILD
endif

ifeq ($(CLEAR_IMAGE), y)
CFLAGS += -DCLEAR_BOOTFLOW
#CONFIG_FPGA=y
#CONFIG_SSTZ=y
endif

ifeq ($(CONFIG_APB_UART_DISABLE), y)
CFLAGS += -DCONFIG_DISABLE_UART
endif

ifeq ($(CONFIG_SM), y)
CFLAGS += -DCONFIG_SM
SFLAGS += -DCONFIG_SM
  ifeq ($(CONFIG_SM_CM3_FW), y)
    CFLAGS += -DCONFIG_SM_CM3_FW_ENABLE
  endif
cSRCS += \
	$(PROJ_DIR)/$(COMMON_DIR)/driver/sm/sm_comm.c

  ifeq ($(CONFIG_SM_CM3_FW), y)
  cSRCS += \
	  $(PROJ_DIR)/source/processes/sm_cm3_process.c \
	  $(PROJ_DIR)/source/sm_cm3_driver.c
  else
  cSRCS += \
	  $(PROJ_DIR)/source/processes/sm_v5te_process.c \
	  $(PROJ_DIR)/source/sm_v5te_driver.c
  endif
  ifeq ($(CONFIG_SM_RAM_PARAM_ENABLE), y)
  CFLAGS += -DSOC_RAM_PARAM_ENABLE
  endif
  ifeq ($(CONFIG_SM_RAM_TS_ENABLE), y)
   CFLAGS += -DSOC_RAM_TS_ENABLE
  endif

endif

#lz4
###ifeq ($(CONFIG_LZ4_BOOT), y)
cSRCS += \
	$(PROJ_DIR)/$(COMMON_DIR)/lib/lz4/lib/lz4.c \
	$(PROJ_DIR)/$(COMMON_DIR)/lib/lz4/lib/lz4hc.c \
	$(PROJ_DIR)/$(COMMON_DIR)/lib/lz4/lib/lz4frame.c \
	$(PROJ_DIR)/$(COMMON_DIR)/lib/lz4/lib/xxhash.c \
	$(PROJ_DIR)/$(COMMON_DIR)/lib/lz4/programs/lz4cli.c
CFLAGS += -I$(PROJ_DIR)/$(COMMON_DIR)/lib/lz4/lib/

CFLAGS += -DCONFIG_LZ4_BOOT
SFLAGS += -DCONFIG_LZ4_BOOT
###endif

ifeq ($(CONFIG_FLASH_TYPE), EMMC)
cSRCS += $(PROJ_DIR)/$(COMMON_DIR)/driver/flash/load_gpt.c
cSRCS += $(PROJ_DIR)/$(COMMON_DIR)/driver/flash_ts/emmc_drv.c

CFLAGS += -DCONFIG_GPT -I$(PROJ_DIR)/$(COMMON_DIR)/driver/flash/
SFLAGS += -DCONFIG_GPT

CFLAGS += -DCONFIG_EMMC_V5
cSRCS += \
	$(COMMON_DIR)/driver/emmc/emmc.c \
	$(COMMON_DIR)/driver/emmc/rpmb.c \

emmc_dir = $(COMMON_DIR)/driver/emmc/
cSRCS += $(emmc_dir)/snps/dwcmshc_mmc.c $(emmc_dir)/snps/mmc.c $(CHIP_DIR)/init_emmc.c
CFLAGS += -I$(emmc_dir)/snps
CFLAGS += -I$(emmc_dir)

cSRCS += \
	$(PROJ_DIR)/source/rpmb.c
CFLAGS += -DEMMC_BOOT
SFLAGS += -DEMMC_BOOT

ifeq ($(CONFIG_BOOT_SPI_THEN_SD), y)
CFLAGS += -DCONFIG_BOOT_SPI_THEN_SD
endif

ifeq ($(CONFIG_GEN_SKIP), y)
CFLAGS += -DCONFIG_GEN_SKIP
endif

endif

cSRCS += \
	 $(CHIP_DIR)/init_clock.c

ifeq ($(CONFIG_FUNCTION_BUTTON_ENABLE), y)
CFLAGS += -DCONFIG_FUNCTION_BUTTON_ENABLE

ifeq ($(CONFIG_FUNCTION_BUTTON_SKIPPER), y)
CFLAGS += -DCONFIG_FUNCTION_BUTTON_SKIPPER
endif

ifeq ($(CONFIG_FUNCTION_BUTTON_ADC), y)
CONFIG_ADC=y
CFLAGS += -DCONFIG_FUNCTION_BUTTON_ADC
CFLAGS += -DCONFIG_FUNCTION_BUTTON_ADC_CH=$(CONFIG_FUNCTION_BUTTON_ADC_CH)
else
CFLAGS += -DCONFIG_FUNCTION_BUTTON_SM_GPIO_PORT=$(CONFIG_FUNCTION_BUTTON_SM_GPIO_PORT)
endif

ifeq ($(CONFIG_FUNCTION_BUTTON_HIGH_LEV_VALID), y)
CFLAGS += -DCONFIG_FUNCTION_BUTTON_HIGH_LEV_VALID
endif

ifeq ($(CONFIG_FUNCTION_BUTTON_FASTBOOT), y)
CFLAGS += -DCONFIG_FUNCTION_BUTTON_FASTBOOT
SFLAGS += -DCONFIG_FUNCTION_BUTTON_FASTBOOT
endif
endif

ifeq ($(CONFIG_ADC), y)
CFLAGS += -Isource/adc
#adc is used by functionbutton and GUI of verifiedboot
ifeq ($(CONFIG_DWC_ADC12B5MSAR), y)
cSRCS += $(PROJ_DIR)/source/adc/dwc_adc12b5msar.c
else
cSRCS += $(PROJ_DIR)/source/adc/mv_auxadc10.c
endif
endif

CFLAGS += -Iinclude/ -Iinclude/auto -I$(COMMON_INC) -I$(CHIP_DIR)
SFLAGS += -Iinclude/ -Iinclude/auto -I$(COMMON_INC)
HOST_CFLAGS += -Iinclude/

CFLAGS += -Isource/clock/include/
SFLAGS += -Isource/clock/include/
HOST_CFLAGS += -Isource/clock/include/

ifeq ($(CONFIG_FPGA), y)
CFLAGS += -DCONFIG_FPGA
SFLAGS += -DCONFIG_FPGA
endif

#FIXME: not used
ifeq ($(CONFIG_SSTZ), y)
CFLAGS += -DCONFIG_SSTZ
SFLAGS += -DCONFIG_SSTZ
endif

cSRCS += \
	$(PROJ_DIR)/$(COMMON_DIR)/driver/timer/generic_timer.c \
	$(PROJ_DIR)/$(COMMON_DIR)/driver/watchdog/apb_watchdog.c \
	$(PROJ_DIR)/$(COMMON_DIR)/driver/uart/apb_uart_driver.c \
	$(PROJ_DIR)/$(COMMON_DIR)/driver/uart/printf.c \
	$(PROJ_DIR)/$(COMMON_DIR)/driver/uart/vsnprintf.c \
	$(PROJ_DIR)/$(COMMON_DIR)/driver/uart/debug.c \
	$(PROJ_DIR)/$(COMMON_DIR)/driver/misc/state.c \
	$(PROJ_DIR)/$(COMMON_DIR)/lib/string.c \
	$(PROJ_DIR)/$(COMMON_DIR)/lib/ctype.c \
	$(PROJ_DIR)/$(COMMON_DIR)/lib/crc32.c \
	$(PROJ_DIR)/$(COMMON_DIR)/lib/sha2.c \
	$(PROJ_DIR)/$(COMMON_DIR)/lib/io.c \
	$(PROJ_DIR)/$(COMMON_DIR)/lib/mmgr.c \
	$(PROJ_DIR)/source/function_button.c \
	$(PROJ_DIR)/$(COMMON_DIR)/driver/flash/flash_adaptor.c \
	$(PROJ_DIR)/$(COMMON_DIR)/driver/flash/load_gih.c \
	$(PROJ_DIR)/$(COMMON_DIR)/driver/flash/load_fixed_images.c \
	$(PROJ_DIR)/$(COMMON_DIR)/driver/misc/boot_mode.c \
	$(PROJ_DIR)/$(COMMON_DIR)/driver/misc/boot_devinfo.c \
	$(PROJ_DIR)/$(COMMON_DIR)/driver/i2c/diag_i2c.c \
	$(PROJ_DIR)/source/ptinfo.c

ifeq ($(BERLIN_CHIP), $(filter $(BERLIN_CHIP), vs640 vs680))
cSRCS += $(PROJ_DIR)/$(COMMON_DIR)/driver/i2c/i2c_fxl6408.c
endif

ifeq ($(BERLIN_CHIP), vs640)
cSRCS += $(PROJ_DIR)/$(COMMON_DIR)/driver/i2c/i2c_ti_tca9538.c
endif

CFLAGS += -I$(PROJ_DIR)/$(COMMON_DIR)/driver/flash/
CFLAGS += -I$(PROJ_DIR)/$(COMMON_DIR)/driver/flash_ts/
CFLAGS += -I$(PROJ_DIR)/$(COMMON_DIR)/driver/ram_ts/
CFLAGS += -I$(PROJ_DIR)/$(COMMON_DIR)/driver/misc/
CFLAGS += -I$(PROJ_DIR)/$(COMMON_DIR)/driver/uart/
CFLAGS += -I$(PROJ_DIR)/source/android/
CFLAGS += -I$(PROJ_DIR)/source/libxbc/

ifeq ($(CONFIG_TRUSTZONE), y)
CFLAGS += -DCONFIG_TRUSTZONE
SFLAGS += -DCONFIG_TRUSTZONE

ifeq ($(CONFIG_TRUSTZONE_RPMB_FEATURE), y)
CFLAGS += -DCONFIG_TRUSTZONE_RPMB_FEATURE
SFLAGS += -DCONFIG_TRUSTZONE_RPMB_FEATURE
endif
ifeq ($(PRELOADTA_ENABLE), y)
CFLAGS += -DCONFIG_PRELOADTA
ifeq ($(TA_KEYMASTER_V4), y)
CFLAGS += -DTA_KEYMASTER_V4
endif
cSRCS += $(PROJ_DIR)/source/preload_ta.c
cSRCS += $(PROJ_DIR)/source/processes/gencrypt_process.c
endif

endif


ifeq ($(CONFIG_DTB), y)
CFLAGS += -DCONFIG_DTB
SFLAGS += -DCONFIG_DTB

cSRCS += $(PROJ_DIR)/source/kernel_param.c

cSRCS += $(COMMON_DIR)/driver/fdt/fdt.c
cSRCS += $(COMMON_DIR)/driver/fdt/fdt_addresses.c
cSRCS += $(COMMON_DIR)/driver/fdt/fdt_empty_tree.c
cSRCS += $(COMMON_DIR)/driver/fdt/fdt_syna.c
cSRCS += $(COMMON_DIR)/driver/fdt/fdt_ro.c
cSRCS += $(COMMON_DIR)/driver/fdt/fdt_rw.c
cSRCS += $(COMMON_DIR)/driver/fdt/fdt_strerror.c
cSRCS += $(COMMON_DIR)/driver/fdt/fdt_sw.c
cSRCS += $(COMMON_DIR)/driver/fdt/fdt_wip.c
CFLAGS += -I$(COMMON_DIR)/driver/fdt

cSRCS += $(COMMON_DIR)/lib/libufdt/ufdt_convert.c
cSRCS += $(COMMON_DIR)/lib/libufdt/ufdt_node.c
cSRCS += $(COMMON_DIR)/lib/libufdt/ufdt_node_pool.c
cSRCS += $(COMMON_DIR)/lib/libufdt/ufdt_overlay.c
cSRCS += $(COMMON_DIR)/lib/libufdt/ufdt_prop_dict.c
cSRCS += $(COMMON_DIR)/lib/libufdt/sysdeps/libufdt_sysdeps_vendor.c
cSRCS += $(COMMON_DIR)/lib/libufdt/ufdt_impl.c
cSRCS += $(PROJ_DIR)/source/dtbo.c
CFLAGS += -I$(COMMON_DIR)/lib/libufdt
CFLAGS += -I$(COMMON_DIR)/lib/libufdt/include
CFLAGS += -I$(COMMON_DIR)/lib/libufdt/sysdeps/include

cSRCS += $(COMMON_DIR)/lib/libufdt/fdtdump.c
endif

CFLAGS += -DBCM_EN -DCONFIG_BCM
SFLAGS += -DBCM_EN -DCONFIG_BCM
cSRCS += $(COMMON_DIR)/driver/bcm/bcm_verify.c $(COMMON_DIR)/driver/bcm/bcm_hal.c

ifeq ($(CONFIG_APB_TIMER_DISABLE), y)
CFLAGS += -DAPB_TIMER_DISABLE
SFLAGS += -DAPB_TIMER_DISABLE
endif

cSRCS += \
        $(PROJ_DIR)/source/processes/functionbutton_process.c \
        $(PROJ_DIR)/source/bootloader.c \
        $(PROJ_DIR)/source/rt_stage.c \
        $(PROJ_DIR)/source/bl_boot_arg.c \
        $(PROJ_DIR)/source/sha/sha1.c \
        $(PROJ_DIR)/source/dummy.c

cSRCS += \
        $(PROJ_DIR)/$(COMMON_DIR)/driver/flash_ts/flash_ts.c \
        $(PROJ_DIR)/$(COMMON_DIR)/driver/ram_ts/ram_ts.c


ifeq ($(CONFIG_FASTLOGO), y)
CFLAGS += -DCONFIG_FASTLOGO
SFLAGS += -DCONFIG_FASTLOGO

ifeq ($(BL_FASTLOGO_RESID), )
CFLAGS += -DPRIMARY_DISPLAY_RES=0
SFLAGS += -DPRIMARY_DISPLAY_RES=0
else
CFLAGS += -DPRIMARY_DISPLAY_RES=$(BL_FASTLOGO_RESID)
SFLAGS += -DPRIMARY_DISPLAY_RES=$(BL_FASTLOGO_RESID)
endif

ifeq ($(BL_DISPLAY_MODE), 0)
CFLAGS += -DBL_DISPLAY_HDMI_ONLY
else ifeq ($(BL_DISPLAY_MODE), 1)
CFLAGS += -DBL_DISPLAY_MIPI_ONLY
else ifeq ($(BL_DISPLAY_MODE), 2)
CFLAGS += -DBL_DISPLAY_DUAL_PIP
else
$(error BL_DISPLAY_MODE $(BL_DISPLAY_MODE) is not supported)
endif

ifneq (,$(filter $(BL_DISPLAY_MODE),1 2))
CFLAGS += -DCONFIG_PANEL_FASTLOGO
endif

ifneq ($(BL_SECONDARY_RESID), )
CFLAGS += -DSECONDARY_DISPLAY_RES=$(BL_SECONDARY_RESID)
endif

cSRCS +=$(PROJ_DIR)/source/processes/display_process_g4.c\
	$(PROJ_DIR)/source/processes/mv_display.c



  ifeq ($(PRELOADTA_ENABLE), y)
  CONFIG_VPP_USE_FASTLOGO_TA=y
  else
  CONFIG_VPP_USE_FASTLOGO_TA=n
  endif

ifeq ($(CONFIG_VPP_USE_FASTLOGO_TA), y)
VPP_DIR = source/thinvpp_ca/
CFLAGS += -DCONFIG_VPP_USE_FASTLOGO_TA
CFLAGS += -DCONFIG_VPP_AUTOPUSH_ENABLED
#CFLAGS += -DCONFIG_VPP_FASTLOGO_SHOW_VBUF_INFO
#CFLAGS += -DCONFIG_VPP_ENABLE_PASSPHY
#CFLAGS += -DCONFIG_VPP_ENABLE_PASSPAR
CFLAGS += -DCONFIG_MV_AMP_TEE_ENABLE
CFLAGS += -DVPP_ENABLE_TZ_MALLOC_MEM_ALLOCATION
#CFLAGS += -DVPP_ENABLE_SECURE_MEM_ALLOCATION
CFLAGS += -DTZ_3_0
CFLAGS += -DVPP_ENABLE_INTERNAL_MEM_MGR
#CFLAGS += -DVPP_ENABLE_FLUSH_CACHE
CFLAGS += -DVPP_SUS_RES -DPE_3D_FMT_CONVERT -DPE_3D_BD
CFLAGS += -DBERLIN -DVPP_IN_TRUST_ZONE
CFLAGS += -DHDMI_HW_EDDC_SUPPORT

CFLAGS += -Isource/thinvpp_ca/ca/include
CFLAGS += -Isource/thinvpp_ca/include
CFLAGS += -Isource/thinvpp_ca/common/include
CFLAGS += -Icommon/driver/tz
CFLAGS += -Isource/thinvpp_ca/osal/include

CFLAGS += -I$(CHIP_DIR)/avpll/include
CFLAGS += -I$(CHIP_DIR)/
CFLAGS += -I$(PROJ_DIR)/source/dhub/include
cSRCS += $(wildcard $(VPP_DIR)/*.c)
cSRCS += $(wildcard $(VPP_DIR)/ca/source/*.c)
cSRCS += $(wildcard $(VPP_DIR)/osal/source/*.c)

cSRCS += $(wildcard $(CHIP_DIR)/avpll/source/avpll.c)
#cSRCS += $(wildcard $(CHIP_DIR)/avpll/source/src/diag_avpll.c)
#cSRCS += $(wildcard $(CHIP_DIR)/avpll/source/src/avpll_clock_cfg.c)
cSRCS += $(wildcard $(CHIP_DIR)/avpll/source/vpp_cfg.c)
cSRCS += $(wildcard $(CHIP_DIR)/diag_pll.c)
cSRCS += $(wildcard $(CHIP_DIR)/diag_clock.c)
endif
endif

ifeq ($(CONFIG_EXTERNAL_PLATFORM), y)
  PLATFORM_CUSTOMIZATION_DIR = $(PLATFORM_DIR)/$(PLATFORM)
  include $(PLATFORM_DIR)/$(PLATFORM)/platform.mk
else
  PLATFORM_CUSTOMIZATION_DIR = $(CHIP_PATH_LOWCASE)/$(PLATFORM)
  include $(CHIP_PATH_LOWCASE)/$(PLATFORM)/platform.mk
endif

ifneq ($(PV_COMP), )
CFLAGS += -DPV_COMP -I$(COMMON_DIR)/driver/pv_comp/
cSRCS += $(COMMON_DIR)/driver/pv_comp/pv_compensation.c \
	$(COMMON_DIR)/driver/pv_comp/pmic_select.c

ifeq ($(PV_COMP), I2C_NEW)
CFLAGS += -DI2C -DI2C_NEWDRIVER
cSRCS += \
	$(COMMON_DIR)/driver/i2c/i2c_pvd_m88pg86x.c \
	$(COMMON_DIR)/driver/i2c/i2c_pvd_sy8827n.c \
	$(COMMON_DIR)/driver/i2c/i2c_pvd_syr837_8.c \
	$(COMMON_DIR)/driver/i2c/i2c_pvd_sy8824b.c \
	$(COMMON_DIR)/driver/i2c/i2c_pvd_ncp6335d.c \
	$(COMMON_DIR)/driver/i2c/i2c_pvd_mp886x.c \
	$(COMMON_DIR)/driver/i2c/i2c_pvd_hl7593.c \
	$(COMMON_DIR)/driver/i2c/i2c_pvd_tps62868.c
endif

ifneq ($(DEFAULT_CORE_VOLTAGE), )
CFLAGS += -DDEFAULT_CORE_VOLTAGE=$(DEFAULT_CORE_VOLTAGE)
endif

endif

CFLAGS += -DCPUPLL=$(CPUPLL)
SFLAGS += -DCPUPLL=$(CPUPLL)

ifeq ($(CONFIG_EMMC_WRITE_PROTECT), y)
CFLAGS += -DCONFIG_EMMC_WRITE_PROTECT
SFLAGS += -DCONFIG_EMMC_WRITE_PROTECT
cSRCS += $(COMMON_DIR)/driver/flash/flash_wp.c
endif

ifeq ($(DISABLE_CRASH_COUNTER), y)
CFLAGS += -DDISABLE_CRASH_COUNTER
SFLAGS += -DDISABLE_CRASH_COUNTER
endif

ifeq ($(CONFIG_MISC), y)
CFLAGS += -DCONFIG_MISC
SFLAGS += -DCONFIG_MISC
endif

ifeq ($(CONFIG_TRUSTZONE), y)
cSRCS += $(COMMON_DIR)/driver/tz/tz_nw_boot.c
cSRCS += $(COMMON_DIR)/driver/tz/tee_client_api.c
cSRCS += $(COMMON_DIR)/driver/tz/tz_malloc.c
cSRCS += $(COMMON_DIR)/driver/tz/tee_ca_mgr_cmd.c
cSRCS += $(COMMON_DIR)/driver/tz/tee_ca_sys_cmd.c
ifeq ($(CPU_ARCH), armv8)
sSRCS += $(COMMON_DIR)/driver/tz/smc_64.S
else
sSRCS += $(COMMON_DIR)/driver/tz/smc.S
endif
CFLAGS += -I$(COMMON_DIR)/driver/tz/
SFLAGS += -I$(COMMON_DIR)/driver/tz/
endif

#############################################################

#
# Common objects
#
OBJECTS := $(sSRCS:%.S=$(OBJ_DIR)/%.o) $(cSRCS:%.c=$(OBJ_DIR)/%.o)

#
# Include the make variables (CC, etc...)
#
ifneq (${CONFIG_RDK_SYS},y)
AS		= $(SDK_CROSS_COMPILE)as
LD		= $(SDK_CROSS_COMPILE)ld
CC		= $(SDK_CROSS_COMPILE)gcc
CPP		= $(SDK_CROSS_COMPILE)g++
AR		= $(SDK_CROSS_COMPILE)ar
NM		= $(SDK_CROSS_COMPILE)nm
STRIP		= $(SDK_CROSS_COMPILE)strip
OBJCOPY		= $(SDK_CROSS_COMPILE)objcopy
OBJDUMP		= $(SDK_CROSS_COMPILE)objdump
endif

GCCINCDIR	= $(shell $(CC) -print-file-name=include)
PLATFORM_LIBS	= -L $(shell dirname `$(CC) $(CFLAGS) -print-libgcc-file-name`) -lgcc

# Suppress annoying output unless V is set
#define V
ifndef V
	SILENT_CC       = @echo '  [CC]       ' $<;
	SILENT_AS       = @echo '  [AS]       ' $<;
	SILENT_CXX      = @echo '  [CXX]      ' $<;
	SILENT_LD       = @echo '  [LD]       ' $@;
	SILENT_AR       = @echo '  [AR]       ' $@;
	SILENT_OBJCOPY  = @echo '  [OBJCOPY]  ' $@;
	SILENT_OBJDUMP  = @echo '  [OBJDUMP]  ' $@;
	SILENT_DISAS    = @echo '  [DISAS]    ' $(@:$(OUT_DIR)/%.bin=%).lss;
endif
#endef


#
# Common rule
#
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(SILENT_CC) $(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<
$(OBJ_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(SILENT_AS) $(CC) $(SFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<

#
# Common Make targets
#
BOOLOADER_ELF := $(OUT_DIR)/bootloader.elf
BOOLOADER_BIN := $(OUT_DIR)/bootloader.bin
BOOLOADER_LSS := $(OUT_DIR)/bootloader.lss
BOOLOADER_MAP := $(OUT_DIR)/bootloader.map

ifeq ($(BOOTLOADER_DDR_START), )
	BOOTLOADER_DDR_START := 0x00680000
endif
ifeq ($(BOOTLOADER_DDR_END), )
	BOOTLOADER_DDR_END := 0x00A00000
endif
ifeq ($(BOOTLOADER_DDR_BUF), )
	BOOTLOADER_DDR_BUF := 0x00600000
endif

ifeq ($(CPU_ARCH), armv8)
LDS_FILE=bootloader_64bit.lds.template
else
LDS_FILE=bootloader.lds.template
endif

all:	clean bootloader
#	@./addheader.sh $<.elf $<.img
#	@./encryption.sh $<.img $<.img.en

bootloader: header $(BOOLOADER_BIN)

$(BOOLOADER_ELF): $(OBJECTS)
	@mkdir -p $(OBJ_DIR)
	@sed "s/CONFIG_BL_DDR_START/$(BOOTLOADER_DDR_START)/g" ./script/$(LDS_FILE) > ./script/bootloader_temp1.lds
	@sed "s/CONFIG_BL_DDR_END/$(BOOTLOADER_DDR_END)/g" ./script/bootloader_temp1.lds > ./script/bootloader_temp2.lds
	@sed "s/CONFIG_BL_DDR_BUF/$(BOOTLOADER_DDR_BUF)/g" ./script/bootloader_temp2.lds > ./script/bootloader.lds
	@rm ./script/bootloader_temp1.lds ./script/bootloader_temp2.lds
	$(SILENT_LD) $(LD) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $(BOOLOADER_ELF) $(OBJECTS) $(PLATFORM_LIBS) -Map=$(BOOLOADER_MAP)

$(BOOLOADER_BIN): $(BOOLOADER_ELF)
	$(SILENT_OBJCOPY) $(OBJCOPY) -O binary $< $@
	$(SILENT_DISAS) $(OBJDUMP) -d -x $< > $(BOOLOADER_LSS)

#############################################################################
#	configuration
#############################################################################
configure: header
	./script/compile_date.sh
#	./script/berlin_config.sh

.PHONY: header
header:	$(INCLUDE_DIR)/Galois_memmap.h $(INCLUDE_DIR)/global.h
	@mkdir -p $(PROJ_DIR)/include/auto
	@./script/header.sh $(INCLUDE_DIR)/Galois_memmap.h $(PROJ_DIR)/include/auto/memmap.h MEMMAP_
	@./script/header.sh $(INCLUDE_DIR)/global.h $(PROJ_DIR)/include/auto/ra_gbl.h RA_Gbl_
	@./script/header.sh $(INCLUDE_DIR)/drmdmx.h $(PROJ_DIR)/include/auto/figo_drm.h Figo_Drm_
#	@./script/header.sh $(INCLUDE_DIR)/drm_sw_spec.h $(PROJ_DIR)/include/auto/drm.h DRM_
	@./script/header.sh $(INCLUDE_DIR)/soc.h $(PROJ_DIR)/include/auto/Berlin_SoC.h System

.PHONY: clean
clean:
	@rm -rf $(OUT_DIR)/*

.PHONY: regen
regen: clean all
