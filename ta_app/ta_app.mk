TA_LOCAL_PATH := $(call my-dir)

include $(wildcard $(TA_LOCAL_PATH)/drm/drm.mk)
include $(wildcard $(TA_LOCAL_PATH)/ta_calc/ta_calc.mk)
include $(wildcard $(TA_LOCAL_PATH)/voice/voice.mk)
include $(wildcard $(TA_LOCAL_PATH)/ta_gfx_img/ta_gfx_img.mk)
include $(wildcard $(TA_LOCAL_PATH)/ta_gfx_img_linux/ta_gfx_img_linux.mk)

