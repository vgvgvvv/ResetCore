//
// Created by 董宸 on 2018/7/26.
//

/**
 * 主程序入口
 */

#include "Application.hpp"
#include "IMGUI.hpp"
#include "LogEngine.h"
#include "BaseRenderer.h"

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
            LogEngine::Error("Main Window init Error");
            assert(false);
            return false;
        }
        if(!IMGUI::Init()){
            LogEngine::Error("IMGUI init Error");
            assert(false);
            return false;
        }
        if(!RendererManager::GetInstance().Init()){
            LogEngine::Error("Renderer init Error");
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
            RendererManager::GetInstance().Render();
            IMGUI::OnGUI();
            main_window->Swap();
        }
        return true;
    }

    bool Application::Quit() {
        if(!RendererManager::GetInstance().UnInit()){
            LogEngine::Error("Renderer uninit Error");
            assert(false);
            return false;
        }
        if(!IMGUI::Uninit()){
            LogEngine::Error("imgui uninit Error");
            assert(false);
            return false;
        }
        if(!main_window->UnInit()){
            LogEngine::Error("Main Window uninit Error");
            assert(false);
            return false;
        }
        return true;
    }
}