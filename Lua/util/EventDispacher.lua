local Global = _G
local package = _G.package
local setmetatable = _G.setmetatable
local assert = _G.assert
local table = _G.table
local pairs = _G.pairs
local ipairs = _G.ipairs


module("EventDispatcher", package.seeall)

-- EventDispatcher = class("EventDispatcher")
--[[
数据层次
 
["EventName1"] =
{
    ["_StaticFunc"] = { Func1, Func2 },
    
    [Object1] = { Func1, Func2 },
    [Object2] = { Func1, Func2 },
},
 
["EventName2"] =
{
    ...
}
 
]]

-- 默认调用函数
local function PreInvoke( EventName, Func, Object, UserData, ... )

    if Object then
        Func( Object, ... )
    else
        Func( ... )
    end

end


function new()

    local NewObj = setmetatable( {}, { __index = package.loaded["EventDispatcher"] } )

    -- 对象成员初始化
    NewObj.mPreInvokeFunc = PreInvoke
    NewObj.mEventTable = {}

    return NewObj
end

-- 添加
function Add( Self, EventName, Func, Object, UserData )

    Self.mEventTable[ EventName ] = Self.mEventTable[ EventName ] or {}

    local Event = Self.mEventTable[ EventName ]

    if not Object then
        Object = "_StaticFunc"
    end

    Event[Object] = Event[Object] or {}
    local ObjectEvent = Event[Object]

    ObjectEvent[Func] = UserData or true

end

-- 设置调用前回调
function SetDispatchHook( Self, HookFunc )

    Self.mPreInvokeFunc = HookFunc
end


-- 派发
function Dispatch( Self, EventName, ... )


    local Event = Self.mEventTable[ EventName ]

    if not Exist(Self, EventName) then
        -- print("DispatchBy  ".. tostring(Self))
        return
    end

    for Object,ObjectFunc in pairs( Event ) do
        
        if Object == "_StaticFunc" then

            for Func, UserData in pairs( ObjectFunc ) do
                Self.mPreInvokeFunc( EventName, Func, nil, UserData, ... )
            end

        else

            for Func, UserData in pairs( ObjectFunc ) do
                Self.mPreInvokeFunc( EventName, Func, Object, UserData, ... )
            end

        end

    end

end

-- 回调是否存在
function Exist( Self, EventName )


    local Event = Self.mEventTable[ EventName ]

    if not Event then
        return false
    end

    -- 需要遍历下map, 可能有事件名存在, 但是没有任何回调的
    for Object,ObjectFunc in pairs( Event ) do

        for Func, _ in pairs( ObjectFunc ) do
            -- 居然有一个
            return true
        end

    end


    return false

end

-- 清除
function Remove( Self, EventName, Func, Object )


    local Event = Self.mEventTable[ EventName ]

    if not Event then
        return
    end

    if not Object then
        Object = "_StaticFunc"
    end


    local ObjectEvent = Event[Object]

    if not ObjectEvent then
        return
    end

    ObjectEvent[Func] = nil


end

-- 清除对象的所有回调
function RemoveObjectAllFunc( Self, EventName, Object )

    local Event = Self.mEventTable[ EventName ]

    if not Event then
        return
    end

    Event[Object] = nil

end

