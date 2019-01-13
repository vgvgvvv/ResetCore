#ifndef __SHADER_H__
#define __SHADER_H__
#include <cstdint>
#include <glad/glad.h>

namespace ReGL
{
    class Shader
    {
    public:
        enum ShaderType
        {
            VERTEX = GL_VERTEX_SHADER,
            FRAGMENT = GL_FRAGMENT_SHADER
        };
        Shader(const ShaderType type)
        {
            shader_id_ = glCreateShader(type);
        }
        bool Init(const char* source) const;
        bool Uninit();

        uint32_t id() const { return shader_id_; }
    private:
        uint32_t shader_id_{0};
    };
}

#endif
