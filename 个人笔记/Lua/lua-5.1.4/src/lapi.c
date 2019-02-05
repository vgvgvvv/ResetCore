/*
** $Id: lapi.c,v 2.55.1.5 2008/07/04 18:41:18 roberto Exp $
** Lua API
** See Copyright Notice in lua.h
*/


#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>

#define lapi_c
#define LUA_CORE

#include "lua.h"

#include "lapi.h"
#include "ldebug.h"
#include "ldo.h"
#include "lfunc.h"
#include "lgc.h"
#include "lmem.h"
#include "lobject.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"
#include "ltm.h"
#include "lundump.h"
#include "lvm.h"


//Lua版本信息
const char lua_ident[] =
  "$Lua: " LUA_RELEASE " " LUA_COPYRIGHT " $\n"
  "$Authors: " LUA_AUTHORS " $\n"
  "$URL: www.lua.org $\n";


//检查元素数量是否大于栈空间
#define api_checknelems(L, n)	api_check(L, (n) <= (L->top - L->base))

//检查是否为空值
#define api_checkvalidindex(L, i)	api_check(L, (i) != luaO_nilobject)

//增加LuaState栈空间
#define api_incr_top(L)   {api_check(L, L->top < L->ci->top); L->top++;}

//将栈索引转为具体的值
static TValue *index2adr (lua_State *L, int idx) {
  if (idx > 0) {
	  // 如果idx > 0,则从栈中base为基础位置取元素
    TValue *o = L->base + (idx - 1);
    // 检查范围
    api_check(L, idx <= L->ci->top - L->base);
    // 如果指针位置大于顶部则返回空值
    if (o >= L->top) {
      return cast(TValue *, luaO_nilobject);
    }
    else {
      return o;
    }
  }
  //注册表的情况
  else if (idx > LUA_REGISTRYINDEX) {
	  // 如果LUA_REGISTRYINDEX > idx < 0,则从栈中top为基础位置取元素
    api_check(L, idx != 0 && -idx <= L->top - L->base);
    // 类似于从栈顶倒着数，例如getvalue(L, -2)，就是top-2的那个值，最终结果不能小于0
    return L->top + idx;
  }
  else {
    switch (idx) {  /* pseudo-indices 伪索引 */
      // 注册表的情况 返回global_State中的注册表
      case LUA_REGISTRYINDEX: return registry(L);
      // 环境索引，返回当前的环境
      case LUA_ENVIRONINDEX: {
        Closure *func = curr_func(L);// 获取当前函数
        sethvalue(L, &L->env, func->c.env);// 获取函数的环境
        return &L->env;// 返回当前环境
      }
      // 全局索引，返回全局表
      case LUA_GLOBALSINDEX: return gt(L);
      default: {
        //获取当前函数
        Closure *func = curr_func(L);
        //从全局索引往下数，则又是从1开始
        idx = LUA_GLOBALSINDEX - idx;
        //如果数量小于上值的数量则返回上值否则返回空
        return (idx <= func->c.nupvalues)
                  ? &func->c.upvalue[idx-1]
                  : cast(TValue *, luaO_nilobject);
      }
    }
  }
}

/**
 * 获取当前环境表
 */
static Table *getcurrenv (lua_State *L) {
  if (L->ci == L->base_ci)  /* no enclosing function? */
	  // 如果当前不在任何函数中,那个使用全局表
    return hvalue(gt(L));  /* use global table as environment */
  else {
    //返回函数环境
    Closure *func = curr_func(L);
    return func->c.env;
  }
}

/**
 * 向栈中推入object
 */
void luaA_pushobject (lua_State *L, const TValue *o) {
  //给栈顶赋值
  setobj2s(L, L->top, o);
  //栈顶指针加 1
  api_incr_top(L);
}

/** 
 * 确保堆栈上至少有 n 个额外空位。 
 * 如果不能把堆栈扩展到相应的尺寸，函数返回假。 
 * 失败的原因包括将把栈扩展到比固定最大尺寸还大 （至少是几千个元素）或分配内存失败。 
 * 这个函数永远不会缩小堆栈； 如果堆栈已经比需要的大了，那么就保持原样。
 * [-0, +0, –]
 */
LUA_API int lua_checkstack (lua_State *L, int size) {
  int res = 1;
  lua_lock(L);
  //先检查栈溢出
  if (size > LUAI_MAXCSTACK || (L->top - L->base + size) > LUAI_MAXCSTACK) {
    res = 0;  /* stack overflow */
  }
  else if (size > 0) {
    luaD_checkstack(L, size);
    //调整调用栈顶
    if (L->ci->top < L->top + size){
      L->ci->top = L->top + size;
    }
  }
  lua_unlock(L);
  return res;
}

/**
 * 交换同一个状态机下不同线程中的值。
 * 这个函数会从 from 的栈上弹出 n 个值， 然后把它们压入 to 的栈上。
 * [-?, +?, –]
 */
LUA_API void lua_xmove (lua_State *from, lua_State *to, int n) {
  int i;
  if (from == to) return;
  lua_lock(to);
  api_checknelems(from, n);
  api_check(from, G(from) == G(to));
  api_check(from, to->ci->top - to->top >= n);
  // from的栈指针-n,表示栈顶需要移动的元素回退
  from->top -= n;
  // 依次从from中把需要的变量移动到to中
  for (i = 0; i < n; i++) {
    setobj2s(to, to->top++, from->top + i);
  }
  lua_unlock(to);
}

//TODO: lua_setlevel 这个是啥意思
LUA_API void lua_setlevel (lua_State *from, lua_State *to) {
  to->nCcalls = from->nCcalls;
}

/**
 * 设置一个新的 panic 函数，并返回之前设置的那个。 （参见 §4.6）。
 * [-0, +0, –]
 */
