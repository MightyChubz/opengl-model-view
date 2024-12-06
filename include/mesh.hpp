#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>
#include <cstddef>
#include <memory>

#include "mesh_loader.hpp"
#include "stddefs.hpp"
#include "texture.hpp"

class Mesh
{
    struct MeshBuffers {
        u32 vao;
        u32 vbo;
        u32 ebo;

        MeshBuffers(u32 vao, u32 vbo, u32 ebo) : vao(vao), vbo(vbo), ebo(ebo)
        {
        }

        ~MeshBuffers()
        {
            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &ebo);
        }
    };

    std::shared_ptr<MeshBuffers> buffers;
    size_t                       indice_size;

  public:
    Mesh() = default;
    Mesh(const MeshLoader::MeshData &data);
    void render() const;
};

#endif
