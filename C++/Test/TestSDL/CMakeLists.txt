cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(../../Libs)

link_directories(../../Libs)
include_directories(../../Includes)

add_executable(sdl01_hello_world Class1/HelloWorld.cpp)
target_link_libraries(sdl01_hello_world SDL2)