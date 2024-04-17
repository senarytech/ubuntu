MOD=v4l2test

all: v4l2dec-test v4l2enc-test

v4l2dec-test:
	$(MAKE) -C $@

v4l2enc-test:
	$(MAKE) -C $@

clean:
	@-rm -r $(BUILD_DIR)

install:
	@install -m 0755 $(BUILD_DIR)/v4l2dec-test $(APP_DIR)
	@install -m 0755 $(BUILD_DIR)/v4l2enc-test $(APP_DIR)

.PHONY: clean all install v4l2dec-test v4l2enc-test
