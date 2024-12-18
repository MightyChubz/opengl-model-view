#ifndef MESH_LOADER_HPP
#define MESH_LOADER_HPP

#include <string_view>
#include <vector>

#include "StdDefs.hpp"
#include "Vertex.hpp"

class MeshLoader final
{
  public:
    struct MeshData {
        std::vector<Vertex> m_vertices;
        std::vector<u32>    m_indices;
    };

    static MeshData LoadObj(const std::string_view path);
};

#endif
