#include "World.h"
#include "utility/Context.h"
#include "utility/logger/ConsoleLogger.h"

namespace ReGL
{
    bool World::running_ = false;

    bool World::Init()
    {
        return true;
    }


    bool World::Update()
    {
        return true;
    }

    bool World::Uninit()
    {
        return true;
    }

    bool World::Run(int argc, char** argv)
    {
        Context::GetLogger().Log("Start World!!");
        if(!Init())
        {
            Context::GetLogger().Error("Init Error!!");
            return false;    
        }
        
        while(running_)
        {
            running_ = Update();
        }

        if(!Uninit())
        {
            Context::GetLogger().Error("Uninit Error!!");
            return false;
        }
        return true;
    }
}
