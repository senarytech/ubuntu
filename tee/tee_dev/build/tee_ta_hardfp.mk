TOP_PATH	:= $(TEE_TOP_REL_PATH)
TOOLS_PATH	:= $(TOP_PATH)/tools

#------------------------------------------------------
#Instruction for KEY_TYPE:
#1) bg2q:	romkey, Internal development
#		custkey, Customer TA
#2) bg3cda0:	mid0x3
#3) bg4ct:	0x0, Internal development
#		0x1, google
#		0x3, non-Verimatrix
#		0x7, Verimatrix
#-------------------------------------------------------
KEY_TYPE	?= romkey
#-------------------------------------------------------
#SoC_Ver means the chip type:bg2q, bg3cda0, bg4cd, bg4ct
#-------------------------------------------------------
SoC_Ver		?= bg2q
#-----------------------------------------------------------
#MODULE just means for chip version higher than BG4CT, BG4CD
#1) bg2q:	MVLTA (no use)
#2) bg4ct:	MVLTA, Marvell TA
#		CUSTA, Custom TA
#		FW, tsp/zsp firmware
#3) bg4cd:
#-----------------------------------------------------------
MODULE		?= MVLTA

CROSS_COMPILE	:= arm-eabi-
CC		:= $(CROSS_COMPILE)gcc
OBJDUMP		:= $(CROSS_COMPILE)objdump
OBJCOPY		:= $(CROSS_COMPILE)objcopy
LD		:= $(CROSS_COMPILE)ld
AR		:= $(CROSS_COMPILE)ar
STRIP		:= $(CROSS_COMPILE)strip --strip-unneeded --keep-file-symbols

CFLAGS		= -Os -nostdlib -fno-builtin -mfloat-abi=hard -march=armv7-a
# FIXME: need replace -Wall with -Werror once the warnings are clean
CFLAGS		+= -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -Wno-format-security
# CFLAGS	+= -Werror-implicit-function-declaration
CFLAGS		+= -fno-strict-aliasing -fno-common -fno-delete-null-pointer-checks
CFLAGS		+= -fPIC -DCONFIG_SO -D__TRUSTEDAPP__
CFLAGS		+= $(addprefix -I, $(sort $(dir $(objs))))
CFLAGS		+= $(addprefix -I, $(TA_C_INCLUDES))
CFLAGS		+= -I. -I$(TOP_PATH)/include/
CFLAGS		+= -I$(TOP_PATH)/lib/openssl/include
CFLAGS		+= -I$(TOP_PATH)/lib/newlib/include/
CFLAGS		+= -fstack-protector

LDFLAGS		:= -L $(TOP_PATH)/lib -lteei_hardfp

PLATFORM_LIBS	:= -L $(TOP_PATH)/lib/openssl/lib -lssl -lcrypto
PLATFORM_LIBS	+= -L $(TOP_PATH)/lib/newlib/arm-eabi-hardfp/lib -lm -lc -lgloss_hardfp
PLATFORM_LIBS	+= -L $(shell dirname `$(CC) $(CFLAGS) -print-libgcc-file-name`)/v7-a/fpv3/hard -lgcc


objs		:= $(patsubst %.c,%.o,$(TA_SRC_FILES))

all: $(TA_MODULE)
%.o: %.c
	@echo CC    $<
	@$(CC) -c -g $(CFLAGS) $(TA_CFLAGS) $< -o $@

$(TA_MODULE).clear: $(objs) $(TOP_PATH)/lib/crt0.o
	@echo LD    $@
	@$(CC) -g $(CFLAGS) $(TA_CFLAGS) -shared -Wl,-soname,$(TA_MODULE) -o $@ $^ $(TA_LDFLAGS) $(LDFLAGS) $(PLATFORM_LIBS)
	@cp $@ $@.unstripped
	@$(OBJDUMP) -xdS $@ > $@.dmp
	$(STRIP) $@
ifneq ($(strip $(TA_INSTALL_PATH)),)
ifneq ($(strip $(TA_INSTALL_NAME)),)
	@cp $@ $(TA_INSTALL_PATH)/$(TA_INSTALL_NAME)
endif
endif

ifneq ($(strip $(SDK_CONFIG)),)
ENC_TA_FLAGS := -c $(SDK_CONFIG)
endif


$(TA_MODULE):$(TA_MODULE).clear
	$(TOOLS_PATH)/enc_tool/enc_ta -i $@.clear -o $@ -k $(KEY_TYPE) \
		-v $(SoC_Ver) -m $(MODULE) $(ENC_TA_FLAGS)

clean:
	rm -f lib*.ta* $(objs)