#include "GameObject.h"

namespace ReGL
{
    std::list<GameObject*> GameObjectManager::game_objects_;

    GameObject& GameObjectManager::Create()
    {
        auto* go = new GameObject();
        game_objects_.push_back(go);
        return *go;
    }
}
