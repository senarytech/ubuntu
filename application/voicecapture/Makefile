MOD=voicecapture
all: $(MOD)

$(MOD): voicecapture-hidraw.c
	$(CC) $(E_CFLAG) $(E_LDFLAG) -lcutils -o $(BUILD_DIR)/$@ $^

clean:
	@-rm $(BUILD_DIR)/$(MOD)

install:
	cp $(BUILD_DIR)/$(MOD) $(APP_DIR)

.PHONY: clean all install
