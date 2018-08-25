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

include_directories(src/utility)
file(GLOB_RECURSE utility
        src/utility/*.cpp
        src/utility/*.hpp)


#===========target==============
add_library(ReGL
        ${3rd}
        ${common}
        ${utility} src/common/Component.cpp src/common/Component.h src/common/BaseRenderer.cpp src/common/BaseRenderer.h)
target_link_libraries(ReGL ${AllLib})


set(SOURCE_FILES main.cpp src/common/RegistType.h)
add_executable(Test ${SOURCE_FILES})
target_link_libraries(Test ReGL)