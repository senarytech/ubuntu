CONFIG_TEE_SYS_MUTEX                   ?= y

ccflags-$(CONFIG_REE_SYS_MUTEX)        += -DCONFIG_REE_SYS_MUTEX
ccflags-$(CONFIG_TEE_SYS_MUTEX)        += -DCONFIG_TEE_SYS_MUTEX

obj-m := tzd.o
tzd-y = tz_driver.o tz_dev_logger.o tz_utils.o tz_shm.o
tzd-$(CONFIG_COMPAT) += tz_compat.o
tzd-y += ../common/tz_comm.o
tzd-y += ../common/smc.o
tzd-y += ../nw_api/tz_nw_comm.o
tzd-y += ../nw_api/tz_nw_task_client.o
tzd-y += ../nw_api/tz_nw_sys_callback.o
tzd-y += ../nw_api/tz_nw_osal_linux.o
tzd-y += ../client_api/tz_client_api_linux_kernel.o
tzd-y += ../../tee/client_api/tee_client_api.o
tzd-y += ../../tee/client_api/tee_client_util.o
tzd-y += ../../tee/mgr/tee_ca_mgr_cmd.o
tzd-y += ../../tee/mgr/tee_ca_sys_cmd.o
tzd-y += ../../tee/ree/ree_sys_callback.o
tzd-y += ../../tee/ree/ree_sys_callback_linux.o
tzd-$(CONFIG_REE_SYS_MUTEX) += ../../tee/ree/ree_sys_callback_mutex.o
tzd-$(CONFIG_TEE_SYS_MUTEX) += ../../tee/ree/ree_sys_callback_mutex_ext.o
tzd-$(CONFIG_REE_SYS_SEMAPHORE) += ../../tee/ree/ree_sys_callback_semaphore.o

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

all: tzd

tzd: clean
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) modules_install

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) clean
	$(Q)rm -rf $(tzd-y)
	$(Q)rm -rf $(tzlogger-y)
else
EXTRA_CFLAGS += -I$(M)/../../tz/nw_api/
EXTRA_CFLAGS += -I$(M)/../../tz/common/
EXTRA_CFLAGS += -I$(M)/../../include/
EXTRA_CFLAGS += -I$(M)/../../arch/arm/include/
EXTRA_CFLAGS += -I$(M)/../../tz/driver/
EXTRA_CFLAGS += -I$(M)/../../tz/client_api/
EXTRA_CFLAGS += -I$(M)/../../tee/common/
EXTRA_CFLAGS += -I$(M)/../../tee/client_api/
EXTRA_CFLAGS += -I$(M)/../../tee/internal_api/
EXTRA_CFLAGS += -I$(M)/../../tee/mgr/
EXTRA_CFLAGS += -I$(M)/../../tee/ree/
EXTRA_CFLAGS += -DCONFIG_TEE
endif
