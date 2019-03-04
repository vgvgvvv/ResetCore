//
// Created by 董宸 on 2019/3/4.
//

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define LUA_HOTSWAPLIBNAME	"hotswap"

/**
 * 初始化热重载地址
 * @param L
 * @return
 */
static int luaB_hsinit(lua_State* L){

    return 1;
}

/**
 * 热重载更新
 * @param L
 * @return
 */
static int luaB_hsupdate(lua_State* L){
    
    return 1;
}


static const luaL_Reg hotswap_funcs[] = {
        {"init", luaB_hsinit},
        {"update", luaB_hsupdate},
        {NULL, NULL}
};

/**
 * 打开热重载库
 * @param L
 * @return
 */
static const int lua_openhotswap(lua_State* L){
    luaL_register(L, LUA_HOTSWAPLIBNAME, hotswap_funcs);
    return 1;
}
