#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "Component.h"

namespace ReGL
{
    class Transform : public Component
    {
    public:
        glm::vec3 position_{ 0, 0, 0 };
        glm::vec3 scale_{ 1, 1, 1 };
        glm::vec3 euler_{ 0, 0, 0 };
    };
}

#endif // __TRANSFORM_H__
