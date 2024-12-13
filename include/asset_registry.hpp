#ifndef ASSET_REGISTRY_HPP
#define ASSET_REGISTRY_HPP

#include <memory>
#include <string_view>
#include <unordered_map>

#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"

class AssetRegistry
{
    std::unordered_map<std::string_view, Shader>  shaders;
    std::unordered_map<std::string_view, Texture> textures;
    std::unordered_map<std::string_view, Mesh>    meshes;

  public:
    static AssetRegistry &get_instance()
    {
        static std::unique_ptr<AssetRegistry> registry{nullptr};
        if (registry == nullptr) {
            registry = std::make_unique<AssetRegistry>();
        }

        return *registry;
    }

    void    add_shader(const std::string_view name, Shader shader);
    void    add_texture(const std::string_view name, Texture texture);
    void    add_mesh(const std::string_view name, Mesh mesh);
    Shader  get_shader(const std::string_view name) const;
    Texture get_texture(const std::string_view name) const;
    Mesh    get_mesh(const std::string_view name) const;
};

#endif
