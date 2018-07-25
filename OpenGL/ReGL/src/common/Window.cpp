//
// Created by 董宸 on 2018/7/26.
//

#include "Window.hpp"

namespace ReGL{

    bool Window::Init() {
        if( !glfwInit() )
        {
            fprintf( stderr, "Failed to initialize GLFW\n" );
            getchar();
            return false;
        }
        //基本设置
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac系统需要加上这一句
#endif
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        //创建窗口
        glWindow = glfwCreateWindow(static_cast<int>(size.x), static_cast<int>(size.y), title.c_str(), nullptr, nullptr);
        if( glWindow == nullptr){
            fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
            getchar();
            glfwTerminate();
        }
        glfwMakeContextCurrent(glWindow);

        //设置回调
        glfwSetKeyCallback(glWindow, Input::DefaultKeyCallback);

        glfwSetFramebufferSizeCallback(glWindow, Input::DefaultFramebufferSizeCallback);


        //
        glewExperimental = GL_TRUE;
        //初始化glew
        glewInit();

        //定义viewport
        int portWidth, portHeight;
        glfwGetFramebufferSize(glWindow, &portWidth, &portHeight);
        glViewport(0, 0, portWidth, portHeight);
        return true;
    }

    bool Window::ShouldQuit() {
        return static_cast<bool>(glfwWindowShouldClose(glWindow));
    }

    void Window::Swap() {
        glfwSwapBuffers(glWindow);
    }

    void Window::PullEvent() {
        glfwPollEvents();
    }
}