LUA_API lua_CFunction lua_atpanic (lua_State *L, lua_CFunction panicf) {
  lua_CFunction old;
  lua_lock(L);
  old = G(L)->panic;
  G(L)->panic = panicf;
  lua_unlock(L);
  return old;
}

/**
 * 创建一条新线程，并将其压栈， 并返回维护这个线程的 lua_State 指针。 
 * 这个函数返回的新线程共享原线程的全局环境， 但是它有独立的运行栈。
 * 没有显式的函数可以用来关闭或销毁掉一个线程。 线程跟其它 Lua 对象一样是垃圾收集的条目之一。
 * [-0, +0, –]
 */ 
LUA_API lua_State *lua_newthread (lua_State *L) {
  lua_State *L1;
  lua_lock(L);
  luaC_checkGC(L);
  //创建新的线程返回这个线程的lua_state
  L1 = luaE_newthread(L);
  //将这个luastate压入栈顶
  setthvalue(L, L->top, L1);
  api_incr_top(L);
  lua_unlock(L);
  luai_userstatethread(L, L1);
  return L1;
}



/*
** basic stack manipulation
*/



/**
 * 返回栈顶元素的索引。 因为索引是从 1 开始编号的， 
 * 所以这个结果等于栈上的元素个数； 特别指出，0 表示栈为空。
 */
LUA_API int lua_gettop (lua_State *L) {
  return cast_int(L->top - L->base);
}

/**
 * 参数允许传入任何索引以及 0 。 它将把堆栈的栈顶设为这个索引。 
 * 如果新的栈顶比原来的大， 超出部分的新元素将被填为 nil 。 
 * 如果 index 为 0 ， 把栈上所有元素移除。
 * [-?, +?, –]
 */ 
LUA_API void lua_settop (lua_State *L, int idx) {
  lua_lock(L);
  if (idx >= 0) {
    //保证索引在当前栈之后 栈尾之前
    api_check(L, idx <= L->stack_last - L->base);
    //将栈上超出top部分的值置空
    while (L->top < L->base + idx){
      setnilvalue(L->top++);
    }
    //设置top位置
    L->top = L->base + idx;
  }
  else {
    api_check(L, -(idx+1) <= (L->top - L->base));
    //设置top位置
    L->top += idx+1;  /* `subtract' index (index is negative) */
  }
  lua_unlock(L);
}

/**
 * 从给定有效索引处移除一个元素， 把这个索引之上的所有元素移下来填补上这个空隙。 
 * 不能用伪索引来调用这个函数，因为伪索引并不指向真实的栈上的位置。
 * [-1, +0, –]
 */
LUA_API void lua_remove (lua_State *L, int idx) {
  StkId p;
  lua_lock(L);
  p = index2adr(L, idx);
  api_checkvalidindex(L, p);
  // 把idx后面的数据往前挪
  while (++p < L->top) setobjs2s(L, p-1, p);
  L->top--;
  lua_unlock(L);
}

/**
 * 把栈顶元素移动到指定的有效索引处， 依次移动这个索引之上的元素。 
 * 不要用伪索引来调用这个函数， 因为伪索引没有真正指向栈上的位置。
 * [-1, +1, –]
 */
LUA_API void lua_insert (lua_State *L, int idx) {
  StkId p;
  StkId q;
  lua_lock(L);
  p = index2adr(L, idx);
  api_checkvalidindex(L, p);
  //往后挪
  for (q = L->top; q>p; q--) setobjs2s(L, q, q-1);
  //设置插入的元素
  setobjs2s(L, p, L->top);
  lua_unlock(L);
}

/**
 * 把栈顶元素放置到给定位置而不移动其它元素 （因此覆盖了那个位置处的值），然后将栈顶元素弹出。
 * [-1, +0, –]
 */
LUA_API void lua_replace (lua_State *L, int idx) {
  StkId o;
  lua_lock(L);
  /* explicit test for incompatible code */
  //不允许对全局环境表使用
  if (idx == LUA_ENVIRONINDEX && L->ci == L->base_ci){
    luaG_runerror(L, "no calling environment");
  }
  api_checknelems(L, 1);
  o = index2adr(L, idx);
  api_checkvalidindex(L, o);
  if (idx == LUA_ENVIRONINDEX) {
    Closure *func = curr_func(L);
    // 必须是table
    api_check(L, ttistable(L->top - 1)); 
    //将环境设置为函数环境
    func->c.env = hvalue(L->top - 1);
    // TODO:垃圾回收部分 luaC_barrier
    luaC_barrier(L, func, L->top - 1);
  }
  else {
    setobj(L, o, L->top - 1);
    //处理上值
    if (idx < LUA_GLOBALSINDEX)  { /* function upvalue? */
      // TODO:luaC_barrier
      luaC_barrier(L, curr_func(L), L->top - 1);
    }
  }
  L->top--;
  lua_unlock(L);
}

/**
 * 把栈上给定索引处的元素作一个副本压栈。
 * [-0, +1, –]
 */
LUA_API void lua_pushvalue (lua_State *L, int idx) {
  lua_lock(L);
  //设值
  setobj2s(L, L->top, index2adr(L, idx));
  //设置top
  api_incr_top(L);
  lua_unlock(L);
}



/*
** access functions (stack -> C)
*/



/**
 * 返回给定有效索引处值的类型， 当索引无效（或无法访问）时则返回 LUA_TNONE。 
 * lua_type 返回的类型被编码为一些个在 lua.h 中定义的常量： 
 * LUA_TNIL， LUA_TNUMBER， LUA_TBOOLEAN， LUA_TSTRING， LUA_TTABLE， LUA_TFUNCTION， 
 * LUA_TUSERDATA， LUA_TTHREAD， LUA_TLIGHTUSERDATA。
 * [-0, +0, –]
 */
