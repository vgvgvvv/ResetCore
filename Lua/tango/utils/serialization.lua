local tinsert = table.insert
local tconcat = table.concat
local tremove = table.remove
local smatch = string.match
local sgmatch = string.gmatch
local sgsub = string.gsub
local ipairs = ipairs
local pairs = pairs
local type = type
local tostring = tostring
local tonumber = tonumber
local loadstring = loadstring
local print = print

--- The default tango serialization module.
-- Uses table serialization from http://lua/users.org/wiki/TableUtils and loadstring for unserialize.
module('tango.utils.serialization')

serialize = nil

local converters = {
  string = function(v)
             v = sgsub(v,"\n","\\n")
             if smatch(sgsub(v,"[^'\"]",""),'^"+$') then
               return "'"..v.."'"
             end
             return '"'..sgsub(v,'"','\\"')..'"'             
           end,
  table = function(v)
            return serialize(v)
          end,
  number = function(v)
             return tostring(v)
           end,
  boolean = function(v)
           return tostring(v)
         end  
}

local valtostr = 
  function(v)
    local conv = converters[type(v)]
    if conv then
      return conv(v)
    else
      return 'nil'
    end
  end

local keytostr = 
  function(k)
    if 'string' == type(k) and smatch(k,"^[_%a][_%a%d]*$") then
      return k
    else
      return '['..valtostr(k)..']'
    end
  end

serialize = 
  function(tbl)
    local result,done = {},{}
    for k,v in ipairs(tbl) do
      tinsert(result,valtostr(v))
      done[k] = true
    end
    for k,v in pairs(tbl) do
      if not done[k] then
        tinsert(result,keytostr(k)..'='..valtostr(v))
      end
    end
    return '{'..tconcat(result,',')..'}'
  end

unserialize = 
  function(strtab)
    return loadstring('return '..strtab)()
  end

return {
  serialize = serialize,
  unserialize = unserialize
}
