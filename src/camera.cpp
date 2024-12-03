#include "camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

Camera::Camera(const int width, const int height)
{
    view       = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
    projection = glm::perspective(glm::radians(45.0f),
                                  static_cast<float>(width) / static_cast<float>(height),
                                  0.1f,
                                  100.0f);
}

void Camera::update()
{
    view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
}