LUA_API int lua_type (lua_State *L, int idx) {
  StkId o = index2adr(L, idx);
  return (o == luaO_nilobject) ? LUA_TNONE : ttype(o);
}

/**
 * 返回类型名
 */
LUA_API const char *lua_typename (lua_State *L, int t) {
  UNUSED(L);//TODO:UNUSED（x）意义不明？
  return (t == LUA_TNONE) ? "no value" : luaT_typenames[t];
}

/**
 * 是否为函数
 */
LUA_API int lua_iscfunction (lua_State *L, int idx) {
  StkId o = index2adr(L, idx);
  return iscfunction(o);
}

/**
 * 是否为数字
 */
LUA_API int lua_isnumber (lua_State *L, int idx) {
  TValue n;
  const TValue *o = index2adr(L, idx);
  return tonumber(o, &n);
}

/**
 * 是否为字符串
 */
LUA_API int lua_isstring (lua_State *L, int idx) {
  int t = lua_type(L, idx);
  //数字也是字符串
  return (t == LUA_TSTRING || t == LUA_TNUMBER);
}

/**
 * 是否为userdata
 */
LUA_API int lua_isuserdata (lua_State *L, int idx) {
  const TValue *o = index2adr(L, idx);
  return (ttisuserdata(o) || ttislightuserdata(o));
}

/**
 * 如果索引 index1 与索引 index2 处的值 本身相等（即不调用元方法），返回 1 。 否则返回 0 。 
 * 当任何一个索引无效时，也返回 0 。
 */
LUA_API int lua_rawequal (lua_State *L, int index1, int index2) {
  StkId o1 = index2adr(L, index1);
  StkId o2 = index2adr(L, index2);
  return (o1 == luaO_nilobject || o2 == luaO_nilobject) ? 0
         : luaO_rawequalObj(o1, o2);
}

/**
 * 判断是否相等
 */
LUA_API int lua_equal (lua_State *L, int index1, int index2) {
  StkId o1, o2;
  int i;
  lua_lock(L);  /* may call tag method */
  o1 = index2adr(L, index1);
  o2 = index2adr(L, index2);
  i = (o1 == luaO_nilobject || o2 == luaO_nilobject) ? 0 : equalobj(L, o1, o2);
  lua_unlock(L);
  return i;
}

/**
 * 判断是否小于
 */
LUA_API int lua_lessthan (lua_State *L, int index1, int index2) {
  StkId o1, o2;
  int i;
  lua_lock(L);  /* may call tag method */
  o1 = index2adr(L, index1);
  o2 = index2adr(L, index2);
  i = (o1 == luaO_nilobject || o2 == luaO_nilobject) ? 0
       : luaV_lessthan(L, o1, o2);
  lua_unlock(L);
  return i;
}


/**
 * 等价于调用 lua_tonumberx， 其参数 isnum 为 NULL。
 * [-0, +0, –]
 */ 
LUA_API lua_Number lua_tonumber (lua_State *L, int idx) {
  TValue n;
  const TValue *o = index2adr(L, idx);
  if (tonumber(o, &n))
    return nvalue(o);
  else
    return 0;
}

/**
 * 等价于调用 lua_tointegerx， 其参数 isnum 为 NULL。
 * [-0, +0, –]
 */
LUA_API lua_Integer lua_tointeger (lua_State *L, int idx) {
  TValue n;
  const TValue *o = index2adr(L, idx);
  if (tonumber(o, &n)) {
    lua_Integer res;
    lua_Number num = nvalue(o);
    lua_number2integer(res, num);
    return res;
  }
  else
    return 0;
}

/**
 * 把给定索引处的 Lua 值转换为一个 C 中的布尔量（ 0 或是 1 ）。 
 * 和 Lua 中做的所有测试一样， lua_toboolean 会把任何不同于 false 和 nil 的值当作真返回； 否则就返回假。 
 * （如果你想只接收真正的 boolean 值， 就需要使用 lua_isboolean 来测试值的类型。）
 * [-0, +0, –]
 */
LUA_API int lua_toboolean (lua_State *L, int idx) {
  const TValue *o = index2adr(L, idx);
  return !l_isfalse(o);
}

/**
 * 把给定索引处的 Lua 值转换为一个 C 字符串。 如果 len 不为 NULL ， 它还把字符串长度设到 *len 中。 
 * 这个 Lua 值必须是一个字符串或是一个数字； 否则返回返回 NULL 。 
 * 如果值是一个数字， lua_tolstring 还会 把堆栈中的那个值的实际类型转换为一个字符串。 
 * （当遍历一张表的时候， 若把 lua_tolstring 作用在键上， 这个转换有可能导致 lua_next 弄错。）
 * lua_tolstring 返回一个已对齐指针 指向 Lua 状态机中的字符串。 这个字符串总能保证 （ C 要求的）最后一个字符为零 ('\0') ， 
 * 而且它允许在字符串内包含多个这样的零。因为 Lua 中可能发生垃圾收集， 所以不保证 lua_tolstring 返回的指针， 
 * 在对应的值从堆栈中移除后依然有效。
 * [-0, +0, e]
 */
LUA_API const char *lua_tolstring (lua_State *L, int idx, size_t *len) {
  StkId o = index2adr(L, idx);
  //不为字符串的时候才进行转换
  if (!ttisstring(o)) {
    lua_lock(L);  /* `luaV_tostring' may create a new string */
    if (!luaV_tostring(L, o)) {  /* 转换失败 */
      if (len != NULL) *len = 0;
      lua_unlock(L);
      return NULL;
    }
    //检查GC
    luaC_checkGC(L);
    o = index2adr(L, idx);  /* previous call may reallocate the stack */
    lua_unlock(L);
  }
  if (len != NULL) *len = tsvalue(o)->len;
  return svalue(o);
}

