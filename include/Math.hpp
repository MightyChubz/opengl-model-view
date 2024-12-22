#ifndef MATH_HPP
#define MATH_HPP

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/trigonometric.hpp"

using Vec2    = glm::vec2;
using Vec3    = glm::vec3;
using Vec4    = glm::vec4;
using Matrix4 = glm::mat4;

constexpr Vec3 UP_AXIS       = Vec3(0.0, 1.0, 0.0);
constexpr Vec3 DOWN_AXIS     = Vec3(0.0, -1.0, 0.0);
constexpr Vec3 LEFT_AXIS     = Vec3(-1.0, 0.0, 0.0);
constexpr Vec3 RIGHT_AXIS    = Vec3(1.0, 0.0, 0.0);
constexpr Vec3 FORWARD_AXIS  = Vec3(0.0, 0.0, 1.0);
constexpr Vec3 BACKWARD_AXIS = Vec3(0.0, 0.0, -1.0);

constexpr Matrix4 Rotate(Matrix4 &mat, float eular, Vec3 axis)
{
    return glm::rotate(mat, glm::radians(eular), axis);
}

constexpr Matrix4 Translate(Matrix4 &mat, Vec3 delta)
{
    return glm::translate(mat, delta);
}

constexpr Matrix4 Scale(Matrix4 &mat, Vec3 scale)
{
    return glm::scale(mat, scale);
}

#endif
