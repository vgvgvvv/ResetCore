#include "Component.h"

namespace ReGL
{
    bool Component::Destroy()
    {
        if(!Object::Destroy())
            return false;
        game_object_ = nullptr;
        return true;
    }
}
