

include_directories(../src)

file(GLOB_RECURSE ExtesionSrc ./*.c)

#≤‚ ‘ø‚
add_library(lua_extesion ${ExtesionSrc})