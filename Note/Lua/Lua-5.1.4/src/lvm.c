/*
** $Id: lvm.c,v 2.63.1.3 2007/12/28 15:32:23 roberto Exp $
** Lua virtual machine
** See Copyright Notice in lua.h
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define lvm_c
#define LUA_CORE

#include "lua.h"

#include "ldebug.h"
#include "ldo.h"
#include "lfunc.h"
#include "lgc.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"
#include "ltm.h"
#include "lvm.h"



/* limit for table tag-method chains (to avoid loops) */
#define MAXTAGLOOP	100

// value转换成数字
const TValue *luaV_tonumber (const TValue *obj, TValue *n) {
  lua_Number num;
  //是number就直接返回
  if (ttisnumber(obj)) return obj;
  //string转number
  if (ttisstring(obj) && luaO_str2d(svalue(obj), &num)) {
    setnvalue(n, num);
    return n;
  }
  else
    return NULL;
}

// 数字转换成字符串
int luaV_tostring (lua_State *L, StkId obj) {
  //不是number的时候就转换失败
  if (!ttisnumber(obj))
    return 0;
  else {
    //number转字符串
    char s[LUAI_MAXNUMBER2STR];
    lua_Number n = nvalue(obj);
    lua_number2str(s, n);
    setsvalue2s(L, obj, luaS_new(L, s));
    return 1;
  }
}

// ????????????
static void traceexec (lua_State *L, const Instruction *pc) {
  lu_byte mask = L->hookmask;
  const Instruction *oldpc = L->savedpc;
  L->savedpc = pc;
  if ((mask & LUA_MASKCOUNT) && L->hookcount == 0) {
    resethookcount(L);
    luaD_callhook(L, LUA_HOOKCOUNT, -1);
  }
  if (mask & LUA_MASKLINE) {
    Proto *p = ci_func(L->ci)->l.p;
    int npc = pcRel(pc, p);
    int newline = getline(p, npc);
    /* call linehook when enter a new function, when jump back (loop),
       or when enter a new line */
    if (npc == 0 || pc <= oldpc || newline != getline(p, pcRel(oldpc, p)))
      luaD_callhook(L, LUA_HOOKLINE, newline);
  }
}

// 调用某函数,最后将结果放在res中
static void callTMres (lua_State *L, StkId res, const TValue *f,
                        const TValue *p1, const TValue *p2) {
  ptrdiff_t result = savestack(L, res);
  setobj2s(L, L->top, f);  /* push function */
  setobj2s(L, L->top+1, p1);  /* 1st argument */
  setobj2s(L, L->top+2, p2);  /* 2nd argument */
  //保证用充足的位置
  luaD_checkstack(L, 3);
  //top+3
  L->top += 3;
  luaD_call(L, L->top - 3, 1);
  res = restorestack(L, result);
  L->top--;
  setobjs2s(L, res, L->top);
}


// 调用某函数,但是没有结果,这与callTMres不同
static void callTM (lua_State *L, const TValue *f, const TValue *p1,
                    const TValue *p2, const TValue *p3) {
  setobj2s(L, L->top, f);  /* push function */
  setobj2s(L, L->top+1, p1);  /* 1st argument */
  setobj2s(L, L->top+2, p2);  /* 2nd argument */
  setobj2s(L, L->top+3, p3);  /* 3th argument */
  luaD_checkstack(L, 4);
  L->top += 4;
  luaD_call(L, L->top - 4, 0);
}

/**
 * 在一个table中查找key对应的值, 找到存放到val中
 * 会进行递归查找
 * 同时会调用元表
 */
