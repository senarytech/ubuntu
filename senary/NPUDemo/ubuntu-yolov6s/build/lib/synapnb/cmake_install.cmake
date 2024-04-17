# Install script for directory: /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/lib/synapnb

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsynapnb.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsynapnb.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsynapnb.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build/lib/synapnb/libsynapnb.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsynapnb.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsynapnb.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsynapnb.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/synap/allocator.hpp;/synap/buffer.hpp;/synap/buffer_cache.hpp;/synap/network.hpp;/synap/npu.hpp;/synap/tensor.hpp;/synap/types.hpp")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/synap" TYPE FILE FILES
    "/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/lib/synapnb/inc/synap/allocator.hpp"
    "/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/lib/synapnb/inc/synap/buffer.hpp"
    "/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/lib/synapnb/inc/synap/buffer_cache.hpp"
    "/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/lib/synapnb/inc/synap/network.hpp"
    "/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/lib/synapnb/inc/synap/npu.hpp"
    "/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/lib/synapnb/inc/synap/tensor.hpp"
    "/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/lib/synapnb/inc/synap/types.hpp"
    )
endif()

