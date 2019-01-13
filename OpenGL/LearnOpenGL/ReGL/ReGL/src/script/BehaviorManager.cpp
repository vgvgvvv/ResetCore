#include "BehaviorManager.h"

namespace ReGL
{
    std::list<Behavior*> BehaviorManager::preupdate_behaviors_;
    std::list<Behavior*> BehaviorManager::update_behaviors_;
    std::list<Behavior*> BehaviorManager::lateupdate_behaviors_;

    bool BehaviorManager::RegistPreUpdate(Behavior* behavior)
    {
        if(behavior == nullptr)
        {
            return false;
        }
        preupdate_behaviors_.push_back(behavior);
        return true;
    }

    bool BehaviorManager::RegistUpdate(Behavior* behavior)
    {
        if (behavior == nullptr)
        {
            return false;
        }
        update_behaviors_.push_back(behavior);
        return true;
    }

    bool BehaviorManager::RegistLateUpdate(Behavior* behavior)
    {
        if (behavior == nullptr)
        {
            return false;
        }
        lateupdate_behaviors_.push_back(behavior);
        return true;
    }


    bool BehaviorManager::RemovePreUpdate(Behavior* behavior)
    {
        if(behavior == nullptr)
        {
            return false;
        }
        preupdate_behaviors_.remove(behavior);
        return true;
    }

    bool BehaviorManager::RemoveUpdate(Behavior* behavior)
    {
        if (behavior == nullptr)
        {
            return false;
        }
        update_behaviors_.remove(behavior);
        return true;
    }

    bool BehaviorManager::RemoveLateUpdate(Behavior* behavior)
    {
        if (behavior == nullptr)
        {
            return false;
        }
        lateupdate_behaviors_.remove(behavior);
        return true;
    }

    bool BehaviorManager::PreUpdate()
    {
        for (auto behavior : preupdate_behaviors_)
        {
            if(behavior->destroyed_ || !behavior->inited_)
            {
                continue;
            }
            if(!behavior->PreUpdate())
            {
                return false;
            }
        }
        return true;
    }

    bool BehaviorManager::Update()
    {
        for (auto behavior : update_behaviors_)
        {
            if (behavior->destroyed_ || !behavior->inited_)
            {
                continue;
            }
            if(!behavior->Update())
            {
                return false;
            }
        }
        return true;
    }

    bool BehaviorManager::LateUpdate()
    {
        for (auto behavior : lateupdate_behaviors_)
        {
            if (behavior->destroyed_ || !behavior->inited_)
            {
                continue;
            }
            if(!behavior->LateUpdate())
            {
                return false;
            }
        }
        return true;
    }
}
