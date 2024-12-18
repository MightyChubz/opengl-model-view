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

    bool operator==(const Vertex &rhs) const
    {
        return m_vertice.x == rhs.m_vertice.x && m_vertice.y == rhs.m_vertice.y && m_vertice.z == rhs.m_vertice.z &&
               m_texcoord.x == rhs.m_texcoord.x && m_texcoord.y == rhs.m_texcoord.y;
    }
};

template <>
struct std::hash<Vertex> {
    size_t operator()(const Vertex &vert) const noexcept
    {
        size_t hash1 = std::hash<float>{}(vert.m_vertice.x);
        size_t hash2 = std::hash<float>{}(vert.m_vertice.y);
        size_t hash3 = std::hash<float>{}(vert.m_vertice.z);
        size_t hash4 = std::hash<float>{}(vert.m_texcoord.x);
        size_t hash5 = std::hash<float>{}(vert.m_texcoord.y);
        return hash1 ^ (hash2 << 1) ^ (hash3 << 2) ^ (hash4 << 3) ^ (hash5 << 4);
    }
};

#endif
