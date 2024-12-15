#ifndef MODEL_HPP
#define MODEL_HPP

#include "Camera.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"

class Model
{
    glm::mat4 m_model{glm::mat4(1.0)};
    Mesh      m_mesh;
    Texture   m_texture;
    Shader    m_shader;

  public:
    Model(Mesh &&mesh, Texture &&texture, Shader &&shader);

    void Rotate(const float angle, const glm::vec3 axis);

    void Render(const Camera &camera) const;
};

#endif
