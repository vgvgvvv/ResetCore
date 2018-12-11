#ifndef __BEHAVIOR_H__
#define __BEHAVIOR_H__
#include "Component.h"

namespace ReGL
{
    class Behavior : public Component
    {
    public:
        virtual bool Awake() = 0;
        virtual bool Start() = 0;
        virtual bool Update() = 0;
        virtual bool LateUpdate() = 0;
        virtual bool OnDestroy() = 0;
    };
}

#endif
