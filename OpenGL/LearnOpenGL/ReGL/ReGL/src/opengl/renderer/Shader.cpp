#include "Shader.h"
#include "utility/Context.h"

namespace ReGL
{
    bool Shader::Init(const char* source) const
    {
        glShaderSource(shader_id_, 1, &source, NULL);
        glCompileShader(shader_id_);

        int success;
        char infoLog[512];
        glGetShaderiv(shader_id_, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader_id_, 512, NULL, infoLog);
            Context::GetLogger().Error(std::string(infoLog));
            return false;
        }
        return true;
    }


    bool Shader::Uninit()
    {
        glDeleteShader(shader_id_);
        shader_id_ = 0;
        return true;
    }
}
