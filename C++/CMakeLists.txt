project(ResetCore)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS         "-Wall -Werror -fmax-errors=5 -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG   "-gdwarf-2 -Og -DDEBUG_ON") # -g
set(CMAKE_CXX_FLAGS_RELEASE "-s -O2")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

include_directories(Util)