/**
 * 获取一个对象的长度
 */
LUA_API size_t lua_objlen (lua_State *L, int idx) {
  StkId o = index2adr(L, idx);
  switch (ttype(o)) {
    //字符串长度
    case LUA_TSTRING: return tsvalue(o)->len;
    //指针长度
    case LUA_TUSERDATA: return uvalue(o)->len;
    //table长度
    case LUA_TTABLE: return luaH_getn(hvalue(o));
    //数字转为字符串后字符串的长度
    case LUA_TNUMBER: {
      size_t l;
      lua_lock(L);  /* `luaV_tostring' may create a new string */
      l = (luaV_tostring(L, o) ? tsvalue(o)->len : 0);
      lua_unlock(L);
      return l;
    }
    default: return 0;
  }
}

/**
 * 把给定索引处的 Lua 值转换为一个 C 函数。 这个值必须是一个 C 函数； 如果不是就返回 NULL 。
 * [-0, +0, –]
 */
LUA_API lua_CFunction lua_tocfunction (lua_State *L, int idx) {
  StkId o = index2adr(L, idx);
  return (!iscfunction(o)) ? NULL : clvalue(o)->c.f;
}

/**
 * 如果给定索引处的值是一个完全用户数据， 函数返回其内存块的地址。 
 * 如果值是一个轻量用户数据， 那么就返回它表示的指针。 否则，返回 NULL 。
 * [-0, +0, –]
 */
LUA_API void *lua_touserdata (lua_State *L, int idx) {
  StkId o = index2adr(L, idx);
  switch (ttype(o)) {
    case LUA_TUSERDATA: return (rawuvalue(o) + 1);
    case LUA_TLIGHTUSERDATA: return pvalue(o);
    default: return NULL;
  }
}

/**
 * 把给定索引处的值转换为一个 Lua 线程 （表示为 lua_State*）。 
 * 这个值必须是一个线程； 否则函数返回 NULL。
 * [-0, +0, –]
 */
LUA_API lua_State *lua_tothread (lua_State *L, int idx) {
  StkId o = index2adr(L, idx);
  return (!ttisthread(o)) ? NULL : thvalue(o);
}

/**
 * 把给定索引处的值转换为一般的 C 指针 (void*) 。 
 * 这个值可以是一个用户对象，表 ，线程或是一个函数； 否则， lua_topointer 返回 NULL 。 
 * 不同的对象有不同的指针。 不存在把指针再转回原有类型的方法。
 * [-0, +0, –]
 */
LUA_API const void *lua_topointer (lua_State *L, int idx) {
  StkId o = index2adr(L, idx);
  switch (ttype(o)) {
    case LUA_TTABLE: return hvalue(o);
    case LUA_TFUNCTION: return clvalue(o);
    case LUA_TTHREAD: return thvalue(o);
    case LUA_TUSERDATA:
    case LUA_TLIGHTUSERDATA:
      return lua_touserdata(L, idx);
    default: return NULL;
  }
}



/*
** push functions (C -> stack)
*/



LUA_API void lua_pushnil (lua_State *L) {
  lua_lock(L);
  setnilvalue(L->top);
  api_incr_top(L);
  lua_unlock(L);
}


LUA_API void lua_pushnumber (lua_State *L, lua_Number n) {
  lua_lock(L);
  setnvalue(L->top, n);
  api_incr_top(L);
  lua_unlock(L);
}


LUA_API void lua_pushinteger (lua_State *L, lua_Integer n) {
  lua_lock(L);
  setnvalue(L->top, cast_num(n));
  api_incr_top(L);
  lua_unlock(L);
}


LUA_API void lua_pushlstring (lua_State *L, const char *s, size_t len) {
  lua_lock(L);
  luaC_checkGC(L);
  setsvalue2s(L, L->top, luaS_newlstr(L, s, len));
  api_incr_top(L);
  lua_unlock(L);
}


LUA_API void lua_pushstring (lua_State *L, const char *s) {
  if (s == NULL)
    lua_pushnil(L);
  else
    lua_pushlstring(L, s, strlen(s));
}

/**
 * 等价于 lua_pushfstring ， 不过是用 va_list 接收参数，而不是用可变数量的实际参数。
 * [-0, +1, e]
 */
LUA_API const char *lua_pushvfstring (lua_State *L, const char *fmt,
                                      va_list argp) {
  const char *ret;
  lua_lock(L);
  luaC_checkGC(L);
  ret = luaO_pushvfstring(L, fmt, argp);
  lua_unlock(L);
  return ret;
}


LUA_API const char *lua_pushfstring (lua_State *L, const char *fmt, ...) {
  const char *ret;
  va_list argp;
  lua_lock(L);
  luaC_checkGC(L);
  va_start(argp, fmt);
  ret = luaO_pushvfstring(L, fmt, argp);
  va_end(argp);
  lua_unlock(L);
  return ret;
}

/**
 * 向栈中push一个C函数,n是upval的数量
 */
LUA_API void lua_pushcclosure (lua_State *L, lua_CFunction fn, int n) {
  Closure *cl;
  lua_lock(L);
  luaC_checkGC(L);
  api_checknelems(L, n);
  // 创建一个closure指针
  cl = luaF_newCclosure(L, n, getcurrenv(L));
  // 记录函数指针
  cl->c.f = fn;
  // 首先将栈空出n个位置来存放参数
  L->top -= n;
  // 初始化upval存放的位置
  while (n--)
    setobj2n(L, &cl->c.upvalue[n], L->top+n);
  // 将closure指针push到栈中
  setclvalue(L, L->top, cl);
  lua_assert(iswhite(obj2gco(cl)));
  // 栈指针加1
  api_incr_top(L);
  lua_unlock(L);
}


