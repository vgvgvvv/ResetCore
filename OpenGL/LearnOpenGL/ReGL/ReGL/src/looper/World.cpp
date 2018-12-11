#include "World.h"
#include "utility/Context.h"
#include "utility/logger/ConsoleLogger.h"
#include "InputManager.h"
#include "opengl/GLManager.h"

namespace ReGL
{
    bool World::running_ = false;

    bool World::Init()
    {
        if(!ContextValue<IRenderManager>::Get().Init())
        {
            Context::GetLogger().Error(" GLManager::Init with error !!!");
            return false;
        }
        running_ = true;
        return true;
    }

    bool World::PreUpdate()
    {
        if (!ContextValue<InputManager>::Get().ProcessInput())
        {
            return false;
        }
        return true;
    }

    bool World::Update()
    {
        if (!ContextValue<IRenderManager>::Get().WillRender())
        {
            return false;
        }
        if (!ContextValue<IRenderManager>::Get().Update())
        {
            return false;
        }
        return true;
    }

    bool World::LateUpdate()
    {
        if(!ContextValue<IRenderManager>::Get().LateUpdate())
        {
            return false;
        }
        if(!ContextValue<InputManager>::Get().PullInput())
        {
            return false;
        }

        return true;
    }


    bool World::Uninit()
    {
        running_ = false;
        if (!ContextValue<IRenderManager>::Get().Uninit())
        {
            Context::GetLogger().Error(" GLManager::Uninit with error !!!");
            return false;
        }
        return true;
    }

    bool World::Run(int argc, char** argv)
    {
        Context::Init();
        Context::GetLogger().Log("Start World!!");
        if(!Init())
        {
            Context::GetLogger().Error("World Init Error!!");
            return false;    
        }
        
        while(running_)
        {
            running_ = PreUpdate();
            if (!running_) return false;
            running_ = Update();
            if (!running_) return false;
            running_ = LateUpdate();
            if (!running_) return false;
        }

        if(!Uninit())
        {
            Context::GetLogger().Error("World Uninit Error!!");
            return false;
        }
        return true;
    }
}
