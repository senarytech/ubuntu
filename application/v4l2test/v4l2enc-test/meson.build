syna_v4l2_enc_test_src_arr = [
    'v4l2enc_test.c'
]

executable('syna-v4l2-enc-test',
    syna_v4l2_enc_test_src_arr,
    dependencies : [syna_v4l2_dep, thread_dep],
    install_dir : get_option('libexecdir') / 'syna',
    install : true)
