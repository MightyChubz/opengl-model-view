#ifndef MODEL_FACTORY_HPP
#define MODEL_FACTORY_HPP

#include <string_view>
#include <utility>

#include "asset_registry.hpp"
#include "model.hpp"

class ModelFactory
{
  public:
    static Model create_model(const std::string_view mesh_id,
                              const std::string_view texture_id,
                              const std::string_view shader_id)
    {
        const AssetRegistry &asset_registry = AssetRegistry::get_instance();
        Mesh                 mesh           = asset_registry.get_mesh(mesh_id);
        Texture              texture        = asset_registry.get_texture(texture_id);
        Shader               shader         = asset_registry.get_shader(shader_id);
        return {std::move(mesh), std::move(texture), std::move(shader)};
    }
};

#endif
