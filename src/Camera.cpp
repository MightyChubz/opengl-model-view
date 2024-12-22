#include "Camera.hpp"
#include "Math.hpp"

Camera::Camera(const int width, const int height)
{
    m_view       = LookAt(m_transform.GetPosition(), m_transform.GetPosition() + m_front);
    m_projection = Perspective(45.0F, static_cast<float>(width) / static_cast<float>(height), 0.1F, 100.0F);
}

void Camera::Update()
{
    m_view = LookAt(m_transform.GetPosition(), m_transform.GetPosition() + m_front);
}
