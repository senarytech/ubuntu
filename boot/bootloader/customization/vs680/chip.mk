##################################################
#
##	Include files
#
##################################################

INCLUDE_DIR = $(COMMON_INC)/chip/vs680/$(CHIP_VER)

##################################################
#
##	Source files
#
##################################################

cSRCS += $(CHIP_PATH_LOWCASE)/chip_customization.c
cSRCS += $(CHIP_PATH_LOWCASE)/dev_management.c
cSRCS += $(CHIP_PATH_LOWCASE)/chip_id.c
cSRCS += $(CHIP_PATH_LOWCASE)/pin_init.c
ifneq ($(PV_COMP), )
cSRCS += $(CHIP_PATH_LOWCASE)/chip_voltage_info.c
endif
cSRCS += $(CHIP_PATH_LOWCASE)/pinmux.c
cSRCS += $(CHIP_PATH_LOWCASE)/gpio.c
cSRCS += $(COMMON_DIR)/driver/gic/gic400-r0p1/gic_diag.c

##################################################
#
##	C/S flags
#
##################################################
CFLAGS += -DBERLIN_SOC_VS680
CFLAGS += -mcpu=cortex-a73
SFLAGS += -DBERLIN_SOC_VS680
CFLAGS += -mcpu=cortex-a73
CFLAGS += -DBERLIN_CHIP_VERSION=BERLIN_VS680
SFLAGS += -DBERLIN_CHIP_VERSION=BERLIN_VS680
CFLAGS += -I$(COMMON_DIR)/driver/gic/gic400-r0p1/
SFLAGS += -I$(COMMON_DIR)/driver/gic/gic400-r0p1/
CFLAGS += -I$(CHIP_PATH_LOWCASE)/
CFLAGS += -DLINUX_4_14
HOST_CFLAGS += -DBERLIN_CHIP_VERSION=BERLIN_VS680
CHIP_DIR = $(COMMON_DIR)/chip/vs680
