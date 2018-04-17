
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