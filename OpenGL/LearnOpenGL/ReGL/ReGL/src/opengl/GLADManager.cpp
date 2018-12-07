#include "GLADManager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utility/Context.h"

namespace ReGL
{
    bool GLADManager::Init()
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Context::GetLogger().Error(" gladLoadGLLoader with error !!!");
            return false;
        }
        return true;
    }

    bool GLADManager::Uninit()
    {
        return true;
    }
}
