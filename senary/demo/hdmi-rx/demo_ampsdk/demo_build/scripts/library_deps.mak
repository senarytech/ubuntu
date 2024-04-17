ifndef LIBRARY_DEPS_MAK
LIBRARY_DEPS_MAK=1
include $(CONFIG_FILE)

define inner-generic-make
	if test -z "$(2)" ; then \
		make -C $(1) $(BUILD_TARGET) || exit "$$?"; \
	else \
		make -C $(1) $(2) || exit "$$?"; \
	fi
endef

define generic-make
	if test -d $(1); then \
		$(call inner-generic-make,$(1),$(2)); \
	elif test -d $(AMPSDKTOP)/build/configs/$(1); then \
		$(call inner-generic-make,$(AMPSDKTOP)/build/configs/$(1),$(2)); \
	fi
endef

###############################################################################
#amp basic library
###############################################################################
build_amp_libs: libshmserver libamphal libamputil_server
prerequisite_amp_libs: flick_mvcc libamprpc_idl

libOSAL:
	@echo build library $@
	@+$(call generic-make,$@)

libshmserver : prerequisite_amp_libs libOSAL libamputil_server
	@echo build library $@
	@+$(call generic-make,$@)

libamputil_server : prerequisite_amp_libs libOSAL
	@echo build library $@
	@+$(call generic-make,$@)

ifeq ($(CONFIG_AMP_LIB_SEPARATE_VPPDRV), y)
LIBVPP := libvppdrv
LIBWATERMARK_TA :=
ifeq ($(enable_vpp_recovery_mode_support), true)
LIBVPP_RECOVERY_TA := libvpp_recovery.ta
endif
else
LIBVPP :=
LIBWATERMARK_TA :=
LIBVPP_RECOVERY_TA :=
endif

ifeq ($(CONFIG_AMP_LIB_HDCP2X), y)
HDCP2X_TA := libhdcp2x.ta
HDCP2X_CA := libhdcp2xdrv
else
HDCP2X_TA :=
HDCP2X_CA :=
endif

ifeq ($(CONFIG_AMP_LIB_NXG), y)
NXG_TA += libnxg.ta
else
NXG_TA :=
endif
NXG_TA += libvpp_ngptv
VMX_A += libvpp_vmx
NXG_CA += libnxgdrv

ifeq ($(CONFIG_AMP_COMPONENT_DISPSRV_ENABLE), y)
DISP_TA := $(LIBVPP_RECOVERY_TA) $(LIBWATERMARK_TA)
DISP_TA += libvpp.ta libdhub.ta liboutctrl.ta libfastlogo.ta
DISP_TA += $(HDCP2X_TA)
DISP_TA += $(NXG_TA)
DISP_TA += $(VMX_A)
endif

libdolbyms12 :
	@echo build library $@
	@+$(call generic-make,$@)

libdolbyms12_v1 :
	@echo build library $@
	@+$(call generic-make,$@)

UDC_LITE_TEST :
	@echo build library $@
	@+$(call generic-make,$@)

ifeq ($(CONFIG_AMP_ISPBE_ENABLE), y)
LIB_ISPSS := libispbe
else
LIB_ISPSS :=
endif

ifeq ($(CONFIG_AMP_COMPONENT_ISP_ENABLE), y)
LIB_ISPSS += libisp
endif

ifeq ($(CONFIG_ENABLE_TUNER_DEMOD), y)
LIB_TUNERDEMOD := libtunerdemod
else
LIB_TUNERDEMOD :=
endif

ifeq ($(CONFIG_SCRD), y)
LIB_SCRD := libscrd
else
LIB_SCRD :=
endif

libamphal : prerequisite_amp_libs libOSAL libshmserver libamputil_server libi2c libampgpio $(LIBVPP) $(LIB_ISPSS) $(HDCP2X_CA) $(LIB_SCRD) $(NXG_CA)
	@echo build library $@
	@+$(call generic-make,$@)

flick_mvcc : libOSAL
	@echo build library $@
	@+$(call generic-make,$@)

libampclient : prerequisite_amp_libs libOSAL
	@echo build library $@
	@+$(call generic-make,$@)

libamprpc_idl :
	@echo build library $@
	@+$(call generic-make,$@)

libsm :
	@echo build library $@
	@+$(call generic-make,$@)

libscrd : libOSAL libampclient
	@echo build library $@
	@+$(call generic-make,$@)

libi2c :
	@echo build library $@
	@+$(call generic-make,$@)

libspeexdsp :
	@echo build library $@
	@+$(call generic-make,$@)

libvppdrv: libOSAL libshmserver libamputil_server libi2c
	@echo build library $@
	@+$(call generic-make,$@)

libispbe: libOSAL libshmserver libamputil_server
	@echo build library $@
	@+$(call generic-make,$@)

libisp: libOSAL libshmserver libamputil_server libi2c libispvsi3a libispmisc libIMX258 libIMX415 libOV5647 libIMX219
	@echo build library $@
	@+$(call generic-make,$@)

ifeq ($(CONFIG_AMP_LIB_HDCP2X), y)
libhdcp2xdrv: libOSAL libshmserver
	@echo build library $@
	@+$(call generic-make,$@)
