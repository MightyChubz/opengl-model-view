#ifndef MATH_HPP
#define MATH_HPP

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"

using Vec2     = glm::vec2;
using Vec3     = glm::vec3;
using BoolVec2 = glm::vec<2, bool>;
using BoolVec3 = glm::vec<3, bool>;
using Vec4     = glm::vec4;
using Matrix4  = glm::mat4;

constexpr Vec3 UP_AXIS       = Vec3(0.0, 1.0, 0.0);
constexpr Vec3 DOWN_AXIS     = Vec3(0.0, -1.0, 0.0);
constexpr Vec3 LEFT_AXIS     = Vec3(-1.0, 0.0, 0.0);
constexpr Vec3 RIGHT_AXIS    = Vec3(1.0, 0.0, 0.0);
constexpr Vec3 FORWARD_AXIS  = Vec3(0.0, 0.0, -1.0);
constexpr Vec3 BACKWARD_AXIS = Vec3(0.0, 0.0, 1.0);

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

constexpr float *ValuePtr(Matrix4 &mat)
{
    return glm::value_ptr(mat);
}

constexpr bool Vec3Equal(const Vec3 &a, const Vec3 &b)
{
    BoolVec3 result = glm::equal(a, b);
    return result == BoolVec3(true, true, true);
}

constexpr bool Vec2Equal(const Vec2 &a, const Vec2 &b)
{
    BoolVec2 result = glm::equal(a, b);
    return result == BoolVec2(true, true);
}

constexpr Matrix4 LookAt(const Vec3 &eye, const Vec3 &center)
{
    return glm::lookAt(eye, center, UP_AXIS);
}

constexpr float Radians(float eular)
{
    return glm::radians(eular);
}

constexpr Matrix4 Perspective(float fov, float aspect, float near, float far)
{
    return glm::perspective(Radians(fov), aspect, near, far);
}

constexpr Vec3 Cross(const Vec3 &a, const Vec3 &b)
{
    return glm::cross(a, b);
}

constexpr Vec3 Normalize(const Vec3 &vector)
{
    return glm::normalize(vector);
}

#endif
