#ifndef __SHADERPROGRAM_H__
#define __SHADERPROGRAM_H__
#include "Shader.h"
#include <string>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/matrix.hpp>

namespace ReGL
{
    class ShaderProgram
    {
    public:
        bool Init(const Shader* vertex_shader, const Shader* fragment_shader);
        bool Use() const;

        void setBool(const std::string &name, bool value) const
        {
            glUniform1i(glGetUniformLocation(program_id_, name.c_str()), (int)value);
        }
        void setInt(const std::string &name, int value) const
        {
            glUniform1i(glGetUniformLocation(program_id_, name.c_str()), value);
        }
        void setFloat(const std::string &name, float value) const
        {
            glUniform1f(glGetUniformLocation(program_id_, name.c_str()), value);
        }
        void setVec2(const std::string &name, const glm::vec2 &value) const
        {
            glUniform2fv(glGetUniformLocation(program_id_, name.c_str()), 1, &value[0]);
        }
        void setVec2(const std::string &name, float x, float y) const
        {
            glUniform2f(glGetUniformLocation(program_id_, name.c_str()), x, y);
        }
        void setVec3(const std::string &name, const glm::vec3 &value) const
        {
            glUniform3fv(glGetUniformLocation(program_id_, name.c_str()), 1, &value[0]);
        }
        void setVec3(const std::string &name, float x, float y, float z) const
        {
            glUniform3f(glGetUniformLocation(program_id_, name.c_str()), x, y, z);
        }
        void setVec4(const std::string &name, const glm::vec4 &value) const
        {
            glUniform4fv(glGetUniformLocation(program_id_, name.c_str()), 1, &value[0]);
        }
        void setVec4(const std::string &name, float x, float y, float z, float w)
        {
            glUniform4f(glGetUniformLocation(program_id_, name.c_str()), x, y, z, w);
        }
        void setMat2(const std::string &name, const glm::mat2 &mat) const
        {
            glUniformMatrix2fv(glGetUniformLocation(program_id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        void setMat3(const std::string &name, const glm::mat3 &mat) const
        {
            glUniformMatrix3fv(glGetUniformLocation(program_id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        void setMat4(const std::string &name, const glm::mat4 &mat) const
        {
            glUniformMatrix4fv(glGetUniformLocation(program_id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }

    private:
        const Shader* vertex_shader_{nullptr};
        const Shader* fragment_shader_{nullptr};
        uint32_t program_id_;
    };
}


#endif