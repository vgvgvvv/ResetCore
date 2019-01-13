#include "ScriptManager.h"
#include "GameObject.h"

namespace ReGL
{
    bool ScriptManager::Init()
    {
        auto go = GameObjectManager::Create();
        
        return true;
    }

    bool ScriptManager::PreUpdate()
    {
        return true;
    }

    bool ScriptManager::Update()
    {
        return true;
    }

    bool ScriptManager::LateUpdate()
    {
        return true;
    }

    bool ScriptManager::Uninit()
    {
        return true;
    }
}
