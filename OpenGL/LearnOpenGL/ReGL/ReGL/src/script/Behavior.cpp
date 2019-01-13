#include "Behavior.h"
#include "BehaviorManager.h"

namespace ReGL
{
    bool Behavior::Init()
    {
        BehaviorManager::RegistPreUpdate(this);
        BehaviorManager::RegistUpdate(this);
        BehaviorManager::RegistLateUpdate(this);
        return true;
    }

    bool Behavior::Destroy()
    {
        if (!Component::Destroy())
            return false;
        BehaviorManager::RemovePreUpdate(this);
        BehaviorManager::RemoveUpdate(this);
        BehaviorManager::RemoveLateUpdate(this);
        return true;
    }

}
