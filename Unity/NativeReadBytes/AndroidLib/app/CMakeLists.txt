cmake_minimum_required(VERSION 3.4.1)

include_directories(src/main/jni)
file(GLOB_RECURSE NDK_SRC src/main/jni/*.cpp)

add_library( # Sets the name of the library.
        NativeLib
        SHARED
        ${NDK_SRC})

find_library(
        android-lib
        android
        log-lib
        log)

target_link_libraries(
        NativeLib
        ${log-lib}
        ${android-lib})