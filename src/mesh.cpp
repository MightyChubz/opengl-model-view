#include "mesh.hpp"

#include "material_system.hpp"
#include "mesh_loader.hpp"
#include "vertex.hpp"
#include <memory>

Mesh::Mesh(const MeshLoader::MeshData &data) : m_indiceSize(data.m_indices.size())
{
    MaterialSystem::GetContext(m_matRenderContext);
    VERTEX_ARRAY_HANDLE vao = m_matRenderContext->GenerateVertexArray();
    BUFFER_HANDLE       vbo = m_matRenderContext->GenerateBuffer();
    BUFFER_HANDLE       ebo = m_matRenderContext->GenerateBuffer();
    m_matRenderContext->BindVertexArray(vao);
    m_matRenderContext->BindBuffer(BufferType::ARRAY, vbo);
    m_matRenderContext->WriteStaticDataInBuffer(data.m_vertices);
    m_matRenderContext->BindBuffer(BufferType::ELEMENT_ARRAY, ebo);
    m_matRenderContext->WriteStaticDataInBuffer(data.m_indices);
    m_matRenderContext->SetAttributePointer(0, 3, sizeof(Vertex), nullptr);
    m_matRenderContext->SetAttributePointer(1,
                                            2,
                                            sizeof(Vertex),
                                            reinterpret_cast<void *>(offsetof(Vertex, m_texcoord)));

    m_buffers = std::make_shared<MeshBuffers>(vao, vbo, ebo);
}

void Mesh::Render() const
{
    m_matRenderContext->BindVertexArray(m_buffers->m_vao);
    m_matRenderContext->BindBuffer(BufferType::ELEMENT_ARRAY, m_buffers->m_ebo);
    m_matRenderContext->SetTargetIndiceSize(m_indiceSize);
    m_matRenderContext->DrawElements();
    m_matRenderContext->UnbindVertexArray();
    m_matRenderContext->UnbindBuffer();
}
