//
// Created by 董宸 on 2018/7/29.
//

#ifndef REGL_GAMEOBJECT_HPP
#define REGL_GAMEOBJECT_HPP

#include <map>
#include <vector>
#include "Object.h"
#include "Singleton.h"
#include "Component.h"

using namespace Utility;

namespace ReGL{

    class GameObject;

    /**
     * GameObject管理器
     */
    class GameObjectManager : public Singleton<GameObjectManager> {
    public:
        void AddGameObject(GameObject& go);
        void RemoveGameObject(GameObject &go);
        const bool GetGameObject(const uint64_t guid, GameObject* result) const;
    private:
        std::map<uint64_t, GameObject> game_object_map_;
    };

    /**
     * GameObject,使用Component进行组合以达成不同的目的
     */
    class GameObject : public Object {
    public:
        GameObject(){
            GameObjectManager::GetInstance().AddGameObject(*this);
        }

        GameObject(const GameObject &test) = delete;
        void operator=(const GameObject &test) = delete;

        void Destroy() override {
            GameObjectManager::GetInstance().RemoveGameObject(*this);
        }

    private:
        std::vector<Component> components_;
    };
}



#endif //REGL_GAMEOBJECT_HPP
