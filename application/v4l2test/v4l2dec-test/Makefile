target=v4l2dec-test
target_srcs=v4l2dec_test.c v4l2dec.c h264_auparser.c tsemaphore.c
MODULE_LDFLAGS=-lsynav4l2 -pthread

$(target): $(target_srcs)
	@$(CC) $(E_CFLAG) $(MODULE_CFLAGS) \
		$(MODULE_LDFLAGS) $(E_LDFLAG) $(LDFLAGS) -o $(BUILD_DIR)/$@ $^
