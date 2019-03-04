/*
** $Id: lobject.h,v 2.20.1.2 2008/08/06 13:29:48 roberto Exp $
** Type definitions for Lua objects
** See Copyright Notice in lua.h
*/


#ifndef lobject_h
#define lobject_h


#include <stdarg.h>


#include "llimits.h"
#include "lua.h"


/* tags for values visible from Lua */
#define LAST_TAG	LUA_TTHREAD

#define NUM_TAGS	(LAST_TAG+1)


/*
** Extra tags for non-values
*/
#define LUA_TPROTO	(LAST_TAG+1)
#define LUA_TUPVAL	(LAST_TAG+2)
#define LUA_TDEADKEY	(LAST_TAG+3)


/*
 * Union of all collectable objects
 * 可回收类型的头
 */
typedef union GCObject GCObject;


/*
 * Common Header for all collectable objects (in macro form, to be
 * included in other objects)
 * 
 * next   GCObject的指针
 * tt     数据类型
 * marked 垃圾回收标志
 */
#define CommonHeader	GCObject *next; lu_byte tt; lu_byte marked


/*
** Common header in struct form
*/
typedef struct GCheader {
  CommonHeader;
} GCheader;




/*
** Union of all Lua values
*/
typedef union {
  GCObject *gc;   //可回收对象
  void *p;        //指针
  lua_Number n;   //数字
  int b;          //bool类型
} Value;


/**
 * Tagged Values
 * 自省的Value
 * 
 * value  具体值
 * tt     数据类型
 */
#define TValuefields	Value value; int tt

typedef struct lua_TValue {
  TValuefields;
} TValue;


/* Macros to test type */
#define ttisnil(o)	(ttype(o) == LUA_TNIL)
#define ttisnumber(o)	(ttype(o) == LUA_TNUMBER)
#define ttisstring(o)	(ttype(o) == LUA_TSTRING)
#define ttistable(o)	(ttype(o) == LUA_TTABLE)
#define ttisfunction(o)	(ttype(o) == LUA_TFUNCTION)
#define ttisboolean(o)	(ttype(o) == LUA_TBOOLEAN)
#define ttisuserdata(o)	(ttype(o) == LUA_TUSERDATA)
#define ttisthread(o)	(ttype(o) == LUA_TTHREAD)
#define ttislightuserdata(o)	(ttype(o) == LUA_TLIGHTUSERDATA)

/* Macros to access values */
//获取TValue的类型
#define ttype(o)	((o)->tt)
//获取GCObject的指针
#define gcvalue(o)	check_exp(iscollectable(o), (o)->value.gc)
//获取lightuserdata
#define pvalue(o)	check_exp(ttislightuserdata(o), (o)->value.p)
//获取TValue中的number
#define nvalue(o)	check_exp(ttisnumber(o), (o)->value.n)
//获取TValue中的字符串TString
#define rawtsvalue(o)	check_exp(ttisstring(o), &(o)->value.gc->ts)
//获取TString中的tsv（TableStringValue）
#define tsvalue(o)	(&rawtsvalue(o)->tsv)
//获取GCObject中的UData
#define rawuvalue(o)	check_exp(ttisuserdata(o), &(o)->value.gc->u)
//获取UData中的uv 也就是实际的UserData
#define uvalue(o)	(&rawuvalue(o)->uv)
//获取TValue中的clousure值
#define clvalue(o)	check_exp(ttisfunction(o), &(o)->value.gc->cl)
//获取TValue中的table值
#define hvalue(o)	check_exp(ttistable(o), &(o)->value.gc->h)
//获取TValue的bool值
#define bvalue(o)	check_exp(ttisboolean(o), (o)->value.b)
#define thvalue(o)	check_exp(ttisthread(o), &(o)->value.gc->th)
//获取栈顶的值 判断是否为false
#define l_isfalse(o)	(ttisnil(o) || (ttisboolean(o) && bvalue(o) == 0))

/*
** for internal debug only
*/
#define checkconsistency(obj) \
  lua_assert(!iscollectable(obj) || (ttype(obj) == (obj)->value.gc->gch.tt))

#define checkliveness(g,obj) \
  lua_assert(!iscollectable(obj) || \
  ((ttype(obj) == (obj)->value.gc->gch.tt) && !isdead(g, (obj)->value.gc)))


