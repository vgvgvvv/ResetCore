/*
** $Id: lstate.h,v 2.24.1.2 2008/01/03 15:20:39 roberto Exp $
** Global State
** See Copyright Notice in lua.h
*/

#ifndef lstate_h
#define lstate_h

#include "lua.h"

#include "lobject.h"
#include "ltm.h"
#include "lzio.h"


//长跳转
struct lua_longjmp;  /* defined in ldo.c */


/* table of globals */
//全局表
#define gt(L)	(&L->l_gt)

/* registry */
//注册表
#define registry(L)	(&G(L)->l_registry)


/* extra stack space to handle TM calls and some other extras */
#define EXTRA_STACK   5

//初始栈大小
#define BASIC_CI_SIZE           8

#define BASIC_STACK_SIZE        (2*LUA_MINSTACK)


//字符串表
typedef struct stringtable {
  GCObject **hash;
  //数量大小
  //元素的数量
  lu_int32 nuse;  /* number of elements */
  int size;       // hash桶数组大小
} stringtable;


/*
** informations about a call
** 调用信息
*/
typedef struct CallInfo {
  //函数栈底
  StkId base;  /* base for this function */ 
  //函数在栈中的位置
  StkId func;  /* function index in the stack */ 
  //函数栈顶
  StkId	top;  /* top for this function */ 
  //TODO:保存的dpc
  const Instruction *savedpc;
  //返回结果的数量
  int nresults;  /* expected number of results from this function */ 
  //尾调用数量
  int tailcalls;  /* number of tail calls lost under this entry */  
} CallInfo;


//获取当前函数
#define curr_func(L)	(clvalue(L->ci->func))
//获取callinfo的函数
#define ci_func(ci)	(clvalue((ci)->func))
//获取callinfo是否为Lua函数
#define f_isLua(ci)	(!ci_func(ci)->c.isC)
//获取callinfo是否为Lua函数，会检查是否是function类型
#define isLua(ci)	(ttisfunction((ci)->func) && f_isLua(ci))


/*
** `global state', shared by all threads of this state
*/
typedef struct global_State {
  stringtable strt;  /* hash table for strings */
  //申请内存的函数指针
  lua_Alloc frealloc;  /* function to reallocate memory */
  void *ud;         /* auxiliary data to `frealloc' */
  lu_byte currentwhite;
  lu_byte gcstate;  /* state of garbage collector */
  int sweepstrgc;  /* position of sweep in `strt' */
  GCObject *rootgc;  /* list of all collectable objects */
  GCObject **sweepgc;  /* position of sweep in `rootgc' */
  GCObject *gray;  /* list of gray objects */
  GCObject *grayagain;  /* list of objects to be traversed atomically */
  GCObject *weak;  /* list of weak tables (to be cleared) */
  // 所有有GC方法的udata都放在tmudata链表中
  GCObject *tmudata;  /* last element of list of userdata to be GC */
  //用于字符串连接的缓冲buffer
  Mbuffer buff;  /* temporary buffer for string concatentation */
  // 一个阈值，当这个totalbytes大于这个阈值时进行自动GC
  lu_mem GCthreshold;
  // 保存当前分配的总内存数量
  lu_mem totalbytes;  /* number of bytes currently allocated */
  // 一个估算值，根据这个计算GCthreshold
  lu_mem estimate;  /* an estimate of number of bytes actually in use */
  // 当前待GC的数据大小，其实就是累加totalbytes和GCthreshold的差值
  lu_mem gcdept;  /* how much GC is `behind schedule' */
  // 可以配置的一个值，不是计算出来的，根据这个计算GCthreshold，以此来控制下一次GC触发的时间
  int gcpause;  /* size of pause between successive GCs */
  // 每次进行GC操作回收的数据比例，见lgc.c/luaC_step函数
  int gcstepmul;  /* GC `granularity' */
  lua_CFunction panic;  /* to be called in unprotected errors */
  TValue l_registry;
  struct lua_State *mainthread;
  UpVal uvhead;  /* head of double-linked list of all open upvalues */
  //基础类型的元方法
  struct Table *mt[NUM_TAGS];  /* metatables for basic types */
  // 内置元方法的名字数组
  TString *tmname[TM_N];  /* array with tag-method names */
} global_State;


