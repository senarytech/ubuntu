TEE_TOP_REL_PATH := ../../../../../tee

CA_PATH		:= ../..
CA_MODULE	:= CustBlobapp

CA_CFLAGS	:=
CA_LDFLAGS	:= -static
CA_C_INCLUDES	:=

CA_SRC_FILES	:= $(wildcard $(CA_PATH)/tee_ca_*.c)

# must be in the end of file
include $(TEE_TOP_REL_PATH)/build/tee/tee_ca_linux.mk
