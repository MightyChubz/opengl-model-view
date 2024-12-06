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
    std::shared_ptr<u32> vao;
    std::shared_ptr<u32> vbo;
    std::shared_ptr<u32> ebo;
    size_t               indice_size;

    static void destruct_attribute_array(u32 *vao)
    {
        glDeleteVertexArrays(1, vao);
    }

    static void destruct_buffer_array(u32 *bo)
    {
        glDeleteBuffers(1, bo);
    }

  public:
    Mesh() = default;
    Mesh(const MeshLoader::MeshData &data);
    void render() const;
};

#endif
