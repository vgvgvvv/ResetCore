local HU = require "luahotupdate"
HU.Init("hotupdatelist", 
    { "E:\\Projects\\ResetCore\\Lua\\lua_hotupdate-master" }) --please replace the second parameter with you src path

function sleep(t)
    local now_time = os.clock()
    while true do
        if os.clock() - now_time > t then
            HU.Update()
            return
        end
    end
end











--[[***************************************
*******************************************]]
local test = require "test"
print("start runing")
while true do
    test.func()
    sleep(3)
end
--***************************************
--***************************************



