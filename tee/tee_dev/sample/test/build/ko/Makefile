obj-m := test.o
test-objs := ../../tee_ko_test.o

ifeq ($(KERNELRELEASE),)
# change below kernel directory to match your system
KERNELDIR ?= ~/gtv/vendor/marvell-sdk/linux

# don't use arm-linux-androideabi-, for it's only for building android
# application. if use it to build kernel or driver, they would get some
# unexpected behaviors, which is very difficult to debug.
CROSS_COMPILE ?= arm-eabi-

ARCH ?= arm

export ARCH CROSS_COMPILE

.PHONY: all clean

all: test

test: clean
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) clean
	$(Q)rm -rf $(test-objs)
	$(Q)rm -rf $(tzlogger-objs)
else
TEEDIR = $(M)/../../../..
EXTRA_CFLAGS += -I$(TEEDIR)/../include/
EXTRA_CFLAGS += -I$(TEEDIR)/common/
EXTRA_CFLAGS += -I$(TEEDIR)/client_api/
EXTRA_CFLAGS += -I$(TEEDIR)/internal_api/
EXTRA_CFLAGS += -DCONFIG_TEE
endif