endif

libnxgdrv: libOSAL libshmserver
	@echo build library $@
	@+$(call generic-make,$@)

libIMX258: libOSAL libi2c
	@echo build library $@
	@+$(call generic-make,$@)

libIMX415: libOSAL libi2c
	@echo build library $@
	@+$(call generic-make,$@)

libOV5647: libOSAL libi2c
	@echo build library $@
	@+$(call generic-make,$@)

libIMX219: libOSAL libi2c
	@echo build library $@
	@+$(call generic-make,$@)
	
libispvsi3a: libOSAL
	@echo build library $@
	@+$(call generic-make,$@)

libispmisc: libOSAL libshmserver libamputil_server libi2c
	@echo build library $@
	@+$(call generic-make,$@)

###############################################################################
#add ddl library here
###############################################################################
COMPONENT_DDLS = build_amp_libs libddlcommon

ifeq ($(CONFIG_AMP_IOMMU), y)
COMP_TA += libptm.ta
endif

ifeq ($(CONFIG_AMP_COMPONENT_DUM_ENABLE), y)
COMPONENT_DDLS += libddldummy
endif

ifeq ($(CONFIG_AMP_COMPONENT_CLK_ENABLE), y)
COMPONENT_DDLS += libddlclk
endif

ifeq ($(CONFIG_AMP_COMPONENT_DMX_ENABLE), y)
COMPONENT_DDLS += libdmxcomm libddldmx libtspdrv
COMP_TA +=libtsp.ta libm2m.ta libdss.ta
endif

ifeq ($(CONFIG_AMP_COMPONENT_VDEC_ENABLE), y)
COMPONENT_DDLS += libddlvsched libddlvdec
COMP_TA += libvmeta.ta
endif

ifeq ($(CONFIG_AMP_COMPONENT_ADEC_ENABLE), y)
COMPONENT_DDLS += libddlacodec libddladec
COMP_TA += libadec.ta
ifeq ($(CONFIG_GEN_AMP_AUDIO_ENABLE_ZSP), y)
COMP_TA += libaac.ta libwma.ta libzsp.ta libdtscore.ta libdtshd.ta libsrstruvol.ta
else
ifeq ($(CONFIG_AMP_AUDIO_AAC_SUPPORTED), y)
COMP_TA += libaac.ta
endif
ifeq ($(CONFIG_AMP_AUDIO_WMA_SUPPORTED), y)
COMP_TA += libwma.ta
endif
endif #CONFIG_GEN_AMP_AUDIO_ENABLE_ZSP
ifeq ($(CONFIG_AMP_AUDIO_USE_DSP_HIFI), y)
COMP_TA += libdsp.ta
endif
endif #CONFIG_AMP_COMPONENT_ADEC_ENABLE

ifeq ($(CONFIG_AMP_COMPONENT_VENC_ENABLE), y)
COMPONENT_DDLS += libddlvsched libddlvenc
COMP_TA += libvmeta.ta
endif

ifeq ($(CONFIG_AMP_COMPONENT_AENC_ENABLE), y)
COMPONENT_DDLS += libddlacodec libddlaenc
endif

ifeq ($(CONFIG_AMP_COMPONENT_VOUT_ENABLE), y)
COMPONENT_DDLS += libddlvout
endif

ifeq ($(CONFIG_AMP_COMPONENT_AREN_ENABLE), y)
COMPONENT_DDLS += libddlaren
endif

ifeq ($(CONFIG_AMP_COMPONENT_APP_ENABLE), y)
COMPONENT_DDLS +=
endif

ifeq ($(CONFIG_AMP_COMPONENT_AOUT_ENABLE), y)
COMPONENT_DDLS += libddlaout
endif

ifeq ($(CONFIG_AMP_COMPONENT_FE_ENABLE), y)
COMPONENT_DDLS += libddlfe
endif

ifeq ($(CONFIG_AMP_COMPONENT_AVIN_ENABLE), y)
COMPONENT_DDLS += libddlavin
endif

COMP_AVIN_DEPS = libddlcommon
ifeq ($(CONFIG_AMP_AVIN_ENABLE_CYPRESS), y)
COMP_AVIN_DEPS += libcypress libdtvbsp
endif

ifeq ($(CONFIG_AMP_COMPONENT_DEINT_ENABLE), y)
COMPONENT_DDLS += libddldeint
COMP_TA += libovp.ta
endif

ifeq ($(CONFIG_AMP_COMPONENT_SCALER_ENABLE), y)
COMPONENT_DDLS += libddlscaler
ifeq (VS680, $(SoC_Ver))
COMP_TA += libvpp.ta
endif
endif

ifeq ($(CONFIG_AMP_COMPONENT_ISP_ENABLE), y)
COMPONENT_DDLS += libddlisp
endif

ifeq ($(CONFIG_AMP_COMPONENT_DEWARP_ENABLE), y)
COMPONENT_DDLS += libddldewarp
endif

ifeq ($(CONFIG_AMP_COMPONENT_ROTATE_ENABLE), y)
COMPONENT_DDLS += libddlrotate
endif

