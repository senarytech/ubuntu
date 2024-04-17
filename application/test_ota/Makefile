all: test_ota
test_ota: test_ota.c
	$(CC) $(E_CFLAG) $(E_LDFLAG) -o $(BUILD_DIR)/$@ $^

clean:
	@-rm $(BUILD_DIR)/test_ota

install:
	cp $(BUILD_DIR)/test_ota $(APP_DIR)

.PHONY: clean all install
