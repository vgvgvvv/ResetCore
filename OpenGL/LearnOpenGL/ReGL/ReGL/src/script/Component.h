#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include "Object.h"

namespace ReGL
{
    class GameObject;
    class Component : public Object
    {
    public:

        const GameObject& GetGameObject() { return *game_object_; }
    private:
        GameObject* game_object_{nullptr};
    };
}

#endif // __COMPONENT_H__
