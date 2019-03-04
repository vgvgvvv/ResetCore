
function test(s)
    print(s)
end

local mt = getmetatable("")
mt["__index"]["print"] = test

local l = "asd"
print(l:print())

local s = "asd"
print(s:len())