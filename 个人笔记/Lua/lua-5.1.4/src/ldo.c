/*
** $Id: ldo.c,v 2.38.1.3 2008/01/18 22:31:22 roberto Exp $
** Stack and Call structure of Lua
** See Copyright Notice in lua.h
*/


#include <setjmp.h>
#include <stdlib.h>
#include <string.h>

#define ldo_c
#define LUA_CORE

#include "lua.h"

#include "ldebug.h"
#include "ldo.h"
#include "lfunc.h"
#include "lgc.h"
#include "lmem.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lparser.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"
#include "ltm.h"
#include "lundump.h"
#include "lvm.h"
#include "lzio.h"




/*
** {======================================================
** Error-recovery functions
** =======================================================
*/


/* chain list of long jump buffers */
//用于长跳转
struct lua_longjmp {
  struct lua_longjmp *previous;
  //luai_jmpbuff
  luai_jmpbuf b;
  //状态码
  volatile int status;  /* error code */
};

//不同的错误方式使用不同的处理方式
void luaD_seterrorobj (lua_State *L, int errcode, StkId oldtop) {
  switch (errcode) {
    case LUA_ERRMEM: {
      //往栈里面压入错误信息
      setsvalue2s(L, oldtop, luaS_newliteral(L, MEMERRMSG));
      break;
    }
    case LUA_ERRERR: {
      //
      setsvalue2s(L, oldtop, luaS_newliteral(L, "error in error handling"));
      break;
    }
    case LUA_ERRSYNTAX:
    case LUA_ERRRUN: {
      //运行错误在放在栈顶
      setobjs2s(L, oldtop, L->top - 1);  /* error message on current top */
      break;
    }
  }
  L->top = oldtop + 1;
}

//恢复栈大小到最大限制值以下
static void restore_stack_limit (lua_State *L) {
  lua_assert(L->stack_last - L->stack == L->stacksize - EXTRA_STACK - 1);
  if (L->size_ci > LUAI_MAXCALLS) {  /* there was an overflow? */
    //正在使用的栈内容数量
    int inuse = cast_int(L->ci - L->base_ci);
    if (inuse + 1 < LUAI_MAXCALLS)  /* can `undo' overflow? */
      luaD_reallocCI(L, LUAI_MAXCALLS);
  }
}

//重置stack
static void resetstack (lua_State *L, int status) {
  L->ci = L->base_ci;
  L->base = L->ci->base;
  luaF_close(L, L->base);  /* close eventual pending closures */
  luaD_seterrorobj(L, status, L->base);
  L->nCcalls = L->baseCcalls;
  L->allowhook = 1;
  restore_stack_limit(L);
  L->errfunc = 0;
  L->errorJmp = NULL;
}

/**
 * 抛出异常
 */
void luaD_throw (lua_State *L, int errcode) {
  //如果有跳跃点，则跳回
  if (L->errorJmp) {
    L->errorJmp->status = errcode;
    LUAI_THROW(L, L->errorJmp);
  }
  //否则退出进程
  else {
    //获取错误码
    L->status = cast_byte(errcode);
    if (G(L)->panic) {
      resetstack(L, errcode);
      lua_unlock(L);
      //运行panic
      G(L)->panic(L);
    }
    exit(EXIT_FAILURE);
  }
}

// 任何需要保护jmp的调用,都要用这个函数保护
int luaD_rawrunprotected (lua_State *L, Pfunc f, void *ud) {
  struct lua_longjmp lj;
  lj.status = 0;
  //记录上一个长跳转的点
  lj.previous = L->errorJmp;  /* chain new error handler */
  L->errorJmp = &lj;
  LUAI_TRY(L, &lj,
    (*f)(L, ud);
  );
  L->errorJmp = lj.previous;  /* restore old error handler */
  return lj.status;
}

/* }====================================================== */

