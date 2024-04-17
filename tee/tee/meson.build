project('libteec', 'c',
        license: 'proprietary',
        version: '1.0.7')

thread_dep = dependency('threads')

comm_inc = include_directories('include')

tz_inc = include_directories(
    'tz/common',
    'tz/client_api',
    'tz/nw_api'
)

tz_client_file = files('tz/client_api/tz_client_api_user.c')

subdir('tee')

install_subdir('include',
    strip_directory : true,
    install_dir : get_option('includedir') / 'tee'
)
