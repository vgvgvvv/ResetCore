cmake_minimum_required(VERSION 3.10)
project(ReAlgorithm)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -g ")

include_directories(algorithm)
set(SRC main.cpp)

add_subdirectory(algorithm)
add_subdirectory(unit_tests)

add_executable(ReAlgorithm ${SRC})


############### Unit Test Start ###############

if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif (APPLE)

# This adds another subdirectory, which has 'project(gtest)'.
add_subdirectory(unit_tests/lib/gtest-1.6.0)

enable_testing()

# Include the gtest library. gtest_SOURCE_DIR is available due to
# 'project(gtest)' above.
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

##############
# Unit Tests
##############

file(GLOB_RECURSE cpp_11_feature_src
        unit_tests/cpp11_feature/*.cpp
        unit_tests/learning_c/*.cpp)

add_executable(runUnitTests
        unit_tests/algorithm_tests/test_vector.cpp
        ${cpp_11_feature_src})

# Standard linking to gtest stuff.
target_link_libraries(runUnitTests gtest gtest_main)

# Extra linking for the project.
target_link_libraries(runUnitTests algorithm)

############### Unit Test End ###############