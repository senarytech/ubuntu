# Install script for directory: /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/install")
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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_device/cmake_install.cmake")
  include("/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_utils/cmake_install.cmake")
  include("/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/nlohmann/cmake_install.cmake")
  include("/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_base/cmake_install.cmake")
  include("/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb/cmake_install.cmake")
  include("/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_app/cmake_install.cmake")
  include("/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_img/cmake_install.cmake")
  include("/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_preprocess/cmake_install.cmake")
  include("/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_postprocess/cmake_install.cmake")
  include("/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/app/synap_cli/cmake_install.cmake")
  include("/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/app/synap_cli_ic/cmake_install.cmake")
  include("/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/app/synap_cli_od/cmake_install.cmake")
  include("/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/app/synap_cli_yolov5/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
