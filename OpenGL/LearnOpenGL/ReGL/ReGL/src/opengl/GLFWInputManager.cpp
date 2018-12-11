#include "GLFWInputManager.h"
#include "utility/Context.h"
#include "GLFWManager.h"


namespace ReGL
{
    bool GLFWInputManager::ProcessInput()
    {
        if (!GLFWManager::HandleInput())
        {
            Context::GetLogger().Error("GLFWManager::HandleInput Init Error!!");
            return false;
        }
        return true;
    }

    bool GLFWInputManager::PullInput()
    {
        if (!GLFWManager::PullEvents())
        {
            Context::GetLogger().Error("GLFWManager::PullEvents Init Error!!");
            return false;
        }
        return true;
    }
}
