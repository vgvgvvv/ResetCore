//
// Created by ¶­å· on 2018/7/29.
//

#include "GameObject.h"

namespace ReGL{

    void GameObjectManager::AddGameObject(GameObject &go) {
        game_object_map_.emplace(go.GUID(), go);
    }

    void GameObjectManager::RemoveGameObject(GameObject &go) {
        game_object_map_.erase(go.GUID());
    }

    const bool GameObjectManager::GetGameObject(const uint64_t guid, GameObject* result) const {
        auto res = game_object_map_.find(guid);
        if(res != game_object_map_.end()){
            result = res->second;
            return true;
        }else{
            result = nullptr;
            return false;
        }
    }

}