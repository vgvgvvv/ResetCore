#include "InputManager.h"
#include "utility/Context.h"
#include "opengl/GLFWManager.h"

namespace ReGL
{

    bool InputManager::ProcessInput()
    {
        if(!GLFWManager::HandleInput())
        {
            Context::GetLogger().Error("GLFWManager::HandleInput Init Error!!");
            return false;
        }
        return true;
    }

    bool InputManager::PullInput()
    {
        if(!GLFWManager::PullEvents())
        {
            Context::GetLogger().Error("GLFWManager::PullEvents Init Error!!");
            return false;
        }
        return true;
    }

}
