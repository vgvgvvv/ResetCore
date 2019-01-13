#include "GLFWManager.h"


namespace ReGL
{
    Window GLFWManager::window_;

    bool GLFWManager::Init()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        if(!window_.Init())
        {
            glfwTerminate();
        }

#if __APPLE__
        //mac需要这个宏
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        glfwMakeContextCurrent(window_.Raw());
        return true;
    }

    bool GLFWManager::Update()
    {
        return true;
    }

    bool GLFWManager::LateUpdate()
    {
        window_.LateUpdate();
       
        glfwPollEvents();
        return true;
    }


    bool GLFWManager::Uninit()
    {
        glfwTerminate();
        return true;
    }


    bool GLFWManager::ShouldFinish()
    {
        return glfwWindowShouldClose(window_.Raw());
    }

    bool GLFWManager::HandleInput()
    {
        if (glfwGetKey(window_.Raw(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window_.Raw(), true);
        return true;
    }

    bool GLFWManager::PullEvents()
    {
        glfwPollEvents();
        return true;
    }
  
}
