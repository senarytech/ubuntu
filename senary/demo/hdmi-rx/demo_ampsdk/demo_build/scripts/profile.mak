-include $(CONFIG_FILE)

.PHONY : all rel_mod dev_mod

ifdef AMP_REL_PATH

#release build list
# DEP_MOD is added because of module dependency.
# If we don't add it, would meet build error
# TODO: should remove the dependency in the future
DEP_MOD := amp_fw amp_ta libamphal libampclient libgraphics
ifneq ($(AMP_EXT_IP_LIST),)
all: $(DEP_MOD) $(AMP_EXT_IP_LIST)
else
IP_MODULES := libddlaout libddlacodec libddldvs libpr25client libpr25srv

ifdef CONFIG_AMP_IP_DRM_NAGRA
IP_MODULES += libsec_ree libnagra_factory nv_talts dalts
endif

ifdef CONFIG_AMP_IP_DRM_NSK_HARMONIZER
IP_MODULES += NskClientTest
endif

ifdef CONFIG_AMP_IP_DRM_VMX_ULTRA
IP_MODULES += libvmxultrasrv
endif

ifdef CONFIG_AMP_IP_DRM_IRDETO
IP_MODULES += libirdetosrv
endif

all: $(DEP_MOD) $(IP_MODULES)
endif

else
all: rel_mod dev_mod

rel_mod: ampservice libampclient

ifdef CONFIG_AMP_COMPONENT_DISPSRV_ENABLE
rel_mod: libgraphics libedid libcecsvs
endif

ifdef CONFIG_ENABLE_DRM
rel_mod: libdrmclient
endif

ifdef CONFIG_AMP_IP_DRM_VMX_ULTRA
rel_mod: libvmxultrasrv
endif

ifdef CONFIG_AMP_IP_DRM_PR_25
rel_mod: libpr25client
endif

ifdef CONFIG_AMP_IP_DRM_NAGRA
rel_mod: libsec_ree libnagra_factory nv_talts dalts
endif

ifdef CONFIG_AMP_IP_DRM_NSK_HARMONIZER
rel_mod: NskClientTest
endif

ifdef CONFIG_AMP_IP_DRM_IRDETO
rel_mod: libirdetosrv
endif


#TODO(Song): move libsm out of ampsdk
rel_mod: libsm

ifeq ($(CONFIG_AMP_COMPONENT_VA_ENABLE), y)
VA_CLIENT := ampclient_va
else
VA_CLIENT :=
endif


ifdef CONFIG_AMP_DEV_TOOLS
dev_mod: AmpUnitTest AmpPipeTest ampdiag mdb ampclient_alpha ampclient_ndi ampclient_watermark ampclient_samples amp_transcode ampclient_customer provision AmpIPControlTest \
ampclient_pvr $(VA_CLIENT)

ifdef CONFIG_AMP_AUDIO_USE_DSP_HIFI
dev_mod: test_dsp
#DO NOT enable $test_rma_local by default
#It is available ONLY in source code SDK.
#dev_mod: test_rma_local
endif

ifdef CONFIG_LINUX_OS
dev_mod: ampclient_avsettings
endif

ifdef CONFIG_AMP_COMPONENT_DISPSRV_ENABLE
dev_mod: test_disp
endif

dev_mod: ampclient_watermark

dev_mod: test_cecsvs

ifdef CONFIG_ENABLE_DRM
dev_mod: test_drm_ipctl
endif

ifdef CONFIG_AMP_COMPONENT_DMX_ENABLE
dev_mod: ampdmxtest ampdmxutil
endif

ifdef CONFIG_AMP_COMPONENT_ISP_ENABLE
dev_mod: ampisptest
endif

ifdef CONFIG_AMP_IP_DRM_NAGRA
dev_mod: nagra_factory_test nagra_stream_test
endif

ifdef CONFIG_SCRD
dev_mod: test_scrd
endif

ifdef CONFIG_AMP_IP_DRM_IRDETO
dev_mod: IrdetoTest
endif

endif #CONFIG_AMP_DEV_TOOLS

endif #AMP_REL_PATH

include $(AMPSDKTOP)/build/scripts/library_deps.mak
