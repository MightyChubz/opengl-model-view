#include "mesh.hpp"
#include "material_system.hpp"
#include "mesh_loader.hpp"
#include "vertex.hpp"

Mesh::Mesh(const MeshLoader::MeshData &data)
{
    MaterialSystem::get_context(mat_render_context);
    VERTEX_ARRAY_HANDLE vao = mat_render_context->generate_vertex_array();
    BUFFER_HANDLE       vbo = mat_render_context->generate_buffer();
    BUFFER_HANDLE       ebo = mat_render_context->generate_buffer();
    mat_render_context->bind_vertex_array(vao);
    mat_render_context->bind_buffer(BufferType::ARRAY, vbo);
    mat_render_context->write_buffer_static_data(BufferType::ARRAY, data.vertices);
    mat_render_context->bind_buffer(BufferType::ELEMENT_ARRAY, ebo);
    mat_render_context->write_buffer_static_data(BufferType::ELEMENT_ARRAY, data.indices);
    mat_render_context->set_attribute_pointer(0, 3, sizeof(Vertex), static_cast<void *>(0));
    mat_render_context->set_attribute_pointer(1,
                                              2,
                                              sizeof(Vertex),
                                              reinterpret_cast<void *>(offsetof(Vertex, texcoord)));

    buffers.reset(new MeshBuffers(vao, vbo, ebo));
    indice_size = data.indices.size();
}

void Mesh::render() const
{
    mat_render_context->bind_vertex_array(buffers->vao);
    mat_render_context->bind_buffer(BufferType::ELEMENT_ARRAY, buffers->ebo);
    mat_render_context->draw_elements(indice_size);
    mat_render_context->unbind_vertex_array();
    mat_render_context->unbind_buffer(BufferType::ELEMENT_ARRAY);
}