ifeq ($(CONFIG_AMP_COMPONENT_DNS_ENABLE), y)
COMPONENT_DDLS += libddldns
endif

ifeq ($(CONFIG_AMP_COMPONENT_SR_ENABLE), y)
#COMPONENT_DDLS += libsrcommon libddlsr
COMPONENT_DDLS += libddlsr
endif

ifeq ($(CONFIG_AMP_COMPONENT_VA_ENABLE), y)
COMPONENT_DDLS += libddlva
endif

ifeq ($(CONFIG_AMP_COMPONENT_VCAP_ENABLE), y)
COMPONENT_DDLS += libddlvcap
COMP_TA += libvcap.ta
endif

ifeq ($(CONFIG_AMP_COMPONENT_DVS_ENABLE), y)
COMPONENT_DDLS += libddldvs
COMP_TA += libdvs.ta
endif

ifeq ($(CONFIG_AMP_COMPONENT_DATA_EXPORTER_ENABLE), y)
COMPONENT_DDLS += libddldata_exporter
COMP_TA += libdata_exporter.ta
endif

ifeq ($(CONFIG_AMP_CPM_ENABLE), y)
COMP_TA += libcpm.ta
endif
build_amp_ddls: $(COMPONENT_DDLS)

COMPONENT_DRM = libdrmsrv

ifeq ($(CONFIG_AMP_IP_DRM_PR_25), y)
COMPONENT_DRM += libpr25srv
endif

ifeq ($(CONFIG_AMP_IP_DRM_VMX_ULTRA), y)
COMPONENT_DRM += libvmxultrasrv
endif

ifeq ($(CONFIG_AMP_IP_DRM_IRDETO), y)
COMPONENT_DRM += libirdetosrv
endif

ifeq ($(CONFIG_ENABLE_DRM), y)
build_amp_drm: $(COMPONENT_DRM)
else
build_amp_drm:
endif

ifeq ($(CONFIG_AMP_DTV_BSP), y)
build_amp_dtvbsp: libdtvbsp
else
build_amp_dtvbsp:
endif

libddlacodec : libddlcommon libshmserver
	@echo build library $@
	@+$(call generic-make,$@)

ifeq ($(CONFIG_AMP_DRM_ENG_BCM), y)
ADEC_DRM_DEP := libdrmkeymgr
else
ADEC_DRM_DEP := libdrmcommca
endif

libddladec:  libddlcommon libddlacodec libamphal $(ADEC_DRM_DEP)
	@echo build library $@
	@+$(call generic-make,$@)

libddlaenc : libddlacodec
	@echo build library $@
	@+$(call generic-make,$@)

libddlvsched : libamphal libddlcommon
	@echo build library $@
	@+$(call generic-make,$@)

ifeq ($(CONFIG_AMP_COMPONENT_DEINT_ENABLE), y)
VDEC_DEP := libddldeint
else
DEINT :=
endif

libddlvdec : libddlvsched libamphal $(VDEC_DEP)
	@echo build library $@
	@+$(call generic-make,$@)

libddlcommon : libshmserver libamputil_server libdebugca
	@echo build library $@
	@+$(call generic-make,$@)

libddldummy : libddlcommon libdebugca
	@echo build library $@
	@+$(call generic-make,$@)

libddlavin : libamphal $(COMP_AVIN_DEPS)
	@echo build library $@
	@+$(call generic-make,$@)

libddlvout: libddlcommon libdispsrv libddlclk
	@echo build library $@
	@+$(call generic-make,$@)

libddldeint: prerequisite_amp_libs libOSAL libamputil_server libshmserver libamphal libddlcommon
	@echo build library $@
	@+$(call generic-make,$@)

SCL_DEP :=
ifeq (VS680, $(SoC_Ver))
SCL_DEP := libvppdrv
endif

libddlscaler: prerequisite_amp_libs libOSAL libamputil_server libshmserver libamphal libddlcommon libh1driver $(SCL_DEP)
	@echo build library $@
	@+$(call generic-make,$@)

libsrcommon: prerequisite_amp_libs libOSAL libamputil_server libshmserver libamphal libddlcommon
	@echo build library $@
	@+$(call generic-make,$@)

libddlsr: prerequisite_amp_libs libOSAL libamputil_server libshmserver libamphal libddlcommon libsrcommon
	@echo build library $@
	@+$(call generic-make,$@)

libddlva: prerequisite_amp_libs libOSAL libamputil_server libshmserver libamphal libddlcommon
	@echo build library $@
	@+$(call generic-make,$@)

libddlvcap: prerequisite_amp_libs libOSAL libamputil_server libshmserver libamphal libddlcommon
	@echo build library $@
	@+$(call generic-make,$@)

libddlisp: prerequisite_amp_libs libOSAL libamputil_server libshmserver libamphal libddlcommon
	@echo build library $@
	@+$(call generic-make,$@)

libddldewarp: prerequisite_amp_libs libOSAL libamputil_server libshmserver libamphal libddlcommon
	@echo build library $@
	@+$(call generic-make,$@)

libddlrotate: prerequisite_amp_libs libOSAL libamputil_server libshmserver libamphal libddlcommon
	@echo build library $@
	@+$(call generic-make,$@)

