#ifndef ASSET_REGISTRY_HPP
#define ASSET_REGISTRY_HPP

#include <string_view>
#include <unordered_map>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

class AssetRegistry final
{
    std::unordered_map<std::string_view, Shader>  m_shaders;
    std::unordered_map<std::string_view, Texture> m_textures;
    std::unordered_map<std::string_view, Mesh>    m_meshes;

  public:
    static AssetRegistry &GetInstance();

    void    AddShader(const std::string_view name, Shader shader);
    void    AddTexture(const std::string_view name, Texture texture);
    void    AddMesh(const std::string_view name, Mesh mesh);
    Shader  GetShader(const std::string_view name) const;
    Texture GetTexture(const std::string_view name) const;
    Mesh    GetMesh(const std::string_view name) const;
};

#endif
