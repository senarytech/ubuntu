all: gpio

gpio: gpio.c
	$(CC) $(E_CFLAG) $(E_LDFLAG) -lampgpio -o $(BUILD_DIR)/$@ $^

clean:
	@-rm $(BUILD_DIR)/gpio

install:
	cp $(BUILD_DIR)/gpio $(APP_DIR)

.PHONY: clean all install
