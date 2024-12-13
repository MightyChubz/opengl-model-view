#include "model.hpp"
#include "camera.hpp"
#include "glm/ext/vector_float3.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include <utility>

Model::Model(Mesh &&mesh, Texture &&texture, Shader &&shader)
    : mesh(std::move(mesh)), texture(std::move(texture)), shader(std::move(shader))
{
}

void Model::rotate(const float angle, const glm::vec3 axis)
{
    model = glm::rotate(model, angle, axis);
}

void Model::render(const Camera &camera) const
{
    shader.use();
    shader.set("model", model);
    shader.set("view", camera.camera_view());
    shader.set("projection", camera.projection_mat());
    texture.use(0);
    mesh.render();
}
