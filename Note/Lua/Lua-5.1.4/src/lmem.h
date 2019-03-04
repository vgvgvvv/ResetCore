/*
** $Id: lmem.h,v 1.31.1.1 2007/12/27 13:02:25 roberto Exp $
** Interface to Memory Manager
** See Copyright Notice in lua.h
*/

#ifndef lmem_h
#define lmem_h


#include <stddef.h>

#include "llimits.h"
#include "lua.h"

#define MEMERRMSG	"not enough memory"

//TODO:函数内存申请部分

//之所以用宏来实现这组API ，是因为内存管理会被高频调用，
//而luaM_reallocv这样的API的传入参数中经常出现常数，实现为宏可以保证常量计算在编译时进行。

//申请内存
#define luaM_reallocv(L,b,on,n,e) \
	((cast(size_t, (n)+1) <= MAX_SIZET/(e)) ?  /* +1 to avoid warnings */ \
		luaM_realloc_(L, (b), (on)*(e), (n)*(e)) : \
		luaM_toobig(L))
//释放函数：newsize为0
#define luaM_freemem(L, b, s)	luaM_realloc_(L, (b), (s), 0)
//释放函数
#define luaM_free(L, b)		luaM_realloc_(L, (b), sizeof(*(b)), 0)
//释放数组
#define luaM_freearray(L, b, n, t)   luaM_reallocv(L, (b), n, 0, sizeof(t))

//申请大小为t的内存
#define luaM_malloc(L,t)	luaM_realloc_(L, NULL, 0, (t))
//申请类型为t的内存
#define luaM_new(L,t)		cast(t *, luaM_malloc(L, sizeof(t)))
//申请尺寸为n的类型t的数组
#define luaM_newvector(L,n,t) \
		cast(t *, luaM_reallocv(L, NULL, 0, n, sizeof(t)))
//更新尺寸
#define luaM_growvector(L,v,nelems,size,t,limit,e) \
          if ((nelems)+1 > (size)) \
            ((v)=cast(t *, luaM_growaux_(L,v,&(size),sizeof(t),limit,e)))
//重新为数组申请内存
#define luaM_reallocvector(L, v,oldn,n,t) \
   ((v)=cast(t *, luaM_reallocv(L, v, oldn, n, sizeof(t))))

//申请内存的基础函数
LUAI_FUNC void *luaM_realloc_ (lua_State *L, void *block, size_t oldsize,
                                                          size_t size);
LUAI_FUNC void *luaM_toobig (lua_State *L);
LUAI_FUNC void *luaM_growaux_ (lua_State *L, void *block, int *size,
                               size_t size_elem, int limit,
                               const char *errormsg);

#endif

