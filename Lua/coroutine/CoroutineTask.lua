
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
