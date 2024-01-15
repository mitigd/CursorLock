# main CMakeLists.txt

cmake_minimum_required(VERSION 3.12)
project(cursorlock)

# Set the build directory to "build"
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

# Detect the target architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message("Target Architecture: x64")
    include(${CMAKE_SOURCE_DIR}/CMakeLists_x64.txt)
else()
    message("Target Architecture: x86")
    include(${CMAKE_SOURCE_DIR}/CMakeLists_x86.txt)
endif()
