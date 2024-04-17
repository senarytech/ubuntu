sources = [
    'tee_ca_daemon.c',
    'ree_sys_callback_ipc_server.c',
    'ree_ca_rpmb_op.c',
    'ree_ca_file_op.c',
    'rpmb.c'
]

inc = include_directories('build/ca')

executable('tee_daemon',
    sources,
    include_directories : inc,
    dependencies : teec_dep,
    install : true,
    install_dir : get_option('sbindir')
)
