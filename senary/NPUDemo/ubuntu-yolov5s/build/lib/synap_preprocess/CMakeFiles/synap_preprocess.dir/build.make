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
include lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/progress.make

# Include the compile flags for this target's objects.
include lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/flags.make

lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/src/input_data.cpp.o: lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/flags.make
lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/src/input_data.cpp.o: ../lib/synap_preprocess/src/input_data.cpp
lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/src/input_data.cpp.o: lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/src/input_data.cpp.o"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_preprocess && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/src/input_data.cpp.o -MF CMakeFiles/synap_preprocess.dir/src/input_data.cpp.o.d -o CMakeFiles/synap_preprocess.dir/src/input_data.cpp.o -c /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synap_preprocess/src/input_data.cpp

lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/src/input_data.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/synap_preprocess.dir/src/input_data.cpp.i"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_preprocess && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synap_preprocess/src/input_data.cpp > CMakeFiles/synap_preprocess.dir/src/input_data.cpp.i

lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/src/input_data.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/synap_preprocess.dir/src/input_data.cpp.s"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_preprocess && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synap_preprocess/src/input_data.cpp -o CMakeFiles/synap_preprocess.dir/src/input_data.cpp.s

lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/src/preprocessor.cpp.o: lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/flags.make
lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/src/preprocessor.cpp.o: ../lib/synap_preprocess/src/preprocessor.cpp
lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/src/preprocessor.cpp.o: lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/src/preprocessor.cpp.o"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_preprocess && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/src/preprocessor.cpp.o -MF CMakeFiles/synap_preprocess.dir/src/preprocessor.cpp.o.d -o CMakeFiles/synap_preprocess.dir/src/preprocessor.cpp.o -c /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synap_preprocess/src/preprocessor.cpp

lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/src/preprocessor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/synap_preprocess.dir/src/preprocessor.cpp.i"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_preprocess && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synap_preprocess/src/preprocessor.cpp > CMakeFiles/synap_preprocess.dir/src/preprocessor.cpp.i

lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/src/preprocessor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/synap_preprocess.dir/src/preprocessor.cpp.s"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_preprocess && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synap_preprocess/src/preprocessor.cpp -o CMakeFiles/synap_preprocess.dir/src/preprocessor.cpp.s

# Object files for target synap_preprocess
synap_preprocess_OBJECTS = \
"CMakeFiles/synap_preprocess.dir/src/input_data.cpp.o" \
"CMakeFiles/synap_preprocess.dir/src/preprocessor.cpp.o"

# External object files for target synap_preprocess
synap_preprocess_EXTERNAL_OBJECTS =

lib/synap_preprocess/libsynap_preprocess.a: lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/src/input_data.cpp.o
lib/synap_preprocess/libsynap_preprocess.a: lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/src/preprocessor.cpp.o
lib/synap_preprocess/libsynap_preprocess.a: lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/build.make
lib/synap_preprocess/libsynap_preprocess.a: lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libsynap_preprocess.a"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_preprocess && $(CMAKE_COMMAND) -P CMakeFiles/synap_preprocess.dir/cmake_clean_target.cmake
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_preprocess && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/synap_preprocess.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/build: lib/synap_preprocess/libsynap_preprocess.a
.PHONY : lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/build

lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/clean:
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_preprocess && $(CMAKE_COMMAND) -P CMakeFiles/synap_preprocess.dir/cmake_clean.cmake
.PHONY : lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/clean

lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/depend:
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/lib/synap_preprocess /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_preprocess /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov5s/build/lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/synap_preprocess/CMakeFiles/synap_preprocess.dir/depend
