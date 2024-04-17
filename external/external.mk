LOCAL_PATH := $(call my-dir)

include $(LOCAL_PATH)/alsa/alsa.mk
include $(LOCAL_PATH)/curl/curl.mk
include $(LOCAL_PATH)/ffmpeg/ffmpeg.mk
#include $(LOCAL_PATH)/gdb/gdb.mk
#include $(LOCAL_PATH)/dbus/dbus.mk
#include $(LOCAL_PATH)/expat/expat.mk
#include $(LOCAL_PATH)/glib2/glib2.mk
#include $(LOCAL_PATH)/libffi/libffi.mk
#include $(LOCAL_PATH)/eudev/eudev.mk
#include $(LOCAL_PATH)/readline/readline.mk
#include $(LOCAL_PATH)/ncurses/ncurses.mk
#include $(LOCAL_PATH)/pulseaudio/pulseaudio.mk
#include $(LOCAL_PATH)/libtool/libtool.mk
#include $(LOCAL_PATH)/libsndfile/libsndfile.mk
#include $(LOCAL_PATH)/zlib/zlib.mk
#include $(LOCAL_PATH)/sbc/sbc.mk
include $(LOCAL_PATH)/boost/boost.mk
include $(LOCAL_PATH)/jsoncpp/jsoncpp.mk
include $(LOCAL_PATH)/cppnetlib/cppnetlib.mk
