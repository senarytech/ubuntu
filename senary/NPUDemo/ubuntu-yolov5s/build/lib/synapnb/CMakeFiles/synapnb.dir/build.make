# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build

# Include any dependencies generated for this target.
include lib/synapnb/CMakeFiles/synapnb.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/synapnb/CMakeFiles/synapnb.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/synapnb/CMakeFiles/synapnb.dir/progress.make

# Include the compile flags for this target's objects.
include lib/synapnb/CMakeFiles/synapnb.dir/flags.make

lib/synapnb/CMakeFiles/synapnb.dir/src/allocator_synap.cpp.o: lib/synapnb/CMakeFiles/synapnb.dir/flags.make
lib/synapnb/CMakeFiles/synapnb.dir/src/allocator_synap.cpp.o: ../lib/synapnb/src/allocator_synap.cpp
lib/synapnb/CMakeFiles/synapnb.dir/src/allocator_synap.cpp.o: lib/synapnb/CMakeFiles/synapnb.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/synapnb/CMakeFiles/synapnb.dir/src/allocator_synap.cpp.o"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/synapnb/CMakeFiles/synapnb.dir/src/allocator_synap.cpp.o -MF CMakeFiles/synapnb.dir/src/allocator_synap.cpp.o.d -o CMakeFiles/synapnb.dir/src/allocator_synap.cpp.o -c /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/allocator_synap.cpp

lib/synapnb/CMakeFiles/synapnb.dir/src/allocator_synap.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/synapnb.dir/src/allocator_synap.cpp.i"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/allocator_synap.cpp > CMakeFiles/synapnb.dir/src/allocator_synap.cpp.i

lib/synapnb/CMakeFiles/synapnb.dir/src/allocator_synap.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/synapnb.dir/src/allocator_synap.cpp.s"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/allocator_synap.cpp -o CMakeFiles/synapnb.dir/src/allocator_synap.cpp.s

lib/synapnb/CMakeFiles/synapnb.dir/src/buffer.cpp.o: lib/synapnb/CMakeFiles/synapnb.dir/flags.make
lib/synapnb/CMakeFiles/synapnb.dir/src/buffer.cpp.o: ../lib/synapnb/src/buffer.cpp
lib/synapnb/CMakeFiles/synapnb.dir/src/buffer.cpp.o: lib/synapnb/CMakeFiles/synapnb.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object lib/synapnb/CMakeFiles/synapnb.dir/src/buffer.cpp.o"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/synapnb/CMakeFiles/synapnb.dir/src/buffer.cpp.o -MF CMakeFiles/synapnb.dir/src/buffer.cpp.o.d -o CMakeFiles/synapnb.dir/src/buffer.cpp.o -c /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/buffer.cpp

lib/synapnb/CMakeFiles/synapnb.dir/src/buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/synapnb.dir/src/buffer.cpp.i"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/buffer.cpp > CMakeFiles/synapnb.dir/src/buffer.cpp.i

lib/synapnb/CMakeFiles/synapnb.dir/src/buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/synapnb.dir/src/buffer.cpp.s"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/buffer.cpp -o CMakeFiles/synapnb.dir/src/buffer.cpp.s

lib/synapnb/CMakeFiles/synapnb.dir/src/network.cpp.o: lib/synapnb/CMakeFiles/synapnb.dir/flags.make
lib/synapnb/CMakeFiles/synapnb.dir/src/network.cpp.o: ../lib/synapnb/src/network.cpp
lib/synapnb/CMakeFiles/synapnb.dir/src/network.cpp.o: lib/synapnb/CMakeFiles/synapnb.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object lib/synapnb/CMakeFiles/synapnb.dir/src/network.cpp.o"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/synapnb/CMakeFiles/synapnb.dir/src/network.cpp.o -MF CMakeFiles/synapnb.dir/src/network.cpp.o.d -o CMakeFiles/synapnb.dir/src/network.cpp.o -c /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/network.cpp

lib/synapnb/CMakeFiles/synapnb.dir/src/network.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/synapnb.dir/src/network.cpp.i"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/network.cpp > CMakeFiles/synapnb.dir/src/network.cpp.i

lib/synapnb/CMakeFiles/synapnb.dir/src/network.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/synapnb.dir/src/network.cpp.s"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/network.cpp -o CMakeFiles/synapnb.dir/src/network.cpp.s

lib/synapnb/CMakeFiles/synapnb.dir/src/npu.cpp.o: lib/synapnb/CMakeFiles/synapnb.dir/flags.make
lib/synapnb/CMakeFiles/synapnb.dir/src/npu.cpp.o: ../lib/synapnb/src/npu.cpp
lib/synapnb/CMakeFiles/synapnb.dir/src/npu.cpp.o: lib/synapnb/CMakeFiles/synapnb.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object lib/synapnb/CMakeFiles/synapnb.dir/src/npu.cpp.o"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/synapnb/CMakeFiles/synapnb.dir/src/npu.cpp.o -MF CMakeFiles/synapnb.dir/src/npu.cpp.o.d -o CMakeFiles/synapnb.dir/src/npu.cpp.o -c /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/npu.cpp

lib/synapnb/CMakeFiles/synapnb.dir/src/npu.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/synapnb.dir/src/npu.cpp.i"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/npu.cpp > CMakeFiles/synapnb.dir/src/npu.cpp.i