void luaV_gettable (lua_State *L, const TValue *t, TValue *key, StkId val) {
  int loop;
  // 函数外层以MAXTAGLOOP做为计数,防止死循环
  for (loop = 0; loop < MAXTAGLOOP; loop++) {
    const TValue *tm;
    if (ttistable(t)) {  /* `t' is a table? */
      Table *h = hvalue(t);
      // 首先尝试在表中查找这个key
      const TValue *res = luaH_get(h, key); /* do a primitive get */      
      if (!ttisnil(res) ||  /* result is no nil? */ // 如果结果不是nil
          (tm = fasttm(L, h->metatable, TM_INDEX)) == NULL) { /* or no TM? */ // 在结果为nil的时候如果metable为nil
        setobj2s(L, val, res);
        return;
      }
      /* else will try the tag method */
    }
    // 来查这个表的meta表, 如果不存在则报错
    else if (ttisnil(tm = luaT_gettmbyobj(L, t, TM_INDEX)))
      luaG_typeerror(L, t, "index");
    // 如果是一个函数则直接调用这个函数
    if (ttisfunction(tm)) {
      callTMres(L, val, tm, t, key);
      return;
    }
    // 否则继续找
    t = tm;  /* else repeat with `tm' */
  }
  luaG_runerror(L, "loop in gettable");
}

// 为什么这个操作需要放在lvm也就是虚拟机相关代码的部分呢??
/**
 * 设置table值,同时会调用元表
 */
void luaV_settable (lua_State *L, const TValue *t, TValue *key, StkId val) {
  int loop;
  for (loop = 0; loop < MAXTAGLOOP; loop++) {
    const TValue *tm;
    if (ttistable(t)) {  /* `t' is a table? */
      // 如果t是一个表, 首先获取它的hash部分
      Table *h = hvalue(t);
      //
      TValue *oldval = luaH_set(L, h, key); /* do a primitive set */
      if (!ttisnil(oldval) ||  /* result is no nil? 如果原来没有值就调用__newindex元方法 */
          (tm = fasttm(L, h->metatable, TM_NEWINDEX)) == NULL) { /* or no TM? */
    	  // 替换原来的旧值
        setobj2t(L, oldval, val);
        luaC_barriert(L, h, val);
        return;
      }
      /* else will try the tag method */
    }
    else if (ttisnil(tm = luaT_gettmbyobj(L, t, TM_NEWINDEX)))
      luaG_typeerror(L, t, "index");
    if (ttisfunction(tm)) {
      callTM(L, tm, t, key, val);
      return;
    }
    t = tm;  /* else repeat with `tm' */ 
  }
  luaG_runerror(L, "loop in settable");
}


static int call_binTM (lua_State *L, const TValue *p1, const TValue *p2,
                       StkId res, TMS event) {
  const TValue *tm = luaT_gettmbyobj(L, p1, event);  /* try first operand */
  if (ttisnil(tm))
    tm = luaT_gettmbyobj(L, p2, event);  /* try second operand */
  if (ttisnil(tm)) return 0;
  callTMres(L, res, tm, p1, p2);
  return 1;
}

/**
 * 获取表中元方法，如果相同则返回 否则返回空
 */
static const TValue *get_compTM (lua_State *L, Table *mt1, Table *mt2,
                                  TMS event) {
  const TValue *tm1 = fasttm(L, mt1, event);
  const TValue *tm2;
  if (tm1 == NULL) return NULL;  /* no metamethod */
  if (mt1 == mt2) return tm1;  /* same metatables => same metamethods */
  tm2 = fasttm(L, mt2, event);
  if (tm2 == NULL) return NULL;  /* no metamethod */
  if (luaO_rawequalObj(tm1, tm2))  /* same metamethods? */
    return tm1;
  return NULL;
}

//调用元方法比较
static int call_orderTM (lua_State *L, const TValue *p1, const TValue *p2,
                         TMS event) {
  //获取元方法
  const TValue *tm1 = luaT_gettmbyobj(L, p1, event);
  const TValue *tm2;
  //找不到返回-1
  if (ttisnil(tm1)) return -1;  /* no metamethod? */
  tm2 = luaT_gettmbyobj(L, p2, event);
  //比较元方法是否相同，不相同就返回-1
  if (!luaO_rawequalObj(tm1, tm2))  /* different metamethods? */
    return -1;
  //调用元方法进行比较
  callTMres(L, L->top, tm1, p1, p2);
  return !l_isfalse(L->top);
}

/**
 * 比较两个TString
 */
