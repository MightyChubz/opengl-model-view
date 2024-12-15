#include "mesh_loader.hpp"
#include "SDL_log.h"
#include "rapidobj.hpp"
#include <string_view>
#include <utility>

MeshLoader::MeshData MeshLoader::LoadObj(const std::string_view path)
{
    rapidobj::Result data = rapidobj::ParseFile(path);

    SDL_Log("Loaded model: %s", path.data());
    SDL_Log("Total indices: %zu", data.shapes.front().mesh.indices.size());
    SDL_Log("Total positions: %zu", data.attributes.positions.size());
    SDL_Log("Total texcoords: %zu", data.attributes.texcoords.size());

    i32                             indexOffset = 0;
    std::vector<u32>                indices;
    std::vector<Vertex>             vertices;
    std::unordered_map<Vertex, u32> verts;
    for (const auto &faces : data.shapes.front().mesh.num_face_vertices) {
        for (int i = 0; i < faces; ++i) {
            rapidobj::Index idx = data.shapes.front().mesh.indices[indexOffset + i];
            Vertex          vertex{};
            vertex.m_vertice.x  = data.attributes.positions[idx.position_index * 3];
            vertex.m_vertice.y  = data.attributes.positions[(idx.position_index * 3) + 1];
            vertex.m_vertice.z  = data.attributes.positions[(idx.position_index * 3) + 2];
            vertex.m_texcoord.x = data.attributes.texcoords[idx.texcoord_index * 2];
            vertex.m_texcoord.y = data.attributes.texcoords[(idx.texcoord_index * 2) + 1];

            if (!verts.contains(vertex)) {
                vertices.push_back(vertex);
                verts[vertex] = vertices.size() - 1;
            }

            indices.push_back(verts[vertex]);
        }

        indexOffset += faces;
    }

    return {.m_vertices = std::move(vertices), .m_indices = std::move(indices)};
}
