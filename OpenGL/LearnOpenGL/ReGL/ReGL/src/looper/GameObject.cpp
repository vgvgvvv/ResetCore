#include "GameObject.h"
#include "utility/memory/Memory.h"

namespace ReGL
{
    std::list<GameObject*> GameObjectManager::game_objects_;

    GameObject& GameObjectManager::Create()
    {
        auto* go = NEW_OBJECT(GameObject);
        game_objects_.push_back(go);
        return *go;
    }


    bool GameObjectManager::Destroy(GameObject* game_object)
    {
        if(game_object == nullptr)
        {
            return false;
        }
        game_objects_.remove(game_object);
        return true;
    }

    bool GameObject::Destroy()
    {
        for (auto* component : components_)
        {
            component->Destroy();
        }
        components_.clear();
        GameObjectManager::Destroy(this);
        return true;
    }
}
