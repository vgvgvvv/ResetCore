/**
 * GLFW相关函数
 */

#ifndef __GLFWMANAGER_H__
#define __GLFWMANAGER_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Window.h"

namespace ReGL
{
    class Window;
    class GLFWManager
    {
    public:
        static bool Init();
        static bool Update();
        static bool LateUpdate();
        static bool Uninit();

        static bool HandleInput();
        static bool PullEvents();

        static bool ShouldFinish();

        static Window& GetWindow() { return window_; }

    private:
        static Window window_;
    };
}


#endif //__GLFWMANAGER_H__