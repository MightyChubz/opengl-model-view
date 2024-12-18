#ifndef MESH_HPP
#define MESH_HPP

#include <array>
#include <cstddef>
#include <memory>

#include "MaterialSystem.hpp"
#include "MeshLoader.hpp"
#include "StdDefs.hpp"

class Mesh
{
    struct MeshBuffers {
        VERTEX_ARRAY_HANDLE m_vao;
        BUFFER_HANDLE       m_vbo;
        BUFFER_HANDLE       m_ebo;

        MeshBuffers(VERTEX_ARRAY_HANDLE vao, BUFFER_HANDLE vbo, BUFFER_HANDLE ebo) : m_vao(vao), m_vbo(vbo), m_ebo(ebo)
        {
        }

        MeshBuffers(const MeshBuffers &)            = delete;
        MeshBuffers(MeshBuffers &&)                 = delete;
        MeshBuffers &operator=(const MeshBuffers &) = delete;
        MeshBuffers &operator=(MeshBuffers &&)      = delete;

        ~MeshBuffers()
        {
            std::array<BUFFER_HANDLE, 2>    buffers = {m_vbo, m_ebo};
            std::shared_ptr<MaterialSystem> matRenderContext;
            MaterialSystem::GetContext(matRenderContext);
            matRenderContext->DeleteVertexArray(m_vao);
            matRenderContext->DeleteBuffers(buffers);
        }
    };

    std::shared_ptr<MaterialSystem> m_matRenderContext;
    std::shared_ptr<MeshBuffers>    m_buffers;
    size_t                          m_indiceSize{};

  public:
    Mesh() = default;
    Mesh(const MeshLoader::MeshData &data);

    void Render() const;
};

#endif