libddldns: prerequisite_amp_libs libOSAL libamputil_server libshmserver libamphal libddlcommon
	@echo build library $@
	@+$(call generic-make,$@)

TSP_DRM_DEP :=
ifeq ($(CONFIG_ENABLE_DRM), y)
TSP_DRM_DEP := libdrmkeymgr
endif

libdmxcomm : libddlcommon libdebugca $(TSP_DRM_DEP)
	@echo build library $@
	@+$(call generic-make,$@)

libddlclk: libddlcommon libamphal
	@echo build library $@
	@+$(call generic-make,$@)


libddldmx: libdmxcomm libtspdrv
	@echo build library $@
	@+$(call generic-make,$@)

libtspdrv:libddlcommon libamphal libdmxcomm
	@echo build library $@
	@+$(call generic-make,$@)

libtspm2m :
	@echo build library $@
	@+$(call generic-make,$@)

libddlaren : libamputil_server libshmserver libddlcommon
	@echo build library $@
	@+$(call generic-make,$@)

libddlaout : libOSAL libamphal libamputil_server libshmserver libddlcommon
	@echo build library $@
	@+$(call generic-make,$@)

libddlvenc : libddlvsched libOSAL libddlvdec libamputil_server libshmserver libddlcommon libh1driver libamphal
	@echo build library $@
	@+$(call generic-make,$@)

libh1driver : libddlvsched libOSAL libddlvdec libamputil_server libshmserver libddlcommon libamphal
	@echo build library $@
	@+$(call generic-make,$@)

ifneq ($(CONFIG_AMP_TEE_ENABLE), y)
DV_SPLIT_DEP := libdvsplit
else
DV_SPLIT_DEP :=
endif
libddldvs : libamphal libddlcommon libamputil_server libshmserver  $(DV_SPLIT_DEP)
	@echo build library $@
	@+$(call generic-make,$@)

libddldata_exporter: libamphal libddlcommon libamputil_server libshmserver libdebugca
	@echo build library $@
	@+$(call generic-make,$@)

###############################################################################
#add service library here
###############################################################################

ifeq ($(CONFIG_AMP_COMPONENT_DEINT_ENABLE), y)
DEINT := libddldeint
else
DEINT :=
endif

ifeq ($(CONFIG_AMP_COMPONENT_SNDSRV_ENABLE), y)
SND_SRV := libsndsrv
else
SND_SRV :=
endif

ifeq ($(CONFIG_AMP_COMPONENT_DISPSRV_ENABLE), y)
DISP_SRV := libdispsrv
else
DISP_SRV :=
endif

libdispsrv : libamphal libddlcommon build_amp_libs build_amp_dtvbsp $(SND_SRV) $(DEINT) $(HDCP2X_CA) $(NXG_CA)
	@echo build library $@
	@+$(call generic-make,$@)

libampsrv : build_amp_ddls
	@echo build library $@
	@+$(call generic-make,$@)

libamprpc_server : prerequisite_amp_libs libamphal build_amp_drm build_amp_ddls $(DISP_SRV) $(SND_SRV) $(DEINT)
	@echo build library $@
	@+$(call generic-make,$@)

libgraphics : libampclient
	@echo build library $@
	@+$(call generic-make,$@)

ifeq ($(CONFIG_AMP_AUDIO_SRC_SUPPORTED), y)
SPEEX := libspeexdsp
else
SPEEX :=
endif

ifeq ($(CONFIG_AMP_AUDIO_ZSP_SECURE_ENABLE), y)
SNDSRV_DRM_DEP := libdrmkeymgr
endif

libsndsrv : build_amp_libs libamphal libddlcommon libddlaout $(SPEEX) $(SNDSRV_DRM_DEP)
	@echo build library $@
	@+$(call generic-make,$@)

libdrmcommca: build_amp_libs
	@echo build library $@
	@+$(call generic-make,$@)

ifeq ($(CONFIG_ENABLE_DRM), y)
COMMON_CA := libdrmcommca
endif

libdrmcrypto : build_amp_libs $(COMMON_CA)
	@echo build library $@
	@+$(call generic-make,$@)

DRM_SRV_DEP := build_amp_libs libdrmkeymgr libdrmcrypto
DRM_CLIENT_DEP := libampclient libdebugca

ifeq ($(CONFIG_AMP_IP_DRM_NSK), y)
DRM_CLIENT_DEP += libnskclient
endif

ifeq ($(CONFIG_AMP_IP_DRM_NSK_HARMONIZER), y)
DRM_CLIENT_DEP += libnskharmonizer libnskdssharmonizer
endif

ifeq ($(CONFIG_ENABLE_DRM), y)
DRM_TA := libdrmse.ta librsaengine.ta
else
DRM_TA :=
endif
DRM_PR_DEP := libOSAL prerequisite_amp_libs