//在伸缩栈大小后修正数据栈的位置
//因为在realloc之后的头指针位置可能会发生改变
static void correctstack (lua_State *L, TValue *oldstack) {
  CallInfo *ci;
  GCObject *up;
  L->top = (L->top - oldstack) + L->stack;
  //修正upvalue的位置
  for (up = L->openupval; up != NULL; up = up->gch.next){
    gco2uv(up)->v = (gco2uv(up)->v - oldstack) + L->stack;
  }
  //修正callinfo中的栈位置
  for (ci = L->base_ci; ci <= L->ci; ci++) {
    ci->top = (ci->top - oldstack) + L->stack;
    ci->base = (ci->base - oldstack) + L->stack;
    ci->func = (ci->func - oldstack) + L->stack;
  }
  L->base = (L->base - oldstack) + L->stack;
}

/**
 * 伸缩栈大小
 */
void luaD_reallocstack (lua_State *L, int newsize) {
  TValue *oldstack = L->stack;
  int realsize = newsize + 1 + EXTRA_STACK;
  lua_assert(L->stack_last - L->stack == L->stacksize - EXTRA_STACK - 1);
  luaM_reallocvector(L, L->stack, L->stacksize, realsize, TValue);
  L->stacksize = realsize;
  L->stack_last = L->stack+newsize;
  correctstack(L, oldstack);
}

/**
 * 重新申请callinfo尺寸
 */
void luaD_reallocCI (lua_State *L, int newsize) {
  //头指针
  CallInfo *oldci = L->base_ci;
  luaM_reallocvector(L, L->base_ci, L->size_ci, newsize, CallInfo);
  L->size_ci = newsize;
  L->ci = (L->ci - oldci) + L->base_ci;
  //更新尾尾指针
  L->end_ci = L->base_ci + L->size_ci - 1;
}

/**
 * 栈的增长方式总是x2，如果不够则添加上不够的部分
 */
void luaD_growstack (lua_State *L, int n) {
  if (n <= L->stacksize)  /* double size is enough? */
    luaD_reallocstack(L, 2*L->stacksize);
  else
    luaD_reallocstack(L, L->stacksize + n);
}

// 增长ci数组的size
static CallInfo *growCI (lua_State *L) {
  if (L->size_ci > LUAI_MAXCALLS)  /* overflow while handling overflow? */
	  // 过大了
    luaD_throw(L, LUA_ERRERR);
  else {
	  // 加大一倍
    luaD_reallocCI(L, 2*L->size_ci);
    //如果过大则栈溢出
    if (L->size_ci > LUAI_MAXCALLS)
      luaG_runerror(L, "stack overflow");
  }
  return ++L->ci;
}


void luaD_callhook (lua_State *L, int event, int line) {
  lua_Hook hook = L->hook;
  if (hook && L->allowhook) {
    ptrdiff_t top = savestack(L, L->top);
    ptrdiff_t ci_top = savestack(L, L->ci->top);
    lua_Debug ar;
    ar.event = event;
    ar.currentline = line;
    if (event == LUA_HOOKTAILRET)
      ar.i_ci = 0;  /* tail call; no debug information about it */
    else
      ar.i_ci = cast_int(L->ci - L->base_ci);
    luaD_checkstack(L, LUA_MINSTACK);  /* ensure minimum stack size */
    L->ci->top = L->top + LUA_MINSTACK;
    lua_assert(L->ci->top <= L->stack_last);
    L->allowhook = 0;  /* cannot call hooks inside a hook */
    lua_unlock(L);
    (*hook)(L, &ar);
    lua_lock(L);
    lua_assert(!L->allowhook);
    L->allowhook = 1;
    L->ci->top = restorestack(L, ci_top);
    L->top = restorestack(L, top);
  }
}

/**
 * 根据函数的参数数量调整base和top指针位置
 */
