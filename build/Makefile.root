
EXTERNAL_MK_FILE = $(CURDIR)/build/.external.mk
-include $(EXTERNAL_MK_FILE)
$(shell build/scripts/create_external_config \
	-m -o '$(EXTERNAL_MK_FILE)' $(EXTERNAL_TREE))
include $(EXTERNAL_MK_FILE)
patch_targets := linux_debug
nobuild_targets := %config help list-defconfigs clean
nobuild_targets += $(patch_targets)
ifeq ($(MAKECMDGOALS),)
	BUILD_TARGET=y
else ifneq ($(filter-out $(nobuild_targets),$(MAKECMDGOALS)),)
	BUILD_TARGET = y
endif

ifeq ($(BUILD_TARGET), y)
include build/Makefile
else
.PHONY: help
help:
	@echo 'Cleaning:'
	@echo '  clean			- delete all files created by build'
	@echo
	@echo 'Build:'
	@echo '  all/target		- make world'
	@echo
	@echo 'Configuration:'
	@echo '  menuconfig			- interactive curses-based configurator'
	@echo '  silentoldconfig	- generate autoconfig.h'
	@echo '  list-defconfigs	- lists available configs'
	@echo
	@echo 'Module rules:'
	@echo '  <module>		- Build and install <module> and all its dependencies'
	@echo '  <module>-clean		- clean the module'
	@echo '  <module>-build		- build the module'
	@echo '  <module>-rebuild	- build module without clean'
	@echo '  <module>-depends	- build module dependencies'
	@echo '  <module>-show-depends	- print module dependencies'
	@echo

define list-defconfig
	$(info $(lastword $(subst /, ,$(1))) : $(shell sed -n 's/^ *CONFIG_DESCRIPTION= *//p' $(1) ))
endef

.PHONY: list-defconfigs
list-defconfigs:
	$(foreach defconfig, $(wildcard $(CURDIR)/configs/product/*/*_defconfig), \
	$(call list-defconfig,$(defconfig)))
	$(foreach defconfig, $(wildcard $(EXTERNAL_ROOT_PATH)/configs/product/*/*_defconfig), \
	$(call list-defconfig,$(defconfig)))
	@echo

.PHONY: build/.Kconfig_external.build
build/.Kconfig_external.build:
	@build/scripts/create_external_config -k -o "$(@)" $(EXTERNAL_TREE)

.PHONY: build/.external.rc
build/.external.rc:
	@build/scripts/create_external_config -r -o "$(@)" $(EXTERNAL_TREE)

.PHONY: del_link amp_soft_link
del_link:
	@rm -f build/module/ampsdk/kconfig/select

ifeq ($(RELEASE),y)
amp_soft_link: del_link
	@ln -s select_release build/module/ampsdk/kconfig/select
else
amp_soft_link: del_link
	@ln -s select_build build/module/ampsdk/kconfig/select
endif

#select override config
ifneq ($(filter $(patch_targets), $(MAKECMDGOALS)),)
.PHONY: $(patch_targets)

sdk_config=$(filter %config, $(MAKECMDGOALS))
profile=$(subst _defconfig, ,$(sdk_config))
ifeq ($(filter $(patch_targets), $(MAKECMDGOALS)), linux_debug)
patch_config=$(addsuffix .dbg, $(profile))
endif
$(info "config":$(sdk_config) "patch":$(patch_config))

linux_debug: $(sdk_config)
	@build/scripts/apply_debug_patch $(sdk_config) ${patch_config}

endif

#.PHONY: %config
%config: amp_soft_link build/.Kconfig_external.build build/.external.rc
	@make -C build -f Makefile.config RELEASE=$(RELEASE) EXTERNAL_TREE=$(EXTERNAL_TREE) $@

.PHONY: clean
clean:
	@-rm -rf out

endif
