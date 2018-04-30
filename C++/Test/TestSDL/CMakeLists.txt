cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

IF(MACOS)
    link_directories(../../Libs/MACOS)
    include_directories(../../Includes/MACOS)
ELSE(WIN32)
    link_directories(../Libs/WIN32)
    include_directories(../../Includes/WIN32)
ENDIF()

include_directories(../../Includes/Common)

add_executable(sdl01_hello_world Class1/HelloWorld.cpp)
target_link_libraries(sdl01_hello_world SDL2)