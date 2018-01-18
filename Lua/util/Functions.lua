module("Common.Functions", package.seeall)
require "Table/ErrorTable"
--[[
    获取字符串的32位哈希值（与c#方法对应）
    times33算法（快速哈希算法）
    返回32位uint，取值范围为[0, (2^32-1)]
    number取值范围为[-2^53, +2^53]
]]
function GetHash32(str)
    if str == nil then
        return 0
    end

    str = tostring(str)
    local l_hash = 0
    local l_strLen = string.len(str)
    for i = 1, l_strLen do
        l_hash = Common.Bit32._lshift(l_hash, 5) + l_hash + string.byte(str, i)
        if l_hash > 4294967295--[[2^32-1]] then --超出了uint的范围
            l_hash = l_hash - 4294967295 - 1 --因为取值从0开始，所以最终值为：超出的部分-1
        end
    end

    return l_hash
end

--[[
    dump表结构
]]
function DumpTable(value, desciption, nesting)
    if type(nesting) ~= "number" then nesting = 3 end
    
    local lookupTable = {}
    local result = {}
    
    local function _v(v)
        if type(v) == "string" then
            v = "\"" .. v .. "\""  
        end
        return tostring(v)
    end
    
    local traceback = string.split(debug.traceback("", 2), "\n")
    print("dump from: " .. (traceback[3]))
    
    local function _dump(value, desciption, indent, nest, keylen)
        desciption = desciption or "<var>"
        spc = ""
        if type(keylen) == "number" then
            spc = string.rep(" ", keylen - string.len(_v(desciption)))
        end
        if type(value) ~= "table" then
            result[#result +1 ] = string.format("%s%s%s = %s", indent, _v(desciption), spc, _v(value))
        elseif lookupTable[value] then    
            result[#result +1 ] = string.format("%s%s%s = *REF*", indent, desciption, spc)
        else
            lookupTable[value] = true
            if nest > nesting then
                result[#result +1 ] = string.format("%s%s = *MAX NESTING*", indent, desciption)
            else
                result[#result +1 ] = string.format("%s%s = {", indent, _v(desciption))
                local indent2 = indent .. "    "
                local keys = {}
                local keylen = 0
                local values = {}
                for k, v in pairs(value) do
                    keys[#keys + 1] = k
                    local vk = _v(k) 
                    local vkl = string.len(vk)
                    if vkl > keylen then keylen = vkl end
                    values[k] = v
                end
                table.sort(keys, function(a, b)
                    if type(a) == "number" and type(b) == "number" then
                        return a < b
                    else
                        return tostring(a) < tostring(b)
                    end
                end) 
                for i, k in ipairs(keys) do
                    _dump(values[k], k, indent2, nest + 1, keylen)
                end
                result[#result +1] = string.format("%s}", indent)
            end
        end
    end
    _dump(value, desciption, "- ", 1)
    
    for i, line in ipairs(result) do
        print(line)
    end
end

function Split(str, delimiter)
    if str==nil or str=='' or delimiter==nil then
        return {}
    end

    local result = {}
    for match in (str..delimiter):gmatch("(.-)"..delimiter) do
        table.insert(result, match)
    end
    return result
end

function VectorToTable(vector)
    local result = {}
    local i = 1
    while i <= vector.Length do
        result[i] = vector[i - 1]
        i = i + 1
    end
    return result
end

function SequenceToTable(seq)
    local result = {}
    for i = 1, seq.Capacity do
        result[i] = seq:get_Item(i-1)
    end
    return result
end

function VectorSequenceToTable(info)
    local ret = {}
    for i = 1, info.Count do
        ret[i] = {}
        for j = 1, info.Capacity do
            ret[i][j] = info:get_Item(i-1, j-1)
        end
    end
    return ret
end

function TableToString(table)
    local str = ""
    for k,v in pairs(table) do
        str = str..tostring(k)..":"..tostring(v)..","
    end
    print(str)
end

ErrorList = nil
ErrorTable = nil
function GetErrorCodeStr(errorId)
    if ErrorList == nil then
        ErrorList = {}
        for i = 1, table.maxn(enum_pb.ERRORCODE.values) do
            ErrorList[enum_pb.ERRORCODE.values[i].number] = enum_pb.ERRORCODE.values[i].name
        end
    end
    if ErrorTable == nil then
        ErrorTable = {}
        local tableInfo = Table.ErrorTable.GetTable()
        for i = 1, table.maxn(tableInfo) do
            ErrorTable[tableInfo[i].Id()] = tableInfo[i].Value()
        end
    end
    local errorStr = ErrorList[errorId]
    if errorStr then
        return ErrorTable[errorStr] or tostring(errorId)
    else
        return tostring(errorId)
    end
end