LUA_API void lua_pushboolean (lua_State *L, int b) {
  lua_lock(L);
  setbvalue(L->top, (b != 0));  /* ensure that true is 1 */
  api_incr_top(L);
  lua_unlock(L);
}


LUA_API void lua_pushlightuserdata (lua_State *L, void *p) {
  lua_lock(L);
  setpvalue(L->top, p);
  api_incr_top(L);
  lua_unlock(L);
}

/**
 * 把 L 表示的线程压栈。 如果这个线程是当前状态机的主线程的话，返回 1 。
 * [-0, +1, –]
 */
LUA_API int lua_pushthread (lua_State *L) {
  lua_lock(L);
  setthvalue(L, L->top, L);
  api_incr_top(L);
  lua_unlock(L);
  return (G(L)->mainthread == L);
}



/*
** get functions (Lua -> stack)
*/



// gettable会调用到luaV_gettable,这样如果在本身没找到,还会根据__index方法到基类中查找
// 而raw系列只会在自己上面查找
/**
 * 把 t[k] 的值压栈， 这里的 t 是指索引指向的值， 而 k 则是栈顶放的值。
 * 这个函数会弹出堆栈上的键，把结果放在栈上相同位置。 和在 Lua 中一样， 这个函数可能触发对应 "index" 事件的元方法 （参见 §2.4 ）。
 * 返回压入值的类型。
 * [-1, +1, e]
 */
LUA_API void lua_gettable (lua_State *L, int idx) {
  StkId t;
  lua_lock(L);
  //获取table
  t = index2adr(L, idx);
  api_checkvalidindex(L, t);
  //从table t中查找
  luaV_gettable(L, t, L->top - 1, L->top - 1);
  lua_unlock(L);
}

// 从一个table中(idx所在)查找key对应的值, 找到值存放在top - 1中返回
/**
 * 把 t[k] 的值压栈， 这里的 t 是索引指向的值。 在 Lua 中，这个函数可能触发对应 "index" 事件对应的元方法 （参见 §2.4 ）。
 * 函数将返回压入值的类型。
 */
LUA_API void lua_getfield (lua_State *L, int idx, const char *k) {
  StkId t;
  TValue key;
  lua_lock(L);
  t = index2adr(L, idx);
  api_checkvalidindex(L, t);
  setsvalue(L, &key, luaS_new(L, k));
  // 查找key对应的值, 找到存放到top中
  luaV_gettable(L, t, &key, L->top);
  api_incr_top(L);
  lua_unlock(L);
}

// 从idx存放的表中,根据top - 1存放的field名称查找表成员,并且将结果push到top - 1的栈中
// raw系列只会在自己上面查找,因为它调用的是luaH_get
// 不含转进行递归查找
LUA_API void lua_rawget (lua_State *L, int idx) {
  StkId t;
  lua_lock(L);
  t = index2adr(L, idx);
  api_check(L, ttistable(t));
  setobj2s(L, L->top - 1, luaH_get(hvalue(t), L->top - 1));
  lua_unlock(L);
}


LUA_API void lua_rawgeti (lua_State *L, int idx, int n) {
  StkId o;
  lua_lock(L);
  o = index2adr(L, idx);
  api_check(L, ttistable(o));
  setobj2s(L, L->top, luaH_getnum(hvalue(o), n));
  api_incr_top(L);
  lua_unlock(L);
}


LUA_API void lua_createtable (lua_State *L, int narray, int nrec) {
  lua_lock(L);
  luaC_checkGC(L);
  sethvalue(L, L->top, luaH_new(L, narray, nrec));
  api_incr_top(L);
  lua_unlock(L);
}


LUA_API int lua_getmetatable (lua_State *L, int objindex) {
  const TValue *obj;
  Table *mt = NULL;
  int res;
  lua_lock(L);
  obj = index2adr(L, objindex);
  switch (ttype(obj)) {
    //table
    case LUA_TTABLE:
      mt = hvalue(obj)->metatable;
      break;
    //userdata
    case LUA_TUSERDATA:
      mt = uvalue(obj)->metatable;
      break;
    //基础类型
    default:
      mt = G(L)->mt[ttype(obj)];
      break;
  }
  if (mt == NULL)
    res = 0;
  else {
    sethvalue(L, L->top, mt);
    api_incr_top(L);
    res = 1;
  }
  lua_unlock(L);
  return res;
}


LUA_API void lua_getfenv (lua_State *L, int idx) {
  StkId o;
  lua_lock(L);
  o = index2adr(L, idx);
  api_checkvalidindex(L, o);
  switch (ttype(o)) {
    case LUA_TFUNCTION:
      sethvalue(L, L->top, clvalue(o)->c.env);
      break;
    case LUA_TUSERDATA:
      sethvalue(L, L->top, uvalue(o)->env);
      break;
    case LUA_TTHREAD:
      //对于线程则直接返回state的globaltable
      setobj2s(L, L->top,  gt(thvalue(o)));
      break;
    default:
      //其他类型都没有所谓环境
      setnilvalue(L->top);
      break;
  }
  api_incr_top(L);
  lua_unlock(L);
}



/*
** set functions (stack -> Lua)
*/



// 向idx索引的表中,插入key为top - 2,val为top - 1的数据,完事了之后top - 2把k/v退栈
LUA_API void lua_settable (lua_State *L, int idx) {
  StkId t;
  lua_lock(L);
  api_checknelems(L, 2);
  t = index2adr(L, idx);
  api_checkvalidindex(L, t);
  luaV_settable(L, t, L->top - 2, L->top - 1);
  L->top -= 2;  /* pop index and value */
  lua_unlock(L);
}