ifeq ($(CONFIG_AMP_IP_DRM_PRIKLADDER), y)
DRM_TA += libpvrprikey.ta
endif
ifeq ($(CONFIG_AMP_IP_DRM_VMX_ULTRA), y)
DRM_TA += libvmx_bg.ta libvmxultra.ta
endif
ifeq ($(CONFIG_AMP_IP_DRM_CISCOVSSS), y)
DRM_TA += libciscovsss.ta
endif
ifeq ($(CONFIG_AMP_IP_DRM_PR_25), y)
DRM_TA += libpr25.ta
endif
ifeq ($(CONFIG_AMP_IP_DRM_WV), y)
DRM_TA += libwidevine.ta libutctl.ta
endif

ifeq ($(CONFIG_AMP_IP_DRM_NAGRA), y)
DRM_TA += libnagrakm.ta libnocsbcm.ta libcert.ta libkeygen.ta libTALTS.ta libCCL.ta libplayer_api.ta
DRM_SRV_DEP += libnocsdrv
endif
ifeq ($(CONFIG_ENABLE_DRM), y)
ifneq ($(CONFIG_AMP_DRM_ENG_BCM), y)
DRM_SRV_DEP += libdrmfwmgr libdrmfwmgra1d
endif
endif

ifeq ($(CONFIG_AMP_ENABLE_GKL), y)
DRM_TA += libgkl.ta
endif

ifeq ($(CONFIG_AMP_IP_DRM_NSK), y)
DRM_TA += libnsk.ta
DRM_TA += libnskhdi.ta
endif

ifeq ($(CONFIG_AMP_IP_DRM_IRDETO), y)
	DRM_TA += libirdeto.ta
endif

libdrmsrv: $(DRM_SRV_DEP)
	@echo build library $@
	@+$(call generic-make,$@)

libpr25srv: $(DRM_SRV_DEP)
	@echo build library $@
	@+$(call generic-make,$@)

ifeq ($(CONFIG_AMP_IP_DRM_VMX_ULTRA), y)
libvmxultrasrv: $(DRM_SRV_DEP) libddlcommon
	@echo build library $@
	@+$(call generic-make,$@)
endif

ifeq ($(CONFIG_AMP_IP_DRM_IRDETO), y)
libirdetosrv: $(DRM_SRV_DEP) libddlcommon
	@echo build library $@
	@+$(call generic-make,$@)
endif

libprtest : libampclient libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)

libdrmclient : $(DRM_CLIENT_DEP)
	@echo build library $@
	@+$(call generic-make,$@)

libnskharmonizer: libampclient
	@echo build library $@
	@+$(call generic-make,$@)

#add libnskharmonizer dependency here to avoid paralle compile error.
#in release build those 2 lib share same rel_path, verify_ip failed
#bk.list generated by libnskharmonizer will be deleted by libnskdssharmonizer
libnskdssharmonizer: libampclient libnskharmonizer
	@echo build library $@
	@+$(call generic-make,$@)

libnskclient : libOSAL libampclient libscrd
	@echo build library $@
	@+$(call generic-make,$@)

libplayready12 : libampclient libamphal
	@echo build library $@
	@+$(call generic-make,$@)

liboemcrypto : libampclient libOSAL libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)
liboemcrypto_new : liboemcrypto
	@echo build library $@
	@+$(call generic-make,$@)

libdrmkeymgr: build_amp_libs libamphal
	@echo build library $@
	@+$(call generic-make,$@)
libdrmfwmgr: build_amp_libs
	@echo build library $@
	@+$(call generic-make,$@)

libdrmfwmgra1d: build_amp_libs
	@echo build library $@
	@+$(call generic-make,$@)

ifeq ($(BUILD_TARGET), release)
libpr25client : $(DRM_CLIENT_DEP) libpr25srv
else
libpr25client : $(DRM_CLIENT_DEP)
endif
	@echo build library $@
	@+$(call generic-make,$@)

libnf: libampclient
	@echo build library $@
	@+$(call generic-make,$@)

libsec_ree: $(DRM_CLIENT_DEP)
	@echo build library $@
	@+$(call generic-make,$@)

nv_talts: libsec_ree libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)

libnocsdrv:
	@echo build library $@
	@+$(call generic-make,$@)

ifeq ($(BUILD_TARGET), release)
libnagra_factory: $(DRM_CLIENT_DEP) libsec_ree
else
libnagra_factory: $(DRM_CLIENT_DEP)
endif
	@echo build library $@
	@+$(call generic-make,$@)

dalts: libnagra_factory libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)

nagra_factory_test: libnagra_factory libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)

nagra_stream_test: libsec_ree libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)


libampgpio: prerequisite_amp_libs
	@echo build library $@
	@+$(call generic-make,$@)

libdebugca:
	@echo build library $@
	@+$(call generic-make,$@)

libsimplecas_ca: libampclient
	@echo build library $@
	@+$(call generic-make,$@)

###############################################################################
# TA
###############################################################################

ifeq ($(wildcard  $(AMPSDKTOP)/ta_amp/Makefile),)
amp_ta:
	@echo no ta_amp bypass it
else
amp_ta: $(COMP_TA) $(DRM_TA) $(DISP_TA)
	@echo build library $<
endif

ifneq ($(CONFIG_AMP_TEE_ENABLE), y)
AMP_TA :=
else
AMP_TA := amp_ta
endif

