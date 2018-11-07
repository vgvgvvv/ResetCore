cmake_minimum_required(VERSION 3.0.2)

project(NativeLib)

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -DDEBUG -D_DEBUG -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall -std=c++11 -s")

include_directories(src)
include_directories(thirdpart/include/libpng)

file(GLOB_RECURSE SRC src/*.cpp)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_OSX_ARCHITECTURES "armv7 arm64")
set(CMAKE_OSX_SYSROOT iphoneos12.1)
set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "8.0")

add_library( # Sets the name of the library.
        NativeLib
        STATIC
        ${SRC})