all: spi_test

spi_test: spidev_test.c
	$(CC) $(E_CFLAG) $(E_LDFLAG) -o $(BUILD_DIR)/$@ $^

clean:
	@-rm $(BUILD_DIR)/spi_test

install:
	cp $(BUILD_DIR)/spi_test $(APP_DIR)

.PHONY: clean all install
