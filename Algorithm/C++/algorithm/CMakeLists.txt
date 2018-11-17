

include_directories(.)
file(GLOB_RECURSE SRC *.cpp *.h)

add_library(algorithm ${SRC})