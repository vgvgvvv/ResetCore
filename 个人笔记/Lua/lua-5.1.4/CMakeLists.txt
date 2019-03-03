cmake_minimum_required(VERSION 3.12.0)
project(TestLua)

include_directories(src)
include_directories(testcproject)

add_subdirectory(extension)

file(GLOB_RECURSE LuaLibSrc
    src/lapi.c src/lauxlib.c src/lbaselib.c src/lcode.c src/ldblib.c
        src/ldebug.c src/ldo.c src/ldump.c src/lfunc.c src/lgc.c src/linit.c
        src/liolib.c src/llex.c src/lmathlib.c src/lmem.c src/loadlib.c
        src/lobject.c src/lopcodes.c src/loslib.c src/lparser.c src/lstate.c
        src/lstring.c src/lstrlib.c src/ltable.c src/ltablib.c src/ltm.c src/lundump.c
        src/lvm.c src/lzio.c src/print.c
        src/lapi.h src/lauxlib.h src/lcode.h src/ldebug.h src/ldo.h src/lfunc.h
        src/lgc.h src/llex.h src/llimits.h src/lmem.h src/lobject.h src/lopcodes.h src/lstate.h
        src/lstring.h src/ltable.h src/ltm.h src/lua.h src/luaconf.h src/lualib.h src/lundump.h src/lvm.h src/lzio.h)

file(GLOB_RECURSE LuaSrc src/lua.c)

file(GLOB_RECURSE LuaCSrc src/luac.c)



file(GLOB_RECURSE 
    TestLuaSrc
    ${LuaLibSrc}
    testcproject/*.cpp
    testcproject/*.c
    testcproject/*.h)

message(status ${TestLuaSrc})



add_executable(testlua ${TestLuaSrc})

