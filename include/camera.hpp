#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

class Camera
{
    glm::mat4 projection{glm::mat4(1.0)};
    glm::mat4 view{glm::mat4(1.0)};
    glm::vec2 rotation{0.0f, 0.0f};
    glm::vec3 camera_position{glm::vec3(0.0f, 0.0f, 3.0f)};
    glm::vec3 camera_front{glm::vec3(0.0f, 0.0f, -1.0f)};
    glm::vec3 camera_up{glm::vec3(0.0f, 1.0f, 0.0f)};

  public:
    Camera(const int width, const int height);

    constexpr const glm::mat4 &projection_mat() const
    {
        return projection;
    }

    constexpr const glm::mat4 &camera_view() const
    {
        return view;
    }

    constexpr glm::vec3 &position()
    {
        return camera_position;
    }

    constexpr glm::vec3 &front()
    {
        return camera_front;
    }

    constexpr glm::vec2 &angle()
    {
        return rotation;
    }

    constexpr const glm::vec3 &up() const
    {
        return camera_up;
    }

    void update();
};

#endif
