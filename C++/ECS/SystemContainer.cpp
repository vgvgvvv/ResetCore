//
// Created by 董宸 on 19/01/2018.
//

#include "SystemContainer.hpp"


namespace ResetCore{
    auto SystemContainer::Add(std::shared_ptr<ISystem> system) -> SystemContainer * {
        if(system != nullptr)
            return this;
        systems.push_back(system);
        return this;
    }

    void SystemContainer::Init() {
        for(const auto &system : systems){
            system->Init();
        }
    }

    void SystemContainer::Update() {
        for(const auto &system : systems){
            system->Update();
        }
    }

    void SystemContainer::FixedUpdate() {
        for(const auto &system : systems){
            system->FixedUpdate();
        }
    }

    void SystemContainer::UnInit() {
        for(const auto &system : systems){
            system->UnInit();
        }
    }
}