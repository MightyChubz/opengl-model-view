#include <string_view>
#include <utility>

#include "asset_registry.hpp"

void AssetRegistry::add_shader(const std::string_view name, Shader shader)
{
    if (!shaders.count(name)) shaders[name] = std::move(shader);
}

void AssetRegistry::add_texture(const std::string_view name, Texture texture)
{
    if (!textures.count(name)) textures[name] = std::move(texture);
}

void AssetRegistry::add_mesh(const std::string_view name, Mesh mesh)
{
    if (!meshes.count(name)) meshes[name] = std::move(mesh);
}

Shader AssetRegistry::get_shader(const std::string_view name) const
{
    return shaders.at(name);
}

Texture AssetRegistry::get_texture(const std::string_view name) const
{
    return textures.at(name);
}

Mesh AssetRegistry::get_mesh(const std::string_view name) const
{
    return meshes.at(name);
}
