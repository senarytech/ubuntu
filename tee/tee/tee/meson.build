pkg = import('pkgconfig')

mgr_src = [
    'mgr/tee_ca_mgr_cmd.c',
    'mgr/tee_ca_sys_cmd.c'
]

ree_src = [
    'ree/ree_sys_callback_user.c',
    'ree/ree_sys_callback_ipc_client.c'
]

teec_inc = include_directories(
    'client_api',
    'common',
    'ree',
    'internal_api'
)

teec_i_inc = include_directories(
    'client_api/build',
    'mgr'
)

teec_lib = library('teec',
    sources: [
        tz_client_file,
        mgr_src,
        ree_src,
        'client_api/tee_client_api.c',
        'client_api/tee_client_util.c'
    ],
    c_args : ['-DCONFIG_TEE'],
    version : meson.project_version(),
    soversion : '1',
    include_directories : [comm_inc, tz_inc, teec_inc, teec_i_inc],
    dependencies : thread_dep,
    install : true
)

teec_dep = declare_dependency(link_with : teec_lib,
    include_directories : [comm_inc, tz_inc, teec_inc]
)

install_headers([
    'client_api/tee_client_api.h',
    'client_api/tee_client_config.h',
    'client_api/tee_client_const.h',
    'client_api/tee_client_type.h',
    'client_api/tee_client_util.h',
    'common/tee_common.h',
    ],
    subdir : 'tee'
)

# I don't want install those internal header files, but hdcp2x, vpp request them
install_subdir('internal_api',
    strip_directory: true,
    install_dir : get_option('includedir') / 'tee'
)

pkg.generate(teec_lib,
    filebase : 'libteec',
    subdirs : 'tee',
    description : 'TEE client library'
)

subdir('app')
