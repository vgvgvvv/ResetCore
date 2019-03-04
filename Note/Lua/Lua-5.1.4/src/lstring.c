/*
** $Id: lstring.c,v 2.8.1.1 2007/12/27 13:02:25 roberto Exp $
** String table (keeps all strings handled by Lua)
** See Copyright Notice in lua.h
*/


#include <string.h>

#define lstring_c
#define LUA_CORE

#include "lua.h"

#include "lmem.h"
#include "lobject.h"
#include "lstate.h"
#include "lstring.h"


/**
 * 对保存string的hash桶进行resize
 */
void luaS_resize (lua_State *L, int newsize) {
  GCObject **newhash;
  stringtable *tb;
  int i;
  //如果正在垃圾回收字符串的时候则直接返回
  if (G(L)->gcstate == GCSsweepstring){
    return;  /* cannot resize during GC traverse */
  }
  //重新申请字符串数组
  newhash = luaM_newvector(L, newsize, GCObject *);
  //获取字符串表
  tb = &G(L)->strt;
  for (i=0; i<newsize; i++) {
    newhash[i] = NULL;
  }
  /* rehash */
  for (i=0; i<tb->size; i++) {
    //遍历hash表
    GCObject *p = tb->hash[i];
    //hash节点中的所有元素都进行遍历
    while (p) {  /* for each node in the list */
      GCObject *next = p->gch.next;  /* save next */
      unsigned int h = gco2ts(p)->hash;
      // 重新计算hash桶索引，这次需要mod新的hash桶大小
      int h1 = lmod(h, newsize);  /* new position */
      lua_assert(cast_int(h%newsize) == lmod(h, newsize));
      p->gch.next = newhash[h1];  /* chain it */
      newhash[h1] = p;
      p = next;
    }
  }
  //释放原来的hash
  luaM_freearray(L, tb->hash, tb->size, TString *);
  tb->size = newsize;
  //用新的hash表进行替换
  tb->hash = newhash;
}

/**
 * 申请新的字符串
 */
static TString *newlstr (lua_State *L, const char *str, size_t l,
                                       unsigned int h) {
  TString *ts;
  stringtable *tb;
  //防止字符串过大
  if (l+1 > (MAX_SIZET - sizeof(TString))/sizeof(char)){
    luaM_toobig(L);
  }
  ts = cast(TString *, luaM_malloc(L, (l+1)*sizeof(char)+sizeof(TString)));
  // 初始化tsv参数
  ts->tsv.len = l;
  ts->tsv.hash = h;
  ts->tsv.marked = luaC_white(G(L));
  ts->tsv.tt = LUA_TSTRING;
  ts->tsv.reserved = 0;
  //拷贝字符串
  memcpy(ts+1, str, l*sizeof(char));
  ((char *)(ts+1))[l] = '\0';  /* ending 0 */
  tb = &G(L)->strt;
  h = lmod(h, tb->size);
  ts->tsv.next = tb->hash[h];  /* chain new entry */
  //字符串池中添加字符串
  tb->hash[h] = obj2gco(ts);
  //字符串的大小
  tb->nuse++;
  // 在hash桶数组大小小于MAX_INT/2的情况下，
  // 只要字符串数量大于桶数组数量就开始成倍的扩充桶的容量
  if (tb->nuse > cast(lu_int32, tb->size) && tb->size <= MAX_INT/2){
    luaS_resize(L, tb->size*2);  /* too crowded */
  }
  return ts;
}

/**
 * 尝试获取字符串，如果之前没有则分配一个新的字符串
 */
TString *luaS_newlstr (lua_State *L, const char *str, size_t l) {
  GCObject *o;
  //
  unsigned int h = cast(unsigned int, l);  /* seed */
  // 只计算前面几位，不会计算全hash
  size_t step = (l>>5)+1;  /* if string is too long, don't hash all its chars */
  size_t l1;
  //计算hash
  for (l1=l; l1>=step; l1-=step) { /* compute hash */
    h = h ^ ((h<<5)+(h>>2)+cast(unsigned char, str[l1-1]));
  } 
  //在globaltable中的hashtable中进行寻找
  for (o = G(L)->strt.hash[lmod(h, G(L)->strt.size)];
       o != NULL;
       o = o->gch.next) {
    TString *ts = rawgco2ts(o);
    //比较字符串
    if (ts->tsv.len == l && (memcmp(str, getstr(ts), l) == 0)) {
      /* string may be dead */
      //如果是死的字符串则恢复white身份
      if (isdead(G(L), o)) changewhite(o);
      return ts;
    }
  }
  //如果未找到则申请新数组
  return newlstr(L, str, l, h);  /* not found */
}

/**
 * 申请新的userdata
 */
Udata *luaS_newudata (lua_State *L, size_t s, Table *e) {
  Udata *u;
  //块过大
  if (s > MAX_SIZET - sizeof(Udata)){
    luaM_toobig(L);
  }
  //申请udata
  u = cast(Udata *, luaM_malloc(L, s + sizeof(Udata)));
  u->uv.marked = luaC_white(G(L));  /* is not finalized */
  u->uv.tt = LUA_TUSERDATA;
  u->uv.len = s;
  u->uv.metatable = NULL;
  u->uv.env = e;
  /* chain it on udata list (after main thread) */
  // TODO:这样让udata链接在mainthread之后，一定是整个GC链表的最后
  u->uv.next = G(L)->mainthread->next;
  G(L)->mainthread->next = obj2gco(u);
  return u;
}