lib/synapnb/CMakeFiles/synapnb.dir/src/npu.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/synapnb.dir/src/npu.cpp.s"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/npu.cpp -o CMakeFiles/synapnb.dir/src/npu.cpp.s

lib/synapnb/CMakeFiles/synapnb.dir/src/predictor.cpp.o: lib/synapnb/CMakeFiles/synapnb.dir/flags.make
lib/synapnb/CMakeFiles/synapnb.dir/src/predictor.cpp.o: ../lib/synapnb/src/predictor.cpp
lib/synapnb/CMakeFiles/synapnb.dir/src/predictor.cpp.o: lib/synapnb/CMakeFiles/synapnb.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object lib/synapnb/CMakeFiles/synapnb.dir/src/predictor.cpp.o"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/synapnb/CMakeFiles/synapnb.dir/src/predictor.cpp.o -MF CMakeFiles/synapnb.dir/src/predictor.cpp.o.d -o CMakeFiles/synapnb.dir/src/predictor.cpp.o -c /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/predictor.cpp

lib/synapnb/CMakeFiles/synapnb.dir/src/predictor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/synapnb.dir/src/predictor.cpp.i"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/predictor.cpp > CMakeFiles/synapnb.dir/src/predictor.cpp.i

lib/synapnb/CMakeFiles/synapnb.dir/src/predictor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/synapnb.dir/src/predictor.cpp.s"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/predictor.cpp -o CMakeFiles/synapnb.dir/src/predictor.cpp.s

lib/synapnb/CMakeFiles/synapnb.dir/src/tensor.cpp.o: lib/synapnb/CMakeFiles/synapnb.dir/flags.make
lib/synapnb/CMakeFiles/synapnb.dir/src/tensor.cpp.o: ../lib/synapnb/src/tensor.cpp
lib/synapnb/CMakeFiles/synapnb.dir/src/tensor.cpp.o: lib/synapnb/CMakeFiles/synapnb.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object lib/synapnb/CMakeFiles/synapnb.dir/src/tensor.cpp.o"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/synapnb/CMakeFiles/synapnb.dir/src/tensor.cpp.o -MF CMakeFiles/synapnb.dir/src/tensor.cpp.o.d -o CMakeFiles/synapnb.dir/src/tensor.cpp.o -c /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/tensor.cpp

lib/synapnb/CMakeFiles/synapnb.dir/src/tensor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/synapnb.dir/src/tensor.cpp.i"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/tensor.cpp > CMakeFiles/synapnb.dir/src/tensor.cpp.i

lib/synapnb/CMakeFiles/synapnb.dir/src/tensor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/synapnb.dir/src/tensor.cpp.s"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb/src/tensor.cpp -o CMakeFiles/synapnb.dir/src/tensor.cpp.s

# Object files for target synapnb
synapnb_OBJECTS = \
"CMakeFiles/synapnb.dir/src/allocator_synap.cpp.o" \
"CMakeFiles/synapnb.dir/src/buffer.cpp.o" \
"CMakeFiles/synapnb.dir/src/network.cpp.o" \
"CMakeFiles/synapnb.dir/src/npu.cpp.o" \
"CMakeFiles/synapnb.dir/src/predictor.cpp.o" \
"CMakeFiles/synapnb.dir/src/tensor.cpp.o"

# External object files for target synapnb
synapnb_EXTERNAL_OBJECTS =

lib/synapnb/libsynapnb.so: lib/synapnb/CMakeFiles/synapnb.dir/src/allocator_synap.cpp.o
lib/synapnb/libsynapnb.so: lib/synapnb/CMakeFiles/synapnb.dir/src/buffer.cpp.o
lib/synapnb/libsynapnb.so: lib/synapnb/CMakeFiles/synapnb.dir/src/network.cpp.o
lib/synapnb/libsynapnb.so: lib/synapnb/CMakeFiles/synapnb.dir/src/npu.cpp.o
lib/synapnb/libsynapnb.so: lib/synapnb/CMakeFiles/synapnb.dir/src/predictor.cpp.o
lib/synapnb/libsynapnb.so: lib/synapnb/CMakeFiles/synapnb.dir/src/tensor.cpp.o
lib/synapnb/libsynapnb.so: lib/synapnb/CMakeFiles/synapnb.dir/build.make
lib/synapnb/libsynapnb.so: lib/synap_utils/libsynap_utils.a
lib/synapnb/libsynapnb.so: lib/synap_base/libsynap_base.a
lib/synapnb/libsynapnb.so: lib/synap_device/libsynap_device.a
lib/synapnb/libsynapnb.so: lib/synap_utils/libsynap_utils.a
lib/synapnb/libsynapnb.so: lib/synapnb/CMakeFiles/synapnb.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX shared library libsynapnb.so"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/synapnb.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/synapnb/CMakeFiles/synapnb.dir/build: lib/synapnb/libsynapnb.so
.PHONY : lib/synapnb/CMakeFiles/synapnb.dir/build

lib/synapnb/CMakeFiles/synapnb.dir/clean:
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb && $(CMAKE_COMMAND) -P CMakeFiles/synapnb.dir/cmake_clean.cmake
.PHONY : lib/synapnb/CMakeFiles/synapnb.dir/clean

lib/synapnb/CMakeFiles/synapnb.dir/depend:
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synapnb /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synapnb/CMakeFiles/synapnb.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/synapnb/CMakeFiles/synapnb.dir/depend

