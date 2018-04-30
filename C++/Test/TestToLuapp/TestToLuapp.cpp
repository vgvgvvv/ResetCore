//
// Created by 董宸 on 2018/4/18.
//

#include <cstring>
#include "Lua/lua.hpp"

int main(){

    char buff[256];
    int error;

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
//    luaL_dofile(L, "TestLua.lua");
    while(fgets(buff, sizeof(buff), stdin) != NULL){
        error = luaL_loadbuffer(L, buff, strlen(buff), "line") || lua_pcall(L, 0, 0, 0);
        if(error){
            fprintf(stderr, "%s", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    lua_close(L);

    return 0;
}