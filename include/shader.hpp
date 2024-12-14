#ifndef SHADER_H
#define SHADER_H

#include <memory>
#include <string_view>

#include "material_system.hpp"
#include "stddefs.hpp"

class Shader
{
    std::shared_ptr<MaterialSystem> mat_render_context{nullptr};
    static void                     delete_program(PROGRAM_HANDLE *handle);
    SHADER_HANDLE                   read_shader(std::string_view filename, ShaderType shader_type);
    std::shared_ptr<PROGRAM_HANDLE> handle{nullptr, delete_program};

  public:
    Shader() = default;
    Shader(const std::string_view vertex_path, const std::string_view fragment_path);

    void use() const;

    template <typename T>
    void set(const std::string_view name, T value) const
    {
        mat_render_context->set_uniform_variable(*handle, name, value);
    }
};

#endif
