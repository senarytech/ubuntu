all: test_standby

test_standby: test_standby.c
	$(CC) -D__LINUX__ $(E_CFLAG) $(E_LDFLAG)  -o $(BUILD_DIR)/$@ $^ -lOSAL -lampclient -lpthread -lm

clean:
	@-rm $(BUILD_DIR)/test_standby

install:
	cp $(BUILD_DIR)/test_standby $(APP_DIR)

.PHONY: clean all install
