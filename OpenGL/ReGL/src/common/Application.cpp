//
// Created by ¶­å· on 2018/7/26.
//

#include "Application.hpp"

namespace ReGL{

    Application* Application::current;
    Window* Application::main_window;

    Application::Application() {
        assert(Application::current == nullptr);
        Application::current = this;
        Application::main_window = new Window();
    }

    Application::~Application() {
        delete Application::main_window;
    }

    bool Application::Init() {
        if(!main_window->Init()){
            assert(false);
            return false;
        }
        return true;
    }

    bool Application::Run() {
        while(!main_window->ShouldQuit()){
            main_window->PullEvent();

//            // Render
//            // Clear the colorbuffer
//            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//            glClear(GL_COLOR_BUFFER_BIT);

            main_window->Swap();
        }
        return true;
    }

    bool Application::Quit() {
        glfwTerminate();
        return true;
    }
}