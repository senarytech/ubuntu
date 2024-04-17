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
TEE_DEV_TEEI ?= GPD11_TZK530

CROSS_COMPILE	:= arm-eabi-
CC		:= $(CROSS_COMPILE)gcc
OBJDUMP		:= $(CROSS_COMPILE)objdump
OBJCOPY		:= $(CROSS_COMPILE)objcopy
LD		:= $(CROSS_COMPILE)ld
AR		:= $(CROSS_COMPILE)ar
STRIP		:= $(CROSS_COMPILE)strip --strip-unneeded --keep-file-symbols

CFLAGS		= -Os -fno-builtin -msoft-float -march=armv7-a
CFLAGS		+= -Wall -Wundef -Wstrict-prototypes
CFLAGS		+= -fno-strict-aliasing -fno-common
CFLAGS		+= -fPIC -D__TRUSTEDAPP__
CFLAGS		+= $(addprefix -I, $(sort $(dir $(objs))))
CFLAGS		+= $(addprefix -I, $(TA_C_INCLUDES))
CFLAGS		+= -I. -I$(TOP_PATH)/teei/$(TEE_DEV_TEEI)/include/
CFLAGS		+= -I$(TOP_PATH)/lib/openssl/include
CFLAGS		+= -I$(TOP_PATH)/lib/newlib/include/
CFLAGS		+= -fstack-protector-strong

LDFLAGS		:= -nostdlib -L $(TOP_PATH)/teei/$(TEE_DEV_TEEI)/lib -lteei

PLATFORM_LIBS	:= -L $(TOP_PATH)/lib/openssl/lib -lssl -lcrypto
PLATFORM_LIBS	+= -L $(TOP_PATH)/lib/newlib/arm-eabi/lib -lm -lc -lgloss
PLATFORM_LIBS	+= -L $(shell dirname `$(CC) $(CFLAGS) -print-libgcc-file-name`) -lgcc

LDFLAGS		+= -T $(TOP_PATH)/build/ta.lds
LDFLAGS		+= -Ttext 0x100

objs		:= $(patsubst %.c,%.o,$(TA_SRC_FILES))

all: $(TA_MODULE)
%.o: %.c
	@echo CC    $<
	@$(CC) -c $(CFLAGS) $(TA_CFLAGS) $< -o $@

$(TA_MODULE).clear: $(objs) $(TOP_PATH)/teei/$(TEE_DEV_TEEI)/lib/crt0.o
	@echo LD    $@
	@$(CC) -shared -Wl,-soname,$(TA_MODULE) -o $@ $^ $(TA_LDFLAGS) $(LDFLAGS) $(PLATFORM_LIBS)
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

clean:
	rm -f lib*.ta* $(objs)
