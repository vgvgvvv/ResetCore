cmake_minimum_required(VERSION 3.12)
project(LearningC)

set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE learn_src src/*.c src/*.h)

add_library(LearningC ${learn_src})


if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif (APPLE)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

message(status ${gtest_SOURCE_DIR})

file(GLOB_RECURSE test_src test/*.cpp test/*.h)

add_executable(testLearningC ${test_src})
target_link_libraries(testLearningC gtest gtest_main)
target_link_libraries(testLearningC LearningC)
