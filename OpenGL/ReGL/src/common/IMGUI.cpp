//
// Created by ¶­å· on 2018/7/26.
//

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include "IMGUI.hpp"
#include "Application.hpp"

namespace ReGL{

    bool IMGUI::Init() {
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(Application::MainWindow()->RawWindow(), true);
        ImGui::StyleColorsDark();

        return true;
    }

    bool IMGUI::OnGUI() {
        ImGui_ImplGlfwGL3_NewFrame();


        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        return true;
    }

    bool IMGUI::Uninit() {
        // Cleanup
        ImGui_ImplGlfwGL3_Shutdown();
        ImGui::DestroyContext();
        return true;
    }

}