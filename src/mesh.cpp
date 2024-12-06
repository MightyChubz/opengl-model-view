#include "mesh.hpp"
#include "mesh_loader.hpp"

Mesh::Mesh(const MeshLoader::MeshData &data)
{
    u32 vao, vbo, ebo;
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

    buffers.reset(new MeshBuffers(vao, vbo, ebo));
    indice_size = data.indices.size();
}

void Mesh::render() const
{
    glBindVertexArray(buffers->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->ebo);
    glDrawElements(GL_TRIANGLES, indice_size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
