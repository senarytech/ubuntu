project('syna-v4l2-emulation-test', 'c', 'cpp',
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

pkgconfig = import('pkgconfig')

thread_dep = dependency('threads')
syna_v4l2_dep = dependency('synav4l2')

subdir('v4l2enc-test')
subdir('v4l2dec-test')
