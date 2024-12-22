#ifndef MODEL_HPP
#define MODEL_HPP

#include "Camera.hpp"
#include "Math.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Transform.hpp"
#include "glm/detail/qualifier.hpp"

class Model final
{
    Transform m_transform;
    Mesh      m_mesh;
    Texture   m_texture;
    Shader    m_shader;

  public:
    Model(Mesh &&mesh, Texture &&texture, Shader &&shader);

    constexpr void Rotate(const float angle, const Vec3 axis)
    {
        if (glm::equal(axis, UP_AXIS) == glm::vec<3, bool>(true, true, true))
            m_transform.GetRotation().y += angle;
        else if (glm::equal(axis, RIGHT_AXIS) == glm::vec<3, bool>(true, true, true))
            m_transform.GetRotation().x += angle;
        else if (glm::equal(axis, FORWARD_AXIS) == glm::vec<3, bool>(true, true, true))
            m_transform.GetRotation().z += angle;
    }

    void Render(const Camera &camera) const;
};

#endif
