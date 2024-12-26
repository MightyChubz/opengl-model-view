#include <utility>

#include "Camera.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

Model::Model(Mesh &&mesh, Texture &&texture, Shader &&shader)
    : m_mesh(std::move(mesh)), m_texture(std::move(texture)), m_shader(std::move(shader)),
      m_modelId(mesh.GetBuffers().m_ebo)
{
}

void Model::Render(const Camera &camera) const
{
    m_shader.Use();
    m_shader.Set("model", m_transform.GetModel());
    m_shader.Set("view", camera.CameraView());
    m_shader.Set("projection", camera.ProjectionMat());
    m_texture.Use(0);
    m_mesh.Render();
}
