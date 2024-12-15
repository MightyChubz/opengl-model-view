#include <string_view>
#include <utility>

#include "AssetRegistry.hpp"

void AssetRegistry::AddShader(const std::string_view name, Shader shader)
{
    if (!m_shaders.contains(name)) m_shaders[name] = std::move(shader);
}

void AssetRegistry::AddTexture(const std::string_view name, Texture texture)
{
    if (!m_textures.contains(name)) m_textures[name] = std::move(texture);
}

void AssetRegistry::AddMesh(const std::string_view name, Mesh mesh)
{
    if (!m_meshes.contains(name)) m_meshes[name] = std::move(mesh);
}

Shader AssetRegistry::GetShader(const std::string_view name) const
{
    return m_shaders.at(name);
}

Texture AssetRegistry::GetTexture(const std::string_view name) const
{
    return m_textures.at(name);
}

Mesh AssetRegistry::GetMesh(const std::string_view name) const
{
    return m_meshes.at(name);
}
