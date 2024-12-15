#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <string>

#include "material_system.hpp"

TEXTURE_HANDLE MaterialSystem::GenerateTexture() const
{
    TEXTURE_HANDLE handle = 0;
    glGenTextures(1, &handle);
    return handle;
}

void MaterialSystem::BindTexture(TEXTURE_HANDLE handle) const
{
    glBindTexture(GL_TEXTURE_2D, handle);
}

void MaterialSystem::SetTextureParameter(const TextureParameters parameter, const TextureSetValues value) const
{
    glTexParameteri(GL_TEXTURE_2D, static_cast<GLenum>(parameter), static_cast<GLint>(value));
}

void MaterialSystem::WriteTextureToActive(const int width, const int height, const void *buffer) const
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}

void MaterialSystem::GenerateMipmaps() const
{
    glGenerateMipmap(GL_TEXTURE_2D);
}

void MaterialSystem::SetActiveTextureSlot(const size_t slot) const
{
    glActiveTexture(TEXTURE_SLOTS[slot]);
}

void MaterialSystem::DeleteTexture(TEXTURE_HANDLE handle) const
{
    glDeleteTextures(1, &handle);
}

SHADER_HANDLE MaterialSystem::CreateShader(const ShaderType shader_type) const
{
    return glCreateShader(static_cast<GLenum>(shader_type));
}

PROGRAM_HANDLE MaterialSystem::CreateProgram() const
{
    return glCreateProgram();
}

void MaterialSystem::WriteShaderSource(SHADER_HANDLE handle, const std::string &content) const
{
    const char *content_ptr = content.c_str();
    glShaderSource(handle, 1, &content_ptr, nullptr);
}

void MaterialSystem::CompileShader(SHADER_HANDLE handle) const
{
    glCompileShader(handle);
}

bool MaterialSystem::ShaderCompileStatus(SHADER_HANDLE handle) const
{
    i32 success = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    return success;
}

void MaterialSystem::AttachShader(PROGRAM_HANDLE handle, SHADER_HANDLE shader_handle) const
{
    glAttachShader(handle, shader_handle);
}

void MaterialSystem::LinkProgram(PROGRAM_HANDLE handle) const
{
    glLinkProgram(handle);
}

bool MaterialSystem::ProgramLinkStatus(PROGRAM_HANDLE handle) const
{
    i32 success = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, &success);
    return success;
}

const std::array<char, 512> MaterialSystem::GetShaderCompileError(SHADER_HANDLE handle) const
{
    std::array<char, 512> infoLog{};
    std::ranges::fill(infoLog, 0);
    glGetShaderInfoLog(handle, infoLog.size(), nullptr, infoLog.data());
    return infoLog;
}

const std::array<char, 512> MaterialSystem::GetProgramLinkError(PROGRAM_HANDLE handle) const
{
    std::array<char, 512> infoLog{};
    std::ranges::fill(infoLog, 0);
    glGetProgramInfoLog(handle, infoLog.size(), nullptr, infoLog.data());
    return infoLog;
}

void MaterialSystem::UseProgram(PROGRAM_HANDLE handle) const
{
    glUseProgram(handle);
}

void MaterialSystem::DeleteShader(SHADER_HANDLE handle) const
{
    glDeleteShader(handle);
}

void MaterialSystem::DeleteProgram(PROGRAM_HANDLE handle) const
{
    glDeleteProgram(handle);
}

BUFFER_HANDLE MaterialSystem::GenerateBuffer() const
{
    BUFFER_HANDLE handle = 0;
    glGenBuffers(1, &handle);
    return handle;
}

VERTEX_ARRAY_HANDLE MaterialSystem::GenerateVertexArray() const
{
    VERTEX_ARRAY_HANDLE handle = 0;
    glGenVertexArrays(1, &handle);
    return handle;
}

void MaterialSystem::BindVertexArray(VERTEX_ARRAY_HANDLE handle) const
{
    glBindVertexArray(handle);
}

void MaterialSystem::BindBuffer(BufferType buffer_type, BUFFER_HANDLE handle) const
{
    glBindBuffer(static_cast<GLenum>(buffer_type), handle);
}

void MaterialSystem::UnbindVertexArray() const
{
    BindVertexArray(0);
}

void MaterialSystem::UnbindBuffer(BufferType buffer_type) const
{
    BindBuffer(buffer_type, 0);
}

void MaterialSystem::SetAttributePointer(u32 index, i32 count, u32 size, void *offset) const
{
    glVertexAttribPointer(index, count, GL_FLOAT, GL_FALSE, static_cast<i32>(size), offset);
    glEnableVertexAttribArray(index);
}

void MaterialSystem::DrawElements(u32 size) const
{
    glDrawElements(GL_TRIANGLES, static_cast<i32>(size), GL_UNSIGNED_INT, nullptr);
}

void MaterialSystem::DeleteBuffer(BUFFER_HANDLE handle) const
{
    glDeleteBuffers(1, &handle);
}

void MaterialSystem::DeleteVertexArray(VERTEX_ARRAY_HANDLE handle) const
{
    glDeleteVertexArrays(1, &handle);
}

void MaterialSystem::EnableDepthTest() const
{
    glEnable(GL_DEPTH_TEST);
}

void MaterialSystem::SetViewport(i32 x, i32 y, i32 width, i32 height) const
{
    glViewport(x, y, width, height);
}

void MaterialSystem::ClearWithColor(f32 r, f32 g, f32 b) const
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MaterialSystem::ToggleDebugWireframe() const
{
    static bool render_wireframe = false;
    render_wireframe             = !render_wireframe;
    if (render_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
