#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <string>
#include "GLFWManager.h"

namespace ReGL
{
    class Window
    {
    public:
        bool Init();
        bool Update();
        bool LateUpdate();
        bool Uninit();
        GLFWwindow* Raw() { return window_; }
        static void Resize(GLFWwindow* window, int width, int height);
    private:
        GLFWwindow* window_{nullptr};
        const int default_height_{ 600 };
        const int default_width_{ 800 };
        const std::string default_title_{"unknown"};
    };
}

#endif //__WINDOW_H__