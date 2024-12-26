#ifndef MODEL_HPP
#define MODEL_HPP

#include "Camera.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Transform.hpp"

class Model final
{
    Transform m_transform;
    Mesh      m_mesh;
    Texture   m_texture;
    Shader    m_shader;

  public:
    Model(Mesh &&mesh, Texture &&texture, Shader &&shader);

    constexpr Transform &GetTransform()
    {
        return m_transform;
    }

    [[nodiscard]] constexpr const Transform &GetTransform() const
    {
        return m_transform;
    }

    [[nodiscard]] constexpr const Texture &GetTexture() const
    {
        return m_texture;
    }

    [[nodiscard]] constexpr const Mesh &GetMesh() const
    {
        return m_mesh;
    }

    [[nodiscard]] constexpr const Shader &GetShader() const
    {
        return m_shader;
    }

    void Render(const Camera &camera) const;
};

#endif
