cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(../Libs)

link_directories(../Libs)
include_directories(../Includes)

add_executable(TestXml TestXml/TestXml.cpp)
target_link_libraries(TestXml pugixml)

add_executable(TestEmbedMono TestEmbedMono/TestEmbedMono.cpp)
target_link_libraries(TestEmbedMono mono)

add_executable(TestToLuapp TestToLuapp/TestToLuapp.cpp)
target_link_libraries(TestToLuapp lua)

add_executable(TestFMod TestFMod/TestFmod.cpp)
target_link_libraries(TestFMod fmodL fmodstudioL)

add_subdirectory(TestSDL)