// 将k与idx的值对应, 换句话说,比如用table[k]找到的是idx的值
LUA_API void lua_setfield (lua_State *L, int idx, const char *k) {
  StkId t;
  TValue key;
  lua_lock(L);
  api_checknelems(L, 1);
  // 首先寻找idx对应的值
  t = index2adr(L, idx);
  api_checkvalidindex(L, t);
  // 分配一个字符串,存放k值
  setsvalue(L, &key, luaS_new(L, k));
  // 将key的值为top - 1的值,将它们的对应关系写到t中(t一般是个table)
  luaV_settable(L, t, &key, L->top - 1);
  L->top--;  /* pop value */
  lua_unlock(L);
}


LUA_API void lua_rawset (lua_State *L, int idx) {
  StkId t;
  lua_lock(L);
  api_checknelems(L, 2);
  t = index2adr(L, idx);
  api_check(L, ttistable(t));
  setobj2t(L, luaH_set(L, hvalue(t), L->top-2), L->top-1);
  luaC_barriert(L, hvalue(t), L->top-1);
  L->top -= 2;
  lua_unlock(L);
}


LUA_API void lua_rawseti (lua_State *L, int idx, int n) {
  StkId o;
  lua_lock(L);
  api_checknelems(L, 1);
  o = index2adr(L, idx);
  api_check(L, ttistable(o));
  setobj2t(L, luaH_setnum(L, hvalue(o), n), L->top-1);
  luaC_barriert(L, hvalue(o), L->top-1);
  L->top--;
  lua_unlock(L);
}


LUA_API int lua_setmetatable (lua_State *L, int objindex) {
  TValue *obj;
  Table *mt;
  lua_lock(L);
  api_checknelems(L, 1);
  obj = index2adr(L, objindex);
  api_checkvalidindex(L, obj);
  if (ttisnil(L->top - 1))
    mt = NULL;
  else {
    api_check(L, ttistable(L->top - 1));
    mt = hvalue(L->top - 1);
  }
  switch (ttype(obj)) {
    case LUA_TTABLE: {
      hvalue(obj)->metatable = mt;
      if (mt)
        luaC_objbarriert(L, hvalue(obj), mt);
      break;
    }
    case LUA_TUSERDATA: {
      uvalue(obj)->metatable = mt;
      if (mt)
        luaC_objbarrier(L, rawuvalue(obj), mt);
      break;
    }
    default: {
      G(L)->mt[ttype(obj)] = mt;
      break;
    }
  }
  L->top--;
  lua_unlock(L);
  return 1;
}


LUA_API int lua_setfenv (lua_State *L, int idx) {
  StkId o;
  int res = 1;
  lua_lock(L);
  api_checknelems(L, 1);
  o = index2adr(L, idx);
  api_checkvalidindex(L, o);
  api_check(L, ttistable(L->top - 1));
  switch (ttype(o)) {
    case LUA_TFUNCTION:
      clvalue(o)->c.env = hvalue(L->top - 1);
      break;
    case LUA_TUSERDATA:
      uvalue(o)->env = hvalue(L->top - 1);
      break;
    case LUA_TTHREAD:
      //设置全局表
      sethvalue(L, gt(thvalue(o)), hvalue(L->top - 1));
      break;
    default:
      res = 0;
      break;
  }
  if (res) luaC_objbarrier(L, gcvalue(o), hvalue(L->top - 1));
  L->top--;
  lua_unlock(L);
  return res;
}



/*
** `load' and `call' functions (run Lua code)
*/



//如果返回值的数量为LUA_MULTRET，则将调用栈的顶设置为当前栈顶，也就是所有返回值压入栈
#define adjustresults(L,nres) \
    { if (nres == LUA_MULTRET && L->top >= L->ci->top) L->ci->top = L->top; }

//如果返回值的数量为LUA_MULTRET，检查调用栈顶和栈顶之差必须大于返回值数量与参数数量之差
#define checkresults(L,na,nr) \
     api_check(L, (nr) == LUA_MULTRET || (L->ci->top - L->top >= (nr) - (na)))
	

LUA_API void lua_call (lua_State *L, int nargs, int nresults) {
  StkId func;
  lua_lock(L);
  api_checknelems(L, nargs+1);
  checkresults(L, nargs, nresults);
  // 当前top指针后退nargs + 1个位置得到函数指针
  func = L->top - (nargs+1);
  luaD_call(L, func, nresults);
  adjustresults(L, nresults);
  lua_unlock(L);
}



/*
** Execute a protected call.
*/
struct CallS {  /* data to `f_call' */
  StkId func;
  int nresults;
};

/**
 * 调用Calls中的函数
 */
static void f_call (lua_State *L, void *ud) {
  struct CallS *c = cast(struct CallS *, ud);
  luaD_call(L, c->func, c->nresults);
}


