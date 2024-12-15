#include <filesystem>
#include <fstream>
#include <memory>
#include <string_view>

#include "SDL_log.h"
#include "material_system.hpp"
#include "shader.hpp"

void Shader::DeleteProgram(PROGRAM_HANDLE *handle)
{
    if (handle == nullptr) return;
    std::shared_ptr<MaterialSystem> matRenderContext;
    MaterialSystem::GetContext(matRenderContext);
    matRenderContext->DeleteProgram(*handle);
    delete handle;
}

SHADER_HANDLE Shader::readShader(std::string_view filename, ShaderType shader_type)
{
    std::filesystem::path filePath(filename);

    if (std::filesystem::exists(filePath)) {
        std::ifstream file(filePath);

        if (file.is_open()) {
            std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
            file.close();

            SDL_Log("Loaded shader: %s", filename.data());

            SHADER_HANDLE handle = m_matRenderContext->CreateShader(shader_type);
            m_matRenderContext->WriteShaderSource(handle, content);
            m_matRenderContext->CompileShader(handle);

            if (!m_matRenderContext->ShaderCompileStatus(handle)) {
                const std::array<char, 512> log = m_matRenderContext->GetShaderCompileError(handle);
                SDL_Log("Error compiling shader: %s", log.data());
            }

            SDL_Log("Compiled shader: %s", filename.data());

            return handle;
        }
    }

    return std::numeric_limits<unsigned int>::max();
}

Shader::Shader(const std::string_view vertex_path, const std::string_view fragment_path)
{
    MaterialSystem::GetContext(m_matRenderContext);
    SHADER_HANDLE vertexShader   = readShader(vertex_path, ShaderType::VERTEX);
    SHADER_HANDLE fragmentShader = readShader(fragment_path, ShaderType::FRAGMENT);

    m_handle = std::make_shared<PROGRAM_HANDLE>(m_matRenderContext->CreateProgram());
    m_matRenderContext->AttachShader(*m_handle, vertexShader);
    m_matRenderContext->AttachShader(*m_handle, fragmentShader);
    m_matRenderContext->LinkProgram(*m_handle);

    if (!m_matRenderContext->ProgramLinkStatus(*m_handle)) {
        const std::array<char, 512> log = m_matRenderContext->GetProgramLinkError(*m_handle);
        SDL_Log("Error linking shader program: %s", log.data());
    }

    m_matRenderContext->DeleteShader(vertexShader);
    m_matRenderContext->DeleteShader(fragmentShader);
}

void Shader::Use() const
{
    m_matRenderContext->UseProgram(*m_handle);
}
