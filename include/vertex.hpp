#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <functional>

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

struct Vertex {
    glm::vec3 m_vertice;
    glm::vec2 m_texcoord;

    Vertex() = default;
    Vertex(glm::vec3 vertice, glm::vec2 texcoord) : m_vertice(vertice), m_texcoord(texcoord)
    {
    }

    bool operator==(const Vertex &v) const
    {
        return m_vertice.x == v.m_vertice.x && m_vertice.y == v.m_vertice.y && m_vertice.z == v.m_vertice.z &&
               m_texcoord.x == v.m_texcoord.x && m_texcoord.y == v.m_texcoord.y;
    }
};

template <>
struct std::hash<Vertex> {
    size_t operator()(const Vertex &v) const noexcept
    {
        size_t h1 = std::hash<float>{}(v.m_vertice.x);
        size_t h2 = std::hash<float>{}(v.m_vertice.y);
        size_t h3 = std::hash<float>{}(v.m_vertice.z);
        size_t h4 = std::hash<float>{}(v.m_texcoord.x);
        size_t h5 = std::hash<float>{}(v.m_texcoord.y);
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4);
    }
};

#endif
