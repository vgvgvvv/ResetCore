//
// Created by ¶­å· on 2018/7/26.
//

#include "Application.hpp"
#include "IMGUI.hpp"

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
        if(!IMGUI::Init()){
            assert(false);
            return false;
        }
        return true;
    }

    bool Application::Run() {
        while(!main_window->ShouldQuit()){
            main_window->Clear();
            main_window->PullEvent();

            //TODO

            IMGUI::OnGUI();
            main_window->Swap();
        }
        return true;
    }

    bool Application::Quit() {
        if(!IMGUI::Uninit()){
            assert(false);
            return false;
        }
        if(!main_window->UnInit()){
            assert(false);
            return false;
        }
        return true;
    }
}