static StkId adjust_varargs (lua_State *L, Proto *p, int actual) {
  int i;
  // numparams是函数的参数数量
  int nfixargs = p->numparams;
  Table *htab = NULL;
  StkId base, fixed;
  // 把没有赋值的函数参数值置nil
  for (; actual < nfixargs; ++actual)
    setnilvalue(L->top++);
#if defined(LUA_COMPAT_VARARG)
  if (p->is_vararg & VARARG_NEEDSARG) { /* compat. with old-style vararg? */
    int nvar = actual - nfixargs;  /* number of extra arguments */
    lua_assert(p->is_vararg & VARARG_HASARG);
    luaC_checkGC(L);
    htab = luaH_new(L, nvar, 1);  /* create `arg' table */
    for (i=0; i<nvar; i++)  /* put extra arguments into `arg' table */
      setobj2n(L, luaH_setnum(L, htab, i+1), L->top - nvar + i);
    /* store counter in field `n' */
    setnvalue(luaH_setstr(L, htab, luaS_newliteral(L, "n")), cast_num(nvar));
  }
#endif
  /* move fixed parameters to final position */
  fixed = L->top - actual;  /* first fixed argument */
  // base指针指向最后一个函数参数的位置
  base = L->top;  /* final position of first argument */
  // OK, 逐个把函数传入的参数挪到局部变量处, 并且把原来传入的参数置nil
  for (i=0; i<nfixargs; i++) {
    setobjs2s(L, L->top++, fixed+i);
    setnilvalue(fixed+i);
  }
  /* add `arg' parameter */
  if (htab) {
    sethvalue(L, L->top++, htab);
    lua_assert(iswhite(obj2gco(htab)));
  }
  return base;
}

/**
 * 尝试获取call的元方法,并且插入栈中
 */
static StkId tryfuncTM (lua_State *L, StkId func) {
  //获取元方法
  const TValue *tm = luaT_gettmbyobj(L, func, TM_CALL);
  StkId p;
  //保存函数在栈中的位置
  ptrdiff_t funcr = savestack(L, func);
  //如果元方法不是一个方法则报错
  if (!ttisfunction(tm)){
      luaG_typeerror(L, func, "call");
  }
  /* Open a hole inside the stack at `func' */
  //在栈中func的后面再加入一个空格
  for (p = L->top; p > func; p--) {
    setobjs2s(L, p, p-1);
  }
  incr_top(L);
  //恢复栈
  func = restorestack(L, funcr);  /* previous call may change stack */
  //这是要调用的新方法
  setobj2s(L, func, tm);  /* tag method is the new function to be called */
  return func;
}


//返回一个车新的callinfo，如果目前已经不够用了就增长ci数组长度
#define inc_ci(L) \
  ((L->ci == L->end_ci) ? growCI(L) : \
   (condhardstacktests(luaD_reallocCI(L, L->size_ci)), ++L->ci))

// 函数调用的预处理, func是函数closure所在位置, nresults是返回值数量
// 
int luaD_precall (lua_State *L, StkId func, int nresults) {
  LClosure *cl;
  ptrdiff_t funcr;
  /* `func' is not a function? */
  if (!ttisfunction(func)) {
    //检查元方法
    func = tryfuncTM(L, func);  /* check the `function' tag method */
  }
  // 首先计算函数指针距离stack的偏移量
  funcr = savestack(L, func);
  // 获取closure指针
  cl = &clvalue(func)->l;
  // 保存PC位置
  L->ci->savedpc = L->savedpc;
  //Lua函数则准备调用
  if (!cl->isC) {  /* Lua function? prepare its call */
    CallInfo *ci;
    StkId st, base;
    Proto *p = cl->p;
    //检查栈空间
    luaD_checkstack(L, p->maxstacksize);
    //保存函数位置
    func = restorestack(L, funcr);
    //非变长参数，根据参数数量来压入参数
    if (!p->is_vararg) {  /* no varargs? */
      base = func + 1;
      if (L->top > base + p->numparams)
        L->top = base + p->numparams;
    }
    //变长参数，将所有的参数都压入
    else {  /* vararg function */
      int nargs = cast_int(L->top - func) - 1;
      base = adjust_varargs(L, p, nargs);
      func = restorestack(L, funcr);  /* previous call may change the stack */
    }
    // 存放新的函数信息
    // 首先从callinfo数组中分配出一个新的callinfo
    ci = inc_ci(L);  /* now `enter' new function */
    //初始化callinfo
    ci->func = func;
    L->base = ci->base = base;
    ci->top = L->base + p->maxstacksize;
    //不允许栈溢出
    lua_assert(ci->top <= L->stack_last);
    // 改变代码执行的路径
    L->savedpc = p->code;  /* starting point */
    //尾调用为0
    ci->tailcalls = 0;
    ci->nresults = nresults;
    //保证函数栈的空闲位置始终为nil
    for (st = L->top; st < ci->top; st++){
      setnilvalue(st);
    }
    //栈空闲位置设置为callinfo顶
    L->top = ci->top;
    //TODO:hook相关
    if (L->hookmask & LUA_MASKCALL) {
      L->savedpc++;  /* hooks assume 'pc' is already incremented */
      luaD_callhook(L, LUA_HOOKCALL, -1);
      L->savedpc--;  /* correct 'pc' */
    }
    return PCRLUA;
  }
  //C函数则直接调用
  else {  /* if is a C function, call it */
    CallInfo *ci;
    int n;
    luaD_checkstack(L, LUA_MINSTACK);  /* ensure minimum stack size */
    // 从CallInfo数组中返回一个CallInfo指针
    ci = inc_ci(L);  /* now `enter' new function */
    // 根据之前保存的偏移量从栈中得到函数地址
    ci->func = restorestack(L, funcr);
    L->base = ci->base = ci->func + 1;
    ci->top = L->top + LUA_MINSTACK;
    lua_assert(ci->top <= L->stack_last);
    // 期待返回多少个返回值
    ci->nresults = nresults;
    if (L->hookmask & LUA_MASKCALL)
      luaD_callhook(L, LUA_HOOKCALL, -1);
    lua_unlock(L);
    // 调用C函数
    n = (*curr_func(L)->c.f)(L);  /* do the actual call */
    lua_lock(L);
    if (n < 0)  /* yielding? */
      return PCRYIELD;
    else {
      // 调用结束之后的处理
      luaD_poscall(L, L->top - n);
      return PCRC;
    }
  }
}


static StkId callrethooks (lua_State *L, StkId firstResult) {
  ptrdiff_t fr = savestack(L, firstResult);  /* next call may change stack */
  luaD_callhook(L, LUA_HOOKRET, -1);
  if (f_isLua(L->ci)) {  /* Lua function? */
    while ((L->hookmask & LUA_MASKRET) && L->ci->tailcalls--) /* tail calls */
      luaD_callhook(L, LUA_HOOKTAILRET, -1);
  }
  return restorestack(L, fr);
}

/**
 * 结束完一次函数调用(无论是C还是lua函数)的处理, firstResult是函数第一个返回值的地址
 * 获取调用方的堆栈并且将返回值压入调用方的堆栈
 */
int luaD_poscall (lua_State *L, StkId firstResult) {
  StkId res;
  int wanted, i;
  CallInfo *ci;
  if (L->hookmask & LUA_MASKRET)
    firstResult = callrethooks(L, firstResult);
  // 得到当时的CallInfo指针
  ci = L->ci--;
  res = ci->func;  /* res == final position of 1st result */
  // 本来需要有多少返回值
  wanted = ci->nresults;
  // 把base和savepc指针置回调用前的位置
  L->base = (ci - 1)->base;  /* restore base */
  L->savedpc = (ci - 1)->savedpc;  /* restore savedpc */
  /* move results to correct place */
  // 返回值压入栈中
  for (i = wanted; i != 0 && firstResult < L->top; i--){
    setobjs2s(L, res++, firstResult++);
  }
  // 剩余的返回值置nil
  while (i-- > 0){
    setnilvalue(res++);
  }
  // 可以将top指针置回调用之前的位置了
  L->top = res;
  return (wanted - LUA_MULTRET);  /* 0 if wanted == LUA_MULTRET */
}


/**
** Call a function (C or Lua). The function to be called is at *func.
** The arguments are on the stack, right after the function.
** When returns, all the results are on the stack, starting at the original
** function position.
*  TODO:luaD_call调用过程
*/ 
void luaD_call (lua_State *L, StkId func, int nResults) {
  // 函数调用栈数量+1, 判断函数调用栈是不是过长
  if (++L->nCcalls >= LUAI_MAXCCALLS) {
    if (L->nCcalls == LUAI_MAXCCALLS){
      luaG_runerror(L, "C stack overflow");
    }
    else if (L->nCcalls >= (LUAI_MAXCCALLS + (LUAI_MAXCCALLS>>3))){
      luaD_throw(L, LUA_ERRERR);  /* error while handing stack error */
    }
  }
  /* is a Lua function? */
  if (luaD_precall(L, func, nResults) == PCRLUA)  {
    //如果是lua函数则进入虚拟机字节码解析循环
    luaV_execute(L, 1);  /* call it */
  }
  // 调用完毕, 函数调用栈-1
  L->nCcalls--;
  //检查GC
  luaC_checkGC(L);
}

/**
 * 从yield状态中返回
 */
static void resume (lua_State *L, void *ud) {
  StkId firstArg = cast(StkId, ud);
  CallInfo *ci = L->ci;
  if (L->status == 0) {  /* start coroutine? */
	  // 协程第一次运行的情况
    lua_assert(ci == L->base_ci && firstArg > L->base);
    if (luaD_precall(L, firstArg - 1, LUA_MULTRET) != PCRLUA)
      return;
  }
  else {  /* resuming from previous yield */
	  // 从之前的状态中恢复
    lua_assert(L->status == LUA_YIELD);
    L->status = 0;
    if (!f_isLua(ci)) {  /* `common' yield? */
      /* finish interrupted execution of `OP_CALL' */
      lua_assert(GET_OPCODE(*((ci-1)->savedpc - 1)) == OP_CALL ||
                 GET_OPCODE(*((ci-1)->savedpc - 1)) == OP_TAILCALL);
      if (luaD_poscall(L, firstArg))  /* complete it... */
        L->top = L->ci->top;  /* and correct top if not multiple results */
    }
    else  /* yielded inside a hook: just continue its execution */
      L->base = L->ci->base;
  }
  luaV_execute(L, cast_int(L->ci - L->base_ci));
}

/**
 * resume错误
 */
static int resume_error (lua_State *L, const char *msg) {
  L->top = L->ci->base;
  setsvalue2s(L, L->top, luaS_new(L, msg));
  incr_top(L);
  lua_unlock(L);
  return LUA_ERRRUN;
}

/**
 * lua的resume操作
 */
LUA_API int lua_resume (lua_State *L, int nargs) {
  int status;
  lua_lock(L);
  // 检查状态
  if (L->status != LUA_YIELD && (L->status != 0 || L->ci != L->base_ci))
      return resume_error(L, "cannot resume non-suspended coroutine");
  // 函数调用层次太多
  if (L->nCcalls >= LUAI_MAXCCALLS)
    return resume_error(L, "C stack overflow");
  luai_userstateresume(L, nargs);
  lua_assert(L->errfunc == 0);
  // 调用之前递增函数调用层次
  L->baseCcalls = ++L->nCcalls;
  // 以保护模式调用函数
  status = luaD_rawrunprotected(L, resume, L->top - nargs);
  if (status != 0) {  /* error? */
    L->status = cast_byte(status);  /* mark thread as `dead' */
    luaD_seterrorobj(L, status, L->top);
    L->ci->top = L->top;
  }
  else {
    lua_assert(L->nCcalls == L->baseCcalls);
    status = L->status;
  }
  // 减少调用层次
  --L->nCcalls;
  lua_unlock(L);
  return status;
}