##############################################################################
# TA libs
##############################################################################
libvmeta.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libvmeta.ta.debug:
	@echo build vmeta debug version
	@make -C $(AMPSDKTOP)/build/configs/libvmeta.ta DEBUG_FW=true

libdebug.ta:
	@echo build library $@
	@+$(call generic-make,$@)

ifeq ($(CONFIG_GEN_AMP_AUDIO_ENABLE_ZSP), y)
libzsp.ta: libbridge_otp_ta
	@echo build library $@
	@+$(call generic-make,$@)
libdtscore.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libdtshd.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libsrstruvol.ta:
	@echo build library $@
	@+$(call generic-make,$@)
endif

libdsp.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libaac.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libadec.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libwma.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libdvs.ta: libbridge_otp_ta
	@echo build library $@
	@+$(call generic-make,$@)

libdata_exporter.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libtsp.ta: libbridge_drmse_ta
	@echo build library $@
	@+$(call generic-make,$@)

libm2m.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libdss.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libptm.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libvcap.ta:
	@echo build library $@
	@+$(call generic-make,$@)

###############################################################################
# DRM TA_libs
###############################################################################
libdrm_common_ta:
	@echo build library $@
	@+$(call generic-make,$@)

libbridge_m2m_ta:
	@echo build library $@
	@+$(call generic-make,$@)

libbridge_otp_ta:
	@echo build library $@
	@+$(call generic-make,$@)

libbridge_drmse_ta:
	@echo build library $@
	@+$(call generic-make,$@)

libvpp_ngptv:
	@echo build library $@
	@+$(call generic-make,$@)

libvpp_vmx:
	@echo build library $@
	@+$(call generic-make,$@)

libcisco_bcm:
	@echo build library $@
	@+$(call generic-make,$@)

librng:
	@echo build library $@
	@+$(call generic-make,$@)

libdrmse.ta: libdrm_common_ta libbridge_m2m_ta
	@echo build library $@
	@+$(call generic-make,$@)

libwidevine.ta: libdrm_common_ta libbridge_m2m_ta libbridge_drmse_ta
	@echo build library $@
	@+$(call generic-make,$@)

libutctl.ta: libdrm_common_ta
	@echo build library $@
	@+$(call generic-make,$@)

libpr25.ta: libdrm_common_ta libbridge_m2m_ta
	@echo build library $@
	@+$(call generic-make,$@)

librsaengine.ta: libdrm_common_ta
	@echo build library $@
	@+$(call generic-make,$@)

libpr32.ta: libdrm_common_ta libbridge_m2m_ta
	@echo build library $@
	@+$(call generic-make,$@)

libpr40.ta: libdrm_common_ta libbridge_m2m_ta
	@echo build library $@
	@+$(call generic-make,$@)

libvmx_bg.ta: libdrm_common_ta librng
	@echo build library $@
	@+$(call generic-make,$@)

libvmxultra.ta: libdrm_common_ta libbridge_m2m_ta libbridge_otp_ta
	@echo build library $@
	@+$(call generic-make,$@)

libciscovsss.ta: libcisco_bcm libdrm_common_ta libbridge_m2m_ta libbridge_otp_ta
	@echo build library $@
	@+$(call generic-make,$@)

libpvrprikey.ta: libdrm_common_ta
	@echo build library $@
	@+$(call generic-make,$@)

libcert.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libkeygen.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libnocsbcm.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libnagrakm.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libTALTS.ta: libbridge_m2m_ta
	@echo build library $@
	@+$(call generic-make,$@)

libCCL.ta: libTALTS.ta
	@echo build library $@
	@+$(call generic-make,$@)

libplayer_api.ta: libTALTS.ta
	@echo build library $@
	@+$(call generic-make,$@)

libcpm.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libnsk.ta: libdrm_common_ta
	@echo build library $@
	@+$(call generic-make,$@)

libnskhdi.ta: libdrm_common_ta
	@echo build library $@
	@+$(call generic-make,$@)

ifeq ($(CONFIG_AMP_ENABLE_GKL), y)
libgkl.ta:
	@echo build library $@
	@+$(call generic-make,$@)
endif

libsimplecas.ta: libdrm_common_ta libbridge_drmse_ta
	@echo build library $@
	@+$(call generic-make,$@)

libirdeto.ta: libdrm_common_ta libbridge_drmse_ta
	@echo build library $@
	@+$(call generic-make,$@)

#############################################
# DISP TA libs
#############################################

ifeq ($(CONFIG_AMP_HDMI_HDCP_2X), y)
ifeq ($(SoC_Ver), $(filter $(SoC_Ver), VS680 VS640))
libhdcp2x.ta:
	@echo build library $@
	@+$(call generic-make,$@)
endif
endif

libvpp.ta: libdrm_common_ta libbridge_otp_ta $(HDCP2X_TA) $(NXG_TA) $(VMX_A)
	@echo build library $@
	@+$(call generic-make,$@)

libvpp_recovery.ta: libdrm_common_ta
	@echo build library $@
	@+$(call generic-make,$@)

libdhub.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libovp.ta:
	@echo build library $@
	@+$(call generic-make,$@)

