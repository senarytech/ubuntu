MOD=watch_dog
all: $(MOD)

SRC=$(wildcard src/*.c)
$(MOD): $(SRC)
	$(CC) -I./include $(E_CFLAG) $(E_LDFLAG) -DVERSION=\"1.00.0\" -DHAS_SM -D__LINUX__ -lsm -o $(BUILD_DIR)/$@ $^

clean:
	@-rm $(BUILD_DIR)/$(MOD)

install:
	cp $(BUILD_DIR)/$(MOD) $(APP_DIR)

.PHONY: clean all install
