MOD=libsynav4l2.so
all: $(MOD)

######################################################
# Module SRC
SRCS = $(wildcard src/*.c)
CPPSRCS = $(wildcard src/cam/*.cpp)

######################################################
# Module OBJ
DIR_OBJ = $(BUILD_DIR)/obj
OBJS = $(patsubst %.c,$(DIR_OBJ)/%.o,$(notdir $(SRCS)))
CPPOBJS = $(patsubst %.cpp,$(DIR_OBJ)/cpp/%.o,$(notdir $(CPPSRCS)))

######################################################
# Module Public header files
PUBLIC_HEADER_FILES =
PUBLIC_HEADER_FILES += 	inc/v4l2_api.h

### MODULE CFLAGS/LDFLAGS ###
INC_CFLAG := -Iinc \
             -Isrc \
              -I$(SYSROOT_DIR)/include/osal \
			  -I$(SYSROOT_DIR)/include/amp \
			  -I$(SYSROOT_DIR)/include

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

###############################################################################
# cppcheck related
###############################################################################
ifeq ($(AMP_CPPCHECK), on)
CPPC_MODULE_CFLAGS_1=$(MODULE_CFLAGS) $(Galois_CFLAGS)
CPPC_MODULE_CFLAGS_2=$(filter -D%,,$(CPPC_MODULE_CFLAGS_1))
CPPC_MODULE_CFLAGS_3=$(filter -I%,,$(CPPC_MODULE_CFLAGS_2))
CPPC_MODULE_CFLAGS=$(CPPC_MODULE_CFLAGS_2) $(CPPC_MODULE_CFLAGS_3)
CPPC_PARAM=--enable=warning --platform=unix32 --template=gcc -q
CPPC_PARAM_C=$(CPPC_PARAM) --std=c99
CPPC_PARAM_CPP=$(CPPC_PARAM) --std=c++03
endif

$(DIR_OBJ)/%.o:src/%.c
	mkdir -p $(DIR_OBJ)
	$(CC) -fPIC $(INC_CFLAG) -D__LINUX__ -Werror  $(E_CFLAG) -c -o $@ $<
ifeq ($(AMP_CPPCHECK), on)
	$(Q)cppcheck $(CPPC_PARAM_C) $(CPPC_MODULE_CFLAGS) $< 2>&1 | tee -a $(BUILD_DIR)/cppcheck_report
endif

$(DIR_OBJ)/cpp/%.o:src/cam/%.cpp
	mkdir -p $(DIR_OBJ)/cpp/
	$(CXX) -fPIC $(INC_CFLAG) -D__LINUX__ -Werror  $(E_CFLAG) -c -o $@ $<
ifeq ($(AMP_CPPCHECK), on)
	$(Q)cppcheck $(CPPC_PARAM_C) $(CPPC_MODULE_CFLAGS) $< 2>&1 | tee -a $(BUILD_DIR)/cppcheck_report
endif

$(MOD): $(OBJS) $(CPPOBJS)
	$(CXX) -shared -Wl,-soname=$(MOD) $(E_CFLAG) $(LINK_FLAG)  -o $(BUILD_DIR)/$@ $^