/* Macros to set values */
//将某个值设为空
#define setnilvalue(obj) ((obj)->tt=LUA_TNIL)

//设置number
#define setnvalue(obj,x) \
  { TValue *i_o=(obj); i_o->value.n=(x); i_o->tt=LUA_TNUMBER; }

//设置lightuserdata
#define setpvalue(obj,x) \
  { TValue *i_o=(obj); i_o->value.p=(x); i_o->tt=LUA_TLIGHTUSERDATA; }

//设置bool
#define setbvalue(obj,x) \
  { TValue *i_o=(obj); i_o->value.b=(x); i_o->tt=LUA_TBOOLEAN; }

//设置string
#define setsvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TSTRING; \
    checkliveness(G(L),i_o); }

//设置userdata
#define setuvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TUSERDATA; \
    checkliveness(G(L),i_o); }

//设置thread
#define setthvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TTHREAD; \
    checkliveness(G(L),i_o); }

//设置closure
#define setclvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TFUNCTION; \
    checkliveness(G(L),i_o); }

//设置table
#define sethvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TTABLE; \
    checkliveness(G(L),i_o); }

//设置proto
#define setptvalue(L,obj,x) \
  { TValue *i_o=(obj); \
    i_o->value.gc=cast(GCObject *, (x)); i_o->tt=LUA_TPROTO; \
    checkliveness(G(L),i_o); }



//将*obj2赋给*obj1
#define setobj(L,obj1,obj2) \
  { const TValue *o2=(obj2); TValue *o1=(obj1); \
    o1->value = o2->value; o1->tt=o2->tt; \
    checkliveness(G(L),o1); }


/*
** different types of sets, according to destination
*/

/* from stack to (same) stack */
#define setobjs2s	setobj
/* to stack (not from same stack) */
#define setobj2s	setobj
#define setsvalue2s	setsvalue
#define sethvalue2s	sethvalue
#define setptvalue2s	setptvalue
/* from table to same table */
#define setobjt2t	setobj
/* to table */
#define setobj2t	setobj
/* to new object */
#define setobj2n	setobj
#define setsvalue2n	setsvalue

#define setttype(obj, tt) (ttype(obj) = (tt))

// 只有这些类型的数据 才是可回收的数据
#define iscollectable(o)	(ttype(o) >= LUA_TSTRING)


//实际就是TValue的地址
typedef TValue *StkId;  /* index to stack elements */


/**
 * String headers for string table
 * 表示字符串的头
 */
typedef union TString {
  L_Umaxalign dummy;    //保证了内存对齐  /* ensures maximum alignment for strings */
  struct {
    CommonHeader;       //GC 类型头
    lu_byte reserved;   //是否是保留字
    unsigned int hash;  //字符串的hash值
    size_t len;         //字符串长度
  } tsv;
} TString;

//将TString转为const char*
#define getstr(ts)	cast(const char *, (ts) + 1)
//将TString转为const char*
#define svalue(o)       getstr(rawtsvalue(o))


// 这里为什么需要使用union类型？
typedef union Udata {
  L_Umaxalign dummy;        //保证内存对齐  /* ensures maximum alignment for `local' udata */
  struct {
    CommonHeader;           //GC头
    struct Table *metatable;//metatable
    struct Table *env;      //环境
    size_t len;             //userdata的长度
  } uv;
} Udata;




/*
** Function Prototypes
*/
// 存放函数原型的数据结构
typedef struct Proto {
  CommonHeader; //GC头
  //函数需要使用的常量
  TValue *k;  /* constants used by the function */
  // 存放函数体的opcode
  Instruction *code;
  // 在这个函数中定义的函数
  struct Proto **p;  /* functions defined inside the function */
  //代码行号信息
  int *lineinfo;  /* map from opcodes to source lines */
  // 存放局部变量的数组
  struct LocVar *locvars;  /* information about local variables */
  //上值的名字
  TString **upvalues;  /* upvalue names */
  //
  TString  *source;
  int sizeupvalues;
  int sizek;  /* size of `k' */
  int sizecode;
  int sizelineinfo;
  int sizep;  /* size of `p' */
  int sizelocvars;
  int linedefined;
  int lastlinedefined;
  GCObject *gclist;
  lu_byte nups;  /* number of upvalues */
  //需求的参数个数
  lu_byte numparams;
  //是否为变长参数
  lu_byte is_vararg;
  lu_byte maxstacksize;
} Proto;


