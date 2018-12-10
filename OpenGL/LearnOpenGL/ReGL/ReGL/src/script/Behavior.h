#ifndef __BEHAVIOR_H__
#define __BEHAVIOR_H__

namespace ReGL
{
    class Behavior
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
