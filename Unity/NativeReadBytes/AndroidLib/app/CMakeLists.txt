cmake_minimum_required(VERSION 3.4.1)

include_directories(src/main/jni)
include_directories(src/main/jni/include/libpng)
file(GLOB_RECURSE NDK_SRC src/main/jni/*.cpp)
link_directories(src/main/jni/thirdpart/${ANDROID_ABI})

add_library( # Sets the name of the library.
        NativeLib
        SHARED
        ${NDK_SRC})

find_library(
        android
        log
        GLESv1_CM
        GLESv2)

target_link_libraries(
        NativeLib
        log
        android
        png
        GLESv1_CM
        GLESv2)