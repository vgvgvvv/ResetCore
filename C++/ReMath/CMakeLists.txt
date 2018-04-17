cmake_minimum_required(VERSION 3.8)
project(ReMath)

set(CMAKE_CXX_STANDARD 11)

include_directories(sources)
file(GLOB_RECURSE src sources/*.cpp sources/*.hpp)

add_library(ReMath ${src})

add_executable(Test sources/Test.cpp)
link_libraries(Test ReMath)