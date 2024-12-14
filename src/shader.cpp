#include <filesystem>
#include <fstream>
#include <memory>
#include <string_view>

#include "SDL_log.h"
#include "material_system.hpp"
#include "shader.hpp"

void Shader::delete_program(PROGRAM_HANDLE *handle)
{
    if (handle == nullptr) return;
    std::shared_ptr<MaterialSystem> mat_render_context;
    MaterialSystem::get_context(mat_render_context);
    mat_render_context->delete_program(*handle);
    delete handle;
}

SHADER_HANDLE Shader::read_shader(std::string_view filename, ShaderType shader_type)
{
    std::filesystem::path file_path(filename);

    if (std::filesystem::exists(file_path)) {
        std::ifstream file(file_path);

        if (file.is_open()) {
            std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
            file.close();

            SDL_Log("Loaded shader: %s", filename.data());

            SHADER_HANDLE handle = mat_render_context->create_shader(shader_type);
            mat_render_context->set_shader_source(handle, content);
            mat_render_context->compile_shader(handle);

            if (!mat_render_context->shader_compile_status(handle)) {
                const std::array<char, 512> log = mat_render_context->get_shader_compile_error(handle);
                SDL_Log("Error compiling shader: %s", log.data());
            }

            SDL_Log("Compiled shader: %s", filename.data());

            return handle;
        }
    }

    return std::numeric_limits<u32>().max();
}

Shader::Shader(const std::string_view vertex_path, const std::string_view fragment_path)
{
    MaterialSystem::get_context(mat_render_context);
    SHADER_HANDLE vertex_shader   = read_shader(vertex_path, ShaderType::VERTEX);
    SHADER_HANDLE fragment_shader = read_shader(fragment_path, ShaderType::FRAGMENT);

    handle.reset(new PROGRAM_HANDLE(mat_render_context->create_program()));
    mat_render_context->attach_shader(*handle, vertex_shader);
    mat_render_context->attach_shader(*handle, fragment_shader);
    mat_render_context->link_program(*handle);

    if (!mat_render_context->program_link_status(*handle)) {
        const std::array<char, 512> log = mat_render_context->get_program_link_error(*handle);
        SDL_Log("Error linking shader program: %s", log.data());
    }

    mat_render_context->delete_shader(vertex_shader);
    mat_render_context->delete_shader(fragment_shader);
}

void Shader::use() const
{
    mat_render_context->use_program(*handle);
}
