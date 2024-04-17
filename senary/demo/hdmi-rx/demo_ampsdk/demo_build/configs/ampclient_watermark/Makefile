include $(BUILDSYS)/config_inc.mak
include $(BUILDSYS)/common.mak

LIB_NAME = ampclient_watermark
LIB_TYPE = application
LIB_DEPEND = libOSAL libampclient

ifeq ($(CONFIG_ENABLE_DRM), y)
LIB_DEPEND += libdrmclient
endif

# FFMPEG and Alsa wrapper need to link dl
LIB_DEPEND_EXTERNAL = libdl

# Opencv Lib
# LIB_DEPEND += libndi libavahi-client libavahi-common libavahi-core libdbus-1
LIB_DEPEND += libz libtbb libopencv_core libopencv_highgui libopencv_imgproc libopencv_ml

ifeq ($(CONFIG_DYNAMIC_FFMPEG),)
# linker ffmpeg library when build
LIB_DEPEND_EXTERNAL += libavcodec libavformat libavutil
endif


ifeq ($(OS), Android)
LIB_DEPEND_EXTERNAL += libcutils
endif

include $(BUILDSYS)/library.mak

ifdef LIB_NAME
SUBMODULES = amp_watermark

amp_watermark :
	echo "======================================== amp watermark  =================================================="
	echo $(SYS_INSTALL_DIR)
#	@cp -r $(AMPSDKTOP)/amp/test/client_test/amp_watermark/lib/opencv_lib $(SYS_INSTALL_DIR)
	@cp -r $(AMPSDKTOP)/amp/test/client_test/amp_watermark/lib $(SYS_INSTALL_DIR)
	@+make -C $(AMPSDKTOP)/amp/test/client_test/amp_watermark $(BUILD_TARGET)
endif

include $(BUILDSYS)/executable.mak
