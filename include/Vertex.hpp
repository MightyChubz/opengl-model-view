#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <functional>

#include "Math.hpp"

struct Vertex {
    Vec3 m_vertice;
    Vec2 m_texcoord;

    Vertex() = default;
    Vertex(Vec3 vertice, Vec2 texcoord) : m_vertice(vertice), m_texcoord(texcoord)
    {
    }

    bool operator==(const Vertex &rhs) const
    {
        return Vec3Equal(m_vertice, rhs.m_vertice) && Vec2Equal(m_texcoord, rhs.m_texcoord);
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
