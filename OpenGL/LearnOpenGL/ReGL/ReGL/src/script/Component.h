#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include "Object.h"

namespace ReGL
{
    class GameObject;
    class Component : public Object
    {
    public:

        GameObject& GetGameObject() const { return *game_object_; }
        bool Destroy() override;
    protected:
        bool inited_;
    private:
        GameObject* game_object_{nullptr};
    };
}

#endif // __COMPONENT_H__
