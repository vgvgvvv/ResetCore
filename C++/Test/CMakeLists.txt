cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(../Libs)

add_executable(TestXml TestXml/TestXml.cpp)
target_link_libraries(TestXml pugixml)