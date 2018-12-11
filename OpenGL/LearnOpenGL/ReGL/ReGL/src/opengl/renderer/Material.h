#ifndef __MATERIAL_H__
#define __MATERIAL_H__
#include "Shader.h"
#include "ShaderProgram.h"

namespace ReGL
{
    class Material
    {
    public:
        bool Init(const ShaderProgram& shader);
    private:
        ShaderProgram* shader_{ nullptr };
    };

}

#endif