liboutctrl.ta: libdrm_common_ta libbridge_otp_ta
	@echo build library $@
	@+$(call generic-make,$@)

libwatermark.ta:
	@echo build library $@
	@+$(call generic-make,$@)

libfastlogo.ta:
	@echo build library $@
	@+$(call generic-make,$@)

ifeq ($(CONFIG_AMP_LIB_NXG), y)
libnxg_tac:
	@echo build library $@
	@+$(call generic-make,$@)

libnxg.ta: libnxg_tac
	@echo build library $@
	@+$(call generic-make,$@)
endif

###############################################################################
# FW
###############################################################################
amp_fw:
	@echo build fw $@
	@+$(call generic-make,$@)
AMP_FW_DEP := amp_fw

###############################################################################
#application executible here
###############################################################################
ampservice : libamprpc_server libampsrv build_amp_drm $(AMP_TA) $(AMP_FW_DEP)
	@echo build library $@
	@+$(call generic-make,$@)

ampclient_alpha : libampclient libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)

#ampclient_ndi : libampclient libdrmclient
#	@echo build library $@
#	@+$(call generic-make,$@)
#
ampclient_watermark : libampclient libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)

ampclient_ndi : libampclient libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)	
	
ampclient_samples : libampclient libdrmclient $(LIB_TUNERDEMOD) libsimplecas_ca
	@echo build library $@
	@+$(call generic-make,$@)

ampclient_va : libampclient
	@echo build library $@
	@+$(call generic-make,$@)

amp_transcode : libampclient
	@echo build library $@
	@+$(call generic-make,$@)

dolby_certify_app : libampclient
	@echo build library $@
	@+$(call generic-make,$@)

ampclient_pvr : libampclient libdrmclient $(LIB_TUNERDEMOD) libsimplecas_ca
	@echo build library $@
	@+$(call generic-make,$@)

ampclient_customer : libampclient libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)

ampclient_avsettings : libampclient
	@echo build library $@
	@+$(call generic-make,$@)

test_opencrypto : libampclient libdrmclient libdrmcrypto
	@echo build library $@
	@+$(call generic-make,$@)

test_nds : libampclient libdrmclient libdrmcrypto
	@echo build library $@
	@+$(call generic-make,$@)

ampdiag : libampclient
	@echo build library $@
	@+$(call generic-make,$@)

mdb : libampclient libamputil_server
	@echo build library $@
	@+$(call generic-make,$@)

set_plane: libampclient libgraphics
	@echo build library $@
	@+$(call generic-make,$@)

AVIF_Test_APP: libOSAL libamphal libdispsrv
	@echo build library $@
	@+$(call generic-make,$@)

VPP_Test_APP: libOSAL libamphal
	@echo build library $@
	@+$(call generic-make,$@)

prlivetvtest:libampclient libdrmclient libprtest
	@echo build library $@
	@+$(call generic-make,$@)

VIP_Test_APP: libOSAL libamphal
	@echo build library $@
	@+make -C $(AMPSDKTOP)/build/configs/$@ $(BUILD_TARGET)

AIP_Test_APP: libOSAL libamphal
	@echo build library $@
	@+make -C $(AMPSDKTOP)/build/configs/$@ $(BUILD_TARGET)

sstore_calc_hash: libOSAL libampclient libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)

prdrmmanagertest:libampclient libdrmclient libprtest
	@echo build library $@
	@+$(call generic-make,$@)

drmtest : libampclient libdrmclient libplayready12
	@echo build library $@
	@+$(call generic-make,$@)

ifeq ($(CONFIG_AMP_IP_DRM_NAGRA_EFPK), y)
provision : libampclient libdrmclient libnagra_factory
	@echo build library $@
	@+$(call generic-make,$@)
else
provision : libampclient libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)
endif

oemcrypto_test: libampclient libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)

netflix_test: libampclient libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)

oem_hal_simulator: libampclient libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)

test_drm_ipctl : libdrmclient libdrmsrv
	@echo build library $@
	@+$(call generic-make,$@)

test_play_video :libOSAL libampclient libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)

ampdmxutil: libampclient libdebugca
	@echo ampdmxutil_dep
	@echo build library $@
	@+$(call generic-make,$@)

ampdmxtest: libampclient libdrmclient libdebugca
	@echo build library $@
	@+$(call generic-make,$@)

ampisptest: libampclient
	@echo build library $@
	@+$(call generic-make,$@)

tsp_m2m:
	@echo build bin $@
	@+$(call generic-make,$@)

amppvrtest: libampclient libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)

ampshmtest:
	@echo build library $@
	@+$(call generic-make,$@)

ampclient_headfilechecker:
	@echo build library $@
	@+$(call generic-make,$@)

amp_pm: libampclient
	@echo build library $@
	@+$(call generic-make,$@)

ISPBE_Test_APP: libOSAL libamphal
	@echo build library $@
	@+$(call generic-make,$@)


###############################################################################
#define tester here
###############################################################################
test_vdec : libddlvdec
	@echo build library $@
	@+$(call generic-make,$@)

test_vmeta_sched :
	@echo build library $@
	@+$(call generic-make,$@)

