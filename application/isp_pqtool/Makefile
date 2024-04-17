MOD=isp_pqtool
all: $(MOD)

$(MOD): source/interface/isp_pqctrl_main.cpp
	$(CXX) $(E_CFLAG) $(MODULE_CFLAGS) $(E_LDFLAG) $(LDFLAGS) -o $(BUILD_DIR)/$@ $<

clean:
	@-rm $(BUILD_DIR)/$(MOD)

install:
	cp $(BUILD_DIR)/$(MOD) $(APP_DIR)

.PHONY: clean all install