static int l_strcmp (const TString *ls, const TString *rs) {
  //获取各自的长度
  const char *l = getstr(ls);
  size_t ll = ls->tsv.len;
  const char *r = getstr(rs);
  size_t lr = rs->tsv.len;

  for (;;) {
    //使用strcoll比较字符串
    int temp = strcoll(l, r);
    if (temp != 0) return temp;
    else {  /* strings are equal up to a `\0' */
      //查看谁先结束
      size_t len = strlen(l);  /* index of first `\0' in both strings */
      if (len == lr)  /* r is finished? */
        return (len == ll) ? 0 : 1;
      else if (len == ll)  /* l is finished? */
        return -1;  /* l is smaller than r (because r is not finished) */
      /* both strings longer than `len'; go on comparing (after the `\0') */
      //如果都没结束，则从最后再来一遍比较流程
      len++;
      l += len; ll -= len; r += len; lr -= len;
    }
  }
}

//判断两个值的大小
int luaV_lessthan (lua_State *L, const TValue *l, const TValue *r) {
  int res;
  //不同类型不允许比较 直接报错
  if (ttype(l) != ttype(r))
    return luaG_ordererror(L, l, r);
  //比较数字
  else if (ttisnumber(l))
    return luai_numlt(nvalue(l), nvalue(r));
  //比较字符串
  else if (ttisstring(l)){
    //使用自定义的方法比较TString
    return l_strcmp(rawtsvalue(l), rawtsvalue(r)) < 0;
  }
  //调用元方法比较
  else if ((res = call_orderTM(L, l, r, TM_LT)) != -1){
    return res;
  }
  //比较错误
  return luaG_ordererror(L, l, r);
}


static int lessequal (lua_State *L, const TValue *l, const TValue *r) {
  int res;
  if (ttype(l) != ttype(r))
    return luaG_ordererror(L, l, r);
  else if (ttisnumber(l))
    return luai_numle(nvalue(l), nvalue(r));
  else if (ttisstring(l))
    return l_strcmp(rawtsvalue(l), rawtsvalue(r)) <= 0;
  else if ((res = call_orderTM(L, l, r, TM_LE)) != -1)  /* first try `le' */
    return res;
  else if ((res = call_orderTM(L, r, l, TM_LT)) != -1)  /* else try `lt' */
    return !res;
  return luaG_ordererror(L, l, r);
}

/**
 * 判断两个值是否相等
 */
int luaV_equalval (lua_State *L, const TValue *t1, const TValue *t2) {
  const TValue *tm;
  lua_assert(ttype(t1) == ttype(t2));
  switch (ttype(t1)) {
    //两个nil一定相等
    case LUA_TNIL: return 1;
    //数字比较
    case LUA_TNUMBER: return luai_numeq(nvalue(t1), nvalue(t2));
    //比较bool
    case LUA_TBOOLEAN: return bvalue(t1) == bvalue(t2);  /* true must be 1 !! */
    //比较指针
    case LUA_TLIGHTUSERDATA: return pvalue(t1) == pvalue(t2);
    //比较userdata
    case LUA_TUSERDATA: {
      //比较指针，如果一样就是相等的
      if (uvalue(t1) == uvalue(t2)) return 1;
      //尝试获取Equal元方法
      tm = get_compTM(L, uvalue(t1)->metatable, uvalue(t2)->metatable,
                         TM_EQ);
      break;  /* will try TM */
    }
    case LUA_TTABLE: {
      //比较指针，如果一样就是相等的
      if (hvalue(t1) == hvalue(t2)) return 1;
      tm = get_compTM(L, hvalue(t1)->metatable, hvalue(t2)->metatable, TM_EQ);
      break;  /* will try TM */
    }
    //直接比较GCObject的指针
    default: return gcvalue(t1) == gcvalue(t2);
  }
  //如果不存在或者没有相同的Equal元方法，则一定不相等
  if (tm == NULL) return 0;  /* no TM? */
  //调用相等的函数
  callTMres(L, L->top, tm, t1, t2);  /* call TM */
  //获得最终结果
  return !l_isfalse(L->top);
}

