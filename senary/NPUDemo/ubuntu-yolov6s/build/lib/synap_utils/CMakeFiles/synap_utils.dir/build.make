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
CMAKE_SOURCE_DIR = /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build

# Include any dependencies generated for this target.
include lib/synap_utils/CMakeFiles/synap_utils.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/synap_utils/CMakeFiles/synap_utils.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/synap_utils/CMakeFiles/synap_utils.dir/progress.make

# Include the compile flags for this target's objects.
include lib/synap_utils/CMakeFiles/synap_utils.dir/flags.make

lib/synap_utils/CMakeFiles/synap_utils.dir/src/file_utils.cpp.o: lib/synap_utils/CMakeFiles/synap_utils.dir/flags.make
lib/synap_utils/CMakeFiles/synap_utils.dir/src/file_utils.cpp.o: ../lib/synap_utils/src/file_utils.cpp
lib/synap_utils/CMakeFiles/synap_utils.dir/src/file_utils.cpp.o: lib/synap_utils/CMakeFiles/synap_utils.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/synap_utils/CMakeFiles/synap_utils.dir/src/file_utils.cpp.o"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build/lib/synap_utils && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/synap_utils/CMakeFiles/synap_utils.dir/src/file_utils.cpp.o -MF CMakeFiles/synap_utils.dir/src/file_utils.cpp.o.d -o CMakeFiles/synap_utils.dir/src/file_utils.cpp.o -c /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/lib/synap_utils/src/file_utils.cpp

lib/synap_utils/CMakeFiles/synap_utils.dir/src/file_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/synap_utils.dir/src/file_utils.cpp.i"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build/lib/synap_utils && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/lib/synap_utils/src/file_utils.cpp > CMakeFiles/synap_utils.dir/src/file_utils.cpp.i

lib/synap_utils/CMakeFiles/synap_utils.dir/src/file_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/synap_utils.dir/src/file_utils.cpp.s"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build/lib/synap_utils && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/lib/synap_utils/src/file_utils.cpp -o CMakeFiles/synap_utils.dir/src/file_utils.cpp.s

lib/synap_utils/CMakeFiles/synap_utils.dir/src/string_utils.cpp.o: lib/synap_utils/CMakeFiles/synap_utils.dir/flags.make
lib/synap_utils/CMakeFiles/synap_utils.dir/src/string_utils.cpp.o: ../lib/synap_utils/src/string_utils.cpp
lib/synap_utils/CMakeFiles/synap_utils.dir/src/string_utils.cpp.o: lib/synap_utils/CMakeFiles/synap_utils.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object lib/synap_utils/CMakeFiles/synap_utils.dir/src/string_utils.cpp.o"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build/lib/synap_utils && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/synap_utils/CMakeFiles/synap_utils.dir/src/string_utils.cpp.o -MF CMakeFiles/synap_utils.dir/src/string_utils.cpp.o.d -o CMakeFiles/synap_utils.dir/src/string_utils.cpp.o -c /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/lib/synap_utils/src/string_utils.cpp

lib/synap_utils/CMakeFiles/synap_utils.dir/src/string_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/synap_utils.dir/src/string_utils.cpp.i"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build/lib/synap_utils && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/lib/synap_utils/src/string_utils.cpp > CMakeFiles/synap_utils.dir/src/string_utils.cpp.i

lib/synap_utils/CMakeFiles/synap_utils.dir/src/string_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/synap_utils.dir/src/string_utils.cpp.s"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build/lib/synap_utils && /home/don/Dq/NPU/Synaptics_Git_Ext_VSSDK-1.10.1-lts/aarch64/gcc-arm-aarch64-linux-gnu-8.3/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/lib/synap_utils/src/string_utils.cpp -o CMakeFiles/synap_utils.dir/src/string_utils.cpp.s

# Object files for target synap_utils
synap_utils_OBJECTS = \
"CMakeFiles/synap_utils.dir/src/file_utils.cpp.o" \
"CMakeFiles/synap_utils.dir/src/string_utils.cpp.o"

# External object files for target synap_utils
synap_utils_EXTERNAL_OBJECTS =

lib/synap_utils/libsynap_utils.a: lib/synap_utils/CMakeFiles/synap_utils.dir/src/file_utils.cpp.o
lib/synap_utils/libsynap_utils.a: lib/synap_utils/CMakeFiles/synap_utils.dir/src/string_utils.cpp.o
lib/synap_utils/libsynap_utils.a: lib/synap_utils/CMakeFiles/synap_utils.dir/build.make
lib/synap_utils/libsynap_utils.a: lib/synap_utils/CMakeFiles/synap_utils.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libsynap_utils.a"
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build/lib/synap_utils && $(CMAKE_COMMAND) -P CMakeFiles/synap_utils.dir/cmake_clean_target.cmake
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build/lib/synap_utils && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/synap_utils.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/synap_utils/CMakeFiles/synap_utils.dir/build: lib/synap_utils/libsynap_utils.a
.PHONY : lib/synap_utils/CMakeFiles/synap_utils.dir/build

lib/synap_utils/CMakeFiles/synap_utils.dir/clean:
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build/lib/synap_utils && $(CMAKE_COMMAND) -P CMakeFiles/synap_utils.dir/cmake_clean.cmake
.PHONY : lib/synap_utils/CMakeFiles/synap_utils.dir/clean

lib/synap_utils/CMakeFiles/synap_utils.dir/depend:
	cd /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/lib/synap_utils /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build/lib/synap_utils /home/don/Dq/NPU/project/paddlepaddle/ubuntu-yolov6s/build/lib/synap_utils/CMakeFiles/synap_utils.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/synap_utils/CMakeFiles/synap_utils.dir/depend

