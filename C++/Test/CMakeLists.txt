cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

IF(MACOS)
    link_directories(../Libs/MACOS)
    include_directories(../Includes/MACOS)
ELSEIF(WIN32)
    link_directories(../Libs/WIN32)
    include_directories(../Includes/WIN32)
ENDIF()

include_directories(../Includes/Common)

add_executable(TestXml TestXml/TestXml.cpp)
target_link_libraries(TestXml pugixml)

add_executable(TestEmbedMono TestEmbedMono/TestEmbedMono.cpp)
target_link_libraries(TestEmbedMono mono)

add_executable(TestToLuapp TestToLuapp/TestToLuapp.cpp)
target_link_libraries(TestToLuapp lua)

add_executable(TestFMod TestFMod/TestFmod.cpp)
target_link_libraries(TestFMod fmodL fmodstudioL)

add_subdirectory(TestSDL)
