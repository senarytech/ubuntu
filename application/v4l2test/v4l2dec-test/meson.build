syna_v4l2_dec_test_src_arr = [
    'h264_auparser.c',
    'tsemaphore.c',
    'v4l2dec.c',
    'v4l2dec_test.c'
]

executable('syna-v4l2-dec-test',
    syna_v4l2_dec_test_src_arr,
    dependencies : [syna_v4l2_dep, thread_dep],
    install_dir : get_option('libexecdir') / 'syna',
    install : true)
