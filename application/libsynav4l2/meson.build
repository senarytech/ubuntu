project('syna-v4l2-emulation', 'c', 'cpp',
    version: '1.10.8',
    meson_version : '>= 0.58',
    default_options : [
        'buildtype=debugoptimized',
        'warning_level=1',
        'c_std=gnu99',
        'cpp_std=gnu++14',
    ],
    license : 'Proprietary')

cc = meson.get_compiler('c')
cxx = meson.get_compiler('cpp')

have_visibility = cc.has_argument('-fvisibility=hidden') and \
                  cc.has_function_attribute('visibility:default')

pkgconfig = import('pkgconfig')
as_version = meson.project_version()

thread_dep = dependency('threads')

dep_libdl = cc.find_library('dl')

ampclient_dep = dependency('syna-ampclient')
amp_osal_dep = dependency('syna-amposal')
tinyxml2_dep = dependency('tinyxml2')

syna_v4l2_incdir_arr = [
    'inc',
    'src'
]

syna_v4l2_src_arr = [
    'src/cam/CamBuffer.cpp',
    'src/cam/CamCompIntf.cpp',
    'src/cam/cam_config_parser.cpp',
    'src/cam/cam_config_reader.cpp',
    'src/cam/CamDewarpComp.cpp',
    'src/cam/CamDnsComp.cpp',
    'src/cam/CamIspComp.cpp',
    'src/cam/CamPipeline.cpp',
    'src/cam/CamRotateComp.cpp',
    'src/cam/cam_stream.cpp',
    'src/v4l2_api.c',
    'src/v4l2_buffer.c',
    'src/v4l2_event.c',
    'src/v4l2_ioctrl.c',
    'src/v4l2_m2m.c',
    'src/v4l2_m2m_cam.c',
    'src/v4l2_m2m_dec.c',
    'src/v4l2_m2m_enc.c',
    'src/v4l2_misc.c',
    'src/v4l2_stream.c'
]

syna_v4l2 = library('synav4l2',
    syna_v4l2_src_arr,
    include_directories : syna_v4l2_incdir_arr,
    dependencies : [ampclient_dep, amp_osal_dep, tinyxml2_dep],
    version : as_version,
    install : true
)

install_headers('inc/v4l2_types.h', 'inc/v4l2_api.h', subdir : 'syna')

pkgconfig.generate(syna_v4l2, subdirs : 'syna')
