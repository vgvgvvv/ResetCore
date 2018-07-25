cmake_minimum_required(VERSION 3.8)
project(ReGL)

set(CMAKE_CXX_STANDARD 11)

#===========3rd==============
find_package(OpenGL REQUIRED)
find_library(GLEW REQUIRED)
find_library(GLFW REQUIRED)
find_library(glm REQUIRED)

set(AllLib
        GLEW
        GLFW
        ${OPENGL_LIBRARY})

include_directories(src/3rd)
file(GLOB_RECURSE 3rd
        src/3rd/*.cpp
        src/3rd/*.h)

#===========src==============
include_directories(src/common)
file(GLOB_RECURSE common
        src/common/*.cpp
        src/common/*.hpp)

include_directories(src/renderer)
file(GLOB_RECURSE renderer
        src/renderer/*.cpp
        src/renderer/*.hpp)


#===========target==============
add_library(ReGL
        ${3rd}
        ${common}
        ${renderer})
target_link_libraries(ReGL ${AllLib})

set(SOURCE_FILES main.cpp)
add_executable(Test ${SOURCE_FILES})
target_link_libraries(Test ReGL)