
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")
set(CMAKE_CXX_STANDARD  11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

add_executable(Max chapter2/max.hpp chapter2/max.cpp)

add_executable(Stack1 chapter3/Stack.cpp chapter3/Stack.hpp)