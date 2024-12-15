#ifndef MESH_LOADER_HPP
#define MESH_LOADER_HPP

#include <string_view>
#include <vector>

#include "stddefs.hpp"
#include "vertex.hpp"

class MeshLoader
{
  public:
    struct MeshData {
        std::vector<Vertex> m_vertices;
        std::vector<u32>    m_indices;
    };

    MeshData LoadObj(const std::string_view path);
};

#endif
