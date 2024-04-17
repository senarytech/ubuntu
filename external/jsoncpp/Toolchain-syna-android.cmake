# CMake toolchain file for building ARM software on OI environment

# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --target=${TARGET} --sysroot=${SYSROOT}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --target=${TARGET} --sysroot=${SYSROOT}")
SET(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -fuse-ld=lld ")
SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -fuse-ld=lld ")
SET(CMAKE_FIND_ROOT_PATH "${SYSROOT}")

# specify the cross compiler
SET(CMAKE_C_COMPILER clang)
SET(CMAKE_CXX_COMPILER clang++)
SET(CMAKE_STRIP llvm-strip)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