test_vout : libddlvout libdispsrv libddlvdec
	@echo build library $@
	@+$(call generic-make,$@)

unit_test:
	@echo build unit test apps
	@+$(call generic-make,$(AMPSDKTOP)/amp/test/unit_test)

test_adec : libddladec
	@echo build library $@
	@+$(call generic-make,$@)

test_disp : libdispsrv libampclient
	@echo build library $@
	@+$(call generic-make,$@)

amp_iptv_channel_zapping : libampclient
	@echo build library $@
	@+$(call generic-make,$@)

hdmi_samples : libampclient
	@echo build library $@
	@+$(call generic-make,$@)

test_disp_lvds : libdispsrv
	@echo build library $@
	@+$(call generic-make,$@)

test_avin : libddlavin
	@echo build library $@
	@+$(call generic-make,$@)

test_venc : libddlvenc libddlvdec
	@echo build library $@
	@+$(call generic-make,$@)

test_gettemp : libsm
	@echo build library $@
	@+$(call generic-make,$@)

test_sm : libsm
	@echo build library $@
	@+$(call generic-make,$@)

test_scrd : libscrd libampclient
	@echo build library $@
	@+$(call generic-make,$@)

test_i2c : libi2c
	@echo build library $@
	@+$(call generic-make,$@)

###############################################################################
#add unit test binary here
###############################################################################
test_monitord : libamputil_server
	@echo build library $@
	@+$(call generic-make,$(AMPSDKTOP)/amp/src/libs/test,-f test_monitor.mak all)

###############################################################################
#define peripherals here
###############################################################################
libdtvbsp: libOSAL libamphal libi2c
	@echo build library $@
	@+$(call generic-make,$@)

libcypress: libdtvbsp
	@echo build library $@
	@+$(call generic-make,$@)

libfrc:
	@echo build library $@
	@+$(call generic-make,$@)

test_frc: libfrc
	@echo build library $@
	@+$(call generic-make,$@)

ifeq ($(CONFIG_GEN_AMP_AUDIO_ENABLE_ZSP), y)
test_azsp : libamphal
	@echo build library $@
	@+$(call generic-make,$@)
endif

libtas5727: libdtvbsp
	@echo build library $@
	@+$(call generic-make,$@)

test_tas5727: libtas5727 libdtvbsp
	@echo build excutable $@
	@+$(call generic-make,$@)

libdvsplit: libOSAL
	@echo build library $@
	@+$(call generic-make,$@)

###############################################################################
#define middleware libraries here
###############################################################################
libedid : libamphal
	@echo build library $@
	@+$(call generic-make,$@)

libcecsvs : libsm libampclient
	@echo build library $@
	@+$(call generic-make,$@)

test_cecsvs : libampclient libcecsvs
	@echo build excutable $@
	@+$(call generic-make,$@)

amp_sbt_test: libampclient
	@echo build library $@
	@+$(call generic-make,$@)

test_ovp:   libamphal libdispsrv
	@echo build library $@
	@+$(call generic-make,$@)

libberlinpm:
	@echo build library $@
	@+$(call generic-make,$@)

berlin_pm: libberlinpm
	@echo build executable $@
	@+$(call generic-make,$@)

###############################################################################
#define graphics examples here
###############################################################################
gfx_examples : libgraphics libampclient libOSAL
	@echo build library $@
	@+$(call generic-make,$@)

###############################################################################
#define AMP test cases based on Google Test framework here
###############################################################################
libampgtest :
	@echo build library $@
	@+$(call generic-make,$@)

gtest_example : libampgtest
	@echo build library $@
	@+$(call generic-make,$@)

AmpUnitTest : libampgtest libOSAL libampclient
	@echo build library $@
	@+$(call generic-make,$@)

AmpPipeTest : libampgtest libOSAL libampclient
	@echo build library $@
	@+$(call generic-make,$@)

AmpIPControlTest : libOSAL libampclient libdrmclient libdrmsrv
	@echo build library $@
	@+$(call generic-make,$@)
###############################################################################
#define DRM test cases based on Google Test framwork here
###############################################################################
DrmUnitTest : libampgtest libOSAL libampclient libdrmclient
	@echo build library $@
	@+$(call generic-make,$@)

libtunerdemod :
	@echo build library $@
	@+$(call generic-make,$@)

librma_local : libOSAL libamputil_server libshmserver
	@echo build library $@
	@+$(call generic-make,$@)

test_rma_local : libOSAL libamputil_server libshmserver
	@echo build library $@
	@+$(call generic-make,$@)

ipctest  : libOSAL libamputil_server libshmserver
	@echo build library $@
	@+$(call generic-make,$@)

test_dsp : libOSAL libamputil_server libshmserver
	@echo build library $@
	@+$(call generic-make,$@)

ms12_iidk_cert : libOSAL libamputil_server libshmserver
	@echo build library $@
	@+$(call generic-make,$@)

NskClientTest: libOSAL libnskclient libnskharmonizer
	@echo build library $@
	@+$(call generic-make,$@)

IrdetoTest: libOSAL libdrmclient libdrmsrv
	@echo build library $@
	@+$(call generic-make,$@)
endif ###LIBRARY_DEPS_MAK###
