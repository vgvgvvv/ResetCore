#ifndef __BEHAVIOEMANAGER_H__
#define __BEHAVIOEMANAGER_H__
#include <list>
#include "Behavior.h"

namespace ReGL
{
    class BehaviorManager
    {
    public:
        static bool PreUpdate();
        static bool Update();
        static bool LateUpdate();

        static bool RegistPreUpdate(Behavior* behavior);
        static bool RegistUpdate(Behavior* behavior);
        static bool RegistLateUpdate(Behavior* behavior);

        static bool RemovePreUpdate(Behavior* behavior);
        static bool RemoveUpdate(Behavior* behavior);
        static bool RemoveLateUpdate(Behavior* behavior);

    private:
        static std::list<Behavior*> preupdate_behaviors_;
        static std::list<Behavior*> update_behaviors_;
        static std::list<Behavior*> lateupdate_behaviors_;
    };
}


#endif
