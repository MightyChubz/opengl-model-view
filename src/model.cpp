#include "model.hpp"
#include "camera.hpp"
#include "glm/ext/vector_float3.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include <utility>

Model::Model(Mesh &&mesh, Texture &&texture, Shader &&shader)
    : m_mesh(std::move(mesh)), m_texture(std::move(texture)), m_shader(std::move(shader))
{
}

void Model::Rotate(const float angle, const glm::vec3 axis)
{
    m_model = glm::rotate(m_model, angle, axis);
}

void Model::Render(const Camera &camera) const
{
    m_shader.Use();
    m_shader.Set("model", m_model);
    m_shader.Set("view", camera.CameraView());
    m_shader.Set("projection", camera.ProjectionMat());
    m_texture.Use(0);
    m_mesh.Render();
}
