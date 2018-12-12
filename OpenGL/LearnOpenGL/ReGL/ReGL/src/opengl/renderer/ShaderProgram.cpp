#include "ShaderProgram.h"
#include "utility/Context.h"

namespace ReGL
{
    bool ShaderProgram::Init(const Shader* vertex_shader, const Shader* fragment_shader)
    {
        vertex_shader_ = vertex_shader;
        fragment_shader_ = fragment_shader;
        program_id_ = glCreateProgram();

        glAttachShader(program_id_, vertex_shader->id());
        glAttachShader(program_id_, fragment_shader->id());
        glLinkProgram(program_id_);

        int success;
        char info_log[512];
        glGetProgramiv(program_id_, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(program_id_, 512, nullptr, info_log);
            Context::GetLogger().Error(std::string(info_log));
            return false;
        }
        return true;
    }


    bool ShaderProgram::Use() const
    {
        glUseProgram(program_id_);
        return true;
    }
}
