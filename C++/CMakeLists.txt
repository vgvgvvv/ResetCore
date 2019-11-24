project(ResetCore)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS         "-Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG   "-gdwarf-2 -Og -DDEBUG_ON") # -g
set(CMAKE_CXX_FLAGS_RELEASE "-s -O2")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

IF(MACOS)
    link_directories(Libs/MACOS)
ELSEIF(WIN32)
    link_directories(Libs/WIN32)
ENDIF()

add_subdirectory(ECS)
add_subdirectory(Util)
add_subdirectory(ReMath)
add_subdirectory(Test)

add_subdirectory(LearnCpp/Macro)
add_subdirectory(LearnCpp/Template)
add_subdirectory(LearnCpp/ModernCpp)

include_directories(Util)
include_directories(ECS)
include_directories(ReMath/sources)
include_directories(Includes)

