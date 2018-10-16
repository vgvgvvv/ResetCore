cmake_minimum_required(VERSION 3.4.1)

include_directories(src/main/jni)

file(GLOB_RECURSE NDK_SRC
        src/main/jni/*.cpp)

add_library(
        NDKDemo
        SHARED
        ${NDK_SRC})

find_library(
        log-lib
        log)

target_link_libraries(
        NDKDemo
        ${log-lib})