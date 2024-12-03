#include "mesh.hpp"
#include "mesh_loader.hpp"
#include <string_view>

Mesh::Mesh(const std::string_view path)
{
    MeshLoader           loader;
    MeshLoader::MeshData data = loader.load_obj(path);

    u32                  vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(Vertex), data.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(u32), data.indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, texcoord)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->vao.reset(new u32(vao), destruct_attribute_array);
    this->vbo.reset(new u32(vbo), destruct_buffer_array);
    this->ebo.reset(new u32(ebo), destruct_buffer_array);
    indice_size = data.indices.size();
}

void Mesh::render() const
{
    glBindVertexArray(*vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
    glDrawElements(GL_TRIANGLES, indice_size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
