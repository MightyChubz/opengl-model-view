#ifndef MODEL_FACTORY_HPP
#define MODEL_FACTORY_HPP

#include <string_view>
#include <utility>

#include "asset_registry.hpp"
#include "model.hpp"

class ModelFactory
{
  public:
    static Model CreateModel(const std::string_view mesh_id,
                             const std::string_view texture_id,
                             const std::string_view shader_id)
    {
        const AssetRegistry &assetRegistry = AssetRegistry::GetInstance();
        Mesh                 mesh          = assetRegistry.GetMesh(mesh_id);
        Texture              texture       = assetRegistry.GetTexture(texture_id);
        Shader               shader        = assetRegistry.GetShader(shader_id);
        return {std::move(mesh), std::move(texture), std::move(shader)};
    }
};

#endif
