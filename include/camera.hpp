#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

class Camera
{
    glm::mat4 m_projection{glm::mat4(1.0)};
    glm::mat4 m_view{glm::mat4(1.0)};
    glm::vec2 m_rotation{0.0F, 0.0F};
    glm::vec3 m_cameraPosition{glm::vec3(0.0F, 0.0F, 3.0F)};
    glm::vec3 m_cameraFront{glm::vec3(0.0F, 0.0F, -1.0F)};
    glm::vec3 m_cameraUp{glm::vec3(0.0F, 1.0F, 0.0F)};

  public:
    Camera(const int width, const int height);

    [[nodiscard]] constexpr const glm::mat4 &ProjectionMat() const
    {
        return m_projection;
    }

    [[nodiscard]] constexpr const glm::mat4 &CameraView() const
    {
        return m_view;
    }

    constexpr glm::vec3 &Position()
    {
        return m_cameraPosition;
    }

    constexpr glm::vec3 &Front()
    {
        return m_cameraFront;
    }

    constexpr glm::vec2 &Angle()
    {
        return m_rotation;
    }

    [[nodiscard]] constexpr const glm::vec3 &Up() const
    {
        return m_cameraUp;
    }

    void Update();
};

#endif
