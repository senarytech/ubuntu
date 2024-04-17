LOCAL_PATH := $(call my-dir)

include $(LOCAL_PATH)/voicecapture/voicecapture.mk
include $(LOCAL_PATH)/powermanaged/powermanaged.mk
include $(LOCAL_PATH)/app_smbox/app_smbox.mk
include $(LOCAL_PATH)/test_spi/test_spi.mk
include $(LOCAL_PATH)/watchdogd/watchdogd.mk
include $(LOCAL_PATH)/app_gpio/app_gpio.mk
include $(LOCAL_PATH)/test_standby/test_standby.mk
include $(LOCAL_PATH)/libsynav4l2/libsynav4l2.mk
include $(LOCAL_PATH)/isp_pqtool/isp_pqtool.mk
include $(LOCAL_PATH)/ota_api/ota_api.mk
include $(LOCAL_PATH)/test_ota/test_ota.mk
include $(LOCAL_PATH)/v4l2test/v4l2test.mk

ifdef CONFIG_AMPLITUDE
    ifeq ($(CONFIG_AMPLITUDE), y)
        include $(LOCAL_PATH)/amplitude/amplitude.mk
    endif
endif
