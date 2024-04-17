all: smbox

smbox: smbox.c
	$(CC) -D__LINUX__ $(E_CFLAG) $(E_LDFLAG)  -o $(BUILD_DIR)/$@ $^ -lsm

clean:
	@-rm $(BUILD_DIR)/smbox

install:
	cp $(BUILD_DIR)/smbox $(APP_DIR)

.PHONY: clean all install