/**
 * 以保护模式调用一个函数。
 * nargs 和 nresults 的含义与 lua_call 中的相同。 如果在调用过程中没有发生错误， lua_pcall 的行为和 lua_call 完全一致。 
 * 但是，如果有错误发生的话， lua_pcall 会捕获它， 然后把唯一的值（错误消息）压栈，然后返回错误码。 
 * 同 lua_call 一样， lua_pcall 总是把函数本身和它的参数从栈上移除。
 * 如果 msgh 是 0 ， 返回在栈顶的错误消息就和原始错误消息完全一致。 
 * 否则， msgh 就被当成是 错误处理函数 在栈上的索引位置。 （在当前的实现里，这个索引不能是伪索引。） 
 * 在发生运行时错误时， 这个函数会被调用而参数就是错误消息。 错误处理函数的返回值将被 lua_pcall 作为错误消息返回在堆栈上。
 * 典型的用法中，错误处理函数被用来给错误消息加上更多的调试信息， 比如栈跟踪信息。 这些信息在 lua_pcall 返回后， 
 * 由于栈已经展开，所以收集不到了。
 * 
 * lua_pcall 函数会返回下列常数 （定义在 lua.h 内）中的一个：
 * 
 * LUA_OK (0): 成功。
 * LUA_ERRRUN: 运行时错误。
 * LUA_ERRMEM: 内存分配错误。对于这种错，Lua 不会调用错误处理函数。
 * LUA_ERRERR: 在运行错误处理函数时发生的错误。
 * LUA_ERRGCMM: 在运行 __gc 元方法时发生的错误。 （这个错误和被调用的函数无关。）
 * 
 * [-(nargs + 1), +(nresults|1), –]
 */
LUA_API int lua_pcall (lua_State *L, int nargs, int nresults, int errfunc) {
  struct CallS c;
  int status;
  //错误函数的栈位置
  ptrdiff_t func;
  lua_lock(L);
  //检查参数数量
  api_checknelems(L, nargs+1);
  checkresults(L, nargs, nresults);
  if (errfunc == 0)
    func = 0;
  else {
    StkId o = index2adr(L, errfunc);
    api_checkvalidindex(L, o);
    //保存栈的位置
    func = savestack(L, o);
  }
  //获取函数位置
  c.func = L->top - (nargs+1);  /* function to be called */
  //获取result的数量
  c.nresults = nresults;
  //尝试调用函数
  status = luaD_pcall(L, f_call, &c, savestack(L, c.func), func);
  
  adjustresults(L, nresults);
  lua_unlock(L);
  return status;
}


/*
** Execute a protected C call.
*/
struct CCallS {  /* data to `f_Ccall' */
  lua_CFunction func;
  void *ud;
};


static void f_Ccall (lua_State *L, void *ud) {
  struct CCallS *c = cast(struct CCallS *, ud);
  Closure *cl;
  cl = luaF_newCclosure(L, 0, getcurrenv(L));
  cl->c.f = c->func;
  setclvalue(L, L->top, cl);  /* push function */
  api_incr_top(L);
  setpvalue(L->top, c->ud);  /* push only argument */
  api_incr_top(L);
  luaD_call(L, L->top - 2, 0);
}


LUA_API int lua_cpcall (lua_State *L, lua_CFunction func, void *ud) {
  struct CCallS c;
  int status;
  lua_lock(L);
  c.func = func;
  c.ud = ud;
  status = luaD_pcall(L, f_Ccall, &c, savestack(L, L->top), 0);
  lua_unlock(L);
  return status;
}


LUA_API int lua_load (lua_State *L, lua_Reader reader, void *data,
                      const char *chunkname) {
  ZIO z;
  int status;
  lua_lock(L);
  if (!chunkname) chunkname = "?";
  luaZ_init(L, &z, reader, data);
  status = luaD_protectedparser(L, &z, chunkname);
  lua_unlock(L);
  return status;
}


LUA_API int lua_dump (lua_State *L, lua_Writer writer, void *data) {
  int status;
  TValue *o;
  lua_lock(L);
  api_checknelems(L, 1);
  o = L->top - 1;
  if (isLfunction(o))
    status = luaU_dump(L, clvalue(o)->l.p, writer, data, 0);
  else
    status = 1;
  lua_unlock(L);
  return status;
}


LUA_API int  lua_status (lua_State *L) {
  return L->status;
}



/*
** Garbage-collection function
*/



LUA_API int lua_gc (lua_State *L, int what, int data) {
  int res = 0;
  global_State *g;
  lua_lock(L);
  g = G(L);
  switch (what) {
    case LUA_GCSTOP: {
      // 停止GC, 把阙值标记为一个无限大的量, 这样就无法自动GC了
      g->GCthreshold = MAX_LUMEM;
      break;
    }
    case LUA_GCRESTART: {
      // GC重新开始, 将阙值定为当前内存值
      g->GCthreshold = g->totalbytes;
      break;
    }
    case LUA_GCCOLLECT: {
      // 全回收 不管三七二十一
      luaC_fullgc(L);
      break;
    }
    case LUA_GCCOUNT: {
      /* GC values are expressed in Kbytes: #bytes/2^10 */
      // 换算成KBytes
      res = cast_int(g->totalbytes >> 10);
      break;
    }
    case LUA_GCCOUNTB: {
      // 换算成bit
      res = cast_int(g->totalbytes & 0x3ff);
      break;
    }
    case LUA_GCSTEP: {
      // 首先换算成byte
      lu_mem a = (cast(lu_mem, data) << 10);
      if (a <= g->totalbytes)
        g->GCthreshold = g->totalbytes - a;
      else
        g->GCthreshold = 0;
      while (g->GCthreshold <= g->totalbytes) {
    	  // 当当前内存数据还是大于所要求的阙值时,就一直进行回收操作
        luaC_step(L);
        // GC停止了, 返回错误
        if (g->gcstate == GCSpause) {  /* end of cycle? */
          res = 1;  /* signal it */
          break;
        }
      }
      break;
    }
    case LUA_GCSETPAUSE: {
      res = g->gcpause;
      g->gcpause = data;
      break;
    }
    case LUA_GCSETSTEPMUL: {
      res = g->gcstepmul;
      g->gcstepmul = data;
      break;
    }
    default: res = -1;  /* invalid option */
  }
  lua_unlock(L);
  return res;
}



/*
** miscellaneous functions
*/



/**
 * 以栈顶的值作为错误对象，抛出一个 Lua 错误。 这个函数将做一次长跳转，所以一定不会返回 （参见 luaL_error）。
 * [-1, +0, v]
 */
