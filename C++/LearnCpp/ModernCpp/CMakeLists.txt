
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD  11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

add_executable(PolicyClass PolicyClasses/PolicyClass.cpp PolicyClasses/PolicyClass.h)