/**
 * lua进行yield操作
 */
LUA_API int lua_yield (lua_State *L, int nresults) {
  luai_userstateyield(L, nresults);
  lua_lock(L);
  if (L->nCcalls > L->baseCcalls){
    luaG_runerror(L, "attempt to yield across metamethod/C-call boundary");
  }
  L->base = L->top - nresults;  /* protect stack slots below */
  L->status = LUA_YIELD;
  lua_unlock(L);
  return -1;
}

// 带错误保护的函数调用
/**
 * TODO:带错误保护的函数调用 
 */
int luaD_pcall (lua_State *L, Pfunc func, void *u,
                ptrdiff_t old_top, ptrdiff_t ef) {
  // 调用之前保存调用前的ci地址和top地址,用于可能发生的错误恢复
  int status;
  unsigned short oldnCcalls = L->nCcalls;
  //获取callinfo在栈上的位置
  ptrdiff_t old_ci = saveci(L, L->ci);
  lu_byte old_allowhooks = L->allowhook;
  ptrdiff_t old_errfunc = L->errfunc;
  //保存错误函数
  L->errfunc = ef;

  //此处包含的是不包含保护的直接调用
  status = luaD_rawrunprotected(L, func, u);

  // 如果status不为0,则表示有错误发生
  if (status != 0) {  /* an error occurred? */
	  // 将保存的ci和top取出来恢复
    StkId oldtop = restorestack(L, old_top);
    //清理栈
    luaF_close(L, oldtop);  /* close eventual pending closures */
    //将错误压栈
    luaD_seterrorobj(L, status, oldtop);
    L->nCcalls = oldnCcalls;
    L->ci = restoreci(L, old_ci);
    L->base = L->ci->base;
    L->savedpc = L->ci->savedpc;
    L->allowhook = old_allowhooks;
    restore_stack_limit(L);
  }
  L->errfunc = old_errfunc;
  return status;
}



/*
** Execute a protected parser.
*/
struct SParser {  /* data to `f_parser' */
  // 读入数据的缓冲区
  ZIO *z;
  // 缓存当前扫描数据的缓冲区
  Mbuffer buff;  /* buffer to be used by the scanner */
  // 源文件的文件名
  const char *name;
};

/**
 * 解析传入的buffer
 */
static void f_parser (lua_State *L, void *ud) {
  int i;
  Proto *tf;
  Closure *cl;
  struct SParser *p = cast(struct SParser *, ud);
  // 预读入第一个字符
  int c = luaZ_lookahead(p->z);
  //检查GC
  luaC_checkGC(L);
  // 根据之前预读的数据来决定下面的分析采用哪个函数
  // 解析二进制代码块或者是源代码，最终得到一个Proto
  tf = ((c == LUA_SIGNATURE[0]) ? luaU_undump : luaY_parser)(L, p->z,
                                                             &p->buff, p->name);
  //新建闭包
  cl = luaF_newLclosure(L, tf->nups, hvalue(gt(L)));
  cl->l.p = tf;
  for (i = 0; i < tf->nups; i++) { /* initialize eventual upvalues */
    cl->l.upvals[i] = luaF_newupval(L);
  } 
  //闭包入栈
  setclvalue(L, L->top, cl);
  incr_top(L);
}

/**
 * 使用进行保护的解析
 */
int luaD_protectedparser (lua_State *L, ZIO *z, const char *name) {
  //初始化SParser
  struct SParser p;
  int status;
  p.z = z; p.name = name;
  //初始化buffer
  luaZ_initbuffer(L, &p.buff);
  //调用parser对代码进行解析
  status = luaD_pcall(L, f_parser, &p, savestack(L, L->top), L->errfunc);
  //释放buffer
  luaZ_freebuffer(L, &p.buff);
  return status;
}


