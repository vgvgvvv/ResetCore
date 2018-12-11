#ifndef __SHADERPROGRAM_H__
#define __SHADERPROGRAM_H__
#include "Shader.h"

namespace ReGL
{
    class ShaderProgram
    {
    public:
        bool Init(const Shader* vertex_shader, const Shader* fragment_shader);
    private:
        const Shader* vertex_shader_{nullptr};
        const Shader* fragment_shader_{nullptr};
        uint32_t program_id_;
    };
}


#endif