/* masks for new-style vararg */
#define VARARG_HASARG		1
#define VARARG_ISVARARG		2
#define VARARG_NEEDSARG		4

// 存放局部变量的结构体
typedef struct LocVar {
  //局部变量名
  TString *varname;
  int startpc;  /* first point where variable is active */
  int endpc;    /* first point where variable is dead */
} LocVar;



/*
** Upvalues
*/

typedef struct UpVal {
  CommonHeader;
  TValue *v;  /* points to stack or to its own value */
  union {
	  // 当这个upval被close时,保存upval的值,后面可能还会被引用到
    // TODO:为啥要存关闭后的upvalue值？是不是关闭的时候有可能其他地方还会引用？
    TValue value;  /* the value (when closed) */
    // 当这个upval还在open状态时,以下链表串连在openupval链表中
    struct {  /* double linked list (when open) */
      struct UpVal *prev;
      struct UpVal *next;
    } l;
  } u;
} UpVal;


/*
** Closures
*/

/**
 * isC        是否是C function
 * nupvalues  upvalue的数量
 * gclist     TODO:GCList
 */

#define ClosureHeader \
	CommonHeader; lu_byte isC; lu_byte nupvalues; GCObject *gclist; \
	struct Table *env

typedef struct CClosure {
  ClosureHeader;
  lua_CFunction f;
  TValue upvalue[1];
} CClosure;


typedef struct LClosure {
  ClosureHeader;
  struct Proto *p;
  UpVal *upvals[1];
} LClosure;


typedef union Closure {
  CClosure c;
  LClosure l;
} Closure;

//是否是C函数
#define iscfunction(o)	(ttype(o) == LUA_TFUNCTION && clvalue(o)->c.isC)
//是否是Lua函数
#define isLfunction(o)	(ttype(o) == LUA_TFUNCTION && !clvalue(o)->c.isC)


/*
** Tables
*/

typedef union TKey {
  struct {
    TValuefields;
    struct Node *next;  /* for chaining */
  } nk;
  TValue tvk;
} TKey;

// 每个节点都有key和val
typedef struct Node {
  TValue i_val;
  TKey i_key;
} Node;


typedef struct Table {
  CommonHeader;
  //1<<p means tagmethod(p) is not present
  //通过位移的方式表示没有这个元方法
  lu_byte flags;  /* 1<<p means tagmethod(p) is not present */ 
  //log2 of size of `node' array
  lu_byte lsizenode;  /* log2 of size of `node' array */
  struct Table *metatable;
  TValue *array;  /* array part */
  Node *node;
  Node *lastfree;  /* any free position is before this position */
  GCObject *gclist;
  int sizearray;  /* size of `array' array */
} Table;



/*
** `module' operation for hashing (size is always a power of 2)
*/
// (size&(size-1))==0是检查size是2的次幂
// (s) & ((size)-1)) = s % size
#define lmod(s,size) \
	(check_exp((size&(size-1))==0, (cast(int, (s) & ((size)-1)))))


#define twoto(x)	(1<<(x))
// sizenode返回的值必然是2的次幂
#define sizenode(t)	(twoto((t)->lsizenode))


#define luaO_nilobject		(&luaO_nilobject_)

LUAI_DATA const TValue luaO_nilobject_;

#define ceillog2(x)	(luaO_log2((x)-1) + 1)

LUAI_FUNC int luaO_log2 (unsigned int x);
LUAI_FUNC int luaO_int2fb (unsigned int x);
LUAI_FUNC int luaO_fb2int (int x);
LUAI_FUNC int luaO_rawequalObj (const TValue *t1, const TValue *t2);
LUAI_FUNC int luaO_str2d (const char *s, lua_Number *result);
LUAI_FUNC const char *luaO_pushvfstring (lua_State *L, const char *fmt,
                                                       va_list argp);
LUAI_FUNC const char *luaO_pushfstring (lua_State *L, const char *fmt, ...);
LUAI_FUNC void luaO_chunkid (char *out, const char *source, size_t len);


#endif

