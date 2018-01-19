

include_directories(.)
file(GLOB_RECURSE util_src *.cpp *.hpp *.h)

add_library(ResetCore.Util STATIC ${util_src})