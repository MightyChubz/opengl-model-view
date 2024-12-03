#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <memory>
#include <string_view>
#include <type_traits>

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stddefs.hpp"

class Shader
{
    static void          delete_program(u32 *data);
    u32                  read_shader(std::string_view filename, GLenum shader_type);
    std::shared_ptr<u32> id{nullptr, delete_program};

  public:
    Shader() = default;
    Shader(const std::string_view vertex_path, const std::string_view fragment_path);

    void                       use() const;

    template <typename T> void set(const std::string_view name, T value) const
    {
        if constexpr (std::is_same_v<T, bool>)
            glUniform1i(glGetUniformLocation(*id, name.data()), static_cast<int>(value));
        else if constexpr (std::is_same_v<T, int>)
            glUniform1i(glGetUniformLocation(*id, name.data()), value);
        else if constexpr (std::is_same_v<T, float>)
            glUniform1f(glGetUniformLocation(*id, name.data()), value);
        else if constexpr (std::is_same_v<T, glm::vec2>)
            glUniform2f(glGetUniformLocation(*id, name.data()), value.x, value.y);
        else if constexpr (std::is_same_v<T, glm::vec3>)
            glUniform3f(glGetUniformLocation(*id, name.data()), value.x, value.y, value.z);
        else if constexpr (std::is_same_v<T, glm::mat4>)
            glUniformMatrix4fv(glGetUniformLocation(*id, name.data()), 1, GL_FALSE, glm::value_ptr(value));
    }
};

#endif
