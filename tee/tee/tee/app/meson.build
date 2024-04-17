fs = import('fs')

if fs.is_dir('tee_daemon')
    subdir('tee_daemon')
endif

if fs.is_dir('rpmb_pair_tool')
    rpmbpair_src = [
        'rpmb_pair_tool/tee_ca_main.c',
        'rpmb_pair_tool/tee_ca_pair.c'
    ]

    executable('rpmbpair',
        rpmbpair_src,
        include_directories : include_directories('rpmb_pair_tool/build/ca'),
        dependencies : teec_dep,
        install : true,
        install_dir : get_option('sbindir')
    )
endif
