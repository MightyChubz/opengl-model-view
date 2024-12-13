#ifndef MODEL_HPP
#define MODEL_HPP

#include "camera.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Model
{
    glm::mat4 model{glm::mat4(1.0)};
    Mesh      mesh;
    Texture   texture;
    Shader    shader;

  public:
    Model(Mesh &&mesh, Texture &&texture, Shader &&shader);

    void rotate(const float angle, const glm::vec3 axis);

    void render(const Camera &camera) const;
};

#endif
