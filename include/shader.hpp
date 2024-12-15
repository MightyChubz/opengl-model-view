#ifndef SHADER_H
#define SHADER_H

#include <memory>
#include <string_view>

#include "material_system.hpp"
#include "stddefs.hpp"

class Shader
{
    std::shared_ptr<MaterialSystem> m_matRenderContext{nullptr};
    static void                     DeleteProgram(PROGRAM_HANDLE *handle);
    SHADER_HANDLE                   readShader(std::string_view filename, ShaderType shader_type);
    std::shared_ptr<PROGRAM_HANDLE> m_handle{nullptr, DeleteProgram};

  public:
    Shader() = default;
    Shader(const std::string_view vertex_path, const std::string_view fragment_path);

    void Use() const;

    template <typename T>
    void Set(const std::string_view name, T value) const
    {
        m_matRenderContext->SetUniformVariable(*m_handle, name, value);
    }
};

#endif
