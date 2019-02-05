# string扩展方法

在阅读源码的过程中，发现strlib给空字符串添加了元表，
如下：

```c
static void createmetatable (lua_State *L) {
  lua_createtable(L, 0, 1);         /* create metatable for strings */     // table
  lua_pushliteral(L, "");           /* dummy string */  // table ""
  lua_pushvalue(L, -2);             // table "" table
  lua_setmetatable(L, -2);          /* set string metatable */ // table "" 
  lua_pop(L, 1);                    /* pop dummy string */ // table
  lua_pushvalue(L, -2);             /* string library... */ // table lib
  lua_setfield(L, -2, "__index");   /* ...is the __index metamethod */ // table["__index"]=lib // table lib
  lua_pop(L, 1);                    /* pop metatable */ // table
}
```

我们实际上可以直接通过字符串去调用string库的函数
类似于：

```lua
local s = "asd"
print(s:len())

--输出为3
```

而我们自己也可以往其中添加方法：

```lua
function test(s)
    print(s)
end

local mt = getmetatable("")
mt["__index"]["print"] = test

local l = "asd"
print(l:print())

--输出为asd
```