/**
 * `per thread' state
 * 
 * stack结构
 * base与top之间为当前函数的调用栈
 * stackbase xxxx base xxxx top xxxx stack_last
 */
struct lua_State {
  CommonHeader;
  //当前的调用状态
  lu_byte status;
  //当前第一个空闲的栈位置
  StkId top;  /* first free slot in the stack */
  //当前函数调用栈底
  StkId base;  /* base of current function */
  //全局表
  global_State *l_G;
  //当前callinfo的栈
  CallInfo *ci;  /* call info for current function */
  //当前函数的dpc
  const Instruction *savedpc;  /* `savedpc' of current function */

  //全局栈的最后一个位置
  StkId stack_last;  /* last free slot in the stack */
  //是一个Tvalue数组
  StkId stack;  /* stack base */

  //CallInfo数组的尾部指针
  CallInfo *end_ci;  /* points after end of ci array  */
  //CallInfo数组的头指针
  CallInfo *base_ci;  /* array of CallInfo's  */

  int stacksize;
  //Callinfo数组的大小
  int size_ci;  /* size of array `base_ci' */
  /* number of nested C calls */
  unsigned short nCcalls;  
  /* nested C calls when resuming coroutine */
  unsigned short baseCcalls;  
  lu_byte hookmask;
  lu_byte allowhook;
  int basehookcount;
  int hookcount;
  lua_Hook hook;
  TValue l_gt;  /* table of globals 全局表 */
  TValue env;  /* temporary place for environments */
  //该栈中的upvaluelist
  GCObject *openupval;  /* list of open upvalues in this stack */
  GCObject *gclist;
  struct lua_longjmp *errorJmp;  /* current error recover point */
  ptrdiff_t errfunc;  /* current error handling function (stack index) */
};

//获取全局状态机
#define G(L)	(L->l_G)


/*
** Union of all collectable objects
*/
union GCObject {
  GCheader gch;
  union TString ts;     //字符串
  union Udata u;        //userdata
  union Closure cl;     //闭包
  struct Table h;       //表
  struct Proto p;       //proto
  struct UpVal uv;      //upvalue
  struct lua_State th;  /* thread */
};


/* macros to convert a GCObject into a specific value */
//gcobject -> Tstring
#define rawgco2ts(o)	check_exp((o)->gch.tt == LUA_TSTRING, &((o)->ts))
//gcobject -> Tstring.tsv
#define gco2ts(o)	(&rawgco2ts(o)->tsv)
//gcobject -> userdata
#define rawgco2u(o)	check_exp((o)->gch.tt == LUA_TUSERDATA, &((o)->u))
//gcobject -> userdata.uv
#define gco2u(o)	(&rawgco2u(o)->uv)
//gcobejct -> closure
#define gco2cl(o)	check_exp((o)->gch.tt == LUA_TFUNCTION, &((o)->cl))
//gcobject -> table
#define gco2h(o)	check_exp((o)->gch.tt == LUA_TTABLE, &((o)->h))
//gcobject -> proto
#define gco2p(o)	check_exp((o)->gch.tt == LUA_TPROTO, &((o)->p))
//gcobject -> upvalue
#define gco2uv(o)	check_exp((o)->gch.tt == LUA_TUPVAL, &((o)->uv))
#define ngcotouv(o) \
	check_exp((o) == NULL || (o)->gch.tt == LUA_TUPVAL, &((o)->uv))
//gcobject -> thread
#define gco2th(o)	check_exp((o)->gch.tt == LUA_TTHREAD, &((o)->th))

/* macro to convert any Lua object into a GCObject */
//LuaObject转成GCObject
#define obj2gco(v)	(cast(GCObject *, (v)))


LUAI_FUNC lua_State *luaE_newthread (lua_State *L);
LUAI_FUNC void luaE_freethread (lua_State *L, lua_State *L1);

#endif
