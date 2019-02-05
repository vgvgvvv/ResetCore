cmake_minimum_required(VERSION 3.12.0)
project(TestLua)

include_directories(src)
include_directories(testcproject)

file(GLOB_RECURSE 
    TestLuaSrc
    src/*.c 
    src/*.h 
    testcproject/*.cpp  
    testcproject/*.c
    testcproject/*.h)

add_executable(testlua ${TestLuaSrc})