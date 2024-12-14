#ifndef MESH_HPP
#define MESH_HPP

#include <array>
#include <cstddef>
#include <memory>

#include "material_system.hpp"
#include "mesh_loader.hpp"
#include "stddefs.hpp"

class Mesh
{
    struct MeshBuffers {
        VERTEX_ARRAY_HANDLE vao;
        BUFFER_HANDLE       vbo;
        BUFFER_HANDLE       ebo;

        MeshBuffers(VERTEX_ARRAY_HANDLE vao, BUFFER_HANDLE vbo, BUFFER_HANDLE ebo) : vao(vao), vbo(vbo), ebo(ebo)
        {
        }

        ~MeshBuffers()
        {
            std::array<BUFFER_HANDLE, 2>    buffers = {vbo, ebo};
            std::shared_ptr<MaterialSystem> mat_render_context;
            MaterialSystem::get_context(mat_render_context);
            mat_render_context->delete_vertex_array(vao);
            mat_render_context->delete_buffers(buffers);
        }
    };

    std::shared_ptr<MaterialSystem> mat_render_context;
    std::shared_ptr<MeshBuffers>    buffers;
    size_t                          indice_size;

  public:
    Mesh() = default;
    Mesh(const MeshLoader::MeshData &data);
    void render() const;
};

#endif
