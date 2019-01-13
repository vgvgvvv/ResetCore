#include "Object.h"

namespace ReGL
{
    uint64_t Object::current_uid_ = 0;

    bool Object::Destroy()
    {
        destroyed_ = true;
        return true;
    }
}
