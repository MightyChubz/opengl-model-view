#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <cstddef>
#include <memory>
#include <string_view>

#include "SDL_surface.h"
#include "material_system.hpp"
#include "stddefs.hpp"

class Texture final
{
    std::shared_ptr<MaterialSystem> mat_render_context{nullptr};
    std::shared_ptr<TEXTURE_HANDLE> texture{nullptr};

    static void destruct_texture(TEXTURE_HANDLE *texture)
    {
        if (texture == nullptr) return;
        std::shared_ptr<MaterialSystem> mat_render_context;
        MaterialSystem::get_context(mat_render_context);
        mat_render_context->delete_texture(*texture);
        delete texture;
    }

    void flip_surface(SDL_Surface *surface, bool horizontal, bool vertical);

  public:
    Texture() = default;
    Texture(const std::string_view path);
    ~Texture() = default;

    void use(size_t slot) const;
};

#endif