//TODO:连接栈上的字符串
void luaV_concat (lua_State *L, int total, int last) {
  do {
    StkId top = L->base + last + 1;
    int n = 2;  /* number of elements handled in this pass (at least 2) */
    if (!(ttisstring(top-2) || ttisnumber(top-2)) || !tostring(L, top-1)) {
      if (!call_binTM(L, top-2, top-1, top-2, TM_CONCAT))
        luaG_concaterror(L, top-2, top-1);
    } else if (tsvalue(top-1)->len == 0) { /* second op is empty? */
      (void)tostring(L, top - 2);  /* result is first op (as string) */
    } 
    else {
      /* at least two string values; get as many as possible */
      size_t tl = tsvalue(top-1)->len;
      char *buffer;
      int i;
      /* collect total length */
      for (n = 1; n < total && tostring(L, top-n-1); n++) {
        size_t l = tsvalue(top-n-1)->len;
        if (l >= MAX_SIZET - tl) luaG_runerror(L, "string length overflow");
        tl += l;
      }
      buffer = luaZ_openspace(L, &G(L)->buff, tl);
      tl = 0;
      for (i=n; i>0; i--) {  /* concat all strings */
        size_t l = tsvalue(top-i)->len;
        memcpy(buffer+tl, svalue(top-i), l);
        tl += l;
      }
      setsvalue2s(L, top-n, luaS_newlstr(L, buffer, tl));
    }
    total -= n-1;  /* got `n' strings to create 1 new */
    last -= n-1;
  } while (total > 1);  /* repeat until only 1 result left */
}


static void Arith (lua_State *L, StkId ra, const TValue *rb,
                   const TValue *rc, TMS op) {
  TValue tempb, tempc;
  const TValue *b, *c;
  if ((b = luaV_tonumber(rb, &tempb)) != NULL &&
      (c = luaV_tonumber(rc, &tempc)) != NULL) {
    lua_Number nb = nvalue(b), nc = nvalue(c);
    switch (op) {
      case TM_ADD: setnvalue(ra, luai_numadd(nb, nc)); break;
      case TM_SUB: setnvalue(ra, luai_numsub(nb, nc)); break;
      case TM_MUL: setnvalue(ra, luai_nummul(nb, nc)); break;
      case TM_DIV: setnvalue(ra, luai_numdiv(nb, nc)); break;
      case TM_MOD: setnvalue(ra, luai_nummod(nb, nc)); break;
      case TM_POW: setnvalue(ra, luai_numpow(nb, nc)); break;
      case TM_UNM: setnvalue(ra, luai_numunm(nb)); break;
      default: lua_assert(0); break;
    }
  }
  else if (!call_binTM(L, rb, rc, ra, op))
    luaG_aritherror(L, rb, rc);
}



/*
** some macros for common tasks in `luaV_execute'
*/

#define runtime_check(L, c)	{ if (!(c)) break; }

//获取寄存器A在栈中的位置
#define RA(i)	(base+GETARG_A(i))
/* to be used after possible stack reallocation */
//获取寄存器B在栈中的位置
#define RB(i)	check_exp(getBMode(GET_OPCODE(i)) == OpArgR, base+GETARG_B(i))
//获取寄存器C在栈中的位置
#define RC(i)	check_exp(getCMode(GET_OPCODE(i)) == OpArgR, base+GETARG_C(i))
//获取常量B，如果是常量则从Proto中的k链表获取，否则则取表中内容
#define RKB(i)	check_exp(getBMode(GET_OPCODE(i)) == OpArgK, \
	ISK(GETARG_B(i)) ? k+INDEXK(GETARG_B(i)) : base+GETARG_B(i))
//获取常量C，如果是常量则从Proto中的k链表获取，否则则取表中内容
#define RKC(i)	check_exp(getCMode(GET_OPCODE(i)) == OpArgK, \
	ISK(GETARG_C(i)) ? k+INDEXK(GETARG_C(i)) : base+GETARG_C(i))

//获取Bx寄存器 先判断是否为常数
//k为proto中的k，也就是常量列表
#define KBx(i)	check_exp(getBMode(GET_OPCODE(i)) == OpArgK, k+GETARG_Bx(i))

//跳转i个字节码
#define dojump(L,pc,i)	{(pc) += (i); luai_threadyield(L);}

//先存下当前指令位置，最后将状态机函数栈设为基础栈
#define Protect(x)	{ L->savedpc = pc; {x;}; base = L->base; }

//如果都是数字，则直接加，否则尝试调用元表进行相加
#define arith_op(op,tm) { \
        TValue *rb = RKB(i); \
        TValue *rc = RKC(i); \
        if (ttisnumber(rb) && ttisnumber(rc)) { \
          lua_Number nb = nvalue(rb), nc = nvalue(rc); \
          setnvalue(ra, op(nb, nc)); \
        } \
        else \
          Protect(Arith(L, ra, rb, rc, tm)); \
      }


/**
 * lua虚拟机循环，就是执行一个代码块时的过程
 * 里面包含了所有的opcode实现
 */
void luaV_execute (lua_State *L, int nexeccalls) {
  LClosure *cl;
  StkId base;
  TValue *k;
  //指令指针
  const Instruction *pc;
  //入口点
 reentry:  /* entry point */
  lua_assert(isLua(L->ci));
  pc = L->savedpc;
  //获取当前callinfo中的的lua闭包
  cl = &clvalue(L->ci->func)->l;
  //设置base为当前的函数栈底
  base = L->base;
  //获取需要的常量
  k = cl->p->k;
  /* main loop of interpreter */
  for (;;) {
    //当前指令
    const Instruction i = *pc++;
    StkId ra;
    if ((L->hookmask & (LUA_MASKLINE | LUA_MASKCOUNT)) &&
        (--L->hookcount == 0 || L->hookmask & LUA_MASKLINE)) {
      traceexec(L, pc);
      if (L->status == LUA_YIELD) {  /* did hook yield? */
        L->savedpc = pc - 1;
        return;
      }
      base = L->base;
    }
    /* warning!! several calls may realloc the stack and invalidate `ra' */
    ra = RA(i);
    lua_assert(base == L->base && L->base == L->ci->base);
    lua_assert(base <= L->top && L->top <= L->stack + L->stacksize);
    lua_assert(L->top == L->ci->top || luaG_checkopenop(i));
    switch (GET_OPCODE(i)) {
      case OP_MOVE: {
        //获取RB放入RA
        setobjs2s(L, ra, RB(i));
        continue;
      }
      case OP_LOADK: {
        //将常量放入RA
        setobj2s(L, ra, KBx(i));
        continue;
      }
      case OP_LOADBOOL: {
        //将bool放入RA
        setbvalue(ra, GETARG_B(i));
        if (GETARG_C(i)) pc++;  /* skip next instruction (if C) */
        continue;
      }
      case OP_LOADNIL: {
        //将rb到ra直接的所有值置空
        TValue *rb = RB(i);
        do {
          setnilvalue(rb--);
        } while (rb >= ra);
        continue;
      }
      case OP_GETUPVAL: {
        //rb为upvalue的index
        int b = GETARG_B(i);
        //设置给ra
        setobj2s(L, ra, cl->upvals[b]->v);
        continue;
      }
      case OP_GETGLOBAL: {
        TValue g;
        TValue *rb = KBx(i);
        sethvalue(L, &g, cl->env);
        lua_assert(ttisstring(rb));
        //保护模式
        Protect(luaV_gettable(L, &g, rb, ra));
        continue;
      }
      case OP_GETTABLE: {
        //RB为table RKC为常量key ra为结果
        Protect(luaV_gettable(L, RB(i), RKC(i), ra));
        continue;
      }
      case OP_SETGLOBAL: {
        TValue g;
        sethvalue(L, &g, cl->env);
        lua_assert(ttisstring(KBx(i)));
        //Bx为key，ra为value，g为函数table
        Protect(luaV_settable(L, &g, KBx(i), ra));
        continue;
      }
      case OP_SETUPVAL: {
        //B为upvalue的index
        UpVal *uv = cl->upvals[GETARG_B(i)];
        //将ra设置到upvalue中
        setobj(L, uv->v, ra);
        //垃圾回收设置
        luaC_barrier(L, uv, ra);
        continue;
      }
      case OP_SETTABLE: {
        //ra为表 rkb为key rkc为值
        Protect(luaV_settable(L, ra, RKB(i), RKC(i)));
        continue;
      }
      case OP_NEWTABLE: {
        int b = GETARG_B(i);
        int c = GETARG_C(i);
        //初始化table，rb为数组尺寸，rc为hash尺寸，结果赋给ra
        sethvalue(L, ra, luaH_new(L, luaO_fb2int(b), luaO_fb2int(c)));
        Protect(luaC_checkGC(L));
        continue;
      }
      case OP_SELF: {
        StkId rb = RB(i);
        //ra为table，+1则是ra的下一个位置
        setobjs2s(L, ra+1, rb);
        //TODO:从表rb中获取元素c并且赋给ra，用法还是不是很清楚
        Protect(luaV_gettable(L, rb, RKC(i), ra));
        continue;
      }
      case OP_ADD: {
        //如果都是数字，则直接加，否则尝试调用元表进行相加,下面的数值运算同理
        arith_op(luai_numadd, TM_ADD);
        continue;
      }
      case OP_SUB: {
        arith_op(luai_numsub, TM_SUB);
        continue;
      }
      case OP_MUL: {
        arith_op(luai_nummul, TM_MUL);
        continue;
      }
      case OP_DIV: {
        arith_op(luai_numdiv, TM_DIV);
        continue;
      }
      case OP_MOD: {
        arith_op(luai_nummod, TM_MOD);
        continue;
      }
      case OP_POW: {
        arith_op(luai_numpow, TM_POW);
        continue;
      }
      case OP_UNM: {
        TValue *rb = RB(i);
        if (ttisnumber(rb)) {
          lua_Number nb = nvalue(rb);
          setnvalue(ra, luai_numunm(nb));
        }
        else {
          Protect(Arith(L, ra, rb, rb, TM_UNM));
        }
        continue;
      }
      case OP_NOT: {
        int res = l_isfalse(RB(i));  /* next assignment may change this value */
        setbvalue(ra, res);
        continue;
      }
      case OP_LEN: {
        //根据不同类型去长度
        const TValue *rb = RB(i);
        switch (ttype(rb)) {
          case LUA_TTABLE: {
            setnvalue(ra, cast_num(luaH_getn(hvalue(rb))));
            break;
          }
          case LUA_TSTRING: {
            setnvalue(ra, cast_num(tsvalue(rb)->len));
            break;
          }
          default: {  /* try metamethod */
            Protect(
              if (!call_binTM(L, rb, luaO_nilobject, ra, TM_LEN))
                luaG_typeerror(L, rb, "get length of");
            )
          }
        }
        continue;
      }
      case OP_CONCAT: {
        int b = GETARG_B(i);
        int c = GETARG_C(i);
        //连接字符串
        Protect(luaV_concat(L, c-b+1, c); luaC_checkGC(L));
        setobjs2s(L, RA(i), base+b);
        continue;
      }
      case OP_JMP: {
        //跳转sBx个字节码
        dojump(L, pc, GETARG_sBx(i));
        continue;
      }
      case OP_EQ: {
        //if ((RK(B) == RK(C)) ~= A) then PC++
        TValue *rb = RKB(i);
        TValue *rc = RKC(i);
        Protect(
          if (equalobj(L, rb, rc) == GETARG_A(i))
            dojump(L, pc, GETARG_sBx(*pc));
        )
        pc++;
        continue;
      }
      case OP_LT: {
        //if ((RK(B) < RK(C)) ~= A) then PC++
        Protect(
          if (luaV_lessthan(L, RKB(i), RKC(i)) == GETARG_A(i))
            dojump(L, pc, GETARG_sBx(*pc));
        )
        pc++;
        continue;
      }
      case OP_LE: {
        //if ((RK(B) <= RK(C)) ~= A) then PC++
        Protect(
          if (lessequal(L, RKB(i), RKC(i)) == GETARG_A(i))
            dojump(L, pc, GETARG_sBx(*pc));
        )
        pc++;
        continue;
      }
      case OP_TEST: {
        //if not (R(A) <=> C) then PC++
        if (l_isfalse(ra) != GETARG_C(i))
          dojump(L, pc, GETARG_sBx(*pc));
        pc++;
        continue;
      }
      case OP_TESTSET: {
        //if (R(B) <=> C) then R(A) := R(B) else PC++
        TValue *rb = RB(i);
        if (l_isfalse(rb) != GETARG_C(i)) {
          setobjs2s(L, ra, rb);
          dojump(L, pc, GETARG_sBx(*pc));
        }
        pc++;
        continue;
      }
      case OP_CALL: {
        //获取参数数量
        int b = GETARG_B(i);
        //获取结果数量
        int nresults = GETARG_C(i) - 1;
        // 如果传入参数数量不为0, 则这是top地址,由它开始后面紧跟着都是函数参数
        if (b != 0) L->top = ra+b;  /* else previous instruction set top */
        // 保存当前pc
        L->savedpc = pc;
        //调用在ra上的函数，precall
        switch (luaD_precall(L, ra, nresults)) {
          //如果调用的是lua函数
          case PCRLUA: {
            //进入vm循环再来一次
            nexeccalls++;
            goto reentry;  /* restart luaV_execute over new Lua function */
          }
          //如果调用的是C函数则适配返回值在栈中的位置
          case PCRC: {
            /* it was a C function (`precall' called it); adjust results */
            //如果有返回值就把top设为callinfo的顶部
            if (nresults >= 0) L->top = L->ci->top;
            base = L->base;
            continue;
          }
          default: {
            return;  /* yield */
          }
        }
      }
      case OP_TAILCALL: {
        //TODO:尾调用
        //获取返回值数量
        int b = GETARG_B(i);
        if (b != 0) L->top = ra+b;  /* else previous instruction set top */
        L->savedpc = pc;
        lua_assert(GETARG_C(i) - 1 == LUA_MULTRET);
        switch (luaD_precall(L, ra, LUA_MULTRET)) {
          case PCRLUA: {
            /* tail call: put new frame in place of previous one */
            CallInfo *ci = L->ci - 1;  /* previous frame */
            int aux;
            StkId func = ci->func;
            StkId pfunc = (ci+1)->func;  /* previous function index */
            if (L->openupval) {
              luaF_close(L, ci->base);
            }
            L->base = ci->base = ci->func + ((ci+1)->base - pfunc);
            for (aux = 0; pfunc+aux < L->top; aux++)  /* move frame down */
              setobjs2s(L, func+aux, pfunc+aux);
            ci->top = L->top = func+aux;  /* correct top */
            lua_assert(L->top == L->base + clvalue(func)->l.p->maxstacksize);
            ci->savedpc = L->savedpc;
            ci->tailcalls++;  /* one more call lost */
            L->ci--;  /* remove new frame */
            goto reentry;
          }
          case PCRC: {  /* it was a C function (`precall' called it) */
            //C函数的情况，precall中已经调用过了
            base = L->base;
            continue;
          }
          default: {
            return;  /* yield */
          }
        }
      }
      case OP_RETURN: {
        //rb存的是返回值数量
        int b = GETARG_B(i);
        //设置top位置
        if (b != 0) L->top = ra+b-1;
        if (L->openupval) luaF_close(L, base);
        L->savedpc = pc;
        //结束调用，将结果压入堆栈
        b = luaD_poscall(L, ra);
        //调用层级减一，如果已经是0则直接返回
        if (--nexeccalls == 0)  /* was previous function running `here'? */
          return;  /* no: return */
        else {  /* yes: continue its execution */
          //返回上一个调用，并且继续lua调用
          if (b) L->top = L->ci->top;
          lua_assert(isLua(L->ci));
          lua_assert(GET_OPCODE(*((L->ci)->savedpc - 1)) == OP_CALL);
          goto reentry;
        }
      }
      case OP_FORLOOP: {
        //获取step
        lua_Number step = nvalue(ra+2);
        //更新index
        lua_Number idx = luai_numadd(nvalue(ra), step); /* increment index */
        //获取index
        lua_Number limit = nvalue(ra+1);
        if (luai_numlt(0, step) ? luai_numle(idx, limit)
                                : luai_numle(limit, idx)) {
          //调回循环头，设置index
          dojump(L, pc, GETARG_sBx(i));  /* jump back */
          setnvalue(ra, idx);  /* update internal index... */
          setnvalue(ra+3, idx);  /* ...and external index */
        }
        continue;
      }
      case OP_FORPREP: {
        //TODO:初始化数字for循环
        const TValue *init = ra;
        const TValue *plimit = ra+1;
        const TValue *pstep = ra+2;
        //保存跳跃点
        L->savedpc = pc;  /* next steps may throw errors */

        //三个计数器必须是数字
        if (!tonumber(init, ra))
          luaG_runerror(L, LUA_QL("for") " initial value must be a number");
        else if (!tonumber(plimit, ra+1))
          luaG_runerror(L, LUA_QL("for") " limit must be a number");
        else if (!tonumber(pstep, ra+2))
          luaG_runerror(L, LUA_QL("for") " step must be a number");
        
        setnvalue(ra, luai_numsub(nvalue(ra), nvalue(pstep)));
        dojump(L, pc, GETARG_sBx(i));
        continue;
      }
      case OP_TFORLOOP: {
        //TODO:一般形式的for循环
        StkId cb = ra + 3;  /* call base */
        setobjs2s(L, cb+2, ra+2);
        setobjs2s(L, cb+1, ra+1);
        setobjs2s(L, cb, ra);
        L->top = cb+3;  /* func. + 2 args (state and index) */
        Protect(luaD_call(L, cb, GETARG_C(i)));
        L->top = L->ci->top;
        cb = RA(i) + 3;  /* previous call may change the stack */
        if (!ttisnil(cb)) {  /* continue loop? */
          setobjs2s(L, cb-1, cb);  /* save control variable */
          dojump(L, pc, GETARG_sBx(*pc));  /* jump back */
        }
        pc++;
        continue;
      }
      case OP_SETLIST: {
        //TODO:设置表的一系列数组元素
        int n = GETARG_B(i);
        int c = GETARG_C(i);
        int last;
        Table *h;
        if (n == 0) {
          n = cast_int(L->top - ra) - 1;
          L->top = L->ci->top;
        }
        if (c == 0) c = cast_int(*pc++);
        runtime_check(L, ttistable(ra));
        h = hvalue(ra);
        last = ((c-1)*LFIELDS_PER_FLUSH) + n;
        if (last > h->sizearray)  /* needs more space? */
          luaH_resizearray(L, h, last);  /* pre-alloc it at once */
        for (; n > 0; n--) {
          TValue *val = ra+n;
          setobj2t(L, luaH_setnum(L, h, last--), val);
          luaC_barriert(L, h, val);
        }
        continue;
      }
      case OP_CLOSE: {
        luaF_close(L, ra);
        continue;
      }
      case OP_CLOSURE: {
        //TODO:创建函数原型的闭包
        Proto *p;
        Closure *ncl;
        int nup, j;
        // Bx中存放的是在上层函数proto数组中的索引
        p = cl->p->p[GETARG_Bx(i)];
        nup = p->nups;
        ncl = luaF_newLclosure(L, nup, cl->env);
        ncl->l.p = p;
        // 紧跟着CLOSURE指令的是MOVE或者GETUPVAL指令
        // 如果是GETUPVAL, 则从上层函数的upval中寻找upval
        // 如果是MOVE, 则从
        // 特别注意这里pc指针每次循环都递增了
        for (j=0; j<nup; j++, pc++) {
          if (GET_OPCODE(*pc) == OP_GETUPVAL)
            ncl->l.upvals[j] = cl->upvals[GETARG_B(*pc)];
          else {
            lua_assert(GET_OPCODE(*pc) == OP_MOVE);
            ncl->l.upvals[j] = luaF_findupval(L, base + GETARG_B(*pc));
          }
        }
        // 将创建好的closure存放到ra中
        setclvalue(L, ra, ncl);
        Protect(luaC_checkGC(L));
        continue;
      }
      case OP_VARARG: {
        //TODO:可变参数
        int b = GETARG_B(i) - 1;
        int j;
        CallInfo *ci = L->ci;
        int n = cast_int(ci->base - ci->func) - cl->p->numparams - 1;
        if (b == LUA_MULTRET) {
          Protect(luaD_checkstack(L, n));
          ra = RA(i);  /* previous call may change the stack */
          b = n;
          L->top = ra + n;
        }
        for (j = 0; j < b; j++) {
          if (j < n) {
            setobjs2s(L, ra + j, ci->base - n + j);
          }
          else {
            setnilvalue(ra + j);
          }
        }
        continue;
      }
    }
  }
}

