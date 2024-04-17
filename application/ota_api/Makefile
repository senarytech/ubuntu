MOD=libota_api.so
all: $(MOD)

######################################################
# Module SRC
SRCS = $(wildcard src/*.c)

######################################################
# Module OBJ
DIR_OBJ = $(BUILD_DIR)/obj
OBJS = $(patsubst %.c,$(DIR_OBJ)/%.o,$(notdir $(SRCS)))

######################################################
# Module Public header files
PUBLIC_HEADER_FILES =
PUBLIC_HEADER_FILES += inc/emmc_api.h
PUBLIC_HEADER_FILES += inc/emmc_partition.h

### MODULE CFLAGS/LDFLAGS ###

INC_CFLAG := -Iinc \
             -Isrc \

LINK_FLAG := $(E_LDFLAG)

clean:
	@-rm $(BUILD_DIR)/$(MOD) $(DIR_OBJ)/*.o

install:
	@mkdir -p $(LIB_DIR)
	@cp -av $(BUILD_DIR)/$(MOD) $(LIB_DIR)
	@mkdir -p $(SYSROOT_DIR)/lib
	@cp -av $(BUILD_DIR)/$(MOD) $(SYSROOT_DIR)/lib
	@mkdir -p $(SYSROOT_DIR)/include
	@cp -av $(PUBLIC_HEADER_FILES) $(SYSROOT_DIR)/include/

.PHONY: clean all install


$(DIR_OBJ)/%.o:src/%.c
	mkdir -p $(DIR_OBJ)
	$(CC) -fPIC $(INC_CFLAG) -D__LINUX__ -Werror  $(E_CFLAG) -c -o $@ $<


$(MOD): $(OBJS)
	$(CC) -shared -Wl,-soname=$(MOD) $(E_CFLAG) $(LINK_FLAG)  -o $(BUILD_DIR)/$@ $^