LUA_API int lua_error (lua_State *L) {
  lua_lock(L);
  api_checknelems(L, 1);
  luaG_errormsg(L);
  lua_unlock(L);
  return 0;  /* to avoid warnings */
}

/**
 * 从栈顶弹出一个键， 然后把索引指定的表中的一个键值对压栈 （弹出的键之后的 “下一” 对）。 
 * 如果表中以无更多元素， 那么 lua_next 将返回 0 （什么也不压栈）。
 */
LUA_API int lua_next (lua_State *L, int idx) {
  StkId t;
  int more;
  lua_lock(L);
  //检出table
  t = index2adr(L, idx);
  api_check(L, ttistable(t));
  //检查是否有下一个
  //TODO:lua table部分阅读
  more = luaH_next(L, hvalue(t), L->top - 1);
  if (more) {
    api_incr_top(L);
  }
  else  /* no more elements */
    L->top -= 1;  /* remove key */
  lua_unlock(L);
  return more;
}


LUA_API void lua_concat (lua_State *L, int n) {
  lua_lock(L);
  api_checknelems(L, n);
  //如果有两个或以上字符串则连接
  if (n >= 2) {
    luaC_checkGC(L);
    luaV_concat(L, n, cast_int(L->top - L->base) - 1);
    L->top -= (n-1);
  }
  //如果没有任何字符串就创建一个新的字符串
  else if (n == 0) {  /* push empty string */
    setsvalue2s(L, L->top, luaS_newlstr(L, "", 0));
    api_incr_top(L);
  }
  //否则什么都不做
  /* else n == 1; nothing to do */
  lua_unlock(L);
}

/**
 * 返回给定状态机的内存分配器函数。 
 * 如果 ud 不是 NULL ， Lua 把设置内存分配函数时设置的那个指针置入 *ud 。
 * [-0, +0, –]
 */
LUA_API lua_Alloc lua_getallocf (lua_State *L, void **ud) {
  lua_Alloc f;
  lua_lock(L);
  if (ud) *ud = G(L)->ud;
  f = G(L)->frealloc;
  lua_unlock(L);
  return f;
}

/**
 * 把指定状态机的分配器函数换成带上用户数据 ud 的 f 。
 * [-0, +0, –]
 */
LUA_API void lua_setallocf (lua_State *L, lua_Alloc f, void *ud) {
  lua_lock(L);
  G(L)->ud = ud;
  G(L)->frealloc = f;
  lua_unlock(L);
}

/**
 * 这个函数分配一块指定大小的内存块， 把内存块地址作为一个完全用户数据压栈， 并返回这个地址。 宿主程序可以随意使用这块内存。
 * [-0, +1, e]
 */
LUA_API void *lua_newuserdata (lua_State *L, size_t size) {
  Udata *u;
  lua_lock(L);
  luaC_checkGC(L);
  u = luaS_newudata(L, size, getcurrenv(L));
  setuvalue(L, L->top, u);
  api_incr_top(L);
  lua_unlock(L);
  return u + 1;
}



/**
 * 尝试获取函数的上值
 */
static const char *aux_upvalue (StkId fi, int n, TValue **val) {
  Closure *f;
  if (!ttisfunction(fi)) return NULL;
  f = clvalue(fi);
  //C函数的情况
  if (f->c.isC) {
    if (!(1 <= n && n <= f->c.nupvalues)) return NULL;
    *val = &f->c.upvalue[n-1];
    //C函数都是空串
    return "";
  }
  //Lua函数的情况
  else {
    Proto *p = f->l.p;
    if (!(1 <= n && n <= p->sizeupvalues)) return NULL;
    *val = f->l.upvals[n-1]->v;
    //返回Upvalue的名字
    return getstr(p->upvalues[n-1]);
  }
}

/**
 * 获取一个闭包的上值信息。 （对于 Lua 函数，上值是函数需要使用的外部局部变量， 因此这些变量被包含在闭包中。） 
 * lua_getupvalue 获取第 n 个上值， 把这个上值的值压栈， 并且返回它的名字。 funcindex 指向闭包在栈上的位置。 
 * （ 因为上值在整个函数中都有效，所以它们没有特别的次序。 因此，它们以字母次序来编号。）
 * 当索引号比上值数量大的时候， 返回 NULL（而且不会压入任何东西）。 对于 C 函数，所有上值的名字都是空串 ""。
 * [-0, +(0|1), –]
 */
LUA_API const char *lua_getupvalue (lua_State *L, int funcindex, int n) {
  const char *name;
  TValue *val;
  lua_lock(L);
  name = aux_upvalue(index2adr(L, funcindex), n, &val);
  if (name) {
    setobj2s(L, L->top, val);
    api_incr_top(L);
  }
  lua_unlock(L);
  return name;
}

/**
 * 设置闭包上值的值。 它把栈顶的值弹出并赋于上值并返回上值的名字。 
 * 参数 funcindex 与 n 和 lua_getupvalue 中的一样 （参见 lua_getupvalue ）。
 * 当索引大于上值的数量时，返回 NULL （什么也不弹出）。
 * [-(0|1), +0, –]
 */
LUA_API const char *lua_setupvalue (lua_State *L, int funcindex, int n) {
  const char *name;
  TValue *val;
  StkId fi;
  lua_lock(L);
  //获取函数
  fi = index2adr(L, funcindex);
  api_checknelems(L, 1);
  //获取上值
  name = aux_upvalue(fi, n, &val);
  if (name) {
    L->top--;
    //将栈顶设到upvalue
    setobj(L, val, L->top);
    luaC_barrier(L, clvalue(fi), L->top);
  }
  lua_unlock(L);
  return name;
}

