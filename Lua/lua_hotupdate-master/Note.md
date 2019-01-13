# Lua热重载

> [github地址](https://github.com/asqbtcupid/lua_hotupdate)
> [原文地址](http://asqbtcupid.github.io/hotupdte-implement/)

```lua
--初始化函数
function HU.Init(UpdateListFile, RootPath, FailNotify, ENV)
end
--更新函数
function HU.Update()
end
```

## 初始化更新参数
首先，在Init中我们需要收集所有我们需要更新的文件的Lua路径以及绝对路径。
其中Lua路径就是我们require中需要填的路径
在该github中是会映射到HUMap中的，我们在之后的使用中也会从中遍历

其中会调用几个函数：

```lua
--用于映射luapath以及systempath
InitFileMap(RootPath)
--初始化沙盒环境
InitFakeTable()
--保证一些代码不会被重载
InitProtection()
```

映射Lua路径与绝对路径我们之前已经提到了

其中沙盒环境我们在后面会提到，是暂时用于存放新代码的函数以及数据的，
防止在热重载的时候出现错误，我们可以及时中断重载

最后是保护一些数据不被重写

```lua
function HU.InitProtection()
    HU.Protection = {}
    --设置元表
    HU.Protection[setmetatable] = true
    --遍历函数
    HU.Protection[pairs] = true
    HU.Protection[ipairs] = true
    HU.Protection[next] = true
    --require函数
    HU.Protection[require] = true
    --自身
    HU.Protection[HU] = true
    --自身的Meta
    HU.Protection[HU.Meta] = true
    --其他内置关键字
    HU.Protection[math] = true
    HU.Protection[string] = true
    HU.Protection[table] = true
end
```

## 更新代码

首先从package.loaded中获取目前正在使用的模块。

```lua
local OldObject = package.loaded[LuaPath]
```

如果原来存在该代码则进行替换流程，否则只是将代码记入到 *OldCode* 这个Table中

### 获取新代码并且存入沙盒
然后从新的代码文件中读取代码并且写入到沙盒环境
函数为 *BuildNewCode* 

```lua
function HU.BuildNewCode(SysPath, LuaPath)
    io.input(SysPath)
    --读取数据
    local NewCode = io.read("*all")
    --如果原来不存在这个文件，那就把这个代码放入表中
    if HU.ALL and HU.OldCode[SysPath] == nil then
        HU.OldCode[SysPath] = NewCode
        return
    end
    --如果旧代码和新代码一致，那就返回
    if HU.OldCode[SysPath] == NewCode then
        io.input():close()
        return false
    end
    io.input(SysPath)
    local chunkName = LuaPath
    local chunk = "--[[" .. LuaPath .. "]] "
    chunk = chunk .. NewCode
    io.input():close()
    --读取文件
    local NewFunction = loadstring(chunk, chunkName)
    --检查语法
    if not NewFunction then
        HU.FailNotify(SysPath .. " has syntax error.")
        collectgarbage("collect")
        return false
    else
        HU.FakeENV = HU.FakeT()
        HU.MetaMap = {}
        HU.RequireMap = {}
        --设置沙盒进行执行，并且通过沙盒获取相关数据
        setfenv(NewFunction, HU.FakeENV)
        local NewObject
        HU.ErrorHappen = false
        --调用新的代码，如果有错误发生则进行处理
        xpcall(function()
            NewObject = NewFunction()
        end, HU.ErrorHandle)
        --如果没有错误则存下来
        if not HU.ErrorHappen then
            HU.OldCode[SysPath] = NewCode
            return true, NewObject
        else
            collectgarbage("collect")
            return false
        end
    end
end
```

至此，已经将需要重载的模块写入沙盒目录了。

### 替换旧代码
替换旧代码主要是由 *ReplaceOld* 这个函数操作的
替换旧代码实际上主要从两个点出发

而且对于新的代码我们会自动重载其require的代码
```lua
for LuaPath, NewObject in pairs(HU.RequireMap) do
    local OldObject = package.loaded[LuaPath]
    HU.ReplaceOld(OldObject, NewObject, LuaPath, "Main_require", "")
end
```

好在lua这种function是first class的存在，我们其实只需要考虑table以及function的情况

* 对于table，遇到值就拷贝值，遇到function和table就再调用ReplaceOld
* 对于function，主要通过getUpvalue以及setUpvalue来保证函数状态本身的正确性

```lua
function HU.ReplaceOld(OldObject, NewObject, LuaPath, From, Deepth)
    --首先旧对象与新对象必须是同一种东西
    if type(OldObject) == type(NewObject) then
        --处理table的情况
        if type(NewObject) == "table" then
            HU.UpdateAllFunction(OldObject, NewObject, LuaPath, From, "")
        --处理函数的情况
        elseif type(NewObject) == "function" then
            HU.UpdateOneFunction(OldObject, NewObject, LuaPath, nil, From, "")
        end
    end
end
```
下面是处理Table的情况

```lua
function HU.UpdateAllFunction(OldTable, NewTable, Name, From, Deepth)
    --保护判断
    if HU.Protection[OldTable] or HU.Protection[NewTable] then
        return
    end
    --判同
    if OldTable == NewTable then
        return
    end
    --进行签名，防止循环拷贝
    local signature = HU.GetTableSignature(OldTable) .. HU.GetTableSignature(NewTable)
    if HU.VisitedSig[signature] then
        return
    end
    HU.VisitedSig[signature] = true
    --log
    HU.DebugNofity(Deepth .. "HU.UpdateAllFunction " .. Name .. "  from:" .. From)
    log.color("HU.UpdateAllFunction ", ToString(NewTable), debug.traceback())
    
    --实际开始拷贝
    for ElementName, Element in pairs(NewTable) do
        local OldElement = OldTable[ElementName]
        log.color("ElementName ", ElementName)

        --遇到table继续递归拷贝
        if type(Element) == type(OldElement) then
            if type(Element) == "function" then
                HU.UpdateOneFunction(OldElement, Element, ElementName, OldTable, "HU.UpdateAllFunction", Deepth .. "    ")
            elseif type(Element) == "table" then
                HU.UpdateAllFunction(OldElement, Element, ElementName, "HU.UpdateAllFunction", Deepth .. "    ")
            end
        --遇到function拷贝function
        elseif OldElement == nil and type(Element) == "function" then
            if pcall(setfenv, Element, HU.ENV) then
                OldTable[ElementName] = Element
            end
        end
    end
    --设置metatable
    local OldMeta = debug.getmetatable(OldTable)
    local NewMeta = HU.MetaMap[NewTable]
    --metatable也是Table，所以同样适用table的update策略
    if type(OldMeta) == "table" and type(NewMeta) == "table" then
        HU.UpdateAllFunction(OldMeta, NewMeta, Name .. "'s Meta", "HU.UpdateAllFunction", Deepth .. "    ")
    end
end
```

下面是处理function的情况

```lua
function HU.UpdateOneFunction(OldObject, NewObject, FuncName, OldTable, From, Deepth)
    --判保护
    if HU.Protection[OldObject] or HU.Protection[NewObject] then
        return
    end
    --判同
    if OldObject == NewObject then
        return
    end
    --签名防循环
    local signature = tostring(OldObject) .. tostring(NewObject)
    if HU.VisitedSig[signature] then
        return
    end
    HU.VisitedSig[signature] = true
    --log
    HU.DebugNofity(Deepth .. "HU.UpdateOneFunction " .. FuncName .. "  from:" .. From)
    --为Function拷贝Upvalue，需要将环境设为实际环境
    if pcall(debug.setfenv, NewObject, getfenv(OldObject)) then
        HU.UpdateUpvalue(OldObject, NewObject, FuncName, "HU.UpdateOneFunction", Deepth .. "    ")
        --改变了的函数
        HU.ChangedFuncList[#HU.ChangedFuncList + 1] = { OldObject, NewObject, FuncName, OldTable }
    end
end
```

拷贝Upvalue的情形

```lua
function HU.UpdateUpvalue(OldFunction, NewFunction, Name, From, Deepth)
    --log
    HU.DebugNofity(Deepth .. "HU.UpdateUpvalue", Name, "  from:" .. From)
    local OldUpvalueMap = {}
    local OldExistName = {}
    
    --尝试从旧函数获取Upvalue并且存下来
    for i = 1, math.huge do
        local name, value = debug.getupvalue(OldFunction, i)
        if not name then
            break
        end
        OldUpvalueMap[name] = value
        OldExistName[name] = true
    end

    --对于值而言也分三种情况，除了普通值以外还需要处理函数以及Table
    for i = 1, math.huge do
        local name, value = debug.getupvalue(NewFunction, i)
        if not name then
            break
        end
        if OldExistName[name] then
            local OldValue = OldUpvalueMap[name]
            --值如果不同那就直接设置就行了
            if type(OldValue) ~= type(value) then
                debug.setupvalue(NewFunction, i, OldValue)
            --处理函数的情况
            elseif type(OldValue) == "function" then
                HU.UpdateOneFunction(OldValue, value, name, nil, "HU.UpdateUpvalue", Deepth .. "    ")
            --处理table的情况
            elseif type(OldValue) == "table" then
                HU.UpdateAllFunction(OldValue, value, name, "HU.UpdateUpvalue", Deepth .. "    ")
                debug.setupvalue(NewFunction, i, OldValue)
            --处理普通值的情况
            else
                debug.setupvalue(NewFunction, i, OldValue)
            end
        else
            HU.ResetENV(value, name, "HU.UpdateUpvalue", Deepth .. "    ")
        end
    end
end
```

看完了所有的处理情形之后我们不难发现，如果我们的table或者函数改了名字，
其实里面的东西是没办法在热更的时候实际拷贝过去的，因为无法建立一一对应的关系。
所以我们在实际热重载的时候也需要注意，尽量减少改名的行为。防止丢失旧有数据。


至此ReplaceOld的职责也完全完成了。
但是所有东西其实还是存在沙盒目录的,所以我们需要遍历沙盒目录。

### 将数据从沙盒拷贝到实际环境


在拷贝沙盒目录之前，我们需要将沙盒目录的元表置空，因为沙盒目录为了一些数据，
例如：require的文件，在元表中编写了一些方法，为了不触发这些方法，我们将其置空

```lua
 setmetatable(HU.FakeENV, nil)
```

然后我们对环境中的函数以及表进行拷贝：
```lua
HU.UpdateAllFunction(HU.ENV, HU.FakeENV, " ENV ", "Main", "")
```
其方式与拷贝普通table别无二致，就不再赘述。

然后调用了一个遍历环境的函数，
其实这个拷贝和之前拷贝table也很像，只不过会将我们在环境中修改过的值再重新赋值到_G中。

```lua
function HU.Travel_G()
    local visited = {}
    visited[HU] = true
    local function f(t)
        --只遍历表以及table
        if (type(t) ~= "function" and type(t) ~= "table") or visited[t] or HU.Protection[t] then
            return
        end
        visited[t] = true
        --函数的情况
        if type(t) == "function" then
            for i = 1, math.huge do
                --获取当前函数中的Upvalue
                local name, value = debug.getupvalue(t, i)
                if not name then
                    break
                end
                --如果是函数的话
                if type(value) == "function" then
                    --看看是不是环境表里面有用到这些被修改过的函数，如果有的话就替换成新的
                    for _, funcs in ipairs(HU.ChangedFuncList) do
                        if value == funcs[1] then
                            debug.setupvalue(t, i, funcs[2])
                        end
                    end
                end
                --更新这些upvalue
                f(value)
            end
        --table的情况
        elseif type(t) == "table" then
            --拷贝元表
            f(debug.getmetatable(t))
            local changeIndexs = {}
            --更新表中的所有东西
            for k, v in pairs(t) do
                f(k); f(v);
                --针对key以及value都要更新
                --以及替换被使用的函数
                if type(v) == "function" then
                    for _, funcs in ipairs(HU.ChangedFuncList) do
                        if v == funcs[1] then
                            t[k] = funcs[2]
                        end
                    end
                end
                if type(k) == "function" then
                    for index, funcs in ipairs(HU.ChangedFuncList) do
                        if k == funcs[1] then
                            changeIndexs[#changeIndexs + 1] = index
                        end
                    end
                end
            end
            --防止重复更新
            for _, index in ipairs(changeIndexs) do
                local funcs = HU.ChangedFuncList[index]
                t[funcs[2]] = t[funcs[1]]
                t[funcs[1]] = nil
            end
        end
    end

    --遍历全局表
    f(_G)
    --遍历注册表
    local registryTable = debug.getregistry()
    f(registryTable)

    for _, funcs in ipairs(HU.ChangedFuncList) do
        if funcs[3] == "HUDebug" then
            funcs[4]:HUDebug()
        end
    end
end
```

至此，更新已经完成了，调用一下垃圾回收，收拾现场即可。


## 关于沙盒目录

之前有提到沙盒目录里面有一些获取数据的方法，这里大概说明一下

```lua
function HU.InitFakeTable()
    local meta = {}
    HU.Meta = meta
    local function FakeT()
        return setmetatable({}, meta)
    end
    local function EmptyFunc()
    end
    local function pairs()
        return EmptyFunc
    end
    --暂时将metatable存下
    local function setmetatable(t, metaT)
        HU.MetaMap[t] = metaT
        return t
    end
    local function getmetatable(t, metaT)
        return setmetatable({}, t)
    end
    --将require的文件全部加到表中，
    --这样我们就知道这个文件导致引用了其他什么文件了
    local function require(LuaPath)
        if not HU.RequireMap[LuaPath] then
            local FakeTable = FakeT()
            HU.RequireMap[LuaPath] = FakeTable
        end
        return HU.RequireMap[LuaPath]
    end
    function meta.__index(t, k)
        if k == "setmetatable" then
            return setmetatable
        elseif k == "pairs" or k == "ipairs" then
            return pairs
        elseif k == "next" then
            return EmptyFunc
        elseif k == "require" then
            return require
        else
            --永远不会调用到元表
            local FakeTable = FakeT()
            rawset(t, k, FakeTable)
            return FakeTable
        end
    end
    function meta.__newindex(t, k, v)
        --永远不会调用到元表
        rawset(t, k, v)
    end

    --下面都是对环境操作的屏蔽
    function meta.__call()
        return FakeT(), FakeT(), FakeT()
    end
    function meta.__add()
        return meta.__call()
    end
    function meta.__sub()
        return meta.__call()
    end
    function meta.__mul()
        return meta.__call()
    end
    function meta.__div()
        return meta.__call()
    end
    function meta.__mod()
        return meta.__call()
    end
    function meta.__pow()
        return meta.__call()
    end
    function meta.__unm()
        return meta.__call()
    end
    function meta.__concat()
        return meta.__call()
    end
    function meta.__eq()
        return meta.__call()
    end
    function meta.__lt()
        return meta.__call()
    end
    function meta.__le()
        return meta.__call()
    end
    function meta.__len()
        return meta.__call()
    end
    return FakeT
end
```

至此，大部分的代码原理以及解释完毕，还不是很清楚有没有没有解释清楚的地方。