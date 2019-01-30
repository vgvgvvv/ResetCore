# 判断是否为Nan的方法

判断数字是否为Nan只需要如下：

```lua
local num = 100/0
if num ~= num then
    print("num is Nan")
end

```