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
        std::vector<Vertex> vertices;
        std::vector<u32>    indices;
    };

    MeshData load_obj(const std::string_view path);
};

#endif
