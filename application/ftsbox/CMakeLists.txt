cmake_minimum_required(VERSION 3.15)

set (CMAKE_SYSTEM_NAME "Linux")
set (CMAKE_SYSTEM_PROCESSOR "aarch64")
set (CMAKE_C_COMPILER "/usr/bin/aarch64-linux-gnu-gcc")
set (CMAKE_CXX_COMPILER "/usr/bin/aarch64-linux-gnu-g++")

project(infotool)

#add_compile_options(-g)   //only for gdb debug.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(_GLIBCXX_USE_CXX11_ABI 1)

set(SRC_LIST main_devinfo.cpp log.cpp)

add_executable(infotool ${SRC_LIST})

