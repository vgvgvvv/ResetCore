//
// Created by ¶­å· on 2018/7/26.
//

#ifndef REGL_WINDOW_HPP
#define REGL_WINDOW_HPP

#include "glm/vec2.hpp"
#include "Common.hpp"
#include "Input.hpp"


using namespace glm;
using namespace std;

namespace ReGL{
    class Window {
    public:
        Window() = default;
        ~Window() = default;

    public:
        bool Init();
        bool ShouldQuit();
        void Swap();
        void PullEvent();

    public:
        vec2 size{800, 600};
        string title{"Window"};
    private:
        GLFWwindow* glWindow{nullptr};
    };
}




#endif //REGL_WINDOW_HPP
