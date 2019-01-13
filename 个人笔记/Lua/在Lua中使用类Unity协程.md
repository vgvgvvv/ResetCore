# 在Lua中使用类Unity协程
#日常学习/Lua

Unity中的协程是通过Update驱动的，通过yield return来打断，在声明周期中回调执行，也就是resume。

在Lua里面其实只需要我们自己进行yield然后我们自己在update里面resume就和Unity的协程一毛一样了。不过Lua的协程好像不支持像Unity一样的中断：yield break……只能自己注意别在协程里写无限循环就好。

不过还没有看过效率怎么样……

协程管理部分
```
require "coroutine/CoroutineTask"
module("coroutine")


local l_coroutineTaskList = {}

timeDelta = 0.1

--开启协程
function StartCoroutine(func, hash)
    local l_task = CoroutineTask.new(coroutine.create(func, hash))
    l_coroutineTaskList[l_task.hash] = l_task
    return l_task
end

--自动对协程进行Resume（从外部调用）
function Update()
    --遍历任务列表
    for _,task in pairs(l_coroutineTaskList) do
        --没有暂停
        if task.pause ~= true then
            --是否结束
            local l_hasNext = coroutine.resume(task.cor)
            if l_hasNext == false then
                --回调
                task:callback()
                --移除
                l_coroutineTaskList[task.hash] = nil
            end
        end
    end
end

--等待时间
function WaitForSeconds(time)

    local currentTime = 0
    while currentTime < time do
        currentTime = currentTime + timeDelta
        coroutine.yield()
    end
end

--等待条件
function WaitForFunc(func)
    
    while not func() do
        coroutine.yield()
    end

end
```

协程实例部分
```

require "util/class"
module("coroutine")

CoroutineTask = class("CoroutineTask")

function CoroutineTask:ctor(cor, hash)
    --协程
    self.cor = cor
    self.hash = hash or cor
    self.pause = false
    self.callback = nil
end

function CoroutineTask:Pause()
    self.pause = true
end

function CoroutineTask:Resume()
    self.pause = false
end

```

测试代码
```
function test()

    local i = 0;
    while i < 100 do
        coroutine.yield()
        print(i)
    end
    WaitForSeconds(3)
    print("finish")

end

StartCoroutine(test)

```