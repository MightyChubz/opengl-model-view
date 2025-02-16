#ifndef MATERIAL_SYSTEM_HPP
#define MATERIAL_SYSTEM_HPP

// This needs to be included first before gl.h and glext.h
#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <array>
#include <cstddef>
#include <memory>
#include <ranges>
#include <string>
#include <string_view>

#include "Math.hpp"
#include "StdDefs.hpp"

template <typename T>
concept SizedContiguousRange = std::ranges::contiguous_range<T> && std::ranges::sized_range<T>;

using TEXTURE_HANDLE      = u32;
using SHADER_HANDLE       = u32;
using PROGRAM_HANDLE      = u32;
using BUFFER_HANDLE       = u32;
using VERTEX_ARRAY_HANDLE = u32;

enum class TextureParameters {
    WRAP_S          = GL_TEXTURE_WRAP_S,
    WRAP_T          = GL_TEXTURE_WRAP_T,
    MIN_FILTER      = GL_TEXTURE_MIN_FILTER,
    MAG_FILTER      = GL_TEXTURE_MAG_FILTER,
    MAX_ANISOTROPHY = GL_TEXTURE_MAX_ANISOTROPY_EXT
};

enum class TextureSetValues {
    // GL_TEXTURE_MIN/MAX_FILTER set parameters
    FILTER_LINEAR                 = GL_LINEAR,
    FILTER_LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR,
    FILTER_LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
    FILTER_NEAREST                = GL_NEAREST,
    FILTER_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    FILTER_NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,

    // WRAP set parameters
    WRAP_REPEAT               = GL_REPEAT,
    WRAP_CLAMP_TO_EDGE        = GL_CLAMP_TO_EDGE,
    WRAP_CLAMP_TO_BORDER      = GL_CLAMP_TO_BORDER,
    WRAP_MIRROR_REPEAT        = GL_MIRRORED_REPEAT,
    WRAP_MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,

    // MAX_ANISOTROPHY set parameters
    ANISOTROPHY_X2  = 2,
    ANISOTROPHY_X4  = 4,
    ANISOTROPHY_X8  = 8,
    ANISOTROPHY_X16 = 16,
};

enum class ShaderType { FRAGMENT = GL_FRAGMENT_SHADER, VERTEX = GL_VERTEX_SHADER };

enum class BufferType { ARRAY = GL_ARRAY_BUFFER, ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER, NONE = -1 };

class MaterialSystem final
{
    static constexpr std::array TEXTURE_SLOTS = {
        GL_TEXTURE0,  GL_TEXTURE1,  GL_TEXTURE2,  GL_TEXTURE3,  GL_TEXTURE4,  GL_TEXTURE5,  GL_TEXTURE6,  GL_TEXTURE7,
        GL_TEXTURE8,  GL_TEXTURE9,  GL_TEXTURE10, GL_TEXTURE11, GL_TEXTURE12, GL_TEXTURE13, GL_TEXTURE14, GL_TEXTURE15,
        GL_TEXTURE16, GL_TEXTURE17, GL_TEXTURE18, GL_TEXTURE19, GL_TEXTURE20, GL_TEXTURE21, GL_TEXTURE22, GL_TEXTURE23,
        GL_TEXTURE24, GL_TEXTURE25, GL_TEXTURE26, GL_TEXTURE27, GL_TEXTURE28, GL_TEXTURE29, GL_TEXTURE30, GL_TEXTURE31,
    };

    BufferType m_boundBufferType{BufferType::NONE};
    u32        m_targetIndiceSize{};

  public:
    static void GetContext(std::shared_ptr<MaterialSystem> &ptr);

    /*
     * ================================
     * Texture functions
     * ================================
     */
    [[nodiscard]] TEXTURE_HANDLE GenerateTexture() const;

    void BindTexture(TEXTURE_HANDLE handle) const;
    void SetTextureParameter(const TextureParameters parameter, const TextureSetValues value) const;
    void WriteTextureToActive(const int width, const int height, const void *buffer) const;
    void GenerateMipmaps() const;
    void SetActiveTextureSlot(const size_t slot) const;
    void DeleteTexture(TEXTURE_HANDLE handle) const;

    /*
     * ================================
     * Shader functions
     * ================================
     */
    [[nodiscard]] SHADER_HANDLE          CreateShader(const ShaderType shader_type) const;
    [[nodiscard]] PROGRAM_HANDLE         CreateProgram() const;
    void                                 WriteShaderSource(SHADER_HANDLE handle, const std::string &src) const;
    void                                 CompileShader(SHADER_HANDLE handle) const;
    [[nodiscard]] bool                   ShaderCompileStatus(SHADER_HANDLE handle) const;
    void                                 AttachShader(PROGRAM_HANDLE handle, SHADER_HANDLE shader_handle) const;
    void                                 LinkProgram(PROGRAM_HANDLE handle) const;
    [[nodiscard]] bool                   ProgramLinkStatus(PROGRAM_HANDLE handle) const;
    [[nodiscard]] const std::string_view GetShaderCompileError(SHADER_HANDLE handle) const;
    [[nodiscard]] const std::string_view GetProgramLinkError(PROGRAM_HANDLE handle) const;
    void                                 UseProgram(PROGRAM_HANDLE handle) const;
    void                                 DeleteShader(SHADER_HANDLE handle) const;
    void                                 DeleteProgram(PROGRAM_HANDLE handle) const;

    template <typename T>
    void SetUniformVariable(PROGRAM_HANDLE handle, const std::string_view name, T value) const
    {
        if constexpr (std::is_same_v<T, bool>)
            glUniform1i(glGetUniformLocation(handle, name.data()), static_cast<int>(value));
        else if constexpr (std::is_same_v<T, int>)
            glUniform1i(glGetUniformLocation(handle, name.data()), value);
        else if constexpr (std::is_same_v<T, float>)
            glUniform1f(glGetUniformLocation(handle, name.data()), value);
        else if constexpr (std::is_same_v<T, Vec2>)
            glUniform2f(glGetUniformLocation(handle, name.data()), value.x, value.y);
        else if constexpr (std::is_same_v<T, Vec3>)
            glUniform3f(glGetUniformLocation(handle, name.data()), value.x, value.y, value.z);
        else if constexpr (std::is_same_v<T, Matrix4>)
            glUniformMatrix4fv(glGetUniformLocation(handle, name.data()), 1, GL_FALSE, ValuePtr(value));
    }

    /*
     * ================================
     * Buffer Functions
     * ================================
     */
    [[nodiscard]] BUFFER_HANDLE       GenerateBuffer() const;
    [[nodiscard]] VERTEX_ARRAY_HANDLE GenerateVertexArray() const;
    void                              BindVertexArray(VERTEX_ARRAY_HANDLE handle) const;
    void                              BindBuffer(BufferType buffer_type, BUFFER_HANDLE handle);
    void                              UnbindVertexArray() const;
    void                              UnbindBuffer() const;
    void                              SetAttributePointer(u32 index, i32 count, u32 size, void *offset) const;
    void                              SetTargetIndiceSize(u32 size);
    void                              DrawElements() const;
    void                              DrawElementsInstanced(const size_t count) const;
    void                              DeleteBuffer(BUFFER_HANDLE handle) const;
    void                              DeleteVertexArray(VERTEX_ARRAY_HANDLE handle) const;

    template <SizedContiguousRange RANGE>
    void WriteStaticDataInBuffer(RANGE range) const
    {
        if (m_boundBufferType == BufferType::NONE) return;
        glBufferData(static_cast<GLenum>(m_boundBufferType),
                     std::ranges::size(range) * sizeof((*std::ranges::begin(range))),
                     &(*std::ranges::begin(range)),
                     GL_STATIC_DRAW);
    }

    template <SizedContiguousRange RANGE>
    void DeleteBuffers(RANGE range)
    {
        glDeleteBuffers(range.size(), &(*range.begin()));
    }

    /*
     * ================================
     * Screen Functions
     * ================================
     */
    void EnableDepthTest() const;
    void SetViewport(i32 x, i32 y, i32 width, i32 height) const;
    void ClearWithColor(f32 r, f32 g, f32 b) const;
    void ToggleDebugWireframe() const;
    void ConfigureErrorHandling() const;
};

#endif
