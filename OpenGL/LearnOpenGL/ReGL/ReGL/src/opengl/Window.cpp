#include "Window.h"
#include "utility/Context.h"
#include <GLFW/glfw3.h>

namespace ReGL
{
    bool Window::Init()
    {
        window_ = glfwCreateWindow(default_width_, default_height_, default_title_.c_str(), nullptr, nullptr);
        if(window_ == nullptr)
        {
            Context::GetLogger().Error("Init Windows With Error!");
            return false;
        }
        glfwSetFramebufferSizeCallback(window_, Resize);
        return true;
    }

    bool Window::Update()
    {
        return true;
    }

    bool Window::LateUpdate()
    {
        
        glfwSwapBuffers(window_);
        return true;    
    }

    bool Window::Uninit()
    {
        return true;
    }


    void Window::Resize(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
}
