

include_directories(.)
include_directories(../Util)
file(GLOB_RECURSE ecs_src *.cpp *.hpp)

add_library(ResetCore.ECS STATIC ${ecs_src})