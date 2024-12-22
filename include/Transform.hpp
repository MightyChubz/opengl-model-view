#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Math.hpp"
#include "glm/trigonometric.hpp"

class Transform
{
    Vec3 m_position;
    Vec3 m_rotation;
    Vec3 m_scale;

  public:
    Transform() : m_position(0), m_rotation(0), m_scale(1)
    {
    }

    constexpr Vec3 &GetPosition()
    {
        return m_position;
    }

    constexpr Vec3 &GetRotation()
    {
        return m_rotation;
    }

    constexpr Vec3 &GetScale()
    {
        return m_scale;
    }

    constexpr void SetPosition(Vec3 position)
    {
        m_position = position;
    }

    constexpr void SetRotation(Vec3 rotation)
    {
        m_rotation = rotation;
    }

    constexpr void SetScale(Vec3 scale)
    {
        m_scale = scale;
    }

    [[nodiscard]] constexpr Matrix4 GetModel() const
    {
        Matrix4 model = Matrix4(1.0);
        model         = Translate(model, m_position);
        model         = Rotate(model, m_rotation.x, RIGHT_AXIS);
        model         = Rotate(model, m_rotation.y, UP_AXIS);
        model         = Rotate(model, m_rotation.z, FORWARD_AXIS);
        model         = Scale(model, m_scale);
        return model;
    }
};

#endif
