#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <string>

#include "material_system.hpp"

TEXTURE_HANDLE MaterialSystem::generate_texture() const
{
    TEXTURE_HANDLE handle;
    glGenTextures(1, &handle);
    return handle;
}

void MaterialSystem::bind_texture(TEXTURE_HANDLE handle) const
{
    glBindTexture(GL_TEXTURE_2D, handle);
}

void MaterialSystem::set_texture_parameter(const TextureParameters parameter, const TextureSetValues value) const
{
    glTexParameteri(GL_TEXTURE_2D, static_cast<GLenum>(parameter), static_cast<GLenum>(value));
}

void MaterialSystem::write_texture_to_active(const int width, const int height, const void *buffer) const
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}

void MaterialSystem::generate_mipmaps() const
{
    glGenerateMipmap(GL_TEXTURE_2D);
}

void MaterialSystem::set_active_texture_slot(const size_t slot) const
{
    glActiveTexture(texture_slots[slot]);
}

void MaterialSystem::delete_texture(TEXTURE_HANDLE handle) const
{
    glDeleteTextures(1, &handle);
}

SHADER_HANDLE MaterialSystem::create_shader(const ShaderType shader_type) const
{
    return glCreateShader(static_cast<GLenum>(shader_type));
}

PROGRAM_HANDLE MaterialSystem::create_program() const
{
    return glCreateProgram();
}

void MaterialSystem::set_shader_source(SHADER_HANDLE handle, const std::string &content) const
{
    const char *content_ptr = content.c_str();
    glShaderSource(handle, 1, &content_ptr, nullptr);
}

void MaterialSystem::compile_shader(SHADER_HANDLE handle) const
{
    glCompileShader(handle);
}

bool MaterialSystem::shader_compile_status(SHADER_HANDLE handle) const
{
    i32 success;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    return success;
}

void MaterialSystem::attach_shader(PROGRAM_HANDLE handle, SHADER_HANDLE shader_handle) const
{
    glAttachShader(handle, shader_handle);
}

void MaterialSystem::link_program(PROGRAM_HANDLE handle) const
{
    glLinkProgram(handle);
}

bool MaterialSystem::program_link_status(PROGRAM_HANDLE handle) const
{
    i32 success;
    glGetProgramiv(handle, GL_LINK_STATUS, &success);
    return success;
}

const std::array<char, 512> MaterialSystem::get_shader_compile_error(SHADER_HANDLE handle) const
{
    std::array<char, 512> infoLog;
    std::ranges::fill(infoLog, 0);
    glGetShaderInfoLog(handle, infoLog.size(), nullptr, infoLog.data());
    return infoLog;
}

const std::array<char, 512> MaterialSystem::get_program_link_error(PROGRAM_HANDLE handle) const
{
    std::array<char, 512> infoLog;
    std::ranges::fill(infoLog, 0);
    glGetProgramInfoLog(handle, infoLog.size(), nullptr, infoLog.data());
    return infoLog;
}

void MaterialSystem::use_program(PROGRAM_HANDLE handle) const
{
    glUseProgram(handle);
}

void MaterialSystem::delete_shader(SHADER_HANDLE handle) const
{
    glDeleteShader(handle);
}

void MaterialSystem::delete_program(PROGRAM_HANDLE handle) const
{
    glDeleteProgram(handle);
}

BUFFER_HANDLE MaterialSystem::generate_buffer() const
{
    BUFFER_HANDLE handle;
    glGenBuffers(1, &handle);
    return handle;
}

VERTEX_ARRAY_HANDLE MaterialSystem::generate_vertex_array() const
{
    VERTEX_ARRAY_HANDLE handle;
    glGenVertexArrays(1, &handle);
    return handle;
}

void MaterialSystem::bind_vertex_array(VERTEX_ARRAY_HANDLE handle) const
{
    glBindVertexArray(handle);
}

void MaterialSystem::bind_buffer(BufferType buffer_type, BUFFER_HANDLE handle) const
{
    glBindBuffer(static_cast<GLenum>(buffer_type), handle);
}

void MaterialSystem::unbind_vertex_array() const
{
    bind_vertex_array(0);
}

void MaterialSystem::unbind_buffer(BufferType buffer_type) const
{
    bind_buffer(buffer_type, 0);
}

void MaterialSystem::set_attribute_pointer(u32 index, i32 count, u32 size, void *offset) const
{
    glVertexAttribPointer(index, count, GL_FLOAT, GL_FALSE, size, offset);
    glEnableVertexAttribArray(index);
}

void MaterialSystem::draw_elements(u32 size) const
{
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
}

void MaterialSystem::delete_buffer(BUFFER_HANDLE handle) const
{
    glDeleteBuffers(1, &handle);
}

void MaterialSystem::delete_vertex_array(VERTEX_ARRAY_HANDLE handle) const
{
    glDeleteVertexArrays(1, &handle);
}

void MaterialSystem::enable_depth_test() const
{
    glEnable(GL_DEPTH_TEST);
}

void MaterialSystem::set_viewport(i32 x, i32 y, i32 width, i32 height) const
{
    glViewport(x, y, width, height);
}

void MaterialSystem::clear_with_color(f32 r, f32 g, f32 b) const
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MaterialSystem::toggle_debug_wireframe() const
{
    static bool render_wireframe = false;
    render_wireframe             = !render_wireframe;
    if (render_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
