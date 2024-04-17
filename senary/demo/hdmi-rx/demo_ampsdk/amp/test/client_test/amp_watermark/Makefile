######################################################
# Build configurations
include $(BUILDSYS)/common.mak

######################################################
# Module library

MODULE_NAME = amp_watermark

MODULE_SRCS = test_main.c

#(Song)client API should not have libamphal dependency
#MODULE_SRCS += test_gpio_ctl.c

MODULE_SRCS += ndi_avin.c
MODULE_SRCS += ndi_buf.c
MODULE_SRCS += disp_test.c
MODULE_SRCS += disp_test_framebuffer.c
MODULE_SRCS += disp_test_prv.c
MODULE_SRCS += ringbuf.c
MODULE_SRCS += watermark.cpp


MODULE_CFLAGS += -DUSE_SINGLE_QUEUE -DUSE_FFMPEG 


######################################################
# Dynamic library

MODULE_DYN_LIB = 

DYN_LIB_SRCS =

######################################################
# Header files

PUBLIC_HEADER_FILES =

######################################################
# Application

PROGRAM =

PROGRAM_SRCS =

PROGRAM_OBJS =

######################################################
# Module flags

#MODULE_CFLAGS = -I$(AMPSDKTOP)/Audio/AudioCommon/include
# supress ffmpeg's deprecated warning
MODULE_CFLAGS += -Wno-deprecated-declarations

MODULE_CFLAGS += -I$(AMPSDKTOP)/osal/include \
                 -I$(AMPSDKTOP)/amp/inc \
                 -I$(AMPSDKTOP)/amp/src/cpcl/inc \
                 -I$(AMPSDKTOP)/amp/src/libs/inc \
                 -I$(AMPSDKTOP)/amp/src/ddl/dmx/vdhub/inc \
                 -I$(AMPSDKTOP)/amp/src/ddl/vcodec/comp_vdec/include \
                 -I$(AMPSDKTOP)/amp/tools/ffmpeg/include \
                 -I$(AMPSDKTOP)/amp/src/libs/shm/inc


MODULE_CFLAGS += -I$(AMPSDKTOP)/amp/src/service/display/inc \
                 -I$(AMPSDKTOP)/amp/src/hal/dhub/include \
                 -I$(AMPSDKTOP)/amp/src/hal/avpll/include \
                 -I$(AMPSDKTOP)/amp/src/hal/vpp/include \
                 -I$(AMPSDKTOP)/amp/src/hal/common/include \
                 $(Galois_SoC_COMMON_INC)
				 
MODULE_CFLAGS += -I./include
MODULE_CFLAGS += -I./include/opencv2

MODULE_LDFLAGS = -L$(AMPSDKTOP)amp/test/client_test/amp_watermark/lib
#MODULE_LDFLAGS += -L/home/lihuiliang/sdk/senary/syna-release/ampsdk/amp/test/client_test/amp_watermark/lib/opencv_lib
#MODULE_LDFLAGS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_ml 
#add path: ampsdk/build/config/ampclient_watermark

######################################################
# Unittest setting

UNITTEST_SRCS =

UNITTEST_OBJS =

######################################################
# Release setting

# released source files
RELEASE_SRCS =

# released header files
RELEASE_HEADER_FILES =

# released library
RELEASE_LIBRARY = MODULE_LIBRARY

# source files to build released library
RELEASE_LIB_SRCS = MODULE_SRCS

# object files to build released library
RELEASE_LIB_OBJS = MODULE_OBJS

######################################################
# Common dependency process

include $(BUILDSYS)/module.mak
