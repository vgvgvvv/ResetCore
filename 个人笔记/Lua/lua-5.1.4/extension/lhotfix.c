//
// Created by 董宸 on 2019/3/4.
//

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define LUA_HOTFIXLIBNAME	"hotswap"

static int luaB_hsinit(lua_State* L){

    return 1;
}

static int luaB_hsupdate(lua_State* L){
    return 1;
}


static const luaL_Reg hotswap_funcs[] = {
        {"init", luaB_hsinit},
        {"update", luaB_hsupdate},
        {NULL, NULL}
};

/**
 * 打开热更新库
 * @param L
 * @return
 */
static const int lua_openhotswap(lua_State* L){
    luaL_register(L, LUA_HOTFIXLIBNAME, hotswap_funcs);
    return 1;
}
