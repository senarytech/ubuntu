TEE_TOP_REL_PATH := ../../../../..

CA_PATH		:= ../..
CA_MODULE	:= $(TEE_TOP_REL_PATH)/tee/bin/tee_daemon

CA_CFLAGS	?= -I$(TEE_TOP_REL_PATH)/tee/ree
CA_LDFLAGS	:= -static
CA_C_INCLUDES	:=

CA_SRC_FILES	:= $(wildcard $(CA_PATH)/tee_ca_*.c)

# must be in the end of file
include $(TEE_TOP_REL_PATH)/build/tee/tee_daemon_linux.mk
