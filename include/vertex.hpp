#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <functional>

#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

struct Vertex {
    glm::vec3 vertice;
    glm::vec2 texcoord;

    Vertex() = default;
    Vertex(glm::vec3 vertice, glm::vec2 texcoord) : vertice(vertice), texcoord(texcoord)
    {
    }

    bool operator==(const Vertex &v) const
    {
        return vertice.x == v.vertice.x && vertice.y == v.vertice.y && vertice.z == v.vertice.z &&
               texcoord.x == v.texcoord.x && texcoord.y == v.texcoord.y;
    }
};

template <>
struct std::hash<Vertex> {
    size_t operator()(const Vertex &v) const noexcept
    {
        size_t h1 = std::hash<float>{}(v.vertice.x);
        size_t h2 = std::hash<float>{}(v.vertice.y);
        size_t h3 = std::hash<float>{}(v.vertice.z);
        size_t h4 = std::hash<float>{}(v.texcoord.x);
        size_t h5 = std::hash<float>{}(v.texcoord.y);
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4);
    }
};

#endif
