//
// Created by ¶­å· on 2018/7/26.
//

#include "Input.hpp"

namespace ReGL{
    void Input::DefaultFramebufferSizeCallback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void Input::DefaultKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }
}