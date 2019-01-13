#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__
#include <list>
#include "script/Transform.h"
#include "utility/Common.h"

namespace ReGL
{
    class Component;
    class GameObject : public Object
    {
    public:
        friend class GameObjectManager;

        bool Destroy() override;

        const Transform& GetTransform() const { return transform_; }

        template <typename T>
        const T& AddComponent()
        {
            auto id = typeid(T);
            
        }

        private:
            std::list<Component*> components_;
            Transform transform_;
    };

    class GameObjectManager
    {
    public:
        static GameObject& Create();
        static bool Destroy(GameObject* game_object);
    private:
        static std::list<GameObject*> game_objects_;
    };
}


#endif
