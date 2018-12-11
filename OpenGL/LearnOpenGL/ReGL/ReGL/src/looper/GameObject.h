#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__
#include <list>
#include "script/Transform.h"

namespace ReGL
{
    class Component;
    class GameObject : Object
    {
    public:
        friend class GameObjectManager;

        GameObject()
        {
            transform_ = new Transform();
        }

        ~GameObject()
        {
            delete transform_;
        }

        private:
            std::list<const Component*> components_;
            Transform* transform_;
    };

    class GameObjectManager
    {
    public:
        static GameObject& Create();
    private:
        static std::list<GameObject*> game_objects_;
    };
}


#endif
