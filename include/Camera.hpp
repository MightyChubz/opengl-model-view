#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Math.hpp"
#include "Transform.hpp"

class Camera final
{
    Matrix4   m_projection{Matrix4(1.0)};
    Matrix4   m_view{Matrix4(1.0)};
    Transform m_transform;
    Vec3      m_front{FORWARD_AXIS};

  public:
    Camera(const int width, const int height);

    [[nodiscard]] constexpr const Matrix4 &ProjectionMat() const
    {
        return m_projection;
    }

    [[nodiscard]] constexpr const Matrix4 &CameraView() const
    {
        return m_view;
    }

    constexpr Transform &GetTransform()
    {
        return m_transform;
    }

    constexpr Vec3 &GetPosition()
    {
        return m_transform.GetPosition();
    }

    constexpr Vec3 &GetRotation()
    {
        return m_transform.GetRotation();
    }

    constexpr Vec3 &Front()
    {
        return m_front;
    }

    void Update();
};

#endif
