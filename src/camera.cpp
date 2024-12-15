#include "camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

Camera::Camera(const int width, const int height)
{
    m_view       = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp);
    m_projection = glm::perspective(glm::radians(45.0F),
                                    static_cast<float>(width) / static_cast<float>(height),
                                    0.1F,
                                    100.0F);
}

void Camera::Update()
{
    m_view = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp);
}
