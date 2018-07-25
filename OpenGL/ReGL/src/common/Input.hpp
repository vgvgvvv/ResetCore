//
// Created by ¶­å· on 2018/7/26.
//

#ifndef REGL_INPUT_HPP
#define REGL_INPUT_HPP

#include "Common.hpp"

namespace ReGL{
    class Input {
    public:
        static void DefaultFramebufferSizeCallback(GLFWwindow *window, int width, int height);
        static void DefaultKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
    };
}


#endif //REGL_INPUT_HPP
