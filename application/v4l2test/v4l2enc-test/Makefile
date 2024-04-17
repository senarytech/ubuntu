target=v4l2enc-test
target_srcs=v4l2enc_test.c
MODULE_LDFLAGS=-lsynav4l2 -pthread

$(target): $(target_srcs)
	@$(CC) $(E_CFLAG) $(MODULE_CFLAGS) \
		$(MODULE_LDFLAGS) $(E_LDFLAG) $(LDFLAGS) -o $(BUILD_DIR)/$@ $^
