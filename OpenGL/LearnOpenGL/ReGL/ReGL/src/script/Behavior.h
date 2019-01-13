#ifndef __BEHAVIOR_H__
#define __BEHAVIOR_H__
#include "Component.h"

namespace ReGL
{
    class BehaviorManager;
    class Behavior : public Component
    {
    public:
        friend BehaviorManager;
        bool Init();
        virtual bool PreUpdate() = 0;
        virtual bool Update() = 0;
        virtual bool LateUpdate() = 0;
        bool Destroy() override;
    };
}

#endif
