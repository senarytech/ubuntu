include $(TEE_TOP_REL_PATH)/build/tee/tee_daemon.mk

APP_CROSS_COMPILE ?= armv7a-cros-linux-gnueabi-
CC	= $(APP_CROSS_COMPILE)gcc
OBJDUMP	= $(APP_CROSS_COMPILE)objdump
OBJCOPY	= $(APP_CROSS_COMPILE)objcopy
LD	= $(APP_CROSS_COMPILE)ld
STRIP	= $(APP_CROSS_COMPILE)strip
AR	= $(APP_CROSS_COMPILE)ar rc

LIBS	= -lpthread -lrt
CFLAGS	= $(TEEC_CFLAGS)
CFLAGS	+= -I. $(addprefix -I, $(sort $(dir $(TEEC_REL_INCLUDES))))
CFLAGS	+= $(addprefix -I, $(CA_C_INCLUDES))

CFILES	:= $(CA_SRC_FILES) $(TEEC_REL_CFILES)
COBJS	:= $(patsubst %.c,%.o,$(CFILES))

all: $(CA_MODULE)

$(CA_MODULE): $(CFILES)
	$(CC) $(CFLAGS) $(CA_CFLAGS) -o $@ $^ $(CA_LDFLAGS) $(LIBS)
	$(STRIP) $@

lib$(CA_MODULE).so: $(CFILES)
	$(CC) $(CFLAGS) $(CA_CFLAGS) -fPIC -shared -o $@ $^ $(CA_LDFLAGS) $(LIBS)
	$(STRIP) $@

lib$(CA_MODULE).a: $(COBJS)
	$(AR) $@ $^
	$(STRIP) $@

.c.o:
	$(CC) $(CFLAGS) $(CA_CFLAGS) -c -o $@ $<

clean:
	rm -f $(COBJS)
	rm -f $(CA_MODULE)
	rm -f lib$(CA